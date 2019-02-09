//
//  TileAnalysor.hpp
//  Image_Mosaic
//
//  Created by WangAnnan on 22/10/2018.
//  Copyright © 2018 WangAnnan. All rights reserved.
//

#ifndef TileAnalyser_hpp
#define TileAnalyser_hpp

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <dirent.h>

#include "ImageCutter.hpp"

using namespace cv;
using namespace std;

int TileAnalyser(string readpath, string defaultpath);
Mat resizer(Mat targetImg, int col, int row);
Vec3b hsvTrans(Vec3b rgbAverage);
int writter(struct dirent *ptr, DIR *dir, vector<string> tiles, vector<Vec3b> averages, vector<int> hue, string path, string defaultpath);
#endif /* TileAnalyser_hpp */
