#pragma once

class Area;

class Gelo
{
public:
	Gelo(float x, float y, float z);

	void desenha() const;

	void setNewPosition(float x, float z);
	Area getArea() const;

private:
	float x;
	float y;
	float z;
};

