///////////////////////////////////
// Filename: D3D11Renderer.hpp
///////////////////////////////////

////////////////////////////////
// Pre-processor Directives
//------------------------------
#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

///////////////////////////////
// Include Headers
//-----------------------------
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include <string>
#include <wrl.h>

////////////////////////////////////
// Class Declaration
//----------------------------------
class Renderer
{


public:
	Renderer();
	~Renderer();
};

