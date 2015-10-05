/******************************************************************************
* File:    texture.hpp                                                       *
* Author:  William Gehring                                                   *
* Created: 2015-08-30                                                        *
*                                                                            *
* A general-purpose texture class for a game engine, with the ability to     *
* load DDS and WIC textures.                                                 *
******************************************************************************/

#pragma once

#include <d3d11.h>
#include <DirectXTex.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include <wrl.h>

class Texture {
	/* Private member variables */
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
	Microsoft::WRL::ComPtr<ID3D11Resource> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureView;

	/* Private enumerations */
private:
	enum filetypes {
		FILETYPE_DDS,
		FILETYPE_PNG,
		FILETYPE_JPG,
		FILETYPE_TGA,
		FILETYPE_BMP,
		FILETYPE_OTHER
	};
	enum errs {
		ERR_MEM_ALLOC,
		ERR_NO_DEVICE,
		ERR_NO_DEVICE_CONTEXT,
		ERR_NOT_IMPLEMENTED,
		ERR_UNABLE_TO_LOAD_FILE,
		ERR_UNKNOWN_FILETYPE
	};

	/* Constructors and public functions */
public:
	Texture();
	Texture(const Texture&);
	~Texture() = default;

	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>, Microsoft::WRL::ComPtr<ID3D11DeviceContext>);

	// Load methods for textures
	// Methods for the lazy who don't initialize the object beforehand
	bool Load(Microsoft::WRL::ComPtr<ID3D11Device>, Microsoft::WRL::ComPtr<ID3D11DeviceContext>, WCHAR*);
	bool Load(Microsoft::WRL::ComPtr<ID3D11Device>, Microsoft::WRL::ComPtr<ID3D11DeviceContext>, std::wstring*);
	// Methods for the diligent who properly initialized the object beforehand
	bool Load(WCHAR*);
	bool Load(std::wstring*);

	Microsoft::WRL::ComPtr<ID3D11Resource> GetTexture();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTextureView();

	/* Private functions */
private:
	int GetFileType(const std::wstring);

	bool LoadDDSTexture(WCHAR*);
	bool LoadWICTexture(WCHAR*);
};