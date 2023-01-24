#ifndef CONFIG_H
#define CONFIG_H



//===================================================================================================
#include <Windows.h>
#define _USE_MATH_DEFINES
#include <cmath>
//===================================================================================================
#define GLOBAL_SCALE 3
#define TIMER_ID WM_USER + 1
//===================================================================================================
class C_s_Config {
public:
	static void Create_Pen_and_Brush(COLORREF rgb, HPEN& pen, HBRUSH& brush);	// > Присвоение цвета перу и кисти

	//level&brick
	static const BYTE X_Offset = 8;												// > Отступ от начала координат по оси Ох
	static const BYTE Y_Offset = 6;												// > Отступ от начала координат по оси Оу
	static const BYTE Brick_Width = 15;											// > Ширина кирпича
	static const BYTE Brick_Height = 7;											// > Высота кирпича
	static const BYTE Cell_Width = Brick_Width + 1;								// > Ширина ячейки
	static const BYTE Cell_Height = Brick_Height + 1;							// > Высота ячейки
	static const BYTE Width = 12;												// > Ширина уровня в ячейках
	static const BYTE Height = 14;												// > Высота уровня в ячейках

	//border
	static const BYTE Border_X_Min = 6;											// > Отступ для левой стороны рамки
	static const BYTE Border_Y_Max = 4;											// > Отступ для верхней стороны рамки
	
	//ball
	static const BYTE Size = 4;													// > Размер шарика

	//platform
	static const BYTE Y_Pos = 185;												// > Позиция платформы по оси Оу
	static const BYTE Step = GLOBAL_SCALE;										// > Шаг перемещения платформы

	//engine
	static const BYTE Max_X_Pos = X_Offset + Cell_Width							// > Крайний правый предел для шарика и платформы
		* Width;																// 
	static const BYTE Max_Y_Pos = 200 - 1 - Size;								// > Крайний нижний предел для шарика

};
//===================================================================================================



#endif // !CONFIG_H

