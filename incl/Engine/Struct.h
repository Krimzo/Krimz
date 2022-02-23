#pragma once

#include "KrimzLib/vecmath/vec4.h"
#include "KrimzLib/vecmath/mat4.h"


// Edtior draw shader buffers
struct DRAW_VS_CB {
	kl::mat4 w;
	kl::mat4 vpCam;
	kl::mat4 vpSun;
};
struct DRAW_PS_CB {
	kl::vec4 ambCol;
	kl::vec4 dirCol;
	kl::vec4 dirDir;
	kl::vec4 camPos;
	kl::vec4 rghFac;
	kl::vec4 objInd;
};

// Editor gizmo shader buffers
struct GIZM_PS_CB {
	kl::vec4 objCol;
	kl::vec4 objInd;
};
