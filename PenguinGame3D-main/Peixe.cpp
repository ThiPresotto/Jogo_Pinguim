#include "Peixe.h"

#include "Area.h"
#include "Cor.h"
#include "Peixe.h"

#include <GL/glut.h>

Peixe::Peixe(float x, float y, float z, float angulo)
	: x(x), y(y), z(z), anguloY(angulo) 
{
}

void Peixe::mover(float delta, float boundary)
{
	if (anguloY < 0.0f)
		z += delta;
	else
		z -= delta;

	if (z > boundary)
	{
		z = boundary;
		anguloY = -anguloY;
	}
	else if (z < -boundary)
	{
		z = -boundary;
		anguloY = -anguloY;
	}
}

Area Peixe::getArea() const
{
	float largura = 1.f; 
	float altura = 1.f; 
	float profundidade = 1.f;

	return Area(altura, largura, profundidade, x, y, z);
}

void Peixe::setNewPosition(float newX, float newZ)
{
	x = newX;
	z = newZ;
}

void Peixe::desenha() const
{
	glPushMatrix();
		glTranslatef(x, y, z);
		glRotatef(anguloY, 0.0f, 1.0f, 0.0f);
		desenhaCorpo();
		desenhaCauda();
	glPopMatrix();
}

void Peixe::desenhaCorpo() const
{
	auto corLaranja = Cor::laranja();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corLaranja[0]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, corLaranja[1]);
	glMaterialfv(GL_FRONT, GL_SHININESS, corLaranja[2]);

	glPushMatrix();
		glScalef(0.5f, 0.2f, 0.2f);
		glutSolidSphere(1.0f, 20, 20);
	glPopMatrix();
}

void Peixe::desenhaCauda() const
{
	auto corLaranja = Cor::laranja();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corLaranja[0]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, corLaranja[1]);
	glMaterialf(GL_FRONT, GL_SHININESS, 160);

	glPushMatrix();
		glTranslatef(-0.8f, 0.0f, 0.0f);
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		glutSolidCone(0.2f, 0.5f, 20, 20);
	glPopMatrix();
}
