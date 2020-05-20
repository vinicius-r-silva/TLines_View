#include "animation.h"

Animation::Animation(QLabel *imShow, double min, double max, double delay){
    this->min = min;
    this->max = max;
    this->delay = delay;
    this->imShow = imShow;
}

void Animation::run()
{
    emit Tfinished();
}