#pragma once

#include "Engine/Scripting/Scripting.h"


namespace Engine {
	class Script {
	private:
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
		jfieldID sizeField = nullptr;
		jfieldID rotationField = nullptr;
		jfieldID positionField = nullptr;
		jfieldID physicsField = nullptr;
		jfieldID accelerField = nullptr;
		jfieldID velocityField = nullptr;
		jfieldID angularField = nullptr;

	public:
		Script(jclass cls);

		// Data editors
		void setData(void* ent);
		void getData(void* ent);

		// Destroys the instnace
		void destroy();

		// Method callers
		void callStart(void* ent);
		void callUpdate(void* ent);
	};
}
