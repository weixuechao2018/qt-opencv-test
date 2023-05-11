#include "mainwindow.h"

#include <QApplication>

#include <stdio.h>
#include <cstdio>
#include <dirent.h>
#include <type_traits>

#include <fstream> //处理文件的头文件
#include <iostream>
#include <sstream>
#include <ostream>

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "file_helper.h"
#include "ir_or_rgb.h"

#define TUNE(r) ( r < 0 ? 0 : (r > 255 ? 255 : r) )

using namespace cv;
using namespace std;

static const cv::Scalar sScalarR = cv::Scalar(0,255,255);
static const cv::Scalar sScalarG = cv::Scalar(255,0,255);
static const cv::Scalar sScalarB = cv::Scalar(255,255,0);

static const cv::Scalar sScalarRa = cv::Scalar(0,255,255,255);
static const cv::Scalar sScalarGa = cv::Scalar(255,0,255,255);
static const cv::Scalar sScalarBa = cv::Scalar(255,255,0,255);
static const cv::Scalar sScalarAa = cv::Scalar(255,255,255,0);

std::vector<Mat> rgb2hsv(Mat image) {
    std::vector<Mat> image_rgb;
    std::vector<Mat> hsv(3);

    split(image, image_rgb);
    cv::Mat B = (cv::Mat_<double>)image_rgb.at(0) / 255;
    cv::Mat G = (cv::Mat_<double>)image_rgb.at(1) / 255;
    cv::Mat R = (cv::Mat_<double>)image_rgb.at(2) / 255;
    cv::Mat_<double> H(image.rows, image.cols, 1);
    cv::Mat_<double> S(image.rows, image.cols, 1);
    cv::Mat_<double> V(image.rows, image.cols, 1);

    for (int m = 0; m <image.rows; m++) {
        for (int n = 0; n < image.cols; n++) {
            double var_B = B.at<double>(m, n);//image.at<cv::Vec3b>(j,i)[0];;B.data[m, n]
            double var_G = G.at<double>(m, n);
            double var_R = R.at<double>(m, n);
            //double var_Min=0;
            //double var_Max=100;
            double var_Min = min(var_R, min(var_G, var_B));    //Min. value of RGB
            double var_Max = max(var_R, max(var_G, var_B));    //Max. value of RGB
            double del_Max = var_Max - var_Min;             //Delta RGB value
            V.at<double>(m, n) = var_Max;

            if (del_Max == 0.0) {                    //This is a gray, no chroma...
                H.at<double>(m, n) = 0.0;                                //HSV results from 0 to 1
                S.at<double>(m, n) = 0.0;
            } else {                                    //Chromatic data...
                if (var_Max == 0.0) {
                    S.at<double>(m, n) = 0.0;
                } else {
                    S.at<double>(m, n) = del_Max / var_Max;
                }

                if (var_R == var_Max) H.at<double>(m, n) = (var_G - var_B) / del_Max;
                else if (var_G == var_Max) H.at<double>(m, n) = 2 + (var_B - var_R) / del_Max;
                else if (var_B == var_Max) H.at<double>(m, n) = 4 + (var_R - var_G) / del_Max;

                H.at<double>(m, n) /= 6;
                if (H.at<double>(m, n) < 0) H.at<double>(m, n) += 1.0;
            }
        }
    } // end for
    hsv.at(0) = H;
    hsv.at(1) = S;
    hsv.at(2) = V;

    return hsv;
}

static void YUV2RGB422(unsigned char *pRGB, unsigned char *pYUV,int size)
{
    unsigned char y, u, v,y1;
    int r,g,b;
    unsigned int i=0;
    unsigned int loop = size >>1;
    while(loop-- > 0) {
        y = *pYUV; pYUV++;
        u = *pYUV; pYUV++;
        y1 = *pYUV;pYUV++;
        v = *pYUV; pYUV++;

        r = 1.164*(y-16) + 1.596*(v-128);
        g = 1.164*(y-16) - 0.813*(v-128) - 0.392*(u-128);
        b = 1.164*(y-16) + 2.017*(u-128);

        *pRGB = TUNE(r);pRGB++;
        *pRGB = TUNE(g);pRGB++;
        *pRGB = TUNE(b);pRGB++;

        r = 1.164*(y1-16) + 1.596*(v-128);
        g = 1.164*(y1-16) - 0.813*(v-128) - 0.392*(u-128);
        b = 1.164*(y1-16) + 2.017*(u-128);
        *pRGB = TUNE(r);pRGB++;
        *pRGB = TUNE(g);pRGB++;
        *pRGB = TUNE(b);pRGB++;
    }
}



cv::VideoCapture *sCamCapture;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    std::string folder_path = "/home/weixuechao/Downloads/1/";
//    std::string folder_path = "/home/weixuechao/Downloads/11/";
//    std::string folder_path = "/home/weixuechao/Downloads/15_1/";
    std::string folder_path = "/home/weixuechao/Downloads/jidu-img/";
    ir_or_rgb::folder_image_check_variance(folder_path, 1);
//    ir_or_rgb::folder_image_check(folder_path, 1);

//    std::string path = "/home/weixuechao/Downloads/15_1/0001.jpg";
//    std::string path = "/home/weixuechao/Downloads/15_1/15121865458175.png";
//    std::string path = "/home/weixuechao/Downloads/15_1/4556643640013.jpg";
    std::string path = "/home/weixuechao/Downloads/jidu-img/939283.jpg";
    cv::Mat image = cv::imread(path);

    int type = image.type();
    int typec = CV_8UC3;
    int channels = image.channels();

//    Judge_color(image);

    ir_or_rgb::variance(image, 15);
    ir_or_rgb::check_is_ir_or_rgb(image, image.cols, image.rows);

    MainWindow w;
    w.show();
    return a.exec();
}









enum color_type
{
    unknown_error = 0,
    channel_3_color = 1,//3通道彩色
    channel_3_gray = 2,//3通道灰度
    channel_3_binary = 3,//3通道二值
    channel_1_gray = 4,//1通道灰度
    channel_1_binary = 5,//1通道二值
};
//1用各个通道进行矩阵对比
color_type Judge_color(cv::Mat img)
{
    color_type color_flag;
    if (!img.data)
        return color_flag = unknown_error;

    if (img.channels() == 3)//3通道
    {
        std::vector<cv::Mat> rgbChannels(3);
        split(img, rgbChannels);
        if (!countNonZero(rgbChannels[0] != rgbChannels[1]) && !countNonZero(rgbChannels[1] != rgbChannels[2]))//3通道完全相同
        {
            //非0.255个数=总数-0个数-255个数(化简后)
            int num = countNonZero(rgbChannels[0]) + countNonZero(~rgbChannels[0]) - rgbChannels[0].rows*rgbChannels[0].cols;
            if (num)//灰度
            {
                color_flag = channel_3_gray;
            }
            else//黑白
            {
                color_flag = channel_3_binary;
            }
        }
        else//三通道不完全同，彩色图
        {
            color_flag = channel_3_color;
        }
    }
    else if (img.channels() == 1)//1通道
    {
        int num = countNonZero(img) + countNonZero(~img) - img.rows*img.cols;
        if (num)//灰度
        {
            color_flag = channel_1_gray;
        }
        else//黑白
        {
            color_flag = channel_1_binary;
        }
    }
    return color_flag;
}




//    cv::Mat_<float> r = rgb - cv::Mat(rgb.rows, rgb.cols, CV_32FC3, cv::Scalar(0.f, 255.f, 255.f));
//    cv::Mat_<float> g = rgb - cv::Mat(rgb.rows, rgb.cols, CV_32FC3, cv::Scalar(255.f, 0.f, 255.f));
//    cv::Mat_<float> b = rgb - cv::Mat(rgb.rows, rgb.cols, CV_32FC3, cv::Scalar(255.f, 255.f, 0.f));


//    if(image.type() == cv::IMREAD_COLOR) {
//        cv::Mat gray;
//        cv::cvtColor(image, gray, cv::ColorConversionCodes::COLOR_BGR2GRAY); // 转换为灰度图
//        cv::Scalar stdDev;        // 均值
//        cv::Scalar meanStdDev;    // 方差
//        cv::meanStdDev(gray,  stdDev, meanStdDev);

//        if(gray.type() != cv::IMREAD_COLOR) {
//            int xxx = 0;
//        }
//        cv::Mat gray2rgb;
//        cv::cvtColor(gray, gray2rgb, cv::ColorConversionCodes::COLOR_GRAY2BGR); // 转换为灰度图
//        if(gray2rgb.empty()) {
//            if(gray.empty()) {

//            }
//        }
//    }


//    sCamCapture = new cv::VideoCapture;
//    sCamCapture->release();
//    sCamCapture->open(0);
//    if (!sCamCapture->isOpened()){
//        cout << "cam open failed!" << endl;
//        return -1;
//    }

//    int fps = sCamCapture->get(CAP_PROP_FPS); // 获取原视频的帧率
//    if (fps <= 0) {
//        fps = 25;
//    }

//    VideoWriter vw;
//    vw.open("/opt/out1120.avi",
//            VideoWriter::fourcc('X', '2', '6', '4'),
//            fps,
//            Size(sCamCapture->get(CAP_PROP_FRAME_WIDTH), sCamCapture->get(CAP_PROP_FRAME_HEIGHT)));

//    if (!vw.isOpened()){ // 判断VideoWriter是否正常打开
//        cout << "videoWriter open failed!" << endl;
//        getchar();
//        return -1;
//    }

//    cv::Mat frame;
//    for(;;){
//        sCamCapture->read(frame); // 读帧
//        if (frame.empty())  {
//            continue;
//        }

//        check_is_ir_or_rgb1(frame, 15);
//        check_is_ir_or_rgb(frame, frame.cols, frame.rows);

////        imshow("cam", frame); // 展示当前帧
//        /*
//        这里可以添加对当前帧的处理操作
//        */
////        vw.write(frame); // 保存当前帧
////        if (waitKey(5) == 'q') {
////            break; // 键入q停止
////        }
//    }
