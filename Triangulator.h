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
* Classe Triangulation
* Prends en entre une liste de sommets
* et cre une liste de face reprsentant
* un une triangulation
*
* S est la nature de l'information porte par une arte
*/
template <class S>
class Triangulator {
public:
	Triangulator() { }
	~Triangulator() {}

	/**
	* Retourne la liste traingule de la liste de sommets en entre
	*/
	vector<Face<S>*>* triangulate(vector<Sommet<Vecteur2D>*>* sommets, Graphe<S, Vecteur2D>* graphe) {
		init(sommets, graphe);
		determiner_triangulation_englobante();
		for (Sommet<Vecteur2D>* s : (*sommets)) {
			Triangle<S>* t = determiner_triangle_contenant_sommet(s);
			if (t) {
				determiner_DTL(s, t);
				determiner_NTL(s);
				supprimer_DTL();
			}
			else
				throw Erreur("Aucun triangle contenant le sommet trouvé");

		}

		return (vector<Face<S>*>*)triangles;
	}


private:
	vector<Sommet<Vecteur2D>*>* sommets;//Sommets en entrée
	Graphe<S, Vecteur2D>* graphe;//Graphe utilisé pour créer des sommets
	vector<Triangle<S>*>* triangles;//Triangles en sortie
	vector<Triangle<S>*>* DTL;//Triangles à supprimer de la triangulation

	/**
	* Initialise les membres
	*/
	void init(vector<Sommet<Vecteur2D>*>* sommets, Graphe<S, Vecteur2D>* graphe) {
		this->sommets = sommets;
		this->graphe = graphe;
		triangles = new vector<Triangle<S>*>();
		DTL = new vector<Triangle<S>*>();
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

		Arete<S, Vecteur2D>* a0, * a1, * a2, * a3, * a4;
		a0 = graphe->creeArete(S(), s0, s2);
		a1 = graphe->creeArete(S(), s2, s3);
		a2 = graphe->creeArete(S(), s3, s0);
		a3 = graphe->creeArete(S(), s0, s1);
		a4 = graphe->creeArete(S(), s1, s2);

		/*
		s3		 a1			s2
			-------------
		a2	|	a0	-	|	a4
			|	-		|
		s0	-------------	s1
					a3
		*/

		/* On crée une triangulation de ce rectangle */

		triangles->push_back(new Triangle<S>(ArcTU<S>(a0, true), ArcTU<S>(a1, true), ArcTU<S>(a2, true)));
		triangles->push_back(new Triangle<S>(ArcTU<S>(a0, false), ArcTU<S>(a3, true), ArcTU<S>(a4, true)));
	}

	/**
	* Retourne le triangle contenant le sommet s, retourn null s'il n'en existe pas
	*/
	Triangle<S>* determiner_triangle_contenant_sommet(Sommet<Vecteur2D>* s) {
		for (Triangle<S> *t : (*triangles))
			if (t->contientPoint(s))
				return t;

		return NULL;
	}

	// Dtermine la liste des triangles  supprimer de la triangulation
	void determiner_DTL(Sommet<Vecteur2D>* s, Triangle<S>* t) {
		if (t != NULL) {
			DTL->push_back(t);
			for (int i = 0; i < 3; i++) {
				Triangle<S>* triangleAdjacent = trouver_triangle_adjacent(t->arcs[i]);
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
		for (Triangle<S> *t : (*DTL)) {
			for (int i = 0; i < 3; i++) {
				Triangle<S>* triangleAdjacent = trouver_triangle_adjacent(t->arcs[i]);
				if ((triangleAdjacent == NULL /*|| count(DTL->begin(), DTL->end(), triangleAdjacent)*/)
					&& !t->arcs[i].estCollineaire(s)) {
					//On crée le nouveau triangle
					vector<ArcTU<S>> arcs;
					bool bonSens = t->arcs[i].estAGauche(s);

					arcs.push_back(ArcTU<S>(t->arcs[i].arete, bonSens));
					arcs.push_back(ArcTU<S>(graphe->creeArete(S(), t->arcs[i].fin(), s), bonSens));
					arcs.push_back(ArcTU<S>(graphe->creeArete(S(), s, t->arcs[i].debut()), bonSens));

					Triangle<S>* new_triangle = new Triangle<S>(arcs);
					this->triangles->push_back(new_triangle);
				}
			}
		}
	}

	/**
	* Supprime les triangles en commun dans DTL et Triangles
	*/
	void supprimer_DTL() {
		int i;
		for (Triangle<S>* dt : (*DTL)) {
			i = 0;
			if (dt != nullptr) {
				for (Triangle<S>* t : (*triangles)) {
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
	Triangle<S>* trouver_triangle_adjacent(ArcTU<S> arcA) {

		for (Triangle<S> *triangle : (*triangles))
			for (ArcTU<S> arcB : triangle->arcs)
				if (arcB != arcA && arcA.arete->estEgal(arcB.arete->debut, arcB.arete->fin))
					return triangle;

		return NULL;
	}
	
};
