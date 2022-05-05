#include "Scripting/Scripting.h"
#include "Types/Entity.h"
#include "Render/Render.h"
#include "Logging/Logging.h"


// Updates static time
void Engine::Scripting::UpdateTime() {
	JavaHandler::env->SetStaticFloatField(JavaHandler::timeClass, JavaHandler::deltaTField, Time::delta);
	JavaHandler::env->SetStaticFloatField(JavaHandler::timeClass, JavaHandler::elapsedTField, Time::elapsed);
}

// Entities scripts callers
void Engine::Scripting::CallStarts() {
	for (auto& ent : Engine::entities) {
		ent->callStarts();
	}
}
void Engine::Scripting::CallUpdates() {
	for (auto& ent : Engine::entities) {
		ent->callUpdates();
	}
}

// Script logging
void Engine::Scripting::HandleLogs() {
	// Calling the flush method
	jobjectArray logBuff = (jobjectArray)Engine::JavaHandler::env->CallStaticObjectMethod(
		Engine::JavaHandler::loggerClass, Engine::JavaHandler::loggerFlushMethod);

	// Getting the log messages
	const int logBuffSize = Engine::JavaHandler::env->GetArrayLength(logBuff);
	for (int i = 0; i < logBuffSize; i++) {
		jstring logMess = (jstring)Engine::JavaHandler::env->GetObjectArrayElement(logBuff, i);
		Engine::log(Engine::JavaHandler::env->GetStringUTFChars(logMess, nullptr));
		Engine::JavaHandler::env->DeleteLocalRef(logMess);
	}

	// Cleanup
	Engine::JavaHandler::env->DeleteLocalRef(logBuff);
}

// Input
void SetScriptInt2(jobject field, const kl::int2& dat) {
	jclass objCls = Engine::JavaHandler::env->GetObjectClass(field);
	jfieldID xField = Engine::JavaHandler::GetField(objCls, "x", "I");
	jfieldID yField = Engine::JavaHandler::GetField(objCls, "y", "I");
	Engine::JavaHandler::env->SetIntField(field, xField, dat.x);
	Engine::JavaHandler::env->SetIntField(field, yField, dat.y);
}
void GetScriptInt2(jobject field, kl::int2& dat) {
	jclass objCls = Engine::JavaHandler::env->GetObjectClass(field);
	jfieldID xField = Engine::JavaHandler::GetField(objCls, "x", "I");
	jfieldID yField = Engine::JavaHandler::GetField(objCls, "y", "I");
	dat.x = Engine::JavaHandler::env->GetIntField(field, xField);
	dat.y = Engine::JavaHandler::env->GetIntField(field, yField);
}
void Engine::Scripting::UpdateInput() {
	// Mouse
	JavaHandler::env->SetStaticBooleanField(JavaHandler::mouseClass, JavaHandler::lmbField, Engine::win.mouse.lmb);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::mouseClass, JavaHandler::mmbField, Engine::win.mouse.mmb);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::mouseClass, JavaHandler::rmbField, Engine::win.mouse.rmb);
	SetScriptInt2(JavaHandler::env->GetStaticObjectField(JavaHandler::mouseClass, JavaHandler::moPosField), Engine::win.mouse.position);
	JavaHandler::env->SetStaticIntField(JavaHandler::mouseClass, JavaHandler::scrollField, Engine::win.mouse.scroll);

	// Keyboard
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::qField, Engine::win.keys.q);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::wField, Engine::win.keys.w);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::eField, Engine::win.keys.e);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::rField, Engine::win.keys.r);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::tField, Engine::win.keys.t);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::zField, Engine::win.keys.z);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::uField, Engine::win.keys.u);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::iField, Engine::win.keys.i);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::oField, Engine::win.keys.o);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::pField, Engine::win.keys.p);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::aField, Engine::win.keys.a);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::sField, Engine::win.keys.s);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::dField, Engine::win.keys.d);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::fField, Engine::win.keys.f);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::gField, Engine::win.keys.g);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::hField, Engine::win.keys.h);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::jField, Engine::win.keys.j);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::kField, Engine::win.keys.k);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::lField, Engine::win.keys.l);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::yField, Engine::win.keys.y);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::xField, Engine::win.keys.x);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::cField, Engine::win.keys.c);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::vField, Engine::win.keys.v);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::bField, Engine::win.keys.b);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::nField, Engine::win.keys.n);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::mField, Engine::win.keys.m);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num0Field, Engine::win.keys.num0);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num1Field, Engine::win.keys.num1);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num2Field, Engine::win.keys.num2);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num3Field, Engine::win.keys.num3);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num4Field, Engine::win.keys.num4);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num5Field, Engine::win.keys.num5);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num6Field, Engine::win.keys.num6);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num7Field, Engine::win.keys.num7);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num8Field, Engine::win.keys.num8);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num9Field, Engine::win.keys.num9);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::periodField, Engine::win.keys.period);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::commaField, Engine::win.keys.comma);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::plusField, Engine::win.keys.plus);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::minusField, Engine::win.keys.minus);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::escField, Engine::win.keys.esc);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::tabField, Engine::win.keys.tab);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::capsField, Engine::win.keys.caps);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::shiftField, Engine::win.keys.shift);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::ctrlField, Engine::win.keys.ctrl);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::altField, Engine::win.keys.alt);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::spaceField, Engine::win.keys.space);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::enterField, Engine::win.keys.enter);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::insertField, Engine::win.keys.insert);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::deletField, Engine::win.keys.delet);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::upField, Engine::win.keys.up);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::leftField, Engine::win.keys.left);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::downField, Engine::win.keys.down);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::rightField, Engine::win.keys.right);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f1Field, Engine::win.keys.f1);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f2Field, Engine::win.keys.f2);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f3Field, Engine::win.keys.f3);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f4Field, Engine::win.keys.f4);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f5Field, Engine::win.keys.f5);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f6Field, Engine::win.keys.f6);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f7Field, Engine::win.keys.f7);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f8Field, Engine::win.keys.f8);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f9Field, Engine::win.keys.f9);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f10Field, Engine::win.keys.f10);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f11Field, Engine::win.keys.f11);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f12Field, Engine::win.keys.f12);
}
void Engine::Scripting::HandleMousePos() {
	kl::int2 tempPos;
	GetScriptInt2(JavaHandler::env->GetStaticObjectField(JavaHandler::mouseClass, JavaHandler::moPosField), tempPos);
	if (tempPos != Engine::win.mouse.position) {
		Engine::win.mouse.move(tempPos);
	}
}
