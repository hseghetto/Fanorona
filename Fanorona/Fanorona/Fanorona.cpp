// Fanorona.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <gl/glut.h>
#include <list>
#include "Piece.h"
using namespace std;

int BOARD[9][5]{};

list<Piece> WHITES, BLACKS, MOVE, CAPTURE;

void init_board() {
    WHITES.clear();
    BLACKS.clear();

    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 2; j++) {
            BOARD[i][j] = -1;
            Piece p(i, j, -1);
            BLACKS.push_back(p);
        }
    }

    for (i = 0; i < 9; i++) {
        for (j = 3; j < 5; j++) {
            BOARD[i][j] = 1;
            Piece p(i, j, 1);
            WHITES.push_back(p);
        }
    }

    BOARD[4][2] = 0;
    for (i = 0; i <= 8; i = i + 2) {
        if (i == 4)
            i++;
        BOARD[i][2] = -1;
        Piece p(i, 2, -1);
        BLACKS.push_back(p);
    }
    for (i = 1; i <= 8; i = i + 2) {
        if (i == 5)
            i++;
        BOARD[i][2] = 1;
        Piece p(i, 2, 1);
        WHITES.push_back(p);
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
    if (p == 0) {
        return;
    }

    int r = 5;

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
        i = k % 3 - 1;
        j = k / 2 - 1;

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
            i = 1 - k / 2 * 2;
            j = 1 - k % 2 * 2;

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

void percorre_campo() {
    for (int i = 0; i <= 8; i++) {
        for (int j = 0; j <= 4; j++) {
            cout << i << "x" << j << ":" << checa_movimento(i, j, BOARD[i][j]) << "\n";
        }
    }
}

int x_s = -1, y_s = -1;
void botao_mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            cout << "Botao esquerdo " << x << "," << y << "\n";
            int er_x, er_y, error, p;
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 5; j++) {
                    er_x = abs(100 + 50 * i - x);
                    er_y = abs(100 + 50 * j - y);
                    error = er_x * er_x + er_y * er_y;

                    if (error < 49) {
                        cout << i << "x" << j << "\n";
                        int k = checa_movimento(i, j, BOARD[i][j]);
                        if (k <= 0)
                            cout << "nao" << "\n";
                        return;

                        if (x_s >= 0) {
                            p = BOARD[x_s][y_s];
                            BOARD[x_s][y_s] = BOARD[i][j];
                            BOARD[i][j] = p;
                            x_s = -1;
                            y_s = -1;
                            cout << BOARD[0][0];
                        }
                        else {
                            x_s = i;
                            y_s = j;
                        }
                        return;
                    }
                }
            }
        }
    }
}

int main(int argc, char** argv)
{
    cout << "Hello World!\n";

    cout << WHITES.size() << BLACKS.size() << "\n";
    init_board();
    cout << WHITES.size() << BLACKS.size() << "\n";
    init_board();
    cout << WHITES.size() << BLACKS.size() << "\n";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0, 0);

    glutCreateWindow("Ponto");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(botao_mouse);
    glutMainLoop();
}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
