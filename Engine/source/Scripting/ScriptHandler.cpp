#include "Scripting/ScriptHandler.h"
#include "Scripting/Scripting.h"


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

// Loads eternal class
jclass LoadEternalClass(JNIEnv* env, const std::string& name) {
    jclass loaded = env->FindClass(name.c_str());
    if (!loaded) {
        std::cout << "Could not load class \"" << name << "\"!";
        std::cin.get();
        exit(69);
    }
    return loaded;
}

Engine::ScriptHandler::ScriptHandler() {
    // Loading java dll
    if (!LoadLibraryA("../OpenJDK/bin/server/jvm.dll")) {
        std::cout << "Could not load java dll!";
        std::cin.get();
        exit(69);
    }

    // Virtual machine arguments
    JavaVMInitArgs vmArgs = {};
    vmArgs.version = JNI_VERSION_10;
    JavaVMOption jvmOptions[1] = {};
    jvmOptions[0].optionString = (char*)"-Djava.class.path=.;../JavApi/JavApi.jar;";
    vmArgs.options = jvmOptions;
    vmArgs.nOptions = 1;

    // Virtual machine creation
    if (JNI_CreateJavaVM(&jvm, (void**)&env, &vmArgs) != JNI_OK) {
        std::cout << "JVM Error";
        std::cin.get();
        exit(69);
    }

    // Engine api class loading
    LoadEternalClass(this->env, "engine/Float2");
    LoadEternalClass(this->env, "engine/Float3");
    LoadEternalClass(this->env, "engine/Entity");
    LoadEternalClass(this->env, "engine/Script");
    Engine::Scripting::engineClass = LoadEternalClass(this->env, "engine/Engine");
    loaderClass = LoadEternalClass(this->env, "engine/Loader");

    // Engine api field getting
    Engine::Scripting::deltaTField = this->getField(Engine::Scripting::engineClass, "deltaT", "F", true);
    Engine::Scripting::elapsedTField = this->getField(Engine::Scripting::engineClass, "elapsedT", "F", true);
    loaderConstr = this->getMethod(loaderClass, "<init>", "()V");

    // Getting system class
    sysClass = env->FindClass("java/lang/System");
    sysGCMethod = env->GetStaticMethodID(sysClass, "gc", "()V");

    // Loader creation
    if (!(loader = env->NewObject(loaderClass, loaderConstr))) {
        std::cout << "Could not create a class loader!";
        std::cin.get();
        exit(69);
    }

    // First cleanup
    env->CallStaticVoidMethod(sysClass, sysGCMethod);
}
Engine::ScriptHandler::~ScriptHandler() {
    jvm->DestroyJavaVM();
}

// Clears all loaded references
void Engine::ScriptHandler::resetLoader() {
    // Deleting references
    for (int i = int(refs.size()) - 1; i >= 0; i--) {
        env->DeleteLocalRef(refs[i]);
    }
    refs.clear();

    // Deleting class loader
    env->DeleteLocalRef(loader);

    // Collecting garbage
    env->CallStaticVoidMethod(sysClass, sysGCMethod);

    // Loader cretaion
    loader = env->NewObject(loaderClass, loaderConstr);
    if (!loader) {
        std::cout << "Could not create a class loader!";
        std::cin.get();
        exit(69);
    }
}

// Loads a new class from file
jclass Engine::ScriptHandler::loadClass(const std::string& name, const std::string& filePath) {
    const std::vector<byte> clsBytes = GetFileBytes(filePath.c_str());
    jclass clsDef = env->DefineClass(name.c_str(), loader, (jbyte*)&clsBytes[0], jsize(clsBytes.size()));
    if (!clsDef) {
        std::cout << "Could not load class \"" << name << "\"!";
        std::cin.get();
        exit(69);
    }
    refs.push_back(clsDef);
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
    refs.push_back(obj);
    return obj;
}

// Deletes a class instance
void Engine::ScriptHandler::delInst(jobject obj) {
    for (int i = 0; i < refs.size(); i++) {
        if (refs[i] == obj) {
            env->DeleteLocalRef(refs[i]);
            refs.erase(refs.begin() + i);
            break;
        }
    }
}

// Creates a new script
Engine::Script* Engine::ScriptHandler::newScript(const std::string& name, const std::string& filePath) {
    return scripts.newInst(new Engine::Script(name, filePath));
}

// Deletes a script
bool Engine::ScriptHandler::delScript(Engine::Script* scr) {
    return scripts.delInst(scr);
}

// Reloads all scripts from files
void Engine::ScriptHandler::reloadScripts() {
    // Old cleanup
    resetLoader();

    // Loading new data
    for (int i = 0; i < scripts.size(); i++) {
        scripts[i]->reload();
    }
}
