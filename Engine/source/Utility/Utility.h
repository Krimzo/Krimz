#pragma once

#include "KrimzLib.h"


namespace Engine {
	inline kl::window win;
	namespace Time {
		inline kl::timer timer;
		inline float delta = 0;
		inline float elapsed = 0;
	}
	namespace Struct {
		struct DRAW_VS_CB {
			kl::mat4 w;
			kl::mat4 vpCam;
			kl::mat4 vpSun;
		};
		struct DRAW_PS_CB {
			kl::float4 ambCol;
			kl::float4 dirCol;
			kl::float4 dirDir;
			kl::float4 camPos;
			kl::float4 rghFac;
			kl::float4 objInd;
		};
		struct GIZM_PS_CB {
			kl::float4 objCol;
			kl::float4 objInd;
		};
	}
}
