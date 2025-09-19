#include "Pinguim.h"

#include <GL/glut.h>
#include <math.h>
#include <iostream>

#include "Area.h"
#include "Cor.h"
#include "Peixe.h"
#include "Filhote.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Pinguim::Pinguim(float x, float y, float z)
		: x(x), y(y), z(z)
{
}

float Pinguim::getX()
{
	return x;
}

float Pinguim::getY()
{
	return y;
}

float Pinguim::getZ()
{
	return z;
}

void Pinguim::desenha() const
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(anguloY, 0.0f, 1.0f, 0.0f);
	if (filhote)
		glScalef(0.5f, 0.5f, 0.5f);

	desenhaCorpo();
	desenhaOlhos();
	desenhaBico();
	desenhaPatas();
	desenhaAsas();
	desenhaCabeca();
	glPopMatrix();
}

void Pinguim::desenhaCorpo() const
{
	auto corPreta = Cor::preto();

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corPreta[0]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, corPreta[1]);
	glMaterialfv(GL_FRONT, GL_SHININESS, corPreta[2]);

	glPushMatrix();
	glScalef(0.6f, 0.7f, 0.6f);
	glutSolidSphere(1.0f, 20, 20);
	glPopMatrix();

	auto corBranca = Cor::branco();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corBranca[0]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, corBranca[1]);
	glMaterialfv(GL_FRONT, GL_SHININESS, corBranca[2]);

	glPushMatrix();
	glTranslatef(0.0f, 0.f, 0.5f);
	glScalef(0.3f, 0.4f, 0.1f);
	glutSolidSphere(1.0f, 20, 20);
	glPopMatrix();
}

void Pinguim::desenhaOlhos() const
{
	auto corBranca = Cor::branco();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corBranca[0]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, corBranca[1]);
	glMaterialfv(GL_FRONT, GL_SHININESS, corBranca[2]);

	glPushMatrix();
	glTranslatef(-0.2f, 1.2f, 0.2f);
	glutSolidSphere(0.1f, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.2f, 1.2f, 0.2f);
	glutSolidSphere(0.1f, 20, 20);
	glPopMatrix();
}

void Pinguim::desenhaBico() const
{
	auto corLaranja = Cor::laranjaOpaco();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corLaranja[0]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, corLaranja[1]);
	glMaterialfv(GL_FRONT, GL_SHININESS, corLaranja[2]);

	glPushMatrix();
	glTranslatef(0.0f, 1.0f, 0.3f);
	glScalef(0.7f, 0.7f, 1.f);
	glutSolidCone(0.2f, 0.4f, 20, 20);
	glPopMatrix();

	if (!temPeixe)
		return;

	Peixe peixeNoBico(0.0f, 1.0f, 0.8f, 180);
	peixeNoBico.desenha();
}

void Pinguim::desenhaPatas() const
{
	auto corLaranja = Cor::laranja();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corLaranja[0]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, corLaranja[1]);
	glMaterialfv(GL_FRONT, GL_SHININESS, corLaranja[2]);

	glPushMatrix();
	glTranslatef(-0.3f, -0.7f, 0.0f);
	glScalef(0.5f, 0.2f, 0.5f);
	glutSolidSphere(0.5f, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3f, -0.7f, 0.0f);
	glScalef(0.5f, 0.2f, 0.5f);
	glutSolidSphere(0.5f, 20, 20);
	glPopMatrix();
}

void Pinguim::desenhaAsas() const
{
	auto corPreta = Cor::preto();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corPreta[0]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, corPreta[1]);
	glMaterialfv(GL_FRONT, GL_SHININESS, corPreta[2]);
	glPushMatrix();
	glTranslatef(-0.6f, 0.0f, 0.0f);
	glScalef(0.2f, 0.5f, 0.2f);
	glutSolidSphere(1.0f, 20, 20);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.6f, 0.0f, 0.0f);
	glScalef(0.2f, 0.5f, 0.2f);
	glutSolidSphere(1.0f, 20, 20);
	glPopMatrix();
}

void Pinguim::desenhaCabeca() const
{
	auto corPreto = Cor::preto();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corPreto[0]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, corPreto[1]);
	glMaterialfv(GL_FRONT, GL_SHININESS, corPreto[2]);

	glPushMatrix();
	glTranslatef(0.0f, 1.f, 0.0f);
	glutSolidSphere(0.4f, 20, 20);
	glPopMatrix();
}

void Pinguim::mover(float delta)
{
	x += sin(anguloY * M_PI / 180.0) * delta;
	z += cos(anguloY * M_PI / 180.0) * delta;
}

void Pinguim::orientar(float deltaRotaciona)
{
	anguloY += deltaRotaciona;
}

void Pinguim::verificarSePegouPeixe(Peixe &peixe)
{
	float posicaoXCabeca = x;
	float posicaoZCabeca = z;
	float posicaoYCabeca = y;

	
	float larguraCabeca = 1.f;
	float profundidadeCabeca = 1.f;
	float alturaCabeca = 1.f;

	Area areaPinguim(
			alturaCabeca, larguraCabeca, profundidadeCabeca,
			posicaoXCabeca, posicaoYCabeca, posicaoZCabeca);

	if (areaPinguim.colideCom(peixe.getArea()))
	{
		temPeixe = true;
		std::cout << "Pinguim pegou o peixe!" << std::endl;
	}
}

bool Pinguim::temPeixePegado() const
{
	return temPeixe;
}

void Pinguim::verificarSeAlimentouFilhote(Filhote& filhote)
{
	float posicaoXCabeca = x;
	float posicaoZCabeca = z;
	float posicaoYCabeca = y;

	float larguraCabeca = 1.f;
	float profundidadeCabeca = 1.f;
	float alturaCabeca = 1.f;

	Area areaPinguim(
		alturaCabeca, larguraCabeca, profundidadeCabeca,
		posicaoXCabeca, posicaoYCabeca, posicaoZCabeca);

	if (areaPinguim.colideCom(filhote.getArea()))
	{
		temPeixe = false;
		filhote.resetarTempoDeVida();
	}
}

Area Pinguim::getArea() const
{
	float largura = 1.f;
	float profundidade = 1.f;
	float altura = 1.f;
	return Area(altura, largura, profundidade, x, y, z);
}
