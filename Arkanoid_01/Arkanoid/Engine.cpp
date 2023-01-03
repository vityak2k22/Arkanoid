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
HPEN Letter_Pen;
HBRUSH Red_Brush;
HBRUSH Blue_Brush;

enum E_Brick_Type {
	EBT_None,
	EBT_Red,
	EBT_Blue
};

enum E_Letter_Type {
	ELT_None,
	ELT_O
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
	Letter_Pen = CreatePen(PS_SOLID, GLOBAL_SCALE - 1, RGB(255, 255, 255));
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
void set_brick_letter_colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush,
	HPEN &back_pen, HBRUSH &back_brush) {
	if (is_switch_color) {
		front_pen = Blue_Pen;
		front_brush = Blue_Brush;
		back_pen = Red_Pen;
		back_brush = Red_Brush;
	}
	else {
		front_pen = Red_Pen;
		front_brush = Red_Brush;
		back_pen = Blue_Pen;
		back_brush = Blue_Brush;
	}
}
//===================================================================================================
// Отрисовка поворота кирпича
void draw_brick_letter(HDC hdc, DWORD x, DWORD y, E_Brick_Type brick_type, E_Letter_Type letter_type,
	DWORD rotation_step) {
	if (brick_type != EBT_Blue && brick_type != EBT_Red)
		return;	// Падающие буквы могут быть от кирпичей такого типа!!!
	
	bool switch_color;
	HPEN front_pen, back_pen;
	HBRUSH front_brush, back_brush;

	if (rotation_step > 4 && rotation_step <= 12)
		switch_color = brick_type == EBT_Red;
	else
		switch_color = brick_type == EBT_Blue;
	set_brick_letter_colors(switch_color, front_pen, front_brush, back_pen, back_brush);

	// Корректировка шара поворота
	rotation_step %= 16;
	double rotation_angle;
	// Преобразование шага поворота (rotation_step) в угол поворота (rotation_angle)
	if (rotation_step < 8)
		rotation_angle = 2. * M_PI / 16. * (double)rotation_step;
	else
		rotation_angle = 2. * M_PI / 16. * (double)(8 - rotation_step);


	int brick_half_height = Brick_Height * GLOBAL_SCALE / 2;

	if (rotation_step == 4 || rotation_step == 12) {
		// Вывод фона
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		Rectangle(hdc, x, y + brick_half_height - GLOBAL_SCALE,
			x + Brick_Width * GLOBAL_SCALE, y + brick_half_height);

		// Вывод переднего плана
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, x, y + brick_half_height,
			x + Brick_Width * GLOBAL_SCALE,	y + brick_half_height + GLOBAL_SCALE - 1);
	}
	else {
		// настройка матрицы "переворота" буквы
		XFORM xForm, old_xForm;
		SetGraphicsMode(hdc, GM_ADVANCED);

		xForm.eM11 = 1.f;
		xForm.eM12 = 0.f;
		xForm.eM21 = 0.f;
		xForm.eM22 = (FLOAT)cos(rotation_angle);
		xForm.eDx = (FLOAT)x;
		xForm.eDy = (FLOAT)y + (FLOAT)brick_half_height;
		GetWorldTransform(hdc, &old_xForm);
		SetWorldTransform(hdc, &xForm);

		// вывод фона
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);
		
		double offset = (1. - fabs(xForm.eM22)) * (double)(GLOBAL_SCALE * GLOBAL_SCALE);
		int back_part_offset = (int)round(offset);
		Rectangle(hdc, 0, -brick_half_height - back_part_offset, Brick_Width * GLOBAL_SCALE,
			brick_half_height - back_part_offset);

		// вывод переднего плана
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);
		Rectangle(hdc, 0, -brick_half_height, Brick_Width * GLOBAL_SCALE, brick_half_height);

		if (rotation_step > 4 && rotation_step <= 12) {
			switch (letter_type) {
			case ELT_O:
				SelectObject(hdc, Letter_Pen);
				Ellipse(hdc, 0 + 5 * GLOBAL_SCALE, -5 * GLOBAL_SCALE / 2,
					0 + 10 * GLOBAL_SCALE, 5 * GLOBAL_SCALE / 2);
				break;


			default:
				return;
			}
		}
		
		SetWorldTransform(hdc, &old_xForm);
	}
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

	for (int i = 0; i < 16; i++) {
		draw_brick_letter(hdc, 20 + i * Cell_Width * GLOBAL_SCALE, 100, EBT_Blue, ELT_O, i);
		draw_brick_letter(hdc, 20 + i * Cell_Width * GLOBAL_SCALE, 130, EBT_Red, ELT_O, i);
	}
}