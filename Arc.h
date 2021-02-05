#pragma once
#include "Arete.h"

template <class S, class T>
class Arc : public Arete<S, T>{
public:
	int sens;
	public Arc(Arete arete, int sens) :super(arete), sens(sens){}
};