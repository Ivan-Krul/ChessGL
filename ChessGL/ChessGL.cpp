#include <windows.h>
#include <gl/GL.h>
#include <string>
#include "Painter.h"

#pragma comment(lib, "opengl32.lib")

#define WINDOW_X 800
#define WINDOW_Y 800
#define MAP_X 8
#define MAP_Y 8

using std::string;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

char Board[MAP_X][MAP_Y];

int KQkq = 0;
bool isTurnWhite = true;
bool isPlayWhite = true;
int turn = 0;

void ScreenToGL(HWND hwnd, int x, int y, float* ox, float* oy) {
	RECT rct;
	GetClientRect(hwnd, &rct);
	*ox = x / float(rct.right) * MAP_X;
	*oy = MAP_Y - y / float(rct.bottom) * MAP_Y;
}

bool IsInMap(int x, int y) {
	return (x >= 0) && (x < MAP_X) && (y >= 0) && (y < MAP_Y);
}

void ResetChess() {
	for (int i = 0;i < MAP_Y;i++) {
		for (int j = 0;j < MAP_X;j++) {
			if (i == 0) {
				switch (j) {
				case 0:
				case MAP_X - 1:
					Board[j][i] = 'R';
					break;
				case 1:
				case MAP_X - 2:
					Board[j][i] = 'N';
					break;
				case 2:
				case MAP_X - 3:
					Board[j][i] = 'B';
					break;
				case 3:
					Board[j][i] = 'Q';
					break;
				case MAP_X - 4:
					Board[j][i] = 'K';
					break;
				}
			}
			else if (i == MAP_Y - 1) {
				switch (j) {
				case 0:
				case MAP_X - 1:
					Board[j][i] = 'r';
					break;
				case 1:
				case MAP_X - 2:
					Board[j][i] = 'n';
					break;
				case 2:
				case MAP_X - 3:
					Board[j][i] = 'b';
					break;
				case 3:
					Board[j][i] = 'q';
					break;
				case MAP_X - 4:
					Board[j][i] = 'k';
					break;
				}
			}
			else if (i == MAP_X - 2)
				Board[j][i] = 'p';
			else if(i == 1)
				Board[j][i] = 'P';
			else Board[j][i] = ' ';
		}
	}
}

void GameBegin() {
	ResetChess();
}

void Paint() {
	glLoadIdentity();
	if (isPlayWhite) glTranslated(-1, -1, 0);
	else glTranslated(1, 1, 0);
	glScaled(2.0 / MAP_X, 2.0 / MAP_Y, 0);
	if (!isPlayWhite) glRotated(180, 0, 0, 1);
	
	for (int i = 0;i < MAP_Y;i++) {
		for (int j = 0;j < MAP_X;j++) {
			glPushMatrix();
			if (!isPlayWhite) glTranslated(i + 1, j + 1, 0);
			else glTranslated(i, j, 0);
			if (!isPlayWhite) glRotated(180, 0, 0, 1);
			DrawBoard((i + j) % 2);

			switch (Board[i][j])
			{
			case 'P':
				DrawPawn(true);
				break;
			case 'p':
				DrawPawn(false);
				break;
			case 'R':
				DrawRook(true);
				break;
			case 'r':
				DrawRook(false);
				break;
			case 'N':
				DrawKnight(true);
				break;
			case 'n':
				DrawKnight(false);
				break;
			case 'B':
				DrawBishop(true);
				break;
			case 'b':
				DrawBishop(false);
				break;
			case 'Q':
				DrawQueen(true);
				break;
			case 'q':
				DrawQueen(false);
				break;
			case 'K':
				DrawKing(true);
				break;
			case 'k':
				DrawKing(false);
				break;
			}

			glPopMatrix();
		}
	}
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;
	HWND hwnd;
	HDC hDC;
	HGLRC hRC;
	MSG msg;
	BOOL bQuit = FALSE;
	float theta = 0.0f;

	bool is_pause = false;

	/* register window class */
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"GLSample";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


	if (!RegisterClassEx(&wcex))
		return 0;

	/* create main window */
	hwnd = CreateWindowEx(0,
		L"GLSample",
		L"ChessGL",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_X,
		WINDOW_Y,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, nCmdShow);

	/* enable OpenGL for the window */
	EnableOpenGL(hwnd, &hDC, &hRC);

	GameBegin();

	/* program main loop */
	while (!bQuit)
	{

		/* check for messages */
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			/* handle or dispatch messages */
			if (msg.message == WM_QUIT)
			{
				bQuit = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (GetAsyncKeyState(' ')) { is_pause = !is_pause; Sleep(100); }

			/* OpenGL animation code goes here */
			if (!is_pause) {

				glClearColor(0, 0, 0, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				Paint();

				SwapBuffers(hDC);

				theta += 1.0f;
			}
			Sleep(5);
		}
	}

	/* shutdown OpenGL */
	DisableOpenGL(hwnd, hDC, hRC);

	/* destroy the window explicitly */
	DestroyWindow(hwnd);

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_DESTROY:
		return 0;

	case WM_LBUTTONDOWN:
	{
		POINTFLOAT pf;
		ScreenToGL(hwnd, LOWORD(lParam), HIWORD(lParam), &pf.x, &pf.y);
		int xx = int(pf.x);
		int yy = int(pf.y);
		if (!isPlayWhite) {
			xx = MAP_X - xx - 1;
			yy = MAP_Y - yy - 1;
		}

		Board[xx][yy] = ' ';
	}
		break;

	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
	}
	break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
	PIXELFORMATDESCRIPTOR pfd;

	int iFormat;

	/* get the device context (DC) */
	*hDC = GetDC(hwnd);

	/* set the pixel format for the DC */
	ZeroMemory(&pfd, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	iFormat = ChoosePixelFormat(*hDC, &pfd);

	SetPixelFormat(*hDC, iFormat, &pfd);

	/* create and enable the render context (RC) */
	*hRC = wglCreateContext(*hDC);

	wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hwnd, hDC);
}
