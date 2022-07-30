#pragma once


#include "KrimzLib.h"
#include "jni.h"


namespace Krimz {
	class Scripter {
	public:
		static const JavaVM* JVM;
		static const JNIEnv* ENV;

		Scripter();
		Scripter(const Scripter&) = delete;
		void operator=(const Scripter&) = delete;
		~Scripter();
	};
}
