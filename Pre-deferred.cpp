//--------------------------------------------------------------------------------------
// BTH - Stefan Petersson 2014.
//	   - modified by FLL
//--------------------------------------------------------------------------------------
#include <windows.h>
#include <math.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment (lib, "D3D10_1.lib")
#pragma comment (lib, "DXGI.lib")
#pragma comment (lib, "D2D1.lib")
#pragma comment (lib, "dwrite.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include <D3D9Types.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include"bth_image.h"
using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#include <dinput.h.>

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

ID3D11Buffer* gVertexBuffer = nullptr;
ID3D11InputLayout* gVertexLayout = nullptr;

ID3D11DepthStencilView* depthStencilView;
ID3D11Texture2D* depthStencilBuffer;
ID3D11Buffer* gDrawDepthCB = NULL;

ID3D11ShaderResourceView* gTextureView = nullptr;


ID3D11VertexShader* gVertexShader = nullptr;
ID3D11GeometryShader* gGeometryShader = nullptr;
ID3D11PixelShader* gPixelShader = nullptr;
ID3D11VertexShader* gVertexShaderFS = nullptr;
ID3D11PixelShader* gPixelShaderFS = nullptr;

ID3D11Buffer* gIndexBuffer = nullptr;

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
XMMATRIX lightView;

XMMATRIX* vsData;
XMVECTOR* psData;

ID3D11Texture2D* gTextureColour;
ID3D11Texture2D* gTextureNormal;

ID3D11Texture2D* gTextureDepth;
ID3D11DepthStencilState* gDepthStencilState = nullptr;
ID3D11RenderTargetView* gRenderTargetView;
ID3D11DepthStencilView* gDepthStencilView;
ID3D11ShaderResourceView* gShaderResourceViewMap;
ID3D11ShaderResourceView* gShaderResourceViewDepth;
ID3D11Buffer* vsBuffer = nullptr;
ID3D11Buffer* psBuffer = nullptr;
ID3D11SamplerState* gSamplerState = NULL;

IDirectInputDevice8* DIKeyboard;
IDirectInputDevice8* DIMouse;
HWND hwnd = NULL;
DIMOUSESTATE mouseLastState;
LPDIRECTINPUT8 DirectInput;

float moveLeftRight = 0.0f;
float moveBackForward = 0.0f;

float camYaw = 0.0f;
float camPitch = 0.0f;
HRESULT CreateShaders()
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
		},
		{
			"NORMAL",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			20,							 // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
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

	ID3DBlob* pGS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"GeometryShader.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"GS_main",		// entry point
		"gs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				// effect compile options
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	if (FAILED(result))
	{
		if (errorBlob)
			errorBlob->Release();
		if (pGS)
			pGS->Release();
		return result;
	}
	gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &gGeometryShader);

	pGS->Release();
	return S_OK;
}
	struct Vertex
	{
		float x, y, z;
		float u, v;
		float n1, n2, n3;
	};
void CreateTriangleData()
{
	Vertex vertices[24] =
	{
		// Front Face
		-1.0f, -1.0f, -1.0f, //Coordinates
		0.0f, 1.0f,			 //UV
		0.0f, 0.0f, 1.0f,	 //normal
		-1.0f,  1.0f, -1.0f,//Coordinates
		0.0f, 0.0f,			//UV
		0.0f, 0.0f, 1.0f,	 //normal
		1.0f,  1.0f, -1.0f, //Coordinates
		1.0f, 0.0f,			//UV
		0.0f, 0.0f, 1.0f,	 //normal
		1.0f, -1.0f, -1.0f,//Coordinates
		1.0f, 1.0f,			//UV
		0.0f, 0.0f, 1.0f,	 //normal
		// Back Face
		-1.0f, -1.0f, 1.0f,//Coordinates
		1.0f, 1.0f,		   //UV
		0.0f, 0.0f, -1.0f,	 //normal
		1.0f, -1.0f, 1.0f,//Coordinates
		0.0f, 1.0f,		  //UV
		0.0f, 0.0f, -1.0f,	 //normal
		1.0f,  1.0f, 1.0f,//Coordinates
		0.0f, 0.0f,		  //UV
		0.0f, 0.0f, -1.0f,	 //normal
		-1.0f,  1.0f, 1.0f,//Coordinates
		1.0f, 0.0f,		  //UV
		0.0f, 0.0f, -1.0f,	 //normal

		// Top Face
		-1.0f, 1.0f, -1.0f,//Coordinates
		0.0f, 1.0f,		   //UV
		0.0f, 1.0f, 0.0f,	 //normal
		-1.0f, 1.0f,  1.0f, //Coordinates
		0.0f, 0.0f,			//UV
		0.0f, 1.0f, 0.0f,	 //normal
		1.0f, 1.0f,  1.0f,//Coordinates
		1.0f, 0.0f,		  //UV
		0.0f, 1.0f, 0.0f,	 //normal
		1.0f, 1.0f, -1.0f,//Coordinates
		1.0f, 1.0f,		  //UV
		0.0f, 1.0f, 0.0f,	 //normal

		// Bottom Face
		-1.0f, -1.0f, -1.0f,//Coordinates
		1.0f, 1.0f,			//UV
		0.0f, -1.0f, 0.0f,	 //normal
		1.0f, -1.0f, -1.0f,//Coordinates
		0.0f, 1.0f,		   //UV
		0.0f, -1.0f, 0.0f,	 //normal
		1.0f, -1.0f,  1.0f,//Coordinates
		0.0f, 0.0f,		   //UV
		0.0f, -1.0f, 0.0f,	 //normal
		-1.0f, -1.0f,  1.0f,//Coordinates
		1.0f, 0.0f,			//UV
		0.0f, -1.0f, 0.0f,	 //normal

		// Left Face
		-1.0f, -1.0f,  1.0f,//Coordinates
		0.0f, 1.0f,			//UV
		-1.0f, 0.0f, 0.0f,	 //normal
		-1.0f,  1.0f,  1.0f,//Coordinates
		0.0f, 0.0f,			//UV
		-1.0f, 0.0f, 0.0f,	 //normal
		-1.0f,  1.0f, -1.0f,//Coordinates
		1.0f, 0.0f,			//UV
		-1.0f, 0.0f, 0.0f,	 //normal
		-1.0f, -1.0f, -1.0f,//Coordinates
		1.0f, 1.0f,			//UV
		-1.0f, 0.0f, 0.0f,	 //normal

		// Right Face
		1.0f, -1.0f, -1.0f,//Coordinates
		0.0f, 1.0f,		   //UV
		1.0f, 0.0f, 0.0f,	 //normal
		1.0f,  1.0f, -1.0f,//Coordinates
		0.0f, 0.0f,		   //UV
		1.0f, 0.0f, 0.0f,	 //normal
		1.0f,  1.0f,  1.0f,//Coordinates
		1.0f, 0.0f,		   //UV
		1.0f, 0.0f, 0.0f,	 //normal
		1.0f, -1.0f,  1.0f,//Coordinates
		1.0f, 1.0f,	       //UV
		1.0f, 0.0f,0.0f,	 //normal


	};

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
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData;

	indexData.pSysMem = indices;
	gDevice->CreateBuffer(&indexBufferDesc, &indexData, &gIndexBuffer);
	gDeviceContext->IASetIndexBuffer(gIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

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

void Render()
{
	float clearColor[] = { 0, 0, 0, 1 };

	gDeviceContext->ClearRenderTargetView(gBackbufferRTV, clearColor);
	gDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);
	gDeviceContext->VSSetConstantBuffers(0, 1, &vsBuffer);
	gDeviceContext->PSSetConstantBuffers(0, 1, &psBuffer);
	UINT32 vertexSize = sizeof(Vertex);
	UINT32 offset = 0;
	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &vertexSize, &offset);
	gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, gDepthStencilView);

	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->IASetInputLayout(gVertexLayout);

	gDeviceContext->DrawIndexed(36, 0, 0);
}
void createConstantBuffer()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(XMMATRIX);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	gDevice->CreateBuffer(&desc, NULL, &vsBuffer);
	desc.ByteWidth = sizeof(XMVECTOR);
	gDevice->CreateBuffer(&desc, NULL, &psBuffer);
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

	if (keyboardState[DIK_ESCAPE] & 0x80)
		PostMessage(hwnd, WM_DESTROY, 0, 0);

	float speed = 0.0005f;

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
	if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
	{
		camYaw += mouseLastState.lX * 0.001f;

		camPitch += mouseCurrState.lY * 0.001f;

		mouseLastState = mouseCurrState;
	}

	updateCamera();

	return;
}
void createDepthbuffer()
{
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = WIDTH;
	depthStencilDesc.Height = HEIGHT;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	gDevice->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	gDevice->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
	gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, depthStencilView);
}
void textureCreation()
{
	D3D11_TEXTURE2D_DESC bthTexDesc;
	ZeroMemory(&bthTexDesc, sizeof(bthTexDesc));
	bthTexDesc.Width = BTH_IMAGE_WIDTH;
	bthTexDesc.Height = BTH_IMAGE_HEIGHT;
	bthTexDesc.MipLevels = bthTexDesc.ArraySize = 1;
	bthTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bthTexDesc.SampleDesc.Count = 1;
	bthTexDesc.SampleDesc.Quality = 0;
	bthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	bthTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bthTexDesc.MiscFlags = 0;
	bthTexDesc.CPUAccessFlags = 0;

	ID3D11Texture2D *pTexture = NULL;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = (void*)BTH_IMAGE_DATA;
	data.SysMemPitch = BTH_IMAGE_WIDTH * 4 * sizeof(char);
	gDevice->CreateTexture2D(&bthTexDesc, &data, &pTexture);

	D3D11_SHADER_RESOURCE_VIEW_DESC resViewDesc;
	ZeroMemory(&resViewDesc, sizeof(resViewDesc));
	resViewDesc.Format = bthTexDesc.Format;
	resViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resViewDesc.Texture2D.MipLevels = bthTexDesc.MipLevels;
	resViewDesc.Texture2D.MostDetailedMip = 0;
	gDevice->CreateShaderResourceView(pTexture, &resViewDesc, &gTextureView);
	pTexture->Release();
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	MSG msg = { 0 };
	HWND wndHandle = InitWindow(hInstance); //1. Skapa fönster

	if (wndHandle)
	{
		CreateDirect3DContext(wndHandle); //2. Skapa och koppla SwapChain, Device och Device Context

		SetViewport(); //3. Sätt viewport
		//createRenderTargets();
		CreateShaders(); //4. Skapa vertex- och pixel-shaders
		createConstantBuffer();
		createDepthbuffer();
		CreateTriangleData(); //5. Definiera triangelvertiser, 6. Skapa vertex buffer, 7. Skapa input layout
		textureCreation();
		ShowWindow(wndHandle, nCmdShow);
		camPos = XMVectorSet(0.0f, 0.0f, -2.0f, 0.0f);
		camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		InitDirectInput(hInstance);
		float gIncrement = 0;
		XMVECTOR lightPos = XMVectorSet(0, 0, -2, 0);
		XMMATRIX Projection = XMMatrixPerspectiveFovLH(0.45f * XM_PI, WIDTH / HEIGHT, 0.1, 20.0f);

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				gIncrement += 0.0001f;
				DetectInput();
				D3D11_MAPPED_SUBRESOURCE mappedMemory;
				vsData = &(camView * Projection);
				psData = &camPos;
				gDeviceContext->Map(vsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
				memcpy(mappedMemory.pData, vsData, sizeof(XMMATRIX));
				gDeviceContext->Unmap(vsBuffer, 0);

				gDeviceContext->Map(psBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
				memcpy(mappedMemory.pData, psData, sizeof(XMVECTOR));
				gDeviceContext->Unmap(psBuffer, 0);
				Render(); //8. Rendera

				gSwapChain->Present(0, 0); //9. Växla front- och back-buffer
			}
		}

		gVertexBuffer->Release();

		gVertexLayout->Release();
		gVertexShader->Release();
		gPixelShader->Release();
		depthStencilView->Release();
		depthStencilBuffer->Release();
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
	wcex.lpszClassName = L"BTH_D3D_DEMO";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND handle = CreateWindow(
		L"BTH_D3D_DEMO",
		L"BTH Direct3D Demo",
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