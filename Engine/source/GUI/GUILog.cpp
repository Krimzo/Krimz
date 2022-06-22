#include "Logging/Logging.h"
#include "GUI.h"


static uint64 lastLogsSize = 0;

void Engine::GUI::LogRender() {
	const std::string titleAddon = Engine::Logging::logUnseenCount ?
		("[" + std::to_string(Engine::Logging::logUnseenCount) + "]###") : "###";
	if (ImGui::Begin(("Log" + titleAddon).c_str())) {
		Engine::Logging::logUnseenCount = 0;
		for (auto& log : Engine::Logging::logs) {
			ImGui::Text(log.c_str());
		}

		for (int i = 0; i < Engine::Logging::logs.size() - lastLogsSize; i++) {
			ImGui::Text("");
			ImGui::SetScrollHereY(1.0f);
		}
		lastLogsSize = Engine::Logging::logs.size();

		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::Button("Clear")) {
				Engine::Logging::logs.clear();
				Engine::Logging::logCounter = 0;
				lastLogsSize = 0;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::End();
	}

	if (Engine::Logging::logs.size() > 10000) {
		Engine::Logging::logs.clear();
		lastLogsSize = 0;
	}
}
