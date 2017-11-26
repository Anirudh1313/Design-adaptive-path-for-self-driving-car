#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "point.h"
#include "parabola.h"
#include <vector>

using namespace cv;
using namespace std;

vector<Parabola> predefine_reference_points(double car_length, double width, co_ordinate src);
co_ordinate new_reference_point_towards_checkpoint(co_ordinate src, co_ordinate checkpoint, vector<Parabola> finalParabolas, double car_length);

//if direction is right = 1, left = 0
bool direction;
bool straight = false;
bool check_point_out = false;

const double INF = 999999999999;

int main(int argc, const char * argv[])
{

    Mat img;  //creating an image of type mat

    double width, car_length;

    ::co_ordinate current_point;
    ::co_ordinate check_point;

    cout << "Enter the length of the car: ";
    cin >> car_length;

    cout << "Enter the width of the imaginary rectangle: ";
    cin >> width;

    cout << "Enter the current location of the car: ";
    cin >> current_point.x >> current_point.y;

    cout << "Enter the destination of the location: ";
    cin >> check_point.x >> check_point.y;

    if(check_point.x > current_point.x+width/2 || check_point.x < current_point.x-width/2 || check_point.y > current_point.y+3*car_length || check_point.y < current_point.y)
    {
        check_point_out = true;
        cout << "\n******* Check_point is outside of the reference rectangle **************\n" << endl;
    }

    if(check_point.x > current_point.x)
	{
		direction = 1;   // if checkpoint is on right side
	}
	else
	{
		direction = 0;   // if checkpoint is on left side
	}

    /*
     *Computing parabolas from the location of the car to each pre-defined reference points on the rectange
     */
    std::vector<Parabola> FinalParabolas = predefine_reference_points(car_length, width/2, current_point);

    /*
     *Computing the new reference point on the rectangle, through which the check_point's new computed parabolic turn passes.
     */
	co_ordinate new_ref_point = new_reference_point_towards_checkpoint(current_point, check_point, FinalParabolas, car_length);


	cout <<"\n New Target reference point: (" << new_ref_point.x << ", " << new_ref_point.y << ") " << endl;


	//Predefining the length of the rectange to be thrice the length of the car.
    double length = 3*car_length;


    img = Mat::zeros(Size(width+200, length+200), CV_8UC3);


    //Drawing an imaginary angle.
    rectangle(img, Point(100,100), Point(width+100, length+100), Scalar(255,255,255), 4, 20, 0);

    //Drawing points to indicate inner segments on rectangle
    line(img, Point(width+100, length/3/3+100), Point(width+101, length/3/3+101), Scalar(55, 100, 255), 4, 10, 0);
    line(img, Point(width+100, 2*length/3/3+100), Point(width+101, 2*length/3/3+101), Scalar(55, 100, 255), 4, 10, 0);
    line(img, Point(width+100, 2*length/3/3+100), Point(width+101, 2*length/3/3+101), Scalar(55, 100, 255), 4, 10, 0);
    line(img, Point(width+100, 2*2*length/3/3+100), Point(width+101, 2*2*length/3/3+101), Scalar(55, 100, 255), 4, 10, 0);
    line(img, Point(width+100, 5*length/3/3+100), Point(width+101, 5*length/3/3+101), Scalar(55, 100, 255), 4, 10, 0);
    line(img, Point(width+100, 7*length/3/3+100), Point(width+101, 7*length/3/3+101), Scalar(55, 100, 255), 4, 10, 0);
    line(img, Point(width+100, 8*length/3/3+100), Point(width+101, 8*length/3/3+101), Scalar(55, 100, 255), 4, 10, 0);
    line(img, Point(100, 2*length/9+100), Point(101,2*length/9+101), Scalar(55, 100, 255), 4, 10, 0);
    line(img, Point(100, 1*length/9+100), Point(101,1*length/9+101), Scalar(55, 100, 255), 4, 10, 0);
    line(img, Point(100, 4*length/9+100), Point(101,4*length/9+101), Scalar(55, 100, 255), 4, 10, 0);
    line(img, Point(100, 5*length/9+100), Point(101,5*length/9+101), Scalar(55, 100, 255), 4, 10, 0);
    line(img, Point(100, 7*length/9+100), Point(101,7*length/9+101), Scalar(55, 100, 255), 4, 10, 0);
    line(img, Point(100, 8*length/9+100), Point(101,8*length/9+101), Scalar(55, 100, 255), 4, 10, 0);

    //Drawing points to indicate segments on rectangle
    line(img, Point(width+100, 100), Point(width+101,101), Scalar(0, 0, 255), 8, 10, 0);
    line(img, Point(width+100, length/3+100), Point(width+101, length/3+101), Scalar(0, 0, 255), 8, 10, 0);
    line(img, Point(width+100, 2*length/3+100), Point(width+101, 2*length/3+101), Scalar(0, 0, 255), 8, 10, 0);
    line(img, Point(width+100, length+100), Point(width+101, length+101), Scalar(0, 0, 255), 8, 10, 0);
    line(img, Point(100, length/3+100), Point(101,length/3+101), Scalar(0, 0, 255), 8, 10, 0);
    line(img, Point(100, length+100), Point(101,length+101), Scalar(0, 0, 255), 8, 10, 0);
    line(img, Point(100, 2*length/3+100), Point(101,2*length/3+101), Scalar(0, 0, 255), 8, 10, 0);
    line(img, Point(100, 100), Point(101,101), Scalar(0, 0, 255), 8, 10, 0);


    //Drawing axis
    line(img, Point(100, length/9+100), Point(width+100, length/9+100), Scalar(69, 69, 74), 0.05, 8, 0);
    line(img, Point(100, 2*length/9+100), Point(width+100, 2*length/9+100), Scalar(69, 69, 74), 0.05, 8, 0);
    line(img, Point(100, 3*length/9+100), Point(width+100, 3*length/9+100), Scalar(69, 69, 74), 0.05, 8, 0);
    line(img, Point(100, 4*length/9+100), Point(width+100, 4*length/9+100), Scalar(69, 69, 74), 0.05, 8, 0);
    line(img, Point(100, 5*length/9+100), Point(width+100, 5*length/9+100), Scalar(69, 69, 74), 0.05, 8, 0);
    line(img, Point(100, 6*length/9+100), Point(width+100, 6*length/9+100), Scalar(69, 69, 74), 0.05, 8, 0);
    line(img, Point(100, 7*length/9+100), Point(width+100, 7*length/9+100), Scalar(69, 69, 74), 0.05, 8, 0);
    line(img, Point(100, 8*length/9+100), Point(width+100, 8*length/9+100), Scalar(69, 69, 74), 0.05, 8, 0);
    line(img, Point(100+width/2, 100), Point(100+width/2, 100+length), Scalar(69, 69, 74), 0.05, 8, 0);


    //Drawing a point in the current location of the car
    line(img, Point(width/2+100, length+100), Point(width/2+101, length+101), Scalar(50, 200, 255), 20, 8, 0);
    line(img, Point(width/2+100, length+100), Point(width/2+101, length+101), Scalar(0, 0, 255), 10, 8, 0);
;

    double c_x = width/2+100;
    double c_y = length+100;

    double ck_x = c_x + (check_point.x - current_point.x);   //checkpoints x
    double ck_y = c_y - (check_point.y - current_point.y);   //checkpoints y

    double rf_x = c_x + (new_ref_point.x - current_point.x);
    double rf_y = c_y - (new_ref_point.y - current_point.y);

    //Drawing the checkpoint on the inner rectangle
    line(img, Point(ck_x, ck_y), Point(ck_x+1, ck_y+1), Scalar(255, 178, 102), 10, 8, 0);

    //Drawing the new refernce point found on the rectangle
    line(img, Point(rf_x, rf_y), Point(rf_x+2, rf_y+2), Scalar(0, 255, 0), 10, 8, 0);

    /*
     *computing the slope of the parabola with centre at the current location of the car and
     * passing through the obtained new reference point
     */

    if(!check_point_out)
	{

    double prev_x = current_point.x;
    double prev_y = current_point.y;
    double p = pow((new_ref_point.y - current_point.y), 2) / (4*(new_ref_point.x - current_point.x));

    double p_1 = pow((new_ref_point.y+car_length/6 - current_point.y), 2) / (4*(new_ref_point.x - current_point.x));
    double p_2 = pow((new_ref_point.y-car_length/6 - current_point.y), 2) / (4*(new_ref_point.x - current_point.x));


    if(straight)     //if there is no need to take any turn and move straight
    {
        //Drawing a straight line perpendicularly through the current location
        line(img, Point(c_x, c_y), Point(c_x, c_y-(new_ref_point.y-current_point.y)), Scalar(120, 255, 120), 2, 7, 0);
    }
    else
    {
        if(direction)   //If the checkpoint is towards right, draw a parabola towards right
        {
            for(int x = 0; x < width/2; x++)
            {
                double x1 = current_point.x + x;
                double y1 = sqrt(4*p*(x1 - current_point.x)) + current_point.y;
                line(img, Point(c_x+(prev_x - current_point.x), c_y-(prev_y - current_point.y)), Point(c_x+(x1-current_point.x), c_y-(y1-current_point.y)), Scalar(120, 255, 120), 2, 7, 0);
                prev_x = x1;
                prev_y = y1;
            }

            prev_x = current_point.x;
            prev_y = current_point.y;

            for(int x = 0; x < width/2; x++)
            {
                double x2 = current_point.x + x;
                double y2 = sqrt(4*p_1*(x2 - current_point.x)) + current_point.y;
                line(img, Point(c_x+(prev_x - current_point.x), c_y-(prev_y - current_point.y)), Point(c_x+(x2-current_point.x), c_y-(y2-current_point.y)), Scalar(65, 119, 255), 2, 7, 0);
                prev_x = x2;
                prev_y = y2;
            }

            prev_x = current_point.x;
            prev_y = current_point.y;

            for(int x = 0; x < width/2; x++)
            {
                double x3 = current_point.x + x;
                double y3 = sqrt(4*p_2*(x3 - current_point.x)) + current_point.y;
                line(img, Point(c_x+(prev_x - current_point.x), c_y-(prev_y - current_point.y)), Point(c_x+(x3-current_point.x), c_y-(y3-current_point.y)), Scalar(65, 119, 255), 2, 7, 0);
                prev_x = x3;
                prev_y = y3;
            }
        }
        else  // if the checkpoint is towards left, draw a parabola towards left
        {
            for(int x = 0; x > -width/2; x--)
            {
                double x1 = current_point.x + x;
                double y1 = sqrt(4*p*(x1 - current_point.x)) + current_point.y;
                line(img, Point(c_x+(prev_x - current_point.x), c_y-(prev_y - current_point.y)), Point(c_x+(x1-current_point.x), c_y-(y1-current_point.y)), Scalar(120, 255, 120), 2, 7, 0);
                prev_x = x1;
                prev_y = y1;
            }

            prev_x = current_point.x;
            prev_y = current_point.y;

            for(int x = 0; x > -width/2; x--)
            {
                double x2 = current_point.x + x;
                double y2 = sqrt(4*p_1*(x2 - current_point.x)) + current_point.y;
                line(img, Point(c_x+(prev_x - current_point.x), c_y-(prev_y - current_point.y)), Point(c_x+(x2-current_point.x), c_y-(y2-current_point.y)), Scalar(65, 119, 255), 2, 7, 0);
                prev_x = x2;
                prev_y = y2;
            }

            prev_x = current_point.x;
            prev_y = current_point.y;

            for(int x = 0; x > -width/2; x--)
            {
                double x3 = current_point.x + x;
                double y3 = sqrt(4*p_2*(x3 - current_point.x)) + current_point.y;
                line(img, Point(c_x+(prev_x - current_point.x), c_y-(prev_y - current_point.y)), Point(c_x+(x3-current_point.x), c_y-(y3-current_point.y)), Scalar(65, 119, 255), 2, 7, 0);
                prev_x = x3;
                prev_y = y3;
            }
        }
    }
}

    //show the image
    imshow("image", img);
    waitKey(0);


}


vector<Parabola> predefine_reference_points(double car_length, double width, co_ordinate src)
{
	//parabolas from sharp-sharp to big-big
	vector<Parabola> rightR_parabolas;      // parabolas towards right
	vector<Parabola> leftR_parabolas;       // parabolas towards left

	double u = car_length/3;   //lenght of the sub-segment eg: sharp-sharp to sharp-medium is one third of the car's length
	double para_P;             //slope of the parabola

	//cout << "Right reference points: ";

	/* Computing reference points and parabolas, passing through them from current location as centre,
	 * towards RIGHT to the car and storing them to a vector
	 */
    for(int i = 0; i < 9; i++)
	{
		co_ordinate pointR(src.x + width, src.y + u);

		//Computing the slope of
		para_P = pow(u, 2) / (4 * width);

		//cout << " (" << pointR.x << ", " << pointR.y << ")  p: " << para_P << " ";

		Parabola para(para_P, src, pointR);

		rightR_parabolas.push_back(para);

		u += car_length/3;
	}

    /* Computing reference points and parabolas, passing through them from current location as centre,
	 * towards LEFT to the car and storing them to a vector
	 */
	u = car_length/3;
	//cout << endl << "left reference points:";
	for(int i = 0; i < 9; i++)
	{
		co_ordinate pointL(src.x - width, src.y + u);

		para_P = pow((pointL.y - src.y), 2) / (4 * (pointL.x - src.x));

		//cout << " (" << pointL.x << ", " << pointL.y << ")  p: " << para_P << " ";

		Parabola para(para_P, src, pointL);

		leftR_parabolas.push_back(para);

		u += car_length/3;
	}

	//Returning RIGHT or LEFT parabolas depending on the checkpoint's location with respect to the car's current location
	if(direction)
	{
		return rightR_parabolas;
	}
	else
	{
		return leftR_parabolas;
	}
}

/* Steps to find the nearest points:
 * 1)  Calculating the distance from the current location to the checkpoint and name it as "d_t"
 * 2)  Computing the points on the parabolas which are at distance of "d_t" from the current location and name them as p_dt[i].
 * 3)  Compute the distances from the each of the p_dt to the checkpoint
 * 4)  Find the closest and second closest p_dt[i] to the checkpoint and the parabolas passing throught them are named as minimum and rank1 parabolas.
 */
co_ordinate new_reference_point_towards_checkpoint(co_ordinate src, co_ordinate checkpoint, vector<Parabola> finalParabolas, double car_length)
{
    // d_t = distance from the current location to the checkpoint
	vector<Point> PointParaDist;  // Points on the each parabola with a distance of "d_t" from the current location

	//finding the distance of the checkpoint from the current location.
	double src_ck_Dist = sqrt( pow(checkpoint.y - src.y, 2) + pow(checkpoint.x - src.x, 2) );
	//cout << "\ndist ck: " << src_ck_Dist << endl;

	// src = source = current_location of the car
	// calculating the point on the parabola, with the same distance of src to ck1, from the src.
	double p, x, y;
	for(int i = 0; i < finalParabolas.size(); i++)
	{
		p = finalParabolas[i].p;
//		x = sqrt(pow(src_ck_Dist, 2) - pow((4*p*(x - src.x) + src.y)-src.y, 2)) + src.x;
//		y = 4*p*(x - src.x) + src.y;

//		x = src.x + sqrt(pow(src_ck_Dist, 2) / (1+(4*pow(p,2))));
//		y = 4*p*(x - src.x) + src.y;


		if(direction)  //if the checkpoint is right wrt car
		{
			x = ((-4*p + sqrt(pow(4*p, 2) + 4*pow(src_ck_Dist, 2)))/2) + src.x;
			y = sqrt(4*p*(x - src.x)) + src.y;
		}
		else         // if checkpoin is left wrt car
		{
			x = ((-4*p - sqrt(pow(4*p, 2) + 4*pow(src_ck_Dist, 2)))/2) + src.x;
			y = sqrt(4*p*(x - src.x)) + src.y;
		}

		Point pDist(x, y);
		PointParaDist.push_back(pDist);
	}

	//finding the closest point computed on parabola to the checkpoint
	double smallest = INF;
	double distance;
	int index_min;

	//cout << "\nindex min: ";
	for(int i = 0; i < PointParaDist.size(); i++)  //finding the minimum parabola
	{
		distance = sqrt( pow((PointParaDist[i].x - checkpoint.x), 2) + pow((PointParaDist[i].y - checkpoint.y), 2));

		if(smallest > distance)
		{
			smallest = distance;
			index_min = i;           //finding the smallest index
		}

	}


	//Finding the second closest point, checkpoint resides inbetween minimum and rank1 parabolas
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

	//Computing the midpoint of the minimum and rank1 parabola's reference points on the rectangle
	co_ordinate new_ref_point((finalParabolas[index_min].refer.x + finalParabolas[index_rank1].refer.x)/2,
			(finalParabolas[index_min].refer.y + finalParabolas[index_rank1].refer.y)/2);

    /*
    * EXCEPTION 1: The check_point is below the sharp_sharp turn segment
    */
	if(index_min == -1 || index_rank1 == -1)
	{
		cout << " ******* Can't turn beyond sharp - sharp turn ********" << endl;
		new_ref_point.x = finalParabolas[0].refer.x;
		new_ref_point.y = finalParabolas[0].refer.y;
	}
	/*
	*  EXCEPTION 2: The checkpoint is above big_big turn segment
	*/
	else if(index_rank1 == 9)
	{
		cout << "******** No need to turn go straight ****************" << endl;
		new_ref_point.x = src.x;
		new_ref_point.y = src.y + 3*car_length;
		straight = true;
	}
	else   //No exception
	{

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

		cout << "\n\n minimum reference point:  ( " << finalParabolas[index_min].refer.x << ", " << finalParabolas[index_min].refer.y << ")  -  " << a[1] << "-" << a[0] << endl;
		cout << " rank1 reference point:    ( " << finalParabolas[index_rank1].refer.x << ", " << finalParabolas[index_rank1].refer.y << ")  -  "  << b[1] << "-" << b[0] << endl;
	}

	if(finalParabolas[index_min].refer == checkpoint)
	{
		return finalParabolas[index_min].refer;
	}
	else
		return new_ref_point;

}
