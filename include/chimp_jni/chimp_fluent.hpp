#ifndef CHIMP_FLUENT_HPP
#define CHIMP_FLUENT_HPP

#include <string>
#include <vector>
#include <map>

namespace chimp_jni
{

struct ChimpFluent {

    enum Type {
        TASK, ACTION, STATE, PLANNED_STATE, UNDEFINED
    };

    int id;
    std::string name;
    Type type;

    long est; ///< earliest start time
    long lst; ///< latest start time
    long eet; ///< earliest end time
    long let; ///< latest end time

    std::vector<ChimpFluent> preconditions;

    bool operator < (const ChimpFluent& other) const
    {
        return id < other.id;
    }

    static std::map<std::string, Type> fluenttype_map ;
    static std::map<Type, std::string> typenames_map;

    static Type typeFromStr(std::string type_str);
};

std::ostream &operator<<(std::ostream &os, const ChimpFluent &constraint);

}

#endif // CHIMP_FLUENT_HPP
