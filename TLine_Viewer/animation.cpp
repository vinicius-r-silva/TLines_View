#include "animation.h"

Animation::Animation(Graph *graphs, QLabel *imShow, double min, double max, int steps, double delay){
    this->min = min;
    this->max = max;
    this->steps = steps;
    this->delay = delay;
    this->graphs = graphs;
    this->imShow = imShow;
}

void Animation::run(){
    double step = (max - min)/(double)steps;

    std::cout << "Thread iniciada\n";

    for(double d = min; d < max; d += step){
        std::cout << (int)(d/step) << std::endl;
        cv::Mat img = graphs->TFixed_Graph(d);
        std::cout << "DEBUG 1\n";
        imShow->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));
        std::cout << "DEBUG 2\n";
        usleep(delay * 1000);
    }

    emit Tfinished();
}
