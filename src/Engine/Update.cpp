#include "Engine/Engine.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"


void Update() {
	// Time
	deltaT = timer.interval();
	elapsedT = timer.elapsed();

	// Input call
	Input();

	// Clearing the frame buffer
	gpu->clear(background);

	// Rendering skybox
	if (skybox) {
		gpu->setDepthTest(false);
		skybox->render(camera.matrix());
		gpu->setDepthTest(true);
	}

	// Setting the camera data
	EDI_VS_CB edi_vert_data = {};
	edi_vert_data.vp = camera.matrix();

	// Setting the lighting data
	EDI_PS_CB edi_pixl_data = {};
	edi_pixl_data.ambient = ambient.getCol();
	edi_pixl_data.dirCol = sun.getCol();
	edi_pixl_data.dirDir = sun.getDir();

	// Rendering objects
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->visible) {
			// Setting the world matrix
			edi_vert_data.w = entities[i]->matrix();

			// Updating the vert data
			editor_sh->setVertData(&edi_vert_data);

			// Setting the obj index
			edi_pixl_data.objIndex.x = *(float*)&i;

			// Updating the pixl data
			editor_sh->setPixlData(&edi_pixl_data);

			// Rendering the object
			entities[i]->render();
		}
	}	

	// Highlighting
	if (selected) {
		// Setting the highlight vertex data
		kl::mat4 wvp = camera.matrix() * selected->matrix();
		highlight_sh->setVertData(&wvp);

		// Setting the highlight pixel data
		kl::vec4 hig_col = highlight;
		highlight_sh->setPixlData(&hig_col);

		// Rendering
		gpu->setDepthTest(false);
		wire_ra->bind();
		selected->render();
		solid_ra->bind();
		gpu->setDepthTest(true);
	}

	// Drawing the gui
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Entity properties
	if (ImGui::Begin("Entity Properties")) {
		if (selected) {
			ImGui::Checkbox("Visible", &selected->visible);
			ImGui::Checkbox("Shadows", &selected->shadows);
			ImGui::SliderFloat3("Size", (float*)&selected->size, 0, 2.5f);
			ImGui::SliderFloat3("Position", (float*)&selected->position, -5, 5);
			ImGui::SliderFloat3("Rotation", (float*)&selected->rotation, 0, 360);
			ImGui::Checkbox("Physics", &selected->physics);
			ImGui::SliderFloat3("Acceleration", (float*)&selected->acceler, 0, 3);
			ImGui::SliderFloat3("Veloctiy", (float*)&selected->velocity, 0, 5);
			ImGui::SliderFloat3("Angular", (float*)&selected->angular, -60, 60);
		}
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Swapping the frame buffers
	gpu->swap(true);
}
