#include "tview.h"
#include "ui_tview.h"

#include <iostream>

#define MAX 101

TView::TView(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::TView){

    ui->setupUi(this);

    zLine = ui->zLine;
    tLine = ui->tLine;

    ui->Voltages->setStyleSheet("background-color:white;");
    ui->Resistances->setStyleSheet("background-color:white;");

    QSize iconsSize(46, 46);

    ui->zLine->setText("0");
    ui->tLine->setText("0");

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

TView::~TView() {
    delete ui;
}

void TView::on_zLine_textChanged(const QString &arg1){
    std::string edit = arg1.toStdString();

    if(arg1.contains('.')){
        int index = arg1.indexOf('.');
        edit.replace(uint32_t(index), 1, ",");
    }

    float fixZ;
    try{
        fixZ = std::stof(edit);
        std::cout << "fix Z: " << fixZ << std::endl;
    }catch(std::invalid_argument e){
        zLine->setText("0");
    }catch(std::out_of_range e2){
        zLine->setText("0");
    }
}

void TView::on_tLine_textChanged(const QString &arg1){
    std::string edit = arg1.toStdString();

    if(arg1.contains('.')){
        int index = arg1.indexOf('.');
        edit.replace(uint32_t(index), 1, ",");
    }

    float fixT;
    try{
        fixT = std::stof(edit);
        std::cout << "fix T: " << fixT << std::endl;
    }catch(std::invalid_argument e){
        tLine->setText("0");
    }catch(std::out_of_range e2){
        tLine->setText("0");
    }

}



void TView::on_firstV_clicked(){
    if(ui->firstV->isChecked()){
        zLine->setText("500");
    }
}

void TView::on_secondV_clicked(){
    if(ui->secondV->isChecked()){
        zLine->setText("200");
    }
}

void TView::on_firstR_clicked(){
    if(ui->firstR->isChecked()){
        tLine->setText("1000");
    }
}

void TView::on_secondR_clicked(){
    if(ui->secondR->isChecked()){
        tLine->setText("500");
    }
}

void TView::on_thirdR_clicked(){
    if(ui->thirdR->isChecked()){
        tLine->setText("200");
    }
}
