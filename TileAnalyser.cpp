//
//  TileAnalyser.cpp
//  Image_Mosaic
//
//  Created by WangAnnan on 22/10/2018.
//  Copyright © 2018 WangAnnan. All rights reserved.
//

#include "TileAnalyser.hpp"

int TileAnalyser(vector<Mat> tiles, vector<Vec3b> averages, vector<int> hue, int rgbArray[SIZE][RGB])
{
    vector<Mat> resizedTiles;
    for (Mat tile : tiles)
    {
        resizedTiles.push_back(resizer(tile, BREAK, BREAK));
        readPixel(SIZE, rgbArray, tile, 0, 0, BREAK);
        averages.push_back(averageValue(SIZE, rgbArray));
        hue.push_back((int)hsvTrans(averageValue(SIZE, rgbArray))[0] * 2);
    }
    return 0;
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
