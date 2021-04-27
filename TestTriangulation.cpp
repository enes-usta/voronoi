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
#include "Triangulator.h"
#include "GUI.h"
#include <random>



using namespace std;

int main(int argc, char** argv)
{

	// distribution uniforme 
	default_random_engine generator;
	uniform_int_distribution<int> distribution(-10, 10);
	//uniform_real_distribution<double> distribution(-1, 1);


	Graphe<char, Vecteur2D>* graphe = new Graphe<char, Vecteur2D>;	// cration  vide
	Sommet<Vecteur2D>* s0, * s1, * s2, * s3, * s4;

	//------------------ on insre des nouveaux sommets isols --------------------

	s0 = graphe->creeSommet(Vecteur2D(1, 1));
	s1 = graphe->creeSommet(Vecteur2D(3, 2));
	s2 = graphe->creeSommet(Vecteur2D(5, 2));
	s3 = graphe->creeSommet(Vecteur2D(2, 4));
	s4 = graphe->creeSommet(Vecteur2D(4, 5));

	//s0 = graphe->creeSommet(Vecteur2D(1, 1));
	//s1 = graphe->creeSommet(Vecteur2D(1, 2));
	//s2 = graphe->creeSommet(Vecteur2D(2, 2));
	//s3 = graphe->creeSommet(Vecteur2D(2, 1));



	//-------------- on dessine la triangulation -----------------------
	Triangulator<char> triangulator;

	vector<Sommet<Vecteur2D>*>* sommets = new vector<Sommet<Vecteur2D>*>;
	//sommets->push_back(s0);
	//sommets->push_back(s1);
	//sommets->push_back(s2);
	//sommets->push_back(s3);
	//sommets->push_back(s4);


	for (int i = 0; i < 20; i++)
		sommets->push_back(graphe->creeSommet(Vecteur2D(distribution(generator), distribution(generator))));

	vector<Face<char>*>* triangulation = triangulator.triangulate(sommets, graphe);

	GUI gui(argc, argv);

	gui.dessiner(triangulation, sommets);
	return 0;

}
*/