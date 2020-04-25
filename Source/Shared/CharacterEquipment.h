#pragma once

#include "EquipmentDef.h"

namespace Core
{
class CharacterEquipment : public Component
{
    URHO3D_OBJECT( CharacterEquipment, Component );
public:
    //! Constructor.
    CharacterEquipment( Context* context );

    //! Deconstructor.
    ~CharacterEquipment();

    //! Set Head Equipment.
    void SetHead( const EquipmentModel& equipmentModel );

    //! Set Armor Equipment.
    void SetArmor( const EquipmentModel& equipmentModel );

    /**
     * Reset Armor Model
     * All animated models related to armor is removed
     */
    void ResetArmor();
private:
    //! Set Equipment Model.
    void SetEquipmentModel( AnimatedModel* model, const EquipmentModel& equipmentModel, const String& type );
private:
    struct CharacterEquipHead
    {
        AnimatedModel* face;
        AnimatedModel* hair;
        AnimatedModel* accessory;
    };

    struct CharacterEquipArmor
    {
        AnimatedModel* body;
        AnimatedModel* legs;
        AnimatedModel* arms;
        AnimatedModel* shoulders;
        AnimatedModel* waist;
        AnimatedModel* knee;
    };

    CharacterEquipHead equipHead_;
    EquipmentModel equipHeadModel_;

    CharacterEquipArmor equipArmor_;
    EquipmentModel equipArmorModel_;
};
};