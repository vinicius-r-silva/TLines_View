#ifndef ANIMATION_H
#define ANIMATION_H

#include <QObject>
#include <QtWidgets/QLabel>
#include <QThreadPool>
#include <QDebug>
#include <unistd.h>
#include "graph.h"

class Animation : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Animation(Graph *graphs, QLabel *imShow, double min, double max, int steps, double delay);

signals:
    void Tfinished();

public slots:

private:
    double min;
    double max;
    double delay;
    int steps;

    QLabel *imShow;
    Graph *graphs;

protected:
    void run();
};

#endif // ANIMATION_H
