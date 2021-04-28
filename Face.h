#pragma once
#include <vector>
#include "ArcTU.h"

/**
* T est la nature de l'information portée par une arête
* S est la nature de l'information portée par cette face
*/
template <class S, class T>
class Face{
public:
	S v;
	vector<ArcTU<T>*> arcs;

	Face() {}

	Face(vector<ArcTU<T>*> arcs, const S& v): v(v){
		this->arcs = arcs;
		if (this->arcs.size() < 3)
			throw "Une face est composée d'au moins 3 arcs!";
	}


	/**
	* Retourne vrai si cette face contient le sommet s
	*/
	bool contientPoint(Sommet<Vecteur2D> *s) {
		for(ArcTU<T>* arc : this->arcs)
			if (!arc->estAGauche(s))
				return false;
				
		return true;
	}

	friend bool operator==(const Face<S, T>& lhs, const Face<S, T>& rhs) {
		if (lhs.size() != rhs.size())
			return false;

		for (int i = 0; i < lhs.size(); i++)
			if (lhs.arcs[i]->arete != rhs.arcs[i]->arete)
				return false;

		return true;
	}

	void print() {
			for (ArcTU<T>* a : arcs) {
				cout << "deb(" << a->debut()->v.x << ", " << a->debut()->v.y << ") ";
				cout << "fin(" << a->fin()->v.x << ", " << a->fin()->v.y << ")" << endl;;
			}
			cout << endl;
			cout << endl;

	}
};