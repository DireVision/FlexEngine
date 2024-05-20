#pragma once

namespace FlexEngine
{

  // The key is specifically the relative path to the asset from the
  // default directory. This is to ensure that the asset manager can
  // easily find the asset.
  // Example: R"(/images/flexengine/flexengine_logo_black.png)"
  // It is stored in a shared pointer to ensure that the key is not
  // deleted when it is still in use.
  using AssetKey = std::string;

}
