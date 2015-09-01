////////////////////////////////////////////////////////////////////////////////
// Filename: light.vs
////////////////////////////////////////////////////////////////////////////////


	/////////////
	// GLOBALS //
	/////////////
	//GLOBAL VARIABLES are implicitly constant
	cbuffer MatrixBuffer
	{
		matrix worldMatrix;
		matrix viewMatrix;
		matrix projectionMatrix;
	};

	cbuffer CameraBuffer
	{
		float3 cameraPosition;
		float padding;
	};

	//////////////
	// TYPEDEFS //
	//////////////
	struct VertexInputType
	{
		float4 position : POSITION;
		float2 tex : TEXCOORD0;
		float3 normal : NORMAL;

		float3 instancePosition : TEXCOORD1;
		float3 instanceRotation : TEXCOORD2;
	};

	struct PixelInputType
	{
		float4 position : SV_POSITION;
		float2 tex : TEXCOORD0;
		float3 normal : NORMAL;
		float3 viewDirection : TEXCOORD1;
	
	};


	////////////////////////////////////////////////////////////////////////////////
	// Vertex Shader
	////////////////////////////////////////////////////////////////////////////////
	PixelInputType LightVertexShader(VertexInputType input)
	{
    PixelInputType output;
	float4 worldPosition;
    

	

	matrix <float, 4, 4> rotationAroundY = {	cos(input.instanceRotation.y), 0.0f, -sin(input.instanceRotation.y), 0.0f,
										0.0f, 1.0f, 0.0f, 0.0f,
										sin(input.instanceRotation.y), 0.0f, cos(input.instanceRotation.y), 0.0f,
										0.0f, 0.0f, 0.0f, 1.0f	
                               };

	matrix <float, 4, 4> composition;

		
	matrix <float, 4, 4> translation =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		input.instancePosition.x, input.instancePosition.y, input.instancePosition.z, 1.0f
		
	};
	
	
	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	composition = mul(rotationAroundY,translation);
	output.position = mul(input.position,composition);	

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(output.position, worldMatrix	);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
    
	//you need to multiply also the normals by the composition matrix, normal vectors must be updated before sending them to the pixel shader.
	output.normal = mul(input.normal,composition);
	// Calculate the normal vector against the world matrix only.
    output.normal = mul(output.normal, (float3x3)worldMatrix);
	// Normalize the normal vector.
    output.normal = normalize(output.normal);

	worldPosition = mul(input.position, worldMatrix);
	output.viewDirection  = cameraPosition.xyz - worldPosition.xyz;
	output.viewDirection = normalize(output.viewDirection);


    return output;
}