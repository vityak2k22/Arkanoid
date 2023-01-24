#ifndef BORDER_H
#define BORDER_H


//===================================================================================================
#include "Config.h"
//===================================================================================================
class C_s_Border {
public:																			// ------------------
	C_s_Border();
	void Init();																// > Настройка рамки при старте
	void Draw(HDC hdc, RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush);			// > Отрисовка рамки

private:																		// ------------------
	void draw_element(HDC hdc, WORD x, WORD y, bool is_top_border,				// > Отрисовка элемента рамки
		HPEN bg_pen, HBRUSH bg_brush);											//
	// ------------------
	HPEN Blue_Pen, White_Pen;													// > Перья и кисти для рисования рамки
	HBRUSH Blue_Brush, White_Brush;												//   (синий и белый цвета)
};
//===================================================================================================




#endif // !BORDER_H
