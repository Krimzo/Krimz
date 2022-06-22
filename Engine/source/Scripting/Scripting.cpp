#include "Scripting/Scripting.h"
#include "Types/Entity.h"
#include "Render/Render.h"
#include "Logging/Logging.h"


void Engine::Scripting::UpdateTime() {
	JavaHandler::env->SetStaticFloatField(JavaHandler::timeClass, JavaHandler::deltaTField, Time::delta);
	JavaHandler::env->SetStaticFloatField(JavaHandler::timeClass, JavaHandler::elapsedTField, Time::elapsed);
}

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

void Engine::Scripting::HandleLogs() {
	jobjectArray logBuff = (jobjectArray)Engine::JavaHandler::env->CallStaticObjectMethod(
		Engine::JavaHandler::loggerClass, Engine::JavaHandler::loggerFlushMethod);

	const int logBuffSize = Engine::JavaHandler::env->GetArrayLength(logBuff);
	for (int i = 0; i < logBuffSize; i++) {
		jstring logMess = (jstring)Engine::JavaHandler::env->GetObjectArrayElement(logBuff, i);
		Engine::log(Engine::JavaHandler::env->GetStringUTFChars(logMess, nullptr));
		Engine::JavaHandler::env->DeleteLocalRef(logMess);
	}

	Engine::JavaHandler::env->DeleteLocalRef(logBuff);
}

void SetScriptInt2(jobject field, const kl::int2& dat) {
	static jclass objCls = Engine::JavaHandler::env->GetObjectClass(field);
	static jfieldID xField = Engine::JavaHandler::GetField(objCls, "x", "I");
	static jfieldID yField = Engine::JavaHandler::GetField(objCls, "y", "I");
	Engine::JavaHandler::env->SetIntField(field, xField, dat.x);
	Engine::JavaHandler::env->SetIntField(field, yField, dat.y);
}
void GetScriptInt2(jobject field, kl::int2& dat) {
	static jclass objCls = Engine::JavaHandler::env->GetObjectClass(field);
	static jfieldID xField = Engine::JavaHandler::GetField(objCls, "x", "I");
	static jfieldID yField = Engine::JavaHandler::GetField(objCls, "y", "I");
	dat.x = Engine::JavaHandler::env->GetIntField(field, xField);
	dat.y = Engine::JavaHandler::env->GetIntField(field, yField);
}
void Engine::Scripting::UpdateInput() {
	JavaHandler::env->SetStaticBooleanField(JavaHandler::mouseClass, JavaHandler::lmbField, Engine::window.mouse.lmb);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::mouseClass, JavaHandler::mmbField, Engine::window.mouse.mmb);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::mouseClass, JavaHandler::rmbField, Engine::window.mouse.rmb);
	SetScriptInt2(JavaHandler::env->GetStaticObjectField(JavaHandler::mouseClass, JavaHandler::moPosField), Engine::window.mouse.position);
	JavaHandler::env->SetStaticIntField(JavaHandler::mouseClass, JavaHandler::scrollField, Engine::window.mouse.scroll);

	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::qField, Engine::window.keys.q);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::wField, Engine::window.keys.w);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::eField, Engine::window.keys.e);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::rField, Engine::window.keys.r);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::tField, Engine::window.keys.t);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::zField, Engine::window.keys.z);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::uField, Engine::window.keys.u);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::iField, Engine::window.keys.i);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::oField, Engine::window.keys.o);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::pField, Engine::window.keys.p);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::aField, Engine::window.keys.a);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::sField, Engine::window.keys.s);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::dField, Engine::window.keys.d);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::fField, Engine::window.keys.f);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::gField, Engine::window.keys.g);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::hField, Engine::window.keys.h);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::jField, Engine::window.keys.j);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::kField, Engine::window.keys.k);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::lField, Engine::window.keys.l);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::yField, Engine::window.keys.y);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::xField, Engine::window.keys.x);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::cField, Engine::window.keys.c);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::vField, Engine::window.keys.v);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::bField, Engine::window.keys.b);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::nField, Engine::window.keys.n);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::mField, Engine::window.keys.m);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num0Field, Engine::window.keys.num0);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num1Field, Engine::window.keys.num1);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num2Field, Engine::window.keys.num2);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num3Field, Engine::window.keys.num3);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num4Field, Engine::window.keys.num4);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num5Field, Engine::window.keys.num5);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num6Field, Engine::window.keys.num6);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num7Field, Engine::window.keys.num7);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num8Field, Engine::window.keys.num8);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num9Field, Engine::window.keys.num9);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::periodField, Engine::window.keys.period);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::commaField, Engine::window.keys.comma);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::plusField, Engine::window.keys.plus);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::minusField, Engine::window.keys.minus);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::escField, Engine::window.keys.esc);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::tabField, Engine::window.keys.tab);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::capsField, Engine::window.keys.caps);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::shiftField, Engine::window.keys.shift);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::ctrlField, Engine::window.keys.ctrl);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::altField, Engine::window.keys.alt);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::spaceField, Engine::window.keys.space);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::enterField, Engine::window.keys.enter);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::insertField, Engine::window.keys.insert);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::deletField, Engine::window.keys.delet);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::upField, Engine::window.keys.up);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::leftField, Engine::window.keys.left);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::downField, Engine::window.keys.down);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::rightField, Engine::window.keys.right);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f1Field, Engine::window.keys.f1);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f2Field, Engine::window.keys.f2);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f3Field, Engine::window.keys.f3);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f4Field, Engine::window.keys.f4);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f5Field, Engine::window.keys.f5);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f6Field, Engine::window.keys.f6);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f7Field, Engine::window.keys.f7);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f8Field, Engine::window.keys.f8);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f9Field, Engine::window.keys.f9);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f10Field, Engine::window.keys.f10);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f11Field, Engine::window.keys.f11);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f12Field, Engine::window.keys.f12);
}
void Engine::Scripting::HandleMousePos() {
	kl::int2 tempPos;
	GetScriptInt2(JavaHandler::env->GetStaticObjectField(JavaHandler::mouseClass, JavaHandler::moPosField), tempPos);
	if (tempPos != Engine::window.mouse.position) {
		Engine::window.mouse.move(tempPos);
	}
}
