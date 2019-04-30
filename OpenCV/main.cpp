#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "FeatureDetector.h"
#include <memory>

const std::string res_dir = "/Users/chengong.cg/Git/native/OpenCV/res/";

short V_351[256], U_179[256], V_86[256], U_443[256]; //查表数组

void table_init() {
    int i;
    int tmp;
    for(i = 0; i < 256; i++) {
        tmp = i - 128;

        V_351[i] = tmp * 351;

        U_179[i] = tmp * 179;

        V_86[i] = tmp * 86;

        U_443[i] = tmp * 443;
    }
}

void YUV2RGB_IYUV(unsigned char *iyuv, int width, int height) {
    int size = width * height;

    unsigned char *Y = NULL;
    unsigned char *V = NULL;
    unsigned char *U = NULL;
    unsigned char *rgb = (unsigned char *)malloc(size * 3);
    memset(rgb, 0, size * 3);

    int i = 0;
    Y = iyuv;
    U = iyuv + size;
    V = U + size / 4;
    for (i = 0; i < size; i++) {
        if ( i > 0) {
            Y = Y + 1;
            if (i % 4 == 0) {
                U = U + 1;
                V = V + 1;
            }
        }

        rgb[3 * i] = *Y + (V_351[*V] >> 8); // R
        rgb[3 * i + 1] = *Y - ((U_179[*U] - V_86[*V]) >> 8); // G
        rgb[3 * i + 2] = *Y + (U_443[*U] >> 8); // B
    }

    cv::Mat frame(height, width, CV_8UC3, (unsigned char *)rgb);
    cv::imwrite(res_dir + "myrgb.jpg", frame);

    free(rgb);
    rgb = NULL;
}

int main(int, char**) {
    // cv::Mat bgr = cv::imread(res_dir + "line.jpeg", 1);
    // int width = bgr.size().width;
    // int height = bgr.size().height;
    // std::cout << "image size(" << width << "," << height << ")\n";
    // cv::namedWindow("ImageWindow");
    // cv::imshow("ImageWindow", bgr);

    // cv::Mat iyuv;
    // cv::cvtColor(bgr, iyuv, CV_BGR2YUV_IYUV);
    // cv::imwrite(res_dir + "iyuv.jpg", iyuv);

    // int64 e1 = cv::getTickCount();
    // cv::Mat rgb;
    // cv::cvtColor(iyuv, rgb, CV_YUV2RGB_IYUV);
    // cv::imwrite(res_dir + "rgb.jpg", rgb);
    // int64 e2 = cv::getTickCount();
    // std::cout << "opencv cost " << e2 - e1 << std::endl;

    // e1 = cv::getTickCount();
    // table_init();
    // YUV2RGB_IYUV(iyuv.data, width, height);
    // e2 = cv::getTickCount();
    // std::cout << "my cost " << e2 - e1 << std::endl;

    // bgr.release();
    // iyuv.release();
    // rgb.release();


    // std::shared_ptr<MyFeatureDetector> spDetector = std::make_shared<MyFeatureDetector>();
    // spDetector->detect(DetectType::FAST);
    // spDetector->detect(DetectType::SIFT);
    return 0;
}
