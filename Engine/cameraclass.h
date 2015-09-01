////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void MoveForward(float units);
	void Strafe(float units);
	void Yaw(float radians);
	void Pitch(float radians);

	//probabilmente inutile
	void SetLookAt(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	float GetPitch()                   { return m_pitch; }
	float GetYaw()                     { return m_yaw; }

	void Render();
	void GetViewMatrix(D3DXMATRIX&);

private:
	//float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;

	D3DXVECTOR3 m_lookAt;
	D3DXVECTOR3 m_look;
	D3DXVECTOR3 m_velocity;
	D3DXVECTOR3 m_right;
	D3DXVECTOR3 m_position;

	float       m_yaw;
	float       m_pitch;
	D3DXVECTOR3 m_up;
	float       m_maxPitch;
	BOOL        m_invertY;

	BOOL        m_enableYMovement;
	float       m_maxVelocity;
};

#endif