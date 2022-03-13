#include "Engine/GUI.h"


void Explorer() {
	guiExplorPos = kl::int2(guiSceneSize.x, int(winSize.y * (1.0f - explorerHeightRatio)));
	guiExplorSize = kl::int2(int(winSize.x - 2.0f * guiSceneSize.x), int(winSize.y * explorerHeightRatio + 1.0f));
	ImGui::SetNextWindowPos(ImVec2(float(guiExplorPos.x), float(guiExplorPos.y)));
	ImGui::SetNextWindowSize(ImVec2(float(guiExplorSize.x), float(guiExplorSize.y)));
	if (ImGui::Begin("Explorer", nullptr, panelWinFlags)) {

		// End draw
		ImGui::End();
	}
}
