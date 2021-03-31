#pragma once
#include <vector>
#include "Arc.h"

/**
* T est la nature de l'information portée par un sommet et
* S est la nature de l'information portée par une arête
*/
template <class S, class T>
class Face{
public:
	vector<Arc<S, T>> arcs;
	Face(vector<Arc<S, T>> arcs) {
		if (arcs.size < 3)
			throw "Une face est composée d'au moins 3 arcs!";
		this->arcs = arcs;
	}

	/**
	* Retourne vrai si cette face contient le sommet s
	*/
	bool contientPoint(Sommet<S> s) {
		for each (Arc<S, T> arc in this->arcs)
			if (!arc.estAGauche(s))
				return false;

		return true;
	}

};