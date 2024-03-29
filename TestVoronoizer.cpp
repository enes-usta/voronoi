/*#include <iostream>
#include <string>
#include "Graphe.h"
#include "Triangle.h"
#include "Cercle.h"
#include "ArcTU.h"
#include "Arete.h"
#include "Vecteur2D.h"
#include <vector>
#include <stdio.h>
#include "Voronoizer.h"
#include "GUI.h"
#include <random>


using namespace std;

int main(int argc, char** argv)
{
	//--------------- g�n�rateur de distribution uniforme -----------------
	default_random_engine generator;
	uniform_int_distribution<int> distribution(-100, 100);

	//-------------- voronoizer, triangulateur et variables concern�es -----------------------
	Voronoizer<Color*, Color*> voronoizer;
	Graphe<Color*, Vecteur2D>* graphe = new Graphe<Color*, Vecteur2D>;
	vector<Sommet<Vecteur2D>*>* sommets = new vector<Sommet<Vecteur2D>*>;


	//-------- g�n�ration des sommets en veillant � ce qu'il n'y ait pas de duplications ----------
	for (int i = 0; i < 500; i++) {
		Vecteur2D a = Vecteur2D(distribution(generator), distribution(generator));
		//Si il n'existe aucun sommet, on l'ajoute
		if (sommets->size() == 0)
			sommets->push_back(graphe->creeSommet(a));

		//On v�rifie si il existe un sommet ayant ces coordonn�es
		bool existe = false;
		for (Sommet<Vecteur2D>* s : (*sommets))
			if ((s->v.x == a.x && s->v.y == a.y))
				existe = true;

		//Si il n'existe aucun sommet de m�me coordonn�es, on l'ajoute
		if (!existe)
			sommets->push_back(graphe->creeSommet(a));
	}

	//--------- g�n�ration du diagramme de voronoi ------------
	vector<Face<Color*, Color*>*>* voronoi = voronoizer.voronoize(sommets, graphe);
	//vector< Sommet<Vecteur2D>*>* germes = voronoizer.germes;

	Triangulator<Color*, Color*> triangulator;
	vector<Triangle<Color*, Color*>*>* triangulation = triangulator.triangulate(sommets, graphe);

	Color* col = new Color(255, 255, 255, 0);
	for (Triangle<Color*, Color*>* t : (*triangulation)) {
		for (ArcTU<Color*, Color*>* a : t->arcs) {
			a->arete->v = col;
		}
		//voronoi->push_back(t);
	}

	for (Face<Color*, Color*>* f : (*voronoi)) {
		f->v = col;
		//voronoi->push_back(t);
	}

	//Face<Color*, Color*>* contour = triangulator.contour;
	//voronoi->push_back(contour);

	vector< Sommet<Vecteur2D>*>* centres = new vector< Sommet<Vecteur2D>*>;


	//for (Triangle<Color*, Color*>* t : (*triangulation)) {
	//	centres->push_back(graphe->creeSommet(t->cercle_circonscrit().centre));
	//}

GUI gui(argc, argv);

gui.dessiner(voronoi, centres);
return 0;

}*/