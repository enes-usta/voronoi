#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Face.h"
#include "Triangle.h"
#include "ArcTU.h"


using namespace std;

/*
* Classe EdgeFlipper
* Prends en entre une triangulation
* et retourne une triangulation de Delaunay
*
* S est la nature de l'information porte par une arête
*/
template <class S>
class EdgeFlipper {
public:
	EdgeFlipper() { }
	~EdgeFlipper() {}

	vector<Triangle<S>*>* triangulation;

	vector<Triangle<S>*>* EdgeFlipDelaunay(vector<Triangle<S>*>* triangulation) {
		this->triangulation = triangulation;
		bool amelioration = true;

		while (amelioration) {
			amelioration = false;
			for (Triangle<S> *t : (*triangulation)) 
				for (ArcTU<S> a : t->arcs) 
					for (Triangle<S>* t2 : (*triangulation))
						for (ArcTU<S> a2 : t->arcs) {
							if (shouldFlip(a, a2))
								flip(a, a2);
								amelioration = true;
								goto next;
							}

			next:
		}

		return triangulation;
	}

	bool shouldFlip(ArcTU a, ArcTU a2) {
		return false;
	}

	void flip(ArcTU a, ArcTU a2) {
		return true;
	}
};
