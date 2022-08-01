#include "GUI/GUIRenderer.h"


Krimz::GUIRenderer::GUIRenderer() {
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGui::StyleColorsDark();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	loadKrimzTheme();
}

Krimz::GUIRenderer::~GUIRenderer() {
	if (m_DirectXInitalized) {
		ImGui_ImplDX11_Shutdown();
	}
	if (m_WinApiInitialized) {
		ImGui_ImplWin32_Shutdown();
	}
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

void Krimz::GUIRenderer::initialize(kl::window& window) {
	ImGui_ImplWin32_Init(window);
	m_WinApiInitialized = true;
}
void Krimz::GUIRenderer::initialize(kl::gpu& gpu) {
	ImGui_ImplDX11_Init(gpu.dev(), gpu.con());
	m_DirectXInitalized = true;
}

void Krimz::GUIRenderer::loadKrimzTheme() {
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowPadding = ImVec2(15.0f, 15.0f);
	style.WindowRounding = 2.0f;
	style.FramePadding = ImVec2(5.0f, 5.0f);
	style.FrameRounding = 2.0f;
	style.ItemSpacing = ImVec2(12.0f, 8.0f);
	style.ItemInnerSpacing = ImVec2(8.0f, 6.0f);
	style.SelectableTextAlign = ImVec2(0.5f, 0.5f);
	style.IndentSpacing = 25.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 5.0f;
	style.GrabRounding = 3.0f;
	style.PopupBorderSize = 1.0f;
	style.PopupRounding = 5.0f;
	style.ChildBorderSize = 1.0f;
	style.ChildRounding = 5.0f;

	const ImVec4 colorNone = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	const ImVec4 colorDark = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
	const ImVec4 colorMid = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	const ImVec4 colorLight = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	const ImVec4 colorSpec = ImVec4(0.7f, 0.4f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = colorLight;
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.077f, 0.077f, 0.077f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = colorDark;
	style.Colors[ImGuiCol_PopupBg] = colorDark;
	style.Colors[ImGuiCol_Border] = colorLight;
	style.Colors[ImGuiCol_BorderShadow] = colorMid;
	style.Colors[ImGuiCol_FrameBg] = colorDark;
	style.Colors[ImGuiCol_FrameBgHovered] = colorMid;
	style.Colors[ImGuiCol_FrameBgActive] = colorLight;
	style.Colors[ImGuiCol_TitleBg] = colorDark;
	style.Colors[ImGuiCol_TitleBgActive] = colorDark;
	style.Colors[ImGuiCol_TitleBgCollapsed] = colorDark;
	style.Colors[ImGuiCol_MenuBarBg] = colorDark;
	style.Colors[ImGuiCol_ScrollbarBg] = colorDark;
	style.Colors[ImGuiCol_ScrollbarGrab] = colorLight;
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = colorMid;
	style.Colors[ImGuiCol_ScrollbarGrabActive] = colorLight;
	style.Colors[ImGuiCol_CheckMark] = colorSpec;
	style.Colors[ImGuiCol_SliderGrab] = colorSpec;
	style.Colors[ImGuiCol_SliderGrabActive] = colorSpec;
	style.Colors[ImGuiCol_Button] = colorMid;
	style.Colors[ImGuiCol_ButtonHovered] = colorLight;
	style.Colors[ImGuiCol_ButtonActive] = colorLight;
	style.Colors[ImGuiCol_Header] = colorMid;
	style.Colors[ImGuiCol_HeaderHovered] = colorLight;
	style.Colors[ImGuiCol_HeaderActive] = colorSpec;
	style.Colors[ImGuiCol_Separator] = colorMid;
	style.Colors[ImGuiCol_SeparatorHovered] = colorLight;
	style.Colors[ImGuiCol_SeparatorActive] = colorSpec;
	style.Colors[ImGuiCol_ResizeGrip] = colorMid;
	style.Colors[ImGuiCol_ResizeGripHovered] = colorLight;
	style.Colors[ImGuiCol_ResizeGripActive] = colorSpec;
	style.Colors[ImGuiCol_Tab] = colorMid;
	style.Colors[ImGuiCol_TabHovered] = colorSpec;
	style.Colors[ImGuiCol_TabActive] = colorSpec;
	style.Colors[ImGuiCol_TabUnfocused] = colorMid;
	style.Colors[ImGuiCol_TabUnfocusedActive] = colorLight;
	style.Colors[ImGuiCol_DockingPreview] = colorSpec;
	style.Colors[ImGuiCol_DockingEmptyBg] = colorMid;
	style.Colors[ImGuiCol_PlotLines] = colorSpec;
	style.Colors[ImGuiCol_PlotLinesHovered] = colorLight;
	style.Colors[ImGuiCol_PlotHistogram] = colorSpec;
	style.Colors[ImGuiCol_PlotHistogramHovered] = colorLight;
	style.Colors[ImGuiCol_TableHeaderBg] = colorMid;
	style.Colors[ImGuiCol_TableBorderStrong] = colorDark;
	style.Colors[ImGuiCol_TableBorderLight] = colorLight;
	style.Colors[ImGuiCol_TableRowBg] = colorDark;
	style.Colors[ImGuiCol_TableRowBgAlt] = colorMid;
	style.Colors[ImGuiCol_TextSelectedBg] = colorLight;
	style.Colors[ImGuiCol_DragDropTarget] = colorSpec;
	style.Colors[ImGuiCol_NavHighlight] = colorSpec;
	style.Colors[ImGuiCol_NavWindowingHighlight] = colorSpec;
	style.Colors[ImGuiCol_NavWindowingDimBg] = colorMid;
	style.Colors[ImGuiCol_ModalWindowDimBg] = colorMid;

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("resource/fonts/Balsamiq.ttf", 15);
}

void Krimz::GUIRenderer::add(kl::ref<GUIRenderable> renderable) {
	m_Renderables.push_back(renderable);
}

void Krimz::GUIRenderer::remove(kl::ref<GUIRenderable> renderable) {
	for (auto index = m_Renderables.begin(); index != m_Renderables.end(); index++) {
		if (*index == renderable) {
			m_Renderables.erase(index--);
		}
	}
}

void Krimz::GUIRenderer::clear() {
	m_Renderables.clear();
}

void Krimz::GUIRenderer::render() {
	if (m_WinApiInitialized && m_DirectXInitalized) {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		for (auto& renderable : m_Renderables) {
			renderable->gui_render();
		}

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}
