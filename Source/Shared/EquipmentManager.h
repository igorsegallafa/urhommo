#pragma once

#include "ManagerImpl.h"
#include "CharacterDef.h"
#include "EquipmentDef.h"

class EquipmentManager : public ManagerImpl
{
    URHO3D_OBJECT( EquipmentManager, ManagerImpl );
public:
    //! Constructor.
    EquipmentManager( Context* context ) : ManagerImpl( context ){}

    //! Deconstructor.
    ~EquipmentManager(){}

    //! Initialize.
    bool Init();

    //! UnInitialize.
    void UnInit();

    /**
     * Get an equipment model data
     * @param type Equipment Type (Armor, Head)
     * @param characterClass Character Class
     * @param equipId Equipment Id
     * @return Equipment Model struct
     */
    const EquipmentModel GetEquipModelData( const EquipType& type, const CharacterClass& characterClass, int equipId );
private:
    //! Load Armor set from Character Class.
    void LoadArmor( const CharacterClass& characterClass );

    //! Load Head set from Character Class.
    void LoadHead( const CharacterClass& characterClass );

    //! Load an specific equipment model from JSON.
    EquipModelData LoadEquipmentModel( const String& type, const JSONValue& value );
private:
    HashMap<CharacterClassFlags, Vector<EquipmentModel>> characterArmorModels_; //!< Character Armor Models.
    HashMap<CharacterClassFlags, Vector<EquipmentModel>> characterHeadModels_;  //!< Character Head Models.
};

