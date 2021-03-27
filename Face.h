#pragma once
#include <vector>
#include "Sommet.h"
#include "Arc.h"

template <class S, class T, class U>
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
	* Retourne vrai si le sommet s est à gauche de l'arête a
	*/
	bool estAGauche(Sommet s, Arc arc) {
		Sommet a = arc.debut;
		Sommet b = arc.fin;
		if (arc.sens == 1) {
			a = arc.fin;
			b = arc.debut;
		}

		return (b.x - a.x) * (s.y - a.y) - (s.x - a.x) * (b.y - a.y);
	}
};