#ifndef CHIMP_FLUENT_HPP
#define CHIMP_FLUENT_HPP

#include <string>

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
};

}

#endif // CHIMP_FLUENT_HPP