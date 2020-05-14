#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#define MAX 101

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow){

    ui->setupUi(this);

    zBox = ui->zBox;
    tBox = ui->tBox;

    for(int i = 0; i < MAX; i++){
        zBox->addItem(QString::number(i));
        tBox->addItem(QString::number(i));
    }

    QSize iconsSize(46, 46);

    ui->firstV->setIcon(QIcon(":/icons/firstV.png"));
    ui->secondV->setIcon(QIcon(":/icons/secondV.png"));
    ui->firstR->setIcon(QIcon(":/icons/firstR.png"));
    ui->secondR->setIcon(QIcon(":/icons/secondR.png"));
    ui->thirdR->setIcon(QIcon(":/icons/thirdR.png"));

    ui->firstV->setIconSize(iconsSize);
    ui->secondV->setIconSize(QSize(120, 180));
    ui->firstR->setIconSize(iconsSize);
    ui->secondR->setIconSize(iconsSize);
    ui->thirdR->setIconSize(iconsSize);

    this->setWindowTitle("Transmission Lines");

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_zBox_currentIndexChanged(int index){
    std::cout << index << std::endl;
}

void MainWindow::on_tBox_currentIndexChanged(int index){
    std::cout << index << std::endl;
}
