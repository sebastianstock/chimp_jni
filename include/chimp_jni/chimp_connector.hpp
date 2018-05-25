#ifndef CHIMP_CONNECTOR_HPP
#define CHIMP_CONNECTOR_HPP

#include <jni.h>
#include <string>
#include "plan.hpp"

namespace chimp_jni
{

class ChimpConnector
{

  public:
    ChimpConnector(std::string chimpPath);

    ~ChimpConnector();

    Plan callChimp(std::string domainPath, std::string problemPath);

  private:

    static const std::string PLANMETHODSIGNATURE;
    static const std::string PLANMETHODNAME;

    JavaVM *jvm; // Pointer to the JVM (Java Virtual Machine)
    JNIEnv *env; // Pointer to native interface
    jclass clsCHIMPConnector;
    jmethodID connectorCtorMethodID;
    jmethodID planMethodID;

    void initJvm(std::string chimpPath);
    void setupChimpClasses();
    void loadChimpConnectorCls();
    void loadConnectorCTtorMethodID();
    void loadPlanMethodID();
    Plan extractPlan(jobject &jPlan);
};

} // namespace chimp_jni

#endif // CHIMP_CONNECTOR_HPP