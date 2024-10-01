#include "Renderer/sprite2d.h"

#include "Components/rendering.h"

#define PostProcessing 0
namespace ChronoShift
{

    void UpdateTransformationMatrix(FlexECS::Entity& currEntity, const Matrix4x4 parent_entity_matrix = Matrix4x4::Identity)
    {
        auto& local_transform = currEntity.GetComponent<Transform>()->transform;
        if (!currEntity.GetComponent<Transform>()->is_dirty) return;
       
        auto& local_position = currEntity.GetComponent<Position>()->position;
        auto& local_scale = currEntity.GetComponent<Scale>()->scale;
        // Get rotation component if it exists
        Rotation* local_rotation = nullptr;
        if (currEntity.TryGetComponent<Rotation>(local_rotation))
            local_rotation = currEntity.GetComponent<Rotation>();

        //Alignment of sprite
        static Vector2 sprite_alignment = Vector2::Zero;
        switch (currEntity.GetComponent<Sprite>()->alignment)
        {
        case Renderer2DProps::Alignment::Alignment_TopLeft: sprite_alignment = Vector2(local_scale.x * 0.5f, local_scale.y * 0.5f); break;
        case Renderer2DProps::Alignment::Alignment_TopRight: sprite_alignment = Vector2(-local_scale.x * 0.5f, local_scale.y * 0.5f); break;
        case Renderer2DProps::Alignment::Alignment_BottomLeft: sprite_alignment = Vector2(local_scale.x * 0.5f, -local_scale.y * 0.5f); break;
        case Renderer2DProps::Alignment::Alignment_BottomRight: sprite_alignment = Vector2(-local_scale.x * 0.5f, -local_scale.y * 0.5f); break;
        default: case Renderer2DProps::Alignment::Alignment_Center: sprite_alignment = Vector2::Zero; break;
        }

        // calculate the transform
        Matrix4x4 translation_matrix = Matrix4x4::Translate(Matrix4x4::Identity, Vector3(-(local_position.x + sprite_alignment.x), local_position.y + sprite_alignment.y, 0.0f));
        Matrix4x4 rotation_matrix = Quaternion::FromEulerAnglesDeg(local_rotation != nullptr ? local_rotation->rotation : Vector3::Zero).ToRotationMatrix();
        Matrix4x4 scale_matrix = Matrix4x4::Scale(Matrix4x4::Identity, local_scale);

        // Apply parent entity's matrix
       local_transform = parent_entity_matrix * (translation_matrix * rotation_matrix * scale_matrix);
    }

    void UpdateSprite2DMatrix()
    {
        //DEBUG CHECKS IF IMAGE IS FROZEN OR NOT SHOWING
        // 1. DID YOU SET IS_DIRTY = true;
        // 2. DO YOU ADD A TRANSFORM COMPONENT TO YOUR OBJECT
        // 3. IS YOUR OBJECTS BEING CALLED IN THE LOOP TO BE PROCESSED? //Uncomment logs to check or ask wei jie

        static std::vector<FlexECS::Entity*> t_entitystack;
        t_entitystack.clear();  // Clear stack at the beginning of each update
        // Unordered set to track processed entities (avoid redundant updates)
        static std::unordered_set<FlexECS::EntityID> t_processedEntities;
        t_processedEntities.clear();

        for (auto& entity : FlexECS::Scene::GetActiveScene()->View<IsActive, Transform>())
        {
            // Check if this entity has already been processed
            if (t_processedEntities.find(entity.Get()) != t_processedEntities.end() || !entity.GetComponent<IsActive>()->is_active) continue; //Skip

            // Traverse up the hierarchy and collect parent entities in a stack
            FlexECS::Entity* t_currentEntity = &entity;
            // Track whether any entity in the stack is dirty
            bool entity_isdirty = false;
            //Update the parent order
            while (true)
            {
                // Push the entity into the stack to process later
                t_entitystack.push_back(t_currentEntity);
                entity_isdirty = t_currentEntity->GetComponent<Transform>()->is_dirty? true: entity_isdirty;

                // Get the parent of the current entity
                Parent* t_parententity = nullptr;
                if ((*t_currentEntity).TryGetComponent<Parent>(t_parententity))
                {
                    t_parententity = (*t_currentEntity).GetComponent<Parent>();
                    
                    // Move up to the parent entity
                    t_currentEntity = &t_parententity->parent;
                }
                else
                {
                    break;
                }
            }
            // At this point, `t_entitystack` contains the chain of entities from the child up to the root

            Matrix4x4 globaltransform = Matrix4x4::Identity;  // Start with identity matrix for the root entity
            for (auto it = t_entitystack.rbegin(); it != t_entitystack.rend(); ++it)
            {
                // Check if the parent has already been processed, don't update matrix
                if (t_processedEntities.find((*it)->Get()) == t_processedEntities.end())
                {
                    // If any entity in the hierarchy is dirty, mark all as dirty
                    (*it)->GetComponent<Transform>()->is_dirty = entity_isdirty ? true : (*it)->GetComponent<Transform>()->is_dirty;
                    //Update current obj transform
                    UpdateTransformationMatrix(**it, globaltransform);
                    // Mark the entity as processed
                    t_processedEntities.insert((*it)->Get());
                }
                // Update the parent's global transform to pass it down to the next child
                globaltransform = (*it)->GetComponent<Transform>()->transform;

                //Log::Debug(FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(*(*it)->GetComponent<EntityName>()));
            }
           // Log::Debug(" ");
            t_entitystack.clear();
        }
        //Log::Debug("****************************************************************");
        
        //Ensure all entities is no longer dirty
        for (auto& entity : FlexECS::Scene::GetActiveScene()->View<IsActive, Transform>())
        {
            if (!entity.GetComponent<IsActive>()->is_active) continue;

            entity.GetComponent<Transform>()->is_dirty = false;
        }
    }

    void RendererSprite2D()
    {
        WindowProps window_props = Application::GetCurrentWindow()->GetProps();
        Renderer2DProps props;
        props.window_size = { static_cast<float>(window_props.width), static_cast<float>(window_props.height) };

        FunctionQueue render_queue;

        if (PostProcessing)
        {
            OpenGLSpriteRenderer::EnablePostProcessing();
            OpenGLSpriteRenderer::ClearFrameBuffer();
        }

        // Render all entities
        for (auto& entity : FlexECS::Scene::GetActiveScene()->View<IsActive, ZIndex, Transform, Shader, Sprite>())
        {
            if (!entity.GetComponent<IsActive>()->is_active) continue;

            auto& z_index = entity.GetComponent<ZIndex>()->z;
            Matrix4x4 transform = entity.GetComponent<Transform>()->transform;
            auto& shader = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(entity.GetComponent<Shader>()->shader);
            auto sprite = entity.GetComponent<Sprite>();

            props.shader = shader;
            props.transform = transform;
            props.texture = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(sprite->texture);
            props.color = sprite->color;
            props.color_to_add = sprite->color_to_add;
            props.color_to_multiply = sprite->color_to_multiply;
            props.alignment = static_cast<Renderer2DProps::Alignment>(sprite->alignment);

            //auto testfn = [&props]() { props.transform.Dump(); };
            //testfn();

            render_queue.Insert({ [props]() { OpenGLSpriteRenderer::DrawTexture2D(props); }, "", z_index });
        }

        // push settings
        bool depth_test = OpenGLRenderer::IsDepthTestEnabled();
        if (depth_test) OpenGLRenderer::DisableDepthTest();

        bool blending = OpenGLRenderer::IsBlendingEnabled();
        if (!blending) OpenGLRenderer::EnableBlending();

        // batch-render

        render_queue.Flush();
        if (PostProcessing)
        {
            OpenGLSpriteRenderer::DisablePostProcessing();
            OpenGLSpriteRenderer::DrawPostProcessingLayer();
        }

        // pop settings

        if (depth_test) OpenGLRenderer::EnableDepthTest();
        if (!blending) OpenGLRenderer::DisableBlending();
    }

}