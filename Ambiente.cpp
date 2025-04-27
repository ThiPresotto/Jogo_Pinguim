#include "Ambiente.h"

#include <GL/glut.h>

Ambiente::Ambiente(double coordenadaXTela, double coordenadaYTela)
    : coordenadaXTela(coordenadaXTela)
    , coordenadaYTela(coordenadaYTela)
    , corCeu(0.53, 0.81, 0.92) // Azul claro
    , corMar(0.0, 0.749, 1.0) // Azul
    , corTerra(0.5, 1, 0) // Verde Paris
{
}

void Ambiente::desenhar()
{
    desenharCeu();
    desenharTerra();
    desenharMar();
}

void Ambiente::desenharCeu()
{
    glBegin(GL_POLYGON);
        glColor3f(corCeu.r, corCeu.g, corCeu.b);
        glVertex2f(-coordenadaXTela, coordenadaYTela);
        glVertex2f(-coordenadaXTela, -coordenadaYTela);
        glVertex2f(coordenadaXTela, -coordenadaYTela);
        glVertex2f(coordenadaXTela, coordenadaYTela);
    glEnd();
}

void Ambiente::desenharMar()
{
    glBegin(GL_POLYGON);
        glColor3f(corMar.r, corMar.g, corMar.b);
        glVertex2f(0, -coordenadaYTela);
        glVertex2f(0, -coordenadaYTela / 2);
        glVertex2f(coordenadaXTela, -coordenadaYTela / 2);
        glVertex2f(coordenadaXTela, -coordenadaYTela);
    glEnd();
}

void Ambiente::desenharTerra()
{
    glBegin(GL_POLYGON);
        glColor3f(corTerra.r, corTerra.g, corTerra.b);
        glVertex2f(-coordenadaXTela, -coordenadaYTela);
        glVertex2f(-coordenadaXTela, -coordenadaYTela / 2);
        glVertex2f(coordenadaXTela, -coordenadaYTela / 2);
        glVertex2f(coordenadaXTela, -coordenadaYTela);
    glEnd();
}
