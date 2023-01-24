#ifndef BALL_H
#define BALL_H



//===================================================================================================
#include "Config.h"
#include "Level.h"
//===================================================================================================
class C_Ball {
public:																			// ------------------
	C_Ball();																	//
	void Init();																// > Настройка шарика при старте
	void Draw(HDC hdc, RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush);			// > Отрисовка шарика
	void Move(HWND hWnd, C_Level* level, short platform_x_pos,					// > Обработка движения шарика
		BYTE platform_width);													//

private:																		// ------------------
	double Speed, Direction;													// > Скорость и направление шарика
	int X_Pos, Y_Pos;															// > Позиция шарика в координатах
	RECT Rect, Prev_Rect;														// > Прямоугольники для рисования шарика
	HPEN White_Pen;																// > Перья и кисти для рисования шарика
	HBRUSH White_Brush;															//   (белый цвет)
};



#endif // !BALL_H
