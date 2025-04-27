#include <GL/glut.h>
#include <iostream>

#include "Ambiente.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int FPS = 60;
const int DELAY = 1000 / FPS;
const int COORDINATES_X = 100;
const int COORDINATES_Y = 100;
const int COORDINATES_Z = 100;

void init();
void display();

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Jogo do Pinquim");


    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

void init(void)
{
    glClearColor(1, 1, 1 , 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-COORDINATES_X, COORDINATES_X, -COORDINATES_Y,
        COORDINATES_Y, -COORDINATES_Z, COORDINATES_Z);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Ambiente ambiente(COORDINATES_X, COORDINATES_Y);
    ambiente.desenhar();

    glutSwapBuffers();
}