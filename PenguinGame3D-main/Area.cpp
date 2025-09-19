#include <cmath> // Adicionado para corrigir o erro
#include "Area.h"

Area::Area(float altura, float largura, float profundidade, float centroX, float centroY, float centroZ)
	: altura(altura),
	largura(largura), 
	profundidade(profundidade),
	centroX(centroX),
	centroY(centroY),
	centroZ(centroZ)
{

}

bool Area::colideCom(const Area& outra) const
{
	return (std::abs(centroX - outra.centroX) * 2 < (largura + outra.largura)) &&
		(std::abs(centroY - outra.centroY) * 2 < (altura + outra.altura)) &&
		(std::abs(centroZ - outra.centroZ) * 2 < (profundidade + outra.profundidade));
}
