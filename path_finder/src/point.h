/*
 * point.h
 *
 *  Created on: Nov 1, 2017
 *      Author: krishnasaianirudhkatamreddy
 */

#ifndef POINT_H_
#define POINT_H_

class Point
{
public:
	Point() : x(0), y(0) {}
	Point(double _x, double _y) : x(_x), y(_y) {}

	bool operator ==(const Point &other);

	double x;
	double y;


};


bool Point::operator ==(const Point &other)
{
	return (x == other.x && y == other.y);

}

#endif /* POINT_H_ */
