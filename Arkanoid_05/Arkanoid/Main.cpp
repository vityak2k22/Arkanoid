#include "Main.h"

C_s_Engine Engine;

//===================================================================================================
int WINAPI wWinMain(_In_ HINSTANCE hInst,
	_In_opt_ HINSTANCE hPrevInst,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd) {

	WNDCLASSW wcl = {};
	wcl.hInstance = hInst;
	wcl.lpfnWndProc = WndProc;
	wcl.lpszClassName = L"Main Class";
	wcl.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wcl.style = CS_HREDRAW | CS_VREDRAW;


	if (RegisterClassW(&wcl) == false)
		return 0;

	RECT window_rect;
	window_rect.left = 0;
	window_rect.top = 0;
	window_rect.right = 320 * GLOBAL_SCALE;
	window_rect.bottom = 200 * GLOBAL_SCALE;
	
	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hWnd = CreateWindowW(wcl.lpszClassName, L"Arkanoid", WS_OVERLAPPEDWINDOW, 0, 0,
		window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
		nullptr, nullptr, hInst, nullptr);

	//------------------------------------------
	Engine.Init(hWnd);
	//------------------------------------------

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessageW(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return 0;
}
//===================================================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		//------------------------------------------
		Engine.Draw_Frame(hdc, ps.rcPaint);
		//------------------------------------------
		EndPaint(hWnd, &ps);
	}
	break;


	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			return Engine.On_Key_Down(EKT_Left);


		case VK_RIGHT:
			return Engine.On_Key_Down(EKT_Right);


		case VK_SPACE:
			return Engine.On_Key_Down(EKT_Space);
		}
		break;

	case WM_TIMER:
		if (wParam == TIMER_ID)
			return Engine.On_Timer();
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	default:
		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}

	return 0;
}
//===================================================================================================