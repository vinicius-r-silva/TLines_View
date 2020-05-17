#ifndef TVIEW_H
#define TVIEW_H

#include <QMainWindow>
#include <QtWidgets>

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

    void on_dT_textChanged(const QString &arg1);
    void on_nT_textChanged(const QString &arg1);
    void on_dZ_textChanged(const QString &arg1);
    void on_nZ_textChanged(const QString &arg1);

private:
    Ui::TView *ui;
    QLineEdit *zLine;
    QLineEdit *tLine;
};

#endif // TVIEW_H
