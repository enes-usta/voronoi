#include <iostream>
#include <string>
#include "Graphe.h"
#include "Triangle.h"
#include "Arc.h"
#include "Arete.h"
#include "Vecteur2D.h"

using namespace std;

int main()
{
		Graphe<char, Vecteur2D> g1;	// création à vide
		Sommet<Vecteur2D>* s0, * s1, * s2, * s3;

		//------------------ on insère des nouveaux sommets isolés --------------------

		s0 = g1.creeSommet(Vecteur2D(1, 1));
		s1 = g1.creeSommet(Vecteur2D(1, 2));
		s2 = g1.creeSommet(Vecteur2D(2, 1));
		s3 = g1.creeSommet(Vecteur2D(2, 2));

		//----------------- on connecte certains sommets -------------------

		Arete<char, Vecteur2D>* a0, * a1, * a2, * a3;

		a0 = g1.creeArete('a', s1, s0);
		a1 = g1.creeArete('b', s2, s1);
		a2 = g1.creeArete('c', s3, s2);
		a3 = g1.creeArete('d', s3, s1);

		vector<Arc<char>> arcs = vector<Arc<char>>();
		arcs.push_back(Arc<char>(a0, 0));
		arcs.push_back(Arc<char>(a1, 0));
		arcs.push_back(Arc<char>(a2, 0));

		Triangle<char> triangle = Triangle<char>(arcs);

		triangle.cercle_circonscrit();


}
