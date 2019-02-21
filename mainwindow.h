#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QImage>
#include <QFileDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>

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
    void on_runButton_clicked();
    void on_saveButton_clicked();

    void on_cleanButton_clicked();

private:
    Ui::MainWindow *ui;
    Mat target;
    QGraphicsScene *targetScene = new QGraphicsScene;
    Mat mosaicTarget;
    QImage targetImg;
    vector<Mat> tiles;
    vector<Mat> resizedTiles;

//    int SIZE;
//    int RGB = 3;

    // the global variables for tiles
    vector<Vec3b> averages;
    vector<int> hue;

    Mat result;
    QImage resultImg;
    QGraphicsScene *resultScene = new QGraphicsScene;
};


#endif // MAINWINDOW_H
