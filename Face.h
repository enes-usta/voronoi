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

	Face() {}

	Face(vector<ArcTU<S>> arcs) {
		this->arcs = arcs;
		if (this->arcs.size() < 3)
			throw "Une face est composée d'au moins 3 arcs!";
	}


	/**
	* Retourne vrai si cette face contient le sommet s
	*/
	bool contientPoint(Sommet<Vecteur2D> s) {
		for(ArcTU<S> arc : this->arcs)
			if (!arc.estAGauche(s))
				return false;
				
		return true;
	}

	friend bool operator==(const Face<S>& lhs, const Face<S>& rhs) {
		if (lhs.size() != rhs.size())
			return false;

		for (int i = 0; i < lhs.size(); i++)
			if (lhs.arcs[i].arete != rhs.arcs[i].arete)
				return false;

		return true;
	}
};