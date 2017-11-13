/*
 * Path_Planner.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: krishnasaianirudhkatamreddy
 */
#include<iostream>
#include <vector>
#include <cmath>
#include "parabola.h"
#include "point.h"


using namespace std;

vector<Parabola> predefine_reference_points(double car_length, double width, Point src);
Point new_reference_point_towards_checkpoint(Point src, Point checkpoint, vector<Parabola> finalParabolas);

//if direction is right = 1, left = 0
bool direction;

const double INF = 999999999999;

int main()
{

	double car_length, width;
	cout << "Enter car length: ";
	cin >> car_length;
	cout << "\nEnter width of the reference rectangle: ";
	cin >> width;


	double xi, yi;
	cout << "car's current location: ";
	cin >> xi >> yi;
	Point src(xi, yi);


	// enter checkpoint to move car there
	double xi_1, yi_1;
	cout << "Enter your destination checkpoint xi_1 and yi_1 : ";
	cin >> xi_1;
	cin >> yi_1;

	Point ck_1(xi_1, yi_1);

	if(xi_1 > xi)
	{
		direction = 1;   // the checkpoint is on rightside
	}
	else
	{
		direction = 0;
	}

	vector<Parabola> FinalParabolas = predefine_reference_points(car_length, width, src);

	Point new_ref_point = new_reference_point_towards_checkpoint(src, ck_1, FinalParabolas);


	cout <<"\n(" << new_ref_point.x << ", " << new_ref_point.y << ") " << endl;
}

vector<Parabola> predefine_reference_points(double car_length, double width, Point src)
{
	//vector goes from sharp-sharp to big-big
	vector<Parabola> rightR_parabolas;
	vector<Parabola> leftR_parabolas;

	double u = car_length/3;
	double para_P;

	cout << "Right reference points: ";
	// storing all the right reference points to a vector
	for(int i = 0; i < 9; i++)
	{
		Point pointR(src.x + width, src.y + u);

		para_P = pow(u, 2) / (4 * width);

		cout << " (" << pointR.x << ", " << pointR.y << ")  p: " << para_P << " ";

		Parabola para(para_P, src, pointR);

		rightR_parabolas.push_back(para);

		u += car_length/3;
	}

	u = car_length/3;
	cout << endl << "left reference points:";
	for(int i = 0; i < 9; i++)
	{
		Point pointL(src.x - width, src.y + u);

		para_P = pow((pointL.y - src.y), 2) / (4 * (pointL.x - src.x));

		cout << " (" << pointL.x << ", " << pointL.y << ")  p: " << para_P << " ";

		Parabola para(para_P, src, pointL);

		leftR_parabolas.push_back(para);

		u += car_length/3;
	}

	if(direction)
	{
		return rightR_parabolas;
	}
	else
	{
		return leftR_parabolas;
	}
}

/*two approaches to find the nearest points:
* 1)  calculating the distance between reference points and checkpoint
* 2)  checking for the coordinates and find the up and down coordinates
*/
Point new_reference_point_towards_checkpoint(Point src, Point checkpoint, vector<Parabola> finalParabolas)
{
//	double smallest, distance;
//	int index;
//	for(int i = 0; i < finalRpoint.size(); i++)
//	{
//		distance = sqrt( pow(finalRpoints[i].x - checkpoint.x,2) + pow(finalRpoints[i].y - checkpoint.y,2));
//
//		if(smallest > distance)
//		{
//			smallest = distance;
//			index = i;           //finding the smallest index.
//		}
//	}

	vector<Point> PointParaDist;

	//finding the distance of the checkpoint from the current location.
	double src_ck_Dist = sqrt( pow(checkpoint.y - src.y, 2) + pow(checkpoint.x - src.x, 2) );
	cout << "\ndist ck: " << src_ck_Dist << endl;
	//calculating the point on the parabola from the src, with the same distance of src to ck1.

	double p, x, y;
	for(int i = 0; i < finalParabolas.size(); i++)
	{
		p = finalParabolas[i].p;
//		x = sqrt(pow(src_ck_Dist, 2) - pow((4*p*(x - src.x) + src.y)-src.y, 2)) + src.x;
//		y = 4*p*(x - src.x) + src.y;

//		x = src.x + sqrt(pow(src_ck_Dist, 2) / (1+(4*pow(p,2))));
//		y = 4*p*(x - src.x) + src.y;


		if(direction)
		{
			x = ((-4*p + sqrt(pow(4*p, 2) + 4*pow(src_ck_Dist, 2)))/2) + src.x;
			y = sqrt(4*p*(x - src.x)) + src.y;
		}
		else
		{
			x = ((-4*p - sqrt(pow(4*p, 2) + 4*pow(src_ck_Dist, 2)))/2) + src.x;
			y = sqrt(4*p*(x - src.x)) + src.y;
		}

		Point pDist(x, y);
		PointParaDist.push_back(pDist);
	}

	double smallest = INF;
	double distance;
	int index_min;

//	cout << "\nPoints on parabola with same dist: ";
//	for(int i = 0; i < PointParaDist.size(); i++)
//	{
//		cout << " (" << PointParaDist[i].x << ", " << PointParaDist[i].y << ") ";
//	}

	cout << "\nindex min: ";
	for(int i = 0; i < PointParaDist.size(); i++)  //finding the minimum parabola
	{
		distance = sqrt( pow((PointParaDist[i].x - checkpoint.x), 2) + pow((PointParaDist[i].y - checkpoint.y), 2));

		if(smallest > distance)
		{
			smallest = distance;
			index_min = i;           //finding the smallest index.c
			//cout << " (" << i << ", " << smallest << " ) ";
		}

		//cout << "!!" << i << ", " << distance << " !! ";
	}

//	for(int i = 0; i < PointParaDist.size(); i++)  //finding the minimum parabola
//	{
//		distance = sqrt( pow((PointParaDist[i].x - checkpoint.x), 2) + pow((PointParaDist[i].y - checkpoint.y), 2));
//
//		if(smallest > distance)
//		{
//			smallest = distance;
//			index_min = i;           //finding the smallest index.c
//			cout << " (" << i << ", " << smallest << " ) ";
//		}
//
//		cout << "!!" << i << ", " << distance << " !! ";
//	}

	int index_rank1;
	if(PointParaDist[index_min].x > checkpoint.x)   // finding the rank1 parabola
	{
		if(direction)
		{
			index_rank1 = index_min + 1;
		}
		else
		{
			index_rank1 = index_min - 1;
		}
	}
	else
	{
		if(direction)
		{
			index_rank1 = index_min - 1;
		}
		else
		{
			index_rank1 = index_min + 1;
		}
	}

	Point new_ref_point((finalParabolas[index_min].ref.x + finalParabolas[index_rank1].ref.x)/2,
			(finalParabolas[index_min].ref.y + finalParabolas[index_rank1].ref.y)/2);

	//double dist_ref = sqrt( pow((finalParabolas[index_min].ref.x - finalParabolas[index_rank1].ref.x), 2) + pow((finalParabolas[index_min].ref.y - finalParabolas[index_rank1].ref.x), 2) );


	char a[2];

	switch(index_min)
	{
	case 0: a[1] = 's';  a[0] = 's'; break;
	case 1: a[1] = 's';  a[0] = 'm'; break;
	case 2: a[1] = 's';  a[0] = 'b'; break;
	case 3: a[1] = 'm';  a[0] = 's'; break;
	case 4: a[1] = 'm';  a[0] = 'm'; break;
	case 5: a[1] = 'm';  a[0] = 'b'; break;
	case 6: a[1] = 'b';  a[0] = 's'; break;
	case 7: a[1] = 'b';  a[0] = 'm'; break;
	case 8: a[1] = 'b';  a[0] = 'b'; break;
	default: a[1] = 'n';  a[0] = 'n'; break;
	}

	char b[2];
	switch(index_rank1)
	{
	case 0: b[1] = 's';  b[0] = 's'; break;
	case 1: b[1] = 's';  b[0] = 'm'; break;
	case 2: b[1] = 's';  b[0] = 'b'; break;
	case 3: b[1] = 'm';  b[0] = 's'; break;
	case 4: b[1] = 'm';  b[0] = 'm'; break;
	case 5: b[1] = 'm';  b[0] = 'b'; break;
	case 6: b[1] = 'b';  b[0] = 's'; break;
	case 7: b[1] = 'b';  b[0] = 'm'; break;
	case 8: b[1] = 'b';  b[0] = 'b'; break;
	default: b[1] = 'n';  b[0] = 'n'; break;
	}

	cout << "\n\n minimum reference point:  ( " << finalParabolas[index_min].ref.x << ", " << finalParabolas[index_min].ref.y << ")  -  " << a[1] << "-" << a[0] << endl;
	cout << " rank1 reference point:    ( " << finalParabolas[index_rank1].ref.x << ", " << finalParabolas[index_rank1].ref.y << ")  -  "  << b[1] << "-" << b[0] << endl;

	if(finalParabolas[index_min].ref == checkpoint)
	{
		return finalParabolas[index_min].ref;
	}
	else
		return new_ref_point;

}
