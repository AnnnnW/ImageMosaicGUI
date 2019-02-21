//
//  Tiler.hpp
//  Image_Mosaic
//
//  Created by WangAnnan on 08/10/2018.
//  Copyright © 2018 WangAnnan. All rights reserved.
//

#ifndef Tiler_hpp
#define Tiler_hpp

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <cmath>

using namespace cv;
using namespace std;

#define transparency 0.6

Mat Tiler(Mat mosaicTarget, Mat targetImg, vector<Mat> resizedTiles, vector<int> hue);
Mat compareHue(vector<Mat> tiles,Vec3b averageRGB, vector<int> hue);
Mat tileReplacement(int size, Mat mosaicImg, Mat targetImg, Mat tile, int pixelY, int pixelX, int breakpoint);

#endif /* Tiler_hpp */
