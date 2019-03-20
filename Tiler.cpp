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

Mat Tiler(Mat mosaicTarget, Mat targetImg, vector<Mat> tiles, vector<Mat> resizedTiles, int tileHeight, int tileWidth, vector<int> hue, vector<int> &tileIndex, bool noRepetition, bool isUnequal, double overlayLevel)
{   
    Mat bestFitTile, result;
    mosaicTarget.copyTo(result);
    
    int i = 0, j = 0, pixelX = 0, pixelY = 0;
    int height = result.rows;
    int width = result.cols;
    Vec3b averageRGB;
    int size = tileHeight * tileWidth;

    row = width / tileWidth;
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

    if (!noRepetition && isUnequal)
    {
        result = unequalSize(result, targetImg, tiles, tileIndex, col, row, tileWidth, tileHeight, overlayLevel);
    }

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
    Mat result;
    mosaicImg.copyTo(result);
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

Mat unequalSize(Mat result, Mat targetImg, vector<Mat> tiles, vector<int> tileIndex, int col, int row, int tileWidth, int tileHeight, double overlayLevel)
{
    Mat temp;
    result.copyTo(temp);
    int tileUnequal[int(tileIndex.size())];
    int currentHeight = tileHeight * 2; // current maximum block height
    int currentWidth = tileWidth * 2;   // current maximum block width
    int currentSize = currentHeight * currentWidth;    // current maximum block size
    Mat tile;   // the tile repeatly used in the blocks

    for (int i = 0; i < (row - 1); i++)    // i*col is the number of block in the previous columns
    {
        for (int j = 0; j < (col - 1); j++)
        {
            // to check if there are four blocks as a square shape use the same tile
            if (tileUnequal[i*col+j] != 1 && tileUnequal[i*col+j+1] != 1 && tileUnequal[i*col+j+col] != 1 &&  tileUnequal[i*col+j+col+1] != 1)
            {
                if (tileIndex.at((unsigned int)(i*col+j)) == tileIndex.at((unsigned int)(i*col+j+1)) && tileIndex.at((unsigned int)(i*col+j+col)) == tileIndex.at((unsigned int)(i*col+j+col+1)) && tileIndex.at((unsigned int)(i*col+j)) == tileIndex.at((unsigned int)(i*col+j+col)))
                {
                    tileUnequal[i*col+j] = tileUnequal[i*col+j+1] = tileUnequal[i*col+j+col] = tileUnequal[i*col+j+col+1] = 1;
                    tile = resizer(tiles.at(tileIndex.at((unsigned int)i*col+j)), currentHeight, currentWidth);
                    temp = tileReplacement(currentSize, temp, targetImg, tile, overlayLevel, j * tileHeight, i * tileWidth, currentHeight);
                }
            }
        }
    }
    return temp;
}
