/*
 * point.h
 *
 *  Created on: Nov 1, 2017
 *      Author: krishnasaianirudhkatamreddy
 */

#ifndef POINT_H_
#define POINT_H_

class co_ordinate
{
public:
	co_ordinate() : x(0), y(0) {}
	co_ordinate(double _x, double _y) : x(_x), y(_y) {}

	bool operator ==(const co_ordinate &other);

	double x;
	double y;


};


bool co_ordinate::operator== (const co_ordinate &other)
{
	return (x == other.x && y == other.y);

}

#endif /* POINT_H_ */
