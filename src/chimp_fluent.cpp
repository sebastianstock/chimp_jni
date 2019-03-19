#include <chimp_jni/chimp_fluent.hpp>
#include <iostream>

namespace chimp_jni
{

std::map<std::string, ChimpFluent::Type> ChimpFluent::fluenttype_map = {
    {"Task", ChimpFluent::Type::TASK},
    {"Activity", ChimpFluent::Type::ACTION},
    {"State", ChimpFluent::Type::STATE},
    {"PlannedState", ChimpFluent::Type::PLANNED_STATE},
    {"Undefined", ChimpFluent::Type::UNDEFINED},
};

std::map<ChimpFluent::Type, std::string> ChimpFluent::typenames_map = {
    {ChimpFluent::Type::TASK, "TASK"},
    {ChimpFluent::Type::ACTION, "ACTION"},
    {ChimpFluent::Type::STATE, "STATE"},
    {ChimpFluent::Type::PLANNED_STATE, "PLANNED_STATE"},
    {ChimpFluent::Type::UNDEFINED, "UNDEFINED"},
};

ChimpFluent::Type ChimpFluent::typeFromStr(std::string type_str)
{
    auto type = ChimpFluent::fluenttype_map.find(type_str);
    if (type != ChimpFluent::fluenttype_map.end())
    {
        return type->second;
    }
    return ChimpFluent::Type::UNDEFINED;
}

std::ostream &operator<<(std::ostream &os, const ChimpFluent &fluent)
{
    os << "(" << fluent.id << ") " << ChimpFluent::typenames_map.at(fluent.type) << " : "  << fluent.name;
    return os;
}

}