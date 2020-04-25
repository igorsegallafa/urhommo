#include "PrecompiledHeader.h"
#include "CharacterEquipment.h"

namespace Core
{
CharacterEquipment::CharacterEquipment( Context* context ) :
    Component( context ),
    equipHead_{ nullptr },
    equipArmor_{ nullptr }
{
}

CharacterEquipment::~CharacterEquipment()
{
}

void CharacterEquipment::SetHead( const EquipmentModel& equipmentModel )
{
    //Save equipment model
    equipHeadModel_ = equipmentModel;

    //Create Animated Model component when necessary
    if( equipHead_.face == nullptr )
    {
        equipHead_.face = node_->GetChild( "CharacterModel" )->CreateComponent<AnimatedModel>();
        equipHead_.face->SetCastShadows( true );
    }

    if( equipHead_.hair == nullptr )
    {
        equipHead_.hair = node_->GetChild( "CharacterModel" )->CreateComponent<AnimatedModel>();
        equipHead_.hair->SetCastShadows( true );
    }

    if( equipHead_.accessory == nullptr )
    {
        equipHead_.accessory = node_->GetChild( "CharacterModel" )->CreateComponent<AnimatedModel>();
        equipHead_.accessory->SetCastShadows( true );
    }

    //Reset Armor because the skeleton will be recreated
    ResetArmor();

    //Set Head
    SetEquipmentModel( equipHead_.face, equipmentModel, "face" );
    SetEquipmentModel( equipHead_.hair, equipmentModel, "hair" );
    SetEquipmentModel( equipHead_.accessory, equipmentModel, "accessory" );

    //Set Armor
    SetArmor( equipArmorModel_ );
}

void CharacterEquipment::SetArmor( const EquipmentModel& equipmentModel )
{
    //Save equipment model
    equipArmorModel_ = equipmentModel;

    //Create Animated model from armor parts when necessary
    if( equipArmor_.body == nullptr )
    {
        equipArmor_.body = node_->GetChild( "CharacterModel" )->CreateComponent<AnimatedModel>();
        equipArmor_.body->SetCastShadows( true );
    }

    if( equipArmor_.legs == nullptr )
    {
        equipArmor_.legs = node_->GetChild( "CharacterModel" )->CreateComponent<AnimatedModel>();
        equipArmor_.legs->SetCastShadows( true );
    }

    if( equipArmor_.arms == nullptr )
    {
        equipArmor_.arms = node_->GetChild( "CharacterModel" )->CreateComponent<AnimatedModel>();
        equipArmor_.arms->SetCastShadows( true );
    }

    if( equipArmor_.shoulders == nullptr )
    {
        equipArmor_.shoulders = node_->GetChild( "CharacterModel" )->CreateComponent<AnimatedModel>();
        equipArmor_.shoulders->SetCastShadows( true );
    }

    if( equipArmor_.waist == nullptr )
    {
        equipArmor_.waist = node_->GetChild( "CharacterModel" )->CreateComponent<AnimatedModel>();
        equipArmor_.waist->SetCastShadows( true );
    }

    if( equipArmor_.knee == nullptr )
    {
        equipArmor_.knee = node_->GetChild( "CharacterModel" )->CreateComponent<AnimatedModel>();
        equipArmor_.knee->SetCastShadows( true );
    }

    //Set Armor
    SetEquipmentModel( equipArmor_.body, equipmentModel, "body" );
    SetEquipmentModel( equipArmor_.legs, equipmentModel, "legs" );
    SetEquipmentModel( equipArmor_.arms, equipmentModel, "arms" );
    SetEquipmentModel( equipArmor_.shoulders, equipmentModel, "shoulders" );
    SetEquipmentModel( equipArmor_.waist, equipmentModel, "waist" );
    SetEquipmentModel( equipArmor_.knee, equipmentModel, "knee" );
}

void CharacterEquipment::ResetArmor()
{
    if( equipArmor_.body )
    {
        equipArmor_.body->Remove();
        equipArmor_.body = nullptr;
    }

    if( equipArmor_.legs )
    {
        equipArmor_.legs->Remove();
        equipArmor_.legs = nullptr;
    }

    if( equipArmor_.arms )
    {
        equipArmor_.arms->Remove();
        equipArmor_.arms = nullptr;
    }

    if( equipArmor_.shoulders )
    {
        equipArmor_.shoulders->Remove();
        equipArmor_.shoulders = nullptr;
    }

    if( equipArmor_.waist )
    {
        equipArmor_.waist->Remove();
        equipArmor_.waist = nullptr;
    }

    if( equipArmor_.knee )
    {
        equipArmor_.knee->Remove();
        equipArmor_.knee = nullptr;
    }
}

void CharacterEquipment::SetEquipmentModel( AnimatedModel* model, const EquipmentModel& equipmentModel, const String& type )
{
    if( model )
    {
        auto it = equipmentModel.models.Find( type );

        if( it != equipmentModel.models.End() )
        {
            //Set Model and mark it as enabled
            if( it->second_.model.Length() && it->second_.materials.Size() )
            {
                model->SetModel( GetSubsystem<ResourceCache>()->GetResource<Model>( it->second_.model ) );
                model->SetEnabled( true );

                for( unsigned i = 0; i < it->second_.materials.Size(); i++ )
                    model->SetMaterial( i, GetSubsystem<ResourceCache>()->GetResource<Material>( it->second_.materials[i] ) );
            }
            else
                model->SetEnabled( false );
        }
    }
}
};