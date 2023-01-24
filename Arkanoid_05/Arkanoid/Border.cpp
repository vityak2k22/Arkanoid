#include "Border.h"

//===================================================================================================
C_s_Border::C_s_Border() {
	Blue_Pen = 0;
	White_Pen = 0;
	Blue_Brush = 0;
	White_Brush = 0;
}
//===================================================================================================
// Настройка рамки при старте
void C_s_Border::Init() {
	C_s_Config::Create_Pen_and_Brush(RGB(85, 255, 255), Blue_Pen, Blue_Brush);
	C_s_Config::Create_Pen_and_Brush(RGB(255, 255, 255), White_Pen, White_Brush);
}
//===================================================================================================
// Отрисовка рамки
void C_s_Border::Draw(HDC hdc, RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush) {
	// 1. Отрисовка рамки слева
	for (BYTE i = 0; i < 50; i++)
		draw_element(hdc, 2, 1 + i * 4, false, bg_pen, bg_brush);
	// 2. Отрисовка рамки справа
	for (BYTE i = 0; i < 50; i++)
		draw_element(hdc, 201, 1 + i * 4, false, bg_pen, bg_brush);
	// 3. Отрисовка рамки сверху
	for (BYTE i = 0; i < 50; i++)
		draw_element(hdc, 3 + i * 4, 0, true, bg_pen, bg_brush);
}
//===================================================================================================
// Отрисовка элемента рамки
void C_s_Border::draw_element(HDC hdc, WORD x, WORD y, bool is_top_border, HPEN bg_pen, HBRUSH bg_brush) {
	// Вывод основной части рамки
	SelectObject(hdc, Blue_Pen);
	SelectObject(hdc, Blue_Brush);

	if (is_top_border)
		Rectangle(hdc, x * GLOBAL_SCALE, (y + 1) * GLOBAL_SCALE, (x + 4) * GLOBAL_SCALE, (y + 4) * GLOBAL_SCALE);
	else
		Rectangle(hdc, (x + 1) * GLOBAL_SCALE, y * GLOBAL_SCALE, (x + 4) * GLOBAL_SCALE, (y + 4) * GLOBAL_SCALE);

	// Вывод белого отблиска рамки
	SelectObject(hdc, White_Pen);
	SelectObject(hdc, White_Brush);

	if (is_top_border)
		Rectangle(hdc, x * GLOBAL_SCALE, y * GLOBAL_SCALE, (x + 4) * GLOBAL_SCALE, (y + 1) * GLOBAL_SCALE);
	else
		Rectangle(hdc, x * GLOBAL_SCALE, y * GLOBAL_SCALE, (x + 1) * GLOBAL_SCALE, (y + 4) * GLOBAL_SCALE);

	// Вывод перфорации
	SelectObject(hdc, bg_pen);
	SelectObject(hdc, bg_brush);

	if (is_top_border)
		Rectangle(hdc, (x + 2) * GLOBAL_SCALE, (y + 2) * GLOBAL_SCALE, (x + 3) * GLOBAL_SCALE, (y + 3) * GLOBAL_SCALE);
	else
		Rectangle(hdc, (x + 2) * GLOBAL_SCALE, (y + 1) * GLOBAL_SCALE, (x + 3) * GLOBAL_SCALE, (y + 2) * GLOBAL_SCALE);
}
//===================================================================================================