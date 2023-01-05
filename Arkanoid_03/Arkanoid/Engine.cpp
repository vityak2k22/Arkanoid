#include "Engine.h"



const char Level_01[C_s_Engine::Level_Height][C_s_Engine::Level_Width] = {
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
C_s_Engine::C_s_Engine(): Ball_Speed(GLOBAL_SCALE), Ball_Direction(M_PI - M_PI_4), Ball_X_Pos(20), Ball_Y_Pos(170),
		Platform_X_Pos(Border_X_Min), Inner_Width(21), Platform_Width(28)
{}
//===================================================================================================
// Настройка игры при старте
void C_s_Engine::Init_Engine(HWND hWnd) {
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
// Отрисовка экрана игры
void C_s_Engine::Draw_Frame(HDC hdc, RECT& paint_area) {
	RECT destination_rect;

	if (IntersectRect(&destination_rect, &paint_area, &Level_Rect))
		draw_level(hdc);

	if (IntersectRect(&destination_rect, &paint_area, &Platform_Rect))
		draw_platform(hdc, Platform_X_Pos, Platform_Y_Pos);

	/*for (int i = 0; i < 16; i++) {
		draw_brick_letter(hdc, 20 + i * Cell_Width * GLOBAL_SCALE, 100, EBT_Blue, ELT_O, i);
		draw_brick_letter(hdc, 20 + i * Cell_Width * GLOBAL_SCALE, 130, EBT_Red, ELT_O, i);
	}*/

	if (IntersectRect(&destination_rect, &paint_area, &Ball_Rect))
		draw_ball(hdc, Ball_X_Pos, Ball_Y_Pos);

	draw_bounds(hdc, paint_area);
}
//===================================================================================================
// Обработка нажатых клавиш
int C_s_Engine::On_Key_Down(E_Key_Type key_type) {
	switch (key_type) {
	case EKT_Left:
		Platform_X_Pos -= Platform_Step;
		if (Platform_X_Pos <= Border_X_Min)
			Platform_X_Pos = Border_X_Min;
		redraw_platform();
		break;


	case EKT_Right:
		Platform_X_Pos += Platform_Step;
		if (Platform_X_Pos + Platform_Width >= Max_X_Pos + 1)
			Platform_X_Pos = Max_X_Pos - Platform_Width + 1;
		redraw_platform();
		break;


	case EKT_Space:

		break;
	}
	return 0;
}
//===================================================================================================
// Обработка таймера
int C_s_Engine::On_Timer() {
	move_ball();
	return 0;
}
//===================================================================================================
// Присвоение цвета перу и кисти
void C_s_Engine::create_pen_and_brush(COLORREF rgb, HPEN& pen, HBRUSH& brush) {
	pen = CreatePen(PS_SOLID, 0, rgb);
	brush = CreateSolidBrush(rgb);
}
//===================================================================================================
// Перерисовка платформы при её перемещении
void C_s_Engine::redraw_platform() {
	Prev_Platform_Rect = Platform_Rect;
	
	Platform_Rect.left = Platform_X_Pos * GLOBAL_SCALE;
	Platform_Rect.top = Platform_Y_Pos * GLOBAL_SCALE;
	Platform_Rect.right = Platform_Rect.left + Platform_Width * GLOBAL_SCALE;
	Platform_Rect.bottom = Platform_Rect.top + Platform_Height * GLOBAL_SCALE;
	
	InvalidateRect(HWnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(HWnd, &Platform_Rect, FALSE);
}
//===================================================================================================
// Отрисовка кирпича
void C_s_Engine::draw_brick(HDC hdc, int x, int y, E_Brick_Type brick_type) {
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
void C_s_Engine::set_brick_letter_colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush,
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
void C_s_Engine::draw_brick_letter(HDC hdc, WORD x, WORD y, E_Brick_Type brick_type, E_Letter_Type letter_type,
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
void C_s_Engine::draw_level(HDC hdc) {
	int i, j;
	for (i = 0; i < Level_Height; i++)
		for (j = 0; j < Level_Width; j++)
			draw_brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height,
				(E_Brick_Type)Level_01[i][j]);
}
//===================================================================================================
// Отрисовка платформы
void C_s_Engine::draw_platform(HDC hdc, int x, int y) {
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
void C_s_Engine::draw_ball(HDC hdc, int x, int y) {
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
// Отрисовка элемента рамки
void C_s_Engine::draw_border(HDC hdc, WORD x, WORD y, bool is_top_border) {
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
	SelectObject(hdc, BG_Pen);
	SelectObject(hdc, BG_Brush);
	
	if (is_top_border)
		Rectangle(hdc, (x + 2) * GLOBAL_SCALE, (y + 2) * GLOBAL_SCALE, (x + 3) * GLOBAL_SCALE, (y + 3) * GLOBAL_SCALE);
	else
		Rectangle(hdc, (x + 2) * GLOBAL_SCALE, (y + 1) * GLOBAL_SCALE, (x + 3) * GLOBAL_SCALE, (y + 2) * GLOBAL_SCALE);
}
//===================================================================================================
// Отрисовка рамки
void C_s_Engine::draw_bounds(HDC hdc, RECT &paint_area) {
	// 1. Отрисовка рамки слева
	for (BYTE i = 0; i < 50; i++)
		draw_border(hdc, 2, 1 + i * 4, false);
	// 2. Отрисовка рамки справа
	for (BYTE i = 0; i < 50; i++)
		draw_border(hdc, 201, 1 + i * 4, false);
	// 3. Отрисовка рамки сверху
	for (BYTE i = 0; i < 50; i++)
		draw_border(hdc, 3 + i * 4, 0, true);
}
//===================================================================================================
// Определение отражения от кирпичей
void C_s_Engine::check_brick_hit(int &next_y_pos) {
	WORD brick_y_pos = Level_Y_Offset + Level_Height * Cell_Height;
	for (int i = Level_Height - 1; i >= 0; i--) {
		for (int j = 0; j < Level_Width; j++) {
			if (Level_01[i][j] == 0)
				continue;
			if (next_y_pos <= brick_y_pos){
				next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
				Ball_Direction = -Ball_Direction;
			}
		}
		brick_y_pos -= Cell_Height;
	}
}
//===================================================================================================
// Обработка движения шарика
void C_s_Engine::move_ball() {
		int next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction)),
		next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction));
	
	// Определение позиции отражения от рамки
	if (next_x_pos < Border_X_Min) {
		next_x_pos = Border_X_Min;
		Ball_Direction = M_PI - Ball_Direction;
	}
	if (next_y_pos < Border_Y_Max) {
		next_y_pos = Border_Y_Max;
		Ball_Direction = -Ball_Direction;
	}
	if (next_x_pos + Ball_Size > Max_X_Pos) {
		next_x_pos = Max_X_Pos - Ball_Size;
		Ball_Direction = M_PI - Ball_Direction;
	}

	if (next_y_pos > Max_Y_Pos) {
		next_y_pos = Max_Y_Pos - (next_y_pos - Max_Y_Pos);
		Ball_Direction = M_PI + (M_PI - Ball_Direction);
	}

	// Определение отражения от платформы
	if (next_y_pos + Ball_Size >= Platform_Y_Pos &&
			(next_x_pos >= Platform_X_Pos &&
			next_x_pos <= Platform_X_Pos + Platform_Width)
		) {
		next_y_pos = Platform_Y_Pos - Ball_Size;
		Ball_Direction = M_PI + (M_PI - Ball_Direction);
	}

	// Определение отражения от кирпичей
	check_brick_hit(next_y_pos);

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