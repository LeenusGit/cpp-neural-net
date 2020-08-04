//
// Created by leenus on 24/07/2020.
//

#ifndef MNIST_DRAWIMAGE_HPP
#define MNIST_DRAWIMAGE_HPP

#include "dataset.hpp"
#include <opencv2/opencv.hpp>

auto draw_image(const Image &data, const int width, const std::string& title = "mnist",  const int scale = 10) -> void {

    cv::Mat draw_image = cv::Mat::zeros(width * scale, width * scale, CV_8U);

    cv::Point start = cv::Point(0, 0);
    cv::Point end = cv::Point(0, 0);

    const int size = width * width;

    for (int i = 0; i < size; ++i) {

        if (i % width == 0) {

            start.x = 0;
            start.y = scale * (i / width);

            end.x = start.x + scale;
            end.y = start.y + scale;
        }

        auto grey_scale = data.at(i);
        cv::rectangle(draw_image, start, end, cv::Scalar(grey_scale), cv::FILLED);

        start.x += scale;
        end.x = start.x + scale;
    }

    imshow(title, draw_image);
    cv::waitKey(0);
}

#endif//MNIST_DRAWIMAGE_HPP
