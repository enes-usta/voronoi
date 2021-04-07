#pragma once
#include "Arete.h"

/**
* S est la nature de l'information portée par une arête
*/
template <class S>
class ArcTU {
public:
	Arete<S, Vecteur2D> *arete;
	bool bonSens; // vrai si dans le bon sens
	ArcTU(Arete<S, Vecteur2D>* arete, int bonSens): arete(arete), bonSens(bonSens){}

	/**
	* Retourne vrai si le sommet s est à gauche de cet arc
	*/
	bool estAGauche(Sommet<Vecteur2D> s) {
		Vecteur2D a = arete->debut->v;
		Vecteur2D b = arete->fin->v;
		if (!bonSens) {
			a = arete->fin->v;
			b = arete->debut->v;
		}

		return (b.x - a.x) * (s.v.y - a.y) - (s.v.x - a.x) * (b.y - a.y);
	}

};