///////////////////////////////////
// Filename: Camera.cpp
//
// Author: Wolfe S. Greene
///////////////////////////////////

////////////////////////////////////////////////////////////////
// Class Header
//-------------------------------------------------------------
#include "Camera.hpp"

///////////////////////////////////////////////////////////////////
// Class Definition
//-----------------------------------------------------------------
Camera::Camera(CONST DirectX::XMFLOAT3& position,
			   CONST DirectX::XMFLOAT3& rotation,
			   CONST DirectX::XMFLOAT3& lookAxis,
			   CONST DirectX::XMFLOAT3& upAxis)
	: m_position(position),
	  m_rotation(rotation)
{
	m_lookVector = DirectX::XMLoadFloat3(&lookAxis);
	m_upVector = DirectX::XMLoadFloat3(&upAxis);
	SecureZeroMemory(&m_viewMatrix, sizeof(DirectX::XMMATRIX));
}

Camera::Camera(CONST Camera& other)
	: m_position(other.m_position),
	  m_rotation(other.m_rotation),
	  m_lookVector(other.m_lookVector),
	  m_upVector(other.m_upVector),
	  m_viewMatrix(other.m_viewMatrix)
{
	// DO NOTHING.
}

Camera& Camera::operator=(Camera& other)
{
	swap(other);
	return *this;
}

VOID Camera::swap(Camera& other)
{
	std::swap(m_position, other.m_position);
	std::swap(m_rotation, other.m_rotation);
	std::swap(m_lookVector, other.m_lookVector);
	std::swap(m_upVector, other.m_upVector);
	std::swap(m_viewMatrix, other.m_viewMatrix);
}

VOID Camera::GetPosition(DirectX::XMFLOAT3& position) CONST
{
	position = m_position;
}

VOID Camera::GetRotation(DirectX::XMFLOAT3& rotation) CONST
{
	rotation = m_rotation;
}

VOID Camera::GetViewMatrix(DirectX::XMMATRIX& viewMatrix) CONST
{
	viewMatrix = m_viewMatrix;
}

VOID Camera::SetPosition(CONST DirectX::XMFLOAT3& position)
{
	m_position = position;
}

VOID Camera::SetRotation(CONST DirectX::XMFLOAT3& rotation)
{
	m_rotation = rotation;
}

VOID Camera::RenderView()
{
	// Load position data into an XMVECTOR structure.
	DirectX::XMVECTOR positionVector = DirectX::XMLoadFloat3(&m_position);

	// Load the rotation data into an XMMatrix structure.
	//
	// NOTE: m_rotation's components are assumed to already be in unitless radians.
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x,
																			 m_rotation.y,
																			 m_rotation.z);

	// Transform the look and up vectors by the rotationMatrix.
    DirectX::XMVECTOR lookVector = DirectX::XMVector3TransformCoord(m_lookVector, rotationMatrix);
	DirectX::XMVECTOR upVector = DirectX::XMVector3TransformCoord(m_upVector, rotationMatrix);

	// Translate the rotated "camera' (the looking vector) to the final viewing position.
	lookVector = DirectX::XMVectorAdd(positionVector, lookVector);

	// Calculate the updated view matrix.
	m_viewMatrix = DirectX::XMMatrixLookAtLH(positionVector, lookVector, upVector);
}
