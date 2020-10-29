#pragma once
#include <windows.h>

HINSTANCE hInstance;
DLGPROC charges_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
DLGPROC information_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
DLGPROC fenetre_modless(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
DLGPROC clients_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


