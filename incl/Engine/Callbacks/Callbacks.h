#pragma once

#include "KrimzLib/KrimzLib.hpp"


namespace Engine {
	namespace Stage {
		void Start();
		void Update();
		void Resize(const kl::int2& siz);
		void End();
	}
	namespace Update {
		void Shadows();
		void Draw();
		void Outline();
		void Gizmo();
		void GUIBefore();
		void GUIAfter();
	}
}
