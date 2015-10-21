#pragma once

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include <windows.h>
#include "Camera.h"
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "resource.h"
#include <dinput.h>
#include "Structures.h"

using namespace DirectX;

class Application
{
private:
	UINT					_WindowHeight;
	UINT					_WindowWidth;
	HINSTANCE               _hInst;
	HWND                    _hWnd;
	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;
	ID3D11Device*           _pd3dDevice;
	ID3D11DeviceContext*    _pImmediateContext;
	IDXGISwapChain*         _pSwapChain;
	ID3D11RenderTargetView* _pRenderTargetView;
	ID3D11VertexShader*     _pVertexShader;
	ID3D11PixelShader*      _pPixelShader;
	ID3D11InputLayout*      _pVertexLayout;
	ID3D11Buffer*           _pVertexBuffer;
	ID3D11Buffer*           _pIndexBuffer;
	ID3D11Buffer*           _pConstantBuffer;
	XMFLOAT4X4              _cube1;
	XMFLOAT4X4				_cube2;

	//depth buffer
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11Texture2D*		_depthStencilBuffer;

	//render states
	ID3D11RasterizerState*	_wireFrame;
	bool					_isWF;

	//camera + view
	XMMATRIX				_wvp;
	Camera					_cam;
	XMFLOAT4X4              _view;
	XMFLOAT4X4              _projection;

	//input
	IDirectInputDevice8*	_DIKeyboard;
	IDirectInputDevice8*	_DIMouse;
	DIMOUSESTATE			_mouseLastState;
	LPDIRECTINPUT8			_DirectInput;

	//lighting
	Light					_light;

	// Compute Colour


private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDirectX();
	HRESULT	InitInput(HINSTANCE hInstance);
	HRESULT InitCamera(HINSTANCE hInstance);
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitShadersAndInputLayout();
	HRESULT InitVertexBuffer();
	HRESULT InitIndexBuffer();

public:
	Application();
	~Application();
	void	Cleanup();
	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void DetectInput(double time);
	void Update(double time);
	void Draw();

	//timing
	void	StartTimer();
	double	GetTime();
	double	GetFrameTime();

	__int64					_frameTimeOld;
	double					_frameTime;
	int						_frameCount;
	int						_fps;
	double					_countsPerSecond;
	__int64					_counterStart;
};

