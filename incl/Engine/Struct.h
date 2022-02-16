#pragma once

#include "KrimzLib/vecmath/vec4.h"
#include "KrimzLib/vecmath/mat4.h"


struct DEF_VS_CB {
	kl::mat4 w;
	kl::mat4 vp;
};
struct DEF_PS_CB {
	kl::vec4 ambient;
	kl::vec4 dirCol;
	kl::vec4 dirDir;
};
