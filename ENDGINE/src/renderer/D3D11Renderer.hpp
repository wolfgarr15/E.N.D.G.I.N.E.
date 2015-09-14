///////////////////////////////////
// Filename: D3D11Renderer.hpp
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
#include "../config/EngineConfig.hpp"

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
	DirectX::XMMATRIX m_orthoMatrix;
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_worldMatrix;

public:
	D3D11Renderer();
	~D3D11Renderer();

	PVOID operator new(UINT uMemorySize);
	VOID operator delete(PVOID pMemoryBlock);

	BOOL Initialize(HWND hWnd, CONST EngineConfig* pConfig);

	VOID BeginScene(FLOAT red, FLOAT green, FLOAT blue, FLOAT alpha);

	VOID EndScene();

	ID3D11Device* GetDevice() CONST;
	ID3D11DeviceContext* GetDeviceContext() CONST;

	VOID GetOrthoMatrix(DirectX::XMMATRIX& orthoMatrix) CONST;
	VOID GetProjectionMatrix(DirectX::XMMATRIX& projectionMatrix) CONST;
	VOID GetWorldMatrix(DirectX::XMMATRIX& worldMatrix) CONST;

	SIZE_T GetVideoMemoryMB() CONST;
	VOID GetVideoCardName(std::string& videoCardName) CONST;

private:
	D3D11Renderer(const D3D11Renderer& other) = delete;
	D3D11Renderer& operator =(const D3D11Renderer& other) = delete;

	VOID IntializeOrthoMatrix();
	VOID InitializeWorldMatrix();
	VOID InitializeProjectionMatrix();
	VOID InitializeViewPort();
	BOOL InitializeRasterState();
	BOOL InitializeDepthStencilView();
	BOOL InitializeDepthStencilState();
	BOOL InitializeDepthStencilBuffer();
	BOOL InitializeSwapChain();

	VOID SetDisplayDimensions(CONST EngineConfig* pCongig);

	BOOL SetRefreshParams(IDXGIOutput* pAdapterOuput);

	BOOL SetVideoCardInfo(IDXGIAdapter* pAdapter);
};
