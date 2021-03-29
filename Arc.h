#pragma once
#include "Arete.h"

/**
* T est la nature de l'information portée par un sommet et
* S est la nature de l'information portée par une arête
*/
template <class S, class T>
class Arc {
public:
	int sens;
	Arete<S, T> arete;
	public Arc(Arete<S, T> arete, int sens) :super(arete), sens(sens){}

	/**
	* Retourne vrai si le sommet s est à gauche de cet arc
	*/
	bool estAGauche(Sommet<T> s) {
		Sommet<T> a = this.arete.debut;
		Sommet<T> b = this.arete.fin;
		if (this.arete.sens == 1) {
			a = this.arete.fin;
			b = this.arete.debut;
		}

		return (b.x - a.x) * (s.y - a.y) - (s.x - a.x) * (b.y - a.y);
	}

};