#include "chimp_jni/chimp_connector.hpp"
#include "chimp_jni/jni_helper.hpp"
#include <iostream>
#include <stdlib.h>

namespace chimp_jni
{

ChimpConnector::ChimpConnector()
{
    initJvm();
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

Plan ChimpConnector::callChimp()
{
    using namespace std;

    jclass clsCHIMPConnector = env->FindClass("examples/CHIMPConnector"); // try to find the class
    jclass clsPlanResult = env->FindClass("examples/CHIMPConnector$PlanResult");
    if (clsCHIMPConnector == nullptr || clsPlanResult == nullptr)
    {
        cerr << "ERROR: class examples/CHIMPConnector or examples/CHIMPConnector$PlanResult not found!\n";
        jvm->DestroyJavaVM();
        throw JniException("class examples/CHIMPConnector or examples/CHIMPConnector$PlanResult not found.");
    }
    // create a CHIMPConnector object
    jmethodID connectorCtor = env->GetMethodID(clsCHIMPConnector, "<init>", "()V");
    if (connectorCtor == nullptr)
    {
        cerr << "ERROR: constructor not found !" << endl;
        jvm->DestroyJavaVM();
        throw JniException("Constructor for CHIMPConnector not found.");
    }

    jobject chimpConnector = env->NewObject(clsCHIMPConnector, connectorCtor);
    // cout << "Object succesfully constructed !" << endl;

    chimp_jni::Plan resulting_plan;
    if (chimpConnector)
    { // start planning
        jmethodID planMethod = env->GetMethodID(clsCHIMPConnector, "plan",
                                                "(Ljava/lang/String;Ljava/lang/String;)Lexamples/CHIMPConnector$PlanResult;");
        if (planMethod == nullptr)
        {
            cerr << "No plan method !!" << endl;
            jvm->DestroyJavaVM();
            throw JniException("No method named 'plan'.");
        }
        jstring domainPath = env->NewStringUTF("domain.ddl");
        jstring problemPath = env->NewStringUTF("problem.pdl");
        jobject plan = env->CallObjectMethod(chimpConnector, planMethod, problemPath, domainPath);
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
                cout << fluent.id << "  " << fluent.name << "\n";
            }
        }
        catch (chimp_jni::JniParsingException &e)
        {
            cout << e.what() << endl;
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
