#include "Engine/GUI/GUI.h"
#include "Engine/GUI/GUIStage.h"
#include "Engine/Time/Time.h"
#include "ImGui/implot.h"


std::vector<float> frameHistory(30);

double GetMaxVal() {
	double maxVal = -1.0;
	for (const auto& ref : frameHistory) {
		if (ref > maxVal) {
			maxVal = ref;
		}
	}
	return maxVal;
}

void Engine::GUI::Performance() {
	if (ImGui::Begin("Performance")) {
		// Saving info
		frameHistory.erase(frameHistory.begin());
		frameHistory.push_back(Engine::Time::delta);

		// Graph
		ImPlot::SetNextAxesLimits(-(frameHistory.size() - 1.0), 0.0, 0.0, max(GetMaxVal() * 2.0, 0.0167), ImPlotCond_Always);
		if (ImPlot::BeginPlot("Frame info", ImVec2(-1.0f, -1.0f))) {
			// Frame times
			ImPlot::SetNextLineStyle(ImVec4(0.7f, 0.4f, 0.0f, 1.0f));
			ImPlot::PlotLine("Frame times", &frameHistory[0], int(frameHistory.size()), 1.0, (frameHistory.size() - 1.0) * -1.0);

			// End graph
			ImPlot::EndPlot();
		}

		// End draw
		ImGui::End();
	}
}
