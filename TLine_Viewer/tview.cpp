#include "tview.h"
#include "ui_tview.h"

#include <iostream>

TView::TView(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::TView){

    ui->setupUi(this);

    zLine = ui->zLine;
    tLine = ui->tLine;

    this->setStyleSheet("background-color: white;");

    QSize iconsSize(46, 46);

    ui->zLine->setText("0");
    ui->tLine->setText("0");
    zFix = 0;
    tFix = 0;

    ui->dT->setText("0.001");
    dt = 0.001;
    ui->nT->setText("10");
    nt = 10;
    ui->dZ->setText("0.01");
    dz = 0.01;
    ui->nZ->setText("100");
    nz = 100;

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

    //ui->firstV->setChecked(true);
    vol = CONTINUA;
    //ui->secondR->setChecked(true);
    res = ZERO;

    //datas = allocMemory(vol, res, dt, nt, dz, nz);
    //graphs = new Graph(*datas, ui->zGraphic->width(), ui->zGraphic->height(), nt, nz);

    //updateTGraphic();
    //updateZGraphic();

    this->setWindowTitle("Transmission Lines");

}

TView::~TView() {
    delete ui;
}

void TView::updateTGraphic(){
    cv::Mat img = graphs->TFixed_Graph(tFix);
    ui->tGraphic->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));
}

void TView::updateZGraphic(){
    cv::Mat img = graphs->ZFixed_Graph(zFix);
    ui->zGraphic->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));
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

        if(fixZ != zFix){
            zFix = fixZ;
            updateZGraphic();
        }
    }catch(std::invalid_argument e){
    }catch(std::out_of_range e2){
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

        if(fixT != tFix){
            tFix = fixT;
            updateTGraphic();
        }
    }catch(std::invalid_argument e){
    }catch(std::out_of_range e2){
    }
}

void TView::on_firstV_clicked(){
    if(ui->firstV->isChecked() && vol != CONTINUA){
        vol = CONTINUA;
        datas = calculateAllValues(datas, vol, res, dt, nt, dz, nz);
        graphs->updateParameters(*datas);
        updateTGraphic();
        updateZGraphic();
    }
}

void TView::on_secondV_clicked(){
    if(ui->secondV->isChecked() && vol != DEGRAU){
        vol = DEGRAU;
        datas = calculateAllValues(datas, vol, res, dt, nt, dz, nz);
        graphs->updateParameters(*datas);
        updateTGraphic();
        updateZGraphic();
    }
}

void TView::on_firstR_clicked(){
    if(ui->firstR->isChecked() && res != INFINITA){
       res = INFINITA;
       datas = calculateAllValues(datas, vol, res, dt, nt, dz, nz);
       graphs->updateParameters(*datas);
       updateTGraphic();
       updateZGraphic();
    }
}

void TView::on_secondR_clicked(){
    if(ui->secondR->isChecked() && res != ZERO){
       res = ZERO;
       datas = calculateAllValues(datas, vol, res, dt, nt, dz, nz);
       graphs->updateParameters(*datas);
       updateTGraphic();
       updateZGraphic();
    }
}

void TView::on_thirdR_clicked(){
    if(ui->thirdR->isChecked() && res != CEM){
       res = CEM;
       datas = calculateAllValues(datas, vol, res, dt, nt, dz, nz);
       graphs->updateParameters(*datas);
       updateTGraphic();
       updateZGraphic();
    }
}

void TView::on_dT_textChanged(const QString &arg1){
    std::string edit = arg1.toStdString();

    if(arg1.contains('.')){
        int index = arg1.indexOf('.');
        edit.replace(uint32_t(index), 1, ",");
    }

    float dT;
    try{
        dT = std::stof(edit);

        if(dT != dt){
            dt = dT;
            datas = calculateAllValues(datas, vol, res, dt, nt, dz, nz);
            graphs->updateParameters(*datas);
            updateTGraphic();
            updateZGraphic();
        }

    }catch(std::invalid_argument e){

    }catch(std::out_of_range e2){

    }
}

void TView::on_nT_textChanged(const QString &arg1){
    std::string edit = arg1.toStdString();

    if(arg1.contains('.')){
        int index = arg1.indexOf('.');
        edit.replace(uint32_t(index), 1, ",");
    }

    float nT;
    try{
        nT = std::stof(edit);

        if(nT != nt){
            nt = nT;
            datas = calculateAllValues(datas, vol, res, dt, nt, dz, nz);
            graphs->updateParameters(*datas);
            updateTGraphic();
            updateZGraphic();
        }
    }catch(std::invalid_argument e){

    }catch(std::out_of_range e2){

    }
}

void TView::on_dZ_textChanged(const QString &arg1){
    std::string edit = arg1.toStdString();

    if(arg1.contains('.')){
        int index = arg1.indexOf('.');
        edit.replace(uint32_t(index), 1, ",");
    }

    float dZ;
    try{
        dZ = std::stof(edit);

        if(dZ != dz){
            dz = dZ;
            datas = calculateAllValues(datas, vol, res, dt, nt, dz, nz);
            graphs->updateParameters(*datas);
            updateTGraphic();
            updateZGraphic();
        }
    }catch(std::invalid_argument e){

    }catch(std::out_of_range e2){

    }

}

void TView::on_nZ_textChanged(const QString &arg1){
    std::string edit = arg1.toStdString();

    if(arg1.contains('.')){
        int index = arg1.indexOf('.');
        edit.replace(uint32_t(index), 1, ",");
    }

    float nZ;
    try{
        nZ = std::stof(edit);
        if(nZ != nz){
            nz = nZ;
            datas = calculateAllValues(datas, vol, res, dt, nt, dz, nz);
            graphs->updateParameters(*datas);
            updateTGraphic();
            updateZGraphic();
        }

    }catch(std::invalid_argument e){

    }catch(std::out_of_range e2){

    }
}
