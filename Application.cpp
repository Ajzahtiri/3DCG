#include "Application.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;

		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
			{
				DestroyWindow(hWnd);
			}
			return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

Application::Application()
{
	_hInst = nullptr;
	_hWnd = nullptr;
	_driverType = D3D_DRIVER_TYPE_NULL;
	_featureLevel = D3D_FEATURE_LEVEL_11_0;
	_pd3dDevice = nullptr;
	_pImmediateContext = nullptr;
	_pSwapChain = nullptr;
	_pRenderTargetView = nullptr;
	_pVertexShader = nullptr;
	_pPixelShader = nullptr;
	_pVertexLayout = nullptr;
	_pConstantBuffer = nullptr;
}
Application::~Application()
{
	Cleanup();
}
void Application::Cleanup()
{
	if (_pImmediateContext) _pImmediateContext->ClearState();

	if (_pConstantBuffer) _pConstantBuffer->Release();
	if (_pVertexLayout) _pVertexLayout->Release();
	if (_pVertexShader) _pVertexShader->Release();
	if (_pPixelShader) _pPixelShader->Release();
	if (_pRenderTargetView) _pRenderTargetView->Release();
	if (_pSwapChain) _pSwapChain->Release();
	if (_pImmediateContext) _pImmediateContext->Release();
	if (_pd3dDevice) _pd3dDevice->Release();
	if (_depthStencilView) _depthStencilView->Release();
	if (_depthStencilBuffer) _depthStencilBuffer->Release();
	if (_wireFrame) _wireFrame->Release();

	_DIKeyboard->Unacquire();
	_DIMouse->Unacquire();
	_DirectInput->Release();
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
    if (FAILED(InitWindow(hInstance, nCmdShow)))
	{
        return E_FAIL;
	}

    RECT rc;
    GetClientRect(_hWnd, &rc);
    _WindowWidth = rc.right - rc.left;
    _WindowHeight = rc.bottom - rc.top;

	//inits device
    if (FAILED(InitDirectX()))
    {
        Cleanup();

        return E_FAIL;
    }

	//inits input
	if (FAILED(InitInput(hInstance)))
	{
		Cleanup();

		return E_FAIL;
	}

	// Initialize the world matrix
	XMStoreFloat4x4(&_cube1, XMMatrixIdentity());
	XMStoreFloat4x4(&_cube2, XMMatrixIdentity());

	_cubbe1.Initialise(_pd3dDevice);
	_cubbe2.Initialise(_pd3dDevice);

	//inits camera (view matrix)
	if (FAILED(_cam.Initialise()))
	{
		Cleanup();

		return E_FAIL;
	}

    // Initialize the projection matrix
	XMStoreFloat4x4(&_projection, XMMatrixPerspectiveFovLH(XM_PIDIV2, _WindowWidth / (FLOAT) _WindowHeight, 1.0f, 100.0f));

	return S_OK;
}
HRESULT Application::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"TutorialWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	_hInst = hInstance;
	RECT rc = { 0, 0, 1920, 1080 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	_hWnd = CreateWindow(L"TutorialWindowClass", L"DX11 Framework", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if (!_hWnd)
		return E_FAIL;

	ShowWindow(_hWnd, nCmdShow);

	return S_OK;
}
HRESULT Application::InitDirectX()
{
	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = _WindowWidth;
	sd.BufferDesc.Height = _WindowHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = _hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pd3dDevice, &_featureLevel, &_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
		return hr;

	// Create depth buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = _WindowWidth;
	depthStencilDesc.Height = _WindowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencilBuffer);
	_pd3dDevice->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthStencilView);

	// Create wireframe raster state
	_isWF = false;
	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	wfdesc.CullMode = D3D11_CULL_NONE;
	hr = _pd3dDevice->CreateRasterizerState(&wfdesc, &_wireFrame);

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	if (FAILED(hr))
		return hr;

	hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
	pBackBuffer->Release();

	if (FAILED(hr))
		return hr;

	_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _depthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)_WindowWidth;
	vp.Height = (FLOAT)_WindowHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_pImmediateContext->RSSetViewports(1, &vp);

	InitShadersAndInputLayout();


	// Create the constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = _pd3dDevice->CreateBuffer(&bd, nullptr, &_pConstantBuffer);

	if (FAILED(hr))
		return hr;

	return S_OK;
}
HRESULT Application::InitInput(HINSTANCE hInstance)
{
	HRESULT hr;

	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_DirectInput, NULL);

	hr = _DirectInput->CreateDevice(GUID_SysKeyboard, &_DIKeyboard, NULL);
	hr = _DirectInput->CreateDevice(GUID_SysMouse, &_DIMouse, NULL);

	hr = _DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = _DIKeyboard->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = _DIMouse->SetDataFormat(&c_dfDIMouse);
	hr = _DIMouse->SetCooperativeLevel(_hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

	return true;
}
HRESULT Application::InitCamera(HINSTANCE hInstance)
{

	_cam.Initialise();

	return true;
}
HRESULT Application::InitShadersAndInputLayout()
{
	HRESULT hr;

    // Compile the vertex shader
    ID3DBlob* pVSBlob = nullptr;
    hr = CompileShaderFromFile(L"DX11 Framework.fx", "VS", "vs_4_0", &pVSBlob);

    if (FAILED(hr))
    {
        MessageBox(nullptr,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

	// Create the vertex shader
	hr = _pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_pVertexShader);

	if (FAILED(hr))
	{	
		pVSBlob->Release();
        return hr;
	}

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
    hr = CompileShaderFromFile(L"DX11 Framework.fx", "PS", "ps_4_0", &pPSBlob);

    if (FAILED(hr))
    {
        MessageBox(nullptr,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

	// Create the pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_pPixelShader);
	pPSBlob->Release();

    if (FAILED(hr))
        return hr;

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Lighting
	// Light direction from surface (XYZ)
	_light.mLightVecW = XMFLOAT3(5.25f, 1.0f, -1.0f);

	// Diffuse lighting properties (RGBA)
	_light.mDiffuseMtrl = XMFLOAT4(0.8f, 0.5f, 0.5f, 1.0f);
	_light.mDiffuseLight = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// Ambient lighting properties (RGBA)
	_light.mAmbientMtrl = XMFLOAT3(0.2f, 0.2f, 0.2f);
	_light.mAmbientLight = XMFLOAT3(0.2f, 0.2f, 0.2f);

	// Specular lighting properties (RGBA)
	//_light.mSpecularMtrl = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	//_light.mSpecularLight = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	//_light.mSpecularPower = 5.0f;

	UINT numElements = ARRAYSIZE(layout);

    // Create the input layout
	hr = _pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
                                        pVSBlob->GetBufferSize(), &_pVertexLayout);
	pVSBlob->Release();

	if (FAILED(hr))
        return hr;

    // Set the input layout
    _pImmediateContext->IASetInputLayout(_pVertexLayout);

	return hr;
}

HRESULT Application::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,         dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

    if (FAILED(hr))
    {
        if (pErrorBlob != nullptr)
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

        if (pErrorBlob) pErrorBlob->Release();

        return hr;
    }

    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}

void Application::DetectInput(double time)
{
	DIMOUSESTATE mouseCurrState;
	BYTE keyboardState[256];

	_DIKeyboard->Acquire(); 
	_DIMouse->Acquire();

	_DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);
	_DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	//move camera
	if (keyboardState[DIK_A] & 0x80)
	{
		_cam.moveLeft();
	}

	if (keyboardState[DIK_D] & 0x80)
	{
		_cam.moveRight();
	}

	if (keyboardState[DIK_W] & 0x80)
	{
		_cam.moveForward();
	}

	if (keyboardState[DIK_S] & 0x80)
	{
		_cam.moveBack();
	}

	if (keyboardState[DIK_Q] & 0x80)
	{
		_cam.moveUp();
	}

	if (keyboardState[DIK_E] & 0x80)
	{
		_cam.moveDown();
	}
	
	if (keyboardState[DIK_F1] & 0x80)
	{
		if (_isWF == true)
		{
			_pImmediateContext->RSSetState(nullptr);
			_isWF = false;
		}
		else
		{
			_pImmediateContext->RSSetState(_wireFrame);
			_isWF = true;
		}
	}
	return;
}

void Application::Update(double time)
{
	// Update our time
	static float t = 0.0f;

	if (_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();

		if (dwTimeStart == 0)
			dwTimeStart = dwTimeCur;

		t = (dwTimeCur - dwTimeStart) / 1000.0f;
	}

	//XMStoreFloat4x4(&_view, XMMatrixLookAtLH(Eye, At, Up));
	// update the camera
	XMStoreFloat4x4(&_view, XMMatrixLookAtLH(_cam.getEyeVector(), _cam.getAtVector(), _cam.getUpVector()));

	// Animate the cube
	XMMATRIX _rotation1, _rotation2, _scale1, _scale2, _translation1, _translation2, _final1, _final2;

	_rotation1 = XMMatrixRotationY(-t);
	_rotation2 = XMMatrixRotationY(t);
	_translation1 = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	_translation2 = XMMatrixTranslation(5.0f, 0.0f, 0.0f);
	_scale1 = XMMatrixScaling(1, 1, 1);
	_scale2 = XMMatrixScaling(.5, .5, .5);

	_final1 = _rotation1 * _translation1 * _scale1;
	_final2 =  _translation2 * _rotation2 * _scale2;

	XMStoreFloat4x4(&_cube1, _final1);
	XMStoreFloat4x4(&_cube2, _final2);

	_cubbe1.SetRotation(t, t, t);
	_cubbe1.SetScale(1, 1, 1);
	_cubbe1.SetTranslation(.0f, .0f, .0f);
	_cubbe2.SetRotation(t, t, t);
	_cubbe2.SetScale(0.5f, 0.5f, 1);
	_cubbe2.SetTranslation(.0f, 2.0f, 2.0f);
	_cubbe1.Update(t);
	_cubbe2.Update(-t);
}

void Application::Draw()
{
    // Clear the back buffer
    float ClearColor[4] = {0.1f, 0.1f, 0.1f, 1.0f}; // red,green,blue,alpha
    _pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ClearColor);
	_pImmediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	XMMATRIX world1 = XMLoadFloat4x4(&_cubbe1.GetWorld());
	XMMATRIX world2 = XMLoadFloat4x4(&_cubbe2.GetWorld());
	XMMATRIX view = XMLoadFloat4x4(&_view);
	XMMATRIX projection = XMLoadFloat4x4(&_projection);

	// Draw cube 1
    ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(world1);
	cb.mView = XMMatrixTranspose(view);
	cb.mProjection = XMMatrixTranspose(projection);
	//cb.mEyePosW = _cam.GetPosition();
	cb.mLight = _light;


	_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

	//_pImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
	//_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	//_pImmediateContext->PSSetShader(_pPixelShader, nullptr, 0);
	//_pImmediateContext->DrawIndexed(36, 0, 0);        
	_cubbe1.Draw(_pd3dDevice, _pImmediateContext, _pConstantBuffer, &cb, _pVertexShader, _pPixelShader);
	// Draw cube 2
	cb.mWorld = XMMatrixTranspose(world2);
	cb.mView = XMMatrixTranspose(view);
	cb.mProjection = XMMatrixTranspose(projection);
	cb.mLight = _light;
	//cb.mEyePosW = _cam.GetPosition();

	_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	
	//_pImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
	//_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	//_pImmediateContext->PSSetShader(_pPixelShader, nullptr, 0);
	//_pImmediateContext->DrawIndexed(36, 0, 0);*/
	_cubbe2.Draw(_pd3dDevice, _pImmediateContext, _pConstantBuffer, &cb, _pVertexShader, _pPixelShader);
    _pSwapChain->Present(0, 0);
}

void	Application::StartTimer()
{
	LARGE_INTEGER frequencyCount;
	QueryPerformanceFrequency(&frequencyCount);

	_countsPerSecond = double(frequencyCount.QuadPart);

	QueryPerformanceCounter(&frequencyCount);
	_counterStart = frequencyCount.QuadPart;
}
double	Application::GetTime()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	return double(currentTime.QuadPart - _counterStart) / _countsPerSecond;
}
double	Application::GetFrameTime()
{
	LARGE_INTEGER currentTime;
	__int64 tickCount;
	QueryPerformanceCounter(&currentTime);

	tickCount = currentTime.QuadPart - _frameTimeOld;
	_frameTimeOld = currentTime.QuadPart;

	if (tickCount < 0.0f)
	{
		tickCount = 0.0f;
	}

	return float(tickCount) / _countsPerSecond;
}

