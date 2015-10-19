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
TextureShader::TextureShader(CONST Microsoft::WRL::ComPtr<ID3D11Device>& device,
							 CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext)
	: ShaderObject(device, deviceContext),
	  m_textureSampler(nullptr)
{
	// DO NOTHING.
}

bool TextureShader::Initialize()
{
	// Use the default vertex input description.
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc;
	AppendVertexInputDescElements(inputDesc);

	// Initialize the shaders.
	RETURN_IF_FALSE(InitializeShaders(L"vsTextureShader.hlsli", 
									  "TextureVertexShader",
									  L"psTextureShader.hlsli",
									  "TexturePixelShader",
									  inputDesc));

	// Create the texture sampler state.
	RETURN_IF_FALSE(CreateSamplerState(m_textureSampler,
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

bool TextureShader::Render(int indexCount,
						   CONST DirectX::XMMATRIX& worldMatrix,
						   CONST DirectX::XMMATRIX& viewMatrix,
						   CONST DirectX::XMMATRIX& projectionMatrix,
						   CONST Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture)
{
	// Set the texture resource in the pixel shader.
	m_deviceContext->PSSetShaderResources(0, 1, texture.GetAddressOf());
	
	// Set the sampler state.
	m_deviceContext->PSSetSamplers(0, 1, m_textureSampler.GetAddressOf());

	// Always return the base render call!
	return ShaderObject::Render(indexCount,
								worldMatrix,
								viewMatrix,
								projectionMatrix);
}
