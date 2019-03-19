#include <chimp_jni/plan.hpp>

namespace chimp_jni
{

std::ostream &operator<<(std::ostream &os, const Plan &plan)
{
    os << "found plan? " << plan.found_plan << "\n";
    os << "Actions:\n";
    for (ChimpFluent fluent : plan.fluents)
    {
        os << fluent << "\n";
    }
    os << "All fluents: \n";
    for (ChimpFluent fluent : plan.all_fluents) 
    {
        os << fluent << "\n";
    }
    // os << "Constraints:\n";
    // for (FluentConstraint con : plan.constraints)
    // {
    //     os << con << "\n";
    // }
    return os;
}
}