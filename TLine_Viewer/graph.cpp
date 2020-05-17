
//g++ mycode.cpp -o main `pkg-config --cflags --libs opencv`

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <time.h>     
#include <stdlib.h>  
#include <math.h>

#include "graph.h"

#define _Z0 0
#define _ZF 100

#define _WIDTH 1000
#define _HEIGHT 500
#define _X_LABEL_SIZE 30
#define _Y_LABEL_SIZE 50
#define _X_LABEL_DIST 100
#define _Y_LABEL_DIST 40

#define QTD_PER_PX 10

float Graph::voltage(float x){
    return 2*cos(0.39*x) + 5*sin(0.5*x) + 0.5*cos(0.1*x) + 10*sin(0.7*x) + 5*sin(1*x) + 5*sin(0.35*x);
}

float Graph::current(float x){
    return 2*sin(0.19*x) + 5*sin(0.25*x) + 0.5*cos(0.1*x) + 10*sin(0.7*x) + 5*cos(1*x) + 5*cos(0.35*x);
}

cv::Mat Graph::print_img(){
    int i = 0;
    int px = 0;
    float vo,ic;

    int printable_width = _WIDTH - _Y_LABEL_SIZE;
    int printable_height = _HEIGHT - _X_LABEL_SIZE; 


    float Max_vo = 30;
    float Min_vo = -30;
    float range_vo = Max_vo - Min_vo;

    float Max_ic = 30;
    float Min_ic = -30;
    float range_ic = Max_ic - Min_ic;

    cv::Mat image(_HEIGHT, _WIDTH, CV_8UC3, white);
    cv::line(image, cv::Point(0, printable_height), cv::Point(_WIDTH, printable_height), black);
    cv::line(image, cv::Point(_Y_LABEL_SIZE, 0), cv::Point(_Y_LABEL_SIZE, _HEIGHT), black);

    cv::Vec3b ic_color = green;
    cv::Vec3b vo_color = orange;

    float vo2px = printable_height / range_vo;
    float ic2px = printable_height / range_ic;

    float dx = float(_ZF) / float(printable_width * QTD_PER_PX);
    float dvo = range_vo / printable_height;
    float dic = range_ic / printable_height;

    float min = 100;
    float max = -100;

    for(i = 0; i < printable_width * QTD_PER_PX; i ++){
        vo = (voltage(dx * i) * vo2px);
        ic = (current(dx * i) * ic2px);

        if(voltage(dx * i) > max) max = voltage(dx * i);
        if(voltage(dx * i) < min) min = voltage(dx * i);

        std::cout << voltage(dx * i) << std::endl;

        vo = (printable_height / 2) - vo;
        ic = (printable_height / 2) - ic;
        
        px = i / QTD_PER_PX + _Y_LABEL_SIZE; 
        cv::circle(image, cv::Point(px, vo), 1, vo_color);
        cv::circle(image, cv::Point(px, ic), 1, ic_color);
    }

    std::cout << "max: " << max << ", min: " << min << std::endl;


    //Axis values
    char value[20];
    cv::Size textSize;
    memset(value, '\0', 20 * sizeof(char));

    i = _X_LABEL_DIST;
    for(; i < _WIDTH - _X_LABEL_DIST; i += _X_LABEL_DIST){
        sprintf(value, "%.2f", dx * i * QTD_PER_PX);
        textSize = getTextSize(value, cv::FONT_HERSHEY_PLAIN, 1, 1, 0);
        cv::putText(image, value, cv::Point(i + _Y_LABEL_SIZE - textSize.width, _HEIGHT - 5), cv::FONT_HERSHEY_PLAIN, 1, black, 1);
    }
    
    i = _Y_LABEL_DIST;
    for(; i < _HEIGHT - _Y_LABEL_DIST; i += _Y_LABEL_DIST){
        sprintf(value, "%03.1f", dvo * i + Min_vo);
        textSize = getTextSize(value, cv::FONT_HERSHEY_PLAIN, 1, 1, 0);
        cv::putText(image, value, cv::Point(0, printable_height - i + textSize.height), cv::FONT_HERSHEY_PLAIN, 1, vo_color, 1);
    }


    //Subtitle

    cv::line(image, cv::Point(_WIDTH - 90, 25), cv::Point(_WIDTH - 40, 25), vo_color, 2);
    cv::line(image, cv::Point(_WIDTH - 90, 55), cv::Point(_WIDTH - 40, 55), ic_color, 2);
    cv::putText(image, "v", cv::Point(_WIDTH - 25, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, vo_color, 2);
    cv::putText(image, "i", cv::Point(_WIDTH - 20, 60), cv::FONT_HERSHEY_SIMPLEX, 0.8, ic_color, 2);

    cv::putText(image, "Z", cv::Point(_WIDTH - 25, _HEIGHT - 5), cv::FONT_HERSHEY_SIMPLEX, 0.8, black, 2);
    
    return image;
}


Graph::Graph(){
    // blue        = cv::Vec3b( 57, 106, 177);
    // orange      = cv::Vec3b(218, 124,  48);
    // green       = cv::Vec3b( 62, 150,  81);
    // red         = cv::Vec3b(204,  37,  41);
    // dark_grey   = cv::Vec3b( 83,  81,  84);
    // purple      = cv::Vec3b(107,  76, 154);
    // dark_red    = cv::Vec3b(146,  36,  40);
    // sycamore    = cv::Vec3b(148, 139,  61);

    white       = cv::Vec3b(255, 255, 255);
    black       = cv::Vec3b(  0,   0,   0);

    blue        = cv::Vec3b(177, 106,  57);
    orange      = cv::Vec3b( 48, 124, 218);
    green       = cv::Vec3b( 81, 150,  62);
    red         = cv::Vec3b( 41,  37, 204);
    dark_grey   = cv::Vec3b( 84,  81,  83);
    purple      = cv::Vec3b(154,  76, 107);
    dark_red    = cv::Vec3b( 40,  36, 146);
    sycamore    = cv::Vec3b( 61, 139, 148);
}  