#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

//void ws(Mat src);
void floodImage(const Mat& B, Mat& objects);
void floodObject(int x, int y, Mat& objects, const Mat& B);
void printObjects(const Mat& src);
void flood(const Mat &src);

Vec3b currentTag = Vec3b(theRNG().uniform(0, 255), theRNG().uniform(0, 255), theRNG().uniform(0, 255));
int main(int, char** argv)
{
    // Load the image
    Mat src = imread("i.jpeg");
    Mat srcG;
    if(src.empty())
    {
    cout << "src not opened!!" << endl;
    return -1;
    }


    imshow("Source Image", src);

    cvtColor(src, srcG, CV_BGR2GRAY);

    imshow("gray ", srcG);



    for(int i = 0; i < src.rows; i++)
    {
        for(int j = 0; j < src.cols; j++)
        {
            if(src.at<Vec3b>(i, j) == Vec3b(255, 255,255))
            {
                src.at<Vec3b>(i, j)[0] = 0;
                src.at<Vec3b>(i, j)[1] = 0;
                src.at<Vec3b>(i, j)[2] = 0;
            }
        }
    }
    // Change the background from white to black, since that will help later to extract
    // better results during the use of Distance Transform
    // Show output image
    imshow("Black Background Image", src);
    // Create a kernel that we will use for accuting/sharpening our image
    Mat kernel = (Mat_<float>(3,3) <<
            1,  1, 1,
            1, -8, 1,
            1,  1, 1); // an approximation of second derivative, a quite strong kernel
    // do the laplacian filtering as it is
    // well, we need to convert everything in something more deeper then CV_8U
    // because the kernel has some negative values,
    // and we can expect in general to have a Laplacian image with negative values
    // BUT a 8bits unsigned int (the one we are working with) can contain values from 0 to 255
    // so the possible negative number will be truncated
    Mat imgLaplacian;
    Mat sharp = src; // copy source image to another temporary one
    filter2D(sharp, imgLaplacian, CV_32F, kernel);
    src.convertTo(sharp, CV_32F);
    Mat imgResult = sharp - imgLaplacian;
    // convert back to 8bits gray scale
    imgResult.convertTo(imgResult, CV_8UC3);
    imgLaplacian.convertTo(imgLaplacian, CV_8UC3);
    // imshow( "Laplace Filtered Image", imgLaplacian );
    imshow( "New Sharped Image", imgResult );
    src = imgResult; // copy back
    // Create binary image from source image
    Mat bw;
    cvtColor(src, bw, CV_BGR2GRAY);
    threshold(bw, bw, 40, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    imshow("Binary Image", bw);


    int morph_size = 20, operator1 = 4;
    Mat elem = getStructuringElement(2, Size(2*morph_size+1, 2*morph_size+1), Point( morph_size, morph_size) );
    morphologyEx(bw, bw, operator1, elem);

    imshow("close morphed", bw);

    Mat bw_8u;
    bw.convertTo(bw_8u, CV_8UC1);

    // Perform the distance transform algorithm
    Mat dist;
    distanceTransform(bw, dist, CV_DIST_L2, 3);
    // Normalize the distance image for range = {0.0, 1.0}
    // so we can visualize and threshold it
    normalize(dist, dist, 0, 1., NORM_MINMAX);
    imshow("Distance Transform Image", dist);
    // Threshold to obtain the peaks
    // This will be the markers for the foreground objects
    threshold(dist, dist, .4, 1., CV_THRESH_BINARY);
    // Dilate a bit the dist image
    Mat kernel1 = Mat::ones(3, 3, CV_8UC1);
    dilate(dist, dist, kernel1);
    imshow("Peaks", dist);


    // Create the CV_8U version of the distance image
    // It is needed for findContours()
    Mat dist_8u;
    dist.convertTo(dist_8u, CV_8UC3);
    // Find total markers

    imshow("Peaks_8U", dist_8u);
    //binary image
    //printObjects(dist_8u);

    //printObjects(outs);
    vector<vector<Point> > contours;
    findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    // Create the marker image for the watershed algorithm
    Mat markers = Mat::zeros(dist.size(), CV_32SC1);
    // Draw the foreground markers
    for (size_t i = 0; i < contours.size(); i++)
    drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i)+1), -1);
    // Draw the background marker
    circle(markers, Point(5,5), 3, CV_RGB(255,255,255), -1);
    imshow("Markers", markers*10000);
    // Perform the watershed algorithm
    watershed(src, markers);
    Mat mark = Mat::zeros(markers.size(), CV_8UC1);
    markers.convertTo(mark, CV_8UC1);
    bitwise_not(mark, mark);
    imshow("Markers_v2", mark); // uncomment this if you want to see how the mark
                                  // image looks like at that point
    // Generate random colors


    vector<Vec3b> colors;
    for (size_t i = 0; i < contours.size(); i++)
    {
        int b = theRNG().uniform(0, 255);
        int g = theRNG().uniform(0, 255);
        int r = theRNG().uniform(0, 255);
        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }
    // Create the result image
    Mat dst = Mat::zeros(markers.size(), CV_8UC3);
    // Fill labeled objects with random colors
    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
            int index = markers.at<int>(i,j);
            if (index > 0 && index <= static_cast<int>(contours.size()))
                dst.at<Vec3b>(i,j) = colors[index-1];
            else
                dst.at<Vec3b>(i,j) = Vec3b(0,0,0);
        }
    }
    // Visualize the final image
    imshow("Final Result", dst);
    waitKey(0);

    return 0;

//ws(outz);
    // Check if everything was fine
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
					currentTag = Vec3b(uchar(theRNG().uniform(0, 255)),
                                    uchar(theRNG().uniform(0, 255)), uchar(theRNG().uniform(0, 255)));
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

    imshow("flood_fill size/3", outs);


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

    imshow("floodfill * 3 == ", outs_3);
}
