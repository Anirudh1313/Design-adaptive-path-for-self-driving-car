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
	Parabola() : p(0), centre(0,0), refer(0,0) {}
	Parabola(double _p, co_ordinate _centre, co_ordinate _ref) : p(_p), centre(_centre), refer(_ref) {}
	double p;
	co_ordinate centre;
	co_ordinate refer;
};

#endif /* PARABOLA_H_ */
