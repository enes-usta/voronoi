#pragma once
#include "Sommet.h"
#include "Vecteur2D.h"


class Cercle {
public:
	double rayon;
	Sommet<Vecteur2D> centre;

	public Cercle(Sommet<Vecteur2D> centre, double rayon) : centre(centre), rayon(rayon) {}

	/**
	* Retourne vrai si ce cercle contient le sommet s
	*/
	bool contientPoint(Sommet<Vecteur2D> s) {
		return (s.x - centre.v.x) * (s.x - centre.v.x) + (s.y - centre.v.y) * (s.y - centre.v.y) < rayon * rayon;
	}
};