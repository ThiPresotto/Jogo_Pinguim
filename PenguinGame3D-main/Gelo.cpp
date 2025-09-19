#include "Gelo.h"

#include "Area.h"
#include "Cor.h"

#include <GL/glut.h>


Gelo::Gelo(float x, float y, float z)
	: x(x), y(y), z(z)
{
}

void Gelo::desenha() const
{
	auto azul = Cor::azul();

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, azul[0]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, azul[1]);
	glMaterialfv(GL_FRONT, GL_SHININESS, azul[2]);

	glPushMatrix();
		glTranslatef(x, y, z);
		glScalef(2.f, 0.05f, 2.f);
		glutSolidSphere(1.f, 20, 20);
	glPopMatrix();

}

void Gelo::setNewPosition(float x, float z)
{
	this->x = x;
	this->z = z;
}

Area Gelo::getArea() const
{
	float profundidade = 2.f;
	float altura = 2.f;
	float largura = 2.f;

	return Area(altura, largura, profundidade, x, y, z);
}