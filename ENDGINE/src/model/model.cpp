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

Model::Model(const Model& other) {}

Model::~Model() {}

/* Public functions */
inline bool Model::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) {
	m_device = device;
	m_context = context;

	return true;
}

inline void Model::Render() {
	Render(m_context);
}

void Model::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) {
	if (m_vertexCount == 0) {
		return;
	}
	if (!context) {
		throw ERR_NO_DEVICE_CONTEXT;
	}
	RenderBuffers(context);
}

bool Model::Load(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, WCHAR* modelFile, WCHAR* textureFile) {
	return Initialize(device,context) && Load(WcharToString(modelFile), textureFile);
}

bool Model::Load(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, char* modelFile, WCHAR* textureFile) {
	return Initialize(device,context) && Load((std::string*) modelFile, textureFile);
}

bool Model::Load(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::string* modelFile, WCHAR* textureFile) {
	return Initialize(device,context) && Load(modelFile, textureFile);
}

inline bool Model::Load(WCHAR* modelFile, WCHAR* textureFile) {
	return LoadModel(modelFile) && LoadTexture(textureFile);
}

inline bool Model::Load(char* modelFile, WCHAR* textureFile) {
	return LoadModel(modelFile) && LoadTexture(textureFile);
}

inline bool Model::Load(std::string* modelFile, WCHAR* textureFile) {
	return LoadModel(modelFile) && LoadTexture(textureFile);
}

bool Model::Load(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, WCHAR* modelFile) {
	return Initialize(device,context) && Load(modelFile);
}

bool Model::Load(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, char* modelFile) {
	return Initialize(device,context) && Load(modelFile);
}

bool Model::Load(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::string* modelFile) {
	return Initialize(device,context) && Load(modelFile);
}

inline bool Model::Load(WCHAR* modelFile) {
	return Load(WcharToString(modelFile));
}

inline bool Model::Load(char* modelFile) {
	return Load((std::string*) modelFile);
}

inline bool Model::Load(std::string* modelFile) {
	return LoadModel(modelFile);
}

bool Model::LoadModel(WCHAR* filename) {
	return LoadModel(WcharToString(filename));
}

bool Model::LoadModel(char* filename) {
	return LoadModel((std::string*) filename);
}

bool Model::LoadModel(std::string* filename) {
	// Buffer the file
	std::ifstream in(*filename);
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
	switch(GetFileType(*filename)) {
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

	if (!InitializeBuffers(m_device)) {
		return false;
	}

	return true;
}

bool Model::LoadTexture(WCHAR* filename) {
	if (!(m_texture = new Texture)) {
		return false;
	}

	return m_texture->Load(m_device, m_context, filename);
}

int Model::GetVertexCount() {
	return m_vertexCount;
}

Microsoft::WRL::ComPtr<ID3D11Resource> Model::GetTexture() {
	return m_texture->GetTexture();
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Model::GetTextureView() {
	return m_texture->GetTextureView();
}

/* Private functions */
inline std::string* Model::WcharToString(WCHAR* wchar) {
	std::wstring ws(wchar);
	return &std::string(ws.begin(),ws.end());
}

inline bool Model::InitializeBuffers() {
	return InitializeBuffers(m_device);
}

inline bool Model::InitializeBuffers(Microsoft::WRL::ComPtr<ID3D11Device> device) {
	if (m_vertexCount == 0) {
		throw ERR_NO_VERTICES;
	}

	Vertex* vertices;
	unsigned long* indices;

	if (!(vertices = new Vertex[m_vertexCount]) || !(indices = new unsigned long[m_vertexCount])) {
		return false;
	}

	for (int i = 0; i < m_vertexCount; i++) {
		vertices[i] = m_model.Get()[i];

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

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertices))) {
		return false;
	}

	delete[] vertices;
	vertices = 0;

	D3D11_BUFFER_DESC indexBufferDesc;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_vertexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indices))) {
		return false;
	}

	delete[] indices;
	indices = 0;

	return true;
}

void Model::RenderBuffers(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) {
	unsigned int stride = sizeof(Vertex), offset = 0;

	context->IASetVertexBuffers(0, 1, &m_vertices, &stride, &offset);
	context->IASetIndexBuffer(m_indices.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

int Model::GetFileType(const std::string filename) {
	std::string ext = filename.substr(filename.find_last_of('.') + 1); // get rid of the .

	if (boost::iequals(ext, "obj"))
		return FILETYPE_OBJ;
	else if (boost::iequals(ext, "txt"))
		return FILETYPE_TXT;
	else
		return FILETYPE_OTHER;
}

bool Model::LoadTextModel(const std::string contents) {
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

	if (!(m_model = new Vertex[m_vertexCount])) {
		return false;
	}

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
		m_model.Get()[i] = v;
	}

	// Clear the istringstream
	ss.str("");
	ss.clear();

	return true;
}

bool Model::LoadObjModel(const std::string contents) {
	// Create an istringstream buffer for easier variable extraction
	std::istringstream ss(contents);

	char input = '\0';

	int pCount, tCount, nCount, fCount;
	pCount = tCount = nCount = fCount = 0;

	ss.get(input);
	while (!ss.eof()) {
		if (input == 'v') {
			ss.get(input);
			if (input == ' ') { pCount++; }
			else if (input == 't') { tCount++; }
			else if (input == 'n') { nCount++;}
		} else if (input == 'f') {
			ss.get(input);
			if (input == ' ') { fCount++; }
		}

		while (input != '\n') {
			if (ss.eof()) {
				throw ERR_FILE_INCOMPLETE;
			}
			ss.get(input);
		}

		ss.get(input);
	}

	if (!(pCount || tCount || nCount || fCount)) {
		throw ERR_FILE_MALFORMED;
	}

	m_vertexCount = fCount * 3;

	DirectX::XMFLOAT3 *positions, *normals;
	DirectX::XMFLOAT2 *texcoords;
	struct Face {
		int p, t, n;
	} *faces;

	if (!(positions = new DirectX::XMFLOAT3[pCount]) || !(texcoords = new DirectX::XMFLOAT2[tCount]) || !(normals = new DirectX::XMFLOAT3[tCount]) || !(faces = new Face[m_vertexCount])) {
		return false;
	}

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
	while (!ss.eof()) {
		if (input == 'v') {
			ss.get(input);
			if (input == ' ') {
				ss >> p.x >> p.y >> p.z;
				p.z = -p.z; // Invert z coordinate for left-hand coordinate conversion
				positions[vInd++] = p;
			} else if (input == 't') {
				ss >> t.x >> t.y;
				t.y = 1.0f - t.y; // Invert p texture coordinate for left-hand coordinate system
				texcoords[tInd++] = t;
			} else if (input == 'n') {
				ss >> p.x >> p.y >> p.z;
				p.z = -p.z; // Invert z normal for left-hand coordinate system
				normals[nInd++] = p;
			}
		} else if (input == 'f') {
			int t = fInd;
			for (int i = t + 2; i >= t; i--) {
				ss >> f.p >> input >> f.t >> input >> f.n;
				faces[i] = f;
				fInd++;
			}
		}

		while (input != '\n') {
			if (ss.eof()) {
				throw ERR_FILE_INCOMPLETE;
			}
			ss.get(input);
		}

		ss.get(input);
	}

	// Clear the istringstream
	ss.str("");
	ss.clear();

	if (!(m_model = new Vertex[m_vertexCount])) {
		return false;
	}

	Vertex v;
	int j = 0;
	for (int i = 0; i < m_vertexCount; i++) {
		f = faces[i];
		v.position = positions[f.p - 1];
		v.tex = texcoords[f.t - 1];
		v.normal = normals[f.n - 1];
		m_model.Get()[j++] = v;
	}

	delete[] positions;
	positions = 0;

	delete[] normals;
	normals = 0;

	delete[] texcoords;
	texcoords = 0;

	delete[] faces;
	faces = 0;

	return true;
}