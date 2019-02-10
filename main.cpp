//
//  main.cpp
//  Image_Mosaic
//
//  Created by WangAnnan on 2018/9/7.
//  Copyright © 2018年 WangAnnan. All rights reserved.
//

// only work when it's on the top :))
#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include <string>
#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "ImageCutter.hpp"
#include "TileAnalyser.hpp"
#include "Tiler.hpp"

using namespace cv;                             //包含cv命名空间
using namespace std;

static string defaultpath = "/Users/wangannan/Image_Mosaic/IMG/";
static string readpath = defaultpath + "Input/";
static string savepath = defaultpath + "Output/";

int main(int argc, char *argv[])
{
//    Mat targetImage = imread(readpath + "testing_image.jpg");             // Load the image
//    //    Mat targetImage = imread(readpath + "C3FDFBFA-AFE0-4F06-8093-8C8C005D6B75-1277-000000D003A7E724_tmp.jpg");             // Load the image

//    if (!targetImage.data)
//    {
//        printf("Can't read the file, please check the path and try again.\n");
//        return -1;
//    }

//    ImageCutter(targetImage);

//    //read the image name and output a file
//    TileAnalyser(readpath, defaultpath);

//    Mat mosaicImage = imread(readpath + "Cutter/25*25Cutter(masking).jpg");             // Load the image
//    if (!mosaicImage.data)
//    {
//        printf("Can't read the mosaic image, please check the path and try again.\n");
//        return -1;
//    }

//    Tiler(mosaicImage, readpath, defaultpath, savepath);


//    if(waitKey(0))                                 // Wait for any key to be pressed to exit
//    {
//        printf("The program is successfully exited by keyboard interruption :) \n");
//    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}




