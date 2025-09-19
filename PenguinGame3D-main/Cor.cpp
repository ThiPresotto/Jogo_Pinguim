#include "Cor.h"



static const float _preto[3][4] = {
    {0.0f, 0.0f, 0.0f, 1.0f}, // ambient
    {0.01f, 0.01f, 0.01f, 1.0f}, // diffuse
    {0.5f, 0.5f, 0.5f, 1.0f}     // specular
};

static const float _branco[3][4] = {
    {1.0f, 1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f, 1.0f}
};

static const float _laranja[3][4] = {
    {1.0f, 0.5f, 0.0f, 1.0f},
    {1.0f, 0.5f, 0.0f, 1.0f},
    {1.0f, 0.5f, 0.0f, 1.0f}
};

static const float _azul[3][4] = {
    {0.0f, 0.0f, 0.5f, 1.0f},
    {0.0f, 0.0f, 1.0f, 1.0f},
    {0.5f, 0.5f, 1.0f, 1.0f}
};

static const float _laranjaOpaco[3][4] = {
    {0.3f, 0.15f, 0.0f, 1.0f},  // ambient (menos intenso)
    {0.8f, 0.4f, 0.0f, 1.0f},   // diffuse (menos saturado)
    {0.1f, 0.1f, 0.1f, 1.0f}    // specular (quase fosco)
};

static const float _azulMar[3][4] = {
    {0.0f, 0.0f, 0.5f, 1.0f},   // ambient
    {0.0f, 0.0f, 1.0f, 1.0f},   // diffuse
    {0.5f, 0.5f, 1.0f, 1.0f}    // specular
};

const float (*Cor::preto())[4] { return _preto; }
const float (*Cor::branco())[4] { return _branco; }
const float (*Cor::laranja())[4] { return _laranja; }
const float (*Cor::azul())[4] { return _azul; }
const float (*Cor::laranjaOpaco())[4] { return _laranjaOpaco; }
const float (*Cor::azulMar())[4] { return _azulMar; }

