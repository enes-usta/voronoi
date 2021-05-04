#pragma once
#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "Sommet.h"
#include "Triangle.h"
#include "Graphe.h"
#include "Cercle.h"
#include "Face.h"
#include "Triangulator.h"
#include "Geometrie.h"

#define MAX_ARRAY 100000

using namespace std;

/*
* Classe Voronoizer
* Prends en entre une liste de sommets
* et crée un diagramme de Voronoï
*
* T est la nature de l'information porte par une arête
* S est la nature de l'information porte par une face
*/
template <class S, class T>
class Voronoizer {
public:
	Voronoizer() {
		this->_germes = new vector<Sommet<Vecteur2D>*>;
		this->cellules = new vector<Face<S, T>*>;
	}
	~Voronoizer() {}

	/**
	* Retourne la liste traingule de la liste de sommets en entre
	*/
	vector<Face<S, T>*>* voronoize(vector<Sommet<Vecteur2D>*>* sommets, Graphe<T, Vecteur2D>* graphe) {
		this->graphe = graphe;
		Triangulator<S, T> triangulator;
		triangulation = triangulator.triangulate(sommets, graphe);
		calculer_germes();
		construire_cellules();
		return this->cellules;
	}

	vector<Sommet<Vecteur2D>*>* germes() {
		return this->_germes;
	}

private:
	vector<Sommet<Vecteur2D>*>* _germes;
	Graphe<T, Vecteur2D>* graphe;
	vector<Triangle<S, T>*>* triangulation;
	vector<Face<S, T>*>* cellules;


	void calculer_germes() {
		for (Triangle<S, T>* triangle : (*triangulation)) {
			Sommet<Vecteur2D>* s = graphe->creeSommet(triangle->cercle_circonscrit().centre);
			_germes->push_back(s);
		}
	}

	void construire_cellules() {
		bool fait[MAX_ARRAY] = { false };
		// Pour éviter les duplications 
		vector<ArcTU<T>*>* arcs_crees = new vector<ArcTU<T>*>;
		vector<Sommet<Vecteur2D>*>* sommets_crees = new vector<Sommet<Vecteur2D>*>;
		for (Triangle<S, T>* triangle : (*triangulation)) {
			for (ArcTU<T>* arc : triangle->arcs) {
				//Germe qu'on traite
				Sommet<Vecteur2D>* germe = arc->fin();
				// On vérifie si on a déjà traité ce germe
				if (!fait[germe->clef]) {
					pivoter_sur_germe(arc, triangle, sommets_crees, arcs_crees);
					fait[germe->clef] = true;
				}
			}
		}
	}

	/**
	* Créé un polygone autour de ce germe si c'est possible
	*/
	void pivoter_sur_germe(ArcTU<T>* arc, Triangle<S, T>* triangle, vector<Sommet<Vecteur2D>*>* sommets_crees, vector<ArcTU<T>*>* arcs_crees) {
		//Germe qu'on traite
		Sommet<Vecteur2D>* germe = arc->fin();
		//Cellules du diagramme de Voronoï
		vector<Sommet<Vecteur2D>*> sommets_cellule;
		// Sommet de départ
		Sommet<Vecteur2D>* centre_triangle = creer_sommet(triangle->cercle_circonscrit().centre, sommets_crees);
		Sommet<Vecteur2D>* depart = centre_triangle;
		sommets_cellule.push_back(centre_triangle);
		//On aborte dans certains cas
		bool aborted = false;
		
		while (true) {
			// Le triangle adjacent sur cet arc
			triangle = trouver_triangle_adjacent(arc);

			// Si pas de triangle adjacent, on ne peut pas créer de polygone
			if (triangle == NULL) {
				aborted = true;
				goto next;
			}

			// On décale à l'arc qui pointe vers le germe
			for (ArcTU<T>* a : triangle->arcs) {
				if (a->fin() == germe)
					arc = a;
			}

			// Centre circonscrit de ce triangle
			centre_triangle = creer_sommet(triangle->cercle_circonscrit().centre, sommets_crees);
			sommets_cellule.push_back(centre_triangle);

			// On arrête quand on a bouclé
			if (centre_triangle == depart)
				goto next;
		}
	next:;
		if(!aborted){
			int i = 0;
			vector<ArcTU<T>*> arcs_cellule;
			for (int i = 0; i < sommets_cellule.size() - 1; i++) {
				ArcTU<T>* nouvel_arc = creer_arc(sommets_cellule[i], sommets_cellule[i+1], arcs_crees);
				arcs_cellule.push_back(nouvel_arc);
			}

			this->cellules->push_back(new Face<S, T>(arcs_cellule, S()));
		}
	}


	/**
	* Crée un sommet en veillant à ce qu'il ne soit pas dupliqué
	*/
	Sommet<Vecteur2D>* creer_sommet(Vecteur2D sommet, vector<Sommet<Vecteur2D>*>* sommets_crees) {
		for (Sommet<Vecteur2D>* s : (*sommets_crees))
			if (s->v.x == sommet.x && s->v.y == sommet.y)
				return s;

		Sommet<Vecteur2D>* res = graphe->creeSommet(sommet);
		sommets_crees->push_back(res);
		return res;
	}

	/**
	* Crée un arc en veillant à ce qu'il ne soit pas dupliqué
	*/
	ArcTU<T>* creer_arc(Sommet<Vecteur2D>* deb, Sommet<Vecteur2D>* fin, vector<ArcTU<T>*>* arcs_crees) {
		for (ArcTU<T>* arc : (*arcs_crees))
			if (arc->arete->estEgal(deb, fin)) 
				return new ArcTU<T>(arc->arete, !arc->bonSens);

		ArcTU<T>* res = new ArcTU<T>(graphe->creeArete(T(), deb, fin), true);
		arcs_crees->push_back(res);
		return res;
	}


	/**
	* Retourne un triangle adjacent l'arc s'il en existe un, sinon retourne null
	*/
	Triangle<S, T>* trouver_triangle_adjacent(ArcTU<T>* arc) {

		for (Triangle<S, T>* triangleB : (*triangulation))
			for (ArcTU<T>* arcB : triangleB->arcs)
				if ((*arc) != (*arcB) && arc->arete->estEgal(arcB->debut(), arcB->fin()))
					return triangleB;

		return NULL;
	}

};
