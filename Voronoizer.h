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
		construire_aretes();
		return (vector<Face<S, T>*>*) triangulation; // à changer
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
		vector<ArcTU<T>*>* arcs_crees = new vector<ArcTU<T>*>;
		vector<Sommet<Vecteur2D>*>* sommets_crees = new vector<Sommet<Vecteur2D>*>;
		bool fait[triangulation->size() * 3 + 10] = { false };

		for (Triangle<S, T>* triangle : (*triangulation)) {
			//Si on tombe sur un arc sans triangle adjacent, on aborte
			bool aborted = false;
			// Centre circonscrit de ce triangle
			Sommet<Vecteur2D>* centre_triangle = creer_sommet(triangle->cercle_circonscrit().centre, sommets_crees);
			for (ArcTU<T>* arc : triangle->arcs) {
				vector<ArcTU<T>*>* arcs_cellule = new vector<ArcTU<T>*>;
				while (true) {
					//Germe qu'on traite
					Sommet<Vecteur2D>* germe = arc->debut();
					// On vérifie si on a déjà traité ce germe
					if (fait[germe])
						goto next;

					// Le triangle adjacent sur cet arc
					Triangle<S, T>* triangle_adjacent = trouver_triangle_adjacent(arc);
					
					// Si pas de triangle adjacent, on ne peut pas créer de polygone
					if (triangle_adjacent == NULL) {
						aborted = true;
						goto next;
					}

					// Le centre de son cercle circonscrit
					Sommet<Vecteur2D>* centre_triangle_adjacent = creer_sommet(triangle_adjacent->cercle_circonscrit().centre);

					// On crée l'arc
					bool bonSens = Geometrie::aGauche(centre_triangle->v, centre_triangle_adjacent->v, germe->v);
					ArcTU<T>* nouvel_arc = creer_arc(centre_triangle, centre_triangle_adjacent, bonSens);

					arcs_cellule->push_back(nouvel_arc);

					if (nouvel_arc->fin() == arcs_cellule[0]->debut())
						goto next;
				}
			next:;
				if (aborted) {
					for (auto a : (*arcs_cellule))
						delete a;
					arcs_cellule->clear();
				}
				else {
					cellules->push_back(new Face<S, T>(arcs_cellule));
				}
			}
		
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
	ArcTU<T>* creer_arc(Sommet<Vecteur2D> deb, Sommet<Vecteur2D> fin, bool bonSens, vector<ArcTU<T>*>* arcs_crees) {
		for (ArcTU<T>* arc : (*arcs_crees))
			if (arc->arete->estEgal(arete->debut, arete->fin)) 
				return new ArcTU<T>(arc->arete, !arc->bonSens);

		ArcTU<T>* res = new ArcTU<T>(arete, bonsens);
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
