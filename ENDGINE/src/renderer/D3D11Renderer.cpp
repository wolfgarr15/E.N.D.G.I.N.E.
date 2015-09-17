///////////////////////////////////
// Filename: D3D11Renderer.cpp
///////////////////////////////////

////////////////////////////////////////////////////////////////
// Class Header
//-------------------------------------------------------------
#include "D3D11Renderer.hpp"

///////////////////////////////////////////////////////////////////
// Class Definition
//-----------------------------------------------------------------
D3D11Renderer::D3D11Renderer()
	: m_hWnd(NULL),
	  m_pDevice(nullptr),
	  m_pDeviceContext(nullptr),
	  m_pDepthStencilBuffer(nullptr),
	  m_pDepthStencilState(nullptr),
	  m_pDepthStencilView(nullptr),
	  m_pRenderTargetView(nullptr),
	  m_pSwapChain(nullptr)
{
	SecureZeroMemory(&m_orthoMatrix, sizeof(DirectX::XMMATRIX));
	SecureZeroMemory(&m_worldMatrix, sizeof(DirectX::XMMATRIX));
	SecureZeroMemory(&m_projectionMatrix, sizeof(DirectX::XMMATRIX));
}

D3D11Renderer::~D3D11Renderer()
{
	// Set the swap chain state to windowed before closing.
	// Otherwise, errors occur with DXGI.
	if (m_pSwapChain)
		m_pSwapChain->SetFullscreenState(FALSE, NULL);
}

PVOID D3D11Renderer::operator new(UINT uMemorySize)
{
	// Set the alignment of the D3D11Renderer memory block to 16 bytes.
	//
	// NOTE: This is necessary to support high speed XMMATRIX calculations,
	//       since WIN32 uses an 8 byte alignment by default.
	UINT uAlignment = 16;
	return _aligned_malloc(uMemorySize, uAlignment);
}

VOID D3D11Renderer::operator delete(PVOID pMemoryBlock)
{
	_aligned_free(pMemoryBlock);
}

BOOL D3D11Renderer::Initialize(HWND hWnd, CONST EngineConfig* pConfig)
{
	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;
	Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter;
	Microsoft::WRL::ComPtr<IDXGIOutput> pAdapterOutput;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;

	m_hWnd = hWnd;
	m_bVsync = pConfig->UseVsync();
	m_bFullscreen = pConfig->IsFullscreen();
	m_fScreenFar = pConfig->GetScreenFar();
	m_fScreenNear = pConfig->GetScreenNear();

	SetDisplayDimensions(pConfig);

	RETURN_IF_FAILS(CreateDXGIFactory(__uuidof(IDXGIFactory), &pFactory));

	// NOTE: For now, support for only one adapter is hardcoded. Will need to
	//       investigate multi-GPU initialization if it is to be a feature.
	RETURN_IF_FAILS(pFactory->EnumAdapters(0, &pAdapter));

	RETURN_IF_FAILS(SetVideoCardInfo(pAdapter.Get()));

	// NOTE: As with the adapter enumeration, this is currently
	//       coded to utilize only one adapter output.
	RETURN_IF_FAILS(pAdapter->EnumOutputs(0, &pAdapterOutput));

	RETURN_IF_FALSE(SetRefreshParams(pAdapterOutput.Get()));

	RETURN_IF_FALSE(InitializeSwapChain());

	RETURN_IF_FAILS(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (VOID**)&pBackBuffer));

	RETURN_IF_FAILS(m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, m_pRenderTargetView.GetAddressOf()));

	RETURN_IF_FALSE(InitializeDepthStencilBuffer());

	RETURN_IF_FALSE(InitializeDepthStencilState());

	RETURN_IF_FALSE(InitializeDepthStencilView());

	RETURN_IF_FALSE(InitializeRasterState());

	InitializeViewPort();

	InitializeProjectionMatrix();

	InitializeWorldMatrix();

	IntializeOrthoMatrix();

	return TRUE;
}

VOID D3D11Renderer::BeginScene(FLOAT red, FLOAT green, FLOAT blue, FLOAT alpha)
{
	FLOAT color[4] = { red, green, blue, alpha };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

VOID D3D11Renderer::EndScene()
{
	if (m_bVsync)
	{
		// Lock to screen refresh rate.
		m_pSwapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_pSwapChain->Present(0, 0);
	}
}

ID3D11Device* D3D11Renderer::GetDevice() CONST
{
	return m_pDevice.Get();
}

ID3D11DeviceContext* D3D11Renderer::GetDeviceContext() CONST
{
	return m_pDeviceContext.Get();
}

VOID D3D11Renderer::GetOrthoMatrix(DirectX::XMMATRIX& orthoMatrix) CONST
{
	orthoMatrix = m_orthoMatrix;
}

VOID D3D11Renderer::GetProjectionMatrix(DirectX::XMMATRIX& projectionMatrix) CONST
{
	projectionMatrix = m_projectionMatrix;
}

VOID D3D11Renderer::GetWorldMatrix(DirectX::XMMATRIX& worldMatrix) CONST
{
	worldMatrix = m_worldMatrix;
}

SIZE_T D3D11Renderer::GetVideoMemoryMB() CONST
{
	return m_iVideoMemoryMB;
}

VOID D3D11Renderer::GetVideoCardName(std::string& videoCardName) CONST
{
	videoCardName = m_sVideoCardName;
}

VOID D3D11Renderer::IntializeOrthoMatrix()
{
	m_orthoMatrix = DirectX::XMMatrixOrthographicLH((FLOAT)m_iDisplayWidth, 
													(FLOAT)m_iDisplayHeight, 
						                            m_fScreenNear, 
													m_fScreenFar);
}

VOID D3D11Renderer::InitializeWorldMatrix()
{
	m_worldMatrix = DirectX::XMMatrixIdentity();
}

VOID D3D11Renderer::InitializeProjectionMatrix()
{
	// Setup the projection matrix.
	FLOAT fieldOfView = DirectX::XM_PI / 4.0f;
	FLOAT displayAspect = (FLOAT)m_iDisplayWidth / (FLOAT)m_iDisplayHeight;

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, displayAspect, m_fScreenNear, m_fScreenFar);
}

VOID D3D11Renderer::InitializeViewPort()
{
	D3D11_VIEWPORT viewPort;
	SecureZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));

	// Setup the viewport for rendering.
	viewPort.Width = (FLOAT)m_iDisplayWidth;
	viewPort.Height = (FLOAT)m_iDisplayHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;

	// Create the viewport.
	m_pDeviceContext->RSSetViewports(1, &viewPort);
}

BOOL D3D11Renderer::InitializeRasterState()
{
	D3D11_RASTERIZER_DESC rasterDesc;
	SecureZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = FALSE;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = TRUE;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = FALSE;
	rasterDesc.MultisampleEnable = FALSE;
	rasterDesc.ScissorEnable = FALSE;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	RETURN_IF_FAILS(m_pDevice->CreateRasterizerState(&rasterDesc, m_pRasterState.GetAddressOf()));
	
	// Now set the rasterizer state.
	m_pDeviceContext->RSSetState(m_pRasterState.Get());

	return TRUE;
}

BOOL D3D11Renderer::InitializeDepthStencilView()
{
	// Initialize the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	RETURN_IF_FAILS(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), &depthStencilViewDesc, m_pDepthStencilView.GetAddressOf()));
	
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

	return TRUE;
}

BOOL D3D11Renderer::InitializeDepthStencilState()
{
	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = TRUE;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	RETURN_IF_FAILS(m_pDevice->CreateDepthStencilState(&depthStencilDesc, m_pDepthStencilState.GetAddressOf()));
	
	// Set the depth stencil state.
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);

	return TRUE;
}

BOOL D3D11Renderer::InitializeDepthStencilBuffer()
{
	// Initialize the description of the depth buffer.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = m_iDisplayWidth;
	depthBufferDesc.Height = m_iDisplayHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	RETURN_IF_FAILS(m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, m_pDepthStencilBuffer.GetAddressOf()));
	
	return TRUE;
}

BOOL D3D11Renderer::InitializeSwapChain()
{
	// Initialize the swap chain description.
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	SecureZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	// Set to use a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height based on the screen.
	swapChainDesc.BufferDesc.Width = m_iDisplayWidth;
	swapChainDesc.BufferDesc.Height = m_iDisplayHeight;

	// Set the back buffer to a regular 32-bit format.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of he back buffer.
	if (m_bVsync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = m_uRefreshNum;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = m_uRefreshDen;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the backbuffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for render window.
	//
	// NOTE: We'll need to rethink this if we ever decide to render to multiple windows.
	swapChainDesc.OutputWindow = m_hWnd;

	// Disable multisampling (CHANGE THIS! Need to read from INI.).
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set fullscreen mode.
	swapChainDesc.Windowed = !(m_bFullscreen);

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	RETURN_IF_FAILS(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		                                          D3D11_SDK_VERSION, &swapChainDesc, m_pSwapChain.GetAddressOf(), 
												  m_pDevice.GetAddressOf(), NULL, m_pDeviceContext.GetAddressOf()));

	return TRUE;
}

VOID D3D11Renderer::SetDisplayDimensions(CONST EngineConfig* pConfig)
{
	if (m_bFullscreen)
	{
		// Use the max system display dimensions.
		m_iDisplayWidth = GetSystemMetrics(SM_CXSCREEN);
		m_iDisplayHeight = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		// Use configuration display dimesnions.
		m_iDisplayWidth = pConfig->GetWindowWidth();
		m_iDisplayHeight = pConfig->GetWindowHeight();
	}
}

BOOL D3D11Renderer::SetRefreshParams(IDXGIOutput* pAdapterOutput)
{
	
	std::unique_ptr<DXGI_MODE_DESC[]> pDisplayModeList;
	UINT numModes;

	// Get the number of display modes.
	//
	// NOTE: For now, the display format is hardcoded.
	RETURN_IF_FAILS(pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL));
	pDisplayModeList = std::make_unique<DXGI_MODE_DESC[]>(numModes);
	if (!pDisplayModeList)
		return FALSE;

	// Fill the display mode list structure.
	RETURN_IF_FAILS(pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, pDisplayModeList.get()));

	// Find the display mode that matches the screen width and height,
	// and store the corresponding refresh rate numerator and denominator.
	for (UINT i = 0; i < numModes; i++)
	{
		if (pDisplayModeList[i].Width == (UINT)m_iDisplayWidth
			&& pDisplayModeList[i].Height == (UINT)m_iDisplayHeight)
		{
			m_uRefreshNum = pDisplayModeList[i].RefreshRate.Numerator;
			m_uRefreshDen = pDisplayModeList[i].RefreshRate.Denominator;
		}
	}

	return TRUE;
}

BOOL D3D11Renderer::SetVideoCardInfo(IDXGIAdapter* pAdapter)
{
	DXGI_ADAPTER_DESC adapterDesc;

	RETURN_IF_FAILS(pAdapter->GetDesc(&adapterDesc));

	m_iVideoMemoryMB = adapterDesc.DedicatedVideoMemory / 1024 / 1024;

	m_sVideoCardName = Convert::Wc_strToString(adapterDesc.Description);

	return TRUE;
}