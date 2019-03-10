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

Mat Tiler(Mat mosaicTarget, Mat targetImg, vector<Mat> resizedTiles, int tileHeight, int tileWidth, vector<int> hue, vector<int> &tileIndex, bool noRepetition, double overlayLevel)
{   
    Mat bestFitTile, result;
    mosaicTarget.copyTo(result);
    
    int i = 0, j = 0, pixelX = 0, pixelY = 0;
    int height = result.rows;
    int width = result.cols;
    Vec3b averageRGB;
    int size = tileHeight * tileWidth;

    col = height / tileHeight;
    
    for (i = 0; i < width; i+=tileWidth)
    {
        for (j = 0; j < height; j+=tileHeight)
        {
            pixelX = i;
            pixelY = j;
            averageRGB = result.at<Vec3b>(pixelY,pixelX);

            bestFitTile = compareHue(resizedTiles, averageRGB, hue, tileIndex, noRepetition);
            
            pixelX = i;     // the first pixel
            pixelY = j;

            result = tileReplacement(size, result, targetImg, bestFitTile, overlayLevel, pixelY, pixelX, tileHeight);
        } // for
    } // for

    return result;
} // Tiler

Mat compareHue(vector<Mat> tiles, Vec3b averageRGB, vector<int> hue, vector<int> &tileIndex, bool noRepetition)
{
    int averageHue = int(hsvTrans(averageRGB)[0] * 2);
    
    int temp, currentMin = 360; // the hue difference is 0-360
    int bestFitIndex = 0;

    for (int i = 0; i < int(hue.size()); i++)
    {
        temp = abs(hue.at((unsigned int) i) - averageHue);        // stored as an absolute value, need to be none-negative
        if (currentMin > temp)
        {
            // check if it has shown before
            if (noRepetition)
            {
                if (!tileRepetition(i, tileIndex))
                {
                    currentMin = temp;
                    bestFitIndex = i;
                }
            }
            else
            {
                currentMin = temp;
                bestFitIndex = i;
            }
        } // if
    } // for

    tileIndex.push_back(bestFitIndex);
    return tiles.at((unsigned int)bestFitIndex);
} // compareHue

bool tileRepetition(int i, vector<int> tileIndex)
{
    bool isRepeat = false;
    int temp = int(tileIndex.size()) - 1; // the index of the last element in tileIndex
    if (temp >= 0)  // if no row ahead
    {
        for (int j = temp; j > temp-1; j--)
            if (tileIndex.at((unsigned int)j) == i)
            {
                isRepeat = true;
                return isRepeat;
            }

        if ((temp - col - 1) >= 0)  // if there is only 1 row ahead
        {
            for (int j = (temp - col + 2); j > (temp - col - 1); j--)  // 1 row ahead
                if (tileIndex.at((unsigned int)j) == i)
                {
                    isRepeat = true;
                    return isRepeat;
                }

            if ((temp - col * 2 - 1) >= 0)   // 2 rows ahead
            {
                for (int j = (temp - col * 2 + 2); j > (temp - col * 2 - 1); j--)
                    if (tileIndex.at((unsigned int)j) == i)
                    {
                        isRepeat = true;
                        return isRepeat;
                    }
            }
        }
    }
    return isRepeat;
}

Mat tileReplacement(int size, Mat mosaicImg, Mat targetImg, Mat tile, double overlayLevel, int pixelY, int pixelX, int breakpoint)
{
    Mat result = mosaicImg;
    int temp = 0, i = 0;
    
    for (int k = 0; k < size; k++)
    {
        result.at<Vec3b>(pixelY, pixelX + temp)
                = (1 - overlayLevel) * tile.at<Vec3b>(i, temp) + overlayLevel * targetImg.at<Vec3b>(pixelY, pixelX + temp);
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
