////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	//qui metto le caratteristiche che voglio poter modificare
	//delle varie istanze di un modello
	struct InstanceType
	{
		D3DXVECTOR3 position;

		//in radianti
		D3DXVECTOR3 rotationAroundXYZ;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetVertexCount();
	int GetInstanceCount();

	//to be removed
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	void GetData(int, D3DXVECTOR3&, D3DXVECTOR3&);
	void updateInstances(int, D3DXVECTOR3(&)[4], D3DXVECTOR3(&)[4], ID3D11DeviceContext*);


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

private:
	ID3D11Buffer* m_instanceBuffer;
	//indexBuffer da rimuovere
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_instanceCount;
	//indexcount da rimuovere
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	ModelType* m_model;

	InstanceType* instances;
};

#endif