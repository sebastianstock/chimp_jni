#ifndef PLAN_HPP
#define PLAN_HPP

#include <iostream>
#include <vector>
#include "chimp_fluent.hpp"

namespace chimp_jni
{

struct Plan
{

    std::vector<chimp_jni::ChimpFluent> fluents;

    bool found_plan;
};


std::ostream &operator<<(std::ostream &os, const Plan &plan);


} // namespace chimp_jni

#endif // PLAN_HPP