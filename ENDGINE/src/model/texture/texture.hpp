/******************************************************************************
 * File:    texture.hpp                                                       *
 * Author:  William Gehring                                                   *
 * Created: 2015-08-30                                                        *
 *                                                                            *
 * Modified: Wolfe S. Greene                                                  *
 *                                                                            *
 * A general-purpose texture class for a game engine, with the ability to     *
 * load DDS and WIC textures.                                                 *
 ******************************************************************************/

#pragma once

#include <d3d11.h>
#include <boost/algorithm/string.hpp>
#include <string>
#include <wrl.h>

#include "../../globals/macros.h"
#include "DDSTextureLoader/DDSTextureLoader.h"
#include "WICTextureLoader/WICTextureLoader.h"

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
	~Texture() = default;

	bool Initialize(CONST Microsoft::WRL::ComPtr<ID3D11Device>& device, 
					CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context);

	// Load methods for textures
	// Methods for the lazy who don't initialize the object beforehand
	bool Load(CONST Microsoft::WRL::ComPtr<ID3D11Device>& device, 
			  CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context, 
			  CONST std::wstring& filename);
	// Methods for the diligent who properly initialized the object beforehand
	bool Load(CONST std::wstring& filename);

	CONST Microsoft::WRL::ComPtr<ID3D11Resource>& GetTextureResource() CONST;
	CONST Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetTextureView() CONST;

/* Private functions */
private:
	// 
	// NOTE: For the time being, I updated the texture to be uncopyable
	//		 and unassignable. --- Wolfe
	//
	Texture(CONST Texture& other) = delete;
	Texture& operator=(CONST Texture& other) = delete;

	int GetFileType(CONST std::wstring& filename) CONST;

	bool LoadDDSTexture(CONST std::wstring& filename);
	bool LoadWICTexture(CONST std::wstring& filename);
};