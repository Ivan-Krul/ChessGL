#pragma once
#include <gl/GL.h>

#pragma comment(lib, "opengl32.lib")

const int colorPieceW = 0xdddddd;
const int colorPieceB = 0x222222;
const int colorBoardW = 0xe8a778;
const int colorBoardB = 0x6c3914;

void SetDraw(bool isWhite) {
	glBegin(GL_TRIANGLE_STRIP);
	int W = colorPieceW;
	int B = colorPieceB;

	if (isWhite) glColor3b((W / 256 / 256) / 2, (W / 256 % 256) / 2, (W % 256) / 2);
	else glColor3b((B / 256 / 256) / 2, (B / 256 % 256) / 2, (B % 256) / 2);
}

void DrawPawn(bool isWhite) {
	SetDraw(isWhite);

	glVertex2d(0.5, 0.8);
	glVertex2d(0.2, 0.2);
	glVertex2d(0.8, 0.2);

	glEnd();
}

void DrawRook(bool isWhite) {
	SetDraw(isWhite);

	glVertex2d(0.35, 0.2);
	glVertex2d(0.65, 0.2);
	glVertex2d(0.35, 0.8);
	glVertex2d(0.65, 0.8);

	glEnd();
}

void DrawKnight(bool isWhite) {
	SetDraw(isWhite);

	glVertex2d(0.1, 0.7);
	glVertex2d(0.5, 0.9);
	glVertex2d(0.9, 0.5);
	glVertex2d(0.5, 0.6);
	glVertex2d(0.2, 0.2);
	glVertex2d(0.9, 0.5);
	glVertex2d(0.9, 0.2);

	glEnd();
}

void DrawBishop(bool isWhite) {
	SetDraw(isWhite);

	glVertex2d(0.5, 0.9);
	glVertex2d(0.3, 0.7);
	glVertex2d(0.7, 0.7);
	glVertex2d(0.5, 0.5);
	glVertex2d(0.5, 0.5);
	glVertex2d(0.8, 0.2);
	glVertex2d(0.2, 0.2);

	glEnd();
}

void DrawQueen(bool isWhite) {
	SetDraw(isWhite);

	glVertex2d(0.5, 0.9);
	glVertex2d(0.3, 0.8);
	glVertex2d(0.7, 0.8);
	glVertex2d(0.5, 0.5);
	glVertex2d(0.5, 0.6);
	glVertex2d(0.8, 0.2);
	glVertex2d(0.2, 0.2);

	glEnd();
}

void DrawKing(bool isWhite) {
	SetDraw(isWhite);

	glVertex2d(0.8, 0.2);//4
	glVertex2d(0.2, 0.2);//5
	glVertex2d(0.5, 0.7);//6
	glVertex2d(0.5, 0.7);//6
	glVertex2d(0.3, 0.7);//1
	glVertex2d(0.3, 0.9);//2
	glVertex2d(0.7, 0.7);//3
	glVertex2d(0.7, 0.9);//7

	glEnd();
}

void DrawBoard(bool isWhite) {
	glBegin(GL_TRIANGLE_STRIP);
	int W = colorBoardW;
	int B = colorBoardB;

	if (isWhite) glColor3b((W / 256 / 256) / 2, (W / 256 % 256) / 2, (W % 256) / 2);
	else glColor3b((B / 256 / 256) / 2, (B / 256 % 256) / 2, (B % 256) / 2);

	glVertex2d(0, 0);
	glVertex2d(1, 0);
	glVertex2d(0, 1);
	glVertex2d(1, 1);
	glEnd();
}

void DrawSelectedBoard(bool isWhite) {
	glBegin(GL_TRIANGLE_STRIP);
	int W = colorBoardW;
	int B = colorBoardB;

	if (isWhite) glColor3b((W / 256 / 256) / 2, (W / 256 % 256 - 120) / 2, (W % 256 - 120) / 2);
	else glColor3b((W / 256 / 256) / 2, (B / 256 % 256 - 120) / 2, (B % 256 - 120) / 2);

	glVertex2d(0, 0);
	glVertex2d(1, 0);
	glVertex2d(0, 1);
	glVertex2d(1, 1);
	glEnd();
}
