/******************************************************************************
* File:    texture.cpp                                                       *
* Author:  William Gehring                                                   *
* Created: 2015-08-30                                                        *
******************************************************************************/

#include "texture.hpp"

Texture::Texture()
	: m_device(nullptr),
<<<<<<< HEAD
	m_context(nullptr),
	m_texture(nullptr),
	m_textureView(nullptr) {}

Texture::Texture(const Texture& other) {}

=======
		m_context(nullptr),
		m_texture(nullptr),
		m_textureView(nullptr) {}

Texture::Texture(const Texture& other) {}

Texture::~Texture() {}

>>>>>>> ENDGINE/dev
bool Texture::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) {
	m_device = device;
	m_context = context;

	return true;
}

bool Texture::Load(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, WCHAR* filename) {
	return Initialize(device, context) && Load(filename);
}

bool Texture::Load(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::wstring* filename) {
	return Initialize(device, context) && Load(filename);
}

bool Texture::Load(WCHAR* filename) {
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

inline bool Texture::Load(std::wstring* filename) {
	return Load((WCHAR*)filename->c_str());
}

Microsoft::WRL::ComPtr<ID3D11Resource> Texture::GetTexture() {
	return m_texture;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Texture::GetTextureView() {
	return m_textureView;
}

int Texture::GetFileType(const std::wstring filename) {
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

bool Texture::LoadDDSTexture(WCHAR* filename) {
	if (FAILED(DirectX::CreateDDSTextureFromFile(m_device.Get(), m_context.Get(), filename, &m_texture, &m_textureView, 0))) {
		return false;
	}

	return true;
}

bool Texture::LoadWICTexture(WCHAR* filename) {
	if (FAILED(DirectX::CreateWICTextureFromFile(m_device.Get(), m_context.Get(), filename, &m_texture, &m_textureView, 0))) {
		return false;
	}

	return true;
}