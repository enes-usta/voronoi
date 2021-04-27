#pragma once

class Color {
public:
	int r, g, b, a;
	Color() {}
	~Color() {}

	Color(int r, int g, int b, int a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

};