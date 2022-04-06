#pragma once

#include <iostream>
#include <jni.h>


namespace Engine
{
	class Script
	{
	public:
		// File info
		std::string path;

		// Instance
		jobject inst = nullptr;

		// Methods
		jmethodID startMethod = nullptr;
		jmethodID updateMethod = nullptr;

		// Fields
		jfieldID nameField = nullptr;
		jfieldID visibleField = nullptr;
		jfieldID shadowsField = nullptr;
		jfieldID roughnessField = nullptr;
		jfieldID scaleField = nullptr;
		jfieldID rotationField = nullptr;
		jfieldID positionField = nullptr;
		jfieldID physicsField = nullptr;
		jfieldID accelerField = nullptr;
		jfieldID velocityField = nullptr;
		jfieldID angularField = nullptr;

		Script(const std::string& filePath);

		// Reloads bytes
		void reload();

		// Data editors
		void setEntityData(void* ent);
		void getEntityData(void* ent);

		// Method callers
		void callStart(void* ent);
		void callUpdate(void* ent);
	};
}
