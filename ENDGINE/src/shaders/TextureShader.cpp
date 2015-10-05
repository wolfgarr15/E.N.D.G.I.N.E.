///////////////////////////////////
// Filename: ShaderObject.cpp
//
// Author: Wolfe S. Greene
///////////////////////////////////

////////////////////////////////////////////////////////////////
// Class Header
//-------------------------------------------------------------
#include "TextureShader.hpp"

///////////////////////////////////////////////////////////////////
// Class Definition
//-----------------------------------------------------------------
TextureShader::TextureShader()
	: ShaderObject(),
	  m_textureSampler(nullptr)
{
	// DO NOTHING.
}

bool TextureShader::Initialize(CONST Microsoft::WRL::ComPtr<ID3D11Device>& renderDevice)
{
	// Use the default vertex input description.
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc;
	AppendVertexInputDescElements(inputDesc);

	// Initialize the shaders.
	RETURN_IF_FALSE(InitializeShaders(renderDevice, 
									  L"vsTexture.hlsl", 
									  "TextureVertexShader",
									  L"psTexture.hlsl",
									  "TexturePixelShader",
									  inputDesc));

	// Create the texture sampler state.
	RETURN_IF_FALSE(CreateSamplerState(renderDevice,
									   m_textureSampler,
									   D3D11_FILTER_MIN_MAG_MIP_LINEAR,
									   D3D11_TEXTURE_ADDRESS_WRAP,
									   D3D11_TEXTURE_ADDRESS_WRAP,
									   D3D11_TEXTURE_ADDRESS_WRAP,
									   0.0f,
									   1,
									   D3D11_COMPARISON_ALWAYS,
									   DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
									   0.0f,
									   D3D11_FLOAT32_MAX));

	return true;
}

bool TextureShader::Render(CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext,
						   int indexCount,
						   CONST DirectX::XMMATRIX& worldMatrix,
						   CONST DirectX::XMMATRIX& viewMatrix,
						   CONST DirectX::XMMATRIX& projectionMatrix,
						   CONST Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture)
{
	RETURN_IF_FALSE(SetShaderResources(deviceContext,
									   worldMatrix,
									   viewMatrix,
									   projectionMatrix,
									   texture));

	RenderShader(deviceContext, indexCount);

	return true;
}

void TextureShader::RenderShader(CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext, int indexCount)
{
	SetShaders(deviceContext);
	deviceContext->PSSetSamplers(0, 1, m_textureSampler.GetAddressOf());
	Draw(deviceContext, indexCount);
}

bool TextureShader::SetShaderResources(CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext,
									   CONST DirectX::XMMATRIX& worldMatrix,
									   CONST DirectX::XMMATRIX& viewMatrix,
									   CONST DirectX::XMMATRIX& projectionMatrix,
									   CONST Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture)
{
	RETURN_IF_FALSE(MapMatrixBuffer(deviceContext, 0, 1, worldMatrix, viewMatrix, projectionMatrix));
	deviceContext->PSSetShaderResources(0, 1, texture.GetAddressOf());
	return true;
}
