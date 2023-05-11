#ifndef IR_OR_RGB_H
#define IR_OR_RGB_H

#include <string>
#include <opencv2/core/core.hpp>

class ir_or_rgb
{
public:
    ir_or_rgb();

public:
    // 1
    static bool variance(cv::Mat &rbg, int threshold);
    static bool variance1(cv::Mat &rbg, int threshold);

    // test
    static bool folder_image_check_variance(std::string &folder_path, int type);

    // 2
    static bool is_ir_or_rgb(cv::Mat &rgb, int w, int h, float &degree);
    static bool check_is_ir_or_rgb(cv::Mat &rgb, int w, int h);

    // test
    static bool folder_image_check(std::string &folder_path, int type);

public:
};

#endif // IR_OR_RGB_H
