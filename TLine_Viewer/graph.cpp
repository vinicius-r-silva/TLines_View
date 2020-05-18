
//g++ mycode.cpp -o main `pkg-config --cflags --libs opencv`

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <time.h>     
#include <stdlib.h>  
#include <math.h>

#include "function.h"
#include "graph.h"

#define _Z0 0
#define _ZF 100

#define _X_LABEL_SIZE 30
#define _Y_LABEL_SIZE 120
#define _X_LABEL_DIST 100
#define _Y_LABEL_DIST 40

#define QTD_PER_PX 10

double Graph::TFixed_vo(double z){
    return (fixedT - 2)*cos(0.39*z) + (fixedT + 5)*sin(0.5*z) + (fixedT - 0.5)*cos(0.1*z) + (fixedT + 10)*sin(0.7*z) + (fixedT - 5)*sin(1*z) + (fixedT + 5)*sin(0.35*z);
}

double Graph::TFixed_ic(double z){
    return (fixedT - 2)*sin(0.19*z) + (fixedT + 5)*sin(0.25*z) + (fixedT - 0.5)*cos(1*z) + (fixedT + 10)*sin(1.4*z) + (fixedT - 5)*cos(1*z) + (fixedT + 5)*cos(0.35*z);
}

double Graph::ZFixed_vo(double t){
    return (fixedZ - 2)*cos(0.39*t) + (fixedZ + 5)*sin(0.5*t) + (fixedZ - 0.5)*cos(0.1*t) + (fixedZ + 10)*sin(0.7*t) + (fixedZ - 5)*sin(1*t) + (fixedZ + 5)*sin(0.35*t);
}

double Graph::ZFixed_ic(double t){
    return (fixedZ - 2)*sin(0.19*t) + (fixedZ + 5)*sin(0.25*t) + (fixedZ - 0.5)*cos(1*t) + (fixedZ + 10)*sin(1.4*t) + (fixedZ - 5)*cos(1*t) + (fixedZ + 5)*cos(0.35*t);
}

cv::Mat Graph::TFixed_Graph(double t){
    fixedT = t;

    PrintParameters p;
    p.max_x = _ZF;
    p.voltage = &Graph::TFixed_vo;
    p.current = &Graph::TFixed_ic;

    return print_img(p);
}

cv::Mat Graph::ZFixed_Graph(double z){
    fixedZ = z;

    PrintParameters p;
    p.max_x = _ZF;
    p.voltage = &Graph::ZFixed_vo;
    p.current = &Graph::ZFixed_ic;

    return print_img(p);
}

void Graph::updateParameters(functionData_t fdata){
    max_vo = 30, min_vo = -30;
    max_ic = 30, min_ic = -30;
}

cv::Mat Graph::print_img(PrintParameters p){
    int i = 0;
    int px = 0;
    double vo,ic;

    int printable_width = width - _Y_LABEL_SIZE;
    int printable_height = height - _X_LABEL_SIZE; 

    double range_vo = max_vo - min_vo;
    double range_ic = max_ic - min_ic;

    cv::Mat image(height, width, CV_8UC3, white);
    cv::line(image, cv::Point(0, printable_height), cv::Point(width, printable_height), black);
    cv::line(image, cv::Point(_Y_LABEL_SIZE, 0), cv::Point(_Y_LABEL_SIZE, height), black);

    cv::Vec3b ic_color = green;
    cv::Vec3b vo_color = orange;

    double vo2px = printable_height / range_vo;
    double ic2px = printable_height / range_ic;

    double dx = double(_ZF) / double(printable_width * QTD_PER_PX);
    double dvo = range_vo / printable_height;
    double dic = range_ic / printable_height;

    for(i = 0; i < printable_width * QTD_PER_PX; i ++){
        vo = (this->*(p.voltage))(dx * i) * vo2px;
        vo = (this->*(p.voltage))(dx * i) * vo2px;

        vo = (printable_height / 2) - vo;
        ic = (printable_height / 2) - ic;
        
        px = i / QTD_PER_PX + _Y_LABEL_SIZE; 
        cv::circle(image, cv::Point(px, vo), 1, vo_color);
        cv::circle(image, cv::Point(px, ic), 1, ic_color);
    }


    //Axis values
    char value[20];
    cv::Size textSize;
    memset(value, '\0', 20 * sizeof(char));

    i = _X_LABEL_DIST;
    for(; i < width - _X_LABEL_DIST; i += _X_LABEL_DIST){
        sprintf(value, "%.2f", dx * i * QTD_PER_PX);
        textSize = getTextSize(value, cv::FONT_HERSHEY_PLAIN, 1, 1, 0);
        cv::putText(image, value, cv::Point(i + _Y_LABEL_SIZE - textSize.width, height - 5), cv::FONT_HERSHEY_PLAIN, 1, black, 1);
    }
    
    i = _Y_LABEL_DIST;
    for(; i < height - _Y_LABEL_DIST; i += _Y_LABEL_DIST){
        sprintf(value, "%6.1f,", dvo * i + min_vo);
        textSize = getTextSize(value, cv::FONT_HERSHEY_PLAIN, 1, 1, 0);
        cv::putText(image, value, cv::Point(0, printable_height - i + textSize.height), cv::FONT_HERSHEY_PLAIN, 1, vo_color, 1);

        sprintf(value, "%6.1f", dic * i + min_ic);
        textSize = getTextSize(value, cv::FONT_HERSHEY_PLAIN, 1, 1, 0);
        cv::putText(image, value, cv::Point(60, printable_height - i + textSize.height), cv::FONT_HERSHEY_PLAIN, 1, ic_color, 1);
    }


    //Subtitle

    cv::line(image, cv::Point(width - 90, 25), cv::Point(width - 40, 25), vo_color, 2);
    cv::line(image, cv::Point(width - 90, 55), cv::Point(width - 40, 55), ic_color, 2);
    cv::putText(image, "v", cv::Point(width - 25, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, vo_color, 2);
    cv::putText(image, "i", cv::Point(width - 20, 60), cv::FONT_HERSHEY_SIMPLEX, 0.8, ic_color, 2);

    cv::putText(image, "Z", cv::Point(width - 25, height - 5), cv::FONT_HERSHEY_SIMPLEX, 0.8, black, 2);
    
    return image;
}


Graph::Graph(functionData_t fdata, int width, int height){
    this->width = width;
    this->height = height;

    updateParameters(fdata);

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