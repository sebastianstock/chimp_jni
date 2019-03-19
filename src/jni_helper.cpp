#include "chimp_jni/jni_helper.hpp"

namespace chimp_jni
{

FluentConstraint::Type extractFluentConstraintTypeField(JNIEnv *env, const jobject &obj, std::string field_name)
{
    std::string type_str = extractStringField(env, obj, field_name);
    return FluentConstraint::typeFromStr(type_str);
}

std::string jstringToString(JNIEnv *env, jstring &jstr)
{
    if (!jstr)
        return "";

    const jclass stringClass = env->GetObjectClass(jstr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray)env->CallObjectMethod(jstr, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t)env->GetArrayLength(stringJbytes);
    jbyte *pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}

std::string extractStringField(JNIEnv *env, const jobject &obj, std::string field_name)
{
    jfieldID field_id = env->GetFieldID(env->GetObjectClass(obj), field_name.c_str(), "Ljava/lang/String;");
    if (!field_id)
    {
        throw JniParsingException {std::string("Parsing exception. Field does not exist: ") + field_name};
    }
    jstring jstr = (jstring)env->GetObjectField(obj, field_id);
    std::string result = jstringToString(env, jstr);
    env->DeleteLocalRef(jstr);
    return result;
}

bool extractBoolField(JNIEnv *env, const jobject &obj, std::string field_name)
{
    jfieldID field_id = env->GetFieldID(env->GetObjectClass(obj), field_name.c_str(), "Z");
    if (!field_id)
    {
        throw JniParsingException {std::string("Parsing exception. Field does not exist: ") + field_name};
    }
    return (bool)env->GetBooleanField(obj, field_id);
}

int extractIntField(JNIEnv *env, const jobject &obj, std::string field_name)
{
    jfieldID field_id = env->GetFieldID(env->GetObjectClass(obj), field_name.c_str(), "I");
    if (!field_id)
    {
        throw JniParsingException {std::string("Parsing exception. Field does not exist: ") + field_name};
    }
    return (int)env->GetIntField(obj, field_id);
}

long extractLongField(JNIEnv *env, const jobject &obj, std::string field_name)
{
    jfieldID field_id = env->GetFieldID(env->GetObjectClass(obj), field_name.c_str(), "J");
    if (!field_id)
    {
        throw JniParsingException {std::string("Parsing exception. Field does not exist: ") + field_name};
    }
    return (long)env->GetLongField(obj, field_id);
}

std::vector<std::string> extractStringArrayField(JNIEnv *env, const jobject &obj, std::string field_name)
{
    jfieldID field_id = env->GetFieldID(env->GetObjectClass(obj), "taskNames", "[Ljava/lang/String;");
    if (!field_id)
    {
        throw JniParsingException {std::string("Parsing exception. Field does not exist: ") + field_name};
    }
    jobject arr_obj = env->GetObjectField(obj, field_id);
    jobjectArray *arr_arr = reinterpret_cast<jobjectArray *>(&arr_obj);

    std::vector<std::string> ret;
    for (int i = 0; i < (int)env->GetArrayLength(*arr_arr); ++i)
    {
        jstring jstr = (jstring)env->GetObjectArrayElement(*arr_arr, i);
        ret.push_back(jstringToString(env, jstr));
        env->DeleteLocalRef(jstr);
    }
    env->DeleteLocalRef(arr_obj);
    return ret;
}

ChimpFluent convertJobjectToChimpFluent(JNIEnv *env, const jobject &obj)
{
    ChimpFluent fluent;
    fluent.id = extractIntField(env, obj, "id");
    fluent.name = extractStringField(env, obj, "name");
    fluent.type = ChimpFluent::typeFromStr(extractStringField(env, obj, "type"));
    fluent.est = extractLongField(env, obj, "est");
    fluent.lst = extractLongField(env, obj, "lst");
    fluent.eet = extractLongField(env, obj, "eet");
    fluent.let = extractLongField(env, obj, "let");
    fluent.preconditions = extractChimpFluentArrayField(env, obj, "preconditions");

    return fluent;
}

FluentConstraint convertJobjectToFluentConstraint(JNIEnv *env, const jobject &obj)
{
    FluentConstraint constraint;
    constraint.from_id = extractIntField(env, obj, "fromId");
    constraint.to_id = extractIntField(env, obj, "toId");
    constraint.type = extractFluentConstraintTypeField(env, obj, "type");
    constraint.negative_effect = extractBoolField(env, obj, "negativeEffect");
    return constraint;
}

std::vector<ChimpFluent> extractChimpFluentArrayField(JNIEnv *env, const jobject &obj, std::string field_name)
{
    jfieldID field_id = env->GetFieldID(env->GetObjectClass(obj), field_name.c_str(), "[Lexamples/CHIMPConnector$FluentStruct;");
    if (!field_id)
    {
        throw JniParsingException {std::string("Parsing exception. Field does not exist: ") + field_name};
    }
    jobject arr_obj = env->GetObjectField(obj, field_id);
    if (arr_obj == NULL) return std::vector<ChimpFluent>();

    jobjectArray *arr_arr = reinterpret_cast<jobjectArray *>(&arr_obj);

    std::vector<ChimpFluent> ret;
    int length = (int)env->GetArrayLength(*arr_arr);

    for (int i = 0; i < length; ++i)
    {
        jobject fluent_obj = env->GetObjectArrayElement(*arr_arr, i);
        if (fluent_obj)
        {
            ChimpFluent fluent = chimp_jni::convertJobjectToChimpFluent(env, fluent_obj);
            ret.push_back(fluent);
            env->DeleteLocalRef(fluent_obj);
        }
    }
    env->DeleteLocalRef(arr_obj);
    return ret;
}

std::vector<FluentConstraint> extractFluentConstraintArrayField(JNIEnv *env, const jobject &obj, std::string field_name)
{
    jfieldID field_id = env->GetFieldID(env->GetObjectClass(obj), field_name.c_str(), "[Lexamples/CHIMPConnector$FluentConstraintStruct;");
    if (!field_id)
    {
        throw JniParsingException {std::string("Parsing exception. Field does not exist: ") + field_name};
    }
    jobject arr_obj = env->GetObjectField(obj, field_id);
    if (arr_obj == NULL) return std::vector<FluentConstraint>();

    jobjectArray *arr_arr = reinterpret_cast<jobjectArray *>(&arr_obj);

    std::vector<FluentConstraint> ret;
    int length = (int)env->GetArrayLength(*arr_arr);

    for (int i = 0; i < length; ++i)
    {
        jobject fluent_constraint_obj = env->GetObjectArrayElement(*arr_arr, i);
        if (fluent_constraint_obj)
        {
            FluentConstraint fluent_constraint = chimp_jni::convertJobjectToFluentConstraint(env, fluent_constraint_obj);
            ret.push_back(fluent_constraint);
            env->DeleteLocalRef(fluent_constraint_obj);
        }
    }
    env->DeleteLocalRef(arr_obj);
    return ret;
}

}
