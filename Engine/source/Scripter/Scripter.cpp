#include "Scripter/Scripter.h"

#pragma comment(lib, "jvm.lib")


static JNIEnv* _envTransport = nullptr;
const JavaVM* Krimz::Scripter::JVM = [&]() {
	kl::console::error(!LoadLibraryA("../JavaDK/bin/server/jvm.dll"), "Failed to load jvm.dll");

	JavaVMInitArgs vmArgs = {};
	vmArgs.version = JNI_VERSION_10;

	std::string classPaths[] = { ".", "../ScriptApi/ScriptApi.jar" };
	std::stringstream jvmOptionsStream;
	jvmOptionsStream << "-Djava.class.path=";
	for (auto& path : classPaths) {
		jvmOptionsStream << path << ";";
	}
	std::string jvmOptionsData = jvmOptionsStream.str();

	JavaVMOption jvmOption = {};
	jvmOption.optionString = (char*) jvmOptionsData.c_str();
	vmArgs.options = &jvmOption;
	vmArgs.nOptions = 1;

	JavaVM* tempVM = nullptr;
	kl::console::error(JNI_CreateJavaVM(&tempVM, (void**) &_envTransport, &vmArgs), "Failed to create JVM");
	return tempVM;
}();
const JNIEnv* Krimz::Scripter::ENV = _envTransport;

Krimz::Scripter::Scripter() {}

Krimz::Scripter::~Scripter() {}
