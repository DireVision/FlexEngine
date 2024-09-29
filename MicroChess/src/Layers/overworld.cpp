#include "overworld.h"

#include "States.h"
#include "Layers.h"

#include "Components/battlecomponents.h"
#include "Components/charactercomponents.h"
#include "Components/physics.h"
#include "Components/rendering.h"
#include "Components/characterinput.h"

#include "BattleSystems/physicssystem.h"

#include "Renderer/sprite2d.h"


namespace ChronoShift {
    float OverworldLayer::m_ScaleDebugTest = 10.f;
    Vector3 OverworldLayer::m_RotateDebugTest = Vector3(0.f, 0.f, 0.f);

  void OverworldLayer::SetupWorld()
  {
    auto scene = FlexECS::Scene::GetActiveScene();

    FlexECS::Entity player1 = FlexECS::Scene::CreateEntity("player1");
    player1.AddComponent<CharacterInput>({ });
    player1.AddComponent<Rigidbody>({ {}, false });
    player1.AddComponent<BoundingBox2D>({ });
    player1.AddComponent<IsActive>({ true });
    player1.AddComponent<Position>({ {200, 600} });
    player1.AddComponent<Rotation>({ });
    player1.AddComponent<Scale>({ { 100,100 } });
    player1.AddComponent<Transform>({ {} });
    player1.AddComponent<ZIndex>({ 10 });
    player1.AddComponent<Sprite>({
      scene->Internal_StringStorage_New(R"(\images\chess_queen.png)"),
      //scene->Internal_StringStorage_New(R"()"),
      Vector3::One,
      Vector3::Zero,
      Vector3::One,
      Renderer2DProps::Alignment_Center
     });
    player1.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });

  
    FlexECS::Entity house = FlexECS::Scene::CreateEntity("house");
    house.AddComponent<Rigidbody>({ {}, true });
    house.AddComponent<BoundingBox2D>({ });
    house.AddComponent<IsActive>({ true });
    house.AddComponent<Position>({ {800, 500 } });
    house.AddComponent<Rotation>({ });
    house.AddComponent<Scale>({ { 250,250 } });
    house.AddComponent<Transform>({ {} });
    house.AddComponent<ZIndex>({ 10 });
    house.AddComponent<Sprite>({
        scene->Internal_StringStorage_New(R"()"),
        { 0.45f, 0.58f, 0.32f },
        Vector3::Zero,
        Vector3::One,
        Renderer2DProps::Alignment_Center
       });
    house.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });

    FlexECS::Entity box = FlexECS::Scene::CreateEntity("box");
    box.AddComponent<Rigidbody>({ {}, false });
    box.AddComponent<BoundingBox2D>({ });
    box.AddComponent<IsActive>({ true });
    box.AddComponent<Position>({ {350, 500 } });
    box.AddComponent<Rotation>({ });
    box.AddComponent<Scale>({ { 150,150 } });
    box.AddComponent<Transform>({ {} });
    box.AddComponent<ZIndex>({ 10 });
    box.AddComponent<Sprite>({
        scene->Internal_StringStorage_New(R"()"),
        { 0.35f, 0.58f, 0.80f },
        Vector3::Zero,
        Vector3::One,
        Renderer2DProps::Alignment_Center
       });
    box.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });

    FlexECS::Entity box2 = FlexECS::Scene::CreateEntity("box2");
    box2.AddComponent<Rigidbody>({ {}, false });
    box2.AddComponent<BoundingBox2D>({ });
    box2.AddComponent<IsActive>({ true });
    box2.AddComponent<Position>({ {1, 0 } });
    box2.AddComponent<Rotation>({ {0,0,30} });
    box2.AddComponent<Scale>({ { 0.8, 0.8 } });
    box2.AddComponent<Transform>({ {} });
    box2.AddComponent<ZIndex>({ 10 });
    box2.AddComponent<Sprite>({
        scene->Internal_StringStorage_New(R"()"),
        { 0.35f, 0.08f, 1.80f },
        Vector3::Zero,
        Vector3::One,
        Renderer2DProps::Alignment_Center
       });
    box2.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });
    box2.AddComponent<Parent>({ box });

    //FlexECS::Entity box3 = FlexECS::Scene::CreateEntity("box3");
    //box3.AddComponent<Rigidbody>({ {}, false });
    //box3.AddComponent<BoundingBox2D>({ });
    //box3.AddComponent<IsActive>({ true });
    //box3.AddComponent<Position>({ {1, 0 } });
    //box3.AddComponent<Rotation>({ {0,0,30} });
    //box3.AddComponent<Scale>({ { 0.8, 0.8 } });
    //box3.AddComponent<Transform>({ {} });
    //box3.AddComponent<ZIndex>({ 10 });
    //box3.AddComponent<Sprite>({
    //    scene->Internal_StringStorage_New(R"()"),
    //    { 1.35f, 0.08f, 0.80f },
    //    Vector3::Zero,
    //    Vector3::One,
    //    Renderer2DProps::Alignment_Center
    //   });
    //box3.AddComponent<Shader>({ scene->Internal_StringStorage_New(R"(\shaders\texture)") });
    //box3.AddComponent<Parent>({ box2 });

  }



  void OverworldLayer::OnAttach()
  {
    FLX_FLOW_BEGINSCOPE();

    // ECS Setup
    auto scene = FlexECS::Scene::CreateScene();
    FlexECS::Scene::SetActiveScene(scene);

    SetupWorld();
  }

  void OverworldLayer::OnDetach()
  {
    FLX_FLOW_ENDSCOPE();
  }

  void OverworldLayer::Update()
  {
    for (auto& entity : FlexECS::Scene::GetActiveScene()->View<CharacterInput>())
    {
      entity.GetComponent<CharacterInput>()->up = Input::GetKey(GLFW_KEY_W);
      entity.GetComponent<CharacterInput>()->down = Input::GetKey(GLFW_KEY_S);
      entity.GetComponent<CharacterInput>()->left = Input::GetKey(GLFW_KEY_A);
      entity.GetComponent<CharacterInput>()->right = Input::GetKey(GLFW_KEY_D);
    }

    for (auto& entity : FlexECS::Scene::GetActiveScene()->View<CharacterInput, Rigidbody>())
    {
      auto& velocity = entity.GetComponent<Rigidbody>()->velocity;
      velocity.x = 0.0f;
      velocity.y = 0.0f;

      if (entity.GetComponent<CharacterInput>()->up)
      {
        velocity.y = -300.0f;
      }

      if (entity.GetComponent<CharacterInput>()->down)
      {
        velocity.y = 300.0f;
      }

      if (entity.GetComponent<CharacterInput>()->left)
      {
        velocity.x = -300.0f;
      }

      if (entity.GetComponent<CharacterInput>()->right)
      {
        velocity.x = 300.0f;
      }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Debug Tests
    //Key hold (Can just alter here, not very elegant but will do for now)
    #if 1 //DEBUG
    if (Input::GetKey(GLFW_KEY_A))
    {
        m_ScaleDebugTest -= 0.8f;
    }
    else if (Input::GetKey(GLFW_KEY_D))
    {
        m_ScaleDebugTest += 0.8f;
    }

    if (Input::GetKey(GLFW_KEY_Q))
    {
        m_RotateDebugTest.z += 1.0f;
    }
    else if (Input::GetKey(GLFW_KEY_E))
    {
        m_RotateDebugTest.z -= 1.0f;
    }

    //Altering entities scale and rotation while game is in debug mode
    // TEST ON EVERYTHING
    //for (auto& entity : FlexECS::Scene::GetActiveScene()->View<IsActive, Scale, Rotation>())
    //{
    //    if (!entity.GetComponent<IsActive>()->is_active) continue;

    //    //Search function for a specific object to test and NOT everything
    //    auto entity_name_component = entity.GetComponent<EntityName>();
    //    //Change "" to whatever object or comment the line to affect everything
    //    if ("box2" != FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(*entity_name_component)) continue;

    //    auto& scale = entity.GetComponent<Scale>()->scale;
    //    auto& rotation = entity.GetComponent<Rotation>()->rotation;

    //    scale = Vector2(m_ScaleDebugTest, m_ScaleDebugTest);
    //    rotation = m_RotateDebugTest;
    //}
    #endif
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    UpdatePhysicsSystem();

    //Update Transformation Matrix of All Entities
    UpdateSprite2DMatrix();

    //Render All Entities
    RendererSprite2D();
  }
}
