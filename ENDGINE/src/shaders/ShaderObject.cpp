///////////////////////////////////
// Filename: ShaderObject.cpp
//
// Author: Wolfe S. Greene
///////////////////////////////////

////////////////////////////////////////////////////////////////
// Class Header
//-------------------------------------------------------------
#include "ShaderObject.hpp"

///////////////////////////////////////////////////////////////////
// Class Definition
//-----------------------------------------------------------------
ShaderObject::ShaderObject(CONST Microsoft::WRL::ComPtr<ID3D11Device>& device,
						   CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
	: m_device(device),
	  m_deviceContext(deviceContext),
	  m_matrixBuffer(nullptr),
	  m_inputLayout(nullptr),
	  m_pixelShader(nullptr),
	  m_vertexShader(nullptr)
{
	// DO NOTHING.
}

bool ShaderObject::InitializeShaders(CONST std::wstring& vertexShaderFilename,
									 CONST std::string& vertexShaderName,
									 CONST std::wstring& pixelShaderFilename,
									 CONST std::string& pixelShaderName,
									 CONST std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc)
{
	Microsoft::WRL::ComPtr<ID3D10Blob> vertexShaderBuffer;
	Microsoft::WRL::ComPtr<ID3D10Blob> pixelShaderBuffer;

	// Compile the vertex shader.
	RETURN_IF_FALSE(CompileShaderFromFile(vertexShaderFilename,
										  vertexShaderName,
										  "vs_5_0",
										  vertexShaderBuffer));

	// Compile the pixel shader.
	RETURN_IF_FALSE(CompileShaderFromFile(pixelShaderFilename,
										  pixelShaderName,
										  "ps_5_0",
										  pixelShaderBuffer));

	// Create the vertex shader from the buffer.
	RETURN_IF_FAILS(m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
											     vertexShaderBuffer->GetBufferSize(),
											     NULL,
											     m_vertexShader.GetAddressOf()));

	// Create the pixel shader from the buffer.
	RETURN_IF_FAILS(m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
												pixelShaderBuffer->GetBufferSize(),
												NULL,
												m_pixelShader.GetAddressOf()));

	// Create the vertex input layout.
	RETURN_IF_FAILS(m_device->CreateInputLayout(inputDesc.data(), 
										        inputDesc.size(),
												vertexShaderBuffer->GetBufferPointer(),
												vertexShaderBuffer->GetBufferSize(),
												m_inputLayout.GetAddressOf()));

	// Create the matrix constant buffer.
	RETURN_IF_FALSE(CreateMatrixBuffer());

	return true;
}

bool ShaderObject::Render(int indexCount,
						  CONST DirectX::XMMATRIX& worldMatrix,
						  CONST DirectX::XMMATRIX& viewMatrix,
						  CONST DirectX::XMMATRIX& projectionMatrix)
{
	RETURN_IF_FALSE(SetShaders(worldMatrix, viewMatrix, projectionMatrix));
	m_deviceContext->DrawIndexed(indexCount, 0, 0);
	return true;
}

VOID ShaderObject::AppendVertexInputDescElements(std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc)
{
	// The position coordinate element.
	AppendInputDescElement(inputDesc,
					   "POSITION",
					   0,
					   DXGI_FORMAT_R32G32B32_FLOAT,
					   0,
					   0,
					   D3D11_INPUT_PER_VERTEX_DATA,
					   0);

	// The texture coordinate element.
	AppendInputDescElement(inputDesc,
		               "TEXCOORD",
					   0,
					   DXGI_FORMAT_R32G32_FLOAT,
					   0,
					   D3D11_APPEND_ALIGNED_ELEMENT,
					   D3D11_INPUT_PER_VERTEX_DATA,
					   0);

	// The normal coordinate element.
	AppendInputDescElement(inputDesc,
		               "NORMAL",
		               0,
		               DXGI_FORMAT_R32G32B32_FLOAT,
		               0,
		               D3D11_APPEND_ALIGNED_ELEMENT,
		               D3D11_INPUT_PER_VERTEX_DATA,
		               0);
}

VOID ShaderObject::AppendInputDescElement(std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc,
						                  CONST LPCSTR semanticName,
						                  UINT semanticIndex,
						                  DXGI_FORMAT format,
						                  UINT inputSlot,
						                  UINT alignedByteOffset,
						                  D3D11_INPUT_CLASSIFICATION inputSlotClass,
						                  UINT instanceDataSetpRate)
{
	D3D11_INPUT_ELEMENT_DESC inputElement =
	{
		semanticName,
		semanticIndex,
		format,
		inputSlot,
		alignedByteOffset,
		inputSlotClass,
		instanceDataSetpRate
	};

	inputDesc.push_back(inputElement);
}

bool ShaderObject::CreateBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& bufferComPtr,
								UINT byteWidth,
								D3D11_USAGE usage,
								UINT bindFlags,
								UINT cpuAccessFlags,
								UINT miscFlags,
								UINT structureByteStride,
								CONST D3D11_SUBRESOURCE_DATA* pInitialData)
{
	D3D11_BUFFER_DESC bufferDesc =
	{
		byteWidth,
		usage,
		bindFlags,
		cpuAccessFlags,
		miscFlags,
		structureByteStride
	};

	RETURN_IF_FAILS(m_device->CreateBuffer(&bufferDesc, 
					pInitialData, 
					bufferComPtr.GetAddressOf()));
	return true;
}

bool ShaderObject::CreateSamplerState(Microsoft::WRL::ComPtr<ID3D11SamplerState>& samplerComPtr,
									  D3D11_FILTER filter,
									  D3D11_TEXTURE_ADDRESS_MODE addressU,
									  D3D11_TEXTURE_ADDRESS_MODE addressV,
									  D3D11_TEXTURE_ADDRESS_MODE addressW,
									  FLOAT mipLodBias,
									  UINT maxAnisotropy,
									  D3D11_COMPARISON_FUNC comparisonFunc,
									  CONST DirectX::XMFLOAT4& borderColor,
									  FLOAT minLod,
									  FLOAT maxLod)
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = filter;
	samplerDesc.AddressU = addressU;
	samplerDesc.AddressV = addressV;
	samplerDesc.AddressW = addressW;
	samplerDesc.MipLODBias = mipLodBias;
	samplerDesc.MaxAnisotropy = maxAnisotropy;
	samplerDesc.ComparisonFunc = comparisonFunc;
	samplerDesc.BorderColor[0] = borderColor.x;
	samplerDesc.BorderColor[1] = borderColor.y;
	samplerDesc.BorderColor[2] = borderColor.z;
	samplerDesc.BorderColor[3] = borderColor.w;
	samplerDesc.MinLOD = minLod;
	samplerDesc.MaxLOD = maxLod;

	RETURN_IF_FAILS(m_device->CreateSamplerState(&samplerDesc, 
												 samplerComPtr.GetAddressOf()));
	return true;
}

bool ShaderObject::CompileShaderFromFile(CONST std::wstring& shaderFileName, 
	                                       CONST std::string& shaderName,
										   CONST std::string& shaderVersion,
										   Microsoft::WRL::ComPtr<ID3D10Blob>& shaderBuffer)
{
	Microsoft::WRL::ComPtr<ID3D10Blob> errorBlob(nullptr);
	HRESULT result = D3DCompileFromFile(shaderFileName.c_str(),
										NULL,
										NULL,
										shaderName.c_str(),
										shaderVersion.c_str(),
										D3D10_SHADER_ENABLE_STRICTNESS,
										NULL,
										shaderBuffer.GetAddressOf(),
										errorBlob.GetAddressOf());

	if (FAILED(result))
	{
		OutputShaderErrors(errorBlob, shaderFileName, "ShaderError.log");
		return false;
	}

	return true;
}

bool ShaderObject::SetShaders(CONST DirectX::XMMATRIX& worldMatrix,
							  CONST DirectX::XMMATRIX& viewMatrix,
						      CONST DirectX::XMMATRIX& projectionMatrix)
{
	// Set the vertex shader input layout.
	m_deviceContext->IAGetInputLayout(m_inputLayout.GetAddressOf());

	// Set the vertex and pixel shaders for rendering.
	m_deviceContext->VSSetShader(m_vertexShader.Get(), NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader.Get(), NULL, 0);

	return MapMatrixBuffer(worldMatrix, viewMatrix, projectionMatrix);
}

bool ShaderObject::MapMatrixBuffer(DirectX::XMMATRIX worldMatrix,
								   DirectX::XMMATRIX viewMatrix,
								   DirectX::XMMATRIX projectionMatrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// Transpose matrices before mapping them to the shader.
	worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
	projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer, do it can be written to.
	RETURN_IF_FAILS(m_deviceContext->Map(m_matrixBuffer.Get(),
									     NULL,
									     D3D11_MAP_WRITE_DISCARD,
									     NULL,
									     &mappedResource));

	// Get a pointer to the mapped resource.
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy data to the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
	m_deviceContext->Unmap(m_matrixBuffer.Get(), NULL);

	// Map the constant buffer to the vertex shader.
	m_deviceContext->VSSetConstantBuffers(0, 1, m_matrixBuffer.GetAddressOf());

	return true;
}

bool ShaderObject::CreateMatrixBuffer()
{
	return CreateBuffer(m_matrixBuffer,
					    sizeof(MatrixBufferType),
						D3D11_USAGE_DYNAMIC,
						D3D11_BIND_CONSTANT_BUFFER,
						D3D11_CPU_ACCESS_WRITE,
						NULL,
						NULL);
}

VOID ShaderObject::OutputShaderErrors(CONST Microsoft::WRL::ComPtr<ID3D10Blob>& errorBlob,
									  CONST std::wstring& shaderFilename,
									  CONST std::string& outputFilename)
{
	std::ofstream fout(outputFilename.c_str());

	if (errorBlob)
	{
		std::string compileErrors((char*)(errorBlob->GetBufferPointer()));
		fout << compileErrors.c_str() << std::endl;
	}
	else
	{
		std::string fileNameString = Convert::Wc_strToString(shaderFilename.c_str());
		fout << "Error: Shader file " << fileNameString.c_str()
			 << " not found!" << std::endl;
	}

	fout.close();
}
