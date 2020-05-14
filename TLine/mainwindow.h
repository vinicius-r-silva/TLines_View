#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void timeChanged(int newTime);
    void zPosChanged(int newZPos);

private slots:
    void on_zBox_currentIndexChanged(int index);
    void on_tBox_currentIndexChanged(int index);

    void on_firstV_clicked();

private:
    Ui::MainWindow *ui;
    QComboBox *zBox;
    QComboBox *tBox;
};

#endif // MAINWINDOW_H
