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
	* Retourne vrai si a b c est collinéaire
	*/
	static bool collineaire(Vecteur2D a, Vecteur2D b, Vecteur2D c) {
		return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y) == 0;
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

	/**
	* Vérifie si il y a une intersection entre a1 b1 et a2 b2
	*/
	static bool intersection(Vecteur2D ap, Vecteur2D aq, Vecteur2D bp, Vecteur2D bq) {
		if (aGauche(ap, aq, bp) && aGauche(ap, aq, bq)) return false;
		if (aGauche(bp, bq, ap) && aGauche(bp, bq, aq)) return false;
		return true;
	}

};

