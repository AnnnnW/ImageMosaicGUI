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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}




