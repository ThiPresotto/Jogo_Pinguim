#pragma once

#include "Pinguim.h"

class Area;

class Filhote : public Pinguim
{
public:
	Filhote(float x, float y, float z);

	void desenhar() const;

	Area getArea(); 
	void resetarTempoDeVida();
	void diminuirTempoDeVida(float deltaTime);
	bool verificarSeEstaVivo() const;

private:
	float tempoDeVida = 60.0f;
	bool estaVivo = true;
};

