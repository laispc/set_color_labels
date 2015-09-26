/*
Visão Computacional (SEL0635)
Laís Pessine do Carmo (7546760)

Exercicio 1: Segmentacao de Cores

INSTRUÇÕES:

1) O arquivo cores.jpeg deve estar no mesmo diretório deste código.
2) Compilar o arquivo usando o comando:
g++ -g -o SetLabels setLabels.cpp `pkg-config --cflags --libs opencv`

3) Para executar, use:
./SetLabels

*/

#include <cv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define MIN_GREEN 40
#define MAX_GREEN 60

#define MIN_BLUE 100
#define MAX_BLUE 107

#define MIN_ORANGE 8
#define MAX_ORANGE 11

#define MIN_YELLOW 26
#define MAX_YELLOW 29

#define MIN_PINK 1
#define MAX_PINK 5

#define MIN_RED 3
#define MAX_RED 7

using namespace cv;

Mat setLabel(Mat img_hsv, int hmin, int smin, int vmin, int hmax, int smax, int vmax)
{
	Mat mask = img_hsv.clone();						// Mask used to detect a label
	Mat label_color = img_hsv.clone();				// Stores final label
	Mat kernel_erode = Mat::ones(4,4, CV_8UC1);		// Erode kernel

	inRange(img_hsv, Scalar(hmin,smin,vmin), Scalar(hmax, smax, vmax), mask);	// Mask for green color (result is a binary image)
	erode(mask, mask, kernel_erode, Point(-1,-1), 1);		// Erode noise pixels
	dilate(mask, mask, kernel_erode, Point(-1,-1), 1);		// Dilate remaining areas
	cvtColor(mask, label_color, CV_GRAY2BGR);				// Convert mask to RGB (need 3 channels for setTo() ) 
	cvtColor(label_color, label_color, CV_BGR2HSV);			// Convert label to HSV (easier to set desired color)
	label_color.setTo(Scalar(hmin,smax,vmax), mask);		// Replace white by label color
	cvtColor(label_color, label_color, CV_HSV2BGR);			// Back to RGB again (easier to merge with other labels!)

	return label_color;
}

int main( int argc, char** argv )
{
	Mat img_src = imread("cores.jpeg", CV_LOAD_IMAGE_COLOR); // Load source
	Mat img_hsv;
	cvtColor(img_src, img_hsv, CV_BGR2HSV);   	// Convert original to HSV (easier to detect colors)
    namedWindow( "Original", WINDOW_AUTOSIZE );	// Create a window for display.
    imshow( "Original", img_src );              // Show original image inside it.

	cvWaitKey(0);				//  Wait for the user to press any key.

	int width, height;			// Image dimensions.
	Size size = img_hsv.size();
	height = size.height;
	width = size.width;

	Mat mask = img_hsv.clone();		// Used to detect a label
	
	// First, each label is stored in a different label
	Mat label_green = setLabel(img_hsv, MIN_GREEN,0,0,MAX_GREEN,255,255);	
	Mat label_blue = setLabel(img_hsv, MIN_BLUE,0,0,MAX_BLUE,255,255);;
	Mat label_orange = setLabel(img_hsv, MIN_ORANGE,0,0,MAX_ORANGE,255,255);;
	Mat label_yellow = setLabel(img_hsv, MIN_YELLOW,130,130,MAX_YELLOW,255,255);;
	Mat label_pink = setLabel(img_hsv, MIN_PINK,0,0,MAX_PINK,180,255);;
	Mat label_red = setLabel(img_hsv, MIN_RED,190,0,MAX_RED,255,255);;

	// Create final image
	Mat final_img = img_src.clone();
	final_img.setTo(Scalar(0,0,0));

	//Merge everything
	final_img = label_green + label_blue + label_yellow + label_orange + label_pink + label_red;

    namedWindow( "Final", WINDOW_AUTOSIZE );	// Create a window for display.
    imshow( "Final", final_img );

	cvWaitKey(0);	// Wait for key
	
	imwrite("labels.jpg",final_img); // Save result
	
	// Release memory
	img_src.release();
	img_hsv.release();
	label_green.release();
	label_green.release();
	label_blue.release();
	label_yellow.release();
	label_orange.release();
	label_pink.release();
	label_red.release();
	final_img.release();
	
	// Destroy all windows
	cvDestroyWindow("mat original"); // Destrói a janela aberta
	cvDestroyWindow("mat final"); // Destrói a janela aberta
	
}
