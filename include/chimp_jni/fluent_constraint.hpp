#ifndef FLUENT_CONSTRAINT_HPP
#define FLUENT_CONSTRAINT_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>

namespace chimp_jni
{

struct FluentConstraint {

    enum Type {
        MATCHES, DC, PRE, OPENS, CLOSES, BEFORE, UNARYAPPLIED, MOVEDURATION,
		AXIOM, RESOURCEUSAGE, UNDEFINED
    };

    int from_id;
    int to_id;
    Type type;
    bool negative_effect;

    bool operator < (const FluentConstraint& other) const
    {
        if (from_id != other.from_id) 
        {
            return from_id < other.from_id;
        }
        if (to_id != other.to_id) 
        {
            return to_id < other.to_id;
        }
        return type < other.type;
    }
    
    static std::map<std::string, Type> fluenttype_map ;
    static std::map<Type, std::string> typenames_map;

    static Type typeFromStr(std::string type_str);
};

std::ostream &operator<<(std::ostream &os, const FluentConstraint &constraint);



}

#endif // FLUENT_CONSTRAINT_HPP
