#include "Engine.h"
#define _USE_MATH_DEFINES
#include <cmath>

//===================================================================================================

const BYTE Level_X_Offset = 8;
const BYTE Level_Y_Offset = 6;
const BYTE Level_Width = 12;	// Ширина уровня в ячейках
const BYTE Level_Height = 14;	// Высота уровня в ячейках
RECT Level_Rect;

const BYTE Brick_Width = 15;
const BYTE Brick_Height = 7;

const BYTE Cell_Width = Brick_Width + 1;
const BYTE Cell_Height = Brick_Height + 1;

const BYTE Ball_Size = 4;
double Ball_Speed = GLOBAL_SCALE, Ball_Direction = M_PI - M_PI_4;
int Ball_X_Pos = 20, Ball_Y_Pos = 170;
RECT Ball_Rect, Prev_Ball_Rect;

const BYTE Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width - Ball_Size;
const BYTE Max_Y_Pos = 200 - 1 - Ball_Size;

const BYTE Circle_Size = 7;
BYTE Inner_Width = 21;
int Platform_X_Pos = 0;
const int Platform_Y_Pos = 185;
BYTE Platform_Width = 28;
const BYTE Platform_Height = 7;
const BYTE Platform_Step = GLOBAL_SCALE;
RECT Platform_Rect, Prev_Platform_Rect;


HWND HWnd;
HPEN Red_Pen;
HPEN Blue_Pen;
HPEN BG_Pen;
HPEN White_Pen;
HBRUSH Red_Brush;
HBRUSH Blue_Brush;
HBRUSH BG_Brush;
HBRUSH White_Brush;

enum E_Brick_Type {
	EBT_None,
	EBT_Red,
	EBT_Blue
};

enum E_Letter_Type {
	ELT_None,
	ELT_O
};

const char Level_01[Level_Height][Level_Width] = {
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
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

//===================================================================================================
// Присвоение цвета перу и кисти
void create_pen_and_brush(COLORREF rgb, HPEN& pen, HBRUSH& brush) {
	pen = CreatePen(PS_SOLID, 0, rgb);
	brush = CreateSolidBrush(rgb);
}
//===================================================================================================
// Перерисовка платформы при её перемещении
void redraw_platform() {
	Prev_Platform_Rect = Platform_Rect;
	
	Platform_Rect.left = (Platform_X_Pos + Level_X_Offset) * GLOBAL_SCALE;
	Platform_Rect.top = Platform_Y_Pos * GLOBAL_SCALE;
	Platform_Rect.right = Platform_Rect.left + Platform_Width * GLOBAL_SCALE;
	Platform_Rect.bottom = Platform_Rect.top + Platform_Height * GLOBAL_SCALE;
	
	InvalidateRect(HWnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(HWnd, &Platform_Rect, FALSE);
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
// Инициализация фона и переднего плана кирпича при его падении
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
void draw_brick_letter(HDC hdc, WORD x, WORD y, E_Brick_Type brick_type, E_Letter_Type letter_type,
	WORD rotation_step) {
	if (brick_type != EBT_Blue && brick_type != EBT_Red)
		return;	// Падающие буквы могут быть только от кирпичей!!!
	
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
		XFORM xForm = {}, old_xForm;
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
				SelectObject(hdc, White_Pen);
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
	for (i = 0; i < Level_Height; i++)
		for (j = 0; j < Level_Width; j++)
			draw_brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height,
				(E_Brick_Type)Level_01[i][j]);
}
//===================================================================================================
// Отрисовка платформы
void draw_platform(HDC hdc, int x, int y) {
	// 0. Очистка фона за платформой
	SelectObject(hdc, BG_Pen);
	SelectObject(hdc, BG_Brush);
	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);
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
// Отрисовка шарика
void draw_ball(HDC hdc, int x, int y) {
	// 1. Очистка следа за шариком
	SelectObject(hdc, BG_Pen);
	SelectObject(hdc, BG_Brush);
	Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);
	
	// 2. Отрисовка шарика
	SelectObject(hdc, White_Pen);
	SelectObject(hdc, White_Brush);
	Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
}
//===================================================================================================
// Отрисовка экрана игры
void Draw_Frame(HDC hdc, RECT& paint_area) {
	RECT destination_rect;
	if (IntersectRect(&destination_rect, &paint_area, &Level_Rect))
		draw_level(hdc);
	if (IntersectRect(&destination_rect, &paint_area, &Platform_Rect))
		draw_platform(hdc, (Platform_X_Pos + Level_X_Offset), Platform_Y_Pos);

	/*for (int i = 0; i < 16; i++) {
		draw_brick_letter(hdc, 20 + i * Cell_Width * GLOBAL_SCALE, 100, EBT_Blue, ELT_O, i);
		draw_brick_letter(hdc, 20 + i * Cell_Width * GLOBAL_SCALE, 130, EBT_Red, ELT_O, i);
	}*/

	 if (IntersectRect(&destination_rect, &paint_area, &Ball_Rect))
		draw_ball(hdc, Ball_X_Pos, Ball_Y_Pos);
}
//===================================================================================================
// Настройка игры при старте
void Init_Engine(HWND hWnd) {
	HWnd = hWnd;
	create_pen_and_brush(RGB(255, 255, 255), White_Pen, White_Brush);
	create_pen_and_brush(RGB(85, 255, 255), Blue_Pen, Blue_Brush);
	create_pen_and_brush(RGB(255, 85, 255), Red_Pen, Red_Brush);
	create_pen_and_brush(RGB(0, 0, 0), BG_Pen, BG_Brush);

	Level_Rect.left = Level_X_Offset * GLOBAL_SCALE;
	Level_Rect.top = Level_Y_Offset * GLOBAL_SCALE;
	Level_Rect.right = Level_Rect.left + Cell_Width * Level_Width * GLOBAL_SCALE - GLOBAL_SCALE;
	Level_Rect.bottom = Level_Rect.top + Cell_Height * Level_Height * GLOBAL_SCALE - GLOBAL_SCALE;

	redraw_platform();
	SetTimer(HWnd, TIMER_ID, 40, NULL);
}
//===================================================================================================
// Обработка нажатых клавиш
int On_Key_Down(E_Key_Type key_type) {
	switch (key_type) {
	case EKT_Left:
		Platform_X_Pos -= Platform_Step;
		redraw_platform();
		break;


	case EKT_Right:
		Platform_X_Pos += Platform_Step;
		redraw_platform();
		break;


	case EKT_Space:
		
		break;
	}
	return 0;
}
//===================================================================================================
// Обработка таймера
void move_ball() {
	// Определение позиции отражения
	int next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction)),
		next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction));

	if (next_x_pos < Level_Y_Offset) {
		next_x_pos = Level_X_Offset - (next_x_pos - Level_X_Offset);
		Ball_Direction = M_PI - Ball_Direction;
	}
	if (next_y_pos < Level_Y_Offset) {
		next_y_pos = Level_Y_Offset - (next_y_pos - Level_Y_Offset);
		Ball_Direction = - Ball_Direction;
	}
	if (next_x_pos > Max_X_Pos) {
		next_x_pos = Max_X_Pos - (next_x_pos - Max_X_Pos);
		Ball_Direction = M_PI - Ball_Direction;
	}

	if (next_y_pos > Max_Y_Pos) {
		next_y_pos = Max_Y_Pos - (next_y_pos - Max_Y_Pos);
		Ball_Direction = M_PI + (M_PI - Ball_Direction);
	}

	Ball_X_Pos = next_x_pos;
	Ball_Y_Pos = next_y_pos;

	// Отображение смещения шарика
	Prev_Ball_Rect = Ball_Rect;

	Ball_Rect.left = Ball_X_Pos * GLOBAL_SCALE;
	Ball_Rect.top = Ball_Y_Pos * GLOBAL_SCALE;
	Ball_Rect.right = Ball_Rect.left + Ball_Size * GLOBAL_SCALE;
	Ball_Rect.bottom = Ball_Rect.top + Ball_Size * GLOBAL_SCALE;

	InvalidateRect(HWnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(HWnd, &Ball_Rect, FALSE);
}
//===================================================================================================
// Обработка таймера
int On_Timer() {
	move_ball();
	return 0;
}