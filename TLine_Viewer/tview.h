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
    QLineEdit *zLine;
    QLineEdit *tLine;
    Ui::TView *ui;
    Graph *graphs;

    float dtPrev;
    float dzPrev;
    float zFix;
    float tFix;
    float dt;
    float dz;
    int ntPrev;
    int nzPrev;
    int vol;
    int res;
    int nt;
    int nz;


    bool changed;
};

#endif // TVIEW_H
