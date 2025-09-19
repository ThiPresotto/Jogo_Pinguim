#pragma once
class Area
{
public:

	Area(float altura, float largura, float profundidade, float centroX, float centroY, float centroZ);

	bool colideCom(const Area& outra) const;

private:

	float altura;
	float largura;
	float profundidade;

	float centroX;
	float centroY;
	float centroZ;
};
