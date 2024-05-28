#ifdef _DEBUG
#pragma comment(lib, "opencv_world490d")
#else
#pragma comment(lib, "opencv_world490")
#endif

#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>

using namespace cv;
using namespace std;

bool sortByX(const Rect& a, const Rect& b) {
	return a.x < b.x;
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

			findContours(cannyImg, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

			vector<Rect> boundingBoxes;
			for (const vector<Point>& contour : contours)
			{
				double area = contourArea(contour);
				Rect rect = boundingRect(contour);
				double aspectRatio = static_cast<double>(rect.width) / rect.height;
				if (area > 200.0 && aspectRatio < 1.0)
				{
					boundingBoxes.push_back(rect);
				}
			}

			// Sorteer de bounding boxes van links naar rechts
			sort(boundingBoxes.begin(), boundingBoxes.end(), sortByX);

			// Verwijder dubbele bounding boxes
			boundingBoxes.erase(unique(boundingBoxes.begin(), boundingBoxes.end()), boundingBoxes.end());


			for (int i = boundingBoxes.size() - 1; i >= 0; i--)
			{
				for (int j = i - 1; j >= 0; j--)
				{
					if (abs(boundingBoxes[i].x - boundingBoxes[j].x) < 5)
					{
						boundingBoxes.erase(boundingBoxes.begin() + j);
						i--; 
					}
				}
			}


			for (const Rect& rect : boundingBoxes)
			{
				Mat cropImg = imgCrop(rect);
				imwrite("Resources/Plates/" + to_string(count) + ".png", cropImg);
				cout << "img: " << count << " | rect: " << rect << '\n';
				count++;
			}

			rectangle(img, plates[i].tl(), plates[i].br(), Scalar(255, 0, 255), 3);
		}

		imshow("Image", img);
		waitKey(100);
	}
}
