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

    // normal image is BRG, the QImage is RGB
    Mat temp;
    cvtColor(target, temp, COLOR_BGR2RGB);
    targetImg = QImage((const unsigned char*) temp.data, temp.cols,temp.rows, temp.cols * temp.channels(), QImage::Format_RGB888);
    // restrict the size of the image to fit the graphicsview
    targetImg = targetImg.scaled(ui->displayTarget->size(), Qt::KeepAspectRatio);
    // targetSelection is the button in the GUI
    targetScene->clear();
    targetScene->addPixmap(QPixmap::fromImage(targetImg));
    ui->displayTarget -> setScene(targetScene);
    ui->displayTarget -> resize(ui->displayTarget->size());
    ui->displayTarget->show();

//    mosaicTarget = ImageCutter(target);
}

void MainWindow::on_tilesSelection_clicked()
{
    QTextEdit* inputPath = ui -> tilePaths;
    inputPath->setReadOnly(true);

    QStringList tilePaths = QFileDialog::getOpenFileNames(this, tr("Selecting Tile Images"),".", tr("JPEG (*.jpg *.jpeg);;PNG (*.png)"));
    vector<String> paths;
    for (int i = 0; i < tilePaths.size(); i++)
    {
        if (!displayed.contains(tilePaths.at((i))))
        {
            inputPath->append(tilePaths.at(i));
            displayed.append(tilePaths.at(i));
        }
        if (access(tilePaths.at(i).toStdString().c_str(), F_OK) == -1 || access(tilePaths.at(i).toStdString().c_str(), R_OK) == -1)
        {
            QMessageBox::warning(this, tr("Warning"),tr("Cannot open the image"));
            return;
        }

        // check if the path has already been added
        paths.push_back(tilePaths.at(i).toStdString());
    }
    inputPath->append("You have choose " + QString::number(displayed.length()) + " tile(s).");

    for (int i = 0; i < int(paths.size()); i++)
    {
        Mat temp = imread(paths.at((unsigned int)i));
        tiles.push_back(temp);
    }
}

void MainWindow::on_runButton_clicked()
{
    if (targetImg.isNull())
    {
        QMessageBox::warning(this, tr("Warning"),tr("Please choose an image"));
        return;
    }

    if (tiles.empty())
    {
        QMessageBox::warning(this, tr("Warning"),tr("Please choose tile images"));
        return;
    }

    if ((target.rows / tileHeight < 2) || (target.cols / tileWidth < 2))
    {
        QMessageBox::warning(this, tr("Warning"),tr("The tile size is too big to fit into the target, please change to a smaller one"));
    }

    // check the tile size from the gui
    if (ui->tileSizeSelection->currentIndex() <= int(tileSIZE.size()))
    {
        tileHeight = tileSIZE.at((unsigned int)ui->tileSizeSelection->currentIndex());
        tileWidth = tileHeight;
    }

    borderTarget = edgeBorder(target, tileHeight, tileWidth);
    mosaicTarget.release();
    mosaicTarget = ImageCutter(borderTarget, tileHeight, tileWidth);

    resizedTiles.clear();
    averages.clear();
    hue.clear();
    TileAnalyser(tiles, tileHeight, tileWidth, resizedTiles, averages, hue);

    int sliderValue = ui->overlaySlider->value();
    if (sliderValue == 100)
        overlayLevel = 0.99;
    else
        overlayLevel = (double)sliderValue / 100;

    // check the gui whether user click to choose no tile repetition
    noRepetition = ui->checkTileRepetition->isChecked();
    // if the number of tiles is really small, ask the user whether to continue

    if (tiles.size() < 2)
    {
        QMessageBox::warning(this, tr("Warning"),tr("There should be more tiles for a mosaic."));
        return;
    }

    if (noRepetition)
    {
        if (tiles.size() < 6)
        {
            // double check whether the user really want to clean up everything
            QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Warning"),tr("The more the tiles, the better the repetition reduction work.\n Do you want to keep going?"));
            if (reply == QMessageBox::No)
            {
                ui->checkTileRepetition->setChecked(false);
                noRepetition = false;
            }
        }
    }

    result.release();
    tileIndex.clear();
    result = Tiler(mosaicTarget, borderTarget, resizedTiles, tileHeight, tileWidth, hue, tileIndex, noRepetition, overlayLevel);

    Mat temp;
    cvtColor(result, temp, COLOR_BGR2RGB);
    resultImg = QImage((const unsigned char*) temp.data, temp.cols,temp.rows, temp.cols * temp.channels(), QImage::Format_RGB888);
    // restrict the size of the image to fit the graphicsview
    resultImg = resultImg.scaled(ui->displayResult->size(), Qt::KeepAspectRatio);
    // targetSelection is the button in the GUI
    resultScene->clear();
    resultScene->addPixmap(QPixmap::fromImage(resultImg));
    ui->displayResult -> setScene(resultScene);
    ui->displayResult -> resize(ui->displayResult->size());
    ui->displayResult->show();
}

void MainWindow::on_saveButton_clicked()
{
    if (resultImg.isNull())
    {
        QMessageBox::warning(this, tr("Warning"),tr("There's no image to save."));
        return;
    }
    QString savePath = QFileDialog::getSaveFileName(this, tr("Savw File"), "", tr("JPEG (*.jpg *.jpeg);;PNG (*.png)"));
    Mat temp;
    cvtColor(result, temp, COLOR_BGR2RGB);
    QImage resultRGB = QImage((const unsigned char*) temp.data, temp.cols, temp.rows, temp.cols * temp.channels(), QImage::Format_RGB888);
    if (resultRGB.save(savePath))
    {
        QMessageBox::information(this, tr("Saved!"),tr("You have successfully saved the image."));
        return;
    }
    else
    {
        QMessageBox::warning(this, tr("Warning!"),tr("Can not save the image."));
        return;
    }
}

void MainWindow::on_cleanButton_clicked()
{
    // double check whether the user really want to clean up everything
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Warning"),tr("Do you really want to reset everything?"));
    if (reply == QMessageBox::Yes)
    {
        target.release();
        targetImg = QImage();
        mosaicTarget.release();
        result.release();
        resultImg = QImage();
        averages.clear();
        hue.clear();
        tileIndex.clear();
        tiles.clear();
        resizedTiles.clear();
        displayed.clear();
        ui->targetPath->clear();
        ui->tilePaths->clear();
        targetScene->clear();
        resultScene->clear();
        ui->overlaySlider->setValue(60);
        ui->checkTileRepetition->setChecked(true);
        ui->tileSizeSelection->setCurrentIndex(2);
    }
}

void MainWindow::on_cleanTilePathButton_clicked()
{
    // double check whether the user really want to clean up everything
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Warning"),tr("Do you really want to reset the tiles?"));
    if (reply == QMessageBox::Yes)
    {
        averages.clear();
        hue.clear();
        tileIndex.clear();
        resizedTiles.clear();
        tiles.clear();
        displayed.clear();
        ui->tilePaths->clear();
    }
}
