#include <iostream>
#include <stdarg.h>
#include <vector>
#include <algorithm>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include "FeatureDetector.h"

#define TAG "FetureDetector"
const std::string compare_dir = "/Users/chengong.cg/Git/native/OpenCV/res/compare/";

using namespace std;
using namespace cv;

// bool responseCompare(const KeyPoint &k1, const KeyPoint &k2) {
//    return k1.response > k2.response;
// }

MyFeatureDetector::MyFeatureDetector() {
    log("construct");
    mSrcImage1 = imread(compare_dir + "frame_1630.jpg");
    mSrcImage2 = imread(compare_dir + "frame_1631.jpg");
}

MyFeatureDetector::~MyFeatureDetector() {
    log("destruct");
    mSrcImage1.release();
    mSrcImage2.release();
}

bool MyFeatureDetector::detect(DetectType type) {
    log("detect %d", type);
    bool ret = false;

    mType = type;
    Mat srcGrayImage1, srcGrayImage2;
    if (mSrcImage1.channels() == 3 && mSrcImage2.channels() == 3) {
        cvtColor(mSrcImage1, srcGrayImage1, CV_RGB2GRAY);
        cvtColor(mSrcImage2, srcGrayImage2, CV_RGB2GRAY);
    } else {
        log("invalid src image channel %d, %d", mSrcImage1.channels(), mSrcImage2.channels());
        return false;
    }

    if (type == DetectType::FAST) {
        ret = detectFAST(srcGrayImage1, srcGrayImage2);
    } else if (type == DetectType::SIFT) {
        ret = detectSIFT(srcGrayImage1, srcGrayImage2);
    }

    srcGrayImage1.release();
    srcGrayImage2.release();

    return ret;
}

bool MyFeatureDetector::detectFAST(Mat &img1, Mat &img2) {
    vector<KeyPoint> detectKeyPoint1, detectKeyPoint2;
    Ptr<FastFeatureDetector> fastDetector = FastFeatureDetector::create(25, true, FastFeatureDetector::TYPE_9_16);
    fastDetector->detect(img1, detectKeyPoint1);
    fastDetector->detect(img2, detectKeyPoint2);

    // for (auto val : detectKeyPoint1) {
    //     log("response: %d", val.response);
    // }
    // sort(detectKeyPoint1.begin(), detectKeyPoint1.end(), responseCompare);

    Mat keyPointImage1, keyPointImage2;
    drawKeypoints(mSrcImage1, detectKeyPoint1, keyPointImage1, Scalar(255,0,0), DrawMatchesFlags::DEFAULT);
    imwrite("/Users/chengong.cg/Git/native/OpenCV/res/FAST_1.jpg", keyPointImage1);
    drawKeypoints(mSrcImage2, detectKeyPoint2, keyPointImage2, Scalar(255,0,0), DrawMatchesFlags::DEFAULT);
    imwrite("/Users/chengong.cg/Git/native/OpenCV/res/FAST_2.jpg", keyPointImage2);

    Ptr<xfeatures2d::SiftDescriptorExtractor> siftDescriptorExtractor =
        xfeatures2d::SiftDescriptorExtractor::create();
    Mat imageDesc1, imageDesc2;
    siftDescriptorExtractor->compute(mSrcImage1, detectKeyPoint1, imageDesc1);
    siftDescriptorExtractor->compute(mSrcImage2, detectKeyPoint2, imageDesc2);

    vector<DMatch> Matches;
    Ptr<FlannBasedMatcher> flannMatcher = FlannBasedMatcher::create();
    flannMatcher->match(imageDesc1, imageDesc2, Matches);
    // Ptr<BruteForceMatcher> bruteMatcher = BruteForceMatcher::create();
    // bruteMatcher->match(imageDesc1, imageDesc2, Matches);
    Mat fastMatchImg;
    drawMatches(mSrcImage1, detectKeyPoint1, mSrcImage2, detectKeyPoint2, Matches, fastMatchImg);
    imwrite("/Users/chengong.cg/Git/native/OpenCV/res/FAST_Match.jpg", fastMatchImg);

    keyPointImage1.release();
    keyPointImage2.release();
    imageDesc1.release();
    imageDesc2.release();
    fastMatchImg.release();
    return true;
}

bool MyFeatureDetector::detectSIFT(Mat &img1, Mat &img2) {
    Ptr<xfeatures2d::SiftFeatureDetector> siftDetector = xfeatures2d::SiftFeatureDetector::create(1600);

    vector<KeyPoint> detectKeyPoint1, detectKeyPoint2;
    siftDetector->detect(img1, detectKeyPoint1);
    siftDetector->detect(img2, detectKeyPoint2);

    Mat keyPointImage1, keyPointImage2;
    drawKeypoints(mSrcImage1, detectKeyPoint1, keyPointImage1, Scalar(255,0,0), DrawMatchesFlags::DEFAULT);
    imwrite("/Users/chengong.cg/Git/native/OpenCV/res/SIFT_1.jpg", keyPointImage1);
    drawKeypoints(mSrcImage2, detectKeyPoint2, keyPointImage2, Scalar(255,0,0), DrawMatchesFlags::DEFAULT);
    imwrite("/Users/chengong.cg/Git/native/OpenCV/res/SIFT_2.jpg", keyPointImage2);

    return true;
}

void MyFeatureDetector::log(const char *strFormat, ...) {
    if (NULL == strFormat) {
        return;
    }

    va_list arg_ptr;
    va_start(arg_ptr, strFormat);
    char strInfo[50] = {0};
    vsprintf(strInfo, strFormat, arg_ptr);
    std::cout << TAG << ">>>" << strInfo << std::endl;
    va_end(arg_ptr);
}