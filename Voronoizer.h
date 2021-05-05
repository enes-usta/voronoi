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
* et cr�e un diagramme de Vorono�
*
* T est la nature de l'information porte par une ar�te
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
		clipping();
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
		// Pour �viter les duplications 
		vector<ArcTU<T, S>*>* arcs_crees = new vector<ArcTU<T, S>*>;
		vector<Sommet<Vecteur2D>*>* sommets_crees = new vector<Sommet<Vecteur2D>*>;
		for (Triangle<S, T>* triangle : (*triangulation)) {
			for (ArcTU<T, S>* arc : triangle->arcs) {
				//Germe qu'on traite
				Sommet<Vecteur2D>* germe = arc->fin();
				// On v�rifie si on a d�j� trait� ce germe
				if (!fait[germe->clef]) {
					germes->push_back(germe);
					pivoter_sur_germe(arc, triangle, sommets_crees, arcs_crees);
					fait[germe->clef] = true;
				}
			}
		}
	}

	/**
	* Cr�� un polygone autour de ce germe si c'est possible
	*/
	void pivoter_sur_germe(ArcTU<T, S>* arc, Triangle<S, T>* triangle, vector<Sommet<Vecteur2D>*>* sommets_crees, vector<ArcTU<T, S>*>* arcs_crees) {
		//Germe qu'on traite
		Sommet<Vecteur2D>* germe = arc->fin();
		//Cellules du diagramme de Vorono�
		vector<Sommet<Vecteur2D>*> sommets_cellule;
		// Sommet de d�part
		Sommet<Vecteur2D>* centre_triangle = creer_sommet(triangle->cercle_circonscrit().centre, sommets_crees);
		Sommet<Vecteur2D>* depart = centre_triangle;
		sommets_cellule.push_back(centre_triangle);
		//On aborte dans certains cas
		bool aborted = false;
		
		while (true) {
			// Le triangle adjacent sur cet arc
			Triangle<S, T>* triangle_traite = trouver_triangle_adjacent(arc);

			if (triangle_traite == NULL) {
			// CAS D'EXCEPTION: on essaye de pivoter sur un germe mais on tombe sur un arc qui n'a pas de triangle adjacent
			// Si pas de triangle adjacent, on ne peut pas cr�er de polygone complet
			// On fait un tour en passant par le milieu des arcs de degr� 1 et le germe,
			// puis on continue l'algo normalement

				Sommet<Vecteur2D>* milieu_arc = creer_sommet(Vecteur2D((arc->debut()->v + arc->fin()->v) / 2), sommets_crees);
				sommets_cellule.push_back(milieu_arc);
				sommets_cellule.push_back(germe);
				// On se place sur le triangle de d�part
				Triangle<S, T>* triangle_adjacent = triangle;
				triangle_traite = triangle_adjacent;
				while(triangle_adjacent != NULL){
					// On parcourt les triangles adjacents dans le sens horaire jusqu'� arriver au triangle extr�me
					for (ArcTU<T, S>* a : triangle_adjacent->arcs) {
						if (a->debut() == germe) {
							triangle_adjacent = trouver_triangle_adjacent(arc);
							if (triangle_adjacent != NULL)
								triangle_traite = triangle_adjacent;
							break;
						}
					}
				}

				// On d�cale � l'arc qui part du germe
				for (ArcTU<T, S>* a : triangle_traite->arcs) {
					if (a->debut() == germe) {
						milieu_arc = creer_sommet(Vecteur2D((a->debut()->v + a->fin()->v) / 2), sommets_crees);
						sommets_cellule.push_back(milieu_arc);
					}
				}
				//aborted = true;
			}

			// Centre du cercle circonscrit de ce triangle
			centre_triangle = creer_sommet(triangle_traite->cercle_circonscrit().centre, sommets_crees);
			sommets_cellule.push_back(centre_triangle);

			// On d�cale � l'arc qui pointe vers le germe
			for (ArcTU<T, S>* a : triangle_traite->arcs) {
				if (a->fin() == germe)
					arc = a;
			}

			// On arr�te quand on a boucl�
			if (centre_triangle == depart)
				goto next;
		}
	next:;
		if (!aborted) {
			vector<ArcTU<T, S>*> arcs_cellule;
			Face<S, T>* face = new Face<S, T>();
			for (int i = 0; i < sommets_cellule.size() - 1; i++) {
				ArcTU<T, S>* nouvel_arc = creer_arc(sommets_cellule[i], sommets_cellule[i + 1], face, arcs_crees);
				arcs_cellule.push_back(nouvel_arc);
			}
			face->arcs = arcs_cellule;

			this->cellules->push_back(face);
		}
		
	}


	/**
	* Cr�e un sommet en veillant � ce qu'il ne soit pas dupliqu�
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
	* Cr�e un arc en veillant � ce qu'il ne soit pas dupliqu�
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
	* Supprime les faces et les sommets qui sont � l'ext�rieur du rectangle
	* d�crit par les sommets extr�mes parmi les sommets � construire
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
