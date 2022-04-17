#include <windows.h>
#include <gl/GL.h>
#include <string>
#include "Painter.h"

#pragma comment(lib, "opengl32.lib")

#define WINDOW_X 700
#define WINDOW_Y 700
#define MAP_X 8
#define MAP_Y 8

using std::string;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

char Board[MAP_X][MAP_Y];

unsigned short PawnBlitz = 0b0000000000000000; // 10101000 00000000 // 1 - pawn made a long length first move, 0 - any else, or nothing movement by the pawn
unsigned char Rocking = 0b11111111; // 11111111 // 11 - can made rocking on any zone, if king move, white lost access of rocking of all variation, but if one of the rook will move, then one of sizes will lost access of rocking
bool isTurnWhite = true;
bool isPlayWhite = true;
int turn = 0;

int xx;
int yy;

struct Cursor {
	int x1, y1, x2, y2;
	bool isMove = false;
} cursor;

void ScreenToGL(HWND hwnd, int x, int y, float* ox, float* oy) {
	RECT rct;
	GetClientRect(hwnd, &rct);
	*ox = x / float(rct.right) * MAP_X;
	*oy = MAP_Y - y / float(rct.bottom) * MAP_Y;
}

int IsUpper(char cha) {
	if (65 <= int(cha) && int(cha) <= 90) // A, B, C, ...
		return 2;
	else if (97 <= int(cha) && int(cha) <= 122) // a, b, c, ...
		return 1;
	else return 0;
}

bool IsInMap(int x, int y) {
	return (x >= 0) && (x < MAP_X) && (y >= 0) && (y < MAP_Y);
}

bool IsYourTurn(int x, int y) {
	return (isTurnWhite && IsUpper(Board[x][y]) == 2) || (!isTurnWhite && IsUpper(Board[x][y]) == 1);
}

void MoveCursor() {
	if (IsInMap(xx, yy)) {
		if (!cursor.isMove) {
			if (Board[xx][yy] != ' ' && IsYourTurn(xx,yy)) {
				cursor.isMove = true;
				cursor.x1 = xx;
				cursor.y1 = yy;
			}
		}
		else {
			if (cursor.x1 == xx && cursor.y1 == yy) cursor.isMove = false;
			else if (IsYourTurn(xx, yy)){
				cursor.x1 = xx;
				cursor.y1 = yy;
			}
			else{
				cursor.x2 = xx;
				cursor.y2 = yy;
				
				if (Board[cursor.x1][cursor.y1] == 'K') {
					if (Rocking / 0b11110000 != 0b0000) {
						if ((Rocking / 0b11110000) % 0b1100 != 0b00)Rocking -= 0b00110000;
						if ((Rocking / 0b11110000) / 0b1100 != 0b00)Rocking -= 0b11000000;
					}
				}
				else if (Board[cursor.x1][cursor.y1] == 'k') {
					if (Rocking % 0b11110000 != 0b0000) {
						if ((Rocking % 0b11110000) % 0b1100 != 0b00)Rocking -= 0b00000011;
						if ((Rocking % 0b11110000) / 0b1100 != 0b00)Rocking -= 0b00001100;
					}
				}

				if (Board[cursor.x1][cursor.y1] == 'P' && cursor.y1 + 2 == cursor.y2) {
					
				}
				else if (Board[cursor.x1][cursor.y1] == 'p' && cursor.y1 - 2 == cursor.y2) {

				}

				Board[cursor.x2][cursor.y2] = Board[cursor.x1][cursor.y1];
				Board[cursor.x1][cursor.y1] = ' ';
				
				if(!isTurnWhite) turn++;
				cursor.isMove = false;
				isTurnWhite = !isTurnWhite;
			}

		}
	}
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
	turn = 0;
	isTurnWhite = true;
	PawnBlitz = 0b0000000000000000;
	Rocking = 0b11111111;
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
			if (j == yy && i == xx) DrawSelectedBoard((i + j) % 2);
			else DrawBoard((i + j) % 2);

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

				std::wstring title = L"ChessGL turn - " + std::to_wstring(turn + 1) + L" " + (isTurnWhite ? L"white" : L"black");
				SetWindowText(hwnd, title.c_str());

				glPointSize(50);
				glBegin(GL_POINTS);
				if(cursor.isMove)glColor3d(0, 0, 1);
				else glColor3d(0, 1, 0);
				glVertex2d(xx, yy);
				glEnd();

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
		xx = int(pf.x);
		yy = int(pf.y);

		if (!isPlayWhite) {
			xx = MAP_X - xx - 1;
			yy = MAP_Y - yy - 1;

		}

		MoveCursor();

		//Board[xx][yy] = ' ';
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
