#include "tview.h"
#include "ui_tview.h"

#include <iostream>

#define MAXMEMORY 2000000000

TView::TView(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::TView){

    ui->setupUi(this);

    zLine = ui->zLine;
    tLine = ui->tLine;

    this->setStyleSheet("background-color: white;");

    QSize iconsSize(46, 46);

    vol = CONTINUA;
    res = ZERO;
    zFix = 0;
    tFix = 0;
    dt = 0.001;
    nt = 1;
    dz = 0.01;
    nz = 100;
    dtPrev = dt;
    ntPrev = nt;
    dzPrev = dz;
    nzPrev = nz;

    datas = NULL;
    datas = allocMemory(vol, res, dt, nt, dz, nz);
    graphs = new Graph(datas, ui->zGraphic->width(), ui->zGraphic->height(), nt, nz);

    ui->zLine->setText("0");
    ui->tLine->setText("0");

    ui->dT->setText("0.001");
    ui->nT->setText("1");
    ui->dZ->setText("0.01");
    ui->nZ->setText("100");

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

    ui->firstV->setChecked(true);
    ui->secondR->setChecked(true);


    updateTGraphic();
    updateZGraphic();

    this->setWindowTitle("Transmission Lines");
    changed = false;

}

TView::~TView() {
    delete ui;
}

bool TView::parametersValid(){
    int n = (int)(nt/dt) * (int)(nz/dz);

    if((long int)8 * n > MAXMEMORY){
        return false;
    }

    return true;
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

void TView::on_BtRecalcular_clicked(){
    if(changed){
        if(parametersValid()){
            changed = false;
            freeMemory(datas);
            datas = allocMemory(vol, res, dt, nt, dz, nz);
            graphs->updateParameters(datas);
            updateTGraphic();
            updateZGraphic();
            dtPrev = dt;
            ntPrev = nt;
            dzPrev = dz;
            nzPrev = nz;
            QMessageBox::information(this, tr("CONFIRMATION"), tr("Tudo enviado e atualizado"));
        }else{
            QMessageBox::warning(this, tr("ERROR BOX"), tr("Parametros ultrapassam o limite da memória"));
            dt = dtPrev;
            nt = ntPrev;
            dz = dzPrev;
            nz = nzPrev;
            ui->dT->setText(QString::number(dt));
            ui->nT->setText(QString::number(nt));
            ui->dZ->setText(QString::number(dz));
            ui->nZ->setText(QString::number(nz));
            changed = false;
        }
    }
}

void TView::on_firstV_clicked(){
    if(ui->firstV->isChecked() && vol != CONTINUA){
        vol = CONTINUA;
        changed = true;
    }
}

void TView::on_secondV_clicked(){
    if(ui->secondV->isChecked() && vol != DEGRAU){
        vol = DEGRAU;
        changed = true;
    }
}

void TView::on_firstR_clicked(){
    if(ui->firstR->isChecked() && res != INFINITA){
       res = INFINITA;
        changed = true;
    }
}

void TView::on_secondR_clicked(){
    if(ui->secondR->isChecked() && res != ZERO){
        res = ZERO;
        changed = true;
    }
}

void TView::on_thirdR_clicked(){
    if(ui->thirdR->isChecked() && res != CEM){
        res = CEM;
        changed = true;
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

        if(dT != dt && dT != 0){
            dt = dT;
            changed = true;
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

        if(nT != nt && nT != 0){
            nt = nT;
            changed = true;
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

        if(dZ != dz && dZ != 0){
            dz = dZ;
            changed = true;
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
        if(nZ != nz && nZ != 0){
            nz = nZ;
            changed = true;
        }

    }catch(std::invalid_argument e){

    }catch(std::out_of_range e2){

    }
}


