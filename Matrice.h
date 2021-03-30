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
		for (int i = 0; i < lignes; i++)
			this->values[i] = new double[colonnes];
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
	int getLSize() {
		return l;
	}

	/**
	 * Retourne le nombre de colonnes du tableau
	 */
	int getCSize() {
		return c;
	}

	/**
	 * @return La matrice est carrée ou non
	 * 
	 */
	bool isCarre() {
		return this->c == this->l;
	}

	/**
	 * 
	 * @param l ligne voulue
	 * @param c colonne voulue
	 * @return la valeur en l, c de la matrice
	 */
	double getValue(int l, int c) {
		return this->values[l][c];
	}


private:
	
	void action(double v, function<double(double, double)> func) {
		for (int i = 0; i < this->l; i++)
			for (int j = 0; j < this->c; j++)
				this->values[i][j] = func(v, this->values[i][j]);
	}

	/*
	void action(Matrice m1, Matrice m2, function<double(Matrice, Matrice, int, int)> func) {
		for (int i = 0; i < this->l; i++)
			for (int j = 0; j < this->c; j++)
				this->values[i][j] = func(m1, m2, i, j);
	}*/

	static double mult_c(double a, double b) { return a * b; }
	static double add_c(double a, double b) { return a + b; }
	static double div_c(double a, double b) { return a / b; }
	static double pow_c(double a, double b) { return pow(a, b); }

	/**
	 * Multiplication de deux matrices
	 */
	static Matrice mult_M(Matrice m1, Matrice m2) {
		if (m1.getCSize() != m2.getLSize())
			throw "Matrice non multipliable";

		Matrice res = Matrice(m1.getLSize(), m2.getCSize());

		for (unsigned int i = 0; i < res.getLSize(); i++)
			for (unsigned int j = 0; j < res.getCSize(); j++)
				for (unsigned int k = 0; k < m1.getCSize(); k++)
					res.values[i][j] = m1.values[i][k] * m2.values[k][j];

		return res;
	}

	static Matrice add_M(Matrice m1, Matrice m2) {
		if (m1.getCSize() != m2.getCSize() || m1.getCSize() != m2.getCSize())
			throw "Matrices non additionnable : Mauvaise dimension";

		Matrice res = Matrice(m1.l, m1.c);

		for (unsigned int i = 0; i < res.l; i++)
			for (unsigned int j = 0; j < res.c; j++)
				res.values[i][j] = m1.values[i][j] + m2.values[i][j];

		return res;
	}

	


public:
	void mult(double v) {
		this->action(v, mult_c);
	}
	void add(double v) {
		this->action(v, add_c);
	}
	void div(double v) {
		this->action(v, div_c);
	}
	void poww(double v) {
		this->action(v, pow_c);
	}



};