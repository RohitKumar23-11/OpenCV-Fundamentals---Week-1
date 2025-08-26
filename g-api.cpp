#include <opencv2/gapi.hpp>
#include <opencv2/gapi/core.hpp>
#include <opencv2/gapi/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

static cv::GMat convertScaleAbs(const cv::GMat &src, double alpha = 1.0, double beta = 0.0) {
    auto result = cv::gapi::absDiffC(cv::gapi::addC(cv::gapi::mulC(src, alpha), beta), 0.0);
    return cv::gapi::convertTo(result, CV_8UC1);
}

int main() {
    cv::GMat in;
    auto blurred = cv::gapi::gaussianBlur(in, cv::Size(3, 3), 0);
    auto gray = cv::gapi::convertTo(blurred, CV_32F);
    auto sobelX = cv::gapi::Sobel(gray, CV_16S, 1, 0, 3);
    auto sobelY = cv::gapi::Sobel(gray, CV_16S, 0, 1, 3);
    auto gradX = convertScaleAbs(sobelX);
    auto gradY = convertScaleAbs(sobelY);

    auto out = cv::gapi::addWeighted(gradX, 0.5, gradY, 0.5, 0);

    cv::GComputation graph(cv::GIn(in), cv::GOut(out));

    cv::Mat imgIn = cv::imread("dog.png"), imgOut;
    graph.apply(cv::gin(imgIn), cv::gout(imgOut));

    cv::imwrite("output.png", imgOut);
    return 0;
}
