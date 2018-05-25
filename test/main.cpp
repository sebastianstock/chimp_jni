#include <chimp_jni/chimp_connector.hpp>
#include <chimp_jni/plan.hpp>
#include <chimp_jni/chimp_fluent.hpp>

#include <iostream>
#include <stdlib.h>

int main(int argc, char **args)
{
    std::string envClasspath = getenv("CLASSPATH");
    std::string chimpPath = "-Djava.class.path=" + envClasspath;
    chimp_jni::ChimpConnector chimp(chimpPath);
    chimp_jni::Plan plan = chimp.callChimp("domain.ddl", "problem.pdl");
    std::cout << plan;
}