#ifndef ANIMATION_H
#define ANIMATION_H

#include <QObject>
#include <QtWidgets/QLabel>
#include <QThreadPool>
#include <QDebug>
#include <unistd.h>

class Animation : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Animation(QLabel *imShow, double min, double max, double delay);

signals:
    void Tfinished();

public slots:

private:
    double min;
    double max;
    double delay;

    QLabel *imShow;

protected:
void run();
};

#endif // ANIMATION_H
