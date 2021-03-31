#pragma once
#include "Sommet.h"

/**
 * T est la nature de l'information portée par un sommet et
 */
template<class T>
class Cercle {
public:
	Sommet<T> _centre;
	double _rayon;

	Cercle(Sommet<T> centre, double rayon) : _centre(centre), _rayon(rayon) {}


	/**
	 * Retourne vrai si ce cercle contient le sommet s
	 */
	bool contientPoint(Sommet<T> s) {
		return (s.x - centre.v.x) * (s.x - centre.v.x) + (s.y - centre.v.y) * (s.y - centre.v.y) < rayon * rayon;
	}
};