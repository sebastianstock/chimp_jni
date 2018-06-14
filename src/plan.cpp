#include <chimp_jni/plan.hpp>

namespace chimp_jni
{

std::ostream &operator<<(std::ostream &os, const Plan &plan)
{
    os << "found plan? " << plan.found_plan << "\n";
    os << "Actions:\n";
    for (ChimpFluent fluent : plan.fluents)
    {
        os << fluent.id << "  " << fluent.name << "\n";
    }
    return os;
}
}