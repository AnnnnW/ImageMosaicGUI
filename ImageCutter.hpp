//
//  ImageCutter.hpp
//  Image_Mosaic
//
//  Created by WangAnnan on 08/10/2018.
//  Copyright © 2018 WangAnnan. All rights reserved.
//

#ifndef ImageCutter_hpp
#define ImageCutter_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <math.h>

#define SIZE 625
#define BREAK 25
#define RGB 3

using namespace cv;
using namespace std;

int ImageCutter(Mat targetImage);
Mat edgeBorder(Mat targetImg);
Mat mosaicFilter(Mat targetImg, int height, int width);
void readPixel(int size, int array[size][RGB], Mat targetImg, int pixelY, int pixelX, int breakpoint);
void writePixel(int size, Vec3b average, Mat targetImg, int pixelY, int pixelX, int breakpoint);
Vec3b averageValue(int size, int array[size][RGB]);

#endif /* ImageCutter_hpp */
