#pragma once

class Area;
class Peixe;
class Filhote;

class Pinguim
{
public:
	Pinguim(float x, float y, float z);
	float getX();
	float getY();
	float getZ();
	void desenha() const;
	void mover(float delta);

	void orientar(float deltaRotaciona);

	void verificarSePegouPeixe(Peixe &peixe);
	bool temPeixePegado() const;
	void verificarSeAlimentouFilhote(Filhote& filhote);

	Area getArea() const;

private:
	void desenhaCorpo() const;
	void desenhaOlhos() const;
	void desenhaBico() const;
	void desenhaPatas() const;
	void desenhaAsas() const;
	void desenhaCabeca() const;

protected:
	float x, y, z;
	float anguloY{90.f};
	bool filhote{false};

	bool temPeixe{false};
};
