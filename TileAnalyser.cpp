//
//  TileAnalyser.cpp
//  Image_Mosaic
//
//  Created by WangAnnan on 22/10/2018.
//  Copyright © 2018 WangAnnan. All rights reserved.
//

#include "TileAnalyser.hpp"

void TileAnalyser(vector<Mat> &tiles, vector<Mat> &resizedTiles, vector<Vec3b> &averages, vector<int> &hue)
{
    int rgbArray[SIZE][RGB];
    for (int i = 0; i < int(tiles.size()); i++)
    {
        resizedTiles.push_back(resizer(tiles.at((unsigned int) i), BREAK, BREAK));
        readPixel(SIZE, rgbArray, resizedTiles.at((unsigned int) i), 0, 0, BREAK);
        averages.push_back(averageValue(SIZE, rgbArray));
        hue.push_back((int)hsvTrans(averageValue(SIZE, rgbArray))[0] * 2);
    }
//    tiles.swap(resizedTiles);
} // ImageAnalysis

Mat resizer(Mat targetImg, int col, int row)
{
    Mat tempImg;
    
    resize(targetImg, tempImg, Size(row, col));
    return tempImg;
} // resizer

Vec3b hsvTrans(Vec3b rgbAverage)
{
    Mat3b hsvAverage;
    Mat3b rgb(rgbAverage);
    
    cvtColor(rgb, hsvAverage, COLOR_BGR2HSV);     // transform rgb value to hsv value
    return hsvAverage.at<Vec3b>();
} // hsvTrans
