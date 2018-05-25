#include <chimp_jni/chimp_connector.hpp>
#include <chimp_jni/plan.hpp>
#include <chimp_jni/chimp_fluent.hpp>

#include <iostream>

int main(int argc, char **args)
{
    std::string envClasspath = getenv("CLASSPATH");
    std::string chimpPath = "-Djava.class.path=" + envClasspath;
    chimp_jni::ChimpConnector chimp(chimpPath);
    chimp_jni::Plan plan = chimp.callChimp("domain.ddl", "problem.pdl");
    if (plan.found_plan)
    {
        for (chimp_jni::ChimpFluent fluent : plan.fluents)
        {
            std::cout << fluent.id << "  " << fluent.name << "\n";
        }
    }
    else
    {
        std::cout << "Could not find a plan.\n";
    }
}