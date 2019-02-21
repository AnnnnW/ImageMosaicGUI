//
//  ImageCutter.cpp
//  Image_Mosaic
//
//  Created by WangAnnan on 08/10/2018.
//  Copyright © 2018 WangAnnan. All rights reserved.
//

#include "ImageCutter.hpp"

static vector<Mat> hsvSplit;

Mat ImageCutter(Mat targetImg)
{
    //mosaic target image
    Mat borderImg = targetImg;
    if (borderImg.cols % BREAK != 0 || borderImg.rows % BREAK != 0)
        borderImg = edgeBorder(borderImg);

    int height = borderImg.rows;
    int width = borderImg.cols;
    
    targetImg = mosaicFilter(borderImg, height, width);
        
    return targetImg;
} // ImageCutter

Mat edgeBorder(Mat targetImg)
{
    Mat tempImg = targetImg;
    
    int addH, addW;
    int height = tempImg.rows;
    int width = tempImg.cols;
    
    addH = BREAK - height % BREAK;          // the pixels need to be added in height to form a (2N*1)*(2N*1) block
    addW = BREAK - width % BREAK;          // pixels to be added in width
    
    copyMakeBorder(tempImg, tempImg, 0, addH, 0, addW, BORDER_REPLICATE); // copy the colour of the edge
    
    return tempImg;
} // EdgeBorder

Mat mosaicFilter(Mat targetImg, int height, int width)
{
    int i = 0, j = 0, pixelX = 0, pixelY = 0;
    Vec3b average;
    
    int mosaicArray[SIZE][RGB];
    
    for (i = 0; i < width; i+=BREAK)
    {
        for (j = 0; j < height; j+=BREAK)
        {
            pixelX = i;     // start from the first pixel for the 3 * 3 filter
            pixelY = j;
            
            readPixel(SIZE, mosaicArray, targetImg, pixelY, pixelX, BREAK);
            
            average = averageValue(SIZE, mosaicArray);
            
            pixelX = i;     // the first pixel for the 3 * 3 filter
            pixelY = j;
            
            writePixel(SIZE, average, targetImg, pixelY, pixelX, BREAK);
        } // for
    } // for
    
    return targetImg;
} // mosaicFilter

void readPixel(int size, int array[size][RGB], Mat targetImg, int pixelY, int pixelX, int breakpoint)
{
    int temp = 0;
    for (int k = 0; k < size; k++)
    {
        array[k][0] = (int)targetImg.at<Vec3b>(pixelY, pixelX + temp).val[0];       // B
        array[k][1] = (int)targetImg.at<Vec3b>(pixelY, pixelX + temp).val[1];       // G
        array[k][2] = (int)targetImg.at<Vec3b>(pixelY, pixelX + temp).val[2];       // R
        temp++;
        if (temp == breakpoint)      // if the first 3 in a row has added to the array, then jump to the next row
        {
            temp = 0;
            pixelY++;
        } // if
    } // for
} // readPixel

void writePixel(int size, Vec3b average, Mat targetImg, int pixelY, int pixelX, int breakpoint)
{
    int temp = 0;
    for (int k = 0; k < size; k++)
    {
        targetImg.at<Vec3b>(pixelY, pixelX + temp) = average;
        temp++;
        if (temp == breakpoint)      // if the first 3 in a row has added to the array, then jump to the next row
        {
            temp = 0;
            pixelY++;
        } // if
    } // for
} // writePixel

// avarage value of the 9 pixels
Vec3b averageValue(int size, int array[size][RGB])
{
    int bSum = 0;
    int gSum = 0;
    int rSum = 0;
    
    for (int i = 0; i < size; i++)
    {
        bSum = bSum + array[i][0];
        gSum = gSum + array[i][1];
        rSum = rSum + array[i][2];
    }
    
    bSum = (int)bSum / size;
    gSum = (int)gSum / size;
    rSum = (int)rSum / size;
    
    Vec3b average = {(unsigned char)bSum, (unsigned char)gSum, (unsigned char)rSum};
    
    return average;
} // averageValue
