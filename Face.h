#pragma once
#include <vector>
#include "Sommet.h"
#include "Arc.h"

/**
* T est la nature de l'information port�e par un sommet et
* S est la nature de l'information port�e par une ar�te
*/
template <class S, class T>
class Face{
public:
	vector<Arc<S, T>> arcs;
	public Face(vector<Arc<S, T>> arcs) :arcs(arcs) {}

	/**
	* Retourne vrai si cette face contient le sommet s
	*/
	bool contientPoint(Sommet s) {
		for each (Arc arc in arcs)
		{
			if (!estAGauche(s, arc))
				return false;
		}
		return true;
	}

	/**
	* Retourne vrai si le sommet s est � gauche de l'ar�te a
	*/
	bool estAGauche(Sommet s, Arc arc) {
		Sommet<T> a = arc.debut;
		Sommet<T> b = arc.fin;
		if (arc.sens == 1) {
			a = arc.fin;
			b = arc.debut;
		}

		return (b.x - a.x) * (s.y - a.y) - (s.x - a.x) * (b.y - a.y);
	}
};