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

//void MainWindow::on_tileSelection_clicked()
//{
//    QStringList tilePaths = QFileDialog::getOpenFileNames(this, tr("Selecting Tile Images"),".", tr("Image File((*.png *.jpg *.jpeg *.bmp))"));
//    QLineEdit *pLineEdit = (QLineEdit *) ui -> dirEdit;
//    pLineEdit->set(tilePaths);
//}

void MainWindow::on_targetSelection_clicked()
{
    QString targetPath = QFileDialog::getOpenFileName(this, tr("Selecting Target Image"),".", tr("Image File((*.png *.jpg *.jpeg))"));

    // to check if the path is exist or if the file is readable
    if (access(targetPath.toStdString().c_str(), F_OK) == -1 || access(targetPath.toStdString().c_str(), R_OK) == -1)
    {
        QMessageBox::warning(this, tr("Warning"),tr("Fail to open the image"));
        return;
    }
    else
    {
        target = ImageCutter(targetPath.toStdString());

        cvtColor(target, target, COLOR_BGR2RGB);

        img = QImage((const unsigned char*) target.data, target.cols,
                             target.rows, target.cols * target.channels(),
                             QImage::Format_RGB888);


        // targetSelection is the button in the GUI
        img = img.scaled(ui -> displayTarget -> size(), Qt::KeepAspectRatio);
        ui -> displayTarget -> setPixmap(QPixmap::fromImage(img));
        ui -> displayTarget -> resize(ui -> displayTarget -> pixmap() -> size());
    }
}

