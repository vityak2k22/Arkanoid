#include "Engine.h"
#define _USE_MATH_DEFINES
#include <cmath>

//===================================================================================================

const DWORD Level_X_Offset = 8;
const DWORD Level_Y_Offset = 6;

const DWORD Brick_Width = 15;
const DWORD Brick_Height = 7;

const DWORD Cell_Width = Brick_Width + 1;
const DWORD Cell_Height = Brick_Height + 1;

const DWORD Circle_Size = 7;
DWORD Inner_Width = 21;

HPEN Red_Pen;
HPEN Blue_Pen;
HBRUSH Red_Brush;
HBRUSH Blue_Brush;

enum E_Brick_Type {
	EBT_None,
	EBT_Red,
	EBT_Blue
};

const char Level_01[14][12] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

//===================================================================================================
// Присвоение цвета перу и кисти
void create_pen_and_brush(COLORREF rgb, HPEN& pen, HBRUSH& brush) {
	pen = CreatePen(PS_SOLID, 0, rgb);
	brush = CreateSolidBrush(rgb);
}
//===================================================================================================
// Настройка игры при старте
void Init() {
	create_pen_and_brush(RGB(255, 85, 255), Red_Pen, Red_Brush);
	create_pen_and_brush(RGB(85, 255, 255), Blue_Pen, Blue_Brush);
}
//===================================================================================================
// Отрисовка кирпича
void draw_brick(HDC hdc, int x, int y, E_Brick_Type brick_type) {
	HPEN pen;
	HBRUSH brush;

	switch (brick_type) {
	case EBT_None:
		return;


	case EBT_Red:
		pen = Red_Pen;
		brush = Red_Brush;
		break;


	case EBT_Blue:
		pen = Blue_Pen;
		brush = Blue_Brush;
		break;


	default:
		return;
	}

	SelectObject(hdc, pen);
	SelectObject(hdc, brush);

	RoundRect(hdc, x * GLOBAL_SCALE, y * GLOBAL_SCALE, (x + Brick_Width) * GLOBAL_SCALE,
		(y + Brick_Height) * GLOBAL_SCALE, 2 * GLOBAL_SCALE, 2 * GLOBAL_SCALE);
}
//===================================================================================================
// Отрисовка поворота кирпича
void draw_brick_letter(HDC hdc, DWORD rotation_step) {
	double rotation_angle = 2. * M_PI / 16. * (double)rotation_step;	// Преобразование шага поворота (rotation_step)
																		// в угол поворота (rotation_angle)
	XFORM xForm, old_xForm;
	SetGraphicsMode(hdc, GM_ADVANCED);

	xForm.eM11 = (FLOAT)cos(rotation_angle);
	xForm.eM12 = (FLOAT)sin(rotation_angle);
	xForm.eM21 = -(FLOAT)sin(rotation_angle);
	xForm.eM22 = (FLOAT)(rotation_angle);
	xForm.eDx = (FLOAT)100.0;
	xForm.eDy = (FLOAT)100.0;
	GetWorldTransform(hdc, &old_xForm);
	SetWorldTransform(hdc, &xForm);

	SelectObject(hdc, Blue_Pen);
	SelectObject(hdc, Blue_Brush);
	Rectangle(hdc, 0, 0, 15 * GLOBAL_SCALE, 7 * GLOBAL_SCALE);

	SetWorldTransform(hdc, &old_xForm);
}
//===================================================================================================
// Отрисовка уровня
void draw_level(HDC hdc) {
	int i, j;
	for (i = 0; i < 14; i++)
		for (j = 0; j < 12; j++)
			draw_brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height,
				(E_Brick_Type)Level_01[i][j]);
}
//===================================================================================================
// Отрисовка платформы
void draw_platform(HDC hdc, DWORD x, DWORD y) {
	
	// 1. Рисуем боковые шарики
	SelectObject(hdc, Red_Pen);
	SelectObject(hdc, Red_Brush);

	Ellipse(hdc, x * GLOBAL_SCALE, y * GLOBAL_SCALE, (x + Circle_Size) * GLOBAL_SCALE,
		(y + Circle_Size) * GLOBAL_SCALE);
	Ellipse(hdc, (x + Inner_Width) * GLOBAL_SCALE, y * GLOBAL_SCALE,
		(x + Inner_Width + Circle_Size) * GLOBAL_SCALE, (y + Circle_Size) * GLOBAL_SCALE);
	
	// 1. Рисуем центральную часть
	SelectObject(hdc, Blue_Pen);
	SelectObject(hdc, Blue_Brush);

	RoundRect(hdc, (x + 4) * GLOBAL_SCALE, (y + 1) * GLOBAL_SCALE,
		(x + 4 + Inner_Width - 1) * GLOBAL_SCALE, (y + 1 + 5) * GLOBAL_SCALE, 3 * GLOBAL_SCALE, 3 * GLOBAL_SCALE);
}
//===================================================================================================
// Отрисовка экрана игры
void Draw_Frame(HDC hdc) {
	/*draw_level(hdc);
	draw_platform(hdc, 100, 120);*/

	for (int i = 0; i < 16; i++)
		draw_brick_letter(hdc, i);
	
}