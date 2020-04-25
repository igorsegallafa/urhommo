#include "PrecompiledHeader.h"
#include "EquipmentManager.h"

bool EquipmentManager::Init()
{
    LoadArmor( CharacterClass::Fighter );
    LoadHead( CharacterClass::Fighter );

    return true;
}

void EquipmentManager::UnInit()
{
    characterArmorModels_.Clear();
    characterHeadModels_.Clear();
}

const EquipmentModel EquipmentManager::GetEquipModelData( const EquipType& type, const CharacterClass& characterClass, int equipId )
{
    HashMap<CharacterClassFlags, Vector<EquipmentModel>> modelsMap;

    //Check what map we should to use for get the equipment model
    switch( type )
    {
        case EquipType::Armor:
            modelsMap = characterArmorModels_;
            break;
        case EquipType::Head:
            modelsMap = characterHeadModels_;
            break;
    }

    //Valid map?
    if( modelsMap.Size() )
    {
        //Find if the equipment model list was already defined for this character class
        auto it = modelsMap.Find( characterClass );

        //Iterator has been found
        if( it != modelsMap.End() )
            for( const auto& equipModels : it->second_ )
                if( equipModels.id == equipId )
                    return equipModels;
    }

    return EquipmentModel{};
}

void EquipmentManager::LoadArmor( const CharacterClass& characterClass )
{
    auto file = GetSubsystem<ResourceCache>()->GetResource<JSONFile>( "Definitions/Equip/Armor/" + GetCharacterClassName( characterClass ) + ".json" );

    if( file )
    {
        auto root = file->GetRoot();

        for( const auto& equip : root.GetArray() )
        {
            auto equipmentId = equip["id"].GetInt();

            //Load equipment parts from armor
            EquipmentModel equipModel;
            equipModel.id = equipmentId;
            equipModel.models["body"] = LoadEquipmentModel( "body", equip );
            equipModel.models["legs"] = LoadEquipmentModel( "legs", equip );
            equipModel.models["arms"] = LoadEquipmentModel( "arms", equip );
            equipModel.models["shoulders"] = LoadEquipmentModel( "shoulders", equip );
            equipModel.models["waist"] = LoadEquipmentModel( "waist", equip );
            equipModel.models["knee"] = LoadEquipmentModel( "knee", equip );

            characterArmorModels_[characterClass].Push( equipModel );
        }
    }
}

void EquipmentManager::LoadHead( const CharacterClass& characterClass )
{
    auto file = GetSubsystem<ResourceCache>()->GetResource<JSONFile>( "Definitions/Equip/Head/" + GetCharacterClassName( characterClass ) + ".json" );

    if( file )
    {
        auto root = file->GetRoot();

        for( const auto& equip : root.GetArray() )
        {
            auto equipmentId = equip["id"].GetInt();

            //Load equipment parts from head
            EquipmentModel equipModel;
            equipModel.id = equipmentId;
            equipModel.models["face"] = LoadEquipmentModel( "face", equip );
            equipModel.models["hair"] = LoadEquipmentModel( "hair", equip );
            equipModel.models["accessory"] = LoadEquipmentModel( "accessory", equip );
            characterHeadModels_[characterClass].Push( equipModel );
        }
    }
}

EquipModelData EquipmentManager::LoadEquipmentModel( const String& type, const JSONValue& value )
{
    auto modelMap = EquipModelData{};
    auto arrayModels = value[type].GetArray();

    for( const auto& elem : arrayModels )
    {
        //If we are looking for an array, so it's a material
        if( elem.IsArray() )
        {
            for( const auto& material : elem.GetArray() )
                modelMap.materials.Push( material.GetString() );
        }
        else
            modelMap.model = elem.GetString();
    }

    return modelMap;
}