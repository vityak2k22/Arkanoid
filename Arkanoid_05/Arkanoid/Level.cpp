#include "Level.h"

//===================================================================================================
const char Level_01[C_s_Config::Height][C_s_Config::Width] = {
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
C_Level::C_Level() {
	Rect = {};
	Red_Pen = 0;
	Blue_Pen = 0;
	White_Pen = 0;
	Red_Brush = 0;
	Blue_Brush = 0;
}
//===================================================================================================
// Настройка уровня при старте
void C_Level::Init() {
	C_s_Config::Create_Pen_and_Brush(RGB(85, 255, 255), Blue_Pen, Blue_Brush);
	C_s_Config::Create_Pen_and_Brush(RGB(255, 85, 255), Red_Pen, Red_Brush);

	White_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

	Rect.left = C_s_Config::X_Offset * GLOBAL_SCALE;
	Rect.top = C_s_Config::Y_Offset * GLOBAL_SCALE;
	Rect.right = Rect.left + C_s_Config::Cell_Width * C_s_Config::Width * GLOBAL_SCALE - GLOBAL_SCALE;
	Rect.bottom = Rect.top + C_s_Config::Cell_Height * C_s_Config::Height * GLOBAL_SCALE - GLOBAL_SCALE;
}
//===================================================================================================
// Определение отражения от кирпичей
void C_Level::Check_Brick_Hit(int& next_y_pos, double& ball_direction) {
	WORD brick_y_pos = C_s_Config::Y_Offset + C_s_Config::Height * C_s_Config::Cell_Height;
	for (int i = C_s_Config::Height - 1; i >= 0; i--) {
		for (int j = 0; j < C_s_Config::Width; j++) {
			if (Level_01[i][j] == 0)
				continue;
			if (next_y_pos <= brick_y_pos) {
				next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
				ball_direction = -ball_direction;
			}
		}
		brick_y_pos -= C_s_Config::Cell_Height;
	}
}
//===================================================================================================
// Отрисовка уровня
void C_Level::Draw(HDC hdc, RECT& paint_area) {
	RECT destination_rect;

	if (!IntersectRect(&destination_rect, &paint_area, &Rect))
		return;

	int i, j;
	for (i = 0; i < C_s_Config::Height; i++)
		for (j = 0; j < C_s_Config::Width; j++)
			draw_brick(hdc, C_s_Config::X_Offset + j * C_s_Config::Cell_Width, C_s_Config::Y_Offset + i * C_s_Config::Cell_Height,
				(E_Brick_Type)Level_01[i][j]);
}
//===================================================================================================
// Отрисовка кирпича
void C_Level::draw_brick(HDC hdc, int x, int y, E_Brick_Type brick_type) {
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

	RoundRect(hdc, x * GLOBAL_SCALE, y * GLOBAL_SCALE, (x + C_s_Config::Brick_Width) * GLOBAL_SCALE,
		(y + C_s_Config::Brick_Height) * GLOBAL_SCALE, 2 * GLOBAL_SCALE, 2 * GLOBAL_SCALE);
}
//===================================================================================================
// Инициализация фона и переднего плана кирпича при его падении
void C_Level::set_brick_letter_colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush,
	HPEN& back_pen, HBRUSH& back_brush) {
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
void C_Level::draw_brick_letter(HDC hdc, WORD x, WORD y, E_Brick_Type brick_type, E_Letter_Type letter_type,
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


	int brick_half_height = C_s_Config::Brick_Height * GLOBAL_SCALE / 2;

	if (rotation_step == 4 || rotation_step == 12) {
		// Вывод фона
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		Rectangle(hdc, x, y + brick_half_height - GLOBAL_SCALE,
			x + C_s_Config::Brick_Width * GLOBAL_SCALE, y + brick_half_height);

		// Вывод переднего плана
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, x, y + brick_half_height,
			x + C_s_Config::Brick_Width * GLOBAL_SCALE, y + brick_half_height + GLOBAL_SCALE - 1);
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
		Rectangle(hdc, 0, -brick_half_height - back_part_offset, C_s_Config::Brick_Width * GLOBAL_SCALE,
			brick_half_height - back_part_offset);

		// вывод переднего плана
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);
		Rectangle(hdc, 0, -brick_half_height, C_s_Config::Brick_Width * GLOBAL_SCALE, brick_half_height);

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
