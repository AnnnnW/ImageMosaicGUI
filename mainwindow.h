#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QImage>
#include <QRadioButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "ImageCutter.hpp"
#include "TileAnalyser.hpp"
#include "Tiler.hpp"

namespace Ui {
class MainWindow;
}
using namespace cv;
using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_targetSelection_clicked();
    void on_tilesSelection_clicked();

    void on_saveButton_clicked();

private:
    Ui::MainWindow *ui;
    Mat target;
    QImage targetImg;
    vector<Mat> tiles;

    // the global variables for tiles
    vector<Vec3b> averages;
    vector<int> hue;
    int rgbArray[SIZE][RGB];

};


#endif // MAINWINDOW_H
