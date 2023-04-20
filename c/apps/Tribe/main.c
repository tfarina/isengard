/* Windows Header Files */
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

/* C RunTime Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "db.h"
#include "resource.h"
#include "alpm_list.h"

#define MAX_LOADSTRING 100

/* Initial window size. */
#define INIT_WINDOW_WIDTH  500
#define INIT_WINDOW_HEIGHT 350

typedef struct _CONTACT
{
	TCHAR szFirstName[MAX_LOADSTRING];
	TCHAR szLastName[MAX_LOADSTRING];
	TCHAR szEmail[MAX_LOADSTRING];
} CONTACT, *LPCONTACT;

/* Global Variables */
HINSTANCE g_hInst;     /* Handle to module instance */

HWND g_hwndWP;         /* Handle to main window */
HWND g_hwndStatusBar;  
HWND g_hwndListView;
HWND g_hwndToolbar;

LPTSTR g_szClassName = TEXT("WuPAB");  /* Window class name */

LONG g_hStatus;

static alpm_list_t *contactList = NULL;

/* Foward declarations of functions included in this code module: */
BOOL				InitWindowClass(HINSTANCE);
BOOL				CreateMainWindow(HINSTANCE, int);
void				CreateChildrenControls(HWND);
void				CreateListView(HWND);
void				CreateToolbar(HWND);
void				CreateStatusBar(HWND);
void				PopulateListView(void);
void				AdjustChildrenControls(HWND);
void				SelectAllItems(void);
LRESULT CALLBACK	MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	AboutDlgProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
    int nCmdShow
	)
{
	MSG msg;
	HACCEL hAccelTable;  /* Handle to accelerator table */
    INITCOMMONCONTROLSEX iccex;
    int rc;

	g_hInst = hInstance;

	iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccex.dwICC = ICC_ANIMATE_CLASS |
				  ICC_BAR_CLASSES |
				  ICC_COOL_CLASSES |
				  ICC_DATE_CLASSES |
				  ICC_LISTVIEW_CLASSES |
				  ICC_NATIVEFNTCTL_CLASS |
				  ICC_PROGRESS_CLASS |
				  ICC_TAB_CLASSES |
				  ICC_WIN95_CLASSES;

	InitCommonControlsEx(&iccex);

	/* Initialize the window */
	if (!InitWindowClass(hInstance))
	{
		return FALSE;
	}

	/* Create the main window */
	if (!CreateMainWindow(hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	rc = db_init();
	if (rc < 0)
	{
		/* We should show a Message Box with an error message here. */
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_MAINWND_ACCEL));

	/* Main Message Loop */
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(g_hwndWP, hAccelTable, &msg)) 
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
	g_hwndWP = CreateWindowEx(
		0,
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


void load_contacts(void)
{
	LPCONTACT contact;

	contact = LocalAlloc(LMEM_ZEROINIT, sizeof(CONTACT));
	if (!contact)
	{
		/* Out of memory. */
	}
	lstrcpy(contact->szFirstName, "John");
	lstrcpy(contact->szLastName, "Doe");
	lstrcpy(contact->szEmail, "john_doe@mail.com");

	contactList = alpm_list_add(contactList, contact);

	contact = NULL;
	contact = LocalAlloc(LMEM_ZEROINIT, sizeof(CONTACT));
	if (!contact)
	{
		/* Out of memory. */
	}
	lstrcpy(contact->szFirstName, "Jane");
	lstrcpy(contact->szLastName, "Doe");
	lstrcpy(contact->szEmail, "jane_doe@mail.com");

	contactList = alpm_list_add(contactList, contact);

	contact = NULL;
	contact = LocalAlloc(LMEM_ZEROINIT, sizeof(CONTACT));
	if (!contact)
	{
		/* Out of memory. */
	}
	lstrcpy(contact->szFirstName, "John");
	lstrcpy(contact->szLastName, "Smith");
	lstrcpy(contact->szEmail, "john_smith@mail.com");

	contactList = alpm_list_add(contactList, contact);
}


void
PopulateListView(
	void
	)
{
	alpm_list_t *item;
	LPCONTACT contact;
	LVITEM lvI = {0};
    int index = 0;

	load_contacts();

	lvI.mask = LVIF_TEXT | LVIF_STATE | LVIF_PARAM;
	lvI.iItem = 0;

	for (item = contactList; item; item = alpm_list_next(item))
	{
		contact = (LPCONTACT) item->data;
		lvI.iSubItem = 0; /* COL_FIRST_NAME */
		lvI.lParam = (LPARAM) contact;
		lvI.pszText = contact->szFirstName;

		index = ListView_InsertItem(g_hwndListView, &lvI);
		if (index != -1)
		{
			ListView_SetItemText(g_hwndListView, index, 1 /* COL_LAST_NAME*/, contact->szLastName);
			ListView_SetItemText(g_hwndListView, index, 2 /* COL_EMAIL */, contact->szEmail);
		}
		lvI.iItem++;
	}
}


void CreateListView(HWND hWndParent)
{
	LVCOLUMN lvc;
	int iCol;
	TCHAR szText[3][100] = {TEXT("First Name"), TEXT("Last Name"), TEXT("Email")};

	g_hwndListView = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		WC_LISTVIEW,
		(LPTSTR) NULL,
		WS_TABSTOP | WS_HSCROLL | WS_VSCROLL | WS_VISIBLE |
		WS_CHILD | LVS_REPORT | LVS_SHOWSELALWAYS | WS_EX_CLIENTEDGE,
		0,
		0,
		0,
		0,
		hWndParent,
		(HMENU) IDC_LISTVIEW,
		g_hInst,
		NULL);

	ListView_SetExtendedListViewStyle(g_hwndListView, LVS_EX_FULLROWSELECT);

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


void CreateToolbar(HWND hWndParent)
{
	HIMAGELIST hImageList = NULL;
	int const imageListID = 0;
	int const numButtons = 3;
	int const bitmapSize = 24;

	TBBUTTON tbButtons[] =
	{
		{ MAKELONG(STD_FILENEW, imageListID), IDC_TB_NEW, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR) TEXT("New") },
		{ MAKELONG(STD_PROPERTIES, imageListID), IDC_TB_PROPERTIES, 0, BTNS_AUTOSIZE, {0}, 0, (INT_PTR) TEXT("Properties") },
		{ MAKELONG(STD_DELETE, imageListID), IDC_TB_DELETE, 0, BTNS_AUTOSIZE, {0}, 0, (INT_PTR) TEXT("Delete") },
	};

	g_hwndToolbar = CreateWindowEx(
		0,                /* ex style */
		TOOLBARCLASSNAME, /* class name - defined in commctrl.h */
		(LPTSTR)NULL,     /* dummy text */
		WS_CHILD | WS_VISIBLE | TBSTYLE_WRAPABLE,  /* style */
		0,                /* x position */
        0,                /* y position */
		0,                /* width */
		0,                /* height */
		hWndParent,
		NULL,
		g_hInst,
		NULL);

	if (!g_hwndToolbar)
		return;

	hImageList = ImageList_Create(
		bitmapSize,
		bitmapSize,
		ILC_COLOR32 | ILC_MASK,
		numButtons,
		0);

	/* Set the image list */
	SendMessage(g_hwndToolbar, TB_SETIMAGELIST, (WPARAM) imageListID, (LPARAM) hImageList);

	/* Load the button images */
	SendMessage(g_hwndToolbar, TB_LOADIMAGES, (WPARAM) IDB_STD_LARGE_COLOR, (LPARAM) HINST_COMMCTRL);

	/* Add buttons */
	SendMessage(g_hwndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM) sizeof(TBBUTTON), 0);
	SendMessage(g_hwndToolbar, TB_ADDBUTTONS, (WPARAM) numButtons, (LPARAM) &tbButtons);

	/* Resize the toolbar, and then show it */
	SendMessage(g_hwndToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(g_hwndToolbar, TRUE);
}


void
CreateStatusBar(
	HWND hWndParent
	)
{
    RECT statusBarRect;

	g_hwndStatusBar = CreateWindowEx(
		0,                /* ex style */
		STATUSCLASSNAME,  /* class name - defined in commctrl.h */
		(LPTSTR) NULL,    /* dummy text */
		WS_CHILD | WS_VISIBLE | SBS_SIZEGRIP,  /* style */
		0,                /* x position */
		0,                /* y position */
		0,                /* width */
		0,                /* height */
		hWndParent,       /* parent */
		(HMENU) IDC_STATUSBAR, /* ID */
		g_hInst,          /* instance */
		NULL);            /* no extra data */

	GetClientRect(g_hwndStatusBar, &statusBarRect);

	/* Calculate the height of statusbar. */
	g_hStatus = statusBarRect.bottom - statusBarRect.top;
}


void CreateChildrenControls(HWND hWndParent)
{
	CreateToolbar(hWndParent);

	CreateStatusBar(hWndParent);

	CreateListView(hWndParent);

	PopulateListView();
}


void AdjustChildrenControls(HWND hWndParent)
{
	RECT rc;
	RECT toolbarRect;
	int tbX, tbY, tbW, tbH;
    int statusX, statusY, statusW, statusH;
	int lvX, lvY, lvW, lvH;

    GetClientRect(hWndParent, &rc);
	GetClientRect(g_hwndToolbar, &toolbarRect);

	tbX = 0;
	tbY = rc.top;
	tbW = rc.right - rc.left;
	tbH = toolbarRect.bottom - toolbarRect.top;

	statusX = 0;
	statusY = rc.bottom - g_hStatus;
	statusW = rc.right - rc.left;
	statusH = g_hStatus;

	lvX = rc.left;
    lvY = rc.top;
	lvW = rc.right - rc.left;
	lvH = rc.bottom - rc.top;

	/*
	 * If statusbar is visible, reduce the height of listview,
	 * otherwise it will take the whole are.
	 */
	if (IsWindowVisible(g_hwndStatusBar))
	{
		lvH = lvH - statusH;
	}

	if (IsWindowVisible(g_hwndToolbar))
	{
		lvY = lvY + tbH;
		lvH = lvH - tbH - 5;
	}

	MoveWindow(g_hwndToolbar,
		       tbX,
		       tbY,
		       tbW,
		       tbH,
		       TRUE);

	MoveWindow(g_hwndStatusBar,
		       statusX,
		       statusY,
		       statusW,
		       statusH,
		       TRUE);

	MoveWindow(g_hwndListView,
		       lvX,
			   lvY,
			   lvW,
			   lvH,
			   TRUE);
}


VOID SelectAllItems(VOID)
{
	int i;
	int iCount = ListView_GetItemCount(g_hwndListView);

	for (i = 0; i < iCount; i++)
	{
		ListView_SetItemState(g_hwndListView, i, LVIS_SELECTED, LVIS_SELECTED);
	}
}


INT_PTR CALLBACK fnNamePageProc(
	HWND hWndDlg,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	TCHAR szBuf[] = TEXT("Not implemented yet!");
	TCHAR szCaption[] = TEXT("Address Book");

	switch (uMsg)
	{
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code)
		{
		case PSN_APPLY: /* OK */
			MessageBox(hWndDlg, szBuf, szCaption, MB_ICONWARNING | MB_OK);
			break;
		case PSN_RESET: /* Cancel */
			break;
		}
		break; /* WM_NOTIFY */

	default:
		break;
	} /* switch */

	return FALSE;
}


static VOID ShowContactPropertiesDialog(HWND hWnd)
{
	PROPSHEETPAGE propSheetPage[1];
	PROPSHEETHEADER propSheetHeader;

	propSheetPage[0].dwSize = sizeof(PROPSHEETPAGE);
	propSheetPage[0].dwFlags = PSP_USETITLE;
	propSheetPage[0].hInstance = g_hInst;
	propSheetPage[0].pszTemplate = MAKEINTRESOURCE(IDD_PROPPAGE_NAME);
	propSheetPage[0].pszIcon = NULL;
	propSheetPage[0].pszTitle = TEXT("Name");
	propSheetPage[0].pfnDlgProc = fnNamePageProc;
	propSheetPage[0].lParam = 0;

	propSheetHeader.dwSize = sizeof(PROPSHEETHEADER);
	propSheetHeader.dwFlags = PSH_PROPSHEETPAGE | PSH_NOAPPLYNOW;
	propSheetHeader.hwndParent = hWnd;
	propSheetHeader.hInstance = g_hInst;
	propSheetHeader.pszIcon = NULL;
	propSheetHeader.pszCaption = TEXT("Properties");
	propSheetHeader.nPages = sizeof(propSheetPage) / sizeof(PROPSHEETPAGE);
	propSheetHeader.nStartPage = 0;
	propSheetHeader.ppsp = (LPCPROPSHEETPAGE) &propSheetPage;
	propSheetHeader.pfnCallback = NULL;

	PropertySheet(&propSheetHeader);
}


static void
UpdateMenuStates(
	void
	)
{
	/* TODO: implement me */
}


static void
HandleListViewNotifications(
	HWND hWnd,
	LPARAM lParam
	)
{
	NMLISTVIEW *nmListView = (NMLISTVIEW *) lParam;

	switch (nmListView->hdr.code)
	{
	case NM_CLICK:
		UpdateMenuStates();
		break;
	}
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
	PAINTSTRUCT ps;
	HDC hdc;

	switch (uMsg)
	{
		case WM_CREATE:
			CreateChildrenControls(hWnd);
			break;

		case WM_COMMAND:
			switch (GET_WM_COMMAND_ID(wParam, lParam))
			{
				case IDC_TB_NEW:
				case IDM_NEW_CONTACT:
					ShowContactPropertiesDialog(hWnd);
					break;

				case IDM_EXIT:
					SendMessage(hWnd, WM_CLOSE, 0, 0L);
					break;

				case IDM_SELECTALL:
					SelectAllItems();
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
					AdjustChildrenControls(hWnd);
					break;

				case IDM_ABOUT:
				   DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)AboutDlgProc);
				   break;
			}
			break;


		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			/* TODO: figure out why this is needed to repaint the listview */
			EndPaint(hWnd, &ps);
			break;


		case WM_SIZE:
			AdjustChildrenControls(hWnd);
			break;


		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;


		case WM_DESTROY:
			PostQuitMessage(0);
			break;


		case WM_NOTIFY:
			switch (wParam)
			{
			case IDC_LISTVIEW:
				HandleListViewNotifications(hWnd, lParam);
				break;
			}
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
