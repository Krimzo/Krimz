#include "Scripting/Scripting.h"
#include "Types/Entity.h"
#include "Render/Render.h"
#include "Logging/Logging.h"


void Krimz::Scripting::UpdateTime()
{
	JavaHandler::env->SetStaticFloatField(JavaHandler::timeClass, JavaHandler::deltaTField, Time::delta);
	JavaHandler::env->SetStaticFloatField(JavaHandler::timeClass, JavaHandler::elapsedTField, Time::elapsed);
}

void Krimz::Scripting::CallStarts()
{
	for (auto& ent : Krimz::entities)
	{
		ent->callStarts();
	}
}
void Krimz::Scripting::CallUpdates()
{
	for (auto& ent : Krimz::entities)
	{
		ent->callUpdates();
	}
}

void Krimz::Scripting::HandleLogs()
{
	jobjectArray logBuff = jobjectArray(Krimz::JavaHandler::env->CallStaticObjectMethod(
		Krimz::JavaHandler::loggerClass, Krimz::JavaHandler::loggerFlushMethod));

	const int logBuffSize = Krimz::JavaHandler::env->GetArrayLength(logBuff);
	for (int i = 0; i < logBuffSize; i++)
	{
		jstring logMess = jstring(Krimz::JavaHandler::env->GetObjectArrayElement(logBuff, i));
		Krimz::log(Krimz::JavaHandler::env->GetStringUTFChars(logMess, nullptr));
		Krimz::JavaHandler::env->DeleteLocalRef(logMess);
	}

	Krimz::JavaHandler::env->DeleteLocalRef(logBuff);
}

void SetScriptInt2(jobject field, const kl::int2& dat)
{
	static jclass objCls = Krimz::JavaHandler::env->GetObjectClass(field);
	static jfieldID xField = Krimz::JavaHandler::GetField(objCls, "x", "I");
	static jfieldID yField = Krimz::JavaHandler::GetField(objCls, "y", "I");
	Krimz::JavaHandler::env->SetIntField(field, xField, dat.x);
	Krimz::JavaHandler::env->SetIntField(field, yField, dat.y);
}
void GetScriptInt2(jobject field, kl::int2& dat)
{
	static jclass objCls = Krimz::JavaHandler::env->GetObjectClass(field);
	static jfieldID xField = Krimz::JavaHandler::GetField(objCls, "x", "I");
	static jfieldID yField = Krimz::JavaHandler::GetField(objCls, "y", "I");
	dat.x = Krimz::JavaHandler::env->GetIntField(field, xField);
	dat.y = Krimz::JavaHandler::env->GetIntField(field, yField);
}
void Krimz::Scripting::UpdateInput()
{
	JavaHandler::env->SetStaticBooleanField(JavaHandler::mouseClass, JavaHandler::lmbField, Krimz::window.mouse.lmb);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::mouseClass, JavaHandler::mmbField, Krimz::window.mouse.mmb);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::mouseClass, JavaHandler::rmbField, Krimz::window.mouse.rmb);
	SetScriptInt2(JavaHandler::env->GetStaticObjectField(JavaHandler::mouseClass, JavaHandler::moPosField), Krimz::window.mouse.position);
	JavaHandler::env->SetStaticIntField(JavaHandler::mouseClass, JavaHandler::scrollField, Krimz::window.mouse.scroll);

	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::qField, Krimz::window.keys.q);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::wField, Krimz::window.keys.w);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::eField, Krimz::window.keys.e);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::rField, Krimz::window.keys.r);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::tField, Krimz::window.keys.t);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::zField, Krimz::window.keys.z);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::uField, Krimz::window.keys.u);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::iField, Krimz::window.keys.i);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::oField, Krimz::window.keys.o);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::pField, Krimz::window.keys.p);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::aField, Krimz::window.keys.a);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::sField, Krimz::window.keys.s);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::dField, Krimz::window.keys.d);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::fField, Krimz::window.keys.f);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::gField, Krimz::window.keys.g);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::hField, Krimz::window.keys.h);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::jField, Krimz::window.keys.j);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::kField, Krimz::window.keys.k);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::lField, Krimz::window.keys.l);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::yField, Krimz::window.keys.y);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::xField, Krimz::window.keys.x);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::cField, Krimz::window.keys.c);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::vField, Krimz::window.keys.v);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::bField, Krimz::window.keys.b);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::nField, Krimz::window.keys.n);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::mField, Krimz::window.keys.m);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num0Field, Krimz::window.keys.num0);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num1Field, Krimz::window.keys.num1);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num2Field, Krimz::window.keys.num2);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num3Field, Krimz::window.keys.num3);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num4Field, Krimz::window.keys.num4);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num5Field, Krimz::window.keys.num5);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num6Field, Krimz::window.keys.num6);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num7Field, Krimz::window.keys.num7);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num8Field, Krimz::window.keys.num8);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::num9Field, Krimz::window.keys.num9);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::periodField, Krimz::window.keys.period);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::commaField, Krimz::window.keys.comma);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::plusField, Krimz::window.keys.plus);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::minusField, Krimz::window.keys.minus);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::escField, Krimz::window.keys.esc);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::tabField, Krimz::window.keys.tab);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::capsField, Krimz::window.keys.caps);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::shiftField, Krimz::window.keys.shift);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::ctrlField, Krimz::window.keys.ctrl);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::altField, Krimz::window.keys.alt);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::spaceField, Krimz::window.keys.space);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::enterField, Krimz::window.keys.enter);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::insertField, Krimz::window.keys.insert);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::deletField, Krimz::window.keys.delet);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::upField, Krimz::window.keys.up);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::leftField, Krimz::window.keys.left);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::downField, Krimz::window.keys.down);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::rightField, Krimz::window.keys.right);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f1Field, Krimz::window.keys.f1);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f2Field, Krimz::window.keys.f2);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f3Field, Krimz::window.keys.f3);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f4Field, Krimz::window.keys.f4);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f5Field, Krimz::window.keys.f5);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f6Field, Krimz::window.keys.f6);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f7Field, Krimz::window.keys.f7);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f8Field, Krimz::window.keys.f8);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f9Field, Krimz::window.keys.f9);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f10Field, Krimz::window.keys.f10);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f11Field, Krimz::window.keys.f11);
	JavaHandler::env->SetStaticBooleanField(JavaHandler::keysClass, JavaHandler::f12Field, Krimz::window.keys.f12);
}
void Krimz::Scripting::HandleMousePos()
{
	kl::int2 tempPos;
	GetScriptInt2(JavaHandler::env->GetStaticObjectField(JavaHandler::mouseClass, JavaHandler::moPosField), tempPos);
	if (tempPos != Krimz::window.mouse.position)
	{
		Krimz::window.mouse.move(tempPos);
	}
}
