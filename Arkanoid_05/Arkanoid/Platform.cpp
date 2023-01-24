#include "Platform.h"

//===================================================================================================
C_s_Platform::C_s_Platform() {
	X_Pos = C_s_Config::Border_X_Min;
	Inner_Width = 21;
	Width = 28;
	Rect = {};
	Prev_Rect = {};
	Red_Pen = 0;
	Blue_Pen = 0;
	Red_Brush = 0;
	Blue_Brush = 0;
}
//===================================================================================================
// Настройка платформы при старте
void C_s_Platform::Init() {
	C_s_Config::Create_Pen_and_Brush(RGB(85, 255, 255), Blue_Pen, Blue_Brush);
	C_s_Config::Create_Pen_and_Brush(RGB(255, 85, 255), Red_Pen, Red_Brush);
}
//===================================================================================================
// Отрисовка платформы
void C_s_Platform::Draw(HDC hdc, HPEN bg_pen, HBRUSH bg_brush, RECT& paint_area) {
	RECT destination_rect;
	if (!IntersectRect(&destination_rect, &paint_area, &Rect))
		return;

	auto x = X_Pos;
	auto y = C_s_Config::Y_Pos;

	// 0. Очистка фона за платформой
	SelectObject(hdc, bg_pen);
	SelectObject(hdc, bg_brush);
	Rectangle(hdc, Prev_Rect.left, Prev_Rect.top, Prev_Rect.right, Prev_Rect.bottom);
	// 1. Рисуем боковые шарики
	SelectObject(hdc, Red_Pen);
	SelectObject(hdc, Red_Brush);

	Ellipse(hdc, x * GLOBAL_SCALE, y * GLOBAL_SCALE, (x + Circle_Size) * GLOBAL_SCALE,
		(y + Circle_Size) * GLOBAL_SCALE);
	Ellipse(hdc, (x + Inner_Width) * GLOBAL_SCALE, y * GLOBAL_SCALE,
		(x + Inner_Width + Circle_Size) * GLOBAL_SCALE, (y + Circle_Size) * GLOBAL_SCALE);

	// 2. Рисуем центральную часть
	SelectObject(hdc, Blue_Pen);
	SelectObject(hdc, Blue_Brush);

	RoundRect(hdc, (x + 4) * GLOBAL_SCALE, (y + 1) * GLOBAL_SCALE,
		(x + 4 + Inner_Width - 1) * GLOBAL_SCALE, (y + 1 + 5) * GLOBAL_SCALE, 3 * GLOBAL_SCALE, 3 * GLOBAL_SCALE);
}
//===================================================================================================
// Перерисовка платформы при её перемещении
void C_s_Platform::Redraw(HWND hwnd) {
	Prev_Rect = Rect;

	Rect.left = X_Pos * GLOBAL_SCALE;
	Rect.top = C_s_Config::Y_Pos * GLOBAL_SCALE;
	Rect.right = Rect.left + Width * GLOBAL_SCALE;
	Rect.bottom = Rect.top + Height * GLOBAL_SCALE;

	InvalidateRect(hwnd, &Prev_Rect, FALSE);
	InvalidateRect(hwnd, &Rect, FALSE);
}
//===================================================================================================
