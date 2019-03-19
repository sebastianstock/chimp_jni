#include <chimp_jni/chimp_connector.hpp>
#include <chimp_jni/plan.hpp>
#include <chimp_jni/chimp_fluent.hpp>

#include <iostream>
#include <stdlib.h>
#include <thread>

void testChimp(chimp_jni::ChimpConnector &chimp)
{
    std::cout << "Test calling CHIMP" << std::endl;
    chimp_jni::Plan plan = chimp.callChimp("domain.ddl", "problem.pdl");
    std::cout << "Resulting plan:" << std::endl;
    std::cout << plan;
}

void testChimp2(std::string chimpPath)
{
    chimp_jni::ChimpConnector chimp(chimpPath);
    std::cout << "Test calling CHIMP" << std::endl;
    chimp_jni::Plan plan = chimp.callChimp("domain.ddl", "problem.pdl");
    std::cout << "Resulting plan:" << std::endl;
    std::cout << plan;
}

int main(int argc, char **args)
{
    std::string envClasspath = getenv("CLASSPATH");
    std::string chimpPath = "-Djava.class.path=" + envClasspath;
    // testChimp2(chimpPath);
    // std::cout << "testCHimp2" << std::endl;
    chimp_jni::ChimpConnector chimp(chimpPath);
    std::cout << "First test: " << std::endl;
    testChimp(chimp);
    // std::cout << "Second test: " << std::endl;
    // std::thread second(testChimp, std::ref(chimp));
    // second.join();
    // std::cout << "Third test: " << std::endl;
    // std::thread third(testChimp, std::ref(chimp));
    // third.join();
    // std::cout << "finished" << std::endl;
}