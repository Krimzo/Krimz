#include "Scripting/Scripting.h"
#include "Logging/Logging.h"
#include <filesystem>


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
jclass Engine::Handler::LoadEternalClass(JNIEnv* env, const std::string& name) {
    jclass loaded = env->FindClass(name.c_str());
    if (!loaded) {
        std::cout << "Could not load class \"" << name << "\"!";
        std::cin.get();
        exit(69);
    }
    return loaded;
}

void Engine::Handler::Init() {
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
    engineClass = LoadEternalClass(env, "engine/Engine");
    loaderClass = LoadEternalClass(env, "engine/script/Loader");
    LoadEternalClass(env, "engine/script/Script");
    LoadEternalClass(env, "engine/math/Int2");
    LoadEternalClass(env, "engine/math/Int3");
    LoadEternalClass(env, "engine/math/Int4");
    LoadEternalClass(env, "engine/math/Float2");
    LoadEternalClass(env, "engine/math/Float3");
    LoadEternalClass(env, "engine/math/Float4");
    LoadEternalClass(env, "engine/math/Mat3");
    LoadEternalClass(env, "engine/math/Mat4");
    LoadEternalClass(env, "engine/math/Plane");
    LoadEternalClass(env, "engine/math/Sphere");
    LoadEternalClass(env, "engine/math/Vertex");
    LoadEternalClass(env, "engine/math/Triangle");
    LoadEternalClass(env, "engine/math/Ray");
    LoadEternalClass(env, "engine/Entity");

    // Engine api field getting
    deltaTField = GetField(engineClass, "deltaT", "F", true);
    elapsedTField = GetField(engineClass, "elapsedT", "F", true);
    loaderConstr = GetMethod(loaderClass, "<init>", "()V");

    // Getting system class
    sysClass = LoadEternalClass(env, "java/lang/System");
    sysGCMethod = GetMethod(sysClass, "gc", "()V", true);

    // Compiler loading
    jclass providerClass = LoadEternalClass(env, "javax/tools/ToolProvider");
    jclass compilerClass = LoadEternalClass(env, "javax/tools/JavaCompiler");
    jmethodID providerMethod = GetMethod(providerClass, "getSystemJavaCompiler", "()Ljavax/tools/JavaCompiler;", true);
    compiler = env->CallStaticObjectMethod(providerClass, providerMethod);
    compileMethod = GetMethod(compilerClass, "run",
        "(Ljava/io/InputStream;Ljava/io/OutputStream;Ljava/io/OutputStream;[Ljava/lang/String;)I");

    // Loader creation
    if (!(loader = env->NewObject(loaderClass, loaderConstr))) {
        std::cout << "Could not create a class loader!";
        std::cin.get();
        exit(69);
    }

    // First cleanup
    env->CallStaticVoidMethod(sysClass, sysGCMethod);
}
void Engine::Handler::Uninit() {
    Handler::scripts.clear();
    Handler::refs.clear();
    jvm->DestroyJavaVM();
}

// Clears all loaded references
void Engine::Handler::ResetLoader() {
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
jclass Engine::Handler::LoadClass(const std::string& name, const std::string& filePath) {
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
jmethodID Engine::Handler::GetMethod(jclass cls, const std::string& name, const std::string& sig, bool isStatic) {
    jmethodID methodID = isStatic ? env->GetStaticMethodID(cls, name.c_str(), sig.c_str()) : env->GetMethodID(cls, name.c_str(), sig.c_str());
    if (!methodID) {
        std::cout << "Could not get method \"" << name << "\"!";
        std::cin.get();
        exit(69);
    }
    return methodID;
}

// Gets a class field
jfieldID Engine::Handler::GetField(jclass cls, const std::string& name, const std::string& sig, bool isStatic) {
    jfieldID fieldID = isStatic ? env->GetStaticFieldID(cls, name.c_str(), sig.c_str()) : env->GetFieldID(cls, name.c_str(), sig.c_str());
    if (!fieldID) {
        std::cout << "Could not get field \"" << name << "\"!";
        std::cin.get();
        exit(69);
    }
    return fieldID;
}

// Creates a new class instance
jobject Engine::Handler::NewInst(jclass cls, jmethodID constr) {
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
void Engine::Handler::DelInst(jobject obj) {
    for (int i = 0; i < refs.size(); i++) {
        if (refs[i] == obj) {
            env->DeleteLocalRef(refs[i]);
            refs.erase(refs.begin() + i);
            break;
        }
    }
}

// Compiles given script
void Engine::Handler::CompileScript(const std::string& filePath) {
    // Setup
    static jclass stringClass = env->FindClass("Ljava/lang/String;");
    jstring fileName = env->NewStringUTF(filePath.c_str());
    jarray nameArray = env->NewObjectArray(1, stringClass, fileName);

    // Compilation and echo
    std::stringstream ss;
    ss << "Compiled \"" << filePath << "\" with error code " <<
        env->CallIntMethod(compiler, compileMethod, NULL, NULL, NULL, nameArray);
    Engine::log(ss.str());

    // Cleanup
    env->DeleteLocalRef(nameArray);
    env->DeleteLocalRef(fileName);
}

// Creates a new script
Engine::Script* Engine::Handler::NewScript(const std::string& filePath) {
    const std::string fileName = std::filesystem::path(filePath).stem().string();
    for (int i = 0; i < scripts.size(); i++) {
        if (scripts[i]->name == fileName) {
            return nullptr;
        }
    }
    return scripts.newInst(new Engine::Script(fileName, filePath));
}

// Deletes a script
bool Engine::Handler::DelScript(Engine::Script* scr) {
    return scripts.delInst(scr);
}

// Reloads all scripts from files
void Engine::Handler::ReloadScripts() {
    // Old cleanup
    ResetLoader();

    // Loading new data
    for (int i = 0; i < scripts.size(); i++) {
        scripts[i]->reload();
    }
}
