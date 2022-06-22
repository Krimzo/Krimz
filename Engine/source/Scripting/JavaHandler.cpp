#include "Scripting/Scripting.h"
#include "Logging/Logging.h"
#include "Types/Entity.h"

#pragma comment(lib, "jvm.lib")


void Engine::JavaHandler::Init() {
	kl::console::error(!LoadLibraryA("../OpenJDK/bin/server/jvm.dll"), "Failed to load java dll");

	JavaVMInitArgs vmArgs = {};
	vmArgs.version = JNI_VERSION_10;
	JavaVMOption jvmOptions[1] = {};
	jvmOptions[0].optionString = (char*)"-Djava.class.path=.;../JavApi/JavApi.jar;";
	vmArgs.options = jvmOptions;
	vmArgs.nOptions = 1;

	kl::console::error(JNI_CreateJavaVM(&jvm, (void**)&env, &vmArgs) != JNI_OK, "Failed to create JVM");

	sysClass = LoadEternalClass("java/lang/System");
	sysGCMethod = GetMethod(sysClass, "gc", "()V", true);

	stringClass = env->FindClass("Ljava/lang/String;");
	jclass providerClass = LoadEternalClass("javax/tools/ToolProvider");
	jclass compilerClass = LoadEternalClass("javax/tools/JavaCompiler");
	jmethodID providerMethod = GetMethod(providerClass, "getSystemJavaCompiler", "()Ljavax/tools/JavaCompiler;", true);
	compiler = env->CallStaticObjectMethod(providerClass, providerMethod);
	compileMethod = GetMethod(compilerClass, "run", "(Ljava/io/InputStream;Ljava/io/OutputStream;Ljava/io/OutputStream;[Ljava/lang/String;)I");

	loaderClass = LoadEternalClass("engine/script/Loader");

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

	timeClass = LoadEternalClass("engine/Time");
	deltaTField = GetField(timeClass, "deltaT", "F", true);
	elapsedTField = GetField(timeClass, "elapsedT", "F", true);

	mouseClass = LoadEternalClass("engine/input/Mouse");
	lmbField = GetField(mouseClass, "lmb", "Z", true);
	mmbField = GetField(mouseClass, "mmb", "Z", true);
	rmbField = GetField(mouseClass, "rmb", "Z", true);
	moPosField = GetField(mouseClass, "position", "Lengine/math/Int2;", true);
	scrollField = GetField(mouseClass, "scroll", "I", true);

	keysClass = LoadEternalClass("engine/input/Keys");
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

	LoadEternalClass("engine/script/Script");

	loggerClass = LoadEternalClass("engine/Logger");
	loggerFlushMethod = GetMethod(loggerClass, "flush", "()[Ljava/lang/String;", true);
	loaderConstr = GetMethod(loaderClass, "<init>", "()V");

	materialClass = LoadEternalClass("engine/entity/Material");
	roughnessField = GetField(materialClass, "roughness", "F");
	colorMapField = GetField(materialClass, "colorMap", "Ljava/lang/String;");
	normalMapField = GetField(materialClass, "normalMap", "Ljava/lang/String;");
	roughnessMapField = GetField(materialClass, "roughnessMap", "Ljava/lang/String;");

	colliderClass = LoadEternalClass("engine/entity/Collider");
	collScaleField = GetField(colliderClass, "scale", "Lengine/math/Float3;");
	collRotationField = GetField(colliderClass, "rotation", "Lengine/math/Float3;");
	collPositionField = GetField(colliderClass, "position", "Lengine/math/Float3;");
	collShapeField = GetField(colliderClass, "shape", "I");

	entityClass = LoadEternalClass("engine/entity/Entity");
	nameField = GetField(entityClass, "name", "Ljava/lang/String;");
	visibleField = GetField(entityClass, "visible", "Z");
	shadowsField = GetField(entityClass, "shadows", "Z");
	materialField = GetField(entityClass, "material", "Lengine/entity/Material;");
	scaleField = GetField(entityClass, "scale", "Lengine/math/Float3;");
	rotationField = GetField(entityClass, "rotation", "Lengine/math/Float3;");
	positionField = GetField(entityClass, "position", "Lengine/math/Float3;");
	meshField = GetField(entityClass, "mesh", "Ljava/lang/String;");
	dynamicField = GetField(entityClass, "dynamic", "Z");
	gravityField = GetField(entityClass, "gravity", "Z");
	frictionField = GetField(entityClass, "friction", "F");
	massField = GetField(entityClass, "mass", "F");
	velocityField = GetField(entityClass, "velocity", "Lengine/math/Float3;");
	angularField = GetField(entityClass, "angular", "Lengine/math/Float3;");
	colliderField = GetField(entityClass, "collider", "Lengine/entity/Collider;");

	loader = env->NewObject(loaderClass, loaderConstr);
	kl::console::error(!loader, "Failed to create class loader");

	env->CallStaticVoidMethod(sysClass, sysGCMethod);
}
void Engine::JavaHandler::Uninit() {
	refs.clear();
	classes.clear();
	jvm->DestroyJavaVM();
	jvm = nullptr;
	env = nullptr;
}

void Engine::JavaHandler::ResetLoader() {
	for (auto& ref : refs) {
		env->DeleteLocalRef(ref);
	}
	refs.clear();
	for (auto& cls : classes) {
		env->DeleteLocalRef(cls.second);
	}
	classes.clear();

	env->DeleteLocalRef(loader);

	env->CallStaticVoidMethod(sysClass, sysGCMethod);

	loader = env->NewObject(loaderClass, loaderConstr);
	kl::console::error(!loader, "Failed to create class loader");
}

jclass Engine::JavaHandler::LoadEternalClass(const std::string& name) {
	jclass loaded = env->FindClass(name.c_str());
	if (!loaded) {
		Engine::log("Could not load eternal class \"" + name + "\"!");
	}
	return loaded;
}

jclass Engine::JavaHandler::LoadClass(const std::string& filePath) {
	CompileFile(filePath);
	const std::string classPath = std::filesystem::path(filePath).replace_extension("class").string();
	const std::vector<byte> clsBytes = kl::file::readB(classPath);

	const std::string parentPath = std::filesystem::path(filePath).parent_path().string();
	for (const auto& file : std::filesystem::recursive_directory_iterator(parentPath)) {
		if (std::filesystem::path(file).extension().string() == ".class") {
			std::filesystem::remove(file);
		}
	}
	if (!clsBytes.size()) {
		return nullptr;
	}

	jclass clsDef = env->DefineClass(nullptr, loader, (jbyte*)&clsBytes[0], jsize(clsBytes.size()));
	if (!clsDef) {
		if (classes.contains(filePath)) {
			return classes[filePath];
		}

		Engine::log("Could not define class \"" + filePath + "\"!");
		return nullptr;
	}

	classes[filePath] = clsDef;
	return clsDef;
}

jmethodID Engine::JavaHandler::GetMethod(jclass cls, const std::string& name, const std::string& sig, bool isStatic) {
	jmethodID methodID = isStatic ? env->GetStaticMethodID(cls, name.c_str(), sig.c_str()) : env->GetMethodID(cls, name.c_str(), sig.c_str());
	if (!methodID) {
		Engine::log("Could not get method \"" + name + "\"!");
	}
	return methodID;
}

jfieldID Engine::JavaHandler::GetField(jclass cls, const std::string& name, const std::string& sig, bool isStatic) {
	jfieldID fieldID = isStatic ? env->GetStaticFieldID(cls, name.c_str(), sig.c_str()) : env->GetFieldID(cls, name.c_str(), sig.c_str());
	if (!fieldID) {
		Engine::log("Could not get field \"" + name + "\"!");
	}
	return fieldID;
}

jobject Engine::JavaHandler::NewInst(jclass cls, jmethodID constr) {
	jobject obj = env->NewObject(cls, constr);
	if (!obj) {
		Engine::log("Could not create a new class instance!");
	}
	else {
		refs.insert(obj);
	}
	return obj;
}

void Engine::JavaHandler::DelInst(jobject obj) {
	if (refs.contains(obj)) {
		if (env) {
			env->DeleteLocalRef(obj);
		}
		refs.erase(obj);
	}
}

void Engine::JavaHandler::CompileFile(const std::string& filePath) {
	jstring args[3] = {
		env->NewStringUTF("-cp"),
		env->NewStringUTF((std::string(".;../JavApi/JavApi.jar;") + std::filesystem::path(filePath).parent_path().string() + ";").c_str()),
		env->NewStringUTF(filePath.c_str())
	};
	jobjectArray argArray = env->NewObjectArray(3, stringClass, args[2]);
	for (int i = 0; i < 3; i++) {
		env->SetObjectArrayElement(argArray, i, args[i]);
	}

	if (env->CallIntMethod(compiler, compileMethod, NULL, NULL, NULL, argArray)) {
		Engine::log("An error occured compiling script \"" + filePath + "\"!");
	}

	env->DeleteLocalRef(argArray);
	for (auto& arg : args) {
		env->DeleteLocalRef(arg);
	}
}

void Engine::JavaHandler::ReloadScripts() {
	ResetLoader();
	for (auto& ent : Engine::entities) {
		for (auto& scr : ent->scripts) {
			scr->reload();
		}
	}
}
