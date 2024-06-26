// OpenCvTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#ifdef _DEBUG
#pragma comment(lib, "opencv_world490d")
#else
#pragma comment(lib, "opencv_world490")
#endif

#include <iostream>
#include <direct.h>
#include <opencv2/opencv.hpp>
#include <filesystem>

using namespace cv;
using namespace std;


const int plateSize = 8;
bool foundPlate = false;

bool sortByX(const Rect& a, const Rect& b)
{
	return a.x < b.x;
}

int FindNextDirectoryIndex(string path)
{
	const filesystem::path root{path};

	std::cout << "directory_iterator:\n";

	int i = 0;
	for (auto const& dir_entry : filesystem::directory_iterator{root})
	{
		i++;
		std::cout << dir_entry.path() << '\n';
	}
	return i + 1; // return the next available index
}

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
	while (!foundPlate)
	{
		cap.read(img);
		plateCascade.detectMultiScale(img, plates, 1.1, 10);

		for (int i = 0; i < static_cast<int>(plates.size()); i++)
		{
			Mat imgCrop = img(plates[i]);

			Mat cannyImg, erodeImg, grayImg, rangeImg;
			cvtColor(imgCrop, grayImg, COLOR_BGR2GRAY);

			//filter letters
			inRange(grayImg, Scalar(0.0, 0.0, 0.0), Scalar(160.0, 160.0, 160.0), rangeImg);

			//find outlines
			Canny(rangeImg, cannyImg, 20, 90);

			//find contours of all the characters
			findContours(cannyImg, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

			vector<Rect> boundingBoxes;
			for (const vector<Point>& contour : contours)
			{
				double area = contourArea(contour);
				Rect rect = boundingRect(contour);
				double aspectRatio = static_cast<double>(rect.width) / rect.height;

				//only add characters with a large area and with an aspect ratio below 1
				if (area > 200.0 && aspectRatio < 1.0)
				{
					boundingBoxes.push_back(rect);
				}
			}

			//sort characters from left to right
			sort(boundingBoxes.begin(), boundingBoxes.end(), sortByX);

			cout << "size: " << static_cast<int>(boundingBoxes.size()) << endl;

			//check if the amount of found characters is the amount we expect
			if (static_cast<int>(boundingBoxes.size()) == plateSize)
			{
				int dirCount = FindNextDirectoryIndex("Resources/Plates/");
				string tempPath = string("Resources/Plates/kenteken") + to_string(dirCount);
				const char* newPath = tempPath.c_str();

				//make directory for new license plate
				_mkdir(newPath);

				//write all characters to the directory
				for (const Rect& rect : boundingBoxes)
				{
					string path = "Resources/Plates/kenteken"
						+ to_string(dirCount)
						+ "/kenteken"
						+ to_string(dirCount)
						+ '_'
						+ to_string(count)
						+ ".png";

					Mat cropImg = imgCrop(rect);
					imwrite(path, cropImg);
					cout << "img: " << count << " | rect: " << rect << '\n';
					count++;
				}
				foundPlate = true;
			}
			rectangle(img, plates[i].tl(), plates[i].br(), Scalar(255, 0, 255), 3);
		}
		imshow("Image", img);
		waitKey(100);
	}
}
