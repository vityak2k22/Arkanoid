#ifndef LEVEL_H
#define LEVEL_H



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
#include "Config.h"
//===================================================================================================
class C_Level {
public:																			// ------------------
	C_Level();
	void Init();																// > Настройка уровня при старте
	void Check_Brick_Hit(int& next_y_pos, double& ball_direction);				// > Определение отражения от кирпичей
	void Draw(HDC hdc, RECT& paint_area);										// > Отрисовка уровня
	// ------------------
	
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




#endif // !LEVEL_H
