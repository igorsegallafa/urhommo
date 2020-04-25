#pragma once

enum class EquipType
{
    Undefined,

    Armor,
    Head,
};

struct EquipModelData
{
    String model;
    Vector<String> materials;
};

struct EquipmentModel
{
    int id;
    HashMap<String, EquipModelData> models;
};