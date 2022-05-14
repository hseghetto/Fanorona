#pragma once
class Piece
{
private:
	int px, py, player;
public:
	Piece(int x, int y, int p) {
		px = x; py = y; player = p;
	}
	void draw_circle(double x, double y, double r);
	void display_piece(int x, int y, int p);


};
