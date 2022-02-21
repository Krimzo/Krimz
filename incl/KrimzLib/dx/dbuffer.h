#pragma once

#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "KrimzLib/vecmath/vec4.h"


namespace kl {
	class dbuffer {
	private:
		ID3D11DeviceContext* devcon = nullptr;
		ID3D11DepthStencilState* stateEnabled = nullptr;
		ID3D11DepthStencilState* stateDisabled = nullptr;
		ID3D11DepthStencilView* view = nullptr;

	public:
		// Constructor
		dbuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon, int width, int height);

		// Desctructor
		~dbuffer();

		// Binds the buffer
		void setState(bool enabled);

		// Returns the view pointer
		ID3D11DepthStencilView* getView();

		// Clears the buffer
		void clear();
	};
}