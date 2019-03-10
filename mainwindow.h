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
#include <QSlider>

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
    void on_cleanTilePathButton_clicked();

private:
    Ui::MainWindow *ui;
    Mat target;
    Mat borderTarget;   // to save the edge border target image
    QGraphicsScene *targetScene = new QGraphicsScene;
    Mat mosaicTarget;
    QImage targetImg;
    QStringList displayed; // to check if the path has already been display, avoid repeated display
    vector<Mat> tiles;
    vector<Mat> resizedTiles;

    vector<int> tileSIZE = {16, 32, 64, 128};
    int tileHeight = 64;
    int tileWidth = 64;
    bool noRepetition; // to check the status whether user need the tile repetition
    double overlayLevel = 0.6;

    // the global variables for tiles
    vector<Vec3b> averages;
    vector<int> hue;

    // the tiles has been used
    vector<int> tileIndex;

    Mat result;
    QImage resultImg;
    QGraphicsScene *resultScene = new QGraphicsScene;
};


#endif // MAINWINDOW_H
