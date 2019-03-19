#ifndef JNI_HELPER_HPP
#define JNI_HELPER_HPP

#include <jni.h>
#include <string>
#include <vector>
#include <stdexcept>
#include "chimp_fluent.hpp"
#include "fluent_constraint.hpp"

namespace chimp_jni
{

class JniParsingException : public std::runtime_error
{
public:
  JniParsingException(const std::string &msg) : runtime_error(msg) {}
};

class JniException : public std::runtime_error
{
public:
  JniException(const std::string &msg) : runtime_error(msg) {}
};

std::string jstringToString(JNIEnv *env, jstring &jstr);

std::string extractStringField(JNIEnv *env, const jobject &obj, std::string field_name);

bool extractBoolField(JNIEnv *env, const jobject &obj, std::string field_name);

int extractIntField(JNIEnv *env, const jobject &obj, std::string field_name);

long extractLongField(JNIEnv *env, const jobject &obj, std::string field_name);

std::vector<std::string> extractStringArrayField(JNIEnv *env, const jobject &obj, std::string field_name);

ChimpFluent convertJobjectToChimpFluent(JNIEnv *env, const jobject &obj);

FluentConstraint convertJobjectToFluentConstraint(JNIEnv *env, const jobject &obj);

std::vector<ChimpFluent> extractChimpFluentArrayField(JNIEnv *env, const jobject &obj, std::string field_name);

std::vector<FluentConstraint> extractFluentConstraintArrayField(JNIEnv *env, const jobject &obj, std::string field_name);
} // namespace chimp_jni

#endif // JNI_HELPER_HPP