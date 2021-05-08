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
		this->germes = new vector<Sommet<Vecteur2D>*>;
		this->cellules = new vector<Face<S, T>*>;
	}
	~Voronoizer() {}

	/**
	* Retourne la liste traingule de la liste de sommets en entre
	*/
	vector<Face<S, T>*>* voronoize(vector<Sommet<Vecteur2D>*>* sommets, Graphe<T, Vecteur2D>* graphe) {
		this->graphe = graphe;
		triangulator = new Triangulator<S, T>;
		triangulation = triangulator->triangulate(sommets, graphe);
		construire_cellules();
		//clipping();
		return this->cellules;
	}

public:
	vector<Sommet<Vecteur2D>*>* germes;
private:
	Graphe<T, Vecteur2D>* graphe;
	Triangulator<S, T>* triangulator;
	vector<Triangle<S, T>*>* triangulation;
	vector<Face<S, T>*>* cellules;


	void construire_cellules() {
		bool fait[MAX_ARRAY] = { false };
		// Pour éviter les duplications 
		vector<ArcTU<T, S>*>* arcs_crees = new vector<ArcTU<T, S>*>;
		vector<Sommet<Vecteur2D>*>* sommets_crees = new vector<Sommet<Vecteur2D>*>;
		for (Triangle<S, T>* triangle : (*triangulation)) {
			for (ArcTU<T, S>* arc : triangle->arcs) {
				//Germe qu'on traite
				Sommet<Vecteur2D>* germe = arc->fin();
				// On vérifie si on a déjà traité ce germe
				if (!fait[germe->clef]) {
					germes->push_back(germe);
					pivoter_sur_germe(arc, triangle, sommets_crees, arcs_crees);
					fait[germe->clef] = true;
				}
			}
		}
	}

	/**
	* Créé un polygone autour de ce germe si c'est possible
	*/
	void pivoter_sur_germe(ArcTU<T, S>* arc, Triangle<S, T>* triangle, vector<Sommet<Vecteur2D>*>* sommets_crees, vector<ArcTU<T, S>*>* arcs_crees) {
		//Germe qu'on traite
		Sommet<Vecteur2D>* germe = arc->fin();
		//Cellules du diagramme de Voronoï
		vector<Sommet<Vecteur2D>*>* sommets_cellule = new vector<Sommet<Vecteur2D>*>;
		// Sommet de départ
		Triangle<S, T>* triangle_traite = triangle;
		//sommets_cellule.push_back(centre_triangle);
		//On aborte dans certains cas
		bool aborted = false;
		
		while (true) {

			if (trouver_triangle_adjacent(arc) == NULL) {
				// On ajoute le centre à la liste des sommets de la face
				ajouter_sommet_cellule(triangle_traite, germe, sommets_cellule, sommets_crees);

				// On arrête quand on a bouclé
				if (sommets_cellule->size() > 2
					&& (sommets_cellule->back() == sommets_cellule->front() || sommets_cellule->at(sommets_cellule->size() - 2) == sommets_cellule->front()))
					goto next;

				triangle_traite = traiter_cellule_infinie(triangle, arc, germe, sommets_cellule, sommets_crees);
				//aborted = true;
				//goto next;
			}
			else {
				// On ajoute le centre à la liste des sommets de la face
				ajouter_sommet_cellule(triangle_traite, germe, sommets_cellule, sommets_crees);

				// On arrête quand on a bouclé
				if (sommets_cellule->size() > 2 
					&& (sommets_cellule->back() == sommets_cellule->front() || sommets_cellule->at(sommets_cellule->size()-2) == sommets_cellule->front()))
					goto next;

				triangle_traite = trouver_triangle_adjacent(arc);		
			}

			// On décale à l'arc qui pointe vers le germe
			for (ArcTU<T, S>* a : triangle_traite->arcs)
				if (a->fin() == germe)
					arc = a;

		}
	next:;
		if (!aborted) {
			vector<ArcTU<T, S>*> arcs_cellule;
			Face<S, T>* face = new Face<S, T>();
			for (int i = 0; i < sommets_cellule->size() - 1; i++) {
				if (sommets_cellule->at(i) != sommets_cellule->at(i + 1)) {
					ArcTU<T, S>* nouvel_arc = creer_arc(sommets_cellule->at(i), sommets_cellule->at(i + 1), face, arcs_crees);
					arcs_cellule.push_back(nouvel_arc);
				}
				
			}
			face->arcs = arcs_cellule;
			this->cellules->push_back(face);
		}
		
	}

	/**
	*	Ajoute les sommets de la cellule de Voronoï générée par germe sur ce triangle
	*/
	void ajouter_sommet_cellule(Triangle<S, T>* triangle, Sommet<Vecteur2D>* germe, vector<Sommet<Vecteur2D>*>* sommets_cellule, vector<Sommet<Vecteur2D>*>* sommets_crees) {
		Sommet<Vecteur2D>* centre_triangle = creer_sommet(triangle->cercle_circonscrit().centre, sommets_crees);
		bool centre_dehors = false;
		for (ArcTU<T, S>* a : triangle->arcs) {
			if (trouver_triangle_adjacent(a) == NULL && !a->estAGauche(centre_triangle)) {
				// Cellule complète, on ajoute les deux intersections sur la l'arc de la bordure
				if (a->debut() != germe && a->fin() != germe) {
					for (ArcTU<T, S>* a2 : triangle->arcs) {
						if (a2->debut() == germe) {
							Triangle<S, T>* triangle_adjacent = trouver_triangle_adjacent(a2);
							Vecteur2D inter = Geometrie::intersection(a->debut()->v, a->fin()->v, triangle_adjacent->cercle_circonscrit().centre, centre_triangle->v);
							sommets_cellule->push_back(creer_sommet(inter, sommets_crees));
						}
					}
					// Si on a pas encore bouclé
					if (sommets_cellule->front() != sommets_cellule->back()) {
						for (ArcTU<T, S>* a2 : triangle->arcs) {
							if (a2->fin() == germe) {
								Triangle<S, T>* triangle_adjacent = trouver_triangle_adjacent(a2);
								Vecteur2D inter = Geometrie::intersection(a->debut()->v, a->fin()->v, triangle_adjacent->cercle_circonscrit().centre, centre_triangle->v);
								sommets_cellule->push_back(creer_sommet(inter, sommets_crees));
							}
						}
					}
					
					//sommets_cellule->push_back(creer_sommet(Vecteur2D((a->debut()->v + a->fin()->v) / 2), sommets_crees));
				}
				//cellule infinie
				else {
					for (ArcTU<T, S>* a2 : triangle->arcs) {
						Triangle<S, T>* triangle_adjacent = trouver_triangle_adjacent(a2);
						if ((a2->fin() == germe || a2->debut() == germe) && triangle_adjacent != NULL) {
							Vecteur2D inter = Geometrie::intersection(a->debut()->v, a->fin()->v, triangle_adjacent->cercle_circonscrit().centre, centre_triangle->v);
							sommets_cellule->push_back(creer_sommet(inter, sommets_crees));
						}
					}
					
					//sommets_cellule->push_back(creer_sommet(Vecteur2D((a->debut()->v + a->fin()->v) / 2), sommets_crees));
				}
				centre_dehors = true;
			}
		}

		if (!centre_dehors)
			sommets_cellule->push_back(centre_triangle);
	}

	bool dehors() {
		return false;
	}

	/**
	*	Traite un cas d'exception:
	*	On essaye de pivoter sur un germe mais on tombe sur un arc qui n'a pas de triangle adjacent
	*	Si il n'y a pas de triangle adjacent, on ne peut pas créer de polygone complet, c'est une cellule infinie
	*/
	Triangle<S, T>* traiter_cellule_infinie(Triangle<S, T>* triangle, ArcTU<T, S>* arc, Sommet<Vecteur2D>* germe, vector<Sommet<Vecteur2D>*>* sommets_cellule, vector<Sommet<Vecteur2D>*>* sommets_crees) {
		Sommet<Vecteur2D>* milieu_arc = creer_sommet(Vecteur2D((arc->debut()->v + arc->fin()->v) / 2), sommets_crees);
		sommets_cellule->push_back(milieu_arc);
		sommets_cellule->push_back(germe);

		Triangle<S, T>* triangle_adjacent = triangle;
		Triangle<S, T>* triangle_traite = triangle_adjacent;
		while (triangle_adjacent != NULL) {
			// On parcourt les triangles adjacents dans le sens horaire jusqu'à arriver au triangle extrême
			for (ArcTU<T, S>* a : triangle_adjacent->arcs) {
				if (a->debut() == germe) {
					triangle_adjacent = trouver_triangle_adjacent(arc);
					if (triangle_adjacent != NULL)
						triangle_traite = triangle_adjacent;
					break;
				}
			}
		}

		// On décale à l'arc qui part du germe
		for (ArcTU<T, S>* a : triangle_traite->arcs) {
			if (a->debut() == germe) {
				milieu_arc = creer_sommet(Vecteur2D((a->debut()->v + a->fin()->v) / 2), sommets_crees);
				sommets_cellule->push_back(milieu_arc);
			}
		}

		return triangle_traite;
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
	ArcTU<T, S>* creer_arc(Sommet<Vecteur2D>* deb, Sommet<Vecteur2D>* fin, Face<S, T>* face, vector<ArcTU<T, S>*>* arcs_crees) {
		for (ArcTU<T, S>* arc : (*arcs_crees))
			if (arc->arete->estEgal(deb, fin)) {
				ArcTU<T, S>* nouvel_arc = new ArcTU<T, S>(arc->arete, !arc->bonSens);
				nouvel_arc->face_adjacente = arc->face;
				nouvel_arc->arc_adjacent = arc;
				nouvel_arc->face = face;

				arc->face_adjacente = face;
				arc->arc_adjacent = nouvel_arc;
				return nouvel_arc;
			}

		ArcTU<T, S>* res = new ArcTU<T, S>(graphe->creeArete(T(), deb, fin), true);
		res->face = face;
		arcs_crees->push_back(res);
		return res;
	}


	/**
	* Retourne un triangle adjacent l'arc s'il en existe un, sinon retourne null
	*/
	Triangle<S, T>* trouver_triangle_adjacent(ArcTU<T, S>* arc) {
		return (Triangle<S, T>*)arc->face_adjacente;
	}


	/**
	* Supprime les faces et les sommets qui sont à l'extérieur du rectangle
	* décrit par les sommets extrêmes parmi les sommets à construire
	*/
	void clipping() {
		// On supprime les faces
		for (auto it = cellules->begin(); it != cellules->end(); ) {
			bool deleted = false;
			Face<S, T>* f = (Face<S, T>*) * it;
			for (ArcTU<T, S>* arc : (f->arcs)) {
				if (arc->debut()->v.x < triangulator->left || arc->debut()->v.x > triangulator->right
					|| arc->debut()->v.y < triangulator->bottom || arc->debut()->v.y > triangulator->top) {
					delete* it;
					it = cellules->erase(it);
					deleted = true;
					goto next;
				}
			}
		next:
			if (!deleted)
				++it;
		}

		// On supprime les sommets
		for (auto it = germes->begin(); it != germes->end(); ) {
			Sommet<Vecteur2D>* s = (Sommet<Vecteur2D>*) * it;
			if (s->v.x < triangulator->left || s->v.x > triangulator->right
				|| s->v.y < triangulator->bottom || s->v.y > triangulator->top) {
				delete* it;
				it = germes->erase(it);
			}else
				++it;
		}
	}
};
