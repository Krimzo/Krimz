#include "Logging/Logging.h"
#include "GUI.h"


static uint64 lastLogsSize = 0;

void Krimz::GUI::LogRender() {
	const std::string titleAddon = Krimz::Logging::logUnseenCount ?
		("[" + std::to_string(Krimz::Logging::logUnseenCount) + "]###") : "###";
	if (ImGui::Begin(("Log" + titleAddon).c_str())) {
		Krimz::Logging::logUnseenCount = 0;
		for (auto& log : Krimz::Logging::logs) {
			ImGui::Text(log.c_str());
		}

		for (int i = 0; i < Krimz::Logging::logs.size() - lastLogsSize; i++) {
			ImGui::Text("");
			ImGui::SetScrollHereY(1.0f);
		}
		lastLogsSize = Krimz::Logging::logs.size();

		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::Button("Clear")) {
				Krimz::Logging::logs.clear();
				Krimz::Logging::logCounter = 0;
				lastLogsSize = 0;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::End();
	}

	if (Krimz::Logging::logs.size() > 10000) {
		Krimz::Logging::logs.clear();
		lastLogsSize = 0;
	}
}
