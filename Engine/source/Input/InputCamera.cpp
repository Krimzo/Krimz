#include "Input/Input.h"
#include "Types/Camera.h"
#include "Render/Render.h"
#include "GUI/GUI.h"


void MovementSetup()
{
	Krimz::window.keys.w.down = [&]()
	{
		if (!ImGui::GetIO().WantCaptureKeyboard)
		{
			Krimz::Selected::camera->moveForward(Krimz::Time::delta);
		}
	};
	Krimz::window.keys.s.down = [&]()
	{
		if (!ImGui::GetIO().WantCaptureKeyboard)
		{
			Krimz::Selected::camera->moveBack(Krimz::Time::delta);
		}
	};
	Krimz::window.keys.d.down = [&]()
	{
		if (!ImGui::GetIO().WantCaptureKeyboard)
		{
			Krimz::Selected::camera->moveRight(Krimz::Time::delta);
		}
	};
	Krimz::window.keys.a.down = [&]()
	{
		if (!ImGui::GetIO().WantCaptureKeyboard)
		{
			Krimz::Selected::camera->moveLeft(Krimz::Time::delta);
		}
	};
	Krimz::window.keys.e.down = [&]()
	{
		if (!ImGui::GetIO().WantCaptureKeyboard)
		{
			Krimz::Selected::camera->moveUp(Krimz::Time::delta);
		}
	};
	Krimz::window.keys.q.down = [&]()
	{
		if (!ImGui::GetIO().WantCaptureKeyboard)
		{
			Krimz::Selected::camera->moveDown(Krimz::Time::delta);
		}
	};
	Krimz::window.keys.shift.press = [&]()
	{
		Krimz::Selected::camera->speed = 5.0f;
	};
	Krimz::window.keys.shift.release = [&]()
	{
		Krimz::Selected::camera->speed = 2.0f;
	};
}

bool firstClick = true;
bool camMoving = false;
void RotationSetup()
{
	Krimz::window.mouse.rmb.press = [&]()
	{
		if (Krimz::GUI::viewportFocus)
		{
			Krimz::window.mouse.hide();
			camMoving = true;
		}
	};
	Krimz::window.mouse.rmb.down = [&]()
	{
		if (camMoving)
		{
			const kl::int2 frameCenter = Krimz::window.center();

			if (firstClick)
			{
				Krimz::window.mouse.position = frameCenter;
				firstClick = false;
			}

			Krimz::Selected::camera->rotate(Krimz::window.mouse.position, frameCenter);
			Krimz::window.mouse.move(frameCenter);
		}
	};
	Krimz::window.mouse.rmb.release = [&]()
	{
		Krimz::window.mouse.show();
		firstClick = true;
		camMoving = false;
	};
}

void Krimz::Input::Camera()
{
	MovementSetup();
	RotationSetup();
}
