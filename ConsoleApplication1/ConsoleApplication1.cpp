//#include "cv.h" //main opencv header
//#include "highgui.h" //GUI header

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	/*IplImage* img;
	img = cvLoadImage(argv[1]);
	if (!img)
		printf("Could not load image file: %s\n", "dog.jpg");
	cvNamedWindow("Image");
	cvShowImage("Image", img);
	cvWaitKey(0);
	cvDestroyWindow("Image");
	cvReleaseImage(&img);
	return 0;*/

	Mat img = imread(argv[1], 1); // Load the color image
	if (!img.data) // check image is available
	{
		cout << "Could not load image file" << endl;
		return -1;
	}

	int height = img.rows;
	int width = img.cols;
	int widthstep = img.step;
	int ch = img.channels();
	printf("height: %d, widht: %d, width step: %d, chanels: %d\n", height, width, widthstep, ch);

	img.at<uchar>(10, 10) = 255;


	Mat gray(height, width, CV_8UC1, Scalar(0)); // create new image, CV_8UC1 means gray(one chanel),  Scalar(0) = default black color

	cvtColor(img, gray, COLOR_BGR2GRAY); // convert RGB image to Gray

	// Copy original image
	Mat copy(height, width, CV_8UC3, Scalar(0, 0, 0)); // create new image, CV_8UC3 means RGB(3 chanel),  Scalar(0, 0, 0) = default black color
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			copy.at<Vec3b>(y, x) = img.at<Vec3b>(y, x); // copy pixel by pixle
		}
	}

	int hist[256]; // create array to  put count of intensity level
	for (int i = 0; i < 256; i++){
		hist[i] = 0; // make all array eliment as 0
	}

	//counting the intensity
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			hist[(int)gray.at<uchar>(y, x)]++;
		}
	}

	for (int i = 0; i < 256; i++) {
		printf("%d: %d\n", i, hist[i]);
	}

	//create image to display the histograme
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / 255);
	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255));

	//Find the maximum number of pixels from histogram array
	int max_hist = hist[0];
	for (int i = 1; i < 256; i++){
		if (max_hist < hist[i]){
			max_hist = hist[i];
		}
	}

	//Normalize the histogram between 0 and histogram height
	for (int i = 0; i < 256; i++){
		hist[i] = ((double)hist[i] / max_hist) * hist_h;
	}

	//draw histrograme lines
	for (int i = 0; i < 256; i++) {
		line(histImage, Point(bin_w * (i), hist_h), Point(bin_w * (i), (hist_h - hist[i])), Scalar(0), 1, 8, 0); //line ( histImage, Point pt1, Point pt2,color , thickness, lineType, shift)
	}


	//default image
	namedWindow("Image"); // Create the window
	imshow("Image", img); // load and show image

	//gray image
	namedWindow("Gray"); // Create the window
	imshow("Gray", gray); // load and show image

	//copy of original image
	namedWindow("Copy"); // Create the window
	imshow("Copy", copy); // load and show image

	// show histrogram
	namedWindow("Histro"); // Create the window
	imshow("Histro", histImage); // load and show image

	waitKey(0); // wait to close

	return 0;

}