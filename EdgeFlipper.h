#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Face.h"
#include "Triangle.h"
#include "ArcTU.h"
#include "Vecteur2D.h"
#include "Graphe.h"

using namespace std;

/*
* Classe EdgeFlipper
* Prends en entre une triangulation
* et retourne une triangulation de Delaunay
* 
* S est la nature de l'information porte par un triangle
* T est la nature de l'information porte par une arête
*/
template <class S, class T>
class EdgeFlipper {
public:
	EdgeFlipper() { }
	~EdgeFlipper() {}

	vector<Triangle<S, T>*>* triangulation;
	Graphe<T, Vecteur2D>* graphe;

	vector<Triangle<S, T>*>* EdgeFlipDelaunay(vector<Triangle<S, T>*>* triangulation, Graphe<T, Vecteur2D>* graphe) {
		this->triangulation = triangulation;
		this->graphe = graphe;

		bool amelioration = true;

		while (amelioration) {
			amelioration = false;
			for (Triangle<S, T>* triangle : (*(this->triangulation))) {
				for (ArcTU<T> *arc : triangle->arcs) {
					Triangle<S, T>* triangle_adjacent = trouver_triangle_adjacent(arc);
					if (triangle_adjacent != NULL) {
						cout << "ici" << endl;
						ArcTU<T>* arc2 = trouver_arc_adjacent(arc, triangle_adjacent);
						if (shouldFlip(triangle, triangle_adjacent, arc, arc2)) {
							flip(triangle, triangle_adjacent, arc, arc2);
							amelioration = true;
							goto next;
						}
					}
				}
			}
		next: ;
		}

		return triangulation;
	}

	/**
	* Retourn vrai si on devrait flip l'arête commune aux deux triangles compris dans les deux arcs
	*/
	bool shouldFlip(Triangle<S, T>* t, Triangle<S, T>* t2, ArcTU<T>* arc, ArcTU<T>* arc2) {
		Sommet<Vecteur2D>* d = t2->sommet_oppose(arc2);

		return t->cercle_circonscrit().contientPoint(d->v);
	}

	/**
	* Retourne vrai si on devrait flip (si le cercle circonscrit du triangle abc contient d)
	*/
	bool shouldFlip(Sommet<Vecteur2D>* a, Sommet<Vecteur2D>* b, Sommet<Vecteur2D>* c, Sommet<Vecteur2D>* d) {
		return Cercle(a->v, b->v, c->v).contientPoint(d->v);
	}

	/**
	* Flip l'arête commune aux deux triangles compris dans les deux arcs
	* Supprime les deux triangles concernés et en crée deux nouveaux dans la triangulation 
	*/
	void flip(Triangle<S, T>* triangle, Triangle<S, T>* triangle2, ArcTU<T>* arc, ArcTU<T>* arc2){
		Sommet<Vecteur2D>* a, * b, * c, * d;

		a = arc->debut();
		b = arc->fin();
		c = triangle->sommet_oppose(arc);
		d = triangle2->sommet_oppose(arc2);

		supprimer_triangle(triangle);
		supprimer_triangle(triangle2);

		Arete<T, Vecteur2D>* bc = graphe->creeArete(T(), b, c);
		Arete<T, Vecteur2D>* cd = graphe->creeArete(T(), c, d);
		Arete<T, Vecteur2D>* db = graphe->creeArete(T(), d, b);
		Arete<T, Vecteur2D>* ca = graphe->creeArete(T(), c, a);
		Arete<T, Vecteur2D>* ad = graphe->creeArete(T(), a, d);

		triangulation->push_back(new Triangle<S, T>(new ArcTU<T>(bc, true), new ArcTU<T>(cd, true), new ArcTU<T>(db, true)));
		triangulation->push_back(new Triangle<S, T>(new ArcTU<T>(ca, true), new ArcTU<T>(ad, true), new ArcTU<T>(cd, false)));
	}

	/**
	* Retourne un triangle adjacent l'arc s'il en existe un, sinon retourne null
	*/
	Triangle<S, T>* trouver_triangle_adjacent(ArcTU<T>* arc) {

		for (Triangle<S, T>* triangle : (*triangulation))
			for (ArcTU<T>* arcB : triangle->arcs)
				if ((*arc) != (*arcB) && arc->arete->estEgal(arcB->debut(), arcB->fin()))
					return triangle;

		return NULL;
	}

	/**
	* Retourne l'arc adjacent à l'arc dans le triangle triangle
	*/
	ArcTU<T>* trouver_arc_adjacent(ArcTU<T>* arc, Triangle<S, T>* triangle) {

		for (ArcTU<T>* arcB : triangle->arcs)
			if (arc->arete->estEgal(arcB->arete->debut, arcB->arete->fin))
				return arcB;

		return NULL;
	}

	void supprimer_triangle(Triangle<S, T>* triangle) {
		int i = 0;
		for (Triangle<S, T>* t : (*triangulation)) {
			if (t == triangle) {
				triangulation->erase((triangulation->begin() + i));
				delete triangle;
				break;
			}
			i++;
		}
	}
};
