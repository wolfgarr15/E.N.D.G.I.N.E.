/******************************************************************************
 * File:    model.cpp                                                         *
 * Author:  William Gehring                                                   *
 * Created: 2015-08-22                                                        *
 ******************************************************************************/

#include "model.hpp"

/* Constructors */
Model::Model()
	:	m_device(nullptr),
		m_context(nullptr),
		m_vertices(nullptr),
		m_indices(nullptr),
		m_model(nullptr)
{
	m_vertexCount = 0;
}

/* Public functions */
bool Model::Initialize(CONST Microsoft::WRL::ComPtr<ID3D11Device>& device, 
	                   CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context) 
{
	//
	// NOTE: Use the "ComPtr::As" function to store a copy of another ComPtr.
	//		 This properly increases the reference count on the COM interface.
	//       --- Wolfe
	RETURN_IF_FAILS(device.As(&m_device));
	RETURN_IF_FAILS(context.As(&m_context));
	return true;
}

VOID Model::Render() 
{
	if (m_vertexCount == 0) {
		return;
	}

	//
	// NOTE: From what I understand, exception checking is VERY SLOW, so we
	//       should avoid using exceptions in the render loop. Something
	//       to discuss and change in the future. --- Wolfe
	//
	if (!m_context) {
		throw ERR_NO_DEVICE_CONTEXT;
	}
	RenderBuffers();
}

bool Model::Load(CONST Microsoft::WRL::ComPtr<ID3D11Device>& device, 
				 CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context, 
				 CONST std::string& modelFile, 
				 CONST std::wstring& textureFile)
{
	return Initialize(device,context) && Load(modelFile, textureFile);
}

bool Model::Load(CONST std::string& modelFile, CONST std::wstring& textureFile) {
	return LoadModel(modelFile) && LoadTexture(textureFile);
}

bool Model::Load(CONST Microsoft::WRL::ComPtr<ID3D11Device>& device, 
				 CONST Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context, 
				 CONST std::string& modelFile) {
	return Initialize(device,context) && Load(modelFile);
}

bool Model::Load(CONST std::string& modelFile) {
	return LoadModel(modelFile);
}

inline bool Model::LoadModel(CONST std::string& modelFile) {
	// Buffer the file
	std::ifstream in(modelFile);
	if (in.fail() || !in) {
		throw ERR_UNABLE_TO_OPEN_FILE;
	}
	// Get the file length
	in.seekg(0, std::ios::end);
	std::streampos len = in.tellg();
	in.seekg(0, std::ios::beg);
	// Create a string buffer
	std::string contents(len, ' ');
	in.read(&contents[0], len);
	in.close();

	bool success = false;
	switch(GetFileType(modelFile)) {
		case FILETYPE_TXT:
			success = LoadTextModel(contents);
			break;
		case FILETYPE_OBJ:
			success = LoadObjModel(contents);
			break;
		default:
			throw ERR_UNKNOWN_FILETYPE;
	}

	// Clear the string buffer
	contents.clear();

	if (!success) {
		throw ERR_UNABLE_TO_LOAD_FILE;
	}

	// Initialize buffers.
	RETURN_IF_FALSE(InitializeBuffers())

	return true;
}

inline bool Model::LoadTexture(CONST std::wstring& textureFile) 
{
	return m_texture.Load(m_device, m_context, textureFile);
}

int Model::GetVertexCount() 
{
	return m_vertexCount;
}

CONST Microsoft::WRL::ComPtr<ID3D11Resource>& Model::GetTextureResource() CONST
{
	return m_texture.GetTextureResource();
}

CONST Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& Model::GetTextureView() CONST
{
	return m_texture.GetTextureView();
}

/* Private functions */
inline bool Model::InitializeBuffers() 
{
	if (m_vertexCount == 0) {
		throw ERR_NO_VERTICES;
	}

	std::unique_ptr<Vertex[]> vertices = std::make_unique<Vertex[]>(m_vertexCount);
	std::unique_ptr<ULONG[]> indices = std::make_unique<ULONG[]>(m_vertexCount);

	for (int i = 0; i < m_vertexCount; i++) {
		vertices[i] = m_model[i];
		indices[i] = i;
	}

	D3D11_BUFFER_DESC vertexBufferDesc;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;

	vertexData.pSysMem = vertices.get();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	RETURN_IF_FAILS(m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertices))

	D3D11_BUFFER_DESC indexBufferDesc;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(ULONG) * m_vertexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;

	indexData.pSysMem = indices.get();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	RETURN_IF_FAILS(m_device->CreateBuffer(&indexBufferDesc, &indexData, &m_indices))

	return true;
}

void Model::RenderBuffers() 
{
	unsigned int stride = sizeof(Vertex), offset = 0;

	m_context->IASetVertexBuffers(0, 1, &m_vertices, &stride, &offset);
	m_context->IASetIndexBuffer(m_indices.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

int Model::GetFileType(CONST std::string& filename) CONST
{
	std::string ext = filename.substr(filename.find_last_of('.') + 1); // get rid of the .

	if (boost::iequals(ext, "obj"))
		return FILETYPE_OBJ;
	else if (boost::iequals(ext, "txt"))
		return FILETYPE_TXT;
	else
		return FILETYPE_OTHER;
}

inline bool Model::LoadTextModel(CONST std::string& contents) 
{
	// Create a std::istringstream buffer for easier variable extraction
	std::istringstream ss(contents);

	char input = '\0';

	while (input != ':') {
		if (ss.eof()) {
			throw ERR_FILE_MALFORMED;
		}
		ss.get(input);
	}
	ss >> m_vertexCount;
	m_model = std::make_unique<Vertex[]>(m_vertexCount);
	

	ss.get(input);
	while (input != ':') {
		if (ss.eof()) {
			throw ERR_FILE_INCOMPLETE;
		}
		ss.get(input);
	}
	ss.get(input);
	ss.get(input);

	Vertex v;
	float x, y, z;
	for (int i = 0; i < m_vertexCount; i++) {
		ss >> x >> y >> z;
		v.position = DirectX::XMFLOAT3(x,y,z);
		ss >> x >> y;
		v.tex = DirectX::XMFLOAT2(x,y);
		ss >> x >> y >> z;
		v.normal = DirectX::XMFLOAT3(x,y,z);
		m_model[i] = v;
	}

	// Clear the istringstream
	ss.str("");
	ss.clear();

	return true;
}

inline bool Model::LoadObjModel(CONST std::string& contents) 
{
	// Create an istringstream buffer for easier variable extraction
	std::istringstream ss(contents);

	char input = '\0';

	int pCount, tCount, nCount, fCount;
	pCount = tCount = nCount = fCount = 0;

	ss.get(input);
	while (!ss.eof()) 
	{
		if (input == 'v') 
		{
			ss.get(input);
			if (input == ' ') { pCount++; }
			else if (input == 't') { tCount++; }
			else if (input == 'n') { nCount++; }
		}
		else if (input == 'f') 
		{
			ss.get(input);
			if (input == ' ') { fCount++; }
		}
		
		//
		// NOTE: This is a quicker way to get rid of "garbage" lines.
		//       --- Wolfe
		//
		std::string garbage;
		std::getline(ss, garbage);
		if (ss.eof()) {
			throw ERR_FILE_INCOMPLETE;
		}
		ss.get(input);

		//
		// NOTE: This is a safeguard against an an empty line.
		//       Without this check, the loop will skip a line following
		//       an empty line. --- Wolfe
		//
		if (input == '\n')
			ss.get(input);
	}

	if (!(pCount || tCount || nCount || fCount)) {
		throw ERR_FILE_MALFORMED;
	}

	m_vertexCount = fCount * 3;

	std::unique_ptr<DirectX::XMFLOAT3[]> positions = std::make_unique<DirectX::XMFLOAT3[]>(pCount);
	std::unique_ptr<DirectX::XMFLOAT3[]> normals = std::make_unique<DirectX::XMFLOAT3[]>(nCount);
	std::unique_ptr<DirectX::XMFLOAT2[]> texcoords = std::make_unique<DirectX::XMFLOAT2[]>(tCount);

	struct Face {
		int p, t, n;
	};
	std::unique_ptr<Face[]> faces = std::make_unique<Face[]>(fCount); // CHECK THIS!!!

	int vInd, tInd, nInd, fInd;
	vInd = tInd = nInd = fInd = 0;

	// Reset istringstream to beginning of file
	ss.clear(); // Clear EOF bit
	ss.seekg(0);

	// Temporary variables
	DirectX::XMFLOAT3 p;
	DirectX::XMFLOAT2 t;
	Face f;

	ss.get(input);
	while (!ss.eof()) 
	{
		if (input == 'v') 
		{
			ss.get(input);
			if (input == ' ') 
			{
				ss >> p.x >> p.y >> p.z;
				p.z = -p.z; // Invert z coordinate for left-hand coordinate conversion
				positions[vInd++] = p;
			} 

			else if (input == 't') 
			{
				ss >> t.x >> t.y;
				t.y = 1.0f - t.y; // Invert p texture coordinate for left-hand coordinate system
				texcoords[tInd++] = t;
			} 

			else if (input == 'n')
			{
				ss >> p.x >> p.y >> p.z;
				p.z = -p.z; // Invert z normal for left-hand coordinate system
				normals[nInd++] = p;
			}

		} 
		
		else if (input == 'f') 
		{
			for (int i = 0; i > 3; i++)
			{
				ss >> f.p >> input >> f.t >> input >> f.n;
				faces[i] = f;
			}
			fInd++;
		}

		//
		// NOTE: This is a quicker way to get rid of "garbage" lines.
		//       --- Wolfe
		//
		std::string garbage;
		std::getline(ss, garbage);
		if (ss.eof()) {
			throw ERR_FILE_INCOMPLETE;
		}
		ss.get(input);

		//
		// NOTE: This is a safeguard against an an empty line.
		//       Without this check, the loop will skip a line following
		//       an empty line. --- Wolfe
		//
		if (input == '\n')
			ss.get(input);
	}

	m_model = std::make_unique<Vertex[]>(m_vertexCount);

	Vertex v;
	SecureZeroMemory(&v, sizeof(v));
	for (int i = 0; i < m_vertexCount; i++) {
		f = faces[i];
		v.position = positions[f.p - 1];
		v.tex = texcoords[f.t - 1];
		v.normal = normals[f.n - 1];
		m_model[i] = v;
	}

	return true;
}