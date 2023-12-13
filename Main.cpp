//*****************************************************************************
//
// Main.cpp : Defines the entry point for the application.
// Creates a white RGB image with a black line and displays it.
// Two images are displayed on the screen.
// Left Pane: Input Image, Right Pane: Modified Image
//
// Author - Parag Havaldar
// Code used by students as a starter code to display and modify images
//
//*****************************************************************************


// Include class files
#include "Image.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
MyImage			inImage;						// image objects
HINSTANCE		hInst;							// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

vector <pair< vector<unordered_set<int>>, string> > pixelsToDraw;
// Main entry point for a windows application
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// Create a separate console window to display output to stdout
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	// The rest of command line argument is truncated.
	// If you want to use it, please modify the code.
	if (lpCmdLine[0] == 0) {
		wprintf(L"No command line arguement.");
		return -1;
	}
	
	vector<string> args;
	istringstream iss(lpCmdLine);
	string arg;

	while (iss >> arg) {
		args.push_back(arg);
	}

	/*for(int i=0;i< args.size();i++)
		cout<< args[i]<<" ";*/
	int w = 640;
	int h = 480;
	inImage.setWidth(w);
	inImage.setHeight(h);

	LPSTR imgSrc = const_cast<LPSTR>(args[0].c_str());
	inImage.setImagePath(imgSrc);
	inImage.ReadImage();
	pixelsToDraw = inImage.Modify(args);
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IMAGE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_IMAGE);

	// Main message loop:
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

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_IMAGE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_IMAGE;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}


//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 660, 680, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


void DrawBoundingBox(HWND hWnd) {
	//vector <pair< vector<unordered_set<int>>, string> > pixelsToDraw

	HDC hdc = GetDC(hWnd);
	HPEN hGreyPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); // RGB values for grey color
	HPEN hOldPen = (HPEN)SelectObject(hdc, hGreyPen); // Select the grey pen into the device context and save the old pen

	for (int clust = 0; clust < pixelsToDraw.size(); clust++) {
		cout << "clust: " << clust << endl;

		for (const auto& pixels : pixelsToDraw[clust].first) {
			cout << "in" << endl;

			// Step 1: Compute the bounding box
			int minX = INT_MAX;
			int minY = INT_MAX;
			int maxX = INT_MIN;
			int maxY = INT_MIN;
			for (auto linearIndex : pixels) {
				int x = linearIndex % 640;
				int y = linearIndex / 640;

				if (x < minX) minX = x;
				if (x > maxX) maxX = x;
				if (y < minY) minY = y;
				if (y > maxY) maxY = y;
			}
			// Step 2: Adjust the bounding box to fit within the window
			if (minX < 0) minX = 0;
			if (minY < 0) minY = 0;
			if (maxX >= 640)  maxX = 640 - 1;
			if (maxY >= 480) maxY = 480 - 1;

			int offsetY = 100;
			minY += offsetY;
			maxY += offsetY;
			// Step 3: Draw the bounding box

			HPEN hGreyPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); // RGB values for grey color
			HPEN hOldPen = (HPEN)SelectObject(hdc, hGreyPen); // Select the grey pen into the device context and save the old pen
			int thickness = 3;

			for (int t = 0; t < thickness; t++) {
				// Adjust the bounding box's coordinates for thickness
				int thickMinX = max(0, minX - t);
				int thickMinY = max(100, minY - t); // Ensure it's not above the image
				int thickMaxX = min(640 - 1, maxX + t);
				int thickMaxY = min(480 - 1 + 100, maxY + t); // Ensure it's not below the image

				// Draw the thick bounding box
				MoveToEx(hdc, thickMinX, thickMinY, NULL);
				LineTo(hdc, thickMaxX, thickMinY);
				LineTo(hdc, thickMaxX, thickMaxY);
				LineTo(hdc, thickMinX, thickMaxY);
				LineTo(hdc, thickMinX, thickMinY);
			}

			string toDraw = pixelsToDraw[clust].second;
			size_t pos = toDraw.find_last_of("/\\");
			toDraw = (pos != string::npos) ? toDraw.substr(pos + 1) : toDraw;

			SetTextColor(hdc, RGB(0, 0, 0));
			SetBkMode(hdc, TRANSPARENT);
			SIZE textSize;
			GetTextExtentPoint32(hdc, toDraw.c_str(), toDraw.length(), &textSize);

			int textStartX = minX + ((maxX - minX - textSize.cx) / 2);  // Centered

			// Check if text fits within bounding box
			if (textSize.cx < (maxX - minX)) {
				// Position the text just above the bottom line of the bounding box.
				int textStartY = maxY - textSize.cy;
				TextOut(hdc, textStartX, textStartY, toDraw.c_str(), toDraw.length());
			}
			else {
				// Text doesn't fit inside bounding box, so place it below but ensure it doesn't go outside the window
				int textStartY = maxY + 5;  // Some padding below the bounding box
				if (textStartY + textSize.cy > 480) {  // Make sure text doesn't go outside the window
					textStartY = 480 - textSize.cy;
				}
				TextOut(hdc, textStartX, textStartY, toDraw.c_str(), toDraw.length());
			}

		}
		}

		

	SelectObject(hdc, hOldPen); // Restore the original pen
	DeleteObject(hGreyPen); // Delete the created pen to avoid resource leak
	ReleaseDC(hWnd, hdc); // Release the device context

}




//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
// TO DO: part useful to render video frames, may place your own code here in this function
// You are free to change the following code in any way in order to display the video

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	RECT rt;
	GetClientRect(hWnd, &rt);

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case ID_MODIFY_IMAGE:
				   InvalidateRect(hWnd, &rt, false); 
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			{
				hdc = BeginPaint(hWnd, &ps);
				// TO DO: Add any drawing code here...

				BITMAPINFO bmi;
				CBitmap bitmap;
				memset(&bmi,0,sizeof(bmi));
				bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
				bmi.bmiHeader.biWidth = inImage.getWidth();
				bmi.bmiHeader.biHeight = -inImage.getHeight();  // Use negative height.  DIB is top-down.
				bmi.bmiHeader.biPlanes = 1;
				bmi.bmiHeader.biBitCount = 24;
				bmi.bmiHeader.biCompression = BI_RGB;
				bmi.bmiHeader.biSizeImage = inImage.getWidth()*inImage.getHeight();

				SetDIBitsToDevice(hdc,
								  0,100,inImage.getWidth(),inImage.getHeight(),
								  0,0,0,inImage.getHeight(),
								  inImage.getImageData(),&bmi,DIB_RGB_COLORS);

				DrawBoundingBox(hWnd);
				EndPaint(hWnd, &ps);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}




// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
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


