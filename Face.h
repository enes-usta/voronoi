#pragma once
#include <vector>
#include "ArcTU.h"

/**
* S est la nature de l'information portée par une arête
*/
template <class S>
class Face{
public:
	vector<ArcTU<S>> arcs;
	Face(vector<ArcTU<S>> arcs) {
		this->arcs = arcs;
		if (this->arcs.size() < 3)
			throw "Une face est composée d'au moins 3 arcs!";
	}

	/**
	* Retourne vrai si cette face contient le sommet s
	*/
	bool contientPoint(Sommet<Vecteur2D> s) {
		//for each (ArcTU<S, Vecteur2D> arc in this->arcs)
		for(ArcTU<S, Vecteur2D> arc : this->arcs)
			if (!arc.estAGauche(s))
				return false;
				
		return true;
	}
};