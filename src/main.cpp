#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

int main()
{   
    //视频变量
    cv::VideoCapture cap("assets/armor.avi");

    //是否正常打开
    if (!cap.isOpened()) {
        std::cerr << "Cannot open video" << std::endl;
        return 1;
    }                               

    //读第一帧是否顺利
    cv::Mat frame;
    if (!cap.read(frame)) {
        std::cerr << "Cannot read first frame" << std::endl;
        return 1;
    }

    //第一帧长宽
    std::cout << "Frame size: "
              << frame.cols << " x " << frame.rows << std::endl;
    //保存第一帧
    if (!cv::imwrite("first_frame.jpg", frame)) {
        std::cerr << "Cannot save image" << std::endl;
        return 1;
    }
    std::cout << "Saved first_frame.jpg" << std::endl;

    //二值化

    //彩色转灰色图片
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    //按亮度筛选
    cv::Mat binary;
    cv::threshold(gray, binary, 100, 255, cv::THRESH_BINARY);

    //存储二值化后图片
    cv::imwrite("gray.jpg", gray);
    cv::imwrite("binary.jpg", binary);

    //寻找轮廓

    //点 轮廓 一组轮廓
    std::vector<std::vector<cv::Point>> contours;
    
    //寻找轮廓 
    cv::findContours(
        binary,
        contours,
        cv::RETR_EXTERNAL, //只检测外轮廓
        cv::CHAIN_APPROX_SIMPLE  //压缩轮廓
    );

    //输出旋转矩形信息
    for (const auto& contour : contours) {

    cv::RotatedRect rect = cv::minAreaRect(contour); //最小能包住的矩形

    cv::Point2f vertices[4];  //二维点类型 四个顶点

    rect.points(vertices);    //四个顶点写入数组


    float edge01 = cv::norm(vertices[1] - vertices[0]);
    float edge12 = cv::norm(vertices[2] - vertices[1]);

    cv::Point2f endpointA;
    cv::Point2f endpointB;

    if (edge01 < edge12) {
        endpointA = (vertices[0] + vertices[1]) * 0.5f;
        endpointB = (vertices[2] + vertices[3]) * 0.5f;
    } else {
        endpointA = (vertices[1] + vertices[2]) * 0.5f;
        endpointB = (vertices[3] + vertices[0]) * 0.5f;
    }

    cv::Point2f top;
    cv::Point2f bottom;

    if (endpointA.y < endpointB.y) {
        top = endpointA;
        bottom = endpointB;
    } else {
        top = endpointB;
        bottom = endpointA;
    }

    std::cout << "Top: " << top << ", bottom: " << bottom << std::endl;

    //输出四个顶点
    for (int i = 0; i < 4; ++i) {
        std::cout << "Vertex " << i << ": " << vertices[i] << std::endl;
    }

    std::cout << "Center: " << rect.center
              << ", size: " << rect.size
              << ", angle: " << rect.angle
              << std::endl;
}

    std::cout << "Contour count: " << contours.size() << std::endl;

    //现在画回原图

    //原图的独立副本
    cv::Mat result = frame.clone();

    //画图 把contours画到result上 
    cv::drawContours(
        result,
        contours,
        -1, //全画
        cv::Scalar(0, 255, 0), //绿色线条画
        1 //粗细
    );

    cv::imwrite("contours.png", result);

    return 0;
}
