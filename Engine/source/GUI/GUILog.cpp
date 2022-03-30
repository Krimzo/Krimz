#include "Logging/Logging.h"
#include "GUI.h"


size_t lastLogsSize = 0;
void Engine::GUI::Log() {
	if (ImGui::Begin("Log")) {
		// Drawing output
		for (auto& log : Engine::Logging::logs) {
			ImGui::Text(log.c_str());
		}

		// Output align
		for (int i = 0; i < Engine::Logging::logs.size() - lastLogsSize; i++) {
			ImGui::Text("");
			ImGui::SetScrollHereY(1.0f);
		}
		lastLogsSize = Engine::Logging::logs.size();

		// RMB
		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::Button("Clear")) {
				Engine::Logging::logs.clear();
				lastLogsSize = 0;
				ImGui::CloseCurrentPopup();
			}

			// End
			ImGui::EndPopup();
		}

		// End draw
		ImGui::End();
	}

	// Cleanup
	if (Engine::Logging::logs.size() > 10000) {
		Engine::Logging::logs.clear();
		lastLogsSize = 0;
	}
}
