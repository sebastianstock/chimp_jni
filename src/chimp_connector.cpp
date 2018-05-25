#include "chimp_jni/chimp_connector.hpp"
#include "chimp_jni/jni_helper.hpp"
#include <iostream>

namespace chimp_jni
{

const std::string ChimpConnector::PLANMETHODSIGNATURE = "(Ljava/lang/String;Ljava/lang/String;)Lexamples/CHIMPConnector$PlanResult;";
const std::string ChimpConnector::PLANMETHODNAME = "plan";

ChimpConnector::ChimpConnector(std::string chimpPath)
{
    initJvm(chimpPath);
    setupChimpClasses();
}

ChimpConnector::~ChimpConnector()
{
    jvm->DestroyJavaVM();
}

void ChimpConnector::initJvm(std::string chimpPath)
{
    JavaVMInitArgs vm_args;                      // Initialization arguments
    JavaVMOption *options = new JavaVMOption[1]; // JVM invocation options
    std::vector<char> chimp_path_vec(chimpPath.begin(), chimpPath.end());
    chimp_path_vec.push_back('\0');
    options[0].optionString = &chimp_path_vec[0];
    vm_args.version = JNI_VERSION_1_6; // minimum Java version
    vm_args.nOptions = 1;              // number of options
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false; // invalid options make the JVM init fail
    jint rc = JNI_CreateJavaVM(&jvm, (void **)&env, &vm_args);
    delete options;
    if (rc != JNI_OK)
    {
        std::cerr << "ERROR: could not create vm\n";
        throw JniParsingException("Could not create VM.");
    }
}

void ChimpConnector::setupChimpClasses()
{
    loadChimpConnectorCls();
    loadConnectorCTtorMethodID();
    loadPlanMethodID();
}

void ChimpConnector::loadChimpConnectorCls()
{
    std::string chimp_connector_classname = "examples/CHIMPConnector";
    clsCHIMPConnector = env->FindClass(chimp_connector_classname.c_str()); // try to find the class
    if (clsCHIMPConnector == nullptr)
    {
        std::cerr << "ERROR: class " << chimp_connector_classname << " not found!" << std::endl;
        throw JniException("class " + chimp_connector_classname + " not found.");
    }
}

void ChimpConnector::loadConnectorCTtorMethodID()
{
    connectorCtorMethodID = env->GetMethodID(clsCHIMPConnector, "<init>", "()V");
    if (connectorCtorMethodID == nullptr)
    {
        std::cerr << "ERROR: constructor not found !" << std::endl;
        jvm->DestroyJavaVM();
        throw JniException("Constructor for CHIMPConnector not found.");
    }
}

void ChimpConnector::loadPlanMethodID()
{

    planMethodID = env->GetMethodID(clsCHIMPConnector, PLANMETHODNAME.c_str(),
                                    PLANMETHODSIGNATURE.c_str());
    if (planMethodID == nullptr)
    {
        std::cerr << "No plan method !!" << std::endl;
        jvm->DestroyJavaVM();
        throw JniException("No method named 'plan'.");
    }
}

Plan ChimpConnector::extractPlan(jobject &jPlan)
{
    Plan resultingPlan;
    resultingPlan.found_plan = extractBoolField(env, jPlan, "foundPlan");
    if (resultingPlan.found_plan)
    {
        // try
        // {
        resultingPlan.fluents = extractChimpFluentArrayField(env, jPlan, "fluents");
        // }
        // catch (JniParsingException &e)
        // {
        //     std::cout << e.what() << std::endl;
        // }
    }
    return resultingPlan;
}

Plan ChimpConnector::callChimp(std::string domainPath, std::string problemPath)
{
    jobject jChimpConnector = env->NewObject(clsCHIMPConnector, connectorCtorMethodID);
    if (!jChimpConnector)
        throw new JniException("Could not create ChimpConnector object.");

    // start planning
    jstring jDomainPath = env->NewStringUTF(domainPath.c_str());
    jstring jProblemPath = env->NewStringUTF(problemPath.c_str());
    jobject jPlan = env->CallObjectMethod(jChimpConnector, planMethodID, jProblemPath, jDomainPath);
    env->DeleteLocalRef(jDomainPath);
    env->DeleteLocalRef(jProblemPath);

    // extract result and cleanup
    Plan resultingPlan = extractPlan(jPlan);
    env->DeleteLocalRef(jPlan);
    env->DeleteLocalRef(jChimpConnector);
    return resultingPlan;
}

} // namespace chimp_jni
