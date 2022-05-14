#include "Piece.h"
#include "Global.h"
#include <gl/glut.h>
#include <iostream>

void Piece::draw_circle(double x, double y, double r) {
	int numTriangles = 30;
	double delta = 2 * 3.1415 / numTriangles;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);
	for (int i = 0; i <= numTriangles; i++) {
		glVertex2f(
			x + (r * cos(i * delta)),
			y + (r * sin(i * delta))
		);
	}
	glEnd();
}
void Piece::display_piece(int x, int y, int p) {
	int i = (x - 100) / 50;
	int j = (y - 100) / 50;
	int r = 5;

	if (p == 0) { //jogadas possiveis
		if (abs(i - _x_s) <= 1 && abs(j - _y_s) <= 1 && _x_s >= 0) {
			if ((_x_s + _y_s) % 2 == 0 || abs(i - _x_s) + abs(j - _y_s) == 1) {
				glColor3f(1, 0.75, 0);
				draw_circle(x, y, r);
			}
		}
		return;
	}

	if (i == _x_s && j == _y_s) { //peça selecionada
		glColor3f(0, 0.5, 1);
		draw_circle(x, y, r + 2);
	}

	if (i == _cx1 && j == _cy1) { //captura ambigua
		glColor3f(1, 0, 0);
		draw_circle(x, y, r + 2);
	}

	if (i == _cx2 && j == _cy2) { //captura ambigua
		glColor3f(1, 0, 0);
		draw_circle(x, y, r + 2);
	}

	if (p == -1) {
		glColor3f(0, 0, 0);
		draw_circle(x, y, r);
	}
	if (p == 1) {
		glColor3f(0, 0, 0);
		draw_circle(x, y, r);

		glColor3f(1, 1, 1);
		draw_circle(x, y, r - 1);
	}
}
