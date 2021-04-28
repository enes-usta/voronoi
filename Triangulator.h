#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "Sommet.h"
#include "Triangle.h"
#include "Graphe.h"
#include "Cercle.h"

using namespace std;

/*
* Classe Triangulator
* Prends en entre une liste de sommets
* et crée une triangulation
*
* T est la nature de l'information porte par une arête
* S est la nature de l'information porte par une face
*/
template <class S, class T>
class Triangulator {
public:
	Triangulator() { }
	~Triangulator() {}

	/**
	* Retourne la liste traingule de la liste de sommets en entre
	*/
	vector<Triangle<S, T>*>* triangulate(vector<Sommet<Vecteur2D>*>* sommets, Graphe<T, Vecteur2D>* graphe) {
		init(sommets, graphe);
		determiner_triangulation_englobante();
		for (Sommet<Vecteur2D>* s : (*sommets)) {
			Triangle<S, T>* t = determiner_triangle_contenant_sommet(s);
			if (t) {
				determiner_DTL(s, t);
				determiner_NTL(s);
				supprimer_DTL();
			}
			else 
				throw Erreur("Aucun triangle contenant le sommet trouvé");
		}
		end:
		return triangles;
	}


private:
	vector<Sommet<Vecteur2D>*>* sommets;//Sommets en entrée
	Graphe<T, Vecteur2D>* graphe;//Graphe utilisé pour créer des sommets
	vector<Triangle<S, T>*>* triangles;//Triangles en sortie
	vector<Triangle<S, T>*>* DTL;//Triangles à supprimer de la triangulation

	/**
	* Initialise les membres
	*/
	void init(vector<Sommet<Vecteur2D>*>* sommets, Graphe<T, Vecteur2D>* graphe) {
		this->sommets = sommets;
		this->graphe = graphe;
		triangles = new vector<Triangle<S, T>*>();
		DTL = new vector<Triangle<S, T>*>();
	}

	/**
	* Dtermine une triangulation qui englobe tous les sommets  trianguler
	*/
	void determiner_triangulation_englobante() {
		/* On cherche les points extrêmes */
		int xMin = sommets->front()->v.x;
		int xMax = sommets->front()->v.x;
		int yMin = sommets->front()->v.y;
		int yMax = sommets->front()->v.y;

		for (Sommet<Vecteur2D>* s : (*sommets)) {
			if (s->v.x < xMin) xMin = s->v.x;
			if (s->v.y < yMin) yMin = s->v.y;
			if (s->v.x > xMax) xMax = s->v.x;
			if (s->v.y > yMax) yMax = s->v.y;
		}

		/* On crée les sommets/artes d'un rectangle avec ces points */
		Sommet<Vecteur2D>* s0, * s1, * s2, * s3;
		double marge = 1;//pour éviter les sommets superposs
		s0 = graphe->creeSommet(Vecteur2D(xMin - marge, yMin - marge));
		s1 = graphe->creeSommet(Vecteur2D(xMax + marge, yMin - marge));
		s2 = graphe->creeSommet(Vecteur2D(xMax + marge, yMax + marge));
		s3 = graphe->creeSommet(Vecteur2D(xMin - marge, yMax + marge));

		Arete<T, Vecteur2D>* a0, * a1, * a2, * a3, * a4;
		a0 = graphe->creeArete(T(), s0, s2);
		a1 = graphe->creeArete(T(), s2, s3);
		a2 = graphe->creeArete(T(), s3, s0);
		a3 = graphe->creeArete(T(), s0, s1);
		a4 = graphe->creeArete(T(), s1, s2);

		/*
		s3		 a1			s2
			-------------
		a2	|	a0	-	|	a4
			|	-		|
		s0	-------------	s1
					a3
		*/

		/* On crée une triangulation de ce rectangle */

		triangles->push_back(new Triangle<S, T>(new ArcTU<T>(a0, true), new ArcTU<T>(a1, true), new ArcTU<T>(a2, true)));
		triangles->push_back(new Triangle<S, T>(new ArcTU<T>(a0, false), new ArcTU<T>(a3, true), new ArcTU<T>(a4, true)));
	}

	/**
	* Retourne le triangle contenant le sommet s, retourn null s'il n'en existe pas
	*/
	Triangle<S, T>* determiner_triangle_contenant_sommet(Sommet<Vecteur2D>* s) {
		for (Triangle<S, T>* t : (*triangles))
			if (t->contientPoint(s))
				return t;

		return NULL;
	}

	// Dtermine la liste des triangles  supprimer de la triangulation
	void determiner_DTL(Sommet<Vecteur2D>* s, Triangle<S, T>* t) {
		if (t != NULL) {
			DTL->push_back(t);
			for (int i = 0; i < 3; i++) {
				Triangle<S, T>* triangleAdjacent = trouver_triangle_adjacent(t->arcs[i]);
				if (triangleAdjacent != NULL && count(DTL->begin(), DTL->end(), triangleAdjacent) == 0) {
					Cercle cercle = triangleAdjacent->cercle_circonscrit();
					if (cercle.contientPoint(s->v))
						determiner_DTL(s, triangleAdjacent);
				}
			}
		}
	}

	/**
	* Dtermine la liste des triangles  rajouter  la triangulation
	*/
	void determiner_NTL(Sommet<Vecteur2D>* s) {
		for (Triangle<S, T>* t : (*DTL)) {
			for (int i = 0; i < 3; i++) {
				Triangle<S, T>* triangleAdjacent = trouver_triangle_adjacent(t->arcs[i]);
				if ((triangleAdjacent == NULL /*|| count(DTL->begin(), DTL->end(), triangleAdjacent)*/)
					&& !t->arcs[i]->estCollineaire(s)) {
					//On crée le nouveau triangle
					vector<ArcTU<T>*> arcs;
					bool bonTens = t->arcs[i]->estAGauche(s);

					arcs.push_back(new ArcTU<T>(t->arcs[i]->arete, bonTens));
					arcs.push_back(new ArcTU<T>(graphe->creeArete(T(), t->arcs[i]->fin(), s), bonTens));
					arcs.push_back(new ArcTU<T>(graphe->creeArete(T(), s, t->arcs[i]->debut()), bonTens));

					Triangle<S, T>* new_triangle = new Triangle<S, T>(arcs);
					this->triangles->push_back(new_triangle);
				}
			}
		}
	}

	/**
	* Tupprime les triangles en commun dans DTL et Triangles
	*/
	void supprimer_DTL() {
		int i;
		for (Triangle<S, T>* dt : (*DTL)) {
			i = 0;
			if (dt != nullptr) {
				for (Triangle<S, T>* t : (*triangles)) {
					if (dt == t) {
						triangles->erase((triangles->begin() + i));
						break;
					}
					i++;
				}

			}
		}

		DTL->clear();
	}

	/**
	* Retourne un triangle adjacent  l'arc a s'il en existe un, sinon retourne null
	*/
	Triangle<S, T>* trouver_triangle_adjacent(ArcTU<T> *arcA) {

		for (Triangle<S, T>* triangle : (*triangles))
			for (ArcTU<T> *arcB : triangle->arcs)
				if ((*arcB) != (*arcA) && arcA->arete->estEgal(arcB->arete->debut, arcB->arete->fin))
					return triangle;

		return NULL;
	}

};
