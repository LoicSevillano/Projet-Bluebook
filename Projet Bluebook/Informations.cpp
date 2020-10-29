#include "resource.h"
#include <string>
#include <windows.h>
#include "Tools.h"

DLGPROC information_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	wstring mois[12] = { L"Janvier", L"Février",L"Mars", L"Avril",L"Mai", L"Juin",L"Juillet", L"Août",
						L"Septembre", L"Octobre",L"Novembre", L"Décembre" };
	int jours_mois[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

	switch (msg)
	{
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hWnd, IDC_INFONOM), WM_SETTEXT, 0, (LPARAM)L"Entrez votre Nom");
		SendMessage(GetDlgItem(hWnd, IDC_INFOPRENOM), WM_SETTEXT, 0, (LPARAM)L"Entrez votre Prénom");

		for (int i = 0; i < 12; i++)
		{
			SendMessage(GetDlgItem(hWnd, IDC_COMBOMOIS), CB_ADDSTRING, 0, (LPARAM)mois[i].data());
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}
	return 0;
}