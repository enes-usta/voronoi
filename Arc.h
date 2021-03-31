#pragma once
#include "Arete.h"

/**
* T est la nature de l'information portée par un sommet et
* S est la nature de l'information portée par une arête
*/
template <class S, class T>
class Arc {
public:
	Arete<S, T> *arete;
	bool bonSens; // vrai si dans le bon sens
	Arc(Arete<S, T>* arete, int bonSens): arete(arete), bonSens(bonSens){}

	/**
	* Retourne vrai si le sommet s est à gauche de cet arc
	*/
	bool estAGauche(Sommet<T> s) {
		Sommet<T> a = arete->debut.v;
		Sommet<T> b = arete->fin.v;
		if (!bonSens) {
			a = arete->fin.v;
			b = arete->debut.v;
		}

		return (b.x - a.x) * (s.y - a.y) - (s.x - a.x) * (b.y - a.y);
	}

};