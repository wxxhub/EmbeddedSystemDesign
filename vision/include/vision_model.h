#ifndef _VISION_MODEL_H_
#define _VISION_MODEL_H_

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>

namespace vision_model
{

struct Wave
{
    int wave_start;
    int wave_end;
};

class VisionModel
{
public:
    VisionModel();
    ~VisionModel();

    void process(cv::Mat image, int function_num);

    void imshowResult(cv::Mat image);

    cv::Mat loadImage(std::string path);
    cv::Mat getBackgroud(cv::Mat image);
    cv::Mat getResultImage();
    void cutRedBoxImage(cv::Mat &image);

    //get function
    std::vector<std::vector<int>> getResultInfo();

    // tool function
    void resizeTo720(cv::Mat input_image, cv::Mat &output_image);
    void resizeTo480(cv::Mat input_image, cv::Mat &output_image);

    double filterdMean(std::vector<int> data, int filter = 10);
    double calculateSlope(std::vector<int> data);
    double whiteRatio(cv::Mat image);

    void splitWave(std::vector<int> data, std::vector<Wave>& wave);
    int filterData(std::vector<int>& data);

    cv::Rect maxInscribedRec(cv::Mat image);
private:
    cv::Mat wait_process_image_;
    cv::Mat result_image_;

    std::vector<std::vector<int>> result_info_;

    // detector
    void detector1(cv::Mat image);
    void detector1_1(cv::Mat image);
    void detector2(cv::Mat image);
    void detector3(cv::Mat image);
    void detector4(cv::Mat image);

    void cutGoalPosion(cv::Mat input_image, cv::Mat &output_image);
    void cutWave(std::vector<int> horizontal_data, std::vector<int> vertical_data, std::vector<Wave>& horizontal_pro_wave,  std::vector<Wave>& vertical_pro_wave);

    void splitResult(cv::Mat image);
    void splitResult(cv::Mat image, std::vector<int> horizontal_data, std::vector<int> vertical_data);

    int matchResult(cv::Mat& image);

    // 计算水平投影
    int horizontalProjection(cv::Mat image, std::vector<int>& data, int cut = -1);

    // 计算竖直投影
    int verticalProjection(cv::Mat image, std::vector<int>& data, int cut = -1);

    std::vector<int> calculateValleyNum(std::vector<int>);

    void drawHorizontalProjection(std::string name, std::vector<int> data_vector, cv::Mat origion_image, int filter);
    void drawVerticalProjection(std::vector<int> data_vector, cv::Mat origion_image);

    int getFirstTrough(std::vector<int> data_vector, int& average);
};

}

#endif /*_VISION_MODEL_H_ */
