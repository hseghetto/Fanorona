#include <iostream>
#include <gl/glut.h>
#include <stdlib.h>
#include <list>
#include "Piece.h"
using namespace std;

int BOARD[9][5]{};

int _x_s = -1, _y_s = -1, _player = 1, _pc = -1, _captura = -1;
int _cx1 = -1, _cy1 = -1, _cx2 = -1, _cy2 = -1;

// initial window screen size
int WIDTH = 800;
int HEIGHT = 800;

void init_board() {
    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 2; j++) {
            BOARD[i][j] = -1;
        }
    }

    for (i = 0; i < 9; i++) {
        for (j = 3; j < 5; j++) {
            BOARD[i][j] = 1;
        }
    }

    BOARD[4][2] = 0;
    for (i = 0; i <= 8; i = i + 2) {
        if (i == 4)
            i++;
        BOARD[i][2] = -1;
    }
    for (i = 1; i <= 8; i = i + 2) {
        if (i == 5)
            i++;
        BOARD[i][2] = 1;
    }


}

void init() {
    glClearColor(1, 1, 1, 1);

    glLoadIdentity();

    gluOrtho2D(0, 800, 800, 0);

}

void draw_circle(double x, double y, double r) {
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

void display_piece(int x, int y, int p) {
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

void display_board() {
    glColor3f(0, 0, 0);

    for (int i = 0; i < 9; i++) {
        glBegin(GL_LINES);
        glVertex2f(100 + 50 * i, 100);
        glVertex2f(100 + 50 * i, 300);
        glEnd();
    }

    for (int i = 0; i < 5; i++) {
        glBegin(GL_LINES);
        glVertex2f(100, 100 + 50 * i);
        glVertex2f(500, 100 + 50 * i);
        glEnd();
    }

    glBegin(GL_LINE_STRIP);
    glVertex2f(100, 100);
    glVertex2f(300, 300);
    glVertex2f(500, 100);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2f(100, 300);
    glVertex2f(300, 100);
    glVertex2f(500, 300);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2f(100, 200);
    glVertex2f(200, 100);
    glVertex2f(400, 300);
    glVertex2f(500, 200);
    glVertex2f(400, 100);
    glVertex2f(200, 300);
    glVertex2f(100, 200);
    glEnd();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 5; j++) {
            int x = 100 + 50 * i;
            int y = 100 + 50 * j;

            display_piece(x, y, BOARD[i][j]);
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.0, 0);
    glPointSize(3.0);

    display_board();

    glutSwapBuffers();
}

bool checa(int x, int y) {
    return x >= 0 && y >= 0 && x <= 8 && y <= 4;
}

int checa_movimento(int x, int y, int p) {
    int m = 0;
    int c = 0;

    if (p == 0) {
        return -1;
    }

    int i, j;
    for (int k = 1; k <= 4; k++) {
        i = k % 3 - 1; //  0  1 -1  0
        j = k / 2 - 1; // -1  0  0  1

        if (checa(x + i, y + j)) {
            if (BOARD[x + i][y + j] == 0) {
                m = 1;

                if (checa(x + i * 2, y + j * 2))
                    if (BOARD[x + i * 2][y + j * 2] == -p)
                        c = 1;

                if (checa(x - i, y - j))
                    if (BOARD[x - i][y - j] == -p)
                        c = 1;
            }
        }
    }

    if ((x + y) % 2 == 0)
        for (int k = 0; k < 4; k++) {
            i = 1 - k / 2 * 2; // 1  1 -1 -1
            j = 1 - k % 2 * 2; // 1 -1  1 -1

            if (checa(x + i, y + j)) {
                if (BOARD[x + i][y + j] == 0) {
                    m = 1;

                    if (checa(x + i * 2, y + j * 2))
                        if (BOARD[x + i * 2][y + j * 2] == -p)
                            c = 1;

                    if (checa(x - i, y - j))
                        if (BOARD[x - i][y - j] == -p)
                            c = 1;
                }
            }
        }

    return c * 10 + m;
}

void movimenta(int i, int j) {
    int p = BOARD[i][j];
    int k = checa_movimento(i, j, p);

    if (_player == p && k != 0 && _captura == -1) {
        _x_s = i; //salva i e j caso a peça seja valida
        _y_s = j;
        return;
    }

    int flag = 0, flag2 = 0;
    if (_captura == 2) {
        if (_cx1 == i && _cy1 == j) {
            flag2 = -1;
            i = (_cx1 + _x_s) / 2;
            j = (_cy1 + _y_s) / 2;
            p = 0;
            _cx1 = -1;
            _cy1 = -1;
            _cx2 = -1;
            _cy2 = -1;
            cout << _x_s << " " << i << " " << _cx1 << " x's\n";
        }
        else if (_cx2 == i && _cy2 == j) {
            flag = -1;
            i = (_cx1 + _x_s) / 2;
            j = (_cy1 + _y_s) / 2;
            p = 0;
            _cx1 = -1;
            _cy1 = -1;
            _cx2 = -1;
            _cy2 = -1;
            cout << _x_s << " " << i << " " << _cx1 << " x's\n";
        }
        else {
            cout << "selecione uma captura \n";
            return;
        }
    }

    if (_x_s >= 0) { //movimento da peça selecionada
        int dx = i - _x_s;
        int dy = j - _y_s;

        if (p != 0 || abs(dx) > 1 || abs(dy) > 1) { //verificando se movimento é valido - posicao vazia e proxima
            cout << "nao \n";
            return;
        }
        if ((i + j) % 2 == 1 && abs(dx) + abs(dy) == 2) { //verificando se movimento é valido - movimento diagonal
            cout << "nao \n";
            return;
        }

        if (checa(i + dx, j + dy) && checa(i - 2 * dx, j - 2 * dy) && _captura != 2) {
            cout << BOARD[i + dx][j + dy] << " " << BOARD[i - 2 * dx][j - 2 * dy] << " " << _player;
            cout << "chacando captura dupla \n";
            
            if (BOARD[i + dx][j + dy] == BOARD[i - 2 * dx][j - 2 * dy] && _player == -BOARD[i + dx][j + dy]) {
                _captura = 2;
                _cx1 = i + dx;
                _cy1 = j + dy;
                _cx2 = i - 2 * dx;
                _cy2 = j - 2 * dy;
                return;
            }
        }

        BOARD[i][j] = BOARD[_x_s][_y_s]; // trocando a peça de lugar
        BOARD[_x_s][_y_s] = 0; // trocando a peça de lugar

        // 1=houve captura, 0=não houve captura
        while (flag >= 0) { //laço de captura em avanço
            i = i + dx;
            j = j + dy;

            if (checa(i, j)) {
                if (BOARD[i][j] == -_player) {
                    cout << i << " " << j << " " << dx << " " << dy << " " << endl;
                    BOARD[i][j] = 0;
                    flag++;
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }

        i = _x_s;
        j = _y_s;
        // 1=houve captura, 0=não houve captura
        while (flag2 >= 0) { //laço de captura em recuo
            i = i - dx;
            j = j - dy;

            if (checa(i, j)) {
                if (BOARD[i][j] == -_player) {
                    cout << i << " " << j << " " << dx << " " << dy << " " << endl;
                    BOARD[i][j] = 0;
                    flag2++;
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }

        if (flag > 0 || flag2 > 0) {//Flag > 0, houve captura
            _x_s = _x_s + dx;
            _y_s = _y_s + dy;
            if (checa_movimento(_x_s, _y_s, _player) > 1) { //existe captura em sequencia
                _captura = 1;
                return;
            }
        }

        _captura = -1;
        _player = -_player;
        _x_s = -1;
        _y_s = -1;
    }
    return;
}

void movimento_aleatorio() { //movimento p computador
    int c = 0;
    list<int> cx,cy,mx,my;

    for (int i = 0; i <= 8; i++) { 
        for (int j = 0; j <= 4; j++) {
            c = checa_movimento(i, j, BOARD[i][j]);

            if (c == 1) { //listando peças com captura
                mx.push_back(i);
                my.push_back(j);
            }
            if (c > 1) { //listando peças com movimento
                cx.push_back(i);
                cy.push_back(j);
            }
        }
    }

    int cs = cx.size(), ms = mx.size(), r;

    if (cs) { //tenta capturar
        r = rand() % cs;

        auto cx_front = cx.begin();
        advance(cx_front, r);

        auto cy_front = cy.begin();
        advance(cy_front, r);

        movimenta(*cx_front, *cy_front);
        //*cx_front posicao x, * cy_front posicao y
        //escolhe o movimento
        //movimenta(x, y);

        return;
    }
    if (ms) { //tenta mover
        r = rand() % ms;

        auto mx_front = mx.begin();
        advance(mx_front, r);

        auto my_front = my.begin();
        advance(my_front, r);

        movimenta(*mx_front, *my_front);
        //*cx_front posicao x, * cy_front posicao y
        //escolhe o movimento
        //movimenta(x, y);
        return;
    }
}

void botao_mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (_player == _pc) {
            movimento_aleatorio();
            return;
        }

        if (state == GLUT_DOWN) {
            cout << "Botao esquerdo " << x << "," << y << "\n";
            float er_x, er_y, error;

            float di = (float)x;
            float dj = (float)y;

            cout << "" << di << "," << dj << "\n";

            float scale = min((float)glutGet(GLUT_WINDOW_WIDTH) / WIDTH, (float)glutGet(GLUT_WINDOW_HEIGHT) / HEIGHT);
            float delta = 50.0 * scale;

            di = (di - 100.0 * scale) / delta;
            dj = (dj - 100.0 * scale) / delta;

            cout << "" << di << "," << dj << "; " << delta << "\n";

            int i = round(di);
            int j = round(dj);

            if (!checa(i, j)) {
                cout << "out of bounds \n";
                return;
            }

            er_x = abs(i - di);
            er_y = abs(j - dj);
            error = er_x * er_x + er_y * er_y;

            if (error < 0.02) { 
                cout << i << "x" << j << "\n";
                movimenta(i, j);
            }
        }
    }
}

void reshape(int width, int height) {
    float scale = min((float)glutGet(GLUT_WINDOW_WIDTH)/WIDTH, (float)glutGet(GLUT_WINDOW_HEIGHT)/HEIGHT);

    cout << scale << endl;

    float YMARGIN = (height - HEIGHT * scale);

    glViewport(0, YMARGIN, WIDTH * scale, HEIGHT * scale);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

}

int main(int argc, char** argv)
{
    cout << "Hello World!\n";

    //cout << "Deseja jogar contra o compoutador? (1=PC joga primeiro, -1 = PC joga dps, 0=PC não joga)" << endl;
    //cin >> _pc;

    init_board();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0, -200);

    glutCreateWindow("Fanorona");
    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(botao_mouse);
    glutMainLoop();
}