#include "ir_or_rgb.h"

#include "file_helper.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdio.h>
#include <cstdio>
#include <dirent.h>
#include <type_traits>

#include <fstream> //处理文件的头文件
#include <iostream>
#include <sstream>
#include <ostream>
#include <vector>

ir_or_rgb::ir_or_rgb()
{

}

// 暂时设置threshold==15
bool ir_or_rgb::variance(cv::Mat &rbg, int threshold) {
    if(rbg.channels() == 1) {
        return true;
    }

    cv::Mat rbg_data;
    rbg.convertTo(rbg_data, CV_32FC3);

    std::vector<cv::Mat_<float>> channels;
    cv::split(rbg_data, channels);
    cv::Mat_<float> r = channels[2];
    cv::Mat_<float> g = channels[1];
    cv::Mat_<float> b = channels[0];
    int bbb = b.channels();

    cv::Scalar r_stdDev;        // 均值
    cv::Scalar r_meanStdDev;    // 方差
    cv::meanStdDev(r - g,  r_stdDev, r_meanStdDev);

    cv::Scalar g_stdDev;
    cv::Scalar g_meanStdDev;
    cv::meanStdDev(g - b,  g_stdDev, g_meanStdDev);

    cv::Scalar b_stdDev;
    cv::Scalar b_meanStdDev;
    cv::meanStdDev(b - r,  b_stdDev, b_meanStdDev);

    float avg = (r_meanStdDev[0] * r_meanStdDev[0] +
                          g_meanStdDev[0] * g_meanStdDev[0] +
                          b_meanStdDev[0] * b_meanStdDev[0]) / 3.0;

//    printf("diff_sum:%d; avg:%f\n", diff_sum, avg);
//    fflush(stdout);
    std::cout << "avg:" << avg << std::endl;
    if ((int)avg <= threshold) {
        return true;
    }
    return false;
}

bool ir_or_rgb::variance1(cv::Mat &rbg, int threshold) {
    if(rbg.channels() == 1) {
        return true;
    }

    cv::Mat rbg_data;
    rbg.convertTo(rbg_data, CV_32FC3);

    std::vector<cv::Mat_<float>> channels;
    cv::split(rbg_data, channels);
    cv::Mat_<float> r = channels[2];
    cv::Mat_<float> g = channels[1];
    cv::Mat_<float> b = channels[0];

//    var = mean(abs(x - x.mean())**2)
    auto variance_func = [&](cv::Mat_<float> &mat, cv::Scalar &variance)->void {
        cv::Mat_<float> mat_pow;
        cv::pow(cv::abs(mat - cv::mean(mat)), 2, mat_pow);
//        cv::multiply(xxxx, xxxx, xxxxxx);
        variance = cv::mean(mat_pow);
    };

    cv::Scalar variance_r;
    cv::Scalar variance_g;
    cv::Scalar variance_b;

    cv::Mat_<float> rg = r - g;
    variance_func(rg, variance_r);

    cv::Mat_<float> gb = g - b;
    variance_func(gb, variance_g);

    cv::Mat_<float> br = b - r;
    variance_func(br, variance_b);

    int diff_sum = (int)((variance_r[0] + variance_g[0] + variance_b[0]) / 3.0);
    std::cout << "diff_sum:" << diff_sum << std::endl;
    if (diff_sum <= threshold) {
        return true;
    }
    return false;
}

bool ir_or_rgb::folder_image_check_variance(std::string &folder_path, int type) {

    std::vector<std::string> file_vector;
    file_helper::getDirFile(folder_path, file_vector);
    if(file_vector.empty()) {
        return false;
    }
    static const int pixel_h = 1920;
    static const int pixel_w = 1280;
    static const int len = pixel_h * pixel_w * 3;

    cv::Mat mat_dst;
    for (auto &iter : file_vector) {
        mat_dst.release();
        auto func = [&mat_dst, &iter]() {
            int type = mat_dst.type();
            int typec = CV_8UC3;

            std::cout << iter << "; channels:" << mat_dst.channels() << std::endl;

            // 图片缩小
            cv::Mat mat;
            int h = mat_dst.cols / 20;
            int w = mat_dst.rows / 20;
            cv::resize(mat_dst, mat, cv::Size(h, w), 0, 0, cv::INTER_AREA);

            ir_or_rgb::variance(mat, 15);
            ir_or_rgb::variance1(mat, 15);
        };

        switch (type) {
        case 1: {
            mat_dst = cv::imread(iter);
            func();
            continue;
        }
        case 2: {
            static const int pixel_h = 1920;
            static const int pixel_w = 1280;
            static const int len = pixel_h * pixel_w * 3;
            unsigned char pSrc[len+1] = {0};
            file_helper::readFile(iter, pSrc);
            cv::Mat yuv(1920, 1280, CV_8UC2, pSrc);
            cv::cvtColor(yuv, mat_dst, cv::COLOR_YUV2RGB_Y422);
            func();
            continue ;
        }
        default:
            break;
        }
    }
    return true;

#if 0
    // 代码重点部分
    cv::Mat yuv(1920, 1280, CV_8UC2, p);
    cv::Mat img;
    cv::cvtColor(yuv, img, cv::COLOR_YUV2BGR_UYVY);
#endif
}

bool ir_or_rgb::is_ir_or_rgb(cv::Mat &rgb, int w, int h, float &degree) {
    static const int thresh_hold = 10;

    int channels_num = rgb.channels();
    if(channels_num == 1) { // 单通道是灰度图
        return true ;
    }

    cv::Mat rbg_data;
    rgb.convertTo(rbg_data, CV_32FC3);

    //把一个三通道图像转化为三个单通道图像
    std::vector<cv::Mat_<float>> channels;
    cv::split(rbg_data, channels);

    cv::Mat_<float> &r = channels.at(0);
    cv::Mat_<float> &g = channels.at(1);
    cv::Mat_<float> &b = channels.at(2);

    cv::Mat_<float> x = (r + b + g) / 3;

    cv::Mat_<float> r_gray = cv::abs(r - x);
    cv::Mat_<float> g_gray = cv::abs(g - x);
    cv::Mat_<float> b_gray = cv::abs(b - x);

    static const int area = w * h;
    cv::Scalar r_sum = cv::sum(r_gray) / area;
    cv::Scalar g_sum = cv::sum(g_gray) / area;
    cv::Scalar b_sum = cv::sum(b_gray) / area;

    float gray_degree = ((r_sum[0] + g_sum[0] + b_sum[0]) / 3.0);
    degree = gray_degree;
//    std::cout << "gray_degree:" << gray_degree << std::endl;
    if(gray_degree < thresh_hold) {
        return true;
    } else {
        return false;
    }
}

bool ir_or_rgb::check_is_ir_or_rgb(cv::Mat &rgb, int w, int h) {
    static const int grid_num = 40;
    static const int grid_width = int(w / grid_num);
    static const int grid_height = int(h / grid_num);

    for(int i = 0; i < grid_num; ++i) {
        float interval_h_start = i * grid_height;
        for (int j = 0; j < grid_num; ++j) {
            float interval_w_start = j * grid_width;
            // 取一个方形图像
            cv::Rect rect(interval_w_start, interval_h_start, grid_width, grid_height);
//            cv::Mat roi;
//            roi = rbg_data(rect).clone();

//            cv::Mat roi(rgb,
//                        cv::Range(interval_h_start, grid_height),
//                        cv::Range(interval_w_start, grid_width));

            cv::Mat roi(rgb, rect);
            float gray_degree;
            if(ir_or_rgb::is_ir_or_rgb(roi, grid_width, grid_height, gray_degree) == false) {
                return false;
            }
        }
    }
    std::cout << "is gray" << std::endl;
    return true;
}

bool ir_or_rgb::folder_image_check(std::string &folder_path, int type) {

    std::vector<std::string> file_vector;
    file_helper::getDirFile(folder_path, file_vector);

    if(file_vector.empty()) {
        return false;
    }
    static const int pixel_h = 1920;
    static const int pixel_w = 1280;
    static const int len = pixel_h * pixel_w * 3;
    for (auto &iter : file_vector) {
        cv::Mat mat_dst;
        if(type == 1) {
            mat_dst = cv::imread(iter);
        } else if(type == 2) {
            static const int pixel_h = 1920;
            static const int pixel_w = 1280;
            static const int len = pixel_h * pixel_w * 3;
            unsigned char pSrc[len+1] = {0};
            file_helper::readFile(iter, pSrc);
            cv::Mat yuv(1920, 1280, CV_8UC2, pSrc);
            cv::cvtColor(yuv, mat_dst, cv::COLOR_YUV2RGB_Y422);
        } else {
            continue;
        }

        std::cout << iter << "; channels:" << mat_dst.channels() << std::endl;
        if(ir_or_rgb::check_is_ir_or_rgb(mat_dst, mat_dst.cols, mat_dst.rows)) {

        }
    }
    return true;
}
