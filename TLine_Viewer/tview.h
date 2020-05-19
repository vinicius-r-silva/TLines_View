#ifndef TVIEW_H
#define TVIEW_H

#include <QMainWindow>
#include <QtWidgets>
#include "graph.h"

namespace Ui {
class TView;
}

class TView : public QMainWindow {
    Q_OBJECT

public:
    explicit TView(QWidget *parent = nullptr);
    ~TView();

private slots:
    void on_tLine_textChanged(const QString &arg1);
    void on_zLine_textChanged(const QString &arg1);

    void on_firstV_clicked();
    void on_secondV_clicked();

    void on_firstR_clicked();
    void on_secondR_clicked();
    void on_thirdR_clicked();

    void on_BtRecalcular_clicked();

    void on_dT_textChanged(const QString &arg1);
    void on_nT_textChanged(const QString &arg1);
    void on_dZ_textChanged(const QString &arg1);
    void on_nZ_textChanged(const QString &arg1);

    void updateZGraphic();
    void updateTGraphic();

    bool parametersValid();

private:
    functionData_t *datas;
    Ui::TView *ui;
    Graph *graphs;

    bool changed;
    double dtPrev;
    double dzPrev;
    double ntPrev;
    double nzPrev;
    double zFix;
    double tFix;
    double dt;
    double dz;
    double nt;
    double nz;
    int vol;
    int res;
};

#endif // TVIEW_H
