#pragma once

enum class CharacterClass
{
    Undefined,

    Fighter,
    Mechanician,
    Archer,
    Pikeman,

    Knight,
    Atalanta,
    Magician,
    Priestess,
}; URHO3D_FLAGSET( CharacterClass, CharacterClassFlags );

enum class CharacterRace
{
    Undefined,

    Tempkrons,
    Morions,
}; URHO3D_FLAGSET( CharacterRace, CharacterRaceFlags );

static const String GetCharacterClassName( const CharacterClassFlags& characterClass )
{
    static HashMap<CharacterClassFlags, String> characterClassName =
    {
        {CharacterClass::Fighter, "Fighter"},
        {CharacterClass::Archer, "Archer"},
        {CharacterClass::Mechanician, "Mechanician"},
        {CharacterClass::Pikeman, "Pikeman"},
        {CharacterClass::Knight, "Knight"},
        {CharacterClass::Atalanta, "Atalanta"},
        {CharacterClass::Magician, "Magician"},
        {CharacterClass::Priestess, "Priestess"}
    };

    auto it = characterClassName.Find( characterClass );

    if( it != characterClassName.End() )
        return characterClassName[characterClass];

    return "Undefined";
}

static const CharacterRaceFlags GetCharacterClassRace( const CharacterClass& characterClass )
{
    static HashMap<CharacterClassFlags, CharacterRaceFlags> characterRace =
    {
        {CharacterClass::Fighter, CharacterRace::Tempkrons},
        {CharacterClass::Archer, CharacterRace::Tempkrons},
        {CharacterClass::Mechanician, CharacterRace::Tempkrons},
        {CharacterClass::Pikeman, CharacterRace::Tempkrons},
        {CharacterClass::Knight, CharacterRace::Morions},
        {CharacterClass::Atalanta, CharacterRace::Morions},
        {CharacterClass::Magician, CharacterRace::Morions},
        {CharacterClass::Priestess, CharacterRace::Morions}
    };

    auto it = characterRace.Find( characterClass );

    if( it != characterRace.End() )
        return characterRace[characterClass];

    return CharacterRace::Undefined;
}

static const String GetCharacterRaceName( const CharacterRaceFlags& characterRace )
{
    static HashMap<CharacterRaceFlags, String> characterRaceName =
    {
        {CharacterRace::Tempkrons, "Tempkrons"},
        {CharacterRace::Morions, "Morions"},
    };

    auto it = characterRaceName.Find( characterRace );

    if( it != characterRaceName.End() )
        return characterRaceName[characterRace];

    return "Undefined";
}

enum CharacterControl
{
    CHARACTERCONTROL_Forward = 1 << 1,
};

namespace CharacterData
{
URHO3D_PARAM( P_ANIMATIONID, AnimationID );
URHO3D_PARAM( P_ANIMATIONEXCLUSIVE, AnimationExclusive );
URHO3D_PARAM( P_MAPID, MapID );
};