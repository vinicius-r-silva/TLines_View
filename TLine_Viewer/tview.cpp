#include "tview.h"
#include "ui_tview.h"

#include <iostream>

#define MAXMEMORY 2000000000
#define MAXDELAY 1

TView::TView(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::TView){

    ui->setupUi(this);

    this->setStyleSheet("background-color: white;");

    QSize iconsSize(46, 46);

    vol = CONTINUA;
    res = CEM;

    zFix = 0;
    tFix = 0;

    std::cout << "DEBUG 1\n";

    ui->zLine->setText(QString::number(zFix));
    ui->tLine->setText(QString::number(tFix));

    ui->dT->setText(QString::number(0.00000000037));
    ui->nT->setText(QString::number(0.000003));
    ui->dZ->setText(QString::number(0.1));
    ui->nZ->setText(QString::number(100));

    dtPrev = dt;
    ntPrev = nt;
    dzPrev = dz;
    nzPrev = nz;

    std::cout << "DEBUG 2\n";

    datas = NULL;
    datas = allocMemory(vol, res, dt, nt, dz, nz);
    std::cout << "DEBUG 2.25\n";
    graphs = new Graph(datas, ui->zGraphic->width(), ui->zGraphic->height(), nt, nz, dt, dz);

    std::cout << "DEBUG 2.5\n";

    ui->firstV->setIcon(QIcon(":/icons/firstV.png"));
    ui->secondV->setIcon(QIcon(":/icons/secondV.png"));
    ui->firstR->setIcon(QIcon(":/icons/firstR.png"));
    ui->secondR->setIcon(QIcon(":/icons/secondR.png"));
    ui->thirdR->setIcon(QIcon(":/icons/thirdR.png"));

    ui->BtPauseT->setIcon(QIcon(":/icons/small_pause.png"));
    ui->BtPauseZ->setIcon(QIcon(":/icons/small_pause.png"));
    ui->BtPlayT->setIcon(QIcon(":/icons/small_play.png"));
    ui->BtPlayZ->setIcon(QIcon(":/icons/small_play.png"));
    ui->BtStopT->setIcon(QIcon(":/icons/small_stop.png"));
    ui->BtStopZ->setIcon(QIcon(":/icons/small_stop.png"));

    std::cout << "DEBUG 3\n";

    ui->SlAnimationT->setValue(100);
    ui->SlAnimationZ->setValue(100);

    animZ = new Animation(graphs, ui->tGraphic, 0.0, nt, 5, (double)(ui->SlAnimationZ->value() * MAXDELAY * 10));
    animT = new Animation(graphs, ui->zGraphic, 0.0, nz, 5, (double)(ui->SlAnimationT->value() * MAXDELAY * 10));

    QObject::connect(animZ, SIGNAL(Tfinished()), this, SLOT(animationZFinished()));
    QObject::connect(animT, SIGNAL(Tfinished()), this, SLOT(animationTFinished()));

    ui->firstV->setIconSize(iconsSize);
    ui->secondV->setIconSize(QSize(120, 180));
    ui->firstR->setIconSize(iconsSize);
    ui->secondR->setIconSize(iconsSize);
    ui->thirdR->setIconSize(iconsSize);

    ui->firstV->setChecked(true);
    ui->thirdR->setChecked(true);

    ui->SliderT->setMaximum(nt*100000000);
    ui->SliderZ->setMaximum(nz);

    updateTGraphic();
    updateZGraphic();

    this->setWindowTitle("Transmission Lines");
    changed = false;

}

TView::~TView() {
    delete ui;
}


void TView::animationZFinished(){
    QMessageBox::information(this, "Animação Z", "ACABOU A ANIMAÇÃO UHUUUUUUUUUUUUUUU!!!!");
}

void TView::animationTFinished(){
    QMessageBox::information(this, "Animação T", "ACABOU A ANIMAÇÃO UHUUUUUUUUUUUUUUU!!!!");
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

void TView::on_SliderT_valueChanged(){
    ui->tLine->setText(QString::number(double(ui->SliderT->value()) / 100000000));
}

void TView::on_SliderZ_valueChanged(){
    ui->zLine->setText(QString::number(double(ui->SliderZ->value())));
}

void TView::on_zLine_textChanged(const QString &arg1){
    std::string edit = arg1.toStdString();

    if(arg1.contains('.')){
        int index = arg1.indexOf('.');
        edit.replace(uint32_t(index), 1, ",");
    }

    double fixZ;
    try{
        fixZ = std::stod(edit);

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

    double fixT;
    try{
        fixT = std::stod(edit);

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
            graphs->updateParameters(datas, nt, nz, dt, dz);
            updateTGraphic();
            updateZGraphic();
            dtPrev = dt;
            ntPrev = nt;
            dzPrev = dz;
            nzPrev = nz;
            QMessageBox::information(this, tr("CONFIRMATION"), tr("Tudo enviado e atualizado"));
        }else{
            QMessageBox::warning(this, tr("ERROR BOX"), tr("Parametros ultrapassam o limite da memória"));
            ui->dT->setText(QString::number(dtPrev));
            ui->nT->setText(QString::number(ntPrev));
            ui->dZ->setText(QString::number(dzPrev));
            ui->nZ->setText(QString::number(nzPrev));
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

    double dT;
    try{
        dT = std::stod(edit);

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

    double nT;
    try{
        nT = std::stod(edit);

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

    double dZ;
    try{
        dZ = std::stod(edit);

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

    double nZ;
    try{
        nZ = std::stod(edit);
        if(nZ != nz && nZ != 0){
            nz = nZ;
            changed = true;
        }

    }catch(std::invalid_argument e){

    }catch(std::out_of_range e2){

    }
}



void TView::on_BtPlayT_clicked(){
    std::cout << "Iniciando a thread do T\n";
    QThreadPool::globalInstance()->start(animT);
}

void TView::on_BtPlayZ_clicked(){
    std::cout << "Iniciando a thread do Z\n";
    QThreadPool::globalInstance()->start(animZ);
}
