#include "GUI/Sections/GUISections.h"


void Krimz::LoadGuiSections(GUIRenderer& renderer) {
	renderer.add(GUISections::MainMenu());
	renderer.add(GUISections::Scene());
	renderer.add(GUISections::Viewport());
	renderer.add(GUISections::Explorer());
	renderer.add(GUISections::LogView());
	renderer.add(GUISections::Performance());
	renderer.add(GUISections::Properties());
}
