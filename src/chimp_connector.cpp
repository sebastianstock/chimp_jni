#include "chimp_jni/chimp_connector.hpp"
#include "chimp_jni/jni_helper.hpp"
#include <iostream>
#include <stdlib.h>

namespace chimp_jni
{

ChimpConnector::ChimpConnector()
{
    initJvm();
    setupChimpClasses();
}

ChimpConnector::~ChimpConnector()
{
    jvm->DestroyJavaVM();
}

void ChimpConnector::initJvm()
{
    std::string env_classpath = getenv("CLASSPATH");
    std::string chimp_path = "-Djava.class.path=" + env_classpath;

    JavaVMInitArgs vm_args;                      // Initialization arguments
    JavaVMOption *options = new JavaVMOption[1]; // JVM invocation options
    std::vector<char> chimp_path_vec(chimp_path.begin(), chimp_path.end());
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

// jclass ChimpConnector::createPlanResultCls()
// {
//     std::string plan_result_classname = "examples/CHIMPConnector$PlanResult";
//     jclass clsPlanResult = env->FindClass(plan_result_classname.c_str());
//     if (clsPlanResult == nullptr)
//     {
//         std::cerr << "ERROR: class " << plan_result_classname << " not found!" << std::endl;
//         throw JniException("class "  + plan_result_classname + " not found.");
//     }
// }

void ChimpConnector::loadPlanMethodID()
{
    std::string planMethodSignature = "(Ljava/lang/String;Ljava/lang/String;)Lexamples/CHIMPConnector$PlanResult;";
    std::string planMethodName = "plan";
    planMethodID = env->GetMethodID(clsCHIMPConnector, planMethodName.c_str(),
                                    planMethodSignature.c_str());
    if (planMethodID == nullptr)
    {
        std::cerr << "No plan method !!" << std::endl;
        jvm->DestroyJavaVM();
        throw JniException("No method named 'plan'.");
    }
}

Plan ChimpConnector::callChimp()
{
    jobject chimpConnector = env->NewObject(clsCHIMPConnector, connectorCtorMethodID);

    chimp_jni::Plan resulting_plan;
    if (chimpConnector)
    { // start planning

        jstring domainPath = env->NewStringUTF("domain.ddl");
        jstring problemPath = env->NewStringUTF("problem.pdl");
        jobject plan = env->CallObjectMethod(chimpConnector, planMethodID, problemPath, domainPath);
        env->DeleteLocalRef(domainPath);
        env->DeleteLocalRef(problemPath);

        // access fields of PlanResult
        bool foundPlan = chimp_jni::extractBoolField(env, plan, "foundPlan");
        std::cout << "foundPlan? " << (foundPlan ? "true" : "false") << std::endl;

        try
        {
            resulting_plan.fluents = chimp_jni::extractChimpFluentArrayField(env, plan, "fluents");
            for (chimp_jni::ChimpFluent fluent : resulting_plan.fluents)
            {
                std::cout << fluent.id << "  " << fluent.name << "\n";
            }
        }
        catch (chimp_jni::JniParsingException &e)
        {
            std::cout << e.what() << std::endl;
        }

        env->DeleteLocalRef(plan);
    }
    else
    {
        jvm->DestroyJavaVM();
        throw new JniException("Could not create ChimpConnector object.");
    }
    env->DeleteLocalRef(chimpConnector);
    return resulting_plan;
}

} // namespace chimp_jni
