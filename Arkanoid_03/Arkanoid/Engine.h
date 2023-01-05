#ifndef ENGINE_H
#define ENGINE_H



//===================================================================================================
#include <Windows.h>
#define _USE_MATH_DEFINES
#include <cmath>
//===================================================================================================
#define GLOBAL_SCALE 3
#define TIMER_ID WM_USER + 1
//===================================================================================================
enum E_Brick_Type {
	EBT_None,
	EBT_Red,
	EBT_Blue
};

enum E_Letter_Type {
	ELT_None,
	ELT_O
};
//===================================================================================================
enum E_Key_Type {
	EKT_Left,
	EKT_Right,
	EKT_Space
};
//===================================================================================================
class C_s_Engine {
public:																			// ------------------
	C_s_Engine();
	
	static const BYTE Level_Width = 12;											// Ширина уровня в ячейках
	static const BYTE Level_Height = 14;										// Высота уровня в ячейках
																				// ------------------
	void Init_Engine(HWND hWnd);												// > Настройка игры при старте
	void Draw_Frame(HDC hdc, RECT& paint_area);									// > Отрисовка экрана игры
	int On_Key_Down(E_Key_Type key_type);										// > Обработка нажатых клавиш
	int On_Timer();																// > Обработка таймера

private:																		// ------------------
	void create_pen_and_brush(COLORREF rgb, HPEN& pen, HBRUSH& brush);			// > Присвоение цвета перу и кисти
	void redraw_platform();														// > Перерисовка платформы при её перемещении
	void draw_brick(HDC hdc, int x, int y, E_Brick_Type brick_type);			// > Отрисовка кирпича
	void set_brick_letter_colors(bool is_switch_color, HPEN& front_pen,			// > Инициализация фона и переднего плана
		HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);				//	 кирпича при его падении
	void draw_brick_letter(HDC hdc, WORD x, WORD y, E_Brick_Type brick_type,	// > Отрисовка поворота кирпича
		E_Letter_Type letter_type, WORD rotation_step);							//
	void draw_level(HDC hdc);													// > Отрисовка уровня
	void draw_platform(HDC hdc, int x, int y);									// > Отрисовка платформы
	void draw_ball(HDC hdc, int x, int y);										// > Отрисовка шарика
	void draw_border(HDC hdc, WORD x, WORD y, bool is_top_border);				// > Отрисовка элемента рамки
	void draw_bounds(HDC hdc, RECT& paint_area);								// > Отрисовка рамки
	void check_brick_hit(int& next_y_pos);										// > Определение отражения от кирпичей
	void move_ball();															// > Обработка движения шарика
	
	static const BYTE Border_X_Min = 6;
	static const BYTE Border_Y_Max = 4;
	static const BYTE Level_X_Offset = 8;
	static const BYTE Level_Y_Offset = 6;
	static const BYTE Brick_Width = 15;
	static const BYTE Brick_Height = 7;
	static const BYTE Cell_Width = Brick_Width + 1;
	static const BYTE Cell_Height = Brick_Height + 1;
	static const BYTE Ball_Size = 4;
	static const BYTE Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width;
	static const BYTE Max_Y_Pos = 200 - 1 - Ball_Size;
	static const BYTE Circle_Size = 7;
	static const BYTE Platform_Y_Pos = 185;
	static const BYTE Platform_Height = 7;
	static const BYTE Platform_Step = GLOBAL_SCALE;

	HWND HWnd;
	RECT Level_Rect;

	double Ball_Speed, Ball_Direction;
	int Ball_X_Pos, Ball_Y_Pos;
	RECT Ball_Rect, Prev_Ball_Rect;
	
	int Platform_X_Pos;
	BYTE Inner_Width, Platform_Width;
	RECT Platform_Rect, Prev_Platform_Rect;
	
	HPEN Red_Pen, Blue_Pen, BG_Pen, White_Pen;
	HBRUSH Red_Brush, Blue_Brush, BG_Brush, White_Brush;
};
//===================================================================================================



#endif