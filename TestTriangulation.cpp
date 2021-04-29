/*#include <iostream>
#include <string>
#include "Graphe.h"
#include "Vecteur2D.h"
#include <vector>
#include <stdio.h>
#include "Triangulator.h"
#include "GUI.h"
#include <random>



using namespace std;

int main(int argc, char** argv)
{

	//--------------- généraeur de distribution uniforme -----------------
	default_random_engine generator;
	uniform_int_distribution<int> distribution(-100, 100);

	//-------------- triangulateur et variables concernées -----------------------
	Triangulator<Color*, Color*> triangulator;
	Graphe<Color*, Vecteur2D>* graphe = new Graphe<Color*, Vecteur2D>;
	vector<Sommet<Vecteur2D>*>* sommets = new vector<Sommet<Vecteur2D>*>;



	//-------- génération des sommets en veillant à ce qu'il n'y ait pas de duplications ----------
	for (int i = 0; i < 100; i++) {
		Vecteur2D a = Vecteur2D(distribution(generator), distribution(generator));
		//Si il n'existe aucun sommet, on l'ajoute
		if(sommets->size() == 0)
			sommets->push_back(graphe->creeSommet(a));

		//On vérifie si il existe un sommet ayant ces coordonnées
		bool existe = false;
		for (Sommet<Vecteur2D>* s : (*sommets))
			if ((s->v.x == a.x && s->v.y == a.y))
				existe = true;

		//Si il n'existe aucun sommet de même coordonnées, on l'ajoute
		if(!existe)
			sommets->push_back(graphe->creeSommet(a));
	}

	//---------- génération de la triangulation -----------------------------
	vector<Triangle<Color*, Color*>*>* triangulation = triangulator.triangulate(sommets, graphe);

	//---------- on dessine -----------------------------
	GUI gui(argc, argv);
	gui.dessiner((vector<Face<Color*, Color*>*>*)triangulation, sommets);

	return 0;

}
*/