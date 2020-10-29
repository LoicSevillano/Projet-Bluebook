#include <windows.h>
#include "resource.h"
#include "variableglobal.h"

LRESULT CALLBACK MyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    WNDCLASS wnd;
    MSG msg;
    HWND hwnd;
    hInstance = hInst;
    const wchar_t szClassName[] = L"MaPremiereClassWindows";
    wnd.style = CS_HREDRAW | CS_VREDRAW; //we will explain this later
    wnd.lpfnWndProc = MyWndProc;
    wnd.cbClsExtra = 0;
    wnd.cbWndExtra = 0;
    wnd.hInstance = hInstance;
    wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION); //default icon
    wnd.hCursor = LoadCursor(NULL, IDC_ARROW);   //default arrow mouse cursor
    wnd.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 4);
    HBRUSH hBrush = CreateSolidBrush(RGB(153, 12, 23));
    wnd.hbrBackground = hBrush;
    wnd.lpszMenuName = MAKEINTRESOURCE(IDR_MENU_MAIN);                    //no menu
    wnd.lpszClassName = szClassName;

    if (!RegisterClass(&wnd))                     //enregistrementde la class WNDCLASS
    {
        MessageBox(NULL, L"This Program Requires Windows 10",
            L"Error", MB_OK);
        return 0;
    }
    hwnd = CreateWindow(szClassName,
        L"Window Title",
        WS_OVERLAPPEDWINDOW, //basic window style
        CW_USEDEFAULT,
        CW_USEDEFAULT,       //set starting point to default value
        CW_USEDEFAULT,
        CW_USEDEFAULT,       //set all the dimensions to default value
        NULL,                //no parent window
        NULL,                //no menu
        hInstance,
        NULL);               //no parameters to pass
    ShowWindow(hwnd, iCmdShow);              //display the window on the screen
    UpdateWindow(hwnd);             //make sure the window is updated correctly

    while (GetMessage(&msg, NULL, 0, 0))      //message loop
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK MyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_COMMAND:
        switch(wParam)
        {
            case ID_FICHIER_CHARGES:
                DialogBoxParamW(hInstance, MAKEINTRESOURCE(IDD_CHARGE), hwnd, (DLGPROC) charges_proc, NULL);
                break;
            case ID_FICHIER_INFORMATIONS:
                DialogBoxParamW(hInstance, MAKEINTRESOURCE(IDD_INFORMATION), hwnd, (DLGPROC)information_proc, NULL);
                break;
            case ID_FICHIER_FENETRE_LIBRE:
                CreateDialogW(hInstance, MAKEINTRESOURCE(IDD_MODLESS), hwnd, (DLGPROC)fenetre_modless);
                break;
                //Fenetre modal : DialogBoxParamW
                // Fenetre modles : sCreateDIalog
            case ID_FICHIER_CLIENTS:
                DialogBoxParamW(hInstance, MAKEINTRESOURCE(IDD_CLIENTS), hwnd, (DLGPROC)clients_proc, NULL);
                break;
        }
    
        
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}