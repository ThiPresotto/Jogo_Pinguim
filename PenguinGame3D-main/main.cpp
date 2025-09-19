#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <GLES/gl.h>

#include "Area.h"
#include "Pinguim.h"
#include "Cor.h"
#include "Filhote.h"
#include "Peixe.h"
#include "Gelo.h"

#define STB_IMAGE_IMPLEMENTATION  
#include "stb_image.h"

void init();
void reshape(int w, int h);
void specialKeyboard(int key, int x, int y);
void doFrame(int value);
void atualizarTempoJogo(int value);

float alturaDoChao{ -0.8 };

float TEMPO_JOGO{ 300 };

GLfloat rotX, rotY;
int INITIAL_WIDTH = 800;
int INITIAL_HEIGHT = 600;
int window_top, window_side, window_front, window_free;
std::vector<int> window_ids;

bool acabouOJogo = false;
bool jogadorGanhou = false;	
std::string razaoDeTerAcabado = "";

Pinguim pinguim(1, 0.0f, 0.0f);
Filhote filhote(0.0f, 0.0f, 0.0f);

GLuint texID[1];
const char * textureFileNames = "imagens/gelo.jpg";

std::vector<Peixe> cardume = {
		Peixe(12.0f, alturaDoChao, -2.0f, 90),
		Peixe(15.0f, alturaDoChao, 13.0f, 180),
		Peixe(8.0f, alturaDoChao, -15.0f, 90),
		Peixe(-10.0f, alturaDoChao, 10.0f, 180),
		Peixe(-9.0f, alturaDoChao, -4.f, 90),
};

std::vector<Gelo> gelos =
{
	Gelo(-10.0f, alturaDoChao, -15.0f),
	Gelo(-10.0f, alturaDoChao, 16.0f),
	Gelo(13.0f, alturaDoChao, -7.0f),
	Gelo(15.0f, alturaDoChao, 10.0f)
};

void carregaGeloTextura() 
{
	int width, height, nrChannels;
	unsigned char* data;

	glGenTextures(1, texID);

	glBindTexture(GL_TEXTURE_2D, texID[0]);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture

	data = stbi_load(textureFileNames, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	}
	else
	{
		printf("Failed to load texture\n");
	}
	stbi_image_free(data);
}

std::string formatTime(int totalSeconds) {
	if (totalSeconds < 0) totalSeconds = 0;
	int minutes = totalSeconds / 60;
	int seconds = totalSeconds % 60;
	std::ostringstream oss;
	oss << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;
	return oss.str();
}

void gerarPosicoesAleatoriasPeixes()
{
    for (auto &peixe : cardume)
    {
        float novoX = -20.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (40.0f)));
        float novoZ = -20.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (40.0f)));
        peixe.setNewPosition(novoX, novoZ);
    }
}

void gerarPosicoesAleatoriasGelos()
{
	for (auto &gelo : gelos)
	{
		float novoX = -15.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (30.0f)));
		float novoZ = -15.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (30.0f)));
		gelo.setNewPosition(novoX, novoZ);
	}
}

void timerGelos(int value)
{
	gerarPosicoesAleatoriasGelos();
	
	glutTimerFunc(5000, timerGelos, 0);
}

void timerPeixes(int value)
{
    gerarPosicoesAleatoriasPeixes();
    
    glutTimerFunc(10000, timerPeixes, 0);
}

void drawPlacar() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_LIGHTING);

	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);

	if (acabouOJogo || jogadorGanhou) {
		std::string endMsg = jogadorGanhou ? "VOCE GANHOU!" : "ACABOU O JOGO!!";
		if (jogadorGanhou) 
			glColor3f(0.1f, 0.8f, 0.1f);
		else 
			glColor3f(0.8f, 0.1f, 0.1f);


		glRasterPos2i(w / 2 - 100, h / 2 + 20);
		for (char c : endMsg) 
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);

		if (acabouOJogo && !razaoDeTerAcabado.empty()) {
			glColor3f(0.8f, 0.1f, 0.1f);
			glRasterPos2i(w / 2 - 120, h / 2);
			for (char c : razaoDeTerAcabado) 
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
		}

	}
	else 
	{
		std::string sessionStr = "Jogo acaba em: " + formatTime(TEMPO_JOGO);
		glColor3f(0.f, 0.f, 0.f);
		glRasterPos2i(10, h - 40);
		for (char c : sessionStr) 
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}

	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void drawScene()
{
	// Plataforma de gelo
	float zoom{ 40.f };

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, new float[4] {0.8f, 0.95f, 1.0f, 1.0f});
	glMaterialfv(GL_FRONT, GL_SPECULAR, new float[4] {1.0f, 1.0f, 1.0f, 1.0f});
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);

	glEnable(GL_TEXTURE_2D);

	/* Draw a teapot with the first texture at (-2.3,0.3,0) */

	glBindTexture(GL_TEXTURE_2D, texID[0]);  // Bind texture #0 for use on the first teapot.

	glPushMatrix();
		glTranslatef(0 , alturaDoChao, 0);
		glScalef(zoom , 0.05f, zoom);
		glutSolidCube(1.0f);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	pinguim.desenha();
	filhote.desenha();

	for (auto &peixe : cardume)
		peixe.desenha();

	for (auto& gelo : gelos)
		gelo.desenha();

}

void display_top()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(pinguim.getX(), pinguim.getY() + 20.0f, pinguim.getZ(),
						pinguim.getX(), pinguim.getY(), pinguim.getZ(),
						0, 0, -1);
	drawScene();
	drawPlacar();
	glutSwapBuffers();
}

void display_side()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(pinguim.getX(), pinguim.getY() + 5.0f, pinguim.getZ() + 20.0f,
						pinguim.getX(), pinguim.getY(), pinguim.getZ(),
						0, 1, 0);
	drawScene();
	drawPlacar();
	glutSwapBuffers();
}

void display_front()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(pinguim.getX() + 20.0f, pinguim.getY() + 5.0f, pinguim.getZ(), pinguim.getX(), pinguim.getY(), pinguim.getZ(),
						0, 1, 0);
	drawScene();
	drawPlacar();
	glutSwapBuffers();
}

void display_free()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(pinguim.getX() + 10.0f, pinguim.getY() + 10.0f, pinguim.getZ() + 10.0f,
						pinguim.getX(), pinguim.getY(), pinguim.getZ(),
						0, 1, 0);
	drawScene();
	drawPlacar();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, GLfloat(w) / GLfloat(h), 0.1, 100.0);
}

void specialKeyboard(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		pinguim.orientar(4);
		break;
	case GLUT_KEY_RIGHT:
		pinguim.orientar(-4);
		break;
	case GLUT_KEY_UP:
		pinguim.mover(0.3);
		break;
	case GLUT_KEY_DOWN:
		pinguim.mover(-0.3);
		break;
	}
	glutPostRedisplay();
}

void doFrame(int value)
{
	for (auto &peixe : cardume)
	{
		if (!pinguim.temPeixePegado())
			pinguim.verificarSePegouPeixe(peixe);
	}

	for (auto& gelo : gelos)
	{
		auto areaGelo = gelo.getArea();

		if (areaGelo.colideCom(pinguim.getArea()))
		{
			razaoDeTerAcabado = "O pinguim escorregou no gelo e caiu no mar!";
			acabouOJogo = true;
		}

	}

	if (pinguim.temPeixePegado())
		pinguim.verificarSeAlimentouFilhote(filhote);


	for (int id : window_ids)
	{
		glutSetWindow(id);
		glutPostRedisplay();
	}
	glutTimerFunc(1000 / 60, doFrame, 0);
}

void atualizarTempoJogo(int value)
{
	filhote.diminuirTempoDeVida(1.0f);

	if (filhote.verificarSeEstaVivo())
	{
		std::cout << "Filhote morreu!" << std::endl;
		razaoDeTerAcabado = "O filhote morreu de fome.";
		acabouOJogo = true;
	}

	if (!acabouOJogo || jogadorGanhou)
	{
		if (TEMPO_JOGO > 0)
			TEMPO_JOGO--;
		else
		{
			std::cout << "Fim de jogo!" << std::endl;
			jogadorGanhou = true;
		}
	}

	std::cout << "Tempo de jogo restante: " << TEMPO_JOGO << " segundos." << std::endl;

	glutPostRedisplay();
	glutTimerFunc(1000, atualizarTempoJogo, 0);
}

void init()
{
	glClearColor(0.529f, 0.808f, 0.922f, 1.0f); // background color

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float white[4] = { 1, 1, 1, 1 };  // A white material, suitable for texturing.
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	const int spacing = 10;
	const int winW = 600;
	const int winH = 450;
	const int baseX = 50;
	const int baseY = 50;


	glutInitWindowSize(winW, winH);
	glutInitWindowPosition(baseX + 0 * (winW + spacing), baseY + 0 * (winH + spacing));
	window_free = glutCreateWindow("Free Camera");
	init();
	carregaGeloTextura();
	
	glutDisplayFunc(display_free);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeyboard);
	window_ids.push_back(window_free);

	glutInitWindowSize(winW, winH);
	glutInitWindowPosition(baseX + 1 * (winW + spacing), baseY + 0 * (winH + spacing));
	window_top = glutCreateWindow("Top Camera");
	init();
	carregaGeloTextura();
	glutDisplayFunc(display_top);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeyboard);
	window_ids.push_back(window_top);

	glutInitWindowSize(winW, winH);
	glutInitWindowPosition(baseX + 0 * (winW + spacing), baseY + 1 * (winH + spacing));
	window_side = glutCreateWindow("Side Camera");
	init();
	carregaGeloTextura();
	glutDisplayFunc(display_side);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeyboard);
	window_ids.push_back(window_side);

	glutInitWindowSize(winW, winH);
	glutInitWindowPosition(baseX + 1 * (winW + spacing), baseY + 1 * (winH + spacing));
	window_front = glutCreateWindow("Front Camera");
	init();

	carregaGeloTextura();

	glutDisplayFunc(display_front);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeyboard);
	window_ids.push_back(window_front);

	glutTimerFunc(100, doFrame, 0);
	srand(static_cast<unsigned int>(time(nullptr)));
	glutTimerFunc(10000, timerPeixes, 0);
	glutTimerFunc(5000, timerGelos, 0);
	glutTimerFunc(1000, atualizarTempoJogo, 0);
	glutMainLoop();
	return 0;
}
