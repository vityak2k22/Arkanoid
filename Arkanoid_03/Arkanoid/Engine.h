#ifndef ENGINE_H
#define ENGINE_H

#include <Windows.h>
#define GLOBAL_SCALE 3
#define TIMER_ID WM_USER + 1

enum E_Key_Type {
	EKT_Left,
	EKT_Right,
	EKT_Space
};

// Настройка игры при старте
void Init_Engine(HWND hWnd);

// Отрисовка экрана игры
void Draw_Frame(HDC hdc, RECT &paint_area);

// Обработка нажатых клавиш
int On_Key_Down(E_Key_Type key_type);

// Обработка таймера
int On_Timer();
#endif