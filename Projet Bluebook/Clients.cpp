#include "resource.h"
#include <string>
#include "XmlParser.h"
#include <windows.h>
#include "Tools.h"
#include "Fonctionglobal.h"
#include "Clients.h"

using namespace std;
DLGPROC CclientDLG::clients_procedure( UINT msg, WPARAM wParam, LPARAM lParam)
{
	int wmId = NULL, wmEvent = NULL;

	switch (msg)
	{
	case WM_INITDIALOG:
		initDialog();
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
		if (wmId == IDOK) EndDialog(getwnd(), 0);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}
	return 0;

}

void CclientDLG::initDialog()
{
	char singleBuffer[128];
	strcpy(singleBuffer, "Clients.xml");
	wstring sz = CTools::OpenTextFile(singleBuffer);
	CXmlParserDocument baseclients;
	baseclients.Text = sz;
	/*baseclients.Parse();
	CNode* pFirstNode = baseclients.first_node();
	CNode* pNodeClients = pFirstNode->first_node((wchar_t*)*L"Client");
	CNode* pCaract;
	for (pCaract = pNodeClients->first_node((wchar_t*)*L"Caracteristique"); pCaract != 0; pCaract = pCaract->next_sibling());
	{
		sz = pCaract->GetValue((wchar_t*)"Nom");
		SendMessage(GetDlgItem(hWnd, IDC_LIST_CLIENT), LB_ADDSTRING, 0, (LPARAM)sz.data());

	}*/
	HWND hWndControlIDCNom = GetDlgItem(getwnd(), IDC_NOM);
	m_editNom.setwnd(hWndControlIDCNom);
	m_editNom.settext(L"Blabla");
}

DLGPROC CclientDLG::clients_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE || msg == WM_INITDIALOG)
	{
		CREATESTRUCT* c = (CREATESTRUCT*)lParam;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)c->lpCreateParams);
		return (DLGPROC) DefDlgProc(hWnd, msg, wParam, lParam);
	}
	CclientDLG* pClient = (CclientDLG*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	pClient->clients_procedure(msg, wParam, lParam);
}
