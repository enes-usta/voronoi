#pragma once
#include <vector>
#include "Sommet.h"
#include "Arc.h"
#include "Face.h"

template <class S, class T>
class Triangle : public Face {
public:
	vector<Arc<S, T>> arcs;
	public Triangle(vector<Arc<S, T>> arcs) :{
		if (arcs->size != 3)
			throw "Un cercle est composé de 3 arcs";
	}

};