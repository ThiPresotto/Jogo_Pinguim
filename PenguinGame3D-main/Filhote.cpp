#include "Filhote.h"

#include "Area.h"

#include <GL/glut.h>
#include "Gelo.h"

Filhote::Filhote(float x, float y, float z)
	:Pinguim(x, y, z)
{
	filhote = true;
}

void Filhote::desenhar() const
{
	glPushMatrix();
		Pinguim::desenha();
	glPopMatrix();
}

Area Filhote::getArea()
{
	return Area(1.0f, 1.0f, 1.0f, x, y, z);
}

void Filhote::resetarTempoDeVida()
{
	tempoDeVida = 60.0f;
}

void Filhote::diminuirTempoDeVida(float deltaTime)
{
	tempoDeVida -= deltaTime;

	if (tempoDeVida <= 0.0f) 
		estaVivo = false;
}

bool Filhote::verificarSeEstaVivo() const
{
	return !estaVivo;
}
