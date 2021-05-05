#pragma once
#include <vector>
#include "ArcTU.h"
#include "Geometrie.h"

/**
* T est la nature de l'information portée par une arête
* S est la nature de l'information portée par cette face
*/
template <class S, class T>
class Face{
public:
	S v;
	vector<ArcTU<T, S>*> arcs;

	Face() {}

	Face(vector<ArcTU<T, S>*> arcs, const S& v): v(v){
		this->arcs = arcs;
		if (this->arcs.size() < 3)
			throw "Une face est composée d'au moins 3 arcs!";
		
		for (ArcTU<T, S>* arc : arcs)
			arc->face = this;
	}

	~Face() {
		for (ArcTU<T, S>* arc : arcs)
			delete arc;
	}

	/**
	* Retourne vrai si cette face contient le sommet s
	*/
	bool contientPoint(Sommet<Vecteur2D>* s) {
		for (ArcTU<T, S>* arc : this->arcs)
			if (!(arc->estAGauche(s) || arc->estCollineaire(s)))
				return false;

		return true;
	}

	/**
	* Retourne vrai si cette face contient le sommet s
	*/
	bool contientPoint(Vecteur2D s) {
		for (ArcTU<T, S>* arc : this->arcs)
			if (!(Geometrie::aGaucheOuCollineaire(arc->debut()->v, arc->fin()->v, s)))
				return false;

		return true;
	}

	/**
	* Retourne vrai si cette face contient le sommet s
	*/
	bool contientPointStrict(Sommet<Vecteur2D> *s) {
		for(ArcTU<T, S>* arc : this->arcs)
			if (!arc->estAGauche(s))
				return false;
				
		return true;
	}

	friend bool operator==(const Face<S, T>& lhs, const Face<S, T>& rhs) {
		if (lhs.arcs.size() != rhs.arcs.size())
			return false;

		for (int i = 0; i < lhs.arcs.size(); i++)
			if (!(lhs.arcs[i]->arete->estEgal(rhs.arcs[i]->arete->debut, rhs.arcs[i]->arete->fin))
				|| lhs.arcs[i]->bonSens != rhs.arcs[i]->bonSens)
				return false;

		return true;
	}

	void print() {
			for (ArcTU<T, S>* a : arcs) {
				cout << "deb(" << a->debut()->v.x << ", " << a->debut()->v.y << ") ";
				cout << "fin(" << a->fin()->v.x << ", " << a->fin()->v.y << ")" << endl;;
			}
			cout << endl;
			cout << endl;

	}
};