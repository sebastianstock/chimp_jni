#ifndef CHIMP_FLUENT_HPP
#define CHIMP_FLUENT_HPP

#include <string>
#include <vector>

namespace chimp_jni
{

struct ChimpFluent {

    enum ChimpFluentType {
        TASK, ACTION, STATE
    };

    int id;
    std::string name;
    ChimpFluentType type;

    long est; ///< earliest start time
    long lst; ///< latest start time
    long eet; ///< earliest end time
    long let; ///< latest end time

    std::vector<ChimpFluent> preconditions;

    bool operator < (const ChimpFluent& other) const
    {
        return id < other.id;
    }
};

}

#endif // CHIMP_FLUENT_HPP
