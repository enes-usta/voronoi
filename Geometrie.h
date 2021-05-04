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

	static bool onSegment(Vecteur2D p, Vecteur2D q, Vecteur2D r)
	{
		if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
			q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
			return true;

		return false;
	}

	// To find orientation of ordered triplet (p, q, r).
	// The function returns following values
	// 0 --> p, q and r are colinear
	// 1 --> Clockwise
	// 2 --> Counterclockwise
	static int orientation(Vecteur2D p, Vecteur2D q, Vecteur2D r)
	{
		// See https://www.geeksforgeeks.org/orientation-3-ordered-points/
		// for details of below formula.
		int val = (q.y - p.y) * (r.x - q.x) -
			(q.x - p.x) * (r.y - q.y);

		if (val == 0) return 0;  // colinear

		return (val > 0) ? 1 : 2; // clock or counterclock wise
	}

	static bool intersection(Vecteur2D p1, Vecteur2D q1, Vecteur2D p2, Vecteur2D q2)
	{
		// Find the four orientations needed for general and
		// special cases
		int o1 = orientation(p1, q1, p2);
		int o2 = orientation(p1, q1, q2);
		int o3 = orientation(p2, q2, p1);
		int o4 = orientation(p2, q2, q1);

		// General case
		if (o1 != o2 && o3 != o4)
			return true;

		// Special Cases
		// p1, q1 and p2 are colinear and p2 lies on segment p1q1
		if (o1 == 0 && onSegment(p1, p2, q1)) return true;

		// p1, q1 and q2 are colinear and q2 lies on segment p1q1
		if (o2 == 0 && onSegment(p1, q2, q1)) return true;

		// p2, q2 and p1 are colinear and p1 lies on segment p2q2
		if (o3 == 0 && onSegment(p2, p1, q2)) return true;

		// p2, q2 and q1 are colinear and q1 lies on segment p2q2
		if (o4 == 0 && onSegment(p2, q1, q2)) return true;

		return false; // Doesn't fall in any of the above cases
	}


	static void resoudreSysteme(const double a1, const double b1, const double c1, const double a2, const double b2, const double c2, double &s, double &t) {

	}

	/**
	 */
	static bool intersectionDroiteDroite(const Vecteur2D &p0, const Vecteur2D &p1, const Vecteur2D &q0, const Vecteur2D &q1,
							double &t, double &s){


	}


	static bool intersectionSegmentSegment(const Vecteur2D& p0, const Vecteur2D& p1, const Vecteur2D& q0, const Vecteur2D& q1,
		double t, double s) {
		intersectionDroiteDroite(p0, p1, q0, q1, t, s);
		if (0< t < 1 && 0 < s < 1)
			return true;
	}





};

