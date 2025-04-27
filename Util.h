#pragma once

struct Posicao
{
    int x;
    int y;
    Posicao (int x, int y) : x(x), y(y) {}
};

struct Cor
{
    double r;
    double g;
    double b;
    Cor (double r, double g, double b) : r(r), g(g), b(b) {}
};

enum class Direcao
{
    CIMA,
    BAIXO,
    ESQUERDA,
    DIREITA
};


