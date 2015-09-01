////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include <dinput.h>

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_Text = 0;
	m_Frustum = 0;

	counter = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{

	
	bool result;
	D3DXMATRIX baseViewMatrix;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);


	// Create the model object.
	m_Model = new ModelClass;
	if(!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), "../Engine/data/t-axe.txt", L"../Engine/data/t-axe.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	//m_Light->SetAmbientColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_Light->SetAmbientColor(0.3f, 0.3f, 0.3f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	//OLD ONE m_Light->SetDirection(0.0f,0.0f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 0.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);


	// Create the frustum object.
	m_Frustum = new FrustumClass;
	if (!m_Frustum)
	{
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame(float runningTime)
{
		D3DXVECTOR3 position;
		bool result;
		static float rotation = 0.0f;

		
			
		// Update the rotation variable each frame.
		//rotation += (float)D3DX_PI * 0.01f;
		rotation += (float)D3DX_PI * 0.001f;
				
		if (rotation > 360.0f)
		{
			rotation -= 360.0f;
		}

		
		// Render the graphics scene.
		result = Render(rotation,runningTime);
		if (!result)
		{
			return false;
		}
	


	return true;
}


bool GraphicsClass::Render(float rotation, float runningTime)
{


	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	int modelCount, renderCount, index;
	float radius;
	D3DXVECTOR3 instancePosition, instanceRotation;

	bool renderInstance, result;
	D3DXVECTOR3 tempInstancesPosition[4];
	D3DXVECTOR3 tempInstancesRotation[4];

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);


	//move the camera based on the keyboard



	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);
	
	// Construct the frustum.
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	// Get the number of istances of the model that will be rendered.
	//modelCount = m_Model->GetIstancesCount();
	modelCount = 4;
	// Initialize the count of models that have been rendered.
	renderCount = 0;

	int temp = 0;

	// Go through all the models and render them only if they can be seen by the camera view.
	for (index = 0; index < modelCount; index++)
	{
		//recupero la posizione dell'ennesima istanza del modello e la salvo in instancePosition
		m_Model->GetData(index,instancePosition,instanceRotation);

		// Set the radius of the sphere to 1.0 since this is already known.
		radius = 0.82f;

		// Check if the sphere model is in the view frustum.
		renderInstance = m_Frustum->CheckRectangle(instancePosition.x, instancePosition.y, instancePosition.z, 0.8f,2.0f,0.8f);

		// If it can be seen then render it, if not skip this model and check the next sphere.
		if (renderInstance)
		{

			tempInstancesPosition[renderCount] = instancePosition;
			tempInstancesRotation[renderCount] = instanceRotation;
			// Add one to the number of trees to draw
			renderCount++;
		}

	}


	//adesso aggiorno il buffer contenente le istanze della mesh

	m_Model->updateInstances(renderCount, tempInstancesPosition,tempInstancesRotation, m_D3D->GetDeviceContext());

	// Rotate the world matrix by the rotation value so that the 3d model will spin.
	//D3DXMatrixRotationY(&worldMatrix, rotation);
	/*
	// translate the world matrix 
	//D3DXMatrixTranslation(&worldMatrix, 1.5f, 1.5f, -5.0f);
	D3DXMATRIX MatTemp;  // Temp matrix for rotations.
	D3DXMATRIX MatRot;   // Final rotation matrix, applied to // pMatWorld.

	// Using the left-to-right order of matrix concatenation,
	// apply the translation to the object's world position
	// before applying the rotations.
	D3DXMatrixTranslation(&worldMatrix, -1.5f, -1.5f, 5.0f);
	D3DXMatrixIdentity(&MatRot);
	D3DXMatrixIdentity(&MatTemp);

	// Now, apply the orientation variables to the world matrix
	D3DXMatrixRotationY(&MatTemp, rotation);           // Yaw
	D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
	// Apply the rotation matrices to complete the world matrix.
	D3DXMatrixMultiply(&worldMatrix, &MatRot, &worldMatrix);
	*/

	
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader. m_Model->GetInstanceCount()
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetVertexCount(), renderCount , worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());




	//AGGIORNAMENTO INTERFACCIA 2D

	// Set the number of models that was actually rendered this frame.
	//result = m_Text->SetRenderCount(runningTime,m_D3D->GetDeviceContext());
	result = m_Text->updateText(0,runningTime, m_D3D->GetDeviceContext());
	result = m_Text->updateText(1, renderCount, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	
	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	//è un passo necessario per fare in modo che non venga renderizzato lo sfondo nero rettangolare attorno ai caratteri
	//ma venga invece fatto un blend tra il carattere e il rendering 3d dietro.
	m_D3D->TurnOnAlphaBlending();

	// Render the text string of the render count.
	m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	if(!result)
	{
		return false;
	}

	//RENDERIZZA LA SCENA
	// Present the rendered scene to the screen.
	m_D3D->EndScene();
	
	return true;
}