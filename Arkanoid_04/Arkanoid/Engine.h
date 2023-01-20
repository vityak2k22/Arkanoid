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
//===================================================================================================
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
// Шарик
class C_s_Engine;
class C_Level;
class C_s_Platform;
class C_Ball {
public:																			// ------------------
	C_Ball();																	//
	void Init();																// > Настройка шарика при старте
	void Draw(HDC hdc, RECT& paint_area, C_s_Engine* engine);					// > Отрисовка шарика
	void Move(C_s_Engine* engine, C_Level* level, C_s_Platform* platform);		// > Обработка движения шарика
																				// ------------------
	static const BYTE Size = 4;													// > Размер шарика
private:																		// ------------------
	double Speed, Direction;													// > Скорость и направление шарика
	int X_Pos, Y_Pos;															// > Позиция шарика в координатах
	RECT Rect, Prev_Rect;														// > Прямоугольники для рисования шарика
	HPEN White_Pen;																// > Перья и кисти для рисования шарика
	HBRUSH White_Brush;															//   (белый цвет)
};
//===================================================================================================
// Уровень
class C_Level {
public:																			// ------------------
	void Init();																// > Настройка уровня при старте
	void Check_Brick_Hit(int& next_y_pos, double& ball_direction);				// > Определение отражения от кирпичей
	void Draw(HDC hdc, RECT& paint_area, C_s_Engine* engine);					// > Отрисовка уровня
																				// ------------------
	static const BYTE X_Offset = 8;												// > Отступ от начала координат по оси Ох
	static const BYTE Y_Offset = 6;												// > Отступ от начала координат по оси Оу
	static const BYTE Brick_Width = 15;											// > Ширина кирпича
	static const BYTE Brick_Height = 7;											// > Высота кирпича
	static const BYTE Cell_Width = Brick_Width + 1;								// > Ширина ячейки
	static const BYTE Cell_Height = Brick_Height + 1;							// > Высота ячейки
	static const BYTE Width = 12;												// > Ширина уровня в ячейках
	static const BYTE Height = 14;												// > Высота уровня в ячейках
private:																		// ------------------
	void draw_brick(HDC hdc, int x, int y, E_Brick_Type brick_type);			// > Отрисовка кирпича
	void set_brick_letter_colors(bool is_switch_color, HPEN& front_pen,			// > Инициализация фона и переднего плана
		HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);				//	 кирпича при его падении
	void draw_brick_letter(HDC hdc, WORD x, WORD y, E_Brick_Type brick_type,	// > Отрисовка поворота кирпича
		E_Letter_Type letter_type, WORD rotation_step);							//
																				// ------------------
	RECT Rect;																	// > Прямоугольник для рисования уровня
	HPEN Red_Pen, Blue_Pen, White_Pen;											// > Перья и кисти для рисования уровня и падающей
	HBRUSH Red_Brush, Blue_Brush;												// > буквы (красный, синий и белый цвета)
};
//===================================================================================================
// Платформа
class C_s_Platform {
public:																			// ------------------
	C_s_Platform();																//
	void Init();																// > Настройка платформы при старте
	void Draw(HDC hdc, C_s_Engine* engine, RECT& paint_area);					// > Отрисовка платформы
	void Redraw(C_s_Engine* engine);											// > Перерисовка платформы при её перемещении
																				// ------------------
	static const BYTE Y_Pos = 185;												// > Позиция платформы по оси Оу
	static const BYTE Step = GLOBAL_SCALE;										// > Шаг перемещения платформы
																				// ------------------
	short X_Pos;																// > Позиция платформы по оси Ох
	BYTE Width;																	// > Полная ширина платформы
private:																		// ------------------
	static const BYTE Circle_Size = 7;											// > Диаметр колёсиков платформы
	static const BYTE Height = 7;												// > Высота платформы
																				// ------------------
	BYTE Inner_Width;															// > Внутренняя ширина платформы (ширина доски)
	RECT Rect, Prev_Rect;														// > Прямоугольники рисования
	HPEN Red_Pen, Blue_Pen;														// > Перья и кисти для рисования платформы
	HBRUSH Red_Brush, Blue_Brush;												//   (красный и синий цвета)
};
//===================================================================================================
// Рамка
class C_s_Border {
public:																			// ------------------
	void Init();																// > Настройка рамки при старте
	void Draw(HDC hdc, RECT& paint_area, C_s_Engine* engine);					// > Отрисовка рамки
																				// ------------------
	static const BYTE Border_X_Min = 6;											// > Отступ для левой стороны рамки
	static const BYTE Border_Y_Max = 4;											// > Отступ для верхней стороны рамки
	static const BYTE Max_X_Pos = C_Level::X_Offset + C_Level::Cell_Width		// > Крайний правый предел для шарика и платформы
		* C_Level::Width;														// 
	static const BYTE Max_Y_Pos = 200 - 1 - C_Ball::Size;						// > Крайний нижний предел для шарика
private:																		// ------------------
	void draw_element(HDC hdc, WORD x, WORD y, bool is_top_border,				// > Отрисовка элемента рамки
		C_s_Engine* engine);													//
																				// ------------------
	HPEN Blue_Pen, White_Pen;													// > Перья и кисти для рисования рамки
	HBRUSH Blue_Brush, White_Brush;												//   (синий и белый цвета)
};
//===================================================================================================
// Движок
class C_s_Engine {
public:																			// ------------------
	void Init(HWND hWnd);														// > Настройка игры при старте
	void Draw_Frame(HDC hdc, RECT& paint_area);									// > Отрисовка экрана игры
	int On_Key_Down(E_Key_Type key_type);										// > Обработка нажатых клавиш
	int On_Timer();																// > Обработка таймера
																				// ------------------
	static void create_pen_and_brush(COLORREF rgb, HPEN& pen, HBRUSH& brush);	// > Присвоение цвета перу и кисти
																				// ------------------
	HWND HWnd;																	// > Дескриптор окна			
	HPEN BG_Pen;																// > Перо для рисования заднего фона
	HBRUSH BG_Brush;															// > Кисть для рисования заднего фона
private:																		// ------------------
	C_Ball Ball;																// > Шарик
	C_Level Level;																// > Уровень
	C_s_Platform Platform;														// > Платформа
	C_s_Border Border;															// > Рамка
};
//===================================================================================================



#endif