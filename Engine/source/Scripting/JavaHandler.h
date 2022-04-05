#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <jni.h>

#include "Scripting/Script.h"
#include "memory/pbuffer.h"

#pragma comment(lib, "jvm.lib")


namespace Engine
{
	namespace JavaHandler
	{
		// JNI pointers
		inline JavaVM* jvm = nullptr;
		inline JNIEnv* env = nullptr;

		// Class loader
		inline jclass loaderClass = nullptr;
		inline jmethodID loaderConstr = nullptr;
		inline jobject loader = nullptr;

		// Garbage collector
		inline jclass    sysClass = nullptr;
		inline jmethodID sysGCMethod = nullptr;

		// Script compiler
		inline jobject compiler = nullptr;
		inline jmethodID compileMethod = nullptr;

		// Engine classes/fields
		inline jclass engineClass = nullptr;
		inline jfieldID deltaTField = nullptr;
		inline jfieldID elapsedTField = nullptr;

		// Buffers
		inline std::vector<jobject> refs;
		inline kl::pbuffer<Engine::Script> scripts;

		// Setup
		void Init();
		void Uninit();

		// Clears all loaded references
		void ResetLoader();

		// Loads eternal class
		jclass LoadEternalClass(JNIEnv* env, const std::string& name);

		// Loads a new class from file
		jclass LoadClass(const std::string& name, const std::string& filePath);

		// Gets a class method
		jmethodID GetMethod(jclass cls, const std::string& name, const std::string& sig, bool isStatic = false);

		// Gets a class field
		jfieldID GetField(jclass cls, const std::string& name, const std::string& sig, bool isStatic = false);

		// Creates a new class instance
		jobject NewInst(jclass cls, jmethodID constr);

		// Deletes a class instance
		void DelInst(jobject obj);

		// Compiles given script
		void CompileFile(const std::string& filePath);

		// Creates a new script
		Engine::Script* NewScript(const std::string& filePath);

		// Deletes a script
		bool DelScript(Engine::Script* scr);

		// Reloads all scripts from files
		void ReloadScripts();
	};
}
