#pragma once
#include "Arete.h"
#include "Sommet.h"
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
	* Retourne vrai si le sommet s est à gauche de cet arc (ou s'il est collinéaire)
	*/
	bool estAGauche(Sommet<Vecteur2D> *s) {
		Vecteur2D a = debut()->v;
		Vecteur2D b = fin()->v;

		return (b.x - a.x) * (s->v.y - a.y) - (s->v.x - a.x) * (b.y - a.y) >= 0;
	}

	/**
	* Retourne vrai si le sommet s est à gauche de cet arc
	*/
	bool estCollineaire(Sommet<Vecteur2D>* s) {
		Vecteur2D a = debut()->v;
		Vecteur2D b = fin()->v;

		return (b.x - a.x) * (s->v.y - a.y) - (s->v.x - a.x) * (b.y - a.y) == 0;
	}

	/**
	* Retourne le début de l'arc
	*/
	Sommet<Vecteur2D>* debut() {
		if (this->bonSens)
			return arete->debut;
		else
			return arete->fin;
	}

	/**
	* Retourne la fin de l'arc
	*/
	Sommet<Vecteur2D>* fin() {
		if (this->bonSens)
			return arete->fin;
		else
			return arete->debut;
	}

	friend bool operator==(const ArcTU<S>& lhs, const ArcTU<S>& rhs) {
		return (lhs.arete->estEgal(rhs.arete->debut, rhs.arete->fin) && lhs.bonSens == rhs.bonSens);
	}

};