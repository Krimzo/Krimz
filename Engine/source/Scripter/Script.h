#pragma once

#include "Scripter/Scripter.h"


namespace Krimz {
	class Script {
		std::string m_Path = {};
		jobject m_Instance = nullptr;

		jmethodID m_StartMethod = nullptr;
		jmethodID m_UpdateMethod = nullptr;

	public:
		Script(const std::string& filePath);
		Script(const Krimz::Script&) = delete;
		void operator=(const Krimz::Script&) = delete;
		~Script();

		//void reload();

		//void setEntityData(void* ent);
		//void getEntityData(void* ent);

		//void start(void* ent);
		//void update(void* ent);
	};
}
