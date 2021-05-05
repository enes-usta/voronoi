#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>

#include "Sommet.h"
#include "Triangle.h"
#include "Graphe.h"
#include "Cercle.h"

#define MAX 10000

using namespace std;

/*
* Classe Triangulator
* Prends en entre une liste de sommets
* et cr�e une triangulation de Delaunay
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
	* Retourne une triangulation de Delaunay
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
		supprimer_englobants();

		return triangulation;
	}

public:
	double left, right, bottom, top;
private:
	vector<Sommet<Vecteur2D>*>* sommets; //Sommets en entr�e, les sommets ne doivent pas �tre dupliqu�s
	Graphe<T, Vecteur2D>* graphe; //Graphe utilis� pour cr�er des sommets
	vector<Triangle<S, T>*>* triangulation;//Triangles en sortie
	vector<Triangle<S, T>*>* DTL; //Triangles � supprimer de la triangulation
	//vector<tuple<ArcTU<T, S>*, Triangle<S, T>*>> adjacence; //Tableau d'adjacence
	//vector<ArcTU<T, S>*>* arcs_crees = new vector<ArcTU<T, S>*>; //Pour �viter les duplications

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
	* D�termine une triangulation qui englobe tous les sommets � trianguler
	*/
	void determiner_triangulation_englobante() {
		/* On cherche les points extr�mes */
		left = sommets->front()->v.x;
		right = sommets->front()->v.x;
		bottom = sommets->front()->v.y;
		top = sommets->front()->v.y;

		for (Sommet<Vecteur2D>* s : (*sommets)) {
			if (s->v.x < left) left = s->v.x;
			if (s->v.x > right) right = s->v.x;
			if (s->v.y < bottom) bottom = s->v.y;
			if (s->v.y > top) top = s->v.y;
		}

		/* On cr�e les sommets/artes d'un rectangle avec ces points */
		Sommet<Vecteur2D>* s0, * s1, * s2, * s3;
		double marge = 10; //pour �viter les sommets superpos�s
		s0 = graphe->creeSommet(Vecteur2D(left - marge, bottom - marge));
		s1 = graphe->creeSommet(Vecteur2D(right + marge, bottom - marge));
		s2 = graphe->creeSommet(Vecteur2D(right + marge, top + marge));
		s3 = graphe->creeSommet(Vecteur2D(left - marge, top + marge));

		/*
		s3		 a1			s2
			-------------
		a2	|	a0	-	|	a4
			|	-		|
		s0	-------------	s1
					a3
		*/

		/* On cr�e une triangulation de ce rectangle */
		Triangle<S, T>* t1 = new Triangle<S, T>();
		Triangle<S, T>* t2 = new Triangle<S, T>();
		vector<ArcTU<T, S>*>* arc_crees = new vector<ArcTU<T, S>*>;
		vector<ArcTU<T, S>*> arcs1;
		vector<ArcTU<T, S>*> arcs2;

		arcs1.push_back(creer_arc(s0, s2, t1, arc_crees));
		arcs1.push_back(creer_arc(s2, s3, t1, arc_crees));
		arcs1.push_back(creer_arc(s3, s0, t1, arc_crees));
		t1->arcs = arcs1;

		arcs2.push_back(creer_arc(s0, s2, t2, arc_crees));
		arcs2.push_back(creer_arc(s0, s1, t2, arc_crees));
		arcs2.push_back(creer_arc(s1, s2, t2, arc_crees));
		t2->arcs = arcs2;

		triangulation->push_back(t1);
		triangulation->push_back(t2);
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

	// D�termine la liste des triangles  supprimer de la triangulation
	void determiner_DTL(Sommet<Vecteur2D>* s, Triangle<S, T>* t) {
		if (t != NULL) {
			DTL->push_back(t);
			for (int i = 0; i < 3; i++) {
				if (t->arcs[i]->arete->degre > 1) {
					Triangle<S, T>* triangleAdjacent = trouver_triangle_adjacent(t->arcs[i]);
					if (count(DTL->begin(), DTL->end(), triangleAdjacent) == 0) {
						Cercle cercle = triangleAdjacent->cercle_circonscrit();
						if (cercle.contientPoint(s->v))
							determiner_DTL(s, triangleAdjacent);
					}
				}
			}
		}
	}

	/**
	* D�termine la liste des triangles � rajouter � la triangulation
	*/
	void determiner_NTL(Sommet<Vecteur2D>* s) {
		vector<ArcTU<T, S>*>* arcs_crees = new vector<ArcTU<T, S>*>;
		for (Triangle<S, T>* t : (*DTL)) {
			for (int i = 0; i < 3; i++) {
				Triangle<S, T>* triangleAdjacent = trouver_triangle_adjacent(t->arcs[i]);
				if ((triangleAdjacent == NULL || count(DTL->begin(), DTL->end(), triangleAdjacent) == 0)
					&& !t->arcs[i]->estCollineaire(s)) {
					creer_triangle(t->arcs[i], s, arcs_crees);
				}
			}
		}
	}

	/**
	* Cr�e un triangle avec l'arc guide et le sommet s
	* utilise les arcs d�j� cr��s pour ne pas dupliquer les ar�tes et garder la convention du bonhomme d'amp�re
	*/
	void creer_triangle(ArcTU<T, S>* arc_guide, Sommet<Vecteur2D>* s, vector<ArcTU<T, S>*>* arcs_crees) {
		//On cr�e le nouveau triangle
		vector<ArcTU<T, S>*> arcs;
		Triangle<S, T>* triangle = new Triangle<S, T>();

		//premier arc, rien ne change
		arc_guide->face = triangle;
		if (arc_guide->arc_adjacent != NULL) {
			arc_guide->arc_adjacent->face_adjacente = triangle;
		}
		arcs.push_back(arc_guide);


		//deuxi�me arc, si on a cr�� un arc ayant la m�me ar�te, on r�utilise l'ar�te
		arcs.push_back(creer_arc(arc_guide->fin(), s, triangle, arcs_crees));

		//troisi�me arc, m�me logique que pour le deuxi�me
		arcs.push_back(creer_arc(s, arc_guide->debut(), triangle, arcs_crees));

		//on l'ajoute � la triangulation
		triangle->arcs = arcs;
		this->triangulation->push_back(triangle);
	}

	/**
	* Retourne un triangle adjacent l'arc s'il en existe un, sinon retourne null
	*/
	Triangle<S, T>* trouver_triangle_adjacent(ArcTU<T, S>* arc) {

		/*for (Triangle<S, T>* triangleB : (*triangulation))
				for (ArcTU<T, S>* arcB : triangleB->arcs)
					if ((*arc) != (*arcB) && arc->arete->estEgal(arcB->debut(), arcB->fin()))
						return triangleB;

		return NULL;*/
		return (Triangle<S, T>*) arc->face_adjacente;
	}

	/**
	* Cr�e un arc en veillant � ce qu'il ne soit pas dupliqu�
	*/
	ArcTU<T, S>* creer_arc(Sommet<Vecteur2D>* deb, Sommet<Vecteur2D>* fin, Triangle<S, T> * triangle, vector<ArcTU<T, S>*>* arcs_crees) {
		for (ArcTU<T, S>* arc : (*arcs_crees))
			if (arc->arete->estEgal(deb, fin)) {
				ArcTU<T, S>* nouvel_arc = new ArcTU<T, S>(arc->arete, !arc->bonSens);
				nouvel_arc->face_adjacente = arc->face;
				nouvel_arc->arc_adjacent = arc;
				nouvel_arc->face = triangle;

				arc->face_adjacente = triangle;
				arc->arc_adjacent = nouvel_arc;
				return nouvel_arc;
			}

		ArcTU<T, S>* res = new ArcTU<T, S>(graphe->creeArete(T(), deb, fin), true);
		res->face = triangle;
		arcs_crees->push_back(res);
		return res;
	}

	/**
	* Supprime les triangulation en commun dans DTL et Triangles
	*/
	void supprimer_DTL() {
		int i;
		for (Triangle<S, T>* dt : (*DTL)) {
			i = 0;
			for (Triangle<S, T>* t : (*triangulation)) {
				if (dt == t) {
					triangulation->erase((triangulation->begin() + i));
					break;
				}
				i++;
			}
		}

		for(auto dt : (*DTL))
			delete dt;
		DTL->clear();
	}

	/**
	* Supprime les sommets englobants �lague les ar�tes arrivant � ce sommet
	*/
	void supprimer_englobants(){
		for (auto it = triangulation->begin(); it != triangulation->end(); ) {
			bool deleted = false;
			Triangle<S, T>* t = (Triangle<S, T>*) *it;
			for (ArcTU<T, S>* arc : (t->arcs)) {
				if (arc->debut()->v.x < left || arc->debut()->v.x > right
					|| arc->debut()->v.y < bottom || arc->debut()->v.y > top) {
					delete* it;
					it = triangulation->erase(it);
					deleted = true;
					goto next;
				}
			}
			next:
			if (!deleted)
				++it;
		}
	}
};
