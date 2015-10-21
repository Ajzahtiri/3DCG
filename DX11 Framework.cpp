#include "Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	Application * theApp = new Application();

	if (FAILED(theApp->Initialise(hInstance, nCmdShow)))
	{
		return -1;
	}

    // Main message loop
    MSG msg = {0};

    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		else
        {
			theApp->_frameCount++;

			if (theApp->GetTime() > 1.0f)
			{
				theApp->_fps = theApp->_frameCount;
				theApp->_frameCount = 0;
				theApp->StartTimer();
			}

			theApp->_frameTime = theApp->GetFrameTime();
			
			theApp->DetectInput(theApp->_frameTime);
			theApp->Update(theApp->_frameTime);
            theApp->Draw();
        }
    }

	delete theApp;
	theApp = nullptr;

    return (int) msg.wParam;
}