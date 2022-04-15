#include "Scripting/Scripting.h"
#include "Logging/Logging.h"
#include "Data/Entities.h"
#include "KrimzLib.h"


void Engine::JavaHandler::Init()
{
	// Loading java dll
	if (!LoadLibraryA("../OpenJDK/bin/server/jvm.dll"))
	{
		kl::console::show();
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
		kl::console::show();
		std::cout << "JVM creation error!";
		std::cin.get();
		exit(69);
	}

	// Engine api class loading
	loaderClass = LoadEternalClass("engine/script/Loader");
	loggerClass = LoadEternalClass("engine/Logger");
	timeClass = LoadEternalClass("engine/Time");
	mouseClass = LoadEternalClass("engine/input/Mouse");
	keysClass = LoadEternalClass("engine/input/Keys");
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
	deltaTField = GetField(timeClass, "deltaT", "F", true);
	elapsedTField = GetField(timeClass, "elapsedT", "F", true);
	loggerFlushMethod = GetMethod(loggerClass, "flush", "()[Ljava/lang/String;", true);
	loaderConstr = GetMethod(loaderClass, "<init>", "()V");
	lmbField = GetField(mouseClass, "lmb", "Z", true);
	mmbField = GetField(mouseClass, "mmb", "Z", true);
	rmbField = GetField(mouseClass, "rmb", "Z", true);
	moPosField = GetField(mouseClass, "position", "Lengine/math/Int2;", true);
	scrollField = GetField(mouseClass, "scroll", "I", true);
	qField = GetField(keysClass, "q", "Z", true);
	wField = GetField(keysClass, "w", "Z", true);
	eField = GetField(keysClass, "e", "Z", true);
	rField = GetField(keysClass, "r", "Z", true);
	tField = GetField(keysClass, "t", "Z", true);
	zField = GetField(keysClass, "z", "Z", true);
	uField = GetField(keysClass, "u", "Z", true);
	iField = GetField(keysClass, "i", "Z", true);
	oField = GetField(keysClass, "o", "Z", true);
	pField = GetField(keysClass, "p", "Z", true);
	aField = GetField(keysClass, "a", "Z", true);
	sField = GetField(keysClass, "s", "Z", true);
	dField = GetField(keysClass, "d", "Z", true);
	fField = GetField(keysClass, "f", "Z", true);
	gField = GetField(keysClass, "g", "Z", true);
	hField = GetField(keysClass, "h", "Z", true);
	jField = GetField(keysClass, "j", "Z", true);
	kField = GetField(keysClass, "k", "Z", true);
	lField = GetField(keysClass, "l", "Z", true);
	yField = GetField(keysClass, "y", "Z", true);
	xField = GetField(keysClass, "x", "Z", true);
	cField = GetField(keysClass, "c", "Z", true);
	vField = GetField(keysClass, "v", "Z", true);
	bField = GetField(keysClass, "b", "Z", true);
	nField = GetField(keysClass, "n", "Z", true);
	mField = GetField(keysClass, "m", "Z", true);
	num0Field = GetField(keysClass, "num0", "Z", true);
	num1Field = GetField(keysClass, "num1", "Z", true);
	num2Field = GetField(keysClass, "num2", "Z", true);
	num3Field = GetField(keysClass, "num3", "Z", true);
	num4Field = GetField(keysClass, "num4", "Z", true);
	num5Field = GetField(keysClass, "num5", "Z", true);
	num6Field = GetField(keysClass, "num6", "Z", true);
	num7Field = GetField(keysClass, "num7", "Z", true);
	num8Field = GetField(keysClass, "num8", "Z", true);
	num9Field = GetField(keysClass, "num9", "Z", true);
	periodField = GetField(keysClass, "period", "Z", true);
	commaField = GetField(keysClass, "comma", "Z", true);
	plusField = GetField(keysClass, "plus", "Z", true);
	minusField = GetField(keysClass, "minus", "Z", true);
	escField = GetField(keysClass, "esc", "Z", true);
	tabField = GetField(keysClass, "tab", "Z", true);
	capsField = GetField(keysClass, "caps", "Z", true);
	shiftField = GetField(keysClass, "shift", "Z", true);
	ctrlField = GetField(keysClass, "ctrl", "Z", true);
	altField = GetField(keysClass, "alt", "Z", true);
	spaceField = GetField(keysClass, "space", "Z", true);
	enterField = GetField(keysClass, "enter", "Z", true);
	insertField = GetField(keysClass, "insert", "Z", true);
	deletField = GetField(keysClass, "delete", "Z", true);
	upField = GetField(keysClass, "up", "Z", true);
	leftField = GetField(keysClass, "left", "Z", true);
	downField = GetField(keysClass, "down", "Z", true);
	rightField = GetField(keysClass, "right", "Z", true);
	f1Field = GetField(keysClass, "f1", "Z", true);
	f2Field = GetField(keysClass, "f2", "Z", true);
	f3Field = GetField(keysClass, "f3", "Z", true);
	f4Field = GetField(keysClass, "f4", "Z", true);
	f5Field = GetField(keysClass, "f5", "Z", true);
	f6Field = GetField(keysClass, "f6", "Z", true);
	f7Field = GetField(keysClass, "f7", "Z", true);
	f8Field = GetField(keysClass, "f8", "Z", true);
	f9Field = GetField(keysClass, "f9", "Z", true);
	f10Field = GetField(keysClass, "f10", "Z", true);
	f11Field = GetField(keysClass, "f11", "Z", true);
	f12Field = GetField(keysClass, "f12", "Z", true);

	// Getting system class
	sysClass = LoadEternalClass("java/lang/System");
	sysGCMethod = GetMethod(sysClass, "gc", "()V", true);

	// Compiler loading
	stringClass = env->FindClass("Ljava/lang/String;");
	jclass providerClass = LoadEternalClass("javax/tools/ToolProvider");
	jclass compilerClass = LoadEternalClass("javax/tools/JavaCompiler");
	jmethodID providerMethod = GetMethod(providerClass, "getSystemJavaCompiler", "()Ljavax/tools/JavaCompiler;", true);
	compiler = env->CallStaticObjectMethod(providerClass, providerMethod);
	compileMethod = GetMethod(compilerClass, "run",
		"(Ljava/io/InputStream;Ljava/io/OutputStream;Ljava/io/OutputStream;[Ljava/lang/String;)I");

	// Loader creation
	if (!(loader = env->NewObject(loaderClass, loaderConstr)))
	{
		kl::console::show();
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
	// Cleanup
	for (int i = int(refs.size()) - 1; i >= 0; i--)
		env->DeleteLocalRef(refs[i]);
	refs.clear();
	classes.clear();

	// Deleting class loader
	env->DeleteLocalRef(loader);

	// Collecting garbage
	env->CallStaticVoidMethod(sysClass, sysGCMethod);

	// Loader creation
	loader = env->NewObject(loaderClass, loaderConstr);
	if (!loader)
	{
		kl::console::show();
		std::cout << "Could not create a class loader!";
		std::cin.get();
		exit(69);
	}
}

// Loads eternal class
jclass Engine::JavaHandler::LoadEternalClass(const std::string& name)
{
	jclass loaded = env->FindClass(name.c_str());
	if (!loaded)
		Engine::log("Could not load eternal class \"" + name + "\"!");
	return loaded;
}

// Loads a new class from file
jclass Engine::JavaHandler::LoadClass(const std::string& filePath)
{
	// Compilation and byte-code loading
	CompileFile(filePath);
	const std::string classPath = std::filesystem::path(filePath).replace_extension("class").string();
	const std::vector<byte> clsBytes = kl::file::readB(classPath);

	// Cleanup and check
	const std::string parentPath = std::filesystem::path(filePath).parent_path().string();
	for (const auto& file : std::filesystem::recursive_directory_iterator(parentPath))
		if (std::filesystem::path(file).extension().string() == ".class")
			std::filesystem::remove(file);
	if (!clsBytes.size())
		return nullptr;

	// Defining class with bytes
	jclass clsDef = env->DefineClass(nullptr, loader, (jbyte*)&clsBytes[0], jsize(clsBytes.size()));
	if (!clsDef)
	{
		// Exists check
		for (auto& cls : classes)
			if (cls.name == filePath)
				return cls.cls;

		// Bad script
		Engine::log("Could not define class \"" + filePath + "\"!");
		return nullptr;
	}

	// Saving and return
	classes.push_back(Engine::JavaClass(filePath, clsDef));
	refs.push_back(clsDef);
	return clsDef;
}

// Gets a class method
jmethodID Engine::JavaHandler::GetMethod(jclass cls, const std::string& name, const std::string& sig, bool isStatic)
{
	jmethodID methodID = isStatic ? env->GetStaticMethodID(cls, name.c_str(), sig.c_str()) : env->GetMethodID(cls, name.c_str(), sig.c_str());
	if (!methodID)
		Engine::log("Could not get method \"" + name + "\"!");
	return methodID;
}

// Gets a class field
jfieldID Engine::JavaHandler::GetField(jclass cls, const std::string& name, const std::string& sig, bool isStatic)
{
	jfieldID fieldID = isStatic ? env->GetStaticFieldID(cls, name.c_str(), sig.c_str()) : env->GetFieldID(cls, name.c_str(), sig.c_str());
	if (!fieldID)
		Engine::log("Could not get field \"" + name + "\"!");
	return fieldID;
}

// Creates a new class instance
jobject Engine::JavaHandler::NewInst(jclass cls, jmethodID constr)
{
	jobject obj = env->NewObject(cls, constr);
	if (!obj)
		Engine::log("Could not create a new class instance!");
	else
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
	jstring args[3]
	{
		env->NewStringUTF("-cp"),
		env->NewStringUTF((std::string(".;../JavApi/JavApi.jar;") +
			std::filesystem::path(filePath).parent_path().string() + ";").c_str()),
		env->NewStringUTF(filePath.c_str())
	};
	jobjectArray argArray = env->NewObjectArray(3, stringClass, args[2]);
	for (int i = 0; i < 3; i++)
		env->SetObjectArrayElement(argArray, i, args[i]);

	// Compilation
	if (env->CallIntMethod(compiler, compileMethod, NULL, NULL, NULL, argArray))
		Engine::log("An error occured compiling script \"" + filePath + "\"!");

	// Cleanup
	env->DeleteLocalRef(argArray);
	for (int i = 0; i < 3; i++)
		env->DeleteLocalRef(args[i]);
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
