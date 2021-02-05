#pragma once
#include <vector>
#include "Sommet.h"
#include "Arc.h"

template <class S, class T>
class Face{
public:
	vector<Arc<S, T>> arcs;
	public Face(vector<Arc<S, T>> arcs) :arcs(arcs) {}
};