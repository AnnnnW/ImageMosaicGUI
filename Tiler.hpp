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
static int row; // the number of tiles in one row

Mat Tiler(Mat mosaicTarget, Mat targetImg, vector<Mat> tiles, vector<Mat> resizedTiles, int tileHeight, int tileWidth, vector<int> hue, vector<int> &tileIndex, bool noRepetition, bool isUnequal, double overlayLevel);
Mat compareHue(vector<Mat> tiles,Vec3b averageRGB, vector<int> hue, vector<int> &tileIndex, bool noRepetition);
bool tileRepetition(int i, vector<int> tileIndex);
Mat tileReplacement(int size, Mat mosaicImg, Mat targetImg, Mat tile, double overlayLevel, int pixelY, int pixelX, int breakpoint);
Mat unequalSize(Mat result, Mat targetImg, vector<Mat> tiles, vector<int> tileIndex, int col, int row, int tileWidth, int tileHeight, double overlayLevel);

#endif /* Tiler_hpp */
