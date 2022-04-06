#include "Scripting/Scripting.h"
#include "Logging/Logging.h"
#include "Data/Entities.h"
#include "KrimzLib.h"


// Loads eternal class
jclass Engine::JavaHandler::LoadEternalClass(const std::string& name)
{
	jclass loaded = env->FindClass(name.c_str());
	if (!loaded)
	{
		std::cout << "Could not load eternal class \"" << name << "\"!";
		std::cin.get();
		exit(69);
	}
	return loaded;
}

void Engine::JavaHandler::Init()
{
	// Loading java dll
	if (!LoadLibraryA("../OpenJDK/bin/server/jvm.dll"))
	{
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
	if (JNI_CreateJavaVM(&jvm, (void**)&env, &vmArgs) != JNI_OK)
	{
		std::cout << "JVM Error";
		std::cin.get();
		exit(69);
	}

	// Engine api class loading
	engineClass = LoadEternalClass("engine/Engine");
	loaderClass = LoadEternalClass("engine/script/Loader");
	LoadEternalClass("engine/script/Script");
	LoadEternalClass("engine/math/Int2");
	LoadEternalClass("engine/math/Int3");
	LoadEternalClass("engine/math/Int4");
	LoadEternalClass("engine/math/Float2");
	LoadEternalClass("engine/math/Float3");
	LoadEternalClass("engine/math/Float4");
	LoadEternalClass("engine/math/Mat3");
	LoadEternalClass("engine/math/Mat4");
	LoadEternalClass("engine/math/Plane");
	LoadEternalClass("engine/math/Sphere");
	LoadEternalClass("engine/math/Vertex");
	LoadEternalClass("engine/math/Triangle");
	LoadEternalClass("engine/math/Ray");
	LoadEternalClass("engine/Entity");

	// Engine api field getting
	deltaTField = GetField(engineClass, "deltaT", "F", true);
	elapsedTField = GetField(engineClass, "elapsedT", "F", true);
	loaderConstr = GetMethod(loaderClass, "<init>", "()V");

	// Getting system class
	sysClass = LoadEternalClass("java/lang/System");
	sysGCMethod = GetMethod(sysClass, "gc", "()V", true);

	// Compiler loading
	jclass providerClass = LoadEternalClass("javax/tools/ToolProvider");
	jclass compilerClass = LoadEternalClass("javax/tools/JavaCompiler");
	jmethodID providerMethod = GetMethod(providerClass, "getSystemJavaCompiler", "()Ljavax/tools/JavaCompiler;", true);
	compiler = env->CallStaticObjectMethod(providerClass, providerMethod);
	compileMethod = GetMethod(compilerClass, "run",
		"(Ljava/io/InputStream;Ljava/io/OutputStream;Ljava/io/OutputStream;[Ljava/lang/String;)I");

	// Loader creation
	if (!(loader = env->NewObject(loaderClass, loaderConstr)))
	{
		std::cout << "Could not create a class loader!";
		std::cin.get();
		exit(69);
	}

	// First cleanup
	env->CallStaticVoidMethod(sysClass, sysGCMethod);
}
void Engine::JavaHandler::Uninit()
{
	classes.clear();
	refs.clear();
	jvm->DestroyJavaVM();
}

// Clears all loaded references
void Engine::JavaHandler::ResetLoader()
{
	// Deleting references
	for (int i = int(refs.size()) - 1; i >= 0; i--)
		env->DeleteLocalRef(refs[i]);
	refs.clear();
	classes.clear();

	// Deleting class loader
	env->DeleteLocalRef(loader);

	// Collecting garbage
	env->CallStaticVoidMethod(sysClass, sysGCMethod);

	// Loader cretaion
	loader = env->NewObject(loaderClass, loaderConstr);
	if (!loader)
	{
		std::cout << "Could not create a class loader!";
		std::cin.get();
		exit(69);
	}
}

// Loads a new class from file
jclass Engine::JavaHandler::LoadClass(const std::string& filePath)
{
	// Loading file data
	const std::vector<byte> clsBytes = kl::file::readB(filePath.c_str());
	const std::string fileName = std::filesystem::path(filePath).stem().string();

	// Defining class bytes
	jclass clsDef = env->DefineClass(fileName.c_str(), loader, (jbyte*)&clsBytes[0], jsize(clsBytes.size()));
	if (!clsDef)
	{
		// Search for already defined class
		for (auto& cls : classes)
			if (cls.name == fileName)
				return cls.cls;

		// Exit
		std::cout << "Could not load class \"" << fileName << "\"!";
		std::cin.get();
		exit(69);
	}

	// Saving and return
	classes.push_back(Engine::JavaClass(fileName, clsDef));
	refs.push_back(clsDef);
	return clsDef;
}

// Gets a class method
jmethodID Engine::JavaHandler::GetMethod(jclass cls, const std::string& name, const std::string& sig, bool isStatic)
{
	jmethodID methodID = isStatic ? env->GetStaticMethodID(cls, name.c_str(), sig.c_str()) : env->GetMethodID(cls, name.c_str(), sig.c_str());
	if (!methodID)
	{
		std::cout << "Could not get method \"" << name << "\"!";
		std::cin.get();
		exit(69);
	}
	return methodID;
}

// Gets a class field
jfieldID Engine::JavaHandler::GetField(jclass cls, const std::string& name, const std::string& sig, bool isStatic)
{
	jfieldID fieldID = isStatic ? env->GetStaticFieldID(cls, name.c_str(), sig.c_str()) : env->GetFieldID(cls, name.c_str(), sig.c_str());
	if (!fieldID)
	{
		std::cout << "Could not get field \"" << name << "\"!";
		std::cin.get();
		exit(69);
	}
	return fieldID;
}

// Creates a new class instance
jobject Engine::JavaHandler::NewInst(jclass cls, jmethodID constr)
{
	jobject obj = env->NewObject(cls, constr);
	if (!obj)
	{
		std::cout << "Could not create a new class instance!";
		std::cin.get();
		exit(69);
	}
	refs.push_back(obj);
	return obj;
}

// Deletes a class instance
void Engine::JavaHandler::DelInst(jobject obj)
{
	for (int i = 0; i < refs.size(); i++)
	{
		if (refs[i] == obj)
		{
			env->DeleteLocalRef(refs[i]);
			refs.erase(refs.begin() + i);
			break;
		}
	}
}

// Compiles given script
void Engine::JavaHandler::CompileFile(const std::string& filePath)
{
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

// Reloads all scripts from files
void Engine::JavaHandler::ReloadScripts()
{
	// Old cleanup
	ResetLoader();

	// Loading new data
	for (int i = 0; i < Engine::entities.size(); i++)
		for (auto& scr : Engine::entities[i]->scripts)
			scr.reload();
}
