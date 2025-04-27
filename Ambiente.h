#pragma once

#include "IDesenhavel.h"
#include "Util.h"


class Ambiente : public IDesenhavel
{
public:
    Ambiente(double coordenadaXTela, double coordenadaYTela);
    void desenhar() override;

private:
    void desenharCeu();
    void desenharMar();
    void desenharTerra();

    Cor corCeu;
    Cor corMar;
    Cor corTerra;
    double larguraTela;
    double alturaTela;
    double coordenadaXTela;
    double coordenadaYTela;
};

