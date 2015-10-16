///////////////////////////////////
// Filename: ShaderObject.hpp
//
// Author: Wolfe S. Greene
///////////////////////////////////

////////////////////////////////
// Pre-processor Directives
//------------------------------
#pragma once

///////////////////////////////
// Include Headers
//-----------------------------
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
#include <vector>
#include <wrl.h>
#include "../globals/macros.h"

////////////////////////////////////
// Class Declaration
//----------------------------------
class ShaderObject
{
protected:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;

public:
	ShaderObject(CONST Microsoft::WRL::ComPtr<ID3D11Device>& device,
				 CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext);

	virtual ~ShaderObject() = default;

	virtual bool Initialize() = 0;

	virtual bool Render(int indexCount,
						CONST DirectX::XMMATRIX& worldMatrix,
						CONST DirectX::XMMATRIX& viewMatrix,
						CONST DirectX::XMMATRIX& projectionMatrix);

protected:
	virtual bool InitializeShaders(CONST std::wstring& vertexShaderFilename,
								   CONST std::string& vertexShaderName,
								   CONST std::wstring& pixelShaderFilename,
								   CONST std::string& pixelShaderName,
								   CONST std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc);

	VOID AppendVertexInputDescElements(std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc);

	VOID AppendInputDescElement(std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc, 
							    CONST LPCSTR semanticName,
							    UINT semanticIndex,
							    DXGI_FORMAT format,
							    UINT inputSlot,
							    UINT alignedByteOffset,
							    D3D11_INPUT_CLASSIFICATION inputSlotClass,
							    UINT instanceDataSetpRate);

	bool CreateBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& bufferComPtr,
		              UINT byteWidth,
					  D3D11_USAGE usage,
					  UINT bindFlags,
					  UINT cpuAccessFlags,
					  UINT miscFlags,
					  UINT structureByteStrid,
					  CONST D3D11_SUBRESOURCE_DATA* pInitialData = NULL);

	bool CreateSamplerState(Microsoft::WRL::ComPtr<ID3D11SamplerState>& samplerComPtr,
							D3D11_FILTER filter,
							D3D11_TEXTURE_ADDRESS_MODE addressU,
							D3D11_TEXTURE_ADDRESS_MODE addressV,
							D3D11_TEXTURE_ADDRESS_MODE addressW,
							FLOAT mipLodBias,
							UINT maxAnisotropy,
							D3D11_COMPARISON_FUNC comparisonFunc,
							CONST DirectX::XMFLOAT4& borderColor,
							FLOAT minLod,
							FLOAT maxLod);

	bool CompileShaderFromFile(CONST std::wstring& fileName,
							   CONST std::string& shaderName,
							   CONST std::string& shaderVersion,
							   Microsoft::WRL::ComPtr<ID3D10Blob>& shaderBuffer);

	virtual bool SetShaders(CONST DirectX::XMMATRIX& worldMatrix,
							CONST DirectX::XMMATRIX& viewMatrix,
							CONST DirectX::XMMATRIX& projectionMatrix);

private:
	ShaderObject(CONST ShaderObject& other) = delete;
	ShaderObject& operator=(CONST ShaderObject& other) = delete;

	bool CreateMatrixBuffer();

	// NOTE: The matrix buffer will always be mapped to the first (BufferNumber = 0)
	//       buffer slot. Therefore, all subclass constant buffers
	//       must be assigned to buffer slots with BufferNumber > 0.
	//
	bool MapMatrixBuffer(DirectX::XMMATRIX worldMatrix,
						 DirectX::XMMATRIX viewMatrix,
						 DirectX::XMMATRIX projectionMatrix);
	
	// Temporary error handler until we decide how we'll handle engine-wide errors.
	VOID OutputShaderErrors(CONST Microsoft::WRL::ComPtr<ID3D10Blob>& errorBlob,
							CONST std::wstring& shaderFilename,
							CONST std::string& outputFilename);
};

