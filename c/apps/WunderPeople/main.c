/* Windows Header Files */
#include <windows.h>
#include <commctrl.h>

/* C RunTime Header Files */
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "resource.h"

#define MAX_LOADSTRING 100

/* Initial window size. */
#define INIT_WINDOW_WIDTH  500
#define INIT_WINDOW_HEIGHT 350

/* Global Variables */
HINSTANCE g_hInst;     /* Handle to module instance */

HWND g_hwndWP;         /* Handle to main window */
HWND g_hwndStatusBar;  
HWND g_hwndListView;

LPTSTR g_szClassName = TEXT("WuPAB");  /* Window class name */


/* Foward declarations of functions included in this code module: */
BOOL				InitWindowClass(HINSTANCE);
BOOL				CreateMainWindow(HINSTANCE, int);
void				CreateChildrenControls(HWND);
void				CreateListView(HWND);
LRESULT CALLBACK	MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	AboutDlgProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	g_hInst = hInstance;

	InitCommonControls();

	if (!InitWindowClass(hInstance))
	{
		return FALSE;
	}

	/* Perform application initialization */
	if (!CreateMainWindow(hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)MainAccel);

	/* Main Message Loop */
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}


/*
 *  FUNCTION: InitWindowClass()
 *
 *  PURPOSE: Registers the window class.
 *
 *  COMMENTS:
 *
 *    This function and its usage is only necessary if you want this code
 *    to be compatible with Win32 systems prior to the 'RegisterClassEx'
 *    function that was added to Windows 95. It is important to call this function
 *    so that the application will get 'well formed' small icons associated
 *    with it.
 */
BOOL InitWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize         = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)MainWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	wcex.lpszMenuName	= NULL; /*(LPCSTR)IDR_MENUBAR;*/
	wcex.lpszClassName	= g_szClassName;
	wcex.hIconSm		= LoadImage(hInstance,
								    MAKEINTRESOURCE(IDI_APP_ICON),
									IMAGE_ICON, 16, 16,
									LR_DEFAULTCOLOR); /*LoadIcon(hInstance, (LPCTSTR)IDI_SMALL);*/

	if (!RegisterClassEx(&wcex))
	{
		return FALSE;
	}

	return TRUE;
}


/*
 *   FUNCTION: CreateMainWindow(HANDLE, int)
 *
 *   PURPOSE: Creates main window
 *
 *   COMMENTS:
 *
 *        In this function, we create and display the main program window.
 */
BOOL CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	TCHAR szTitle[MAX_LOADSTRING];  /* Title bar text */
	HMENU hMenu;

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);

	hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENUBAR));

	/* Create the main window for this application instance. */
	g_hwndWP = CreateWindowEx(0,
		                      g_szClassName,
	                          szTitle,
		                      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                              CW_USEDEFAULT,
					          CW_USEDEFAULT,
					          INIT_WINDOW_WIDTH,
					          INIT_WINDOW_HEIGHT,
					          NULL,
					          hMenu,
					          hInstance,
					          NULL);

	if (!g_hwndWP)
	{
		return FALSE;
	}

	ShowWindow(g_hwndWP, nCmdShow);
	UpdateWindow(g_hwndWP);

	return TRUE;
}


void CreateListView(HWND hWndParent)
{
	LVCOLUMN lvc;
	int iCol;
	TCHAR szText[3][100] = {TEXT("First Name"), TEXT("Last Name"), TEXT("Email")};

	/* Create List View */
	g_hwndListView = CreateWindowEx(WS_EX_CLIENTEDGE,
		                            WC_LISTVIEW,
		                            (LPTSTR) NULL,
		                            WS_TABSTOP | WS_HSCROLL | WS_VSCROLL | WS_VISIBLE |
		                            WS_CHILD | LVS_REPORT | LVS_SHOWSELALWAYS | WS_EX_CLIENTEDGE,
		                            0,
									0,
									0,
									0,
		                            hWndParent,
		                            NULL,
		                            g_hInst,
			                        NULL);

	/* Init List View (images, columns, etc...) */
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	lvc.cx = 100;

	for (iCol = 0; iCol < 3; iCol++)
	{
		lvc.iSubItem = iCol;
		lvc.pszText = szText[iCol];
		ListView_InsertColumn(g_hwndListView, iCol, &lvc);
	}
}

void CreateChildrenControls(HWND hWndParent)
{
	/* Create Status Bar */
	g_hwndStatusBar = CreateWindowEx(0,                /* ex style */
			                         STATUSCLASSNAME,  /* class name - defined in commctrl.h */
	                                 (LPTSTR)NULL,     /* dummy text */
	                                 WS_CHILD | WS_VISIBLE | SBS_SIZEGRIP,  /* style */
	                                 0,                /* x position */
						             0,                /* y position */
						             0,                /* width */
						             0,                /* height */
	                                 hWndParent,       /* parent */
	                                 NULL,             /* ID */
	                                 g_hInst,          /* instance */
	                                 NULL);            /* no extra data */

	CreateListView(hWndParent);
}

/*
 *  FUNCTION: MainWndProc(HWND, unsigned, WORD, LONG)
 *
 *  PURPOSE:  Processes messages for the main window.
 *
 *  WM_COMMAND	- process the application menu
 *  WM_PAINT	- Paint the main window
 *  WM_DESTROY	- post a quit message and return
 */
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;
	RECT rc;

	switch (uMsg) 
	{
		case WM_CREATE:
			CreateChildrenControls(hWnd);
			break;

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			/* Parse the menu selections: */
			switch (wmId)
			{
				case IDM_EXIT:
					SendMessage(hWnd, WM_CLOSE, 0, 0L);
					break;

				case IDM_STATUSBAR:
					if (IsWindowVisible(g_hwndStatusBar))
					{
						CheckMenuItem(GetMenu(hWnd), IDM_STATUSBAR, MF_BYCOMMAND | MF_UNCHECKED);
						ShowWindow(g_hwndStatusBar, SW_HIDE);
					}
					else
					{
						CheckMenuItem(GetMenu(hWnd), IDM_STATUSBAR, MF_BYCOMMAND | MF_CHECKED);
						ShowWindow(g_hwndStatusBar, SW_NORMAL);
					}
					break;

				case IDM_ABOUT:
				   DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)AboutDlgProc);
				   break;

				default:
				   return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
			break;


		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			/* TODO: Add any drawing code here...*/
			GetClientRect(hWnd, &rt);
			EndPaint(hWnd, &ps);
			break;


		case WM_SIZE:
			GetClientRect(hWnd, &rc);

			MoveWindow(g_hwndStatusBar,
				       0,
				       rc.bottom - 14,
				       rc.right - rc.left,
				       14,
				       TRUE);

			MoveWindow(g_hwndListView,
				       rc.left,
					   rc.top,
					   rc.right - rc.left,
					   rc.bottom - rc.top,
					   TRUE);
			break;


		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;


		case WM_DESTROY:
			PostQuitMessage(0);
			break;


		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
   }

   return 0;
}


/*
 * Mesage handler for about box.
 */
LRESULT CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
			return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}

    return FALSE;
}
