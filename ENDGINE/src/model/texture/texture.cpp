/******************************************************************************
 * File:    texture.cpp                                                       *
 * Author:  William Gehring                                                   *
 * Created: 2015-08-30                                                        *
 *                                                                            *
 * Modified: Wolfe S. Greene                                                  *
 ******************************************************************************/

#include "texture.hpp"

Texture::Texture()
	: m_device(nullptr),
	  m_context(nullptr),
	  m_texture(nullptr),
	  m_textureView(nullptr)
{
	// DO NOTHING.
}

bool Texture::Initialize(CONST Microsoft::WRL::ComPtr<ID3D11Device>& device, 
						 CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context) 
{
	RETURN_IF_FAILS(device.As(&m_device));
	RETURN_IF_FAILS(context.As(&m_context));
	return true;
}

bool Texture::Load(CONST Microsoft::WRL::ComPtr<ID3D11Device>& device, 
				   CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context, 
				   CONST std::wstring& filename) 
{
	return Initialize(device, context) && Load(filename);
}

bool Texture::Load(CONST std::wstring& filename) 
{
	if (!m_device) {
		throw ERR_NO_DEVICE;
	}

	if (!m_context) {
		throw ERR_NO_DEVICE_CONTEXT;
	}

	bool success = false;
	switch (GetFileType(filename)) {
		case FILETYPE_DDS:
			success = LoadDDSTexture(filename);
			break;
		case FILETYPE_JPG:
		case FILETYPE_PNG:
		case FILETYPE_TGA:
		case FILETYPE_BMP:
			success = LoadWICTexture(filename);
			break;
		default:
			throw ERR_UNKNOWN_FILETYPE;
	}

	if (!success) {
		throw ERR_UNABLE_TO_LOAD_FILE;
	}

	return true;
}

CONST Microsoft::WRL::ComPtr<ID3D11Resource>& Texture::GetTextureResource() CONST
{
	return m_texture;
}

CONST Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& Texture::GetTextureView() CONST
{
	return m_textureView;
}

int Texture::GetFileType(CONST std::wstring& filename) CONST 
{
	std::wstring ext = filename.substr(filename.find_last_of(L'.') + 1); // get rid of the .

	if (boost::iequals(ext, L"dds"))
		return FILETYPE_DDS;
	else if (boost::iequals(ext, L"jpg"))
		return FILETYPE_JPG;
	else if (boost::iequals(ext, L"png"))
		return FILETYPE_PNG;
	else if (boost::iequals(ext, L"tga"))
		return FILETYPE_TGA;
	else if (boost::iequals(ext, L"bmp"))
		return FILETYPE_BMP;
	else
		return FILETYPE_OTHER;
}

bool Texture::LoadDDSTexture(CONST std::wstring& filename) 
{
	RETURN_IF_FAILS(DirectX::CreateDDSTextureFromFile(m_device.Get(), 
												 m_context.Get(), 
												 filename.c_str(), 
												 &m_texture, 
												 &m_textureView, 
												 0))
	return true;
}

bool Texture::LoadWICTexture(CONST std::wstring& filename) 
{
	RETURN_IF_FAILS(DirectX::CreateWICTextureFromFile(m_device.Get(), 
													  m_context.Get(), 
													  filename.c_str(), 
													  &m_texture, 
													  &m_textureView,
													  0))
	return true;
}