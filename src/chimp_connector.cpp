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
    jvm->DetachCurrentThread();
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
    delete[] options;
    if (rc != JNI_OK)
    {
        std::cerr << "ERROR: could not create vm\n";
        throw JniParsingException("Could not create VM.");
    }
}

void ChimpConnector::loadExceptionHandlingClasses()
{
    clsThrowable = env->FindClass("java/lang/Throwable");
    throwableGetCause =
        env->GetMethodID(clsThrowable,
                    "getCause",
                    "()Ljava/lang/Throwable;");
    throwableGetStackTrace =
        env->GetMethodID(clsThrowable,
                    "getStackTrace",
                    "()[Ljava/lang/StackTraceElement;");
    throwableToString =
        env->GetMethodID(clsThrowable,
                    "toString",
                    "()Ljava/lang/String;");

    clsFrame = env->FindClass("java/lang/StackTraceElement");
    frameToString =
        env->GetMethodID(clsFrame,
                    "toString",
                    "()Ljava/lang/String;");
}

void ChimpConnector::setupChimpClasses()
{
    loadExceptionHandlingClasses();
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
        resultingPlan.all_fluents = extractChimpFluentArrayField(env, jPlan, "allFluents");
        resultingPlan.constraints = extractFluentConstraintArrayField(env, jPlan, "fluentConstraints");
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
    jvm->AttachCurrentThreadAsDaemon((void **) &env, NULL);
    jobject jChimpConnector = env->NewObject(clsCHIMPConnector, connectorCtorMethodID);
    if (!jChimpConnector)
        throw new JniException("Could not create ChimpConnector object.");

    // start planning
    jstring jDomainPath = env->NewStringUTF(domainPath.c_str());
    jstring jProblemPath = env->NewStringUTF(problemPath.c_str());

    // extract result and cleanup
    Plan resultingPlan;
    jobject jPlan = env->CallObjectMethod(jChimpConnector, planMethodID, jProblemPath, jDomainPath);
    if (env->ExceptionCheck())
    {
        resultingPlan.found_plan = false;
        resultingPlan.exception = true;
        jthrowable flag = env->ExceptionOccurred();
        appendExceptionTraceMessages(resultingPlan.exception_description, flag);
        env->ExceptionClear();
    }
    else
    {
        resultingPlan = extractPlan(jPlan);
    }

    env->DeleteLocalRef(jPlan);
    env->DeleteLocalRef(jDomainPath);
    env->DeleteLocalRef(jProblemPath);

    env->DeleteLocalRef(jChimpConnector);
    return resultingPlan;
}

void ChimpConnector::appendExceptionTraceMessages(std::string& exceptionString, jthrowable exception)
{
    // copied from: https://stackoverflow.com/a/10410117

    // Get the array of StackTraceElements.
    jobjectArray frames =
    (jobjectArray) env->CallObjectMethod(exception, throwableToString);
    jsize frames_length = env->GetArrayLength(frames);

    // Add Throwable.toString() before descending
    // stack trace messages.
    if (0 != frames)
    {
        jstring msg_obj =
        (jstring) env->CallObjectMethod(exception, throwableToString);
        const char* msg_str = env->GetStringUTFChars(msg_obj, 0);

        // If this is not the top-of-the-trace then
        // this is a cause.
        if (!exceptionString.empty())
        {
            exceptionString += "\nCaused by: ";
            exceptionString += msg_str;
        }
        else
        {
            exceptionString = msg_str;
        }

        env->ReleaseStringUTFChars(msg_obj, msg_str);
        env->DeleteLocalRef(msg_obj);
    }

    // Append stack trace messages if there are any.
    if (frames_length > 0)
    {
        jsize i = 0;
        for (i = 0; i < frames_length; i++)
        {
            // Get the string returned from the 'toString()'
            // method of the next frame and append it to
            // the error message.
            jobject frame = env->GetObjectArrayElement(frames, i);
            jstring msg_obj =
            (jstring) env->CallObjectMethod(frame, frameToString);

            const char* msg_str = env->GetStringUTFChars(msg_obj, 0);

            exceptionString += "\n    ";
            exceptionString += msg_str;

            env->ReleaseStringUTFChars(msg_obj, msg_str);
            env->DeleteLocalRef(msg_obj);
            env->DeleteLocalRef(frame);
        }
    }

    // If 'exception' has a cause then append the
    // stack trace messages from the cause.
    if (0 != frames)
    {
        jthrowable cause =
        (jthrowable) env->CallObjectMethod(exception, throwableGetCause);
        if (0 != cause)
        {
            appendExceptionTraceMessages(exceptionString, cause);
        }
    }
}

} // namespace chimp_jni
