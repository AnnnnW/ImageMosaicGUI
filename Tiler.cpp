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
        {
            if (tileIndex.at((unsigned int)j) == i)
            {
                isRepeat = true;
                break;
            }
        }

        if ((temp - col - 1) >= 0)  // if there is only 1 row ahead
        {
            for (int j = (temp - col + 2); j > (temp - col - 1); j--)  // 1 row ahead
                if (tileIndex.at((unsigned int)j) == i)
                {
                    isRepeat = true;
                    break;
                }

            if ((temp - col * 2 - 1) >= 0)   // 2 rows ahead
            {
                for (int j = (temp - col * 2 + 2); j > (temp - col * 2 - 1); j--)
                    if (tileIndex.at((unsigned int)j) == i)
                    {
                        isRepeat = true;
                        break;
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
    for (int i = 0; i < int(sizeof(tileUnequal)/sizeof(*tileUnequal)); i++) // initialise the array of the
        tileUnequal[i] = tileHeight;

    int currentHeight = tileHeight; // current maximum block height
    int currentWidth = tileWidth;   // current maximum block width
    int currentSize;   // current maximum block size
    int dif;    // the quotient of the currentHeight and the minimum height of the block
    Mat tile;   // the tile repeatly used in the blocks

    for (int k = 1; k < 4; k++)
    {
        dif = currentHeight / tileHeight;
        currentHeight *= 2;
        currentWidth *= 2;
        currentSize = currentHeight * currentWidth;

        for (int i = 0; i < (row - dif); i++)    // i*col  is the number of block in the previous columns
        {
            for (int j = 0; j < (col - dif); j++)
            {
                // to check if there are four blocks as a square shape use the same tile
                if (isSameScale(tileUnequal, currentHeight, (i*col + j), (currentHeight/tileHeight)))
                {
                    if (isSameTile(tileIndex, (i*col + j), (currentHeight/tileHeight)))
                    {
                        updateBlockSize(tileUnequal, currentHeight, (i*col + j), (currentHeight/tileHeight));

                        tile = resizer(tiles.at(tileIndex.at((unsigned int)i*col + j)), currentHeight, currentWidth);
                        temp = tileReplacement(currentSize, temp, targetImg, tile, overlayLevel, j*tileHeight, i*tileWidth, currentHeight);
                    }
                }
            }
        }
    }
    return temp;
}

void updateBlockSize(int tileUnequal[], int currentLength, int startInd, int scale)
{
    for (int s = 0; s < scale; s++)   // each column of the new and larger block
        for(int t = 0; t < scale; t++)    // each row of the block
            tileUnequal[startInd + 1*t + col*s] = currentLength;
}

bool isSameScale(int tileUnequal[], int currentLength, int startInd, int scale)
{
    bool isSame = true;
    for (int s = 0; s < scale; s++)   // each column of the new and larger block
    {
        for(int t = 0; t < scale; t++)    // each row of the block
        {
            if (tileUnequal[startInd + 1*t + col*s] != int(currentLength / 2))
            {
                isSame = false;
                break;
            }
        }
        if  (!isSame) break;
    }
    return isSame;
}

bool isSameTile(vector<int> tileIndex, int startInd, int scale)
{
    bool isSame = true;
    int previousInd = tileIndex.at((unsigned int) startInd);
    for (int s = 0; s < scale; s++)   // each column of the new and larger block
    {
        for(int t = 0; t < scale; t++)    // each row of the block
        {
            if (tileIndex.at((unsigned int)(startInd + 1*t + col*s)) != previousInd)
            {
                isSame = false;
                break;
            }
        }
        if (!isSame) break;
    }
    return isSame;
}
