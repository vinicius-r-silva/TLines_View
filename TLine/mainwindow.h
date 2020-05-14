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

private slots:
    void on_tLine_textChanged(const QString &arg1);
    void on_zLine_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QLineEdit *zLine;
    QLineEdit *tLine;
};

#endif // MAINWINDOW_H
