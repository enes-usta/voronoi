#pragma once
#include <stdio.h>
#include <math.h> 
#include <functional>
using namespace std;

/**
 *  C colonnes
 * [] [] [] [] 
 * [] [] [] []  L lignes
 * [] [] [] []
 */
class Matrice {


private:
	double **values;
	int c, l;

public:

	/**
	 * Création d'une matrice avec c colonnes et l lignes
	 * @param l: nombre de ligne
	 * @param c Nombre de colonnes 
	 *
	 */
	Matrice(int lignes, int colonnes):l(lignes), c(colonnes) {
		if (lignes <= 0 || colonnes <= 0) throw "La matrice ne peut avoir une taille inférieure ou égale à 0";
		this->values = new double* [lignes];
		for (int i = 0; i < lignes; i++) {
			this->values[i] = new double[colonnes];
			for (int j = 0; j < colonnes; j++)
				this->values[i][j] = 0;
		}
	}

	/** 
	 * Création d'une matrice avec un tableau déja défini
	 * 
	 */
	Matrice(int lignes, int colonnes, double** value): Matrice(lignes, colonnes) {
		this->values = value;
	}

	/**
	 * Constructeur par copie
	 */
	Matrice(Matrice const& m) : Matrice(m.l, m.c) {
		for (int i = 0; i < m.l; i++)
			for (int j = 0; j < m.c; j++)
				this->values[i][j] = m.values[i][j];
	}

	/**
	 * Création d'une matrice carrée
	 */
	Matrice(int s) {
		Matrice(s, s);
	}

	/**
	 * Retourne le nombre de ligne du tableau
	 */
	int getLSize() const {
		return l;
	}

	/**
	 * Retourne le nombre de colonnes du tableau
	 */
	int getCSize() const {
		return c;
	}

	/**
	 * @return La matrice est carrée ou non
	 * 
	 */
	bool isCarre() const {
		return this->c == this->l;
	}

	/**
	 * 
	 * @param l ligne voulue
	 * @param c colonne voulue
	 * @return la valeur en l, c de la matrice
	 */
	double getValue(int l, int c) const {
		return this->values[l][c];
	}

	/**
	 * Définir la valeur d'une case de la matrice
	 * @param l Numéro de ligne
	 * @param c Numéro de colonne
	 * 
	 */
	void setValue(int l, int c, double value) {
		this->values[l][c] = value;
	}

	void setValue(unsigned int l, unsigned int c, double** value) {
		this->l = l;
		this->c = c;
		this->values = value;
	}


private:
	
	Matrice action(double v, function<double(double, double)> func) const {
		Matrice m1 = Matrice(*this);
		for (int i = 0; i < this->l; i++)
			for (int j = 0; j < this->c; j++)
				m1.values[i][j] = func(v, this->values[i][j]);
		return m1;
	}

	static double mult_c(double a, double b) { return a * b; }
	static double add_c(double a, double b) { return a + b; }
	static double div_c(double a, double b) { return a / b; }
	static double pow_c(double a, double b) { return pow(a, b); }

	Matrice mult(double v) const { return this->action(v, mult_c); }
	Matrice add(double v) const { return this->action(v, add_c); }
	Matrice div(double v) const { return this->action(v, div_c); }
	Matrice poww(double v) const { return this->action(v, pow_c); }





	/**
	 * Multiplication de deux matrices
	 */
	static Matrice mult_M(const Matrice &m1, const Matrice &m2) {
		if (m1.getCSize() != m2.getLSize())
			throw "Matrice non multipliable";

		Matrice res = Matrice(m1.getLSize(), m2.getCSize());

		for (unsigned int i = 0; i < res.getLSize(); i++)
			for (unsigned int j = 0; j < res.getCSize(); j++)
				for (unsigned int k = 0; k < m1.getCSize(); k++)
					res.values[i][j] += m1.values[i][k] * m2.values[k][j];

		return res;
	}

	/**
	 * Addition de deux matrices
	 */
	static Matrice add_M(const Matrice m1, const Matrice m2) {
		if (m1.getCSize() != m2.getCSize() || m1.getLSize() != m2.getCSize())
			throw "Matrices non additionnable : Mauvaise dimension";

		Matrice res = Matrice(m1.l, m1.c);

		for (unsigned int i = 0; i < res.l; i++)
			for (unsigned int j = 0; j < res.c; j++)
				res.values[i][j] = m1.values[i][j] + m2.values[i][j];

		return res;
	}



public:

	/**
	 * Retourne la matrice 1/m
	 */
	Matrice pow_m1() const {
		Matrice newm = Matrice(this->l, this->c);
		for (int i = 0; i < newm.l; i++)
			for (int j = 0; j < newm.c; j++)
				newm.values[i][j] = 1 / this->values[i][j];

		return newm;
	}

	Matrice operator*(double v) {
		return this->mult(v);
	}
	Matrice operator+(double v) {
		return this->add(v);
	}
	Matrice operator-(double v) {
		return this->add(-v);
	}
	Matrice operator/(double v) {
		return this->mult(1/v);
	}


	Matrice operator-(Matrice m) {
		return Matrice::add_M(*this, m*-1);
	}
	Matrice operator+(Matrice m) {
		return Matrice::add_M(*this, m);
	}
	Matrice operator*(Matrice m) {
		return Matrice::mult_M(*this, m);
	}

	bool operator==(const Matrice& m) const {
		if (this->l != m.l || this->c != m.c)
			return false;
		
		for (int i = 0; i < m.l; i++)
			for (int j = 0; j < m.c; j++)
				if (this->values[i][j] != m.values[i][j])
					return false;
		return true;
	}
};



inline std::ostream& operator << (std::ostream& Str, const Matrice &m) {
	for (int i = 0; i < m.getLSize(); i++) {
		Str << "| ";
		for (int j = 0; j < m.getCSize(); j++)
			Str << m.getValue(i, j) << ", ";
		Str << '|' << '\n' << '|';
	}

	return Str;
}

