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

static int col; // the number of tiles in one column

Mat Tiler(Mat mosaicTarget, Mat targetImg, vector<Mat> resizedTiles, vector<int> hue, vector<int> &tileIndex, double overlayLevel);
Mat compareHue(vector<Mat> tiles,Vec3b averageRGB, vector<int> hue, vector<int> &tileIndex);
bool tileRepetition(int i, vector<int> tileIndex);
Mat tileReplacement(int size, Mat mosaicImg, Mat targetImg, Mat tile, double overlayLevel, int pixelY, int pixelX, int breakpoint);

#endif /* Tiler_hpp */
