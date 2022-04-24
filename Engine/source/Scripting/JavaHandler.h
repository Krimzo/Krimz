#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <jni.h>

#include "KrimzLib.h"
#include "Scripting/JavaClass.h"
#include "Scripting/Script.h"

#pragma comment(lib, "jvm.lib")


namespace Engine {
	namespace JavaHandler {
		// JNI pointers
		inline JavaVM* jvm = nullptr;
		inline JNIEnv* env = nullptr;

		// Class loader
		inline jclass loaderClass = nullptr;
		inline jmethodID loaderConstr = nullptr;
		inline jobject loader = nullptr;

		// Garbage collector
		inline jclass sysClass = nullptr;
		inline jmethodID sysGCMethod = nullptr;

		// Script compiler
		inline jclass stringClass = nullptr;
		inline jobject compiler = nullptr;
		inline jmethodID compileMethod = nullptr;

		// Time classe/fields
		inline jclass timeClass = nullptr;
		inline jfieldID deltaTField = nullptr;
		inline jfieldID elapsedTField = nullptr;

		// Script logging
		inline jclass loggerClass = nullptr;
		inline jmethodID loggerFlushMethod = nullptr;

		// Collider
		inline jclass colliderClass = nullptr;
		inline jfieldID collScaleField = nullptr;
		inline jfieldID collRotationField = nullptr;
		inline jfieldID collPositionField = nullptr;
		inline jfieldID collShapeField = nullptr;

		// Entity
		inline jclass entityClass = nullptr;
		inline jfieldID nameField = nullptr;
		inline jfieldID visibleField = nullptr;
		inline jfieldID shadowsField = nullptr;
		inline jfieldID roughnessField = nullptr;
		inline jfieldID scaleField = nullptr;
		inline jfieldID rotationField = nullptr;
		inline jfieldID positionField = nullptr;
		inline jfieldID dynamicField = nullptr;
		inline jfieldID gravityField = nullptr;
		inline jfieldID frictionField = nullptr;
		inline jfieldID massField = nullptr;
		inline jfieldID velocityField = nullptr;
		inline jfieldID angularField = nullptr;
		inline jfieldID colliderField = nullptr;

		// Input class/fields
		inline jclass mouseClass = nullptr;
		inline jfieldID lmbField = nullptr;
		inline jfieldID mmbField = nullptr;
		inline jfieldID rmbField = nullptr;
		inline jfieldID moPosField = nullptr;
		inline jfieldID scrollField = nullptr;
		inline jclass keysClass = nullptr;
		inline jfieldID qField = nullptr;
		inline jfieldID wField = nullptr;
		inline jfieldID eField = nullptr;
		inline jfieldID rField = nullptr;
		inline jfieldID tField = nullptr;
		inline jfieldID zField = nullptr;
		inline jfieldID uField = nullptr;
		inline jfieldID iField = nullptr;
		inline jfieldID oField = nullptr;
		inline jfieldID pField = nullptr;
		inline jfieldID aField = nullptr;
		inline jfieldID sField = nullptr;
		inline jfieldID dField = nullptr;
		inline jfieldID fField = nullptr;
		inline jfieldID gField = nullptr;
		inline jfieldID hField = nullptr;
		inline jfieldID jField = nullptr;
		inline jfieldID kField = nullptr;
		inline jfieldID lField = nullptr;
		inline jfieldID yField = nullptr;
		inline jfieldID xField = nullptr;
		inline jfieldID cField = nullptr;
		inline jfieldID vField = nullptr;
		inline jfieldID bField = nullptr;
		inline jfieldID nField = nullptr;
		inline jfieldID mField = nullptr;
		inline jfieldID num0Field = nullptr;
		inline jfieldID num1Field = nullptr;
		inline jfieldID num2Field = nullptr;
		inline jfieldID num3Field = nullptr;
		inline jfieldID num4Field = nullptr;
		inline jfieldID num5Field = nullptr;
		inline jfieldID num6Field = nullptr;
		inline jfieldID num7Field = nullptr;
		inline jfieldID num8Field = nullptr;
		inline jfieldID num9Field = nullptr;
		inline jfieldID periodField = nullptr;
		inline jfieldID commaField = nullptr;
		inline jfieldID plusField = nullptr;
		inline jfieldID minusField = nullptr;
		inline jfieldID escField = nullptr;
		inline jfieldID tabField = nullptr;
		inline jfieldID capsField = nullptr;
		inline jfieldID shiftField = nullptr;
		inline jfieldID ctrlField = nullptr;
		inline jfieldID altField = nullptr;
		inline jfieldID spaceField = nullptr;
		inline jfieldID enterField = nullptr;
		inline jfieldID insertField = nullptr;
		inline jfieldID deletField = nullptr;
		inline jfieldID upField = nullptr;
		inline jfieldID leftField = nullptr;
		inline jfieldID downField = nullptr;
		inline jfieldID rightField = nullptr;
		inline jfieldID f1Field = nullptr;
		inline jfieldID f2Field = nullptr;
		inline jfieldID f3Field = nullptr;
		inline jfieldID f4Field = nullptr;
		inline jfieldID f5Field = nullptr;
		inline jfieldID f6Field = nullptr;
		inline jfieldID f7Field = nullptr;
		inline jfieldID f8Field = nullptr;
		inline jfieldID f9Field = nullptr;
		inline jfieldID f10Field = nullptr;
		inline jfieldID f11Field = nullptr;
		inline jfieldID f12Field = nullptr;

		// Buffers
		inline std::vector<jobject> refs;
		inline std::vector<Engine::JavaClass> classes;

		// Setup
		void Init();
		void Uninit();

		// Clears all loaded references
		void ResetLoader();

		// Loads eternal class
		jclass LoadEternalClass(const String& name);

		// Loads a new class from file
		jclass LoadClass(const String& filePath);

		// Gets a class method
		jmethodID GetMethod(jclass cls, const String& name, const String& sig, bool isStatic = false);

		// Gets a class field
		jfieldID GetField(jclass cls, const String& name, const String& sig, bool isStatic = false);

		// Creates a new class instance
		jobject NewInst(jclass cls, jmethodID constr);

		// Deletes a class instance
		void DelInst(jobject obj);

		// Compiles given script
		void CompileFile(const String& filePath);

		// Reloads all scripts from files
		void ReloadScripts();
	};
}
