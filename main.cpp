#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <array>
#include <string>

const int WIDTH = 800;
const int HEIGHT = 600;
const int FPS = 60;
const int DELAY = 1000 / FPS;
const int COORDINATES_X = 200;
const int COORDINATES_Y = 200;
const int COORDINATES_Z = 200;

int DURACAO_JOGO = 5*60; // 5 minutos


//////////////////////////////////////////////////////////////
///declaracao de funcoes
void elipse(float x, float y, float a, float b);
void triangle();
void init();
void display();
void specialKeys(int key, int x, int y);
void atualizaTempo(int valor);
void atualizarPeixes(int valor);
void desenharTempoJogo();
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
    double x;
    double y;
    Posicao(double x, double y) : x(x), y(y) {}
};

struct Cor
{
    double r;
    double g;
    double b;
    Cor(double r, double g, double b) : r(r), g(g), b(b) {}
};

struct Area 
{
    double x;
    double y;
    double largura;
    double altura;
    Area(double x, double y, double largura, double altura)
        : x(x), y(y), largura(largura), altura(altura) {}

    bool colideCom(const Area& outra) const
    {
        return (x < outra.x + outra.largura &&
                x + largura > outra.x &&
                y < outra.y + outra.altura &&
                y + altura > outra.y);
    }
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
    void mover(int delta);
    Area getArea() const { return Area(posicao.x - 5, posicao.y - 5, 10, 10); }

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

void Peixe::mover(int delta)
{
    if (direcao == Direcao::ESQUERDA)
        posicao.x -= delta;
    else if (direcao == Direcao::DIREITA)
        posicao.x += delta;

    if (posicao.x > COORDINATES_X - 2)
        this->direcao = Direcao::ESQUERDA;
    else if (posicao.x < 5)
        this->direcao = Direcao::DIREITA;
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
    Area getAreaCabeca() const;
    void capturouPeixe();
    bool temPeixeNaBoca() const { return peixeNaBoca; }
    bool entregouPeixe(Posicao posicaoFilhote);

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
    bool peixeNaBoca { false };
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

Area Pinguim::getAreaCabeca() const
{
    if (estado == ANDANDO) 
    {
        return Area(
            posicao.x - tamanhoCabeca / 2,
            posicao.y + tamanhoCorpo * 1.5 - tamanhoCabeca / 2,
            tamanhoCabeca,
            tamanhoCabeca
        );
    } 
    else
    { 
        if (direcao == Direcao::DIREITA) {
            return Area(
                posicao.x + tamanhoCorpo * 1.5 - tamanhoCabeca / 2,
                posicao.y - tamanhoCabeca / 2,
                tamanhoCabeca,
                tamanhoCabeca
            );
        } else { 
            return Area(
                posicao.x - tamanhoCorpo * 1.5 - tamanhoCabeca / 2,
                posicao.y - tamanhoCabeca / 2,
                tamanhoCabeca,
                tamanhoCabeca
            );
        }
    }
}

void Pinguim::capturouPeixe()
{
    peixeNaBoca = true;
}

bool Pinguim::entregouPeixe(Posicao posicaoFilhote)
{
    if (!temPeixeNaBoca())
        return false;

    if (posicao.x > posicaoFilhote.x - 10 && posicao.x < posicaoFilhote.x + 10 &&
        posicao.y > posicaoFilhote.y - 10 && posicao.y < posicaoFilhote.y + 10)
    {
        peixeNaBoca = false;
        return true;
    }

    return false;
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
    double posicaoBico{ 10 };
    double posicaoPeixe { 7 };

    if (direcao == Direcao::ESQUERDA)
    {
        posicaoBico = -posicaoBico;
        posicaoPeixe = -posicaoPeixe;
    }


    glColor3f(1.0, 0.65, 0.0);

        glPushMatrix();
        glTranslatef(posicaoBico, tamanhoCorpo * 1.5  - tamanhoCabeca / 2, 0.0f);
        glScalef(5, 5, 1);
        triangle();
    glPopMatrix();

    if (temPeixeNaBoca())
    {
        glPushMatrix();
        Peixe peixeNaBoca(Direcao::CIMA, { posicaoPeixe, 
                                    (tamanhoCorpo * 1.5 - tamanhoCabeca / 2  - 5)});  
        glScalef(2, 2, 1);    
        peixeNaBoca.desenhar();
        glPopMatrix();
    }
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
    Filhote(Direcao direcao, Posicao posicao, double tempoInicialVida = 30.0);

    void desenhar() override;
    void diminuirTempoVida()
    {
        tempoRestanteVida -= 1;
        if (tempoRestanteVida <= 0)
            morreu = true;

        std::cout << "Tempo de vida restante: " << tempoRestanteVida << std::endl;
    }

    bool estaMorto() const
    {
        return morreu;
    }

    void resetarTempoVida()
    {
        tempoRestanteVida = 60.0;
    }

private:
    double tempoRestanteVida;
    double escala{ 0.8 };
    bool morreu { false };
};

Filhote::Filhote(Direcao direcao, Posicao posicao, double tempoInicialVida)
    : Pinguim(direcao, posicao)
    , tempoRestanteVida(tempoInicialVida)
{
    peixeNaBoca = false;
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

Posicao posicaoFilhote{ POSICAO_X_PINGUIM - 20, POSICAO_Y_TERRA - 6 };

Pinguim pinguim(Direcao::DIREITA, { POSICAO_X_PINGUIM, POSICAO_Y_TERRA });
Filhote filhote(Direcao::DIREITA, posicaoFilhote);


const std::array<double, 4> posicaoInicialYPeixes = { -180, -160, -140, -120 };
const std::array<double, 7> posicaoInicialXPeixes = { 50, 25, 75, 100, 125, 150, 175 };

std::vector<Peixe> peixes {
    Peixe(Direcao::DIREITA, { posicaoInicialXPeixes[0], posicaoInicialYPeixes[0] }),
    Peixe(Direcao::DIREITA, { posicaoInicialXPeixes[1], posicaoInicialYPeixes[1] }),
    Peixe(Direcao::ESQUERDA, { posicaoInicialXPeixes[2], posicaoInicialYPeixes[2] }),
    Peixe(Direcao::ESQUERDA, { posicaoInicialXPeixes[3], posicaoInicialYPeixes[3] })
};

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Jogo do Pinquim");
    glutSpecialFunc(specialKeys);
    glutTimerFunc(1000, atualizaTempo, 0);
    glutTimerFunc(200, atualizarPeixes, 0); 

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

void specialKeys(int key, int x, int y) 
{
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

    if (pinguim.entregouPeixe(posicaoFilhote))
        filhote.resetarTempoVida();

    glutPostRedisplay();
}

void desenharTempoJogo()
{
    glPushMatrix();
        glColor3f(1, 1, 1);
        glRasterPos2f(0, 0);
        std::string tempo = "Tempo restante: " + std::to_string(DURACAO_JOGO);
        for (char c : tempo)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    glPopMatrix();
}

void atualizaTempo(int valor) 
{
    filhote.diminuirTempoVida();

    if (filhote.estaMorto())
    {
        std::cout << "Filhote morreu!" << std::endl;
        exit(0);
    }

    if (DURACAO_JOGO > 0)
        DURACAO_JOGO--; 
    else
    {
        std::cout << "Fim de jogo!" << std::endl;
        exit(0);
    }

    glutPostRedisplay();
    glutTimerFunc(1000, atualizaTempo, 0);
}

void atualizarPeixes(int valor)
{
    for (auto& peixe : peixes)
    {
        peixe.mover(1);


        if (pinguim.temPeixeNaBoca())
            continue;


        if (pinguim.getAreaCabeca().colideCom(peixe.getArea()))
            pinguim.capturouPeixe();
    }

    glutPostRedisplay();
    glutTimerFunc(DELAY, atualizarPeixes, 0);
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Ambiente ambiente(COORDINATES_X, COORDINATES_Y);
    ambiente.desenhar();
    desenharTempoJogo();

    pinguim.desenhar();
    filhote.desenhar();

    for (auto& peixe : peixes)
        peixe.desenhar();

    glutSwapBuffers();
}
