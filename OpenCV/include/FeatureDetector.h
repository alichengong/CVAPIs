#ifndef FEATURE_DETECTOR_H
#define FEATURE_DETECTOR_H

#include <stdarg.h>

enum class DetectType {
    INIT = 0,
    FAST = 1, // FastFeatureDetector, 点特征
    STAR = 2, // StarFeatureDetector
    SIFT = 3, // SIFT (nonfree module) 块特征
    SURF = 4, // SURF (nonfree module) 块特征
    ORB = 5, // ORB
    MSER = 6, // MSER
    GFTT = 7, // GoodFeaturesToTrackDetector 点特征
    HARRIS = 8, // GoodFeaturesToTrackDetector with Harris detector enabled 点特征
    Dense = 9, // DenseFeatureDetector
    SimpleBlob = 10, // SimpleBlobDetector 块特征
};

class MyFeatureDetector {
    public:
        MyFeatureDetector();
        ~MyFeatureDetector();
        bool detect(DetectType);

    private:
        void log(const char *strFormat, ...);
        bool detectFAST(cv::Mat &img1, cv::Mat &imag2);
        bool detectSIFT(cv::Mat &img1, cv::Mat &imag2);

        DetectType mType{DetectType::INIT};
        cv::Mat mSrcImage1;
        cv::Mat mSrcImage2;
};

#endif