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
	int sens;
	public Arc(Arete<S, T>* arete, int sens): arete(arete), sens(sens){}

	/**
	* Retourne vrai si le sommet s est à gauche de cet arc
	*/
	bool estAGauche(Sommet<T> s) {
		Sommet<T> a = this.arete->debut.v;
		Sommet<T> b = this.arete->fin.v;
		if (this.sens == 1) {
			a = this.arete->fin.v;
			b = this.arete->debut.v;
		}

		return (b.x - a.x) * (s.y - a.y) - (s.x - a.x) * (b.y - a.y);
	}

};