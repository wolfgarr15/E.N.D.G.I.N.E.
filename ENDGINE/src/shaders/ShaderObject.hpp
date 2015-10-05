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

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;

public:
	ShaderObject();
	virtual ~ShaderObject() = 0;

	virtual bool Initialize(CONST Microsoft::WRL::ComPtr<ID3D11Device>& renderDevice) = 0;

protected:
	virtual bool InitializeShaders(CONST Microsoft::WRL::ComPtr<ID3D11Device>& renderDevice,
								   CONST std::wstring& vertexShaderFilename,
								   CONST std::string& vertexShaderName,
								   CONST std::wstring& pixelShaderFilename,
								   CONST std::string& pixelShaderName,
								   CONST std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc);

	void AppendVertexInputDescElements(std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc);

	void AppendInputDescElement(std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc, 
							    CONST LPCSTR semanticName,
							    UINT semanticIndex,
							    DXGI_FORMAT format,
							    UINT inputSlot,
							    UINT alignedByteOffset,
							    D3D11_INPUT_CLASSIFICATION inputSlotClass,
							    UINT instanceDataSetpRate);

	bool CreateBuffer(CONST Microsoft::WRL::ComPtr<ID3D11Device>& renderDevice,
					  Microsoft::WRL::ComPtr<ID3D11Buffer>& bufferComPtr,
		              UINT byteWidth,
					  D3D11_USAGE usage,
					  UINT bindFlags,
					  UINT cpuAccessFlags,
					  UINT miscFlags,
					  UINT structureByteStrid,
					  CONST D3D11_SUBRESOURCE_DATA* pInitialData = NULL);

	bool CreateSamplerState(CONST Microsoft::WRL::ComPtr<ID3D11Device>& renderDevice,
							Microsoft::WRL::ComPtr<ID3D11SamplerState>& samplerComPtr,
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

	bool MapMatrixBuffer(CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext,
					     UINT bufferNumber,
		                 UINT bufferCount,
		                 DirectX::XMMATRIX worldMatrix,
		                 DirectX::XMMATRIX viewMatrix,
		                 DirectX::XMMATRIX projectionMatrix);

	virtual void SetShaders(CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext);

	virtual void Draw(CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext, int indexCount);

	virtual void RenderShader(CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext, int indexCount) = 0;

private:
	ShaderObject(CONST ShaderObject& other) = delete;
	ShaderObject& operator=(CONST ShaderObject& other) = delete;

	bool CreateMatrixBuffer(CONST Microsoft::WRL::ComPtr<ID3D11Device>& renderDevice);
	
	// Temporary error handler until we decide how we'll handle engine-wide errors.
	void OutputShaderErrors(CONST Microsoft::WRL::ComPtr<ID3D10Blob>& errorBlob,
							CONST std::wstring& shaderFilename,
							CONST std::string& outputFilename);
};

