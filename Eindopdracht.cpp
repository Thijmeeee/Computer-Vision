// OpenCvTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#ifdef _DEBUG
#pragma comment(lib, "opencv_world490d")
#else
#pragma comment(lib, "opencv_world490")
#endif

#include <iostream>
#include <opencv2/opencv.hpp>
// #include <experimental/filesystem>

using namespace cv;
using namespace std;


void main()
{
	Mat img;
	VideoCapture cap(0);

	CascadeClassifier plateCascade;
	plateCascade.load("Resources/haarcascade_russian_plate_number.xml");

	if (plateCascade.empty()) { cout << "File not loaded" << endl; }

	vector<Rect> plates;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	int count = 0;
	while (true)
	{
		cap.read(img);
		plateCascade.detectMultiScale(img, plates, 1.1, 10);

		for (int i = 0; i < static_cast<int>(plates.size()); i++)
		{
			Mat imgCrop = img(plates[i]);

			Mat cannyImg, erodeImg, grayImg, rangeImg;
			cvtColor(imgCrop, grayImg, COLOR_BGR2GRAY);
			inRange(grayImg, Scalar(40.0, 40.0, 40.0), Scalar(90.0, 90.0, 90.0), rangeImg);
			Canny(rangeImg, cannyImg, 40, 60);
			
			// imshow("Image", rangeImg);
			// imshow("Image", cannyImg);
			// waitKey();

			findContours(cannyImg, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

			for (const vector<Point>& contour : contours)
			{
				double area = contourArea(contour);
				Rect rect = boundingRect(contour);
				double aspectRatio = static_cast<double>(rect.width) / rect.height;
				if (area > 200.0 && aspectRatio < 1.0)
				{
					Mat cropImg;
					cropImg = imgCrop(rect);
					// imshow("Cropped Image", cropImg);
					imwrite("Resources/Plates/" + to_string(count) + ".png", cropImg);
					cout << area << " | img: " << count << " | aspectRatio: " << aspectRatio << '\n';
					count++;
				}
			}

			rectangle(img, plates[i].tl(), plates[i].br(), Scalar(255, 0, 255), 3);
		}

		imshow("Image", img);
		waitKey(100);
	}
}
