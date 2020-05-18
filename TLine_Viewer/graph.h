#ifndef GRAPH_CLASS
#define GRAPH_CLASS

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "function.h"

class Graph{

private:
    double TFixed_vo(double z);
    double TFixed_ic(double z);
    double ZFixed_vo(double t);
    double ZFixed_ic(double t);

    int width;
    int height;

    double fixedT;
    double fixedZ;

    double max_vo, min_vo;
    double max_ic, min_ic;

    struct PrintParameters{
        double (Graph::*voltage)(double);
        double (Graph::*current)(double);

        double max_x;
    };

    cv::Vec3b white;
    cv::Vec3b black;
    cv::Vec3b blue;
    cv::Vec3b orange;
    cv::Vec3b green;
    cv::Vec3b red;
    cv::Vec3b dark_grey;
    cv::Vec3b purple;
    cv::Vec3b dark_red;
    cv::Vec3b sycamore;

public:
    Graph(functionData_t fdata, int width, int height, double TMax, double ZMax);
    cv::Mat print_img(PrintParameters p);
    void updateParameters(functionData_t fdata);

    cv::Mat TFixed_Graph(double t);
    cv::Mat ZFixed_Graph(double z);

};



#endif //GRAPH_CLASS