#include "fenetremodless.h"
#include <string>
#include <windowsx.h>
#include "rectangle.h"

using namespace std;
bool bmouseclick;
POINT mouseposition;

DLGPROC fenetre_modless(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDc;
	PAINTSTRUCT ps;
	
	switch (msg)
	{
		case WM_PAINT:
			hDc = BeginPaint(hWnd, &ps);
			peinturefenetre(hWnd, hDc);
			EndPaint(hWnd, &ps);

		break;	

		case WM_CREATE :
			bmouseclick = true;
		break;

		case WM_LBUTTONDOWN:
			bmouseclick = true;
		break;

		case WM_LBUTTONUP:
			bmouseclick = false;
		break;

		case WM_MOUSEMOVE:
			if (bmouseclick)
			{
				mouseposition.x = GET_X_LPARAM(lParam);
				mouseposition.y = GET_Y_LPARAM(lParam);
				ScreenToClient(hWnd, &mouseposition);
				InvalidateRect(hWnd, NULL, false);
			}
			break;

		case WM_KEYDOWN:
			switch (wParam)
			{
			case 38:
			{
				POINT old = mouseposition;
				time_t tdepart = GetCurrentTime();
				time_t tfin = tdepart + 200;
				mouseposition.y = mouseposition.y - 40;
				InvalidateRect(hWnd, NULL, false);
				/*Sleep(200);
				mouseposition = old;
				InvalidateRect(hWnd, NULL, false);*/
				MSG mSg;
				while (GetCurrentTime() < tfin)
				{
					PeekMessage(&mSg, NULL, 0, 0, PM_REMOVE);
					TranslateMessage(&mSg);
					DispatchMessage(&mSg);
				}
				mouseposition.y = mouseposition.y + 40;
				InvalidateRect(hWnd, NULL, false);

			}
			break;
			}
		break;
	}
	return 0;

}
void peinturefenetre(HWND hWnd,HDC hDc)
{
	RECT R;
	int TailleX;
	HPEN pen;

	GetWindowRect(hWnd, &R);
	R = { 0,0, R.right - R.left, R.bottom - R.top };
	TailleX = GetDeviceCaps(hDc, HORZRES);
	pen = CreatePen(PS_DOT, 10, RGB(0, 0, 255));

	// //////////////////////////////////////////////////////////// Font utilise /////////////////////////////////////////////////
	HFONT font = CreateFont(
		-MulDiv(10, GetDeviceCaps(hDc, LOGPIXELSY), 72),
		0,
		0,
		0,
		FW_EXTRABOLD,
		FALSE,
		FALSE,
		0,
		ANSI_CHARSET,
		OUT_DEVICE_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FF_SWISS || DEFAULT_PITCH,
		L"Arial");

	/// //////////////////////////////////////////////////////////////////////// Couleur de fond  /////////////////////////////////////////
	SetBkColor(hDc, RGB(192, 192, 192));
	SelectObject(hDc, font);
	SetTextColor(hDc, RGB(0, 0, 0));
	SetTextAlign(hDc, TA_LEFT);
	wstring ma_chaine = L"Hello my friend";
	HBRUSH grey_brush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hDc, grey_brush);
	FillRect(hDc, &R, grey_brush);
	SelectObject(hDc, pen);
	MoveToEx(hDc, 50, 50, NULL);
	LineTo(hDc, 300, 200);

	/// //////////////////////////////////////////////////////////////////////// Bitmap /////////////////////////////////////////

	HANDLE image;
	HDC hDcmem = CreateCompatibleDC(hDc);
	HPEN rouge = CreatePen(PS_DOT, 2, RGB(255, 0, 0));
	HBRUSH brush_jaune = CreateSolidBrush(RGB(255, 255, 128));
	SelectObject(hDc, rouge);
	SelectObject(hDc, brush_jaune);
	Rectangle(hDc, 70, 50, 120, 80);
	Ellipse(hDc, 130, 90, 170, 130);
	TextOut(hDc, 50, 50, ma_chaine.data(), ma_chaine.length());
	image = LoadImage(NULL, L"C:\\Users\\loic-\\source\\repos\\Projet Bluebook\\icone.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SelectObject(hDcmem, image);
	BitBlt(hDc, mouseposition.x, mouseposition.y , 106, 126, hDcmem, 0, 0, SRCCOPY);
	DeleteObject(pen);
	DeleteObject(rouge);
	DeleteObject(brush_jaune);
	DeleteDC(hDcmem);
	DeleteObject(image);
	DeleteObject(font);

	Crectangle Mon_Rectangle(12, 123, 34, 955);

	int Peri = Mon_Rectangle.Perimetre();
}