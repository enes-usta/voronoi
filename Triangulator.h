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
* et cr�e une triangulation
*
* T est la nature de l'information porte par une ar�te
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
				throw Erreur("Aucun triangle contenant le sommet trouv�");
		}

		cout << les_arcs_sont_bien_orientes() << endl;

		return triangulation;
	}


private:
	vector<Sommet<Vecteur2D>*>* sommets;//Sommets en entr�e
	Graphe<T, Vecteur2D>* graphe;//Graphe utilis� pour cr�er des sommets
	vector<Triangle<S, T>*>* triangulation;//Triangles en sortie
	vector<Triangle<S, T>*>* DTL;//Triangles � supprimer de la triangulation

	/**
	* Initialise les membres
	*/
	void init(vector<Sommet<Vecteur2D>*>* sommets, Graphe<T, Vecteur2D>* graphe) {
		this->sommets = sommets;
		this->graphe = graphe;
		triangulation = new vector<Triangle<S, T>*>();
		DTL = new vector<Triangle<S, T>*>();
	}

	/**
	* Dtermine une triangulation qui englobe tous les sommets  trianguler
	*/
	void determiner_triangulation_englobante() {
		/* On cherche les points extr�mes */
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

		/* On cr�e les sommets/artes d'un rectangle avec ces points */
		Sommet<Vecteur2D>* s0, * s1, * s2, * s3;
		double marge = 1;//pour �viter les sommets superposs
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

		/* On cr�e une triangulation de ce rectangle */

		triangulation->push_back(new Triangle<S, T>(new ArcTU<T>(a0, true), new ArcTU<T>(a1, true), new ArcTU<T>(a2, true)));
		triangulation->push_back(new Triangle<S, T>(new ArcTU<T>(a0, false), new ArcTU<T>(a3, true), new ArcTU<T>(a4, true)));
	}

	/**
	* Retourne le triangle contenant le sommet s, retourn null s'il n'en existe pas
	*/
	Triangle<S, T>* determiner_triangle_contenant_sommet(Sommet<Vecteur2D>* s) {
		for (Triangle<S, T>* t : (*triangulation))
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
	* Dtermine la liste des triangles � rajouter � la triangulation
	*/
	void determiner_NTL(Sommet<Vecteur2D>* s) {
		vector<ArcTU<T>*> arcs_crees;
		for (Triangle<S, T>* t : (*DTL)) {
			for (int i = 0; i < 3; i++) {
				Triangle<S, T>* triangleAdjacent = trouver_triangle_adjacent(t->arcs[i]);
				if ((triangleAdjacent == NULL /*|| count(DTL->begin(), DTL->end(), triangleAdjacent)*/)
					&& !t->arcs[i]->estCollineaire(s)) {
					//On cr�e le nouveau triangle
					vector<ArcTU<T>*> arcs;

					//premier arc, rien ne change
					arcs.push_back(t->arcs[i]);

					//deuxi�me arc, si on a cr�� un arc ayant la m�me ar�te, on r�utilise l'ar�te
					bool cree = false;
					for (ArcTU<T>* arc : arcs_crees) {
						if (arc->arete->estEgal(t->arcs[i]->fin(), s)) {
							arcs.push_back(new ArcTU<T>(arc->arete, !arc->bonSens));
							cree = true;
						}
					}
					if (!cree) {
						ArcTU<T>* arc = new ArcTU<T>(graphe->creeArete(T(), t->arcs[i]->fin(), s), true);
						arcs_crees.push_back(arc);
						arcs.push_back(arc);
					}

					//troisi�me arc, m�me logique que pour le deuxi�me
					cree = false;
					for (ArcTU<T>* arc : arcs_crees) {
						if (arc->arete->estEgal(s, t->arcs[i]->debut())) {
							arcs.push_back(new ArcTU<T>(arc->arete, !arc->bonSens));
							cree = true;
						}
					}
					if (!cree) {
						ArcTU<T>* arc = new ArcTU<T>(graphe->creeArete(T(), s, t->arcs[i]->debut()), true);
						arcs_crees.push_back(arc);
						arcs.push_back(arc);
					}

					this->triangulation->push_back(new Triangle<S, T>(arcs, S()));
				}
			}
		}
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

	/**
	* Tupprime les triangulation en commun dans DTL et Triangles
	*/
	void supprimer_DTL() {
		int i;
		for (Triangle<S, T>* dt : (*DTL)) {
			i = 0;
			if (dt != nullptr) {
				for (Triangle<S, T>* t : (*triangulation)) {
					if (dt == t) {
						triangulation->erase((triangulation->begin() + i));
						break;
					}
					i++;
				}

			}
		}

		DTL->clear();
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
};
