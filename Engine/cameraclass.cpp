////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	//m_positionX = 0.0f;
	//m_positionY = 0.0f;
	//m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_enableYMovement = TRUE;
	m_velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_maxVelocity = 1.0f;
	m_look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_maxPitch = D3DXToRadian(89.0f);
	m_invertY = FALSE;

	m_position = D3DXVECTOR3(0.0f, 0.0f, -20.0f);
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	return;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Moves the camera forward and backward
Parameters:
[in] units - Amount to move
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CameraClass::MoveForward(float units)
{
	if (m_enableYMovement)
	{
		m_velocity += m_look * units;
	}
	else
	{
		D3DXVECTOR3 moveVector(m_look.x, 0.0f, m_look.z);
		D3DXVec3Normalize(&moveVector, &moveVector);
		moveVector *= units;
		m_velocity += moveVector;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Moves the camera left and right
Parameters:
[in] units - Amount to move
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CameraClass::Strafe(float units)
{
	m_velocity += m_right * units;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Yaw the camera around its Y-axis.
Parameters:
[in] radians - Radians to yaw.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CameraClass::Yaw(float radians)
{
	if (radians == 0.0f)
	{
		return;
	}
	D3DXMATRIX rotation;
	//genera una matrice di rotazione attorno all'asse Y di angolo radians
	//la funzione necessità di 2 parametri, l'asse arbitrario attorno a cui effettuare la rotazione
	//e l'angolo in radianti
	D3DXMatrixRotationAxis(&rotation, &m_up, radians);
	//applico la matrice di rotazione appena calcolata ai due vettori right e look della camera
	D3DXVec3TransformNormal(&m_right, &m_right, &rotation);
	D3DXVec3TransformNormal(&m_look, &m_look, &rotation);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Pitch the camera around its X-axis.
Parameters:
[in] radians - Radians to pitch.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CameraClass::Pitch(float radians)
{
	if (radians == 0.0f)
	{
		return;
	}

	radians = (m_invertY) ? -radians : radians;
	m_pitch -= radians;
	if (m_pitch > m_maxPitch)
	{
		radians += m_pitch - m_maxPitch;
	}
	else if (m_pitch < -m_maxPitch)
	{
		radians += m_pitch + m_maxPitch;
	}


	D3DXMATRIX rotation;
	//calcolo una matrice di rotazione attorno all'asse X di angolo radians
	D3DXMatrixRotationAxis(&rotation, &m_right, radians);
	//applico la matrice di rotazione ai due vettori up e look della camera
	D3DXVec3TransformNormal(&m_up, &m_up, &rotation);
	D3DXVec3TransformNormal(&m_look, &m_look, &rotation);
}

void CameraClass::SetLookAt(float x, float y, float z)
{
	/*lookAt.x = x;
	lookAt.y = y;
	lookAt.z = z;*/
	return;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_position.x, m_position.y, m_position.z);
}


D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}


void CameraClass::Render()
{
	//D3DXVECTOR3 up, position, lookAt;
	D3DXVECTOR3 up;
	//float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;


	// Cap velocity to max velocity
	if (D3DXVec3Length(&m_velocity) > m_maxVelocity)
	{
		m_velocity = *(D3DXVec3Normalize(&m_velocity, &m_velocity)) * m_maxVelocity;
	}

	// Move the camera
	m_position += m_velocity;
	// Could decelerate here. I'll just stop completely.
	//se non resetto m_velocity alla riga seguente, premendo un tasto di spostamento una sola volta
	//otterrò uno spostamento della camera anche nei frame successivi, pur non premendo più tasti di spostamento.
	//mentre io voglio che la posizione della camera si incrementi UNA sola volta
	//in seguito alla pressione di un tasto di spostamento.
	m_velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_lookAt = m_position + m_look;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &m_lookAt, &up);


	//questa seconda parte del codice serve per aggiornare m_pitch,
	//ovvero l'angolo, in radianti, compreso tra il piano XZ
	//e il vettore lookAt
	//m_pitch viene usato nella funzione CameraClass::Pitch alle successive iterazioni

	// Set the camera axes from the view matrix
	//first column
	m_right.x = m_viewMatrix._11;
	m_right.y = m_viewMatrix._21;
	m_right.z = m_viewMatrix._31;
	//second column
	m_up.x = m_viewMatrix._12;
	m_up.y = m_viewMatrix._22;
	m_up.z = m_viewMatrix._32;
	//third column
	m_look.x = m_viewMatrix._13;
	m_look.y = m_viewMatrix._23;
	m_look.z = m_viewMatrix._33;

	// Calculate pitch
	//length of the look vector on the XZ plan (projection)
	float lookLengthOnXZ = sqrtf(m_look.z * m_look.z + m_look.x * m_look.x);
	m_pitch = atan2f(m_look.y, lookLengthOnXZ);
	//commentato perché apparentemente non serve a un cazzo
	//m_yaw = atan2f(m_look.x, m_look.z);

	return;
}


void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}