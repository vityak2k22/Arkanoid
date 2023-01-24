#include "Ball.h"

// C_Ball
//===================================================================================================
C_Ball::C_Ball() {
	Speed = GLOBAL_SCALE;
	Direction = M_PI - M_PI_4;
	X_Pos = 20;
	Y_Pos = 170;
	Rect = {};
	Prev_Rect = {};
	White_Pen = 0;
	White_Brush = 0;
}
//===================================================================================================
// Настройка шарика при старте
void C_Ball::Init() {
	C_s_Config::Create_Pen_and_Brush(RGB(255, 255, 255), White_Pen, White_Brush);
}
//===================================================================================================
// Отрисовка шарика
void C_Ball::Draw(HDC hdc, RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush) {
	RECT destination_rect;
	if (!IntersectRect(&destination_rect, &paint_area, &Rect))
		return;

	// 1. Очистка следа за шариком
	SelectObject(hdc, bg_pen);
	SelectObject(hdc, bg_brush);
	Ellipse(hdc, Prev_Rect.left, Prev_Rect.top, Prev_Rect.right - 1, Prev_Rect.bottom - 1);

	// 2. Отрисовка шарика
	SelectObject(hdc, White_Pen);
	SelectObject(hdc, White_Brush);
	Ellipse(hdc, Rect.left, Rect.top, Rect.right - 1, Rect.bottom - 1);
}
//===================================================================================================
// Обработка движения шарика
void C_Ball::Move(HWND hWnd, C_Level* level, short platform_x_pos, BYTE platform_width) {
	int next_x_pos = X_Pos + (int)(Speed * cos(Direction)),
		next_y_pos = Y_Pos - (int)(Speed * sin(Direction));

	// Определение позиции отражения от рамки
	if (next_x_pos < C_s_Config::Border_X_Min) {
		next_x_pos = C_s_Config::Border_X_Min;
		Direction = M_PI - Direction;
	}
	if (next_y_pos < C_s_Config::Border_Y_Max) {
		next_y_pos = C_s_Config::Border_Y_Max;
		Direction = -Direction;
	}
	if (next_x_pos + C_s_Config::Size > C_s_Config::Max_X_Pos) {
		next_x_pos = C_s_Config::Max_X_Pos - C_s_Config::Size;
		Direction = M_PI - Direction;
	}

	if (next_y_pos > C_s_Config::Max_Y_Pos) {
		next_y_pos = C_s_Config::Max_Y_Pos - (next_y_pos - C_s_Config::Max_Y_Pos);
		Direction = M_PI + (M_PI - Direction);
	}

	// Определение отражения от платформы
	if (next_y_pos + C_s_Config::Size >= C_s_Config::Y_Pos &&
		(next_x_pos >= platform_x_pos &&
			next_x_pos <= platform_x_pos + platform_width)
		) {
		next_y_pos = C_s_Config::Y_Pos - C_s_Config::Size;
		Direction = M_PI + (M_PI - Direction);
	}

	// Определение отражения от кирпичей
	level->Check_Brick_Hit(next_y_pos, Direction);

	X_Pos = next_x_pos;
	Y_Pos = next_y_pos;

	// Отображение смещения шарика
	Prev_Rect = Rect;

	Rect.left = X_Pos * GLOBAL_SCALE;
	Rect.top = Y_Pos * GLOBAL_SCALE;
	Rect.right = Rect.left + C_s_Config::Size * GLOBAL_SCALE;
	Rect.bottom = Rect.top + C_s_Config::Size * GLOBAL_SCALE;

	InvalidateRect(hWnd, &Prev_Rect, FALSE);
	InvalidateRect(hWnd, &Rect, FALSE);
}
//===================================================================================================
