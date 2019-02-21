//
//  Tiler.cpp
//  Image_Mosaic
//
//  Created by WangAnnan on 08/10/2018.
//  Copyright © 2018 WangAnnan. All rights reserved.
//

#include "Tiler.hpp"
#include "TileAnalyser.hpp"
#include "ImageCutter.hpp"

Mat Tiler(Mat mosaicTarget, Mat targetImg, vector<Mat> resizedTiles, vector<int> hue)
{   
    Mat tempImg = mosaicTarget;
    Mat bestFitTile, result;
    
    int i = 0, j = 0, pixelX = 0, pixelY = 0;
    int height = tempImg.rows;
    int width = tempImg.cols;
    Vec3b averageRGB;

    Mat borderImg = targetImg;
    if (borderImg.cols % BREAK != 0 || borderImg.rows % BREAK != 0)
        borderImg = edgeBorder(borderImg);

    result = mosaicTarget;

    // initialise the count of the use of tiles
    
    for (i = 0; i < width; i+=BREAK)
    {
        for (j = 0; j < height; j+=BREAK)
        {
            pixelX = i;
            pixelY = j;
            averageRGB = tempImg.at<Vec3b>(pixelY,pixelX);

            bestFitTile = compareHue(resizedTiles, averageRGB, hue);
            
            pixelX = i;     // the first pixel
            pixelY = j;

            result = tileReplacement(SIZE, tempImg, borderImg, bestFitTile, pixelY, pixelX, BREAK);
        } // for
    } // for

    return result;
} // Tiler

Mat compareHue(vector<Mat> tiles, Vec3b averageRGB, vector<int> hue)
{
    int averageHue = (int)hsvTrans(averageRGB)[0] * 2;
    
    int temp, currentMin = 360; // the hue difference is 0-360
    int bestFitIndex = 0;
    int best[3];
    
    for (int i = 0; i < int(hue.size()); i++)
    {
        temp = abs(hue.at((unsigned int) i) - averageHue);        // stored as an absolute value, need to be none-negative
        if (currentMin > temp)
        {
            currentMin = temp;
            bestFitIndex = i;
        } // if
    } // for
    return tiles.at((unsigned int)bestFitIndex);
} // compareHue

Mat tileReplacement(int size, Mat mosaicImg, Mat targetImg, Mat tile, int pixelY, int pixelX, int breakpoint)
{
    Mat result = mosaicImg;
    int temp = 0, i = 0;
    
    for (int k = 0; k < size; k++)
    {
        result.at<Vec3b>(pixelY, pixelX + temp)
                = (1 - transparency) * tile.at<Vec3b>(i, temp) +transparency * targetImg.at<Vec3b>(pixelY, pixelX + temp);
        temp++;
        if (temp == breakpoint)
        {
            temp = 0;
            i++;
            pixelY++;
        } // if
    } // for
    return result;
} // tileReplacement
