#pragma once
#include "Vecteur2D.h"

class Geometrie {
public:
	Geometrie(){}
	~Geometrie(){}

	/**
	* Retourne vrai si c est à gauche de (a,b) ou 
	*/
	static bool aGauche(Vecteur2D a, Vecteur2D b, Vecteur2D c) {
		return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y) > 0;
	}

	/**
	* Retourne vrai si c est à gauche de (a,b) ou s'ils sont collinéaires
	*/
	static bool aGaucheOuCollineaire(Vecteur2D a, Vecteur2D b, Vecteur2D c) {
		return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y) >= 0;
	}

	/**
	* Retourne vrai si abcd est un poylgone convexe
	*/
	static bool forment_polygone_convexe(Vecteur2D a, Vecteur2D b, Vecteur2D c, Vecteur2D d) {
		bool res = false;

		bool abc = aGauche(a, b, c);
		bool abd = aGauche(a, b, d);
		bool bcd = aGauche(b, c, d);
		bool cad = aGauche(c, a, d);

		if (abc && abd && bcd && !cad)
			res = true;
		else if (abc && abd && !bcd && cad)
			res = true;
		else if (abc && !abd && bcd && cad)
			res = true;
		else if (!abc && !abd && !bcd && cad)
			res = true;
		else if (!abc && !abd && bcd && !cad)
			res = true;
		else if (!abc && abd && !bcd && !cad)
			res = true;

		return res;
	}

};

