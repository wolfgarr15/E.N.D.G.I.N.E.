///////////////////////////////////
// Filename: Camera.hpp
//
// Author: Wolfe S. Greene
///////////////////////////////////

////////////////////////////////
// Pre-processor Directives
//------------------------------
#pragma once

///////////////////////////////
// Include Headers
//-----------------------------
#include "..\globals\LeanWindows.h"
#include <algorithm>
#include <directxmath.h>

class Camera
{
	DirectX::XMFLOAT3 m_position;    // The camera's (x, y, z) position relative to the world.
	DirectX::XMFLOAT3 m_rotation;    // The camera's (yaw, pitch, roll) rotation/orientation in RADIANS.

	DirectX::XMVECTOR m_lookVector;  // Defines the default viewing direction of the unrotated camera.
	DirectX::XMVECTOR m_upVector;    // Defines the camera's "up" direction.

	DirectX::XMMATRIX m_viewMatrix;  // The view matrix defined by the camera's field of view.

public:
	Camera(CONST DirectX::XMFLOAT3& position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),  // Default to the origin position.
		   CONST DirectX::XMFLOAT3& rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),  // Default to unrotated.
		   CONST DirectX::XMFLOAT3& lookAxis = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),  // Default facing the +z-direction.
		   CONST DirectX::XMFLOAT3& upAxis = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));   // Default "up" as being +y-direction.

	Camera(CONST Camera& other);

	~Camera() = default;

	Camera& operator=(Camera& other);

	VOID swap(Camera& other);

	VOID GetPosition(DirectX::XMFLOAT3& position) CONST;
	VOID GetRotation(DirectX::XMFLOAT3& rotation) CONST;
	VOID GetViewMatrix(DirectX::XMMATRIX& viewMatrix) CONST;

	VOID SetPosition(CONST DirectX::XMFLOAT3& position);
	VOID SetRotation(CONST DirectX::XMFLOAT3& rotation);

	VOID RenderView();
};

