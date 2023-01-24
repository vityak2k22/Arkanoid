#include "Engine.h"

//C_s_Engine
//===================================================================================================
// Настройка игры при старте
void C_s_Engine::Init(HWND hWnd) {
	HWnd = hWnd;
	C_s_Config::Create_Pen_and_Brush(RGB(0, 0, 0), BG_Pen, BG_Brush);

	Ball.Init();
	Level.Init();
	Platform.Init();
	Platform.Redraw(hWnd);
	Border.Init();
	
	SetTimer(HWnd, TIMER_ID, 40, NULL);
}
//===================================================================================================
// Отрисовка экрана игры
void C_s_Engine::Draw_Frame(HDC hdc, RECT& paint_area) {
	Level.Draw(hdc, paint_area);
	Ball.Draw(hdc, paint_area, BG_Pen, BG_Brush);
	Platform.Draw(hdc, BG_Pen, BG_Brush, paint_area);
	Border.Draw(hdc, paint_area, BG_Pen, BG_Brush);

	/*for (int i = 0; i < 16; i++) {
		draw_brick_letter(hdc, 20 + i * Cell_Width * GLOBAL_SCALE, 100, EBT_Blue, ELT_O, i);
		draw_brick_letter(hdc, 20 + i * Cell_Width * GLOBAL_SCALE, 130, EBT_Red, ELT_O, i);
	}*/
}
//===================================================================================================
// Обработка нажатых клавиш
int C_s_Engine::On_Key_Down(E_Key_Type key_type) {
	switch (key_type) {
	case EKT_Left:
		Platform.X_Pos -= C_s_Config::Step;
		if (Platform.X_Pos <= C_s_Config::Border_X_Min)
			Platform.X_Pos = C_s_Config::Border_X_Min;
		Platform.Redraw(HWnd);
		break;


	case EKT_Right:
		Platform.X_Pos += C_s_Config::Step;
		if (Platform.X_Pos + Platform.Width >= C_s_Config::Max_X_Pos + 1)
			Platform.X_Pos = C_s_Config::Max_X_Pos - Platform.Width + 1;
		Platform.Redraw(HWnd);
		break;


	case EKT_Space:

		break;
	}
	return 0;
}
//===================================================================================================
// Обработка таймера
int C_s_Engine::On_Timer() {
	Ball.Move(HWnd, &Level, Platform.X_Pos, Platform.Width);
	return 0;
}
//===================================================================================================