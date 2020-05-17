#ifndef GRAPH_CLASS
#define GRAPH_CLASS

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

class Graph{

private:
    float voltage(float x);
    float current(float x);

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
    Graph(functionData_t fdata, int widht, int height);
    cv::Mat print_img();

};



#endif //GRAPH_CLASS