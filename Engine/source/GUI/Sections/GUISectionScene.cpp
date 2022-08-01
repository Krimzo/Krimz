#include "GUI/Sections/GUISections.h"


static bool Start() {
	return false;
}

static void Render() {

}

static void End() {

}

kl::ref<Krimz::GUISection> Krimz::GUISections::Scene() {
	kl::ref<GUISection> section = kl::make<GUISection>();
	section->start = Start;
	section->render = Render;
	section->end = End;
	return section;
}
