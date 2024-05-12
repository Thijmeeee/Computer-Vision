// OpenCvTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#ifdef _DEBUG
#pragma comment(lib, "opencv_world490d")
#else
#pragma comment(lib, "opencv_world490")
#endif

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int threshhold1 = 0;

void opdracht2_3() {
    Mat img = imread("blacklines.png");
    Mat imgErode, kernel;

    kernel = getStructuringElement(MORPH_RECT, Size(9, 9));

    erode(img, imgErode, kernel);

    imshow("Image", img);
    imshow("Image Erode", imgErode);

    waitKey();
}

void opdracht2_2() {
    Mat img = imread("torenvanpisa.png");
    Mat imgCanny;

    namedWindow("Trackbars", (640, 200));
    createTrackbar("Canny", "Trackbars", &threshhold1, 255);

    while (true) {
        Canny(img, imgCanny, threshhold1, 255);
        imshow("Image Canny", imgCanny);
        waitKey(5);
    }

    
}


void opdracht2_1() {

    Mat img = imread("test.jpeg");
    Mat imgResize, imgBlackWhite, imgFilter1, imgFilter2, imgFilter3, kernelHor, kernelVer, kernelDia;

    cvtColor(img, imgBlackWhite, COLOR_BGR2GRAY);

    kernelHor = (Mat_<float>(3, 3) <<
        -1, -1, -1,
        2, 2, 2,
        -1,-1, -1);

    kernelVer = (Mat_<float>(3, 3) <<
        -1, 2, -1,
        -1, 2, -1,
        -1, 2, -1);

    kernelDia = (Mat_<float>(3, 3) <<
        -1, -1, 2,
        -1, 2, -1,
        2, -1, -1);

    imshow("Image", img);
 
  //  filter2D(img, imgFilter1, MORPH_RECT, kernelHor);
  //  filter2D(img, imgFilter2, MORPH_RECT, kernelVer);
  //  filter2D(img, imgFilter3, MORPH_RECT, kernelDia);

  ////  imshow("Image Black White", imgBlackWhite);
  // imshow("Image Filter1", imgFilter1);
  // imshow("Image Filter2", imgFilter2);
  // imshow("Image Filter3", imgFilter3);

    waitKey();
}


//int main() {
//
//    opdracht2_2();
//
//	return 1;	
//}
