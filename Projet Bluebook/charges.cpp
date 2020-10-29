#include "resource.h"
#include <string>
#include "XmlParser.h"
#include <windows.h>
#include "Tools.h"
#include "Fonctionglobal.h"

using namespace std;
DLGPROC charges_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char singleBuffer[128];
	strcpy(singleBuffer, "Bluebookconfig.xml");
	wstring sz = CTools::OpenTextFile(singleBuffer);
	CXmlParserDocument bluebookconfig;
	bluebookconfig.Text = sz;
	bluebookconfig.Parse();
	int wmId=NULL, wmEvent=NULL;
	wstring mois[12] = { L"Janvier", L"Février",L"Mars", L"Avril",L"Mai", L"Juin",L"Juillet", L"Août",
						L"Septembre", L"Octobre",L"Novembre", L"Décembre"};
	int jours_mois[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	const int tauxcharges = 22;
	const int tauximpot = 2;

	switch (msg)
	{
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hWnd, IDC_MONTANT), WM_SETTEXT, 0, (LPARAM)L"0.00");

		for (int i = 0; i < 12; i++)
		{
			SendMessage(GetDlgItem(hWnd, IDC_MOIS), LB_ADDSTRING, 0, (LPARAM)mois[i].data());
		}
	break;

	case WM_COMMAND:
		if (!lParam)
		{
			wmId = HIWORD(wParam);
			wmEvent = LOWORD(wParam);
		}
		else
		{
			wmId = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
		}
		if (wmId == IDC_CALCULER)
		{
			int LigneMois;
			LigneMois =SendMessage( GetDlgItem(hWnd, IDC_MOIS), LB_GETCURSEL, 0,0);
			wchar_t Buffer[128];
			SendMessage(GetDlgItem(hWnd, IDC_MONTANT), WM_GETTEXT, 128, (LPARAM) &Buffer);
			wstring szMontant = Buffer;
			double Montant = _wtof(szMontant.data());
			int nombrejours = jours_mois[LigneMois];
			
			swprintf_s(Buffer, 128, L"%.2lf€", calcul_charges(Montant));
			SendMessage(GetDlgItem(hWnd, IDC_CHARGE), WM_SETTEXT, 0, (LPARAM)&Buffer);

			swprintf_s(Buffer, 128, L"%.2lf€", calcul_impots(Montant));
			SendMessage(GetDlgItem(hWnd, IDC_IMPOT), WM_SETTEXT, 0, (LPARAM)&Buffer);

			swprintf_s(Buffer, 128, L"%.2lf€", calcul_total(calcul_charges(Montant),calcul_impots(Montant)));
			SendMessage(GetDlgItem(hWnd, IDC_TOTAL), WM_SETTEXT, 0, (LPARAM)&Buffer);

		}
		else if (wmId == IDOK) EndDialog(hWnd, 0);
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	break;
	}
	return 0;
}
