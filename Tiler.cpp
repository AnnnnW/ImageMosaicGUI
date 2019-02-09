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

int Tiler(Mat mosaicImg, string readpath, string defaultpath, string savepath)
{
    string path = readpath + "Resource/";
    string filePath = defaultpath + "data.csv";
    
    Mat targetImg = imread(readpath + "EdgeBorder/25*25Edgeborder.jpg");
    if (!targetImg.data)
    {
        printf("Can't read the file, please check the path and try again.\n");
        return -1;
    }
    
    vector<string> tiles;
    vector<Vec3b> averages;
    vector<int> hue;
    
    reader(filePath, tiles, averages, hue);
    
    Mat tempImg = mosaicImg, bestFitTile;
    
    int i = 0, j = 0, pixelX = 0, pixelY = 0, bestFitIndex;
    int height = tempImg.rows;
    int width = tempImg.cols;
    
    Vec3b averageRGB;
    
    // initialise the count of the use of tiles
    
    for (i = 0; i < width; i+=BREAK)
    {
        for (j = 0; j < height; j+=BREAK)
        {
            pixelX = i;
            pixelY = j;
            averageRGB = tempImg.at<Vec3b>(pixelY,pixelX);
            
            bestFitIndex = compareHue(averageRGB, hue);
            bestFitTile = findBestFitTile(readpath, bestFitIndex, tiles);
            if (!bestFitTile.data)
            {
                printf("Can't read the best fit tile.\n");
                return -1;
            }
            
            pixelX = i;     // the first pixel
            pixelY = j;
            
            tileReplacement(SIZE, tempImg, targetImg, bestFitTile, pixelY, pixelX, BREAK);
        } // for
    } // for
    
    imshow("Result", tempImg);
    imwrite(savepath + "result(25*25) with masking.jpg", tempImg);
    printf("The result picture has been created.\n");
    
    
    return 0;
} // Tiler

void reader(string filePath, vector<string> &tiles, vector<Vec3b> &averages, vector<int> &hue)
{
    ifstream data(filePath);
    
    if (!data)
    {
        printf("Can't open the file.");
        exit(EXIT_FAILURE);
    }
    string line;
    string word;
    Vec3b average;
    int count = -1;
    
    if (count == -1)// get rid of the first title line in data.csv
    {
        getline(data, line);
        count++;
    } // if
    while(getline(data, line))
    {
        stringstream parsing(line);
        while (getline(parsing, word, ','))
        {
            if (count == 0)
            {
                tiles.push_back(word);
                count++;
            } // if
            else if (count == 1)
            {
                average[0] = stoi(word);
                count++;
            } // else if
            else if (count == 2)
            {
                average[1] = stoi(word);
                count++;
            } // else if
            else if (count == 3)
            {
                average[2] = stoi(word);
                averages.push_back(average);
                count++;
            } // else if
            else if (count == 4)
            {
                hue.push_back(stoi(word));
                count = 0;
            } // else if
        } // while
    } // while
    
} // reader

int compareHue(Vec3b averageRGB, vector<int> hue)
{
    int averageHue = (int)hsvTrans(averageRGB)[0] * 2;
    
    int temp, currentMin = 360, bestFitIndex = 0;
    int best[3];
    
    for (int i = 0; i < hue.size(); i++)
    {
        temp = abs(hue[i] - averageHue);        // stored as an absolute value, need to be none-negative
        if (currentMin > temp)
        {
            currentMin = temp;
            bestFitIndex = i;
        } // if
    } // for
    
    return bestFitIndex;
} // compareHue

Mat findBestFitTile(string readPath, int bestFitIndex, vector<string> tiles)
{
    Mat tile = imread(readPath + "Resizer/" + tiles[bestFitIndex]);
    
    return tile;
}

void tileReplacement(int size, Mat mosaicImg, Mat targetImg, Mat tile, int pixelY, int pixelX, int breakpoint)
{
    int temp = 0, i = 0;
    
    for (int k = 0; k < size; k++)
    {
        mosaicImg.at<Vec3b>(pixelY, pixelX + temp) = (1 - transparency) * tile.at<Vec3b>(i, temp) +
                                                     transparency * targetImg.at<Vec3b>(pixelY, pixelX + temp);
        temp++;
        if (temp == breakpoint)
        {
            temp = 0;
            i++;
            pixelY++;
        } // if
    } // for
} // tileReplacement
