#include <chimp_jni/fluent_constraint.hpp>
#include <map>

namespace chimp_jni
{

std::map<std::string, FluentConstraint::Type> FluentConstraint::fluenttype_map = {
    {"MATCHES", FluentConstraint::Type::MATCHES},
    {"DC", FluentConstraint::Type::DC},
    {"PRE", FluentConstraint::Type::PRE},
    {"OPENS", FluentConstraint::Type::OPENS},
    {"CLOSES", FluentConstraint::Type::CLOSES},
    {"BEFORE", FluentConstraint::Type::BEFORE},
    {"UNARYAPPLIED", FluentConstraint::Type::UNARYAPPLIED},
    {"MOVEDURATION", FluentConstraint::Type::MOVEDURATION},
    {"AXIOM", FluentConstraint::Type::AXIOM},
    {"RESOURCEUSAGE", FluentConstraint::Type::RESOURCEUSAGE}
};

std::map<FluentConstraint::Type, std::string> FluentConstraint::typenames_map = {
    {FluentConstraint::Type::MATCHES, "MATCHES"},
    {FluentConstraint::Type::DC, "DC"},
    {FluentConstraint::Type::PRE, "PRE"},
    {FluentConstraint::Type::OPENS, "OPENS"},
    {FluentConstraint::Type::CLOSES, "CLOSES"},
    {FluentConstraint::Type::BEFORE, "BEFORE"},
    {FluentConstraint::Type::UNARYAPPLIED, "UNARYAPPLIED"},
    {FluentConstraint::Type::MOVEDURATION, "MOVEDURATION"},
    {FluentConstraint::Type::AXIOM, "AXIOM"},
    {FluentConstraint::Type::RESOURCEUSAGE, "RESOURCEUSAGE"},
    {FluentConstraint::Type::UNARYAPPLIED, "UNDEFINED"}
};

FluentConstraint::Type FluentConstraint::typeFromStr(std::string type_str)
{
    auto type = FluentConstraint::fluenttype_map.find(type_str);
    if (type != FluentConstraint::fluenttype_map.end())
    {
        return type->second;
    }
    return FluentConstraint::Type::UNDEFINED;
}

std::ostream &operator<<(std::ostream &os, const FluentConstraint &constraint)
{
    os << constraint.from_id;
    os << " -" << FluentConstraint::typenames_map.at(constraint.type) << "-> ";
    os << constraint.to_id;
    return os;
}

}