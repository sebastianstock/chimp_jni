#ifndef PLAN_HPP
#define PLAN_HPP

#include <vector>
#include "chimp_fluent.hpp"

namespace chimp_jni
{

struct Plan {

    std::vector<chimp_jni::ChimpFluent> fluents;

    bool found_plan;

};

}

#endif // PLAN_HPP