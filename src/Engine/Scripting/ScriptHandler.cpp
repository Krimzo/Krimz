#include "Engine/Scripting/ScriptHandler.h"
#include "Engine/Scripting/Scripting.h"


// Returns buffer of file bytes
std::vector<byte> GetFileBytes(const std::string& filePath) {
    // Open file
    FILE* file = nullptr;
    fopen_s(&file, filePath.c_str(), "rb");
    if (!file) {
        std::cout << "File: Could not open file \"" << filePath << "\"!";
        std::cin.get();
        exit(69);
    }

    // Seek to end and get pos
    fseek(file, 0, SEEK_END);
    const int byteSize = ftell(file);

    // Create buff and read data
    std::vector<byte> buff(byteSize);
    rewind(file);
    fread(&buff[0], 1, byteSize, file);

    // Close file
    fclose(file);

    // Return data
    return buff;
}

// Class loader class bytes
const byte loaderClassBytes[] = {
    0xCA, 0xFE, 0xBA, 0xBE, 0x00, 0x00, 0x00, 0x37, 0x00, 0x10, 0x0A, 0x00, 0x03, 0x00, 0x0D, 0x07,
    0x00, 0x0E, 0x07, 0x00, 0x0F, 0x01, 0x00, 0x06, 0x3C, 0x69, 0x6E, 0x69, 0x74, 0x3E, 0x01, 0x00,
    0x03, 0x28, 0x29, 0x56, 0x01, 0x00, 0x04, 0x43, 0x6F, 0x64, 0x65, 0x01, 0x00, 0x0F, 0x4C, 0x69,
    0x6E, 0x65, 0x4E, 0x75, 0x6D, 0x62, 0x65, 0x72, 0x54, 0x61, 0x62, 0x6C, 0x65, 0x01, 0x00, 0x12,
    0x4C, 0x6F, 0x63, 0x61, 0x6C, 0x56, 0x61, 0x72, 0x69, 0x61, 0x62, 0x6C, 0x65, 0x54, 0x61, 0x62,
    0x6C, 0x65, 0x01, 0x00, 0x04, 0x74, 0x68, 0x69, 0x73, 0x01, 0x00, 0x08, 0x4C, 0x4C, 0x6F, 0x61,
    0x64, 0x65, 0x72, 0x3B, 0x01, 0x00, 0x0A, 0x53, 0x6F, 0x75, 0x72, 0x63, 0x65, 0x46, 0x69, 0x6C,
    0x65, 0x01, 0x00, 0x0B, 0x4C, 0x6F, 0x61, 0x64, 0x65, 0x72, 0x2E, 0x6A, 0x61, 0x76, 0x61, 0x0C,
    0x00, 0x04, 0x00, 0x05, 0x01, 0x00, 0x06, 0x4C, 0x6F, 0x61, 0x64, 0x65, 0x72, 0x01, 0x00, 0x15,
    0x6A, 0x61, 0x76, 0x61, 0x2F, 0x6C, 0x61, 0x6E, 0x67, 0x2F, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x4C,
    0x6F, 0x61, 0x64, 0x65, 0x72, 0x00, 0x21, 0x00, 0x02, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x01, 0x00, 0x04, 0x00, 0x05, 0x00, 0x01, 0x00, 0x06, 0x00, 0x00, 0x00, 0x2F, 0x00,
    0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x05, 0x2A, 0xB7, 0x00, 0x01, 0xB1, 0x00, 0x00, 0x00, 0x02,
    0x00, 0x07, 0x00, 0x00, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0x00,
    0x00, 0x0C, 0x00, 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x09, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x0B, 0x00, 0x00, 0x00, 0x02, 0x00, 0x0C
};

Engine::ScriptHandler::ScriptHandler() {
    // Loading java dll
    if (!LoadLibraryA("jdk/bin/server/jvm.dll")) {
        std::cout << "Could not load java dll!";
        std::cin.get();
        exit(69);
    }

    // Virtual machine arguments
    JavaVMInitArgs vmArgs = {};
    vmArgs.version = JNI_VERSION_10;

    // Virtual machine creation
    if (JNI_CreateJavaVM(&jvm, (void**)&env, &vmArgs) != JNI_OK) {
        std::cout << "JVM Error";
        std::cin.get();
        exit(69);
    }

    // Class loader
    resetLoader();
}
Engine::ScriptHandler::~ScriptHandler() {
    jvm->DestroyJavaVM();
}

// Loads engine classes
void Engine::ScriptHandler::loadEngineClasses() {
    // Class loading
    loadClass("engine/Float2", "scpt/out/production/scpt/engine/Float2.class");
    loadClass("engine/Float3", "scpt/out/production/scpt/engine/Float3.class");
    Engine::Scripting::engineClass = loadClass("engine/Engine", "scpt/out/production/scpt/engine/Engine.class");
    loadClass("engine/Entity", "scpt/out/production/scpt/engine/Entity.class");
    loadClass("engine/Script", "scpt/out/production/scpt/engine/Script.class");

    // Field getting
    Engine::Scripting::deltaTField = getField(Engine::Scripting::engineClass, "deltaT", "F", true);
    Engine::Scripting::elapsedTfield = getField(Engine::Scripting::engineClass, "elapsedT", "F", true);
}

// Clears all loaded references
void Engine::ScriptHandler::resetLoader() {
    // Cleanup
    if (loader) {
        // Deleting references
        for (int i = int(refBuff.size()) - 1; i >= 0; i--) {
            env->DeleteLocalRef(refBuff[i]);
        }
        refBuff.clear();

        // Deleting class loader
        env->DeleteLocalRef(loader);

        // Collecting garbage
        env->CallStaticVoidMethod(sysClass, sysGCMethod);
    }
    // First run
    else {
        // Getting class loader class
        loaderClass = env->DefineClass("Loader", nullptr, (jbyte*)loaderClassBytes, sizeof(loaderClassBytes));
        if (!loaderClass) {
            std::cout << "Could not define loader class!";
            std::cin.get();
            exit(69);
        }
        loaderConstr = env->GetMethodID(loaderClass, "<init>", "()V");
        if (!loaderConstr) {
            std::cout << "Could not find loader constructor!";
            std::cin.get();
            exit(69);
        }

        // Getting system class
        sysClass = env->FindClass("java/lang/System");
        sysGCMethod = env->GetStaticMethodID(sysClass, "gc", "()V");

        // First cleanup
        env->CallStaticVoidMethod(sysClass, sysGCMethod);
    }

    // Loader cretaion
    loader = env->NewObject(loaderClass, loaderConstr);
    if (!loader) {
        std::cout << "Could not create a class loader!";
        std::cin.get();
        exit(69);
    }

    // Loading engine classes
    loadEngineClasses();
}

// Loads a new class from file
jclass Engine::ScriptHandler::loadClass(const std::string& name, const std::string& filePath) {
    auto clsBytes = GetFileBytes(filePath.c_str());
    jclass clsDef = env->DefineClass(name.c_str(), loader, (jbyte*)&clsBytes[0], jsize(clsBytes.size()));
    if (!clsDef) {
        std::cout << "Could not load class \"" << name << "\"!";
        std::cin.get();
        exit(69);
    }
    refBuff.push_back(clsDef);
    return clsDef;
}

// Gets a class method
jmethodID Engine::ScriptHandler::getMethod(jclass cls, const std::string& name, const std::string& sig, bool isStatic) {
    jmethodID methodID = isStatic ? env->GetStaticMethodID(cls, name.c_str(), sig.c_str()) : env->GetMethodID(cls, name.c_str(), sig.c_str());
    if (!methodID) {
        std::cout << "Could not get method \"" << name << "\"!";
        std::cin.get();
        exit(69);
    }
    return methodID;
}

// Gets a class field
jfieldID Engine::ScriptHandler::getField(jclass cls, const std::string& name, const std::string& sig, bool isStatic) {
    jfieldID fieldID = isStatic ? env->GetStaticFieldID(cls, name.c_str(), sig.c_str()) : env->GetFieldID(cls, name.c_str(), sig.c_str());
    if (!fieldID) {
        std::cout << "Could not get field \"" << name << "\"!";
        std::cin.get();
        exit(69);
    }
    return fieldID;
}

// Creates a new class instance
jobject Engine::ScriptHandler::newInst(jclass cls, jmethodID constr) {
    jobject obj = env->NewObject(cls, constr);
    if (!obj) {
        std::cout << "Could not create a new class instance!";
        std::cin.get();
        exit(69);
    }
    refBuff.push_back(obj);
    return obj;
}

// Deletes a class instance
void Engine::ScriptHandler::delInst(jobject obj) {
    for (int i = 0; i < refBuff.size(); i++) {
        if (refBuff[i] == obj) {
            env->DeleteLocalRef(refBuff[i]);
            refBuff.erase(refBuff.begin() + i);
            break;
        }
    }
}
