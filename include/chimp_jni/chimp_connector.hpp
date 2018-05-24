#ifndef CHIMP_CONNECTOR_HPP
#define CHIMP_CONNECTOR_HPP

#include <jni.h>
#include "plan.hpp"

namespace chimp_jni
{

class ChimpConnector
{

  public:
    ChimpConnector();

    ~ChimpConnector();

    Plan callChimp();

  private:

    JavaVM *jvm; // Pointer to the JVM (Java Virtual Machine)
    JNIEnv *env; // Pointer to native interface

    void initJvm();
};

} // namespace chimp_jni

#endif // CHIMP_CONNECTOR_HPP