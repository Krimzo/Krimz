#pragma once

#include <iostream>
#include <string>
#include <jni.h>


namespace Krimz
{
	class Script
	{
	public:
		std::string path;
		jobject inst;

		jmethodID startMethod = nullptr;
		jmethodID updateMethod = nullptr;

		Script(const std::string& filePath);
		Script(const Krimz::Script&) = delete;
		void operator=(const Krimz::Script&) = delete;
		~Script();

		void reload();

		void setEntityData(void* ent);
		void getEntityData(void* ent);

		void callStart(void* ent);
		void callUpdate(void* ent);
	};
}
