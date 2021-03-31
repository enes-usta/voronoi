#pragma once
#include "Arete.h"

/**
* S est la nature de l'information port�e par une ar�te
*/
template <class S>
class Arc {
public:
	Arete<S, Vecteur2D> *arete;
	bool bonSens; // vrai si dans le bon sens
	Arc(Arete<S, Vecteur2D>* arete, int bonSens): arete(arete), bonSens(bonSens){}

	/**
	* Retourne vrai si le sommet s est � gauche de cet arc
	*/
	bool estAGauche(Sommet<Vecteur2D> s) {
		Sommet<Vecteur2D> a = arete->debut.v;
		Sommet<Vecteur2D> b = arete->fin.v;
		if (!bonSens) {
			a = arete->fin.v;
			b = arete->debut.v;
		}

		return (b.x - a.x) * (s.v.y - a.y) - (s.v.x - a.x) * (b.y - a.y);
	}

};