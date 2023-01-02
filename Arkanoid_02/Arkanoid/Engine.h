#ifndef ENGINE_H
#define ENGINE_H

#include <Windows.h>
#define GLOBAL_SCALE 3



// Настройка игры при старте
void Init();

// Отрисовка экрана игры
void Draw_Frame(HDC hdc);

#endif