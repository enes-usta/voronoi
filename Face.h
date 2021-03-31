#pragma once
#include <vector>
#include "Arc.h"

/**
* S est la nature de l'information portée par une arête
*/
template <class S>
class Face{
public:
	vector<Arc<S>> arcs;
	Face(vector<Arc<S>> arcs) {
		this->arcs = arcs;
		if (this->arcs.size() < 3)
			throw "Une face est composée d'au moins 3 arcs!";
	}

	/**
	* Retourne vrai si cette face contient le sommet s
	*/
	bool contientPoint(Sommet<Vecteur2D> s) {
		for each (Arc<S> arc in this->arcs)
			if (arc.estAGauche(s))
				return false;
				
		return true;
	}

};