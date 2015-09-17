/******************************************************************************
 * File:    model.hpp                                                         *
 * Author:  William Gehring                                                   *
 * Created: 2015-08-22                                                        *
 *                                                                            *
 * A general-purpose model class for a game engine, with the ability to read  *
 * multiple file formats.                                                     *
 ******************************************************************************/

#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <boost/algorithm/string.hpp>

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
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;
	ID3D11Buffer *m_vertices, *m_indices;
	int m_vertexCount;
	Texture* m_texture;
	Vertex* m_model;

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
	Model(const Model&);
	~Model();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*);
	void Release(); // Aliases `void Unload()` inside
	void Render();
	void Render(ID3D11DeviceContext*);

	// Dual-file load methods for model formats that don't contain references to their own textures
	// Methods for the lazy who don't initialize the object beforehand
	bool Load(ID3D11Device*, ID3D11DeviceContext*, WCHAR*, WCHAR*);
	bool Load(ID3D11Device*, ID3D11DeviceContext*, char*, WCHAR*);
	bool Load(ID3D11Device*, ID3D11DeviceContext*, std::string*, WCHAR*);
	// Methods for the diligent who properly initialized the object beforehand
	bool Load(WCHAR*, WCHAR*);
	bool Load(char*, WCHAR*);
	bool Load(std::string*, WCHAR*);

	// Single-file load methods for model formats that contain references to their own textures
	// Methods for the lazy who don't initialize the object beforehand
	bool Load(ID3D11Device*, ID3D11DeviceContext*, WCHAR*);
	bool Load(ID3D11Device*, ID3D11DeviceContext*, char*);
	bool Load(ID3D11Device*, ID3D11DeviceContext*, std::string*);
	// Methods for the diligent who properly initialized the object beforehand
	bool Load(WCHAR*);
	bool Load(char*);
	bool Load(std::string*);

	/* FUTURE: Make these functions private? */
	bool LoadModel(WCHAR*);
	bool LoadModel(char*);
	bool LoadModel(std::string*);
	bool LoadTexture(WCHAR*);

	int GetVertexCount();
	ID3D11Resource* GetTexture();
	ID3D11ShaderResourceView* GetTextureView();

	// Unload various bits of the model
	void Unload();        // Unload entire model & texture
	/* FUTURE: Make these functions private? */
	void UnloadModel();   // Unload just the model
	void UnloadTexture(); // Unload just the texture

/* Private functions */
private:
	// Convert WCHAR* to std::string*
	std::string* WcharToString(WCHAR*);

	// Initialize buffers
	bool InitializeBuffers();
	bool InitializeBuffers(ID3D11Device*);
	void ReleaseBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	// Get the filetype from the file extension
	int GetFileType(const std::string);

	// Loading methods for various file formats
	bool LoadTextModel(const std::string);
	bool LoadObjModel(const std::string);

	// Release various aspects of the model
	void ReleaseModel();
	void ReleaseTexture();
};