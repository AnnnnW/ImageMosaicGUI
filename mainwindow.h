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
#include <QPushButton>
#include <QPixmap>

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
//    void on_tileSelection_clicked();
//    void on_imageShow_clicked();

    void on_targetSelection_clicked();

private:
    Ui::MainWindow *ui;
    Mat target;
    QImage img;
};


#endif // MAINWINDOW_H
