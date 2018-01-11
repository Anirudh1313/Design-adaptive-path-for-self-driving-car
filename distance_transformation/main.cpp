// for normal pot images


#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
using namespace std;
using namespace cv;

void floodImage(const Mat& B, Mat& objects);
void floodObject(int x, int y, Mat& objects, const Mat& B);
void printObjects(const Mat& src);
void flood(const Mat &src);

Vec3b currentTag = Vec3b(theRNG().uniform(0, 255), theRNG().uniform(0, 255), theRNG().uniform(0, 255));

int main(int, char** argv)
{
        // Load the image
     Mat src;
     //src = imread("pots.jpg");
     //try with second image
     src = imread("p2.jpeg");

     Mat srcG;

     cvtColor(src, srcG, CV_BGR2GRAY);

     Mat bImg, src1;
     imshow("original ", src);
     imshow("gray ", srcG);
     Mat outz;


     //operator 3 = morph close
     int morph_size = 20, operator1 = 3;
     Mat elem = getStructuringElement(2, Size(2*morph_size+1, 2*morph_size+1), Point( morph_size, morph_size) );
     morphologyEx(srcG, outz, operator1, elem);

     threshold(outz, src1, 150, 255, THRESH_BINARY_INV);
     imshow("morph close", outz);
     imshow("morph close binary ", src1);

     Mat src_8u;
     src1.convertTo(src_8u, CV_8UC3);

     imshow("binary 8U", src_8u);

     flood(src_8u);

     waitKey(0);

     return 0;

}


void floodObject(int x, int y, Mat& objects,const Mat& B) // flood object from current pixel; this procedure is recursive
{
    int ROWS = objects.rows;
    int COLUMNS = objects.cols;

	objects.at<Vec3b>(x, y) = currentTag; // tag the current pixel
#ifdef DEBUG
	printf("pixel %i,%i tagged as part of object %i\n", x, y, currentTag);
#endif

	// see if East neighbor is in the same object (and within the boundaries of the binary image B)
	if (y < COLUMNS-1) { // there is an East neighbor
		if (B.at<Vec3b>(x, y+1)[0] > 0 || B.at<Vec3b>(x, y+1)[1] > 0 || B.at<Vec3b>(x, y+1)[2] > 0) { // and it is part of (the same) object
			if (objects.at<Vec3b>(x, y+1)== Vec3b(0,0,0)) // and it has not been tagged already
				floodObject(x, y+1, objects, B); // continue flooding the current object from the East neighbor (recursively)
		}
	} // done with East neighbor

	// see if South neighbor is in the same object (and within the boundaries of the binary image B)
	if (x < ROWS-1) { // there is an South neighbor
		if (B.at<Vec3b>(x+1, y)[0] > 0 || B.at<Vec3b>(x+1, y)[1] > 0 || B.at<Vec3b>(x+1, y)[2] > 0) { // and it is part of (the same) object
			if (objects.at<Vec3b>(x+1, y)== Vec3b(0,0,0)) // and it has not been tagged already
				floodObject(x+1, y, objects, B); // continue flooding the current object from the South neighbor (recursively)
		}
	} // done with South neighbor

	// see if West neighbor is in the same object (and within the boundaries of the binary image B)
	if (y > 0) { // there is an West neighbor
		if (B.at<Vec3b>(x, y-1)[0] > 0 || B.at<Vec3b>(x, y-1)[1] > 0 || B.at<Vec3b>(x, y-1)[2] > 0) { // and it is part of (the same) object
			if (objects.at<Vec3b>(x, y-1)== Vec3b(0,0,0)) // and it has not been tagged already
				floodObject(x, y-1, objects, B); // continue flooding the current object from the West neighbor (recursively)
		}
	} // done with West neighbor

	// see if North neighbor is in the same object (and within the boundaries of the binary image B)
	if (x > 0) { // there is an North neighbor
		if (B.at<Vec3b>(x-1, y)[0] > 0 || B.at<Vec3b>(x-1, y)[1] > 0 || B.at<Vec3b>(x-1, y)[2] > 0) { // and it is part of (the same) object
			if (objects.at<Vec3b>(x-1, y)==Vec3b(0,0,0)) // and it has not been tagged already
				floodObject(x-1, y, objects, B); // continue flooding the current object from the North neighbor (recursively)
		}
	} // done with North neighbor

} // end flood


void floodImage(const Mat& B, Mat& objects)
{
	int ROWS = objects.rows, COLUMNS = objects.cols; // local variables for search
	int x, y;
	// initialize output image objects to all zeros
	for (x = 0; x < ROWS; x++)
		for (y = 0; y < COLUMNS; y++)
			objects.at<Vec3b>(x, y) = Vec3b(0,0,0);

	// initializde the currentTag to the value of the first object
    currentTag[0] = uchar(theRNG().uniform(0, 255));
    currentTag[1] = uchar(theRNG().uniform(0, 255));
    currentTag[2] = uchar(theRNG().uniform(0, 255));

	// search each pixel in binary image B
#ifdef DEBUG
	printB();
	printObjects();
	printf("start search\n");
#endif

	for (x = 0; x < ROWS; x++) { // for each row in binary image B
#ifdef DEBUG
	 	printf("start row %i\n", x);
#endif
		for (y = 0; y < COLUMNS; y++) { // for each column in B
			if (B.at<Vec3b>(x,y)[0] > 0 || B.at<Vec3b>(x,y)[1] > 0 || B.at<Vec3b>(x,y)[2] > 0) { // this pixel is part of an object
#ifdef DEBUG
	 			printf("pixel %i,%i is part of an object\n", x, y);
#endif
				if (objects.at<Vec3b>(x, y) == Vec3b(0,0,0)) { // that has not beed previously tagged
#ifdef DEBUG
	 				printf("that has not beed previously tagged\n");
#endif
					floodObject(x, y, objects, B); // flood object starting at current pixel as seed
#ifdef DEBUG
	 				printf("object %i flooded\n", currentTag);
#endif
                        currentTag[0] = uchar(theRNG().uniform(0, 255));
                        currentTag[1] = uchar(theRNG().uniform(0, 255));
                        currentTag[2] = uchar(theRNG().uniform(0, 255));
                                    // increment currentTag_ to value for next new object found
#ifdef DEBUG
	 				printf("next object tag: %i\n", currentTag);
#endif
				}
			} // done with pixel
		} // end of row search
	} // end of image search
}

void printObjects(const Mat& src)
{
	int rows = src.rows, cols = src.cols;

	for (int x=0; x<rows; x++)
	{ // for each row in binary image B
		for (int y=0; y<cols; y++) // for each column in binary image B
		{
		    cout << src.at<Vec3b>(x, y);
		}
		cout << endl;
	}
}

void flood(const Mat &src)
{
    Mat outs;
    outs.create(src.rows, src.cols/3, CV_8SC3);
    floodImage(src, outs);

    imshow("flood_fill / 3 ", outs);


    Mat outs_3 = Mat::zeros(src.rows, src.cols, CV_8UC3);

    for(int j = src.cols/3-1; j >= 0; j--)
    {
        for(int i = src.rows-1; i >= 0; i--)
        {
            outs_3.at<Vec3b>(i, (j+1)*3-1) = outs.at<Vec3b>(i, j);
            outs_3.at<Vec3b>(i, (j+1)*3-2) = outs.at<Vec3b>(i, j);
            outs_3.at<Vec3b>(i, (j+1)*3-3) = outs.at<Vec3b>(i, j);
        }
    }

    for(int j = (src.cols/3)*3; j < src.cols; j++)
    {
        for(int i = 0; i < src.rows; i++)
        {
            outs_3.at<Vec3b>(i, j) = Vec3b(0,0,0);
        }
    }

    imshow("flood-fill * 3 == ", outs_3);
}
