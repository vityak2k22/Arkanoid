#include "Config.h"

//===================================================================================================
// Присвоение цвета перу и кисти
void C_s_Config::Create_Pen_and_Brush(COLORREF rgb, HPEN& pen, HBRUSH& brush) {
	pen = CreatePen(PS_SOLID, 0, rgb);
	brush = CreateSolidBrush(rgb);
}
//===================================================================================================
