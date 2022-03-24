#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

#include "jni.h"

#pragma comment(lib, "jvm.lib")


namespace Engine {
    class ScriptHandler {
    private:
        // Class loader
        jclass loaderClass = nullptr;
        jmethodID loaderConstr = nullptr;
        jobject loader = nullptr;

        // Garbage collector
        jclass    sysClass = nullptr;
        jmethodID sysGCMethod = nullptr;

        // Reference buffer
        std::vector<jobject> refBuff;

    public:
        JavaVM* jvm = nullptr;
        JNIEnv* env = nullptr;

        ScriptHandler();
        ~ScriptHandler();

        // Loads engine classes
        void loadEngineClasses();

        // Clears all loaded references
        void resetLoader();

        // Loads a new class from file
        jclass loadClass(const std::string& name, const std::string& filePath);

        // Gets a class method
        jmethodID getMethod(jclass cls, const std::string& name, const std::string& sig, bool isStatic = false);

        // Gets a class field
        jfieldID getField(jclass cls, const std::string& name, const std::string& sig, bool isStatic = false);

        // Creates a new class instance
        jobject newInst(jclass cls, jmethodID constr);

        // Deletes a class instance
        void delInst(jobject obj);
    };
}
