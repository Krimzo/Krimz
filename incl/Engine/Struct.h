#pragma once

#include "KrimzLib/vecmath/vec4.h"
#include "KrimzLib/vecmath/mat4.h"


// Edtior draw shader buffers
struct DRAW_VS_CB {
	kl::mat4 w;
	kl::mat4 vp;
};
struct DRAW_PS_CB {
	kl::vec4 ambient;
	kl::vec4 dirCol;
	kl::vec4 dirDir;
	kl::vec4 objIndex;
};

// Editor hightlight shader buffers
struct HIGH_VS_CB {
	kl::mat4 wvp;
};
struct HIGH_PS_CB {
	kl::vec4 highCol;
};

// Editor gizmo shader buffers
struct GIZM_VS_CB {
	kl::mat4 wvp;
};
struct GIZM_PS_CB {
	kl::vec4 objColor;
	kl::vec4 objIndex;
};
