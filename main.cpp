#include <GL/glut.h>
#include <iostream>
#include <vector>

const int WIDTH = 800;
const int HEIGHT = 600;
const int FPS = 60;
const int DELAY = 1000 / FPS;
const int COORDINATES_X = 200;
const int COORDINATES_Y = 200;
const int COORDINATES_Z = 200;


//////////////////////////////////////////////////////////////
///declaracao de funcoes
void elipse(float x, float y, float a, float b);
void triangle();
void init();
void display();
void specialKeys(int key, int x, int y);
//////////////////////////////////////////////////////////////

// interface para desenhar objetos
class IDesenhavel
{
public:
    virtual void desenhar() = 0;
    virtual ~IDesenhavel() {}
};

//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
/// codigo com funcoes comuns
struct Posicao
{
    int x;
    int y;
    Posicao(int x, int y) : x(x), y(y) {}
};

struct Cor
{
    double r;
    double g;
    double b;
    Cor(double r, double g, double b) : r(r), g(g), b(b) {}
};

enum class Direcao
{
    CIMA,
    BAIXO,
    ESQUERDA,
    DIREITA
};

void triangle()
{
    glBegin(GL_TRIANGLES);
    glVertex3f(-1, 0, 0);
    glVertex3f(0, 1, 0);
    glVertex3f(1, 0, 0);
    glEnd();
}

void elipse(float x, float y, float a, float b)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        float angle = i * 3.14159265358979323846 / 180.0f;
        glVertex2f(x + a * cos(angle), y + b * sin(angle));
    }
    glEnd();
}
//////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////// 
// codigo sobre o ambiente
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
    double coordenadaXTela;
    double coordenadaYTela;
};

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

//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// codigo sobre o peixe
class Peixe : public IDesenhavel
{
public:
    Peixe(Direcao direcao, Posicao posicao);
    void desenhar() override;

private:
    void desenharCorpo();
    void desenharCauda();

    Direcao direcao;
    Posicao posicao;
    Cor cor{ 1.0, 0.65, 0.0 };
    double tamanhoCorpo{ 18 };
};


Peixe::Peixe(Direcao direcao, Posicao posicao)
    : direcao(direcao)
    , posicao(posicao)
{
}

void Peixe::desenhar()
{
    glPushMatrix();
    glTranslatef(posicao.x, posicao.y, 0.0f);
    glScalef(0.5, 0.5, 1);
    glColor3f(cor.r, cor.g, cor.b);
    desenharCorpo();
    desenharCauda();
    glPopMatrix();
}

void Peixe::desenharCorpo()
{
    glPushMatrix();
    switch (direcao)
    {
    case Direcao::CIMA:
    case Direcao::BAIXO:
    {
        elipse(0, 0, 5, 10);
        break;
    }
    case Direcao::ESQUERDA:
    case Direcao::DIREITA:
        elipse(0, 0, 10, 5);
        break;
    }
    glPopMatrix();
}

void Peixe::desenharCauda()
{
    glPushMatrix();
    switch (direcao)
    {
    case Direcao::CIMA:
    {
        glTranslatef(0.0f, -tamanhoCorpo, 0.0f);
        break;
    }
    case Direcao::BAIXO:
    {
        glTranslatef(0.0f, tamanhoCorpo, 0.0f);
        glRotatef(180, 0.0f, 0.0f, 1.0f);
        break;
    }
    case Direcao::ESQUERDA:
    {
        glTranslatef(tamanhoCorpo, 0.0f, 0.0f);
        glRotatef(90, 0.0f, 0.0f, 1.0f);
        break;
    }
    case Direcao::DIREITA:
    {
        glTranslatef(-tamanhoCorpo, 0.0f, 0.0f);
        glRotatef(-90, 0.0f, 0.0f, 1.0f);
        break;
    }
    }
    glScalef(5, 10, 1);
    triangle();
    glPopMatrix();
}

//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// codigo sobre o pinguim pai
class Pinguim : public IDesenhavel
{
public:
    Pinguim(Direcao direcao, Posicao posicao);
    void desenhar() override;
    void mover(int deltaX, int deltaY, Direcao direcao);

private:

    enum Estado
    {
        ANDANDO,
        NADANDO
    };

    void desenharCorpo();
    void desenharCabeca();
    void desenharOlhos();
    void desenharBico();
    void desenharPatas();

protected:
    Direcao direcao;
    Posicao posicao;
    Estado estado{ ANDANDO };
    double tamanhoCorpo{ 18 };
    double tamanhoCabeca{ 10 };
    double tamanhoOlhos{ 2 };
    double tamanhoBico{ 3 };
    double tamanhoPatas{ 8 };
};

Pinguim::Pinguim(Direcao direcao, Posicao posicao)
    : direcao(direcao)
    , posicao(posicao)
{

}

void Pinguim::desenhar()
{
    glPushMatrix();
    glTranslatef(posicao.x, posicao.y, 0.0f);
    glScalef(0.5, 0.5, 1);

    if (estado == Estado::NADANDO)
    {
        if (direcao == Direcao::DIREITA)
            glRotatef(-90, 0, 0, 1);
        else if (direcao == Direcao::ESQUERDA)
            glRotatef(90, 0, 0, 1);
    }

    glPushMatrix();
    glPopMatrix();
    desenharCorpo();
    desenharCabeca();
    desenharOlhos();
    desenharBico();
    desenharPatas();
    glPopMatrix();
}

void Pinguim::mover(int deltaX, int deltaY, Direcao direcao)
{
    if (direcao == Direcao::ESQUERDA || direcao == Direcao::DIREITA)
        this->direcao = direcao;

    if (estado == Estado::ANDANDO)
    {
        if (posicao.x + deltaX >= -COORDINATES_X)
            posicao.x += deltaX;

        if (posicao.x >= 5)
        {
            estado = Estado::NADANDO;
            posicao.y = -120;
            posicao.x = 25;
        }

    }
    else if (estado == Estado::NADANDO)
    {
        if (posicao.x + deltaX >= 0 && posicao.x + deltaX <= COORDINATES_X)
            posicao.x += deltaX;

        if ((posicao.y + deltaY) <= -100 && (posicao.y + deltaY) >= -COORDINATES_Y)
            posicao.y += deltaY;

        if (posicao.x < 5 && posicao.y > -110)
        {
            estado = Estado::ANDANDO;
            posicao.y = -71;
        }
    }

}

void Pinguim::desenharCorpo()
{
    float posicaoBarriga{ 10.0f };

    if (direcao == Direcao::ESQUERDA)
        posicaoBarriga = -posicaoBarriga;

    glColor3f(0.12, 0.12, 0.12);
    glPushMatrix();
    glTranslatef(0.0f, -tamanhoCorpo, 0.0f);
    elipse(0, 0, tamanhoCorpo, tamanhoCorpo * 2);
    glTranslatef(posicaoBarriga, 0, 0.0f);
    glColor3f(1.0, 1.0, 1.0);
    elipse(0, 0, tamanhoCorpo * 0.5, tamanhoCorpo * 1.5);
    glPopMatrix();
}

void Pinguim::desenharCabeca()
{
    glColor3f(0.12, 0.12, 0.12);
    glPushMatrix();
    glTranslatef(0.0f, tamanhoCorpo * 1.5, 0.0f);
    elipse(0, 0, tamanhoCabeca, tamanhoCabeca);
    glPopMatrix();
}

void Pinguim::desenharOlhos()
{
    int posicaoOlho{ 2 };

    if (direcao == Direcao::ESQUERDA)
        posicaoOlho = -posicaoOlho;

    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(0.0f, tamanhoCorpo * 1.5, 0.0f);
    glTranslatef(posicaoOlho, 0.0f, 0.0f);
    elipse(0, 0, tamanhoOlhos, tamanhoOlhos);
    glPopMatrix();
}

void Pinguim::desenharBico()
{
    int posicaoBico{ 10 };

    if (direcao == Direcao::ESQUERDA)
        posicaoBico = -posicaoBico;


    glColor3f(1.0, 0.65, 0.0);

    glPushMatrix();
    glTranslatef(0.0f, tamanhoCorpo * 1.5, 0.0f);
    glTranslatef(posicaoBico, -tamanhoCabeca / 2, 0.0f);
    glScalef(5, 5, 1);
    triangle();
    glPopMatrix();
}

void Pinguim::desenharPatas()
{
    glColor3f(0.78, 0.39, 0.0);

    glPushMatrix();
    glTranslatef(4, -tamanhoCorpo * 3, 0.0f);
    elipse(0, 0, tamanhoPatas, tamanhoPatas / 2);
    glTranslatef(-tamanhoCorpo / 2, 0.0f, 0.0f);
    elipse(0, 0, tamanhoPatas, tamanhoPatas / 2);
    glPopMatrix();
}
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// codigo Filhote
class Filhote : public Pinguim
{
public:
    Filhote(Direcao direcao, Posicao posicao, double tempoInicialVida = 60.0);

    void desenhar() override;

private:
    double tempoRestanteVida;
    double escala{ 0.8 }; // menor que o pinguim adulto
};

Filhote::Filhote(Direcao direcao, Posicao posicao, double tempoInicialVida)
    : Pinguim(direcao, posicao)
    , tempoRestanteVida(tempoInicialVida)
{
}

void Filhote::desenhar()
{
    glPushMatrix();
    glTranslatef(posicao.x, posicao.y, 0.0f);
    glScalef(escala, escala, 1);
    glTranslatef(-posicao.x, -posicao.y, 0.0f);
    Pinguim::desenhar();         
    glPopMatrix();
}

/////////////////////////////////////////////////////////////////////


const int POSICAO_Y_TERRA = -COORDINATES_Y / 2 + 29;
const int POSICAO_X_PINGUIM = -COORDINATES_X / 2 - 50;

Pinguim pinguim(Direcao::DIREITA, { POSICAO_X_PINGUIM, POSICAO_Y_TERRA });
Filhote filhote(Direcao::DIREITA, { POSICAO_X_PINGUIM -20, POSICAO_Y_TERRA - 6 });

std::vector<Peixe> peixes = {
    Peixe(Direcao::DIREITA, {15 , -75}),
    Peixe(Direcao::ESQUERDA, { 30, -60}),
    Peixe(Direcao::DIREITA, {65, -74}),
    Peixe(Direcao::ESQUERDA, {74, -80}),
};

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Jogo do Pinquim");
    glutSpecialFunc(specialKeys);

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

void init(void)
{
    glClearColor(1, 1, 1, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-COORDINATES_X, COORDINATES_X, -COORDINATES_Y,
        COORDINATES_Y, -COORDINATES_Z, COORDINATES_Z);
}

void specialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        pinguim.mover(-5, 0, Direcao::ESQUERDA);
        break;
    case GLUT_KEY_RIGHT:
        pinguim.mover(5, 0, Direcao::DIREITA);
        break;
    case GLUT_KEY_UP:
        pinguim.mover(0, 5, Direcao::CIMA);
        break;
    case GLUT_KEY_DOWN:
        pinguim.mover(0, -5, Direcao::BAIXO);
        break;
    }
    glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Ambiente ambiente(COORDINATES_X, COORDINATES_Y);
    ambiente.desenhar();

    pinguim.desenhar();
    filhote.desenhar();

    glutSwapBuffers();
}