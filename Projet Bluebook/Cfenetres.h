#pragma 
#include "rectangle.h"
#include <Windows.h>

class Cfenetres 
{
	protected:
	// Handle sur la fenetre
	HWND m_hWnd;
	Crectangle Rfenetres;

	public:
		HWND getwnd();

		void setwnd(HWND hWnd);
};

