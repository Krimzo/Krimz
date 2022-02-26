#include "Engine/GUI.h"


void Explorer() {
	if (ImGui::Begin("Explorer")) {
		// Updating the pos/size
		ImGui::SetWindowPos("Explorer", ImVec2(winSize.x * sidePanelsWidth - 1, winSize.y * (1 - explorerHeight)));
		ImGui::SetWindowSize("Explorer", ImVec2(winSize.x * (1 - 2 * sidePanelsWidth) + 1, winSize.y * explorerHeight));

		// End draw
		ImGui::End();
	}
	else {
		// Updating the pos
		ImGui::SetWindowPos("Explorer", ImVec2(winSize.x * sidePanelsWidth - 1, mainMenuSize.y - 1));
	}
}
