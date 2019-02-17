#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_targetSelection_clicked()
{
    QLineEdit *inputPath = ui -> targetPath;
    inputPath->setReadOnly(true);

    QString targetPath = QFileDialog::getOpenFileName(this, tr("Selecting Target Image"),".", tr("JPEG (*.jpg *.jpeg);;PNG (*.png)"));

    inputPath->setText(targetPath);

    // if there no image selected
    if (targetPath.isNull())
    {
        QMessageBox::warning(this, tr("Warning"),tr("Please choose an image"));
        return;
    }
    target = imread(targetPath.toStdString());
    // to check if the path is exist or if the file is readable
    if (!target.data)
    {
        QMessageBox::warning(this, tr("Warning"),tr("Cannot open the image"));
        return;
    }

    cvtColor(target, target, COLOR_BGR2RGB);

    targetImg = QImage((const unsigned char*) target.data, target.cols,target.rows, target.cols * target.channels(), QImage::Format_RGB888);
    // restrict the size of the image to fit the graphicsview
    targetImg = targetImg.scaled(ui->displayTarget->size(), Qt::KeepAspectRatio);
    QGraphicsScene *scene = new QGraphicsScene;
    // targetSelection is the button in the GUI
    scene->addPixmap(QPixmap::fromImage(targetImg));
    ui->displayTarget -> setScene(scene);
    ui->displayTarget -> resize(ui->displayTarget->size());
    ui->displayTarget->show();

    target = ImageCutter(target);
}


void MainWindow::on_tilesSelection_clicked()
{
    QTextEdit* inputPath = ui -> tilePaths;
    inputPath->setReadOnly(true);

    QStringList tilePaths = QFileDialog::getOpenFileNames(this, tr("Selecting Tile Images"),".", tr("Image File((*.png *.jpg *.jpeg *.bmp))"));
    for (int i = 0; i < tilePaths.size(); i++)
        inputPath->append(tilePaths.at(i));

    if (tilePaths.empty())
    {
        QMessageBox::warning(this, tr("Warning"),tr("Please choose an image"));
        return;
    }
    vector<string> paths;

    vector<Mat> tiles;
    foreach (QString path, tilePaths)
    {
        if (access(path.toStdString().c_str(), F_OK) == -1 || access(path.toStdString().c_str(), R_OK) == -1)
        {
            QMessageBox::warning(this, tr("Warning"),tr("Cannot open the image"));
            return;
        }
        tiles.push_back(imread(path.toStdString()));
        TileAnalyser(tiles, averages, hue, rgbArray);
    }
}



void MainWindow::on_saveButton_clicked()
{

}
