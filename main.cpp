
#include <windows.h>
#include <math.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment (lib, "D3D10_1.lib")
#pragma comment (lib, "DXGI.lib")
#pragma comment (lib, "D2D1.lib")
#pragma comment (lib, "dwrite.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment(lib,"d3dcompiler.lib")
#include<chrono>

#include <D3D9Types.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include"bth_image.h"
#include"DDSTextureLoader.h"

using namespace DirectX;


#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#include <dinput.h.>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
#define WIDTH 768.0f
#define HEIGHT 768.0f
#define ZERO(X) ZeroMemory(&X, sizeof(X))

HRESULT CreateDirect3DContext(HWND wndHandle);

IDXGISwapChain* gSwapChain = nullptr;

ID3D11Device* gDevice = nullptr;
ID3D11DeviceContext* gDeviceContext = nullptr;

ID3D11RenderTargetView* gBackbufferRTV = nullptr;
ID3D11Buffer* gVertexBufferFS = nullptr;
ID3D11InputLayout* gVertexLayoutFS = nullptr;
ID3D11InputLayout* gVertexLayoutBFCTest = nullptr;
ID3D11InputLayout* gVertexLayoutBullet = nullptr;

ID3D11Buffer* gVertexBuffer = nullptr;
ID3D11Buffer* gVertexBufferDynamic = nullptr;
ID3D11Buffer* gVertexBufferTwo = nullptr;
ID3D11Buffer* gVertexBufferSM = nullptr;
ID3D11Buffer* gGoalVertexBuffer = nullptr;
ID3D11Buffer* gWaterCubeVertexBuffer = nullptr;
ID3D11Buffer* gDCTestCubeVertexBuffer = nullptr;
ID3D11InputLayout* gVertexLayout = nullptr;
ID3D11InputLayout* gHMLayout = nullptr;
ID3D11InputLayout* gSMLayout = nullptr;
ID3D11InputLayout* gGoalLayout = nullptr;
ID3D11InputLayout* gWaterCubeLayout = nullptr;

ID3D11Texture2D* depthStencilBuffer;
ID3D11Buffer* gDrawDepthCB = NULL;

ID3D11ShaderResourceView* gCubeColourmap = nullptr;
ID3D11ShaderResourceView* gEndscreenTex = nullptr;
ID3D11ShaderResourceView* gCubeDisplacementmap = nullptr;
ID3D11ShaderResourceView* gCubeNormalmap = nullptr;
ID3D11ShaderResourceView* gCubeSpecView = nullptr;
ID3D11ShaderResourceView* gWaterTexture = nullptr;
ID3D11ShaderResourceView* gSkyMapView = nullptr;

ID3D11ShaderResourceView* gHMTexture = nullptr;

ID3D11VertexShader* gVertexShader = nullptr;
ID3D11HullShader* gHullShader = nullptr;
ID3D11DomainShader* gDomainShader = nullptr;
ID3D11PixelShader* gPixelShader = nullptr;
ID3D11VertexShader* gVertexShaderHM = nullptr;
ID3D11PixelShader* gPixelShaderHM = nullptr;
ID3D11PixelShader* gDCPixelShaderHM = nullptr;
ID3D11VertexShader* gVertexShaderFS = nullptr;
ID3D11PixelShader* gPixelShaderFS = nullptr;
ID3D11PixelShader* gPixelShaderEndscreen = nullptr;
ID3D11VertexShader* gVertexShaderSM = nullptr;
ID3D11GeometryShader* gGeometryShader = nullptr;
ID3D11GeometryShader* gGeometryShaderHM = nullptr;
ID3D11VertexShader* gVertexShaderBFCTest = nullptr;
ID3D11VertexShader* gBulletVertexShader = nullptr;
ID3D11VertexShader* gGoalVertexShader = nullptr;
ID3D11VertexShader* gWaterCubeVertexShader = nullptr;
ID3D11VertexShader* gDynamicCubeVertexShader = nullptr;
ID3D11GeometryShader* gGeometryShaderBFCTest = nullptr;
ID3D11PixelShader* gPixelShaderBFCTest = nullptr;
ID3D11PixelShader* gBulletFragmentShader = nullptr;
ID3D11PixelShader* gGoalFragmentShader = nullptr;
ID3D11PixelShader* gWaterCubeFragmentShader = nullptr;
ID3D11PixelShader* gDynamicWaterCubeFragment = nullptr;
ID3D11PixelShader* gDynamicCubeFragmentShader = nullptr;
ID3D11PixelShader* gDCTestCubeFragment = nullptr;
ID3D11VertexShader* gDCTestCubeVertex = nullptr;
ID3D11PixelShader* gPixelShaderSM = nullptr;
ID3D11PixelShader* gDynamicSkymapFragment = nullptr;

ID3D11Buffer* gCubeIndexBuffer = nullptr;

ID3D11Buffer* gIndexBufferHM = nullptr;
ID3D11Buffer* gVertexBufferHM = nullptr;
ID3D11Buffer* gVertexBufferBFCTest = nullptr;

XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
XMVECTOR camForward = DefaultForward;
XMVECTOR camRight = DefaultRight;
XMVECTOR camTarget;
XMVECTOR camUp;
XMVECTOR camPos;
XMMATRIX camRotationMatrix;
XMMATRIX groundWorld;
XMMATRIX camView;
XMMATRIX dynamicCubeUpView;
XMMATRIX dynamicCubeFrontView;
XMMATRIX dynamicCubeBackView;
XMMATRIX dynamicCubeLeftView;
XMMATRIX dynamicCubeRightView;
XMMATRIX lightView;

XMMATRIX rotationX;
XMMATRIX rotationY;

struct dsStruct
{
	XMMATRIX viewProj;
	XMVECTOR camPos;
};

dsStruct* dsData;
XMVECTOR* rotation;
XMVECTOR* camPosValue;
XMVECTOR frontFaceCamPos = XMVectorSet(-12, 0, 2, 1);
XMVECTOR backFaceCamPos = XMVectorSet(-12, 0, 4, 1);
XMVECTOR upFaceCamPos = XMVectorSet(-12, 1, 3, 1);
XMVECTOR leftFaceCamPos = XMVectorSet(-13, 0, 3, 1);
XMVECTOR rightFaceCamPos = XMVectorSet(-11, 0, 3, 1);
XMMATRIX* viewProjData;
XMMATRIX* SkymapWVP;
ID3D11Texture2D* gTextureColour;
ID3D11Texture2D* gTextureNormal;
ID3D11Texture2D* gTexturePosition;
ID3D11Texture2D* gTextureDepth;
ID3D11Texture2D* gDynamicFaceUp;
ID3D11Texture2D* gDynamicFaceFront;
ID3D11Texture2D* gDynamicFaceBack;
ID3D11Texture2D* gDynamicFaceLeft;
ID3D11Texture2D* gDynamicFaceRight;
ID3D11DepthStencilState* gDepthStencilState = nullptr;
ID3D11RenderTargetView* gRenderTargetView[3];
ID3D11RenderTargetView* gDynamicCubeTargetViewUp;
ID3D11RenderTargetView* gDynamicCubeTargetViewFront;
ID3D11RenderTargetView* gDynamicCubeTargetViewBack;
ID3D11RenderTargetView* gDynamicCubeTargetViewLeft;
ID3D11RenderTargetView* gDynamicCubeTargetViewRight;
ID3D11RenderTargetView* gTestTargetView[5];
ID3D11DepthStencilView* gDepthStencilView;
ID3D11DepthStencilState* DSLessEqual;
ID3D11RasterizerState* RSCullNone;
ID3D11ShaderResourceView* gColorViewMap;
ID3D11ShaderResourceView* gNormalViewMap;
ID3D11ShaderResourceView* gPositionViewMap;
ID3D11ShaderResourceView* gShaderResourceViewDepth;
ID3D11ShaderResourceView* gDynamicFaceUpView;
ID3D11ShaderResourceView* gDynamicFaceFrontView;
ID3D11ShaderResourceView* gDynamicFaceBackView;
ID3D11ShaderResourceView* gDynamicFaceLeftView;
ID3D11ShaderResourceView* gDynamicFaceRightView;
ID3D11Buffer* dsBuffer = nullptr;
ID3D11Buffer* psBuffer = nullptr;
ID3D11Buffer* camPosBuffer = nullptr;
ID3D11Buffer* ViewProjBuffer = nullptr;
ID3D11Buffer* rotationBuffer = nullptr;
ID3D11Buffer* projectionBuffer = nullptr;
ID3D11Buffer* viewBuffer = nullptr;
ID3D11Buffer* SkymapWVPBuffer = nullptr;
ID3D11Buffer* BFCTestWorldBuffer = nullptr;
ID3D11Buffer* identityBuffer = nullptr;
ID3D11Buffer* DCViewProjBuffer = nullptr;
ID3D11SamplerState* sampleState;
XMMATRIX cubeWorldMatrix;
XMMATRIX Projection;
XMMATRIX ProjectionDC;
XMMATRIX SkymapWorld;
XMMATRIX Scale;
XMMATRIX Translation;
float gIncrement = 0;
float deltaTime = 0;
bool renderEndscreen = false;
struct HeightMapInfo {
	int terrainWidth;
	int terrainHeight;
	XMFLOAT3 *heightMap;    // Array to store terrain's vertex positions
};
HeightMapInfo hmInfo;

int NumFaces = 0;
int NumVertices = 0;
IDirectInputDevice8* DIKeyboard;
IDirectInputDevice8* DIMouse;
HWND hwnd = NULL;
DIMOUSESTATE mouseLastState;
LPDIRECTINPUT8 DirectInput;

float moveLeftRight = 0.0f;
float moveBackForward = 0.0f;

float camYaw = 0.0f;
float camPitch = 0.0f;

struct HMVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
};
std::vector<HMVertex> HMVertices;
XMFLOAT4 camPosF4;
float oldHeight = INFINITY;
float newHeight = 0;
float heightChange = 0;
XMVECTOR heightChangeVector;
void walkOnHM()
{
	int foundPos = -1;
	for (int i = 0; i < hmInfo.terrainHeight * hmInfo.terrainWidth && foundPos == -1; i++)
	{
		//if (std::round(camPosF4.x) == HMVertices[i].pos.x && std::round(camPosF4.z) == HMVertices[i].pos.z)
		//{
		//	foundPos = i;

		//}
	}
	if (foundPos != -1)
	{
		if (oldHeight == INFINITY)
			oldHeight = HMVertices[foundPos].pos.y;
		else
			oldHeight = newHeight;

		newHeight = HMVertices[foundPos].pos.y;

		heightChange = oldHeight - newHeight;

		heightChangeVector = XMVectorSet(0, heightChange, 0, 0);
		camPos -= heightChangeVector;
	}

}
ID3D11SamplerState* gSamplerState = NULL;
HRESULT CreateFSShaders()
{
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"VertexFS.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pVS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pVS)
			pVS->Release();
		return result;
	}

	gDevice->CreateVertexShader(
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		nullptr,
		&gVertexShaderFS
	);

	// create input layout (verified using vertex shader)
	// Press F1 in Visual Studio with the cursor over the datatype to jump
	// to the documentation online!
	// please read:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb205117(v=vs.85).aspx
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			0,							 // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
		{
			"UV_COORD",
			0,				// same slot as previous (same erer)
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			8,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayoutFS);

	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	//create pixel shader
	ID3DBlob* pPS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"FragmentFS.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pPS)
			pPS->Release();
		return result;
	}

	gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShaderFS);
	// we do not need anymore this COM object, so we release it.
	pPS->Release();

	return S_OK;
}
HRESULT CreateVSPSShaders()
{
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"Vertex.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pVS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pVS)
			pVS->Release();
		return result;
	}

	gDevice->CreateVertexShader(
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		nullptr,
		&gVertexShader
	);

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32A32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			0,							 // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
		{
			"TANGENT",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32A32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			16,							 // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
		{
			"NORMAL",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32A32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			32,							 // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
		{
			"TEXCOORD",
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			48,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	result = gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayout);

	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	//create pixel shader
	ID3DBlob* pPS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"Fragment.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pPS)
			pPS->Release();
		return result;
	}

	gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShader);
	pPS->Release();



	return S_OK;
}
HRESULT CreateHMShaders()
{
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"VertexHM.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pVS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pVS)
			pVS->Release();
		return result;
	}

	gDevice->CreateVertexShader(
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		nullptr,
		&gVertexShaderHM
	);

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			0,							 // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
		{
			"TEXCOORD",
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			12,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	result = gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gHMLayout);

	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	//create pixel shader
	ID3DBlob* pPS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"FragmentHM.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pPS)
			pPS->Release();
		return result;
	}

	gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShaderHM);
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"DCFragmentHM.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pPS)
			pPS->Release();
		return result;
	}

	gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gDCPixelShaderHM);
	return S_OK;
}
HRESULT CreateHSDSShaders()
{
	ID3DBlob* pHS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"Hull.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"HS_main",		// entry point
		"hs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pHS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pHS)
			pHS->Release();
		return result;
	}

	gDevice->CreateHullShader(
		pHS->GetBufferPointer(),
		pHS->GetBufferSize(),
		nullptr,
		&gHullShader
	);
	pHS->Release();

	ID3DBlob* pDS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"DomainShader.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"DS_main",		// entry point
		"ds_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				// effect compile options
		&pDS,			// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pDS)
			pDS->Release();
		return result;
	}

	gDevice->CreateDomainShader(pDS->GetBufferPointer(), pDS->GetBufferSize(), nullptr, &gDomainShader);
	pDS->Release();
}
HRESULT CreateSMShaders()
{
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"VertexSM.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pVS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pVS)
			pVS->Release();
		return result;
	}

	gDevice->CreateVertexShader(
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		nullptr,
		&gVertexShaderSM
	);



	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	//create pixel shader
	ID3DBlob* pPS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"FragmentSM.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pPS)
			pPS->Release();
		return result;
	}

	gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShaderSM);
	pPS->Release();

	result = D3DCompileFromFile(
		L"DynamicSkymapFragment.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pPS)
			pPS->Release();
		return result;
	}

	gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gDynamicSkymapFragment);
	return S_OK;
}
HRESULT CreateBFCShaders()
{
	ID3DBlob* pGS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"BackfaceCulling.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"GS_main",		// entry point
		"gs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreateGeometryShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gGeometryShader
	);

	pGS = nullptr;
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"BackfaceCullingHM.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"GS_main",		// entry point
		"gs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreateGeometryShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gGeometryShaderHM
	);
	return S_OK;
}
HRESULT CreateBulletShaders()
{
	ID3DBlob* pGS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"BulletVertex.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreateVertexShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gBulletVertexShader
	);
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
	{
		"POSITION",		// "semantic" name in shader
		0,				// "semantic" index (not used)
		DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
		0,							 // input slot
		0,							 // offset of first element
		D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
		0							 // used for INSTANCING (ignore)
	}
	};

	result = gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pGS->GetBufferPointer(), pGS->GetBufferSize(), &gVertexLayoutBullet);

	pGS = nullptr;
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"BulletFragment.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreatePixelShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gBulletFragmentShader
	);


	// we do not need anymore this COM object, so we release it.
	pGS->Release();
	return S_OK;
}
HRESULT CreateGoalShaders()
{
	ID3DBlob* pGS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"GoalVertex.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreateVertexShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gGoalVertexShader
	);
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			0,							 // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
		{
			"UV",
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			12,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	result = gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pGS->GetBufferPointer(), pGS->GetBufferSize(), &gGoalLayout);

	pGS = nullptr;
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"GoalFragment.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreatePixelShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gGoalFragmentShader
	);
	result = D3DCompileFromFile(
		L"FragmentEndscreen.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreatePixelShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gPixelShaderEndscreen

	);
	// we do not need anymore this COM object, so we release it.
	pGS->Release();
	return S_OK;
}
HRESULT CreateBFCTestShaders()
{
	ID3DBlob* pGS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"VertexBFCTest.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreateVertexShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gVertexShaderBFCTest
	);
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
	{
		"POSITION",		// "semantic" name in shader
		0,				// "semantic" index (not used)
		DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
		0,							 // input slot
		0,							 // offset of first element
		D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
		0							 // used for INSTANCING (ignore)
	}
	};

	result = gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pGS->GetBufferPointer(), pGS->GetBufferSize(), &gVertexLayoutBFCTest);

	pGS = nullptr;
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"BackfaceCullingTest.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"GS_main",		// entry point
		"gs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreateGeometryShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gGeometryShaderBFCTest
	);

	pGS = nullptr;
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"FragmentBFCTest.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreatePixelShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gPixelShaderBFCTest
	);


	// we do not need anymore this COM object, so we release it.
	pGS->Release();
	return S_OK;
}
HRESULT CreateWaterCubeShaders()
{
	ID3DBlob* pGS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"WaterCubeVertex.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreateVertexShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gWaterCubeVertexShader
	);
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			0,							 // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
		{
			"UV",
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			12,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	result = gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pGS->GetBufferPointer(), pGS->GetBufferSize(), &gWaterCubeLayout);

	pGS = nullptr;
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"WaterCubeFragment.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreatePixelShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gWaterCubeFragmentShader
	);

	result = D3DCompileFromFile(
		L"DCFragmentWater.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreatePixelShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gDynamicWaterCubeFragment
	);

	pGS->Release();
	return S_OK;
}
HRESULT CreateDynamicCubeShaders()
{
	ID3DBlob* pGS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"VertexDC.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreateVertexShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gDynamicCubeVertexShader
	);
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			0,							 // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
		{
			"UV",
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			12,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	result = gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pGS->GetBufferPointer(), pGS->GetBufferSize(), &gWaterCubeLayout);

	pGS = nullptr;
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"FragmentDC.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreatePixelShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gDynamicCubeFragmentShader
	);

	pGS->Release();
	return S_OK;
}
HRESULT CreateDCTestCubeShaders()
{
	ID3DBlob* pGS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"DCTestCubeVertex.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreateVertexShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gDCTestCubeVertex
	);

	pGS = nullptr;
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"DCTestCubeFragment.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options (DEBUGGING)
		0,				// IGNORE...DEPRECATED.
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreatePixelShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gDCTestCubeFragment
	);

	pGS->Release();
	return S_OK;
}
struct Pos2UV {
	float x, y;
	float u, v;
};
void CreateFullScreenQuad()
{
	Pos2UV myQuad[6] = {
		-1,-1, 0, 0,
		-1,+1, 0, 1,
		+1,+1, 1, 1,
		-1,-1, 0, 0,
		+1,+1, 1, 1,
		+1,-1, 1, 0,
	};
	D3D11_BUFFER_DESC bufferDesc;
	ZERO(bufferDesc);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(myQuad);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = myQuad;

	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBufferFS);
}
struct UVVertex
{
	float x, y, z;
	float u, v;
};

void createSMCube()
{
	UVVertex vertices[24] =
	{
		// Front Face
		-1.0f, -1.0f, -1.0f, //Coordinates
		0.0f, 1.0f,			 //UV
		-1.0f,  1.0f, -1.0f,//Coordinates
		0.0f, 0.0f,			//UV
		1.0f,  1.0f, -1.0f, //Coordinates
		1.0f, 0.0f,			//UV
		1.0f, -1.0f, -1.0f,//Coordinates
		1.0f, 1.0f,			//UV
		// Back Face
		-1.0f, -1.0f, 1.0f,//Coordinates
		1.0f, 1.0f,		   //UV
		1.0f, -1.0f, 1.0f,//Coordinates
		0.0f, 1.0f,		  //UV
		1.0f,  1.0f, 1.0f,//Coordinates
		0.0f, 0.0f,		  //UV
		-1.0f,  1.0f, 1.0f,//Coordinates
		1.0f, 0.0f,		  //UV

		// Top Face
		-1.0f, 1.0f, -1.0f,//Coordinates
		0.0f, 1.0f,		   //UV
		-1.0f, 1.0f, 1.0f, //Coordinates
		0.0f, 0.0f,			//UV
		1.0f, 1.0f,1.0f,//Coordinates
		1.0f, 0.0f,		  //UV
		1.0f, 1.0f, -1.0f,//Coordinates
		1.0f, 1.0f,		  //UV

		// Bottom Face
		-1.0f, -1.0f, -1.0f,//Coordinates
		1.0f, 1.0f,			//UV
		1.0f, -1.0f, -1.0f,//Coordinates
		0.0f, 1.0f,		   //UV
		1.0f, -1.0f,  1.0f,//Coordinates
		0.0f, 0.0f,		   //UV
		-1.0f, -1.0f, 1.0f,//Coordinates
		1.0f, 0.0f,			//UV

		// Left Face
		-1.0f, -1.0f,  1.0f,//Coordinates
		0.0f, 1.0f,			//UV
		-1.0f,  1.0f, 1.0f,//Coordinates
		0.0f, 0.0f,			//UV
		-1.0f,  1.0f, -1.0f,//Coordinates
		1.0f, 0.0f,			//UV
		-1.0f, -1.0f, -1.0f,//Coordinates
		1.0f, 1.0f,			//UV

		// Right Face
		1.0f, -1.0f, -1.0f,//Coordinates
		0.0f, 1.0f,		   //UV
		1.0f, 1.0f, -1.0f,//Coordinates
		0.0f, 0.0f,		   //UV
		1.0f, 1.0f, 1.0f,//Coordinates
		1.0f, 0.0f,		   //UV
		1.0f, -1.0f,  1.0f,//Coordinates
		1.0f, 1.0f,	       //UV
	};

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertices;
	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBufferSM);
}

struct Vertex
{
	float x, y, z, padding;
	float tangentX, tangentY, tangentZ, padding2;
	float n1, n2, n3, padding3;
	float u, v;
};
float getDistanceBetweenPoints(float point[3], XMVECTOR camPos)
{
	XMFLOAT3 pointB;
	XMStoreFloat3(&pointB, camPos);
	float deltaX = abs(point[0] - pointB.x);
	float deltaY = abs(point[1] - pointB.y);
	float deltaZ = abs(point[2] - pointB.z);
	float distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2) + pow(deltaZ, 2));
	return distance;
}
Vertex vertices[24] =
{
	// Front Face
	-1.0f, -1.0f, -1.0f,0, //Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	0.0f,0.0f,1.0f,0,	//Normal
	0.0f, 1.0f,			 //UV

	-1.0f,  1.0f, -1.0f,0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	0.0f,0.0f,1.0f,0,//Normal
	0.0f, 0.0f,			//UV

	1.0f,  1.0f, -1.0f,0, //Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	0.0f,0.0f,1.0f,0,//Normal
	1.0f, 0.0f,			//UV

	1.0f, -1.0f, -1.0f,0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	0.0f,0.0f,1.0f,0,//Normal
	1.0f, 1.0f,			//UV
	// Back Face
	-1.0f, -1.0f, 1.0f,0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	0.0f,0.0f,-1.0f,0,//Normal
	1.0f, 1.0f,		   //UV

	1.0f, -1.0f, 1.0f,0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	0.0f,0.0f,-1.0f,0,//Normal
	0.0f, 1.0f,		  //UV

	1.0f,  1.0f, 1.0f,0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	0.0f,0.0f,-1.0f,0,//Normal
	0.0f, 0.0f,		  //UV

	-1.0f,  1.0f, 1.0f,0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	0.0f,0.0f,-1.0f,0,//Normal
	1.0f, 0.0f,		  //UV

	// Top Face
	-1.0f, 1.0f, -1.0f,0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	0.0f,1.0f,0.0f,0,//Normal
	0.0f, 1.0f,		   //UV

	-1.0f, 1.0f,  1.0f,0, //Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	0.0f,1.0f,0.0f,0,//Normal
	0.0f, 0.0f,			//UV

	1.0f, 1.0f,  1.0f,0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	0.0f,1.0f,0.0f,0,//Normal
	1.0f, 0.0f,		  //UV

	1.0f, 1.0f, -1.0f,0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	0.0f,1.0f,0.0f,0,//Normal
	1.0f, 1.0f,		  //UV

	// Bottom Face
	-1.0f, -1.0f, -1.0f,0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	0.0f,-1.0f,0.0f,0,//Normal
	1.0f, 1.0f,			//UV

	1.0f, -1.0f, -1.0f,0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	0.0f,-1.0f,0.0f,0,//Normal
	0.0f, 1.0f,		   //UV

	1.0f, -1.0f,  1.0f,0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	0.0f,-1.0f,0.0f,0,//Normal
	0.0f, 0.0f,		   //UV

	-1.0f, -1.0f,  1.0f,0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	0.0f,-1.0f,0.0f,0,//Normal
	1.0f, 0.0f,			//UV

	// Left Face
	-1.0f, -1.0f,  1.0f,0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	-1.0f,0.0f,0.0f,0,//Normal
	0.0f, 1.0f,			//UV

	-1.0f,  1.0f,  1.0f,0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	-1.0f,0.0f,0.0f,0,//Normal
	0.0f, 0.0f,			//UV

	-1.0f,  1.0f, -1.0f, 0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	-1.0f,0.0f,0.0f,0,//Normal
	1.0f, 0.0f,			//UV

	-1.0f, -1.0f, -1.0f,0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	-1.0f,0.0f,0.0f,0,//Normal
	1.0f, 1.0f,			//UV

	// Right Face
	1.0f, -1.0f, -1.0f,0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	1.0f,0.0f,0.0f,0,//Normal
	0.0f, 1.0f,		   //UV

	1.0f,  1.0f, -1.0f,0,//Coordinates
	0.0f,0.0f,0.0f,0,	//Tangent
	1.0f,0.0f,0.0f,0,//Normal
	0.0f, 0.0f,		   //UV

	1.0f,  1.0f,  1.0f,0,//Coordinates
	0.0f, 0.0f, 0.0f,0,	//Tangent
	1.0f, 0.0f, 0.0f, 0,//Normal
	1.0f, 0.0f,		   //UV

	1.0f, -1.0f,  1.0f,0,//Coordinates
	0.0f, 0.0f, 0.0f,0,	//Tangent
	1.0f, 0.0f, 0.0f, 0,//Normal
	1.0f, 1.0f,	       //UV
};
Vertex verticesTwo[24] =
{
	// Front Face
		-1.0f, -1.0f, 2.0f,0, //Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		0.0f,0.0f,1.0f,0,	//Normal
		0.0f, 1.0f,			 //UV

		-1.0f,  1.0f, 2.0f,0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		0.0f,0.0f,1.0f,0,//Normal
		0.0f, 0.0f,			//UV

		1.0f,  1.0f, 2.0f,0, //Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		0.0f,0.0f,1.0f,0,//Normal
		1.0f, 0.0f,			//UV

		1.0f, -1.0f, 2.0f,0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		0.0f,0.0f,1.0f,0,//Normal
		1.0f, 1.0f,			//UV
		// Back Face
		-1.0f, -1.0f, 4.0f,0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		0.0f,0.0f,-1.0f,0,//Normal
		1.0f, 1.0f,		   //UV

		1.0f, -1.0f, 4.0f,0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		0.0f,0.0f,-1.0f,0,//Normal
		0.0f, 1.0f,		  //UV

		1.0f,  1.0f, 4.0f,0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		0.0f,0.0f,-1.0f,0,//Normal
		0.0f, 0.0f,		  //UV

		-1.0f,  1.0f, 4.0f,0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		0.0f,0.0f,-1.0f,0,//Normal
		1.0f, 0.0f,		  //UV

		// Top Face
		-1.0f, 1.0f, 2.0f,0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		0.0f,1.0f,0.0f,0,//Normal
		0.0f, 1.0f,		   //UV

		-1.0f, 1.0f,  4.0f,0, //Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		0.0f,1.0f,0.0f,0,//Normal
		0.0f, 0.0f,			//UV

		1.0f, 1.0f,  4.0f,0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		0.0f,1.0f,0.0f,0,//Normal
		1.0f, 0.0f,		  //UV

		1.0f, 1.0f, 2.0f,0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		0.0f,1.0f,0.0f,0,//Normal
		1.0f, 1.0f,		  //UV

		// Bottom Face
		-1.0f, -1.0f, 2.0f,0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		0.0f,-1.0f,0.0f,0,//Normal
		1.0f, 1.0f,			//UV

		1.0f, -1.0f, 2.0f,0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		0.0f,-1.0f,0.0f,0,//Normal
		0.0f, 1.0f,		   //UV

		1.0f, -1.0f,  4.0f,0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		0.0f,-1.0f,0.0f,0,//Normal
		0.0f, 0.0f,		   //UV

		-1.0f, -1.0f, 4.0f,0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		0.0f,-1.0f,0.0f,0,//Normal
		1.0f, 0.0f,			//UV

		// Left Face
		-1.0f, -1.0f,  4.0f,0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		-1.0f,0.0f,0.0f,0,//Normal
		0.0f, 1.0f,			//UV

		-1.0f,  1.0f,  4.0f,0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		-1.0f,0.0f,0.0f,0,//Normal
		0.0f, 0.0f,			//UV

		-1.0f,  1.0f, 2.0f, 0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		-1.0f,0.0f,0.0f,0,//Normal
		1.0f, 0.0f,			//UV

		-1.0f, -1.0f, 2.0f,0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		-1.0f,0.0f,0.0f,0,//Normal
		1.0f, 1.0f,			//UV

		// Right Face
		1.0f, -1.0f, 2.0f,0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		1.0f,0.0f,0.0f,0,//Normal
		0.0f, 1.0f,		   //UV

		1.0f,  1.0f, 2.0f,0,//Coordinates
		0.0f,0.0f,0.0f,0,	//Tangent
		1.0f,0.0f,0.0f,0,//Normal
		0.0f, 0.0f,		   //UV

		1.0f,  1.0f,  4.0f,0,//Coordinates
		0.0f, 0.0f, 0.0f,0,	//Tangent
		1.0f, 0.0f, 0.0f, 0,//Normal
		1.0f, 0.0f,		   //UV

		1.0f, -1.0f,  4.0f,0,//Coordinates
		0.0f, 0.0f, 0.0f,0,	//Tangent
		1.0f, 0.0f, 0.0f, 0,//Normal
		1.0f, 1.0f,	       //UV
};
struct objectStruct
{
	bool render = true;
	float centerPos[3]{ 0,0,0 };
	ID3D11Buffer* vertexBuffer;
};
std::vector<objectStruct> cubes;
DWORD indices[] = {
	// Front Face
	0,  1,  2,
	0,  2,  3,

	// Back Face
	4,  5,  6,
	4,  6,  7,

	// Top Face
	8,  9, 10,
	8, 10, 11,

	// Bottom Face
	12, 13, 14,
	12, 14, 15,

	// Left Face
	16, 17, 18,
	16, 18, 19,

	// Right Face
	20, 21, 22,
	20, 22, 23
};
void CreateCube()
{
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA indexData;

	indexData.pSysMem = indices;
	gDevice->CreateBuffer(&indexBufferDesc, &indexData, &gCubeIndexBuffer);

	for (int i = 0; i < 36; i += 3)
	{
		Vertex& v0 = vertices[indices[i]];
		Vertex& v1 = vertices[indices[i + 1]];
		Vertex& v2 = vertices[indices[i + 2]];
		float edge1X = v1.x - v0.x;
		float edge1Y = v1.y - v0.y;
		float edge1Z = v1.z - v0.z;

		float edge2X = v2.x - v0.x;
		float edge2Y = v2.y - v0.y;
		float edge2Z = v2.z - v0.z;

		float deltaU1 = v1.u - v0.u;
		float deltaV1 = v1.v - v0.v;
		float deltaU2 = v2.u - v0.u;
		float deltaV2 = v2.v - v0.v;

		float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);
		float tangentX;
		float tangentY;
		float tangentZ;

		tangentX = f * (deltaV2 * edge1X - deltaV1 * edge2X);
		tangentY = f * (deltaV2 * edge1Y - deltaV1 * edge2Y);
		tangentZ = f * (deltaV2 * edge1Z - deltaV1 * edge2Z);
		v0.tangentX += tangentX;
		v0.tangentY += tangentY;
		v0.tangentZ += tangentZ;

		v1.tangentX += tangentX;
		v1.tangentY += tangentY;
		v1.tangentZ += tangentZ;

		v2.tangentX += tangentX;
		v2.tangentY += tangentY;
		v2.tangentZ += tangentZ;
	}
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertices;
	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
	data.pSysMem = verticesTwo;
	for (int i = 0; i < 36; i += 3)
	{
		Vertex& v0 = verticesTwo[indices[i]];
		Vertex& v1 = verticesTwo[indices[i + 1]];
		Vertex& v2 = verticesTwo[indices[i + 2]];
		float edge1X = v1.x - v0.x;
		float edge1Y = v1.y - v0.y;
		float edge1Z = v1.z - v0.z;

		float edge2X = v2.x - v0.x;
		float edge2Y = v2.y - v0.y;
		float edge2Z = v2.z - v0.z;

		float deltaU1 = v1.u - v0.u;
		float deltaV1 = v1.v - v0.v;
		float deltaU2 = v2.u - v0.u;
		float deltaV2 = v2.v - v0.v;

		float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);
		float tangentX;
		float tangentY;
		float tangentZ;

		tangentX = f * (deltaV2 * edge1X - deltaV1 * edge2X);
		tangentY = f * (deltaV2 * edge1Y - deltaV1 * edge2Y);
		tangentZ = f * (deltaV2 * edge1Z - deltaV1 * edge2Z);
		v0.tangentX += tangentX;
		v0.tangentY += tangentY;
		v0.tangentZ += tangentZ;

		v1.tangentX += tangentX;
		v1.tangentY += tangentY;
		v1.tangentZ += tangentZ;

		v2.tangentX += tangentX;
		v2.tangentY += tangentY;
		v2.tangentZ += tangentZ;
	}
	bufferDesc.ByteWidth = sizeof(verticesTwo);
	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBufferTwo);
	objectStruct firstCube;
	objectStruct secondCube;

	//Calculate center
	for (int i = 0; i < 24; i++)
	{
		firstCube.centerPos[0] += vertices[i].x;
		firstCube.centerPos[1] += vertices[i].y;
		firstCube.centerPos[2] += vertices[i].z;
	}
	firstCube.centerPos[0] /= 24;
	firstCube.centerPos[1] /= 24;
	firstCube.centerPos[2] /= 24;
	firstCube.vertexBuffer = gVertexBuffer;
	cubes.push_back(firstCube);

	for (int i = 0; i < 24; i++)
	{
		secondCube.centerPos[0] += verticesTwo[i].x;
		secondCube.centerPos[1] += verticesTwo[i].y;
		secondCube.centerPos[2] += verticesTwo[i].z;
	}
	secondCube.centerPos[0] /= 24;
	secondCube.centerPos[1] /= 24;
	secondCube.centerPos[2] /= 24;
	secondCube.vertexBuffer = gVertexBufferTwo;
	cubes.push_back(secondCube);
}
//Front face
//-8,0,2
//back face
//-8,0,4
//top face
//-8,1,3
//Left face
//-9, 0, 3
//right face
//-7, 0, 3

UVVertex dynamicCubeVertices[24] =
{
	// Front Face
	-13.0f, -1.0f, 2.0f, //Coordinates
	0.0f, 1.0f,			 //UV

	-13.0f,  1.0f, 2.0f,//Coordinates
	0.0f, 0.0f,			//UV

	-11.0f,  1.0f, 2.0f, //Coordinates
	1.0f, 0.0f,			//UV

	-11.0f, -1.0f, 2.0f,//Coordinates
	1.0f, 1.0f,			//UV

	// Back Face
	-13.0f, -1.0f, 4.0f,//Coordinates
	1.0f, 1.0f,		   //UV

	-11.0f, -1.0f, 4.0f,//Coordinates
	0.0f, 1.0f,		  //UV

	-11.0f,  1.0f, 4.0f,//Coordinates
	0.0f, 0.0f,		  //UV

	-13.0f,  1.0f, 4.0f,//Coordinates
	1.0f, 0.0f,		  //UV

	// Top Face
	-13.0f, 1.0f, 2.0f,//Coordinates
	0.0f, 1.0f,		   //UV

	-13.0f, 1.0f,  4.0f, //Coordinates
	0.0f, 0.0f,			//UV

	-11.0f, 1.0f,  4.0f,//Coordinates
	1.0f, 0.0f,		  //UV

	-11.0f, 1.0f, 2.0f,//Coordinates
	1.0f, 1.0f,		  //UV

	// Bottom Face
	-13.0f, -1.0f, 2.0f,//Coordinates
	1.0f, 1.0f,			//UV

	-11.0f, -1.0f, 2.0f,//Coordinates

	0.0f, 1.0f,		   //UV

	-11.0f, -1.0f,  4.0f,//Coordinates

	0.0f, 0.0f,		   //UV

	-13.0f, -1.0f, 4.0f,//Coordinates
	1.0f, 0.0f,			//UV

	// Left Face
	-13.0f, -1.0f,  4.0f,//Coordinates
	0.0f, 1.0f,			//UV

	-13.0f,  1.0f,  4.0f,//Coordinates
	0.0f, 0.0f,			//UV

	-13.0f,  1.0f, 2.0f,//Coordinates
	1.0f, 0.0f,			//UV

	-13.0f, -1.0f, 2.0f,//Coordinates
	1.0f, 1.0f,			//UV

	// Right Face
	-11.0f, -1.0f, 2.0f,//Coordinates
	0.0f, 1.0f,		   //UV

	-11.0f,  1.0f, 2.0f,//Coordinates
	0.0f, 0.0f,		   //UV

	-11.0f,  1.0f,  4.0f,//Coordinates
	1.0f, 0.0f,		   //UV

	-11.0f, -1.0f,  4.0f,//Coordinates
	1.0f, 1.0f,	       //UV
};
void CreateDynamicCube()
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(dynamicCubeVertices);
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = dynamicCubeVertices;
	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBufferDynamic);
}
struct TriangleVertex {
	float x, y, z;
};
void CreateBFCTest()
{
	TriangleVertex triangleVertices[6] =
	{
		-3.0f, -1.0f, 0.0f,	//v0 pos

		-3.0f, 1.0f, 0.0f,	//v2

		-1.0f, 1.0f, 0.0f, //v1

		-3.0f, -1.0f, 0.0f,	//v0 pos

		-1.0f,1.0f, 0.0f, //v2

		-1.0f, -1.0, 0.0f,	//v1
	};

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(triangleVertices);
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;
	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBufferBFCTest);
}
class Bullet
{
public:
	XMFLOAT3 vertices[24];
	XMVECTOR direction;
	XMVECTOR startPos;
	XMMATRIX worldMatrix;
	ID3D11Buffer* worldBuffer;
	ID3D11Buffer* vertexBuffer;
	bool render = true;
	float increment = 0;
	float radius = 0.1f;
	Bullet() {
		this->direction = camForward;
		this->worldMatrix = XMMatrixIdentity();
		this->startPos = camPos;
		XMFLOAT3 bulletVertices[24] = {
			// Front Face
			XMFLOAT3(camPosF4.x - radius,camPosF4.y - radius,camPosF4.z - radius),
			XMFLOAT3(camPosF4.x - radius,camPosF4.y + radius,camPosF4.z - radius),
			XMFLOAT3(camPosF4.x + radius,camPosF4.y + radius,camPosF4.z - radius),
			XMFLOAT3(camPosF4.x + radius,camPosF4.y - radius,camPosF4.z - radius),
			//Back face
			XMFLOAT3(camPosF4.x - radius,camPosF4.y - radius,camPosF4.z + radius),
			XMFLOAT3(camPosF4.x + radius,camPosF4.y - radius,camPosF4.z + radius),
			XMFLOAT3(camPosF4.x + radius,camPosF4.y + radius,camPosF4.z + radius),
			XMFLOAT3(camPosF4.x - radius,camPosF4.y + radius,camPosF4.z + radius),
			//Top face
			XMFLOAT3(camPosF4.x - radius,camPosF4.y + radius,camPosF4.z - radius),
			XMFLOAT3(camPosF4.x - radius,camPosF4.y + radius,camPosF4.z + radius),
			XMFLOAT3(camPosF4.x + radius,camPosF4.y + radius,camPosF4.z + radius),
			XMFLOAT3(camPosF4.x + radius,camPosF4.y + radius,camPosF4.z - radius),
			//Bottom face
			XMFLOAT3(camPosF4.x - radius,camPosF4.y - radius,camPosF4.z - radius),
			XMFLOAT3(camPosF4.x + radius,camPosF4.y - radius,camPosF4.z - radius),
			XMFLOAT3(camPosF4.x + radius,camPosF4.y - radius,camPosF4.z + radius),
			XMFLOAT3(camPosF4.x - radius,camPosF4.y - radius,camPosF4.z + radius),
			//Left face
			XMFLOAT3(camPosF4.x - radius,camPosF4.y - radius,camPosF4.z + radius),
			XMFLOAT3(camPosF4.x - radius,camPosF4.y + radius,camPosF4.z + radius),
			XMFLOAT3(camPosF4.x - radius,camPosF4.y + radius,camPosF4.z - radius),
			XMFLOAT3(camPosF4.x - radius,camPosF4.y - radius,camPosF4.z - radius),
			//Right face

			XMFLOAT3(camPosF4.x + radius,camPosF4.y - radius,camPosF4.z - radius),
			XMFLOAT3(camPosF4.x + radius,camPosF4.y + radius,camPosF4.z - radius),
			XMFLOAT3(camPosF4.x + radius,camPosF4.y + radius,camPosF4.z + radius),
			XMFLOAT3(camPosF4.x + radius,camPosF4.y - radius,camPosF4.z + radius),
		};
		for (int i = 0; i < 24; i++)
		{
			vertices[i] = bulletVertices[i];
		}

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(bulletVertices);
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = bulletVertices;
		gDevice->CreateBuffer(&bufferDesc, &data, &this->vertexBuffer);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.ByteWidth = sizeof(XMMATRIX);
		gDevice->CreateBuffer(&bufferDesc, nullptr, &this->worldBuffer);
	};
	~Bullet()
	{
	}
};

std::vector<Bullet> bullets;
float shootCooldown = 1;
bool shootActivated = false;
void createBullet()
{
	if (shootCooldown < 0 && shootActivated)
	{
		Bullet temp;
		bullets.push_back(temp);
		shootCooldown = 1;
	}

}
void SetViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = WIDTH;
	vp.Height = HEIGHT;
	vp.MinDepth = 0.0001f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	gDeviceContext->RSSetViewports(1, &vp);
}
bool HeightMapLoad(char* filename, HeightMapInfo &hminfo)
{
	FILE *filePtr;                            // Point to the current position in the file
	BITMAPFILEHEADER bitmapFileHeader;        // Structure which stores information about file
	BITMAPINFOHEADER bitmapInfoHeader;        // Structure which stores information about image
	int imageSize, index;
	unsigned char height;
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return 0;

	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	hminfo.terrainWidth = bitmapInfoHeader.biWidth;
	hminfo.terrainHeight = bitmapInfoHeader.biHeight;

	imageSize = hminfo.terrainWidth * hminfo.terrainHeight * 3;

	unsigned char* bitmapImage = new unsigned char[imageSize];

	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	fread(bitmapImage, 1, imageSize, filePtr);

	fclose(filePtr);

	hminfo.heightMap = new XMFLOAT3[hminfo.terrainWidth * hminfo.terrainHeight];

	// We use a greyscale image, so all 3 rgb values are the same, but we only need one for the height
	// So we use this counter to skip the next two components in the image data (we read R, then skip BG)
	int k = 0;

	// We divide the height by this number to "water down" the terrains height, otherwise the terrain will
	// appear to be "spikey" and not so smooth.
	float heightFactor = 10.0f;

	for (int j = 0; j < hminfo.terrainHeight; j++)
	{
		for (int i = 0; i < hminfo.terrainWidth; i++)
		{
			height = bitmapImage[k];

			index = (hminfo.terrainHeight * j) + i;

			hminfo.heightMap[index].x = (float)i;
			hminfo.heightMap[index].y = (float)height / heightFactor;
			hminfo.heightMap[index].z = (float)j;

			k += 3;
		}
	}

	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}
std::vector<HMVertex> InitializeHeightMap()
{
	HeightMapLoad("wall.bmp", hmInfo);

	int cols = hmInfo.terrainWidth;
	int rows = hmInfo.terrainHeight;

	NumVertices = rows * cols;
	NumFaces = (rows - 1)*(cols - 1) * 2;

	std::vector<HMVertex> v(NumVertices);

	for (DWORD i = 0; i < rows; ++i)
	{
		for (DWORD j = 0; j < cols; ++j)
		{
			v[i*cols + j].pos = hmInfo.heightMap[i*cols + j];
			v[i*cols + j].pos.x -= 40;
			v[i*cols + j].pos.y -= 11;
			v[i*cols + j].pos.z -= 40;
		}
	}

	std::vector<DWORD> indices(NumFaces * 3);

	int k = 0;
	int texUIndex = 0;
	int texVIndex = 0;
	for (DWORD i = 0; i < rows - 1; i++)
	{
		for (DWORD j = 0; j < cols - 1; j++)
		{
			indices[k] = i * cols + j;        // Bottom left of quad
			v[i*cols + j].texCoord = XMFLOAT2(texUIndex + 0.0f, texVIndex + 1.0f);

			indices[k + 1] = i * cols + j + 1;        // Bottom right of quad
			v[i*cols + j + 1].texCoord = XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);

			indices[k + 2] = (i + 1)*cols + j;    // Top left of quad
			v[(i + 1)*cols + j].texCoord = XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);


			indices[k + 3] = (i + 1)*cols + j;    // Top left of quad
			v[(i + 1)*cols + j].texCoord = XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);

			indices[k + 4] = i * cols + j + 1;        // Bottom right of quad
			v[i*cols + j + 1].texCoord = XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);

			indices[k + 5] = (i + 1)*cols + j + 1;    // Top right of quad
			v[(i + 1)*cols + j + 1].texCoord = XMFLOAT2(texUIndex + 1.0f, texVIndex + 0.0f);

			k += 6; // next quad

			texUIndex++;
		}
		texUIndex = 0;
		texVIndex++;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];
	gDevice->CreateBuffer(&indexBufferDesc, &iinitData, &gIndexBufferHM);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(HMVertex) * NumVertices;

	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &v[0];
	gDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &gVertexBufferHM);
	return v;

}
void RenderBullets()
{
	gDeviceContext->VSSetShader(gBulletVertexShader, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gBulletFragmentShader, nullptr, 0);


	UINT32 vertexSize = sizeof(XMFLOAT3);
	UINT32 offset = 0;
	gDeviceContext->IASetIndexBuffer(gCubeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->IASetInputLayout(gVertexLayoutBullet);
	gDeviceContext->VSSetConstantBuffers(1, 1, &ViewProjBuffer);

	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets.at(i).render)
		{
			gDeviceContext->IASetVertexBuffers(0, 1, &bullets.at(i).vertexBuffer, &vertexSize, &offset);
			gDeviceContext->VSSetConstantBuffers(0, 1, &bullets.at(i).worldBuffer);
			gDeviceContext->DrawIndexed(36, 0, 0);
		}
	}
}
void RenderBFCTest()
{
	UINT32 vertexSize = sizeof(TriangleVertex);
	UINT32 offset = 0;

	gDeviceContext->IASetInputLayout(gVertexLayoutBFCTest);
	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBufferBFCTest, &vertexSize, &offset);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	gDeviceContext->VSSetShader(gVertexShaderBFCTest, nullptr, 0);
	gDeviceContext->GSSetShader(gGeometryShaderBFCTest, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShaderBFCTest, nullptr, 0);

	gDeviceContext->GSSetConstantBuffers(0, 1, &projectionBuffer);
	gDeviceContext->GSSetConstantBuffers(1, 1, &viewBuffer);
	gDeviceContext->VSSetConstantBuffers(1, 1, &BFCTestWorldBuffer);
	gDeviceContext->VSSetConstantBuffers(0, 1, &ViewProjBuffer);

	gDeviceContext->Draw(6, 0);
}
UVVertex goalVertices[6] = {
	-3.0, 1.0, 2.0,	//v0 pos
	0.0f,1.0f,
	-3.0, 1.0, 0.0,	//v2
	0.0f,0.0f,
	-1.0, 1.0, 0.0, //v1
	1.0f,0.0f,
	-3.0, 1.0, 2.0,	//v0 pos
	0.0f,1.0f,
	-1.0,1.0, 0.0, //v2
	1.0f,0.0f,
	-1.0, 1.0, 2.0,	//v1
	1.0f,1.0f
};
void checkGoalCollision()
{

	if (camPosF4.x < -1.0f && camPosF4.x > -3.0f &&
		camPosF4.z < 2.0f && camPosF4.z > 0.0f)
	{
		renderEndscreen = true;
	}

}
void createGoal()
{

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(goalVertices);
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = goalVertices;
	gDevice->CreateBuffer(&bufferDesc, &data, &gGoalVertexBuffer);
}
void renderGoal()
{
	UINT32 vertexSize = sizeof(UVVertex);
	UINT32 offset = 0;

	gDeviceContext->IASetInputLayout(gGoalLayout);
	gDeviceContext->IASetVertexBuffers(0, 1, &gGoalVertexBuffer, &vertexSize, &offset);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	gDeviceContext->VSSetShader(gGoalVertexShader, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gGoalFragmentShader, nullptr, 0);

	gDeviceContext->VSSetConstantBuffers(0, 1, &ViewProjBuffer);

	gDeviceContext->Draw(6, 0);
	checkGoalCollision();

}

UVVertex waterCubeVertices[24] = {
	// Front Face
	-4.0f, -1.0f, 2.0f, //Coordinates
	0.0f, 1.0f,			 //UV

	-4.0f,  1.0f, 2.0f,//Coordinates
	0.0f, 0.0f,			//UV

	-2.0f,  1.0f, 2.0f, //Coordinates
	1.0f, 0.0f,			//UV

	-2.0f, -1.0f, 2.0f,//Coordinates
	1.0f, 1.0f,			//UV
	// Back Face
	-4.0f, -1.0f, 4.0f,//Coordinates
	1.0f, 1.0f,		   //UV

	-2.0f, -1.0f, 4.0f,//Coordinates
	0.0f, 1.0f,		  //UV

	-2.0f,  1.0f, 4.0f,//Coordinates
	0.0f, 0.0f,		  //UV

	-4.0f,  1.0f, 4.0f,//Coordinates
	1.0f, 0.0f,		  //UV

	// Top Face
	-4.0f, 1.0f, 2.0f,//Coordinates
	0.0f, 1.0f,		   //UV

	-4.0f, 1.0f,  4.0f, //Coordinates
	0.0f, 0.0f,			//UV

	-2.0f, 1.0f,  4.0f,//Coordinates
	1.0f, 0.0f,		  //UV

	-2.0f, 1.0f, 2.0f,//Coordinates
	1.0f, 1.0f,		  //UV

	// Bottom Face
	-4.0f, -1.0f, 2.0f,//Coordinates
	1.0f, 1.0f,			//UV

	-2.0f, -1.0f, 2.0f,//Coordinates

	0.0f, 1.0f,		   //UV

	-2.0f, -1.0f,  4.0f,//Coordinates

	0.0f, 0.0f,		   //UV

	-4.0f, -1.0f, 4.0f,//Coordinates
	1.0f, 0.0f,			//UV

	// Left Face
	-4.0f, -1.0f,  4.0f,//Coordinates
	0.0f, 1.0f,			//UV

	-4.0f,  1.0f,  4.0f,//Coordinates
	0.0f, 0.0f,			//UV

	-4.0f,  1.0f, 2.0f,//Coordinates
	1.0f, 0.0f,			//UV

	-4.0f, -1.0f, 2.0f,//Coordinates
	1.0f, 1.0f,			//UV

	// Right Face
	-2.0f, -1.0f, 2.0f,//Coordinates
	0.0f, 1.0f,		   //UV

	-2.0f,  1.0f, 2.0f,//Coordinates
	0.0f, 0.0f,		   //UV

	-2.0f,  1.0f,  4.0f,//Coordinates
	1.0f, 0.0f,		   //UV

	-2.0f, -1.0f,  4.0f,//Coordinates
	1.0f, 1.0f,	       //UV
};
void createWaterCube()
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(waterCubeVertices);
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = waterCubeVertices;
	gDevice->CreateBuffer(&bufferDesc, &data, &gWaterCubeVertexBuffer);
}
UVVertex DCTestCubeVertices[24] = {
	// Front Face
	-17.0f, -1.0f, 2.0f, //Coordinates
	0.0f, 1.0f,			 //UV

	-17.0f,  1.0f, 2.0f,//Coordinates
	0.0f, 0.0f,			//UV

	-15.0f,  1.0f, 2.0f, //Coordinates
	1.0f, 0.0f,			//UV

	-15.0f, -1.0f, 2.0f,//Coordinates
	1.0f, 1.0f,			//UV
	// Back Face
	-17.0f, -1.0f, 4.0f,//Coordinates
	1.0f, 1.0f,		   //UV

	-15.0f, -1.0f, 4.0f,//Coordinates
	0.0f, 1.0f,		  //UV

	-15.0f,  1.0f, 4.0f,//Coordinates
	0.0f, 0.0f,		  //UV

	-17.0f,  1.0f, 4.0f,//Coordinates
	1.0f, 0.0f,		  //UV

	// Top Face
	-17.0f, 1.0f, 2.0f,//Coordinates
	0.0f, 1.0f,		   //UV

	-17.0f, 1.0f,  4.0f, //Coordinates
	0.0f, 0.0f,			//UV

	-15.0f, 1.0f,  4.0f,//Coordinates
	1.0f, 0.0f,		  //UV

	-15.0f, 1.0f, 2.0f,//Coordinates
	1.0f, 1.0f,		  //UV

	// Bottom Face
	-17.0f, -1.0f, 2.0f,//Coordinates
	1.0f, 1.0f,			//UV

	-15.0f, -1.0f, 2.0f,//Coordinates

	0.0f, 1.0f,		   //UV

	-15.0f, -1.0f,  4.0f,//Coordinates

	0.0f, 0.0f,		   //UV

	-17.0f, -1.0f, 4.0f,//Coordinates
	1.0f, 0.0f,			//UV

	// Left Face
	-17.0f, -1.0f,  4.0f,//Coordinates
	0.0f, 1.0f,			//UV

	-17.0f,  1.0f,  4.0f,//Coordinates
	0.0f, 0.0f,			//UV

	-17.0f,  1.0f, 2.0f,//Coordinates
	1.0f, 0.0f,			//UV

	-17.0f, -1.0f, 2.0f,//Coordinates
	1.0f, 1.0f,			//UV

	// Right Face
	-15.0f, -1.0f, 2.0f,//Coordinates
	0.0f, 1.0f,		   //UV

	-15.0f,  1.0f, 2.0f,//Coordinates
	0.0f, 0.0f,		   //UV

	-15.0f,  1.0f,  4.0f,//Coordinates
	1.0f, 0.0f,		   //UV

	-15.0f, -1.0f,  4.0f,//Coordinates
	1.0f, 1.0f,	       //UV
};
void createDCTestCube()
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(DCTestCubeVertices);
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = DCTestCubeVertices;
	gDevice->CreateBuffer(&bufferDesc, &data, &gDCTestCubeVertexBuffer);
}
void renderWaterCube()
{
	UINT32 vertexSize = sizeof(UVVertex);
	UINT32 offset = 0;

	gDeviceContext->IASetInputLayout(gWaterCubeLayout);
	gDeviceContext->IASetVertexBuffers(0, 1, &gWaterCubeVertexBuffer, &vertexSize, &offset);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->IASetIndexBuffer(gCubeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	gDeviceContext->VSSetShader(gWaterCubeVertexShader, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gWaterCubeFragmentShader, nullptr, 0);

	gDeviceContext->VSSetConstantBuffers(0, 1, &ViewProjBuffer);
	gDeviceContext->VSSetConstantBuffers(1, 1, &rotationBuffer);
	gDeviceContext->PSSetShaderResources(0, 1, &gWaterTexture);

	gDeviceContext->DrawIndexed(36, 0, 0);
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	gDeviceContext->PSSetShader(gDynamicWaterCubeFragment, nullptr, 0);

	//for (int i = 0; i < 5; i++)
	//{
	//	gDeviceContext->Map(DCViewProjBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);

	//	switch (i)
	//	{
	//	case 0:
	//		gDeviceContext->OMSetRenderTargets(1, &gDynamicCubeTargetViewUp, NULL);
	//		memcpy(mappedMemory.pData, &(dynamicCubeUpView * Projection), sizeof(XMMATRIX));
	//		break;
	//	case 1:
	//		gDeviceContext->OMSetRenderTargets(1, &gDynamicCubeTargetViewFront, NULL);
	//		memcpy(mappedMemory.pData, &(dynamicCubeFrontView * Projection), sizeof(XMMATRIX));
	//		break;
	//	case 2:
	//		gDeviceContext->OMSetRenderTargets(1, &gDynamicCubeTargetViewBack, NULL);
	//		memcpy(mappedMemory.pData, &(dynamicCubeBackView * Projection), sizeof(XMMATRIX));
	//		break;
	//	case 3:
	//		gDeviceContext->OMSetRenderTargets(1, &gDynamicCubeTargetViewLeft, NULL);
	//		memcpy(mappedMemory.pData, &(dynamicCubeLeftView * Projection), sizeof(XMMATRIX));
	//		break;
	//	case 4:
	//		gDeviceContext->OMSetRenderTargets(1, &gDynamicCubeTargetViewRight, NULL);
	//		memcpy(mappedMemory.pData, &(dynamicCubeRightView * Projection), sizeof(XMMATRIX));
	//		break;
	//	default:
	//		break;
	//	}
	//	gDeviceContext->Unmap(DCViewProjBuffer, 0);
	//	gDeviceContext->VSSetConstantBuffers(0, 1, &DCViewProjBuffer);
	//	gDeviceContext->DrawIndexed(36, 0, 0);

	//}
	//gDeviceContext->OMSetRenderTargets(3, &gRenderTargetView[0], gDepthStencilView);

}
void renderDCTestCube()
{
	UINT32 vertexSize = sizeof(UVVertex);
	UINT32 offset = 0;
//	gDeviceContext->RSSetState(RSCullNone);

	gDeviceContext->IASetInputLayout(gWaterCubeLayout);
	gDeviceContext->IASetVertexBuffers(0, 1, &gDCTestCubeVertexBuffer, &vertexSize, &offset);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->IASetIndexBuffer(gCubeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	gDeviceContext->VSSetShader(gDCTestCubeVertex, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gDCTestCubeFragment, nullptr, 0);

	gDeviceContext->VSSetConstantBuffers(0, 1, &ViewProjBuffer);
	gDeviceContext->VSSetConstantBuffers(1, 1, &rotationBuffer);
	gDeviceContext->VSSetConstantBuffers(2, 1, &BFCTestWorldBuffer);

	gDeviceContext->PSSetShaderResources(0, 1, &gWaterTexture);
	gDeviceContext->OMSetRenderTargets(3, &gRenderTargetView[0], gDepthStencilView);

	gDeviceContext->DrawIndexed(36, 0, 0);
	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	for (int i = 0; i < 5; i++)
	{
		gDeviceContext->Map(DCViewProjBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);

		switch (i)
		{
		case 0:
			gDeviceContext->OMSetRenderTargets(0, NULL, NULL);
			gDeviceContext->OMSetRenderTargets(1, &gDynamicCubeTargetViewUp, NULL);
			memcpy(mappedMemory.pData, &(dynamicCubeUpView * Projection), sizeof(XMMATRIX));
			break;
		case 1:
			gDeviceContext->OMSetRenderTargets(0, NULL, NULL);
			gDeviceContext->OMSetRenderTargets(1, &gDynamicCubeTargetViewFront, NULL);
			memcpy(mappedMemory.pData, &(dynamicCubeFrontView * Projection), sizeof(XMMATRIX));
			break;
		case 2:
			gDeviceContext->OMSetRenderTargets(0, NULL, NULL);
			gDeviceContext->OMSetRenderTargets(1, &gDynamicCubeTargetViewBack, NULL);
			memcpy(mappedMemory.pData, &(dynamicCubeBackView * Projection), sizeof(XMMATRIX));
			break;
		case 3:
			gDeviceContext->OMSetRenderTargets(0, NULL, NULL);
			gDeviceContext->OMSetRenderTargets(1, &gDynamicCubeTargetViewLeft, NULL);
			memcpy(mappedMemory.pData, &(dynamicCubeLeftView * Projection), sizeof(XMMATRIX));
			break;
		case 4:
			gDeviceContext->OMSetRenderTargets(0, NULL, NULL);
			gDeviceContext->OMSetRenderTargets(1, &gDynamicCubeTargetViewRight, NULL);
			memcpy(mappedMemory.pData, &(dynamicCubeRightView * Projection), sizeof(XMMATRIX));
			break;
		default:
			break;
		}
		gDeviceContext->Unmap(DCViewProjBuffer, 0);
		gDeviceContext->VSSetConstantBuffers(0, 1, &DCViewProjBuffer);

		gDeviceContext->DrawIndexed(36, 0, 0);

	}
	gDeviceContext->OMSetRenderTargets(0, NULL, NULL);

	gDeviceContext->OMSetRenderTargets(3, &gRenderTargetView[0], gDepthStencilView);
}
ID3D11Texture2D* upTexture;
ID3D11Texture2D* frontTexture;
ID3D11Texture2D* backTexture;
ID3D11Texture2D* leftTexture;
ID3D11Texture2D* rightTexture;
void createDynamicCubeTextures()
{
	D3D11_TEXTURE2D_DESC textureDesc;
	ZERO(textureDesc);
	textureDesc.Width = WIDTH;
	textureDesc.Height = HEIGHT;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = D3D11_USAGE_DEFAULT;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;

	gDevice->CreateTexture2D(&textureDesc, NULL, &upTexture);
	gDevice->CreateTexture2D(&textureDesc, NULL, &frontTexture);
	gDevice->CreateTexture2D(&textureDesc, NULL, &backTexture);
	gDevice->CreateTexture2D(&textureDesc, NULL, &leftTexture);
	gDevice->CreateTexture2D(&textureDesc, NULL, &rightTexture);
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZERO(shaderResourceViewDesc);
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	gDevice->CreateShaderResourceView(upTexture, &shaderResourceViewDesc, &gDynamicFaceUpView);
	gDevice->CreateShaderResourceView(frontTexture, &shaderResourceViewDesc, &gDynamicFaceFrontView);
	gDevice->CreateShaderResourceView(backTexture, &shaderResourceViewDesc, &gDynamicFaceBackView);
	gDevice->CreateShaderResourceView(leftTexture, &shaderResourceViewDesc, &gDynamicFaceLeftView);
	gDevice->CreateShaderResourceView(rightTexture, &shaderResourceViewDesc, &gDynamicFaceRightView);
}
void renderDynamicCube()
{

	UINT32 vertexSize = sizeof(UVVertex);
	UINT32 offset = 0;
	//gDeviceContext->RSSetState(RSCullNone);

	gDeviceContext->CopyResource(upTexture, gDynamicFaceUp);
	gDeviceContext->CopyResource(frontTexture, gDynamicFaceFront);
	gDeviceContext->CopyResource(backTexture, gDynamicFaceBack);
	gDeviceContext->CopyResource(leftTexture, gDynamicFaceLeft);
	gDeviceContext->CopyResource(rightTexture, gDynamicFaceRight);

	gDeviceContext->IASetInputLayout(gWaterCubeLayout);
	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBufferDynamic, &vertexSize, &offset);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->IASetIndexBuffer(gCubeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	gDeviceContext->VSSetShader(gDynamicCubeVertexShader, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gDynamicCubeFragmentShader, nullptr, 0);
	gDeviceContext->PSSetSamplers(0, 1, &sampleState);

	gDeviceContext->VSSetConstantBuffers(0, 1, &ViewProjBuffer);
	for (int i = 0; i < 6; i++)
	{
		switch (i)
		{
		case 0:
			//Back
			gDeviceContext->PSSetShaderResources(0, 1, &gDynamicFaceBackView);
			break;
		case 1:
			//Front
			gDeviceContext->PSSetShaderResources(0, 1, &gDynamicFaceFrontView);
			break;
		case 2:
			//Bottom
			gDeviceContext->PSSetShaderResources(0, 1, &gCubeColourmap);
			break;
		case 3:
			//Top
			gDeviceContext->PSSetShaderResources(0, 1, &gDynamicFaceUpView);
			break;
		case 4:
			//Left
			gDeviceContext->PSSetShaderResources(0, 1, &gDynamicFaceLeftView);
			break;
		case 5:
			gDeviceContext->PSSetShaderResources(0, 1, &gDynamicFaceRightView);
			break;
		default:
			break;
		}
		gDeviceContext->DrawIndexed(6 * (i + 1), i * 6, 0);

	}
}
void RenderCube()
{
	//gDeviceContext->RSSetState(RSCullNone);

	gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	gDeviceContext->GSSetShader(gGeometryShader, nullptr, 0);
	gDeviceContext->HSSetShader(gHullShader, nullptr, 0);
	gDeviceContext->DSSetShader(gDomainShader, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);


	gDeviceContext->GSSetConstantBuffers(0, 1, &projectionBuffer);
	gDeviceContext->GSSetConstantBuffers(0, 1, &viewBuffer);

	gDeviceContext->DSSetConstantBuffers(0, 1, &dsBuffer);
	gDeviceContext->DSSetShaderResources(0, 1, &gCubeDisplacementmap);

	gDeviceContext->PSSetSamplers(0, 1, &sampleState);
	gDeviceContext->PSSetShaderResources(0, 1, &gCubeColourmap);
	gDeviceContext->PSSetShaderResources(1, 1, &gCubeNormalmap);

	UINT32 vertexSize = sizeof(Vertex);
	UINT32 offset = 0;
	gDeviceContext->IASetIndexBuffer(gCubeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	gDeviceContext->IASetInputLayout(gVertexLayout);
	std::vector<int> drawOrder;
	for (int i = 0; i < cubes.size(); i++)
	{
		drawOrder.push_back(i);
	}
	for (int i = 0; i < cubes.size(); i++)
	{
		for (int j = i + 1; j < cubes.size(); j++)
		{
			if (getDistanceBetweenPoints(cubes.at(j).centerPos, camPos) < getDistanceBetweenPoints(cubes.at(i).centerPos, camPos))
				std::swap(drawOrder.at(i), drawOrder.at(j));

		}
	}
	int nrOfRenderedBoxes = 0;
	for (int i = 0; i < cubes.size(); i++)
	{
		if (cubes.at(drawOrder.at(i)).render)
		{
			nrOfRenderedBoxes++;
			gDeviceContext->IASetVertexBuffers(0, 1, &cubes.at(drawOrder.at(i)).vertexBuffer, &vertexSize, &offset);
			gDeviceContext->DrawIndexed(36, 0, 0);
		}
	}
	if (nrOfRenderedBoxes == 0)
	{
		renderGoal();
	}


}
void RenderHeightmap()
{
	gDeviceContext->OMSetRenderTargets(3, &gRenderTargetView[0], gDepthStencilView);

	gDeviceContext->VSSetShader(gVertexShaderHM, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShaderHM, nullptr, 0);

	gDeviceContext->VSSetConstantBuffers(0, 1, &ViewProjBuffer);
	//gDeviceContext->RSSetState(RSCullNone);

	UINT32 vertexSize = sizeof(HMVertex);
	UINT32 offset = 0;
	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBufferHM, &vertexSize, &offset);
	gDeviceContext->IASetIndexBuffer(gIndexBufferHM, DXGI_FORMAT_R32_UINT, 0);
	gDeviceContext->IASetInputLayout(gHMLayout);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->PSSetShaderResources(0, 1, &gHMTexture);


	gDeviceContext->DrawIndexed(NumFaces * 3, 0, 0);
}
void RenderSkymap()
{
	gDeviceContext->VSSetShader(gVertexShaderSM, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShaderSM, nullptr, 0);

	gDeviceContext->VSSetConstantBuffers(0, 1, &SkymapWVPBuffer);
	gDeviceContext->PSSetShaderResources(0, 1, &gSkyMapView);
	gDeviceContext->GSSetConstantBuffers(0, 1, &camPosBuffer);

	gDeviceContext->OMSetDepthStencilState(DSLessEqual, 0);
//	gDeviceContext->RSSetState(RSCullNone);

	UINT32 vertexSize = sizeof(UVVertex);
	UINT32 offset = 0;
	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBufferSM, &vertexSize, &offset);
	gDeviceContext->IASetIndexBuffer(gCubeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	gDeviceContext->IASetInputLayout(gSMLayout);

	gDeviceContext->DrawIndexed(36, 0, 0);

	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	gDeviceContext->PSSetShader(gDynamicSkymapFragment, nullptr, 0);

	for (int i = 0; i < 5; i++)
	{
		gDeviceContext->Map(DCViewProjBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);

		switch (i)
		{
		case 0:
			gDeviceContext->OMSetRenderTargets(1, &gDynamicCubeTargetViewUp, NULL);
			memcpy(mappedMemory.pData, &(dynamicCubeUpView * Projection), sizeof(XMMATRIX));
			break;
		case 1:
			gDeviceContext->OMSetRenderTargets(1, &gDynamicCubeTargetViewFront, NULL);
			memcpy(mappedMemory.pData, &(dynamicCubeFrontView * Projection), sizeof(XMMATRIX));
			break;
		case 2:
			gDeviceContext->OMSetRenderTargets(1, &gDynamicCubeTargetViewBack, NULL);
			memcpy(mappedMemory.pData, &(dynamicCubeBackView * Projection), sizeof(XMMATRIX));
			break;
		case 3:
			gDeviceContext->OMSetRenderTargets(1, &gDynamicCubeTargetViewLeft, NULL);
			memcpy(mappedMemory.pData, &(dynamicCubeLeftView * Projection), sizeof(XMMATRIX));
			break;
		case 4:
			gDeviceContext->OMSetRenderTargets(1, &gDynamicCubeTargetViewRight, NULL);
			memcpy(mappedMemory.pData, &(dynamicCubeRightView * Projection), sizeof(XMMATRIX));
			break;
		default:
			break;
		}
		gDeviceContext->Unmap(DCViewProjBuffer, 0);
		gDeviceContext->VSSetConstantBuffers(0, 1, &DCViewProjBuffer);

		gDeviceContext->DrawIndexed(36, 0, 0);
	}
	gDeviceContext->OMSetRenderTargets(3, &gRenderTargetView[0], gDepthStencilView);

}
void RenderFullscreen()
{
	float whiteBackground[3] = { 1.0f, 0.0f, 1.0f };

	gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, NULL);
	gDeviceContext->ClearRenderTargetView(gBackbufferRTV, whiteBackground);

	gDeviceContext->VSSetShader(gVertexShaderFS, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShaderFS, nullptr, 0);

	UINT32 vertexSize = sizeof(Pos2UV);
	UINT32 offset = 0;
	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBufferFS, &vertexSize, &offset);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->IASetInputLayout(gVertexLayoutFS);
	gDeviceContext->PSSetShaderResources(0, 1, &gColorViewMap);
	gDeviceContext->PSSetShaderResources(1, 1, &gNormalViewMap);
	gDeviceContext->PSSetShaderResources(2, 1, &gPositionViewMap);
	gDeviceContext->PSSetShaderResources(3, 1, &gCubeSpecView);

	gDeviceContext->PSSetConstantBuffers(0, 1, &camPosBuffer);

	gDeviceContext->Draw(6, 0);

	ID3D11ShaderResourceView* nullRTV = { NULL };
	gDeviceContext->PSSetShaderResources(0, 1, &nullRTV);
}
void renderEndScreen()
{
	float whiteBackground[3] = { 1.0f, 0.0f, 1.0f };

	gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, NULL);
	gDeviceContext->ClearRenderTargetView(gBackbufferRTV, whiteBackground);

	gDeviceContext->VSSetShader(gVertexShaderFS, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShaderEndscreen, nullptr, 0);
	gDeviceContext->PSSetShaderResources(0, 1, &gEndscreenTex);

	UINT32 vertexSize = sizeof(Pos2UV);
	UINT32 offset = 0;
	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBufferFS, &vertexSize, &offset);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->IASetInputLayout(gVertexLayoutFS);

	gDeviceContext->Draw(6, 0);

	ID3D11ShaderResourceView* nullRTV = { NULL };
	gDeviceContext->PSSetShaderResources(0, 1, &nullRTV);
}
void render()
{
	float clearColor[3] = { 0, 0, 0 };
	gDeviceContext->OMSetDepthStencilState(gDepthStencilState, 0);
	gDeviceContext->OMSetRenderTargets(3, &gRenderTargetView[0], gDepthStencilView);
	gDeviceContext->OMSetDepthStencilState(DSLessEqual, 0);

	gDeviceContext->ClearRenderTargetView(gRenderTargetView[0], clearColor);
	gDeviceContext->ClearRenderTargetView(gRenderTargetView[1], clearColor);
	gDeviceContext->ClearRenderTargetView(gRenderTargetView[2], clearColor);
	gDeviceContext->ClearRenderTargetView(gDynamicCubeTargetViewUp, clearColor);
	gDeviceContext->ClearRenderTargetView(gDynamicCubeTargetViewFront, clearColor);
	gDeviceContext->ClearRenderTargetView(gDynamicCubeTargetViewBack, clearColor);
	gDeviceContext->ClearRenderTargetView(gDynamicCubeTargetViewLeft, clearColor);
	gDeviceContext->ClearRenderTargetView(gDynamicCubeTargetViewRight, clearColor);
	gDeviceContext->ClearDepthStencilView(gDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0);
	//gDeviceContext->RSSetState(RSCullNone);
	if (!renderEndscreen)
	{
		RenderBullets();
		renderWaterCube();
		RenderBFCTest();
		RenderCube();
		RenderHeightmap();
		RenderSkymap();
		renderDCTestCube();
		renderDynamicCube();
		RenderFullscreen();
	}
	else
		renderEndScreen();
}
void createRenderTargets()
{
	D3D11_TEXTURE2D_DESC textureDesc;
	ZERO(textureDesc);
	textureDesc.Width = WIDTH;
	textureDesc.Height = HEIGHT;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = D3D11_USAGE_DEFAULT;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	gDevice->CreateTexture2D(&textureDesc, NULL, &gTextureColour);
	gDevice->CreateTexture2D(&textureDesc, NULL, &gTextureNormal);
	gDevice->CreateTexture2D(&textureDesc, NULL, &gTexturePosition);
	gDevice->CreateTexture2D(&textureDesc, NULL, &gDynamicFaceUp);
	gDevice->CreateTexture2D(&textureDesc, NULL, &gDynamicFaceFront);
	gDevice->CreateTexture2D(&textureDesc, NULL, &gDynamicFaceBack);
	gDevice->CreateTexture2D(&textureDesc, NULL, &gDynamicFaceLeft);
	gDevice->CreateTexture2D(&textureDesc, NULL, &gDynamicFaceRight);

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZERO(renderTargetViewDesc);
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	gDevice->CreateRenderTargetView(gTextureColour, &renderTargetViewDesc, &gRenderTargetView[0]);
	gDevice->CreateRenderTargetView(gTextureNormal, &renderTargetViewDesc, &gRenderTargetView[1]);
	gDevice->CreateRenderTargetView(gTexturePosition, &renderTargetViewDesc, &gRenderTargetView[2]);
	gDevice->CreateRenderTargetView(gDynamicFaceUp, &renderTargetViewDesc, &gDynamicCubeTargetViewUp);
	gDevice->CreateRenderTargetView(gDynamicFaceFront, &renderTargetViewDesc, &gDynamicCubeTargetViewFront);
	gDevice->CreateRenderTargetView(gDynamicFaceBack, &renderTargetViewDesc, &gDynamicCubeTargetViewBack);
	gDevice->CreateRenderTargetView(gDynamicFaceLeft, &renderTargetViewDesc, &gDynamicCubeTargetViewLeft);
	gDevice->CreateRenderTargetView(gDynamicFaceRight, &renderTargetViewDesc, &gDynamicCubeTargetViewRight);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZERO(shaderResourceViewDesc);
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	gDevice->CreateShaderResourceView(gTextureColour, &shaderResourceViewDesc, &gColorViewMap);
	gDevice->CreateShaderResourceView(gTextureNormal, &shaderResourceViewDesc, &gNormalViewMap);
	gDevice->CreateShaderResourceView(gTexturePosition, &shaderResourceViewDesc, &gPositionViewMap);


	// create depth stencil texture
	D3D11_TEXTURE2D_DESC depthStencilTextDesc;
	ZERO(depthStencilTextDesc);
	depthStencilTextDesc.Width = WIDTH;
	depthStencilTextDesc.Height = HEIGHT;
	depthStencilTextDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthStencilTextDesc.MipLevels = 1;
	depthStencilTextDesc.ArraySize = 1;
	depthStencilTextDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthStencilTextDesc.CPUAccessFlags = 0;
	depthStencilTextDesc.MiscFlags = 0;
	depthStencilTextDesc.SampleDesc.Count = 1;
	gDevice->CreateTexture2D(&depthStencilTextDesc, NULL, &gTextureDepth);

	// create depth stencil state (Output Merger stuff)
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZERO(depthStencilDesc);
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = false;

	gDevice->CreateDepthStencilState(&depthStencilDesc, &gDepthStencilState);

	// create depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZERO(depthStencilViewDesc);
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	gDevice->CreateDepthStencilView(gTextureDepth, &depthStencilViewDesc, &gDepthStencilView);

	// finally, create a shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC depthSRV;
	ZERO(depthSRV);
	depthSRV.Format = DXGI_FORMAT_R32_FLOAT;
	depthSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	depthSRV.Texture2D.MostDetailedMip = 0;
	depthSRV.Texture2D.MipLevels = 1;
	gDevice->CreateShaderResourceView(gTextureDepth, &depthSRV, &gShaderResourceViewDepth);
}
void createSampleState()
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the Sample State
	gDevice->CreateSamplerState(&sampDesc, &sampleState);
}
void createConstantBuffer()
{
	dsData = (dsStruct*)_aligned_malloc(sizeof(dsStruct), 16);

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	desc.ByteWidth = sizeof(dsStruct);
	gDevice->CreateBuffer(&desc, NULL, &dsBuffer);
	desc.ByteWidth = sizeof(XMVECTOR);
	gDevice->CreateBuffer(&desc, NULL, &camPosBuffer);
	gDevice->CreateBuffer(&desc, NULL, &rotationBuffer);
	desc.ByteWidth = sizeof(XMMATRIX);
	gDevice->CreateBuffer(&desc, NULL, &ViewProjBuffer);
	gDevice->CreateBuffer(&desc, NULL, &projectionBuffer);
	gDevice->CreateBuffer(&desc, NULL, &viewBuffer);
	gDevice->CreateBuffer(&desc, NULL, &SkymapWVPBuffer);
	gDevice->CreateBuffer(&desc, NULL, &BFCTestWorldBuffer);
	gDevice->CreateBuffer(&desc, NULL, &DCViewProjBuffer);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &XMMatrixIdentity();
	gDevice->CreateBuffer(&desc, &data, &identityBuffer);
}
void updateCamera()
{
	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);
	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camYaw);

	camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
	camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

	camPos += moveLeftRight * camRight;
	camPos += moveBackForward * camForward;
	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;
	XMStoreFloat4(&camPosF4, camPos);
	camTarget = camPos + camTarget;
	camView = XMMatrixLookAtLH(camPos, camTarget, camUp);

}
void InitDirectInput(HINSTANCE hInstance)
{
	DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&DirectInput,
		NULL);

	DirectInput->CreateDevice(GUID_SysKeyboard,
		&DIKeyboard,
		NULL);

	DirectInput->CreateDevice(GUID_SysMouse,
		&DIMouse,
		NULL);

	DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	DIMouse->SetDataFormat(&c_dfDIMouse);
	DIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

}
void DetectInput()
{
	DIMOUSESTATE mouseCurrState;
	BYTE keyboardState[256];

	DIKeyboard->Acquire();
	DIMouse->Acquire();

	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	float speed = 0.001f;

	if (keyboardState[DIK_A] & 0x80)
	{

		moveLeftRight -= speed;
	}
	if (keyboardState[DIK_D] & 0x80)
	{
		moveLeftRight += speed;
	}
	if (keyboardState[DIK_W] & 0x80)
	{
		moveBackForward += speed;
	}
	if (keyboardState[DIK_S] & 0x80)
	{
		moveBackForward -= speed;
	}
	if (keyboardState[DIK_R] & 0x80)
	{
		shootActivated = true;
	}

	if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
	{
		camYaw += mouseLastState.lX * 0.001f;

		camPitch -= mouseCurrState.lY * 0.001f;

		mouseLastState = mouseCurrState;
	}
	//left click
	if (mouseCurrState.rgbButtons[0])
	{
		createBullet();
	}
	updateCamera();

	return;
}
void textureCreation(char* filename, ID3D11ShaderResourceView* &texture)
{
	int bpp = 0;
	int height = 0;
	int width = 0;

	unsigned char* rgb = stbi_load(filename, &width, &height, &bpp, STBI_rgb_alpha);

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.MiscFlags = 0;
	texDesc.CPUAccessFlags = 0;

	ID3D11Texture2D *pTexture = NULL;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = (void*)rgb;
	data.SysMemPitch = width * 4 * sizeof(char);
	gDevice->CreateTexture2D(&texDesc, &data, &pTexture);

	D3D11_SHADER_RESOURCE_VIEW_DESC resViewDesc;
	ZeroMemory(&resViewDesc, sizeof(resViewDesc));
	resViewDesc.Format = texDesc.Format;
	resViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resViewDesc.Texture2D.MipLevels = texDesc.MipLevels;
	resViewDesc.Texture2D.MostDetailedMip = 0;
	gDevice->CreateShaderResourceView(pTexture, &resViewDesc, &texture);
	pTexture->Release();
}
void UpdateBulletMatrix()
{
	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	for (int i = 0; i < bullets.size(); i++)
	{
		bullets.at(i).worldMatrix = XMMatrixTranslationFromVector(bullets.at(i).direction * (bullets.at(i).increment * 3));
		gDeviceContext->Map(bullets.at(i).worldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
		memcpy(mappedMemory.pData, &bullets.at(i).worldMatrix, sizeof(XMMATRIX));
		gDeviceContext->Unmap(bullets.at(i).worldBuffer, 0);
	}
}
void checkBulletCollision()
{
	XMVECTOR temp;
	XMFLOAT4 result;
	int radius = 1;
	for (int i = 0; i < bullets.size(); i++)
	{
		if (!bullets.at(i).render)
			continue;
		for (int k = 0; k < 24; k++)
		{
			temp = XMLoadFloat4(&XMFLOAT4(bullets.at(i).vertices[k].x, bullets.at(i).vertices[k].y, bullets.at(i).vertices[k].z, 1));
			temp = XMVector4Transform(temp, bullets.at(i).worldMatrix);
			XMStoreFloat4(&result, temp);
			//check for each cube
			for (int j = 0; j < 2; j++)
			{
				if (cubes.at(j).render)
				{
					if (result.x < cubes.at(j).centerPos[0] + radius && result.x > cubes.at(j).centerPos[0] - radius &&
						result.y < cubes.at(j).centerPos[1] + radius && result.y > cubes.at(j).centerPos[1] - radius &&
						result.z < cubes.at(j).centerPos[2] + radius && result.z > cubes.at(j).centerPos[2] - radius)
					{
						cubes.at(j).render = false;
						bullets.at(i).render = false;
					}
				}
			}

		}
	}

}
void updateBuffers()
{
	dsData->viewProj = (camView * Projection);
	dsData->camPos = camPos;
	viewProjData = &(camView * Projection);
	SkymapWVP = &(SkymapWorld * camView * Projection);
	camPosValue = &camPos;
	rotation = &XMVectorSet(gIncrement / 3, 0, 0, 0);
	UpdateBulletMatrix();
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	gDeviceContext->Map(dsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, dsData, sizeof(dsStruct));
	gDeviceContext->Unmap(dsBuffer, 0);

	gDeviceContext->Map(camPosBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, camPosValue, sizeof(XMMATRIX));
	gDeviceContext->Unmap(camPosBuffer, 0);

	gDeviceContext->Map(ViewProjBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, viewProjData, sizeof(XMMATRIX));
	gDeviceContext->Unmap(ViewProjBuffer, 0);

	gDeviceContext->Map(rotationBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, rotation, sizeof(XMVECTOR));
	gDeviceContext->Unmap(rotationBuffer, 0);

	gDeviceContext->Map(projectionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &Projection, sizeof(XMMATRIX));
	gDeviceContext->Unmap(projectionBuffer, 0);

	gDeviceContext->Map(viewBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &camView, sizeof(XMMATRIX));
	gDeviceContext->Unmap(viewBuffer, 0);

	gDeviceContext->Map(SkymapWVPBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, SkymapWVP, sizeof(XMMATRIX));
	gDeviceContext->Unmap(SkymapWVPBuffer, 0);

	gDeviceContext->Map(BFCTestWorldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, &cubeWorldMatrix, sizeof(XMMATRIX));
	gDeviceContext->Unmap(BFCTestWorldBuffer, 0);
}
void createSMRenderStates()
{
	D3D11_RASTERIZER_DESC cmdesc;

	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.FrontCounterClockwise = false;
	cmdesc.CullMode = D3D11_CULL_NONE;
	gDevice->CreateRasterizerState(&cmdesc, &RSCullNone);

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	gDevice->CreateDepthStencilState(&dssDesc, &DSLessEqual);
}
void updateScene()
{
	SkymapWorld = XMMatrixIdentity();

	Scale = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	Translation = XMMatrixTranslation(XMVectorGetX(camPos), XMVectorGetY(camPos), XMVectorGetZ(camPos));

	SkymapWorld = Scale * Translation;
}
void initialize()
{
	SetViewport();
	CreateVSPSShaders();
	CreateHSDSShaders();
	CreateHMShaders();
	CreateSMShaders();
	CreateBFCShaders();
	CreateBulletShaders();
	CreateGoalShaders();
	CreateDCTestCubeShaders();
	createDynamicCubeTextures();
	CreateDynamicCubeShaders();
	createGoal();
	CreateBFCTestShaders();
	CreateWaterCubeShaders();
	createWaterCube();
	createDCTestCube();
	CreateDynamicCube();
	CreateBFCTest();
	createRenderTargets();
	CreateFSShaders();
	CreateFullScreenQuad();
	createConstantBuffer();
	HMVertices = InitializeHeightMap();
	CreateCube();
	createSampleState();
	textureCreation("endScreen.jpg", gEndscreenTex);
	textureCreation("Brick_wall_COLOR.jpg", gCubeColourmap);
	textureCreation("Brick_wall_DISP.png", gCubeDisplacementmap);
	textureCreation("Brick_wall_NORM.jpg", gCubeNormalmap);
	textureCreation("Brick_wall_SPEC.png", gCubeSpecView);
	textureCreation("waterTexture.png", gWaterTexture);
	textureCreation("grass.jpg", gHMTexture);
	CreateDDSTextureFromFile(gDevice, L"skymap.dds", nullptr, &gSkyMapView);
	createSMRenderStates();
	createSMCube();
	camPos = XMVectorSet(0.0f, 0.0f, -2.0f, 1.0f);
	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	camView = XMMatrixLookAtLH(camPos, camTarget, camUp);

	dynamicCubeUpView = XMMatrixLookAtLH(upFaceCamPos, upFaceCamPos + XMVectorSet(0, 1, 0, 0), camUp);
	dynamicCubeFrontView = XMMatrixLookAtLH(frontFaceCamPos, frontFaceCamPos + XMVectorSet(0, 0, 1, 0), camUp);
	dynamicCubeBackView = XMMatrixLookAtLH(backFaceCamPos, backFaceCamPos + XMVectorSet(0, 0, -1, 0), camUp);
	dynamicCubeLeftView = XMMatrixLookAtLH(leftFaceCamPos, leftFaceCamPos + XMVectorSet(-1, 0, 0, 0), camUp);
	dynamicCubeRightView = XMMatrixLookAtLH(rightFaceCamPos, rightFaceCamPos + XMVectorSet(1, 0, 0, 0), camUp);

	Projection = XMMatrixPerspectiveFovLH(0.45f * XM_PI, WIDTH / HEIGHT, 0.1, 20.0f);
	ProjectionDC = XMMatrixPerspectiveFovLH(0.65f * XM_PI, WIDTH / HEIGHT, 0.1, 20.0f);
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	MSG msg = { 0 };
	HWND wndHandle = InitWindow(hInstance);

	if (wndHandle)
	{
		CreateDirect3DContext(wndHandle);
		initialize();
		ShowWindow(wndHandle, nCmdShow);
		InitDirectInput(hInstance);

		while (WM_QUIT != msg.message)
		{
			auto start = std::chrono::high_resolution_clock::now();

			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				shootCooldown -= deltaTime;
				gIncrement += 0.5f * deltaTime;
				cubeWorldMatrix = XMMatrixRotationY(gIncrement/2);
				for (int i = 0; i < bullets.size(); i++)
				{
					bullets.at(i).increment += 0.5f * deltaTime;
				}
				checkBulletCollision();
				DetectInput();
				walkOnHM();
				updateScene();
				updateBuffers();
				render();
				gSwapChain->Present(0, 0);
			}
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> elapsed = end - start;
			deltaTime = elapsed.count();

		}

		gVertexBuffer->Release();
		gVertexBufferDynamic->Release();
		gVertexBufferTwo->Release();
		gGoalVertexBuffer->Release();

		gVertexLayout->Release();
		gVertexShader->Release();
		gPixelShader->Release();
		gBackbufferRTV->Release();
		gSwapChain->Release();
		gDevice->Release();
		gDeviceContext->Release();
		DestroyWindow(wndHandle);
	}

	return (int)msg.wParam;
}

HWND InitWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"DirectX Renderer";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND handle = CreateWindow(
		L"DirectX Renderer",
		L"DirectX Renderer",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	return handle;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HRESULT CreateDirect3DContext(HWND wndHandle)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = wndHandle;                           // the window to be used
	scd.SampleDesc.Count = 1;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&gSwapChain,
		&gDevice,
		NULL,
		&gDeviceContext);

	if (SUCCEEDED(hr))
	{
		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		gDevice->CreateRenderTargetView(pBackBuffer, NULL, &gBackbufferRTV);
		pBackBuffer->Release();

		// set the render target as the back buffer
		gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, NULL);
	}
	return hr;
}