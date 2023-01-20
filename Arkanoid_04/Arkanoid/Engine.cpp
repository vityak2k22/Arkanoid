#include "Engine.h"



const char Level_01[C_Level::Height][C_Level::Width] = {
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




// C_Level
//===================================================================================================
// Настройка уровня при старте
void C_Level::Init() {
	C_s_Engine::create_pen_and_brush(RGB(85, 255, 255), Blue_Pen, Blue_Brush);
	C_s_Engine::create_pen_and_brush(RGB(255, 85, 255), Red_Pen, Red_Brush);

	White_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

	Rect.left = C_Level::X_Offset * GLOBAL_SCALE;
	Rect.top = C_Level::Y_Offset * GLOBAL_SCALE;
	Rect.right = Rect.left + C_Level::Cell_Width * C_Level::Width * GLOBAL_SCALE - GLOBAL_SCALE;
	Rect.bottom = Rect.top + C_Level::Cell_Height * C_Level::Height * GLOBAL_SCALE - GLOBAL_SCALE;
}
//===================================================================================================
// Определение отражения от кирпичей
void C_Level::Check_Brick_Hit(int& next_y_pos, double& ball_direction) {
	WORD brick_y_pos = Y_Offset + Height * Cell_Height;
	for (int i = Height - 1; i >= 0; i--) {
		for (int j = 0; j < Width; j++) {
			if (Level_01[i][j] == 0)
				continue;
			if (next_y_pos <= brick_y_pos) {
				next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
				ball_direction = -ball_direction;
			}
		}
		brick_y_pos -= Cell_Height;
	}
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

	RoundRect(hdc, x * GLOBAL_SCALE, y * GLOBAL_SCALE, (x + Brick_Width) * GLOBAL_SCALE,
		(y + Brick_Height) * GLOBAL_SCALE, 2 * GLOBAL_SCALE, 2 * GLOBAL_SCALE);
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
			x + Brick_Width * GLOBAL_SCALE, y + brick_half_height + GLOBAL_SCALE - 1);
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
void C_Level::Draw(HDC hdc, RECT& paint_area, C_s_Engine* engine) {
	RECT destination_rect;

	if (!IntersectRect(&destination_rect, &paint_area, &Rect))
		return;

	int i, j;
	for (i = 0; i < Height; i++)
		for (j = 0; j < Width; j++)
			draw_brick(hdc, X_Offset + j * Cell_Width, Y_Offset + i * Cell_Height,
				(E_Brick_Type)Level_01[i][j]);
}
//===================================================================================================




// C_Ball
//===================================================================================================
C_Ball::C_Ball() {
	Speed = GLOBAL_SCALE;
	Direction = M_PI - M_PI_4;
	X_Pos = 20;
	Y_Pos = 170;
}
//===================================================================================================
// Настройка шарика при старте
void C_Ball::Init() {
	C_s_Engine::create_pen_and_brush(RGB(255, 255, 255), White_Pen, White_Brush);
}
//===================================================================================================
// Отрисовка шарика
void C_Ball::Draw(HDC hdc, RECT& paint_area, C_s_Engine* engine) {
	RECT destination_rect;
	if (!IntersectRect(&destination_rect, &paint_area, &Rect))
		return;

	// 1. Очистка следа за шариком
	SelectObject(hdc, engine->BG_Pen);
	SelectObject(hdc, engine->BG_Brush);
	Ellipse(hdc, Prev_Rect.left, Prev_Rect.top, Prev_Rect.right - 1, Prev_Rect.bottom - 1);

	// 2. Отрисовка шарика
	SelectObject(hdc, White_Pen);
	SelectObject(hdc, White_Brush);
	Ellipse(hdc, Rect.left, Rect.top, Rect.right - 1, Rect.bottom - 1);
}
//===================================================================================================
// Обработка движения шарика
void C_Ball::Move(C_s_Engine* engine, C_Level* level, C_s_Platform* platform) {
	int next_x_pos = X_Pos + (int)(Speed * cos(Direction)),
		next_y_pos = Y_Pos - (int)(Speed * sin(Direction));

	// Определение позиции отражения от рамки
	if (next_x_pos < C_s_Border::Border_X_Min) {
		next_x_pos = C_s_Border::Border_X_Min;
		Direction = M_PI - Direction;
	}
	if (next_y_pos < C_s_Border::Border_Y_Max) {
		next_y_pos = C_s_Border::Border_Y_Max;
		Direction = -Direction;
	}
	if (next_x_pos + Size > C_s_Border::Max_X_Pos) {
		next_x_pos = C_s_Border::Max_X_Pos - Size;
		Direction = M_PI - Direction;
	}

	if (next_y_pos > C_s_Border::Max_Y_Pos) {
		next_y_pos = C_s_Border::Max_Y_Pos - (next_y_pos - C_s_Border::Max_Y_Pos);
		Direction = M_PI + (M_PI - Direction);
	}

	// Определение отражения от платформы
	if (next_y_pos + Size >= C_s_Platform::Y_Pos &&
		(next_x_pos >= platform->X_Pos &&
			next_x_pos <= platform->X_Pos + platform->Width)
		) {
		next_y_pos = C_s_Platform::Y_Pos - Size;
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
	Rect.right = Rect.left + Size * GLOBAL_SCALE;
	Rect.bottom = Rect.top + Size * GLOBAL_SCALE;

	InvalidateRect(engine->HWnd, &Prev_Rect, FALSE);
	InvalidateRect(engine->HWnd, &Rect, FALSE);
}
//===================================================================================================




//C_s_Platform
//===================================================================================================
C_s_Platform::C_s_Platform() {
	X_Pos = C_s_Border::Border_X_Min;
	Inner_Width = 21;
	Width = 28;
}
//===================================================================================================
// Настройка платформы при старте
void C_s_Platform::Init() {
	C_s_Engine::create_pen_and_brush(RGB(85, 255, 255), Blue_Pen, Blue_Brush);
	C_s_Engine::create_pen_and_brush(RGB(255, 85, 255), Red_Pen, Red_Brush);
}
//===================================================================================================
// Отрисовка платформы
void C_s_Platform::Draw(HDC hdc, C_s_Engine* engine, RECT& paint_area) {
	RECT destination_rect;
	if (!IntersectRect(&destination_rect, &paint_area, &Rect))
		return;
	
	auto x = X_Pos;
	auto y = Y_Pos;

	// 0. Очистка фона за платформой
	SelectObject(hdc, engine->BG_Pen);
	SelectObject(hdc, engine->BG_Brush);
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
void C_s_Platform::Redraw(C_s_Engine* engine) {
	Prev_Rect = Rect;

	Rect.left = X_Pos * GLOBAL_SCALE;
	Rect.top = Y_Pos * GLOBAL_SCALE;
	Rect.right = Rect.left + Width * GLOBAL_SCALE;
	Rect.bottom = Rect.top + Height * GLOBAL_SCALE;

	InvalidateRect(engine->HWnd, &Prev_Rect, FALSE);
	InvalidateRect(engine->HWnd, &Rect, FALSE);
}
//===================================================================================================




//C_s_Border
//===================================================================================================
// Настройка рамки при старте
void C_s_Border::Init() {
	C_s_Engine::create_pen_and_brush(RGB(85, 255, 255), Blue_Pen, Blue_Brush);
	C_s_Engine::create_pen_and_brush(RGB(255, 255, 255), White_Pen, White_Brush);
}
//===================================================================================================
// Отрисовка элемента рамки
void C_s_Border::draw_element(HDC hdc, WORD x, WORD y, bool is_top_border, C_s_Engine* engine) {
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
	SelectObject(hdc, engine->BG_Pen);
	SelectObject(hdc, engine->BG_Brush);

	if (is_top_border)
		Rectangle(hdc, (x + 2) * GLOBAL_SCALE, (y + 2) * GLOBAL_SCALE, (x + 3) * GLOBAL_SCALE, (y + 3) * GLOBAL_SCALE);
	else
		Rectangle(hdc, (x + 2) * GLOBAL_SCALE, (y + 1) * GLOBAL_SCALE, (x + 3) * GLOBAL_SCALE, (y + 2) * GLOBAL_SCALE);
}
//===================================================================================================
// Отрисовка рамки
void C_s_Border::Draw(HDC hdc, RECT& paint_area, C_s_Engine* engine) {
	// 1. Отрисовка рамки слева
	for (BYTE i = 0; i < 50; i++)
		draw_element(hdc, 2, 1 + i * 4, false, engine);
	// 2. Отрисовка рамки справа
	for (BYTE i = 0; i < 50; i++)
		draw_element(hdc, 201, 1 + i * 4, false, engine);
	// 3. Отрисовка рамки сверху
	for (BYTE i = 0; i < 50; i++)
		draw_element(hdc, 3 + i * 4, 0, true, engine);
}
//===================================================================================================




//C_s_Engine
//===================================================================================================
// Настройка игры при старте
void C_s_Engine::Init(HWND hWnd) {
	HWnd = hWnd;
	create_pen_and_brush(RGB(0, 0, 0), BG_Pen, BG_Brush);

	Ball.Init();
	Level.Init();
	Platform.Init();
	Platform.Redraw(this);
	Border.Init();
	
	SetTimer(HWnd, TIMER_ID, 40, NULL);
}
//===================================================================================================
// Отрисовка экрана игры
void C_s_Engine::Draw_Frame(HDC hdc, RECT& paint_area) {
	Level.Draw(hdc, paint_area, this);
	Ball.Draw(hdc, paint_area, this);
	Platform.Draw(hdc, this, paint_area);
	Border.Draw(hdc, paint_area, this);

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
		Platform.X_Pos -= Platform.Step;
		if (Platform.X_Pos <= C_s_Border::Border_X_Min)
			Platform.X_Pos = C_s_Border::Border_X_Min;
		Platform.Redraw(this);
		break;


	case EKT_Right:
		Platform.X_Pos += Platform.Step;
		if (Platform.X_Pos + Platform.Width >= C_s_Border::Max_X_Pos + 1)
			Platform.X_Pos = C_s_Border::Max_X_Pos - Platform.Width + 1;
		Platform.Redraw(this);
		break;


	case EKT_Space:

		break;
	}
	return 0;
}
//===================================================================================================
// Обработка таймера
int C_s_Engine::On_Timer() {
	Ball.Move(this, &Level, &Platform);
	return 0;
}
//===================================================================================================
// Присвоение цвета перу и кисти
void C_s_Engine::create_pen_and_brush(COLORREF rgb, HPEN& pen, HBRUSH& brush) {
	pen = CreatePen(PS_SOLID, 0, rgb);
	brush = CreateSolidBrush(rgb);
}
//===================================================================================================