///////////////////////////////////
// Filename: D3D11Renderer.hpp
//
// Author: Wolfe S. Greene
///////////////////////////////////

////////////////////////////////
// Pre-processor Directives
//------------------------------
#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

///////////////////////////////
// Include Headers
//-----------------------------
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include <string>
#include <wrl.h>
#include "..\globals\functions.hpp"
#include "..\globals\macros.h"
#include "..\config\EngineConfig.hpp"

////////////////////////////////////
// Class Declaration
//----------------------------------
class D3D11Renderer
{
	HWND m_hWnd;

	BOOL m_bVsync;
	BOOL m_bFullscreen;

	FLOAT m_fScreenFar;
	FLOAT m_fScreenNear;

	INT m_iDisplayWidth;
	INT m_iDisplayHeight;

	UINT m_uRefreshNum;
	UINT m_uRefreshDen;

	SIZE_T m_iVideoMemoryMB;
	std::string m_sVideoCardName;

	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pRasterState;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView; // <-- Should this be a vector of render targets? 

	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;

	// Should the renderer own these matrices? Something to think about...
	DirectX::XMMATRIX m_viewMatrix;
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_worldMatrix;

public:
	D3D11Renderer();
	~D3D11Renderer();

	PVOID operator new(size_t uMemorySize);
	VOID operator delete(PVOID pMemoryBlock);

	BOOL Initialize(HWND hWnd, CONST EngineConfig* pConfig);

	VOID BeginScene(FLOAT red, FLOAT green, FLOAT blue, FLOAT alpha);

	VOID EndScene();

	CONST Microsoft::WRL::ComPtr<ID3D11Device>& GetDevice() CONST;
	CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetDeviceContext() CONST;

	CONST DirectX::XMMATRIX& GetViewMatrix() CONST;
	CONST DirectX::XMMATRIX& GetProjectionMatrix() CONST;
	CONST DirectX::XMMATRIX& GetWorldMatrix() CONST;

	SIZE_T GetVideoMemoryMB() CONST;
	VOID GetVideoCardName(std::string& videoCardName) CONST;

private:
	D3D11Renderer(CONST D3D11Renderer& other) = delete;
	D3D11Renderer& operator=(CONST D3D11Renderer& other) = delete;

	VOID IntializeOrthoMatrix();
	VOID InitializeWorldMatrix();
	VOID InitializeProjectionMatrix();
	VOID InitializeViewPort();
	BOOL InitializeRasterState();
	BOOL InitializeDepthStencilView();
	BOOL InitializeDepthStencilState();
	BOOL InitializeDepthStencilBuffer();
	BOOL InitializeSwapChain();

	VOID SetDisplayDimensions(CONST EngineConfig* pConfig);

	BOOL SetRefreshParams(IDXGIOutput* pAdapterOuput);

	BOOL SetVideoCardInfo(IDXGIAdapter* pAdapter);
};

