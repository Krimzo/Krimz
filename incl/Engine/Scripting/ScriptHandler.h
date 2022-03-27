#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <jni.h>

#include "Engine/Scripting/Script.h"
#include "KrimzLib/memory/pbuffer.h"

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
        std::vector<jobject> refs;

        // Loads engine classes
        void loadEngineClasses();

    public:
        JavaVM* jvm = nullptr;
        JNIEnv* env = nullptr;
        kl::pbuffer<Engine::Script> scripts;

        ScriptHandler();
        ~ScriptHandler();

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

        // Creates a new script
        Engine::Script* newScript(const std::string& name, const std::string& filePath);

        // Deletes a script
        bool delScript(Engine::Script* scr);

        // Reloads all scripts from files
        void reloadScripts();
    };
}
