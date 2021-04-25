#pragma once
#include "Vecteur2D.h"


class Cercle {
public:
	double rayon;
	Vecteur2D centre;

	Cercle(Vecteur2D centre, double rayon) : centre(centre), rayon(rayon) {}

	/**
	* Retourne vrai si ce cercle contient le sommet s
	*/
	bool contientPoint(Vecteur2D s) {
		return (s.x - centre.x) * (s.x - centre.x) + (s.y - centre.y) * (s.y - centre.y) </*=*/ rayon * rayon;
	}
};