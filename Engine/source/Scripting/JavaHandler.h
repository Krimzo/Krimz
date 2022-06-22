#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <jni.h>

#include "KrimzLib.h"
#include "Scripting/Script.h"


namespace Engine {
	namespace JavaHandler {
		inline JavaVM* jvm = nullptr;
		inline JNIEnv* env = nullptr;

		inline jclass loaderClass = nullptr;
		inline jmethodID loaderConstr = nullptr;
		inline jobject loader = nullptr;

		inline jclass sysClass = nullptr;
		inline jmethodID sysGCMethod = nullptr;

		inline jclass stringClass = nullptr;
		inline jobject compiler = nullptr;
		inline jmethodID compileMethod = nullptr;

		inline jclass timeClass = nullptr;
		inline jfieldID deltaTField = nullptr;
		inline jfieldID elapsedTField = nullptr;

		inline jclass loggerClass = nullptr;
		inline jmethodID loggerFlushMethod = nullptr;

		inline jclass materialClass = nullptr;
		inline jfieldID roughnessField = nullptr;
		inline jfieldID colorMapField = nullptr;
		inline jfieldID normalMapField = nullptr;
		inline jfieldID roughnessMapField = nullptr;

		inline jclass colliderClass = nullptr;
		inline jfieldID collScaleField = nullptr;
		inline jfieldID collRotationField = nullptr;
		inline jfieldID collPositionField = nullptr;
		inline jfieldID collShapeField = nullptr;

		inline jclass entityClass = nullptr;
		inline jfieldID nameField = nullptr;
		inline jfieldID visibleField = nullptr;
		inline jfieldID shadowsField = nullptr;
		inline jfieldID materialField = nullptr;
		inline jfieldID scaleField = nullptr;
		inline jfieldID rotationField = nullptr;
		inline jfieldID positionField = nullptr;
		inline jfieldID meshField = nullptr;
		inline jfieldID dynamicField = nullptr;
		inline jfieldID gravityField = nullptr;
		inline jfieldID frictionField = nullptr;
		inline jfieldID massField = nullptr;
		inline jfieldID velocityField = nullptr;
		inline jfieldID angularField = nullptr;
		inline jfieldID colliderField = nullptr;

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

		inline std::set<jobject> refs;
		inline std::map<std::string, jclass> classes;

		void Init();
		void Uninit();

		void ResetLoader();

		jclass LoadEternalClass(const std::string& name);

		jclass LoadClass(const std::string& filePath);

		jmethodID GetMethod(jclass cls, const std::string& name, const std::string& sig, bool isStatic = false);

		jfieldID GetField(jclass cls, const std::string& name, const std::string& sig, bool isStatic = false);

		jobject NewInst(jclass cls, jmethodID constr);

		void DelInst(jobject obj);

		void CompileFile(const std::string& filePath);

		void ReloadScripts();
	};
}
