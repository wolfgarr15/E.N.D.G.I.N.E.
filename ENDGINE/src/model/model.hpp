/******************************************************************************
 * File:    model.hpp                                                         *
 * Author:  William Gehring                                                   *
 * Created: 2015-08-22                                                        *
 *                                                                            *
 * Modified: Wolfe S. Greene                                                  *
 *                                                                            *
 * A general-purpose model class for a game engine, with the ability to read  *
 * multiple file formats.                                                     *
 ******************************************************************************/

#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <wrl.h>

#include "../globals/functions.hpp"
#include "../globals/macros.h"
#include "./texture/texture.hpp"

class Model {
/* Private data types */
private:
	struct Vertex {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 tex;
		DirectX::XMFLOAT3 normal;
	};

/* Private member variables */
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertices;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indices;

	int m_vertexCount;

	Texture m_texture;

	//
	// NOTE: Use std::unique_ptr or std::shared_ptr when
	//       applicable, and avoid using raw pointers when
	//       you don't need to. --- Wolfe
	//
	std::unique_ptr<Vertex[]> m_model;

/* Private enumerations */
private:
	enum filetypes {
		FILETYPE_OBJ,
		FILETYPE_TXT,
		FILETYPE_OTHER
	};
	enum errs {
		ERR_FILE_INCOMPLETE,
		ERR_FILE_MALFORMED,
		ERR_MEM_ALLOC,
		ERR_NO_DEVICE,
		ERR_NO_DEVICE_CONTEXT,
		ERR_NO_VERTICES,
		ERR_NOT_IMPLEMENTED,
		ERR_UNABLE_TO_LOAD_FILE,
		ERR_UNABLE_TO_OPEN_FILE,
		ERR_UNKNOWN_FILETYPE
	};

/* Constructors and public functions */
public:
	Model();
	~Model() = default;

	//
	// NOTE: Since ComPtrs are objects (and rather robust ones, from what I understand...),
	//       it's best to pass them by reference. Not only is it more efficient, but it actually
	//       makes the syntax a bit cleaner when you access the ComPtrs contents. --- Wolfe
	//
	bool Initialize(CONST Microsoft::WRL::ComPtr<ID3D11Device>& device, 
					CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context);
	void Render();

	// Dual-file load methods for model formats that don't contain references to their own textures
	// Method for the lazy who don't initialize the object beforehand
	bool Load(CONST Microsoft::WRL::ComPtr<ID3D11Device>& device, 
			  CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context, 
			  CONST std::string& modelFile, 
			  CONST std::wstring& textureFile);

	// Method for the diligent who properly initialized the object beforehand
	bool Load(CONST std::string& modelFile, CONST std::wstring& textureFile);

	// Single-file load methods for model formats that contain references to their own textures
	// Methods for the lazy who don't initialize the object beforehand
	bool Load(CONST Microsoft::WRL::ComPtr<ID3D11Device>& device, 
			  CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context, 
			  CONST std::string& modelFile);
	// Methods for the diligent who properly initialized the object beforehand
	bool Load(CONST std::string& modelFile);

	int GetVertexCount();
	CONST Microsoft::WRL::ComPtr<ID3D11Resource>& GetTextureResource() CONST;
	CONST Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetTextureView() CONST;

/* Private functions */
private:
	// 
	// NOTE: For the time being, I updated the model to be uncopyable
	//		 and unassignable. We will no doubt need to change this, but
	//       we need to discuss our ownership policy for the texture and
	//       the model. --- Wolfe
	//
	Model(CONST Model& other) = delete;
	Model& operator=(CONST Model& other) = delete;

	bool LoadModel(CONST std::string& modelFile);
	bool LoadTexture(CONST std::wstring& textureFile);

	// Initialize buffers
	bool InitializeBuffers();
	void RenderBuffers();

	// Get the filetype from the file extension
	int GetFileType(CONST std::string& filename) CONST;

	// Loading methods for various file formats
	bool LoadTextModel(CONST std::string& objFilename);
	bool LoadObjModel(CONST std::string& txtFilename);
};