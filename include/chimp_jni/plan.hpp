#ifndef PLAN_HPP
#define PLAN_HPP

#include <iostream>
#include <vector>
#include "chimp_fluent.hpp"
#include "fluent_constraint.hpp"

namespace chimp_jni
{

struct Plan
{

    std::vector<chimp_jni::ChimpFluent> fluents;
    std::vector<chimp_jni::FluentConstraint> constraints;
    std::vector<chimp_jni::ChimpFluent> all_fluents; // TODO or split this up into state_fluents and complex_task_fluents???
    bool found_plan;
    bool exception;
    std::string exception_description;
};


std::ostream &operator<<(std::ostream &os, const Plan &plan);


} // namespace chimp_jni

#endif // PLAN_HPP
