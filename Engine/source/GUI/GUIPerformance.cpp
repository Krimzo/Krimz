#include "GUI/GUI.h"
#include "Render/Render.h"


static std::vector<float> frameHistory(30);

double GetMaxVal()
{
	double maxVal = -1.0;
	for (auto& ref : frameHistory)
	{
		if (ref > maxVal)
		{
			maxVal = ref;
		}
	}
	return maxVal;
}

void Krimz::GUI::PerformanceRender()
{
	if (ImGui::Begin("Performance"))
	{
		frameHistory.erase(frameHistory.begin());
		frameHistory.push_back(Krimz::Time::delta);
		ImPlot::SetNextAxesLimits(-(frameHistory.size() - 1.0), 0.0, 0.0, max(GetMaxVal() * 2.0, 0.0167), ImPlotCond_Always);
		if (ImPlot::BeginPlot("Frame info", ImVec2(-1.0f, -1.0f)))
		{
			ImPlot::SetNextLineStyle(ImVec4(0.7f, 0.4f, 0.0f, 1.0f));
			ImPlot::PlotLine("Frame times", &frameHistory[0], int(frameHistory.size()), 1.0, (frameHistory.size() - 1.0) * -1.0);
			ImPlot::EndPlot();
		}
		ImGui::End();
	}
}
