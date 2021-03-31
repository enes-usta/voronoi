#pragma once
#include <vector>
#include "Sommet.h"
#include "Arc.h"

/**
* T est la nature de l'information portée par un sommet et
* S est la nature de l'information portée par une arête
*/
template <class S, class T>
class Face{
public:
	vector<Arc<S, T>> arcs;
	public Face(vector<Arc<S, T>> arcs) :arcs(arcs) {}

	/**
	* Retourne vrai si cette face contient le sommet s
	*/
	bool contientPoint(Sommet<S> s) {
		for each (Arc<S, T> arc in arcs)
			if (!arc.estAGauche(s))
				return false;

		return true;
	}

};