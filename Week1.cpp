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
using namespace std;


void opdracht2a() {

    Mat img = imread("test.jpeg");
    Mat imgResize, imgCrop;

    // to resize the image to another size
    resize(img, imgResize, Size(), 0.5, 0.5);

    // to crop the image to get a specific area of the image
    Rect roi(100, 100, 500, 300);
    imgCrop = img(roi);

    imshow("Image", img);
    imshow("Image Resize", imgResize);
    imshow("Image Crop", imgCrop);

    waitKey();
}

void opdracht3a() {
    // Blank image
    Mat img(512, 512, CV_8UC3, Scalar(255, 255, 255)); // BGR
    
    circle(img, Point(256, 256), 155, Scalar(0, 69, 255), FILLED);
    putText(img, "Jooo maatjes", Point(180, 250), FONT_HERSHEY_DUPLEX, 0.75, Scalar(255, 255, 255), 2);
   
    imshow("Image", img);

    waitKey();
}

void opdracht4a() {
    float w = 250, h = 650;
    Mat img = imread("torenvanpisa.png");
    Mat matrix, imgWrap, imgWrapGray, imgWrapBlur, imgWrapCanny, imgWrapDilate, imgWrapErode;


    Point2f src[4] = { {366, 200}, {541, 200}, {253, 934}, {483, 934} };
    Point2f dst[4] = { {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };

    matrix = getPerspectiveTransform(src, dst);
    warpPerspective(img, imgWrap, matrix, Point(w, h));

    cvtColor(imgWrap, imgWrapGray, COLOR_BGR2GRAY);
    GaussianBlur(imgWrap, imgWrapBlur, Size(9, 9), 0, 0);
    Canny(imgWrapBlur, imgWrapCanny, 15, 20);

    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3)); 
    dilate(imgWrapCanny, imgWrapDilate, kernel);

    erode(imgWrapDilate, imgWrapErode, kernel);
    
    imshow("Image", img);
    imshow("Image Wrap", imgWrap);
 /*   imshow("Image Gray", imgWrapGray);
    imshow("Image Blur", imgWrapBlur);
    imshow("Image Canny", imgWrapCanny);
    imshow("Image Dilate", imgWrapDilate);
    imshow("Image Erode", imgWrapErode);*/
    waitKey();
}
//
//int main()
//{
//    opdracht4a();
//}
