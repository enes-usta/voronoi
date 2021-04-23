#include <iostream>
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

using namespace std;

int main(int argc, char** argv)
{
		Graphe<char, Vecteur2D> graphe;	// création à vide
		Sommet<Vecteur2D>* s0, * s1, * s2, * s3;

		//------------------ on insère des nouveaux sommets isolés --------------------

		s0 = graphe.creeSommet(Vecteur2D(1, 1));
		s1 = graphe.creeSommet(Vecteur2D(2, 1));
		s2 = graphe.creeSommet(Vecteur2D(2, 2));
		s3 = graphe.creeSommet(Vecteur2D(1, 2));

		//----------------- on connecte certains sommets -------------------

		Arete<char, Vecteur2D>* a0, * a1, * a2, * a3, * a4;

		a0 = graphe.creeArete('a', s0, s1);
		a1 = graphe.creeArete('b', s1, s2);
		a2 = graphe.creeArete('c', s2, s3);
		a3 = graphe.creeArete('d', s3, s0);
		a4 = graphe.creeArete('e', s2, s0);

		//-------------- on dessine un triangle -----------------------

		vector<ArcTU<char>> arcs = vector<ArcTU<char>>();
		arcs.push_back(ArcTU<char>(a0, 0));
		arcs.push_back(ArcTU<char>(a1, 0));
		arcs.push_back(ArcTU<char>(a4, 0));

		Triangle<char> triangle = Triangle<char>(arcs);
		
		Cercle cercle = triangle.cercle_circonscrit();
		
		vector<Face<char>> faces;
		faces.push_back(triangle);

		GUI gui(argc, argv);
		//gui.dessiner(faces);

		//-------------- on dessine un triangle -----------------------
		Triangulator<char> triangulator;

		vector<Sommet<Vecteur2D>*> *sommets = new vector<Sommet<Vecteur2D>*>;
		sommets->push_back(s0);
		sommets->push_back(s1);
		sommets->push_back(s2);
		sommets->push_back(s3);

		vector<Face<char>> triangulation = triangulator.triangulate(sommets, &graphe);
		gui.dessiner(triangulation);
		return 0;

}
