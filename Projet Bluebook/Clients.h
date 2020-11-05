#pragma once
#include <Windows.h>
#include "Ceditcontrole.h"

class CclientDLG : public Cfenetres
{
public:
	static DLGPROC clients_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	DLGPROC clients_procedure(UINT msg, WPARAM wParam, LPARAM lParam);
		void initDialog();

private:

	Ceditcontrole m_editNom;
};