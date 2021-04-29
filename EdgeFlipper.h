#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Face.h"
#include "Triangle.h"
#include "ArcTU.h"
#include "Vecteur2D.h"
#include "Graphe.h"
#include "Geometrie.h"

using namespace std;

/*
* Classe EdgeFlipper
* Prends en entre une triangulation
* et retourne une triangulation de Delaunay
* 
* S est la nature de l'information porte par un triangle
* T est la nature de l'information porte par une ar�te
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
		int cpt = 0;

		while (amelioration && cpt < 1000) {
			amelioration = false;
			cpt++;
			for (Triangle<S, T>* triangle : (*(this->triangulation))) {
				for (ArcTU<T> *arc : triangle->arcs) {
					Triangle<S, T>* triangle_adjacent = trouver_triangle_adjacent(arc);
					if (triangle_adjacent != NULL) {
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
	* Retourn vrai si on devrait flip l'ar�te commune aux deux triangles compris dans les deux arcs
	*/
	bool shouldFlip(Triangle<S, T>* triangle, Triangle<S, T>* triangle2, ArcTU<T>* arc, ArcTU<T>* arc2) {
		Sommet<Vecteur2D>* d = triangle2->sommet_oppose(arc2);

		return (triangle->cercle_circonscrit().contientPoint(d->v) && forment_polygone_convexe(triangle, triangle2, arc, arc2));
	}

	/**
	* Retourne vrai si on devrait flip (si le cercle circonscrit du triangle abc contient d)
	*/
	bool shouldFlip(Sommet<Vecteur2D>* a, Sommet<Vecteur2D>* b, Sommet<Vecteur2D>* c, Sommet<Vecteur2D>* d) {
		return (Cercle(a->v, b->v, c->v).contientPoint(d->v) && forment_polygone_convexe(a, b, c, d));
	}

	/**
	* Flip l'ar�te commune aux deux triangles compris dans les deux arcs
	* Supprime les deux triangles concern�s et en cr�e deux nouveaux dans la triangulation 
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
				if((*arc) != (*arcB))
					if (arc->arete->estEgal(arcB->arete->debut, arcB->arete->fin))
						return triangle;

		return NULL;
	}

	/**
	* Retourne l'arc adjacent � l'arc dans la triangulation s'il en existe un
	*/
	ArcTU<T>* trouver_arc_adjacent(ArcTU<T>* arc) {
		Triangle<S, T>* triangle = trouver_triangle_adjacent(arc);
		if (triangle != NULL) 
			for (ArcTU<T>* arcB : triangle->arcs)
				if (arc->arete->estEgal(arcB->arete->debut, arcB->arete->fin))
					return arcB;
		
		return NULL;
	}

	/**
	* Retourne l'arc adjacent � l'arc dans le triangle triangle
	*/
	ArcTU<T>* trouver_arc_adjacent(ArcTU<T>* arc, Triangle<S, T>* triangle) {

		for (ArcTU<T>* arcB : triangle->arcs)
			if (arc->arete->estEgal(arcB->arete->debut, arcB->arete->fin))
				return arcB;

		return NULL;
	}

	/**
	* Retourne vrai si les deux triangles forment un polygone convexe
	*/
	bool forment_polygone_convexe(Triangle<S, T>* triangle, Triangle<S, T>* triangle2, ArcTU<T>* arc, ArcTU<T>* arc2) {
		Sommet<Vecteur2D>* a, * b, * c, * d;

		a = arc->debut();
		b = arc->fin();
		c = triangle->sommet_oppose(arc);
		d = triangle2->sommet_oppose(arc2);

		return Geometrie::forment_polygone_convexe(a->v, b->v, c->v, d->v);
	}

	
	/**
	* Supprime le triangle de la triangulation
	*/
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


	bool les_arcs_sont_bien_orientes() {
		int cpt = 0;
		for (Triangle<S, T>* triangle : (*triangulation)) {
			for (ArcTU<T>* arc : triangle->arcs) {
				ArcTU<T>* arc2 = trouver_arc_adjacent(arc);
				if (arc2 != NULL) {
					cpt++;
					if (arc->bonSens == arc2->bonSens)
						return false;
				}	
			}
		}

		return cpt > 0;
	}
};
