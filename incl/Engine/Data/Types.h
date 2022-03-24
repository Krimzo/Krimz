#pragma once

#include "KrimzLib/math/float4.h"
#include "KrimzLib/math/mat4.h"


namespace Engine {
	namespace Types {
		// Edtior draw shader buffers
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

		// Editor gizmo shader buffers
		struct GIZM_PS_CB {
			kl::float4 objCol;
			kl::float4 objInd;
		};
	}
}
