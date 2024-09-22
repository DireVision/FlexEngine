#include "charactercomponents.h"

namespace ChronoShift {
  FLX_REFL_REGISTER_START(IsPlayer)
    FLX_REFL_REGISTER_PROPERTY(is_player)
    FLX_REFL_REGISTER_END;

  FLX_REFL_REGISTER_START(CharacterName)
    FLX_REFL_REGISTER_PROPERTY(character_name)
    FLX_REFL_REGISTER_END;

  FLX_REFL_REGISTER_START(CharacterSpeed)
    FLX_REFL_REGISTER_PROPERTY(current_speed)
    FLX_REFL_REGISTER_PROPERTY(base_speed)
    FLX_REFL_REGISTER_END;

  FLX_REFL_REGISTER_START(CharacterHealth)
    FLX_REFL_REGISTER_PROPERTY(base_health)
    FLX_REFL_REGISTER_PROPERTY(current_health)
    FLX_REFL_REGISTER_END;

  FLX_REFL_REGISTER_START(CharacterChronoGear)
    FLX_REFL_REGISTER_PROPERTY(equipped_chrono_gear)
    FLX_REFL_REGISTER_END;

  FLX_REFL_REGISTER_START(CharacterWeapon)
    FLX_REFL_REGISTER_PROPERTY(equipped_weapon)
    FLX_REFL_REGISTER_END;

  FLX_REFL_REGISTER_START(CharacterPassive)
    FLX_REFL_REGISTER_PROPERTY(character_passive)
    FLX_REFL_REGISTER_END;
}