/*
 * parabola.h
 *
 *  Created on: Nov 1, 2017
 *      Author: krishnasaianirudhkatamreddy
 */

#ifndef PARABOLA_H_
#define PARABOLA_H_

#include "point.h"

class Parabola
{
public:
	Parabola() : p(0), centre(0,0), ref(0,0) {}
	Parabola(double _p, Point _centre, Point _ref) : p(_p), centre(_centre), ref(_ref) {}
	double p;
	Point centre;
	Point ref;
};

#endif /* PARABOLA_H_ */
