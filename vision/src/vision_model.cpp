#include "../vision/include/vision_model.h"

#define PI 3.1415926

using namespace vision_model;
using namespace cv;
using namespace std;

const bool SHOW_IMAGE = false;

VisionModel::VisionModel()
{

}

VisionModel::~VisionModel()
{

}

void VisionModel::process(Mat image, int function_num)
{
    switch (function_num) {
    case 1:
        detector1(image);
        break;
    case 2:
        detector2(image);
        break;
    case 3:
        detector3(image);
        break;
    case 4:
        detector4(image);
        break;
    default:
        break;
    }
}

void VisionModel::detector1(Mat image)
{
    Mat process_image;
    Mat lab_image;
    Mat gray_image;
    resizeTo720(image, process_image);

    cutGoalPosion(process_image, process_image);

    cvtColor(process_image,gray_image, COLOR_BGR2GRAY);

    cvtColor(process_image, lab_image, COLOR_BGR2Lab);

//    vector<Mat> rgb_channels;
    vector<Mat> lab_channels;

//    split(process_image, rgb_channels);
    split(lab_image, lab_channels);
    /*
     *  lab_channels[0] light 0->100
     *  lab_channels[1] greed->red 0-255
     *  lab_channels[2] blue->yellow 0-255
     */

    if (SHOW_IMAGE)
    {
        imshow("origion", process_image);
    //    imshow("blue", rgb_channels[0]);
    //    imshow("green", rgb_channels[1]);
    //    imshow("red", rgb_channels[2]);

        imshow("l", lab_channels[0]);
        imshow("a", lab_channels[1]);
        imshow("b", lab_channels[2]);
    }

    Mat red_button;
    Mat greed_button;
    Mat yellow_button;
    Mat test_image = gray_image - getBackgroud(gray_image);

    threshold(test_image, test_image, 0, 255.0, CV_THRESH_BINARY | THRESH_OTSU);

    // close
    medianBlur(test_image, test_image, 5);
    dilate(test_image, test_image, Mat(), Point(0, 0), 2);
    erode(test_image, test_image, Mat(), Point(0, 0), 2);

    erode(test_image, test_image, Mat(), Point(0, 0), 2);
    medianBlur(test_image, test_image, 3);
    medianBlur(test_image, test_image, 3);
    dilate(test_image, test_image, Mat(), Point(0, 0), 2);
//    erode(test_image, test_image, Mat(), Point(0, 0), 5);
//    dilate(test_image, test_image, Mat(), Point(0, 0), 3);

    threshold(lab_channels[1], red_button, 150, 255.0, CV_THRESH_BINARY);
    threshold(lab_channels[1], greed_button, 110, 255.0, CV_THRESH_BINARY);
    threshold(lab_channels[2], yellow_button, 180, 255.0, CV_THRESH_BINARY);

    bitwise_not(greed_button, greed_button);

    Mat result_image = red_button + greed_button + yellow_button + test_image;

    if (SHOW_IMAGE)
    {
//            imshow("red_button", red_button);
//            imshow("greed_button", greed_button);
//            imshow("yellow_button", yellow_button);
//            imshow("l_back_image", l_back_image);
//            imshow("test_image", test_image);
//            imshow("l-b", lab_channels[0] - lab_channels[2]);
            imshow("result_image", result_image);
    }

    splitResult(result_image);

//    result_image_ = result_image.clone();
    if (SHOW_IMAGE)
    {
        cvWaitKey(-1);
    }

    result_image_ = result_image.clone();
}

void VisionModel::detector1_1(Mat image)
{
    Mat process_image;
    Mat lab_image;
    Mat gray_image;
    resizeTo720(image, process_image);

    cvtColor(process_image,gray_image, COLOR_BGR2GRAY);

    cvtColor(process_image, lab_image, COLOR_BGR2Lab);

//    vector<Mat> rgb_channels;
    vector<Mat> lab_channels;

//    split(process_image, rgb_channels);
    split(lab_image, lab_channels);
    /*
     *  lab_channels[0] light 0->100
     *  lab_channels[1] greed->red 0-255
     *  lab_channels[2] blue->yellow 0-255
     */

    if (SHOW_IMAGE)
    {
        imshow("origion", process_image);
    //    imshow("blue", rgb_channels[0]);
    //    imshow("green", rgb_channels[1]);
    //    imshow("red", rgb_channels[2]);

        imshow("l", lab_channels[0]);
        imshow("a", lab_channels[1]);
        imshow("b", lab_channels[2]);
    }

    Mat red_button;
    Mat greed_button;
    Mat yellow_button;

    threshold(lab_channels[1], red_button, 150, 255.0, CV_THRESH_BINARY | THRESH_OTSU);
    threshold(255 - lab_channels[1] - mean(lab_channels[1])(0), greed_button, 110, 255.0, CV_THRESH_BINARY | THRESH_OTSU);
    threshold(lab_channels[2], yellow_button, 180, 255.0, CV_THRESH_BINARY | THRESH_OTSU);

    if (whiteRatio(red_button) > 0.5)
    {
        bitwise_not(red_button, red_button);
    }

    if (whiteRatio(greed_button) > 0.5)
    {
        bitwise_not(greed_button, greed_button);
    }

    if (whiteRatio(yellow_button) > 0.5)
    {
        bitwise_not(yellow_button, yellow_button);
    }

    Mat colour_button = red_button + greed_button + yellow_button;

    Mat white_button = gray_image - getBackgroud(gray_image);

    threshold(white_button, white_button, 0, 255.0, CV_THRESH_BINARY | THRESH_OTSU);

    // close
    medianBlur(white_button, white_button, 5);
    dilate(white_button, white_button, Mat(), Point(0, 0), 2);
    erode(white_button, white_button, Mat(), Point(0, 0), 2);

    erode(white_button, white_button, Mat(), Point(0, 0), 2);
    medianBlur(white_button, white_button, 3);
    medianBlur(white_button, white_button, 3);
    dilate(white_button, white_button, Mat(), Point(0, 0), 2);

    Mat result_image = colour_button + white_button;

    vector<int> horizontal_data(0);
    vector<int> vertical_data(0);

    int horizontal_min_site = horizontalProjection(result_image, horizontal_data, 1);
    int vertical_min_site = verticalProjection(result_image, vertical_data, 1);

    if (SHOW_IMAGE)
    {
        drawHorizontalProjection("test1", horizontal_data, result_image, 10);
    //    drawVerticalProjection(vert);
        printf("horizontal_min_site: %d\n", horizontal_min_site);
        fflush(stdout);
    }

    Rect process_rect(vertical_min_site, horizontal_min_site, vertical_data.size(), horizontal_data.size());

    Mat result_image2(result_image, process_rect);

    if (SHOW_IMAGE)
    {
            imshow("red_button", red_button);
            imshow("greed_button", greed_button);
            imshow("yellow_button", yellow_button);
            imshow("white_button", white_button);
            imshow("colour_button", colour_button);
            imshow("result_image", result_image);
            imshow("result_image2", result_image2);
    }
    splitResult(result_image2, horizontal_data, vertical_data);

    if (SHOW_IMAGE)
    {
        cvWaitKey(-1);
    }

    result_image_ = result_image2.clone();
}

void VisionModel::detector2(Mat image)
{
    Mat process_image;
    Mat gray_image;
    resizeTo720(image, process_image);

    cvtColor(process_image, gray_image, COLOR_BGR2GRAY);

    vector<Wave> horizontal_pro_wave(0);
    vector<Wave> vertical_pro_wave(0);

    // delete background and calculate projection
    Mat delete_back_image = gray_image - getBackgroud(gray_image);
    if (SHOW_IMAGE)
    {
        imshow("delete_back_image", delete_back_image);
        cvWaitKey(0);
    }
    threshold(delete_back_image, delete_back_image, 0, 255.0, CV_THRESH_BINARY | THRESH_OTSU);
    erode(delete_back_image, delete_back_image, Mat(), Point(0, 0), 1);
    dilate(delete_back_image, delete_back_image, Mat(), Point(0, 0), 2);

    vector<int> horizontal_data(0);
    vector<int> vertical_data(0);
    horizontalProjection(delete_back_image, horizontal_data);
    verticalProjection(delete_back_image, vertical_data);

    cutWave(horizontal_data, vertical_data, horizontal_pro_wave, vertical_pro_wave);

//    for (int i = 0; i < horizontal_pro_wave.size(); i++)
//    {
//        for (int j = 0; j < vertical_pro_wave.size(); j++)
//        {
//            char title[16];
//            sprintf("cut[%d]", i*horizontal_pro_wave.size()+j);
//            int x = vertical_pro_wave[i].wave_start;
//            int y = horizontal_pro_wave[i].wave_start;
//            int width = vertical_pro_wave[i].wave_end - vertical_pro_wave[i].wave_start;
//            int height = horizontal_pro_wave[i].wave_end - horizontal_pro_wave[i].wave_start;
//            Rect cut_rect(x, y, width, height);
//            Mat cut_image = Mat()
//            imshow(title, );
//        }
//    }

//    cvWaitKey(-1);
}

void VisionModel::detector3(Mat image)
{
    Mat process_image = image.clone();
    Mat lab_image;

    cvtColor(process_image, lab_image, COLOR_BGR2Lab);

    vector<Mat> lab_channels;
    split(lab_image, lab_channels);

    Mat red_image = lab_channels[1];
    Mat back_image(red_image.rows, red_image.cols, CV_8UC1, Scalar(0,0,0));

    Mat result;
    cvtColor(red_image, result, CV_GRAY2BGR);

    threshold(red_image, red_image, 0, 255, CV_THRESH_BINARY | THRESH_OTSU);

    vector<vector<Point>> contours;

    findContours(red_image, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

    double max_area = 0;
    int max_index = 0;

    for (size_t i = 0; i < contours.size(); i++)
    {
        double area = contourArea(contours[i]);
        if (area > max_area)
        {
            max_area = area;
            max_index = i;
        }
    }

    drawContours(back_image, contours, max_index + 1, Scalar(255, 0, 255), -1);
    erode(back_image, back_image, Mat(), Point(0, 0), 3);

    Rect max_rect = boundingRect(contours[max_index+1]);

    Mat process_area(process_image, max_rect);
    Mat back_area(back_image, max_rect);
    Mat goal_image;
    Mat goal_image2;
    Mat patch_image;
    cvtColor(back_area, back_area, COLOR_GRAY2BGR);
    bitwise_and(process_area, back_area, goal_image);
    bitwise_not(back_area, back_area);
    Mat goal_image_average(process_area.rows, process_area.cols, goal_image.type(), mean(process_image));
    bitwise_and(goal_image_average, back_area, patch_image);
    goal_image2 = goal_image + patch_image;

    detector1_1(goal_image2);
    if (SHOW_IMAGE)
    {
        imshow("red_image", red_image);
        imshow("goal_image2", goal_image2);
        imshow("goal_image", goal_image);
        imshow("goal_image_average", goal_image_average);
        imshow("back_image", back_image);
        cvWaitKey(-1);
    }
}

void VisionModel::detector4(Mat image)
{
    Mat process_image = image.clone();
    Mat lab_image;
    Mat gray_image;

    cutRedBoxImage(process_image);

    resizeTo720(process_image, process_image);

    cvtColor(process_image,gray_image, COLOR_BGR2GRAY);

    cvtColor(process_image, lab_image, COLOR_BGR2Lab);

//    vector<Mat> rgb_channels;
    vector<Mat> lab_channels;

//    split(process_image, rgb_channels);
    split(lab_image, lab_channels);
    /*
     *  lab_channels[0] light 0->100
     *  lab_channels[1] greed->red 0-255
     *  lab_channels[2] blue->yellow 0-255
     */

    if (SHOW_IMAGE)
    {
        imshow("origion", process_image);

        imshow("l", lab_channels[0]);
        imshow("a", lab_channels[1]);
        imshow("b", lab_channels[2]);
    }

    Mat yellow_button;

    threshold(lab_channels[2], yellow_button, 0, 255, CV_THRESH_BINARY | THRESH_OTSU);

    vector<int> horizontal_data;
    vector<int> vertical_data;

    horizontalProjection(yellow_button, horizontal_data);
    verticalProjection(yellow_button, vertical_data);

    drawHorizontalProjection("test1", horizontal_data, yellow_button, 10);
    drawVerticalProjection( vertical_data, yellow_button);

    Mat test = gray_image - getBackgroud(gray_image);
    Mat test2;
    threshold(test, test2, 0, 255, CV_THRESH_BINARY | THRESH_OTSU);

    if (SHOW_IMAGE)
    {
            imshow("test", test);
            imshow("test2", test2);
            imshow("yellow_button", yellow_button);
    }

    if (SHOW_IMAGE)
    {
        cvWaitKey(-1);
    }
}

void VisionModel::cutGoalPosion(Mat input_image, Mat &output_image)
{
    Mat process_image;
    cvtColor(input_image, process_image, COLOR_BGR2GRAY);

    Mat delete_back_image = process_image - getBackgroud(process_image);

    threshold(delete_back_image, delete_back_image, 0, 255.0, CV_THRESH_BINARY | THRESH_OTSU);
    erode(delete_back_image, delete_back_image, Mat(), Point(0, 0), 1);
    dilate(delete_back_image, delete_back_image, Mat(), Point(0, 0), 2);

    vector<int> horizontal_data;
    vector<int> vertical_data;
    horizontalProjection(delete_back_image, horizontal_data);
    verticalProjection(delete_back_image, vertical_data);

//    drawVerticalProjection(vertical_data, delete_back_image);

    int average = 0;
    int first_trough = getFirstTrough(horizontal_data, average);

//    drawHorizontalProjection("Horizontal projImg1", horizontal_data, delete_back_image, average);

    int vertical_min_site = -1;
    int vertical_max_site = -1;
    int vertical_filter   = 50;

    for (int i = 0; i < vertical_data.size(); i++)
    {
        if (vertical_data[i] > vertical_filter)
        {
            if (vertical_min_site == -1)
            {
                vertical_min_site = i;
            }

            vertical_max_site = i;
        }
    }

    int horizontal_max_site = -1;
    int horizontal_filter   = 10;

    for (int i = 0; i < horizontal_data.size(); i++)
    {
        if (horizontal_data[i] > horizontal_filter)
        {

            horizontal_max_site = i;
        }
    }

    Rect cut_rect(vertical_min_site, first_trough, vertical_max_site - vertical_min_site, horizontal_max_site - first_trough);

    Mat image_cut = Mat(input_image, cut_rect);

    output_image = image_cut.clone();
}

void VisionModel::cutWave(vector<int> horizontal_data, vector<int> vertical_data, vector<Wave> &horizontal_pro_wave, vector<Wave> &vertical_pro_wave)
{

    splitWave(horizontal_data, horizontal_pro_wave);
    splitWave(vertical_data, vertical_pro_wave);
}

void VisionModel::imshowResult(Mat image)
{

}

Mat VisionModel::loadImage(string image_path)
{
    Mat image = imread(image_path);
    wait_process_image_ = image.clone();
    return image;
}

Mat VisionModel::getBackgroud(Mat image)
{
    Mat back_groud;

    erode(image, back_groud, Mat(), Point(0, 0), 6);
    dilate(back_groud, back_groud, Mat(), Point(0, 0), 6);

    return back_groud;
}

Mat VisionModel::getResultImage()
{
    return result_image_;
}

void VisionModel::cutRedBoxImage(Mat &image)
{
    Mat process_image = image.clone();
    Mat lab_image;

    cvtColor(process_image, lab_image, COLOR_BGR2Lab);

    // split to lab
    vector<Mat> lab_channels;
    split(lab_image, lab_channels);

    Mat red_image = lab_channels[1];

    // create black back_image
    Mat back_image(red_image.rows, red_image.cols, CV_8UC1, Scalar(0,0,0));

    threshold(red_image, red_image, 0, 255, CV_THRESH_BINARY | THRESH_OTSU);

    // get contours
    vector<vector<Point>> contours;

    findContours(red_image, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

    // get max area and index
    double max_area = 0;
    int max_index = 0;

    for (size_t i = 0; i < contours.size(); i++)
    {
        double area = contourArea(contours[i]);
        if (area > max_area)
        {
            max_area = area;
            max_index = i;
        }
    }

    // draw goal area on back_image
    drawContours(back_image, contours, max_index + 1, Scalar(255, 0, 255), -1);
    erode(back_image, back_image, Mat(), Point(0, 0), 3);

    // cut
    Rect max_rect = boundingRect(contours[max_index+1]);
    Mat process_area(process_image, max_rect);
    Mat back_area(back_image, max_rect);
    Mat goal_image;
    Mat patch_image;

    cvtColor(back_area, back_area, COLOR_GRAY2BGR);
    bitwise_and(process_area, back_area, goal_image);
    bitwise_not(back_area, back_area);
    Mat goal_image_average(process_area.rows, process_area.cols, goal_image.type(), mean(process_image));
    bitwise_and(goal_image_average, back_area, patch_image);
    Mat new_image = goal_image + patch_image;

    image = new_image.clone();
}

vector<vector<int>> VisionModel::getResultInfo()
{
    return result_info_;
}

void VisionModel::resizeTo720(Mat input_image, Mat &output_image)
{
    resize(input_image, output_image, Size(720, 720*input_image.rows/input_image.cols));
}

void VisionModel::resizeTo480(Mat input_image, Mat &output_image)
{
    resize(input_image, output_image, Size(480, 480*input_image.rows/input_image.cols));
}

double VisionModel::filterdMean(vector<int> data, int filter)
{
    int sum = 0;
    int count = 0;

    for (int i = 0; i < data.size(); i++)
    {
        if (data[i] > filter)
        {
            sum += data[i];
            count++;
        }
    }

    return 1.0 * sum / count;
}

void VisionModel::splitWave(vector<int> data, vector<Wave>& wave)
{
    wave.resize(0);
    wave.clear();

    int size =data.size();

    // split wave
    while (true)
    {
        int max_have_value = 0;
        int max_not_value = 0;

        for (int i = 0; i < size; i++)
        {
            if (data[i] == 0)
            {
                max_not_value = i;
            }
            else
            {
                max_have_value = i;
            }
        }

        if (max_not_value > max_have_value)
        {
            size = max_have_value;
        }
        else
        {
            size = max_not_value;

            Wave new_wave;
            new_wave.wave_start = max_not_value;
            new_wave.wave_end = max_have_value;
            wave.push_back(new_wave);
        }

        if (max_have_value == 0 || max_not_value == 0)
            break;
    }
}

int VisionModel::filterData(vector<int> &data)
{
    int sum_value = 0;
    int sum_count = 0;
    int data_mean;
    int min_site;

    for (int i = 0; i < data.size(); i++)
    {
        if (data[i] > 10)
        {
            sum_value += data[i];
            sum_count++;
        }
    }

    data_mean = sum_value/sum_count;

    for (int i = 0; i < data.size(); i++)
    {
        if (data[i] < data_mean)
        {
            min_site = i;
        }
        else
        {
            break;
        }
    }

    for (; min_site >= 0; min_site--)
    {
        if (data[min_site] < 10)
            break;
    }

    int new_size =  data.size();

    for (; new_size >= 0; new_size--)
    {
        if (data[new_size] > data_mean)
            break;
    }

    for (; new_size < data.size(); new_size++)
    {
        if (data[new_size] < 10)
            break;
    }

    vector<int> data2(data.begin() + min_site, data.begin() + new_size);
    data.clear();
    data.resize(new_size - min_site);
    data.assign(data2.begin(), data2.end());
    return min_site;
}

Rect VisionModel::maxInscribedRec(Mat image)
{

}

void VisionModel::splitResult(Mat image)
{
    if (result_info_.size() > 0)
        result_info_.clear();

    vector<int> horizontal_data;
    vector<int> vertical_data;

    vector<Wave> horizontal_pro_wave;
    vector<Wave> vertical_pro_wave;

    horizontalProjection(image, horizontal_data);
    verticalProjection(image, vertical_data);

    cutWave(horizontal_data, vertical_data, horizontal_pro_wave, vertical_pro_wave);

    if (SHOW_IMAGE)
    {
        drawHorizontalProjection("test1", horizontal_data, image, 10);
        drawVerticalProjection( vertical_data, image);

        printf("horizontal_pro_wave.size(): %d\n", horizontal_pro_wave.size());
        printf("vertical_pro_wave.size(): %d\n", vertical_pro_wave.size());
    }

    for (int i = horizontal_pro_wave.size() - 1; i >= 0; i--)
    {
        vector<int> row_result;
        for (int j = vertical_pro_wave.size() - 1; j >= 0; j--)
        {
            int y = horizontal_pro_wave[i].wave_start;
            int x = vertical_pro_wave[j].wave_start;

            int height = horizontal_pro_wave[i].wave_end - horizontal_pro_wave[i].wave_start;
            int width = vertical_pro_wave[j].wave_end - vertical_pro_wave[j].wave_start;

            Rect cut_rect(x, y, width, height);
            Mat cut_image = Mat(image, cut_rect);

            int result = matchResult(cut_image);
            printf("%5d", result);
            row_result.push_back(result);

            if (SHOW_IMAGE)
            {
                char title[16];
                sprintf(title, "%d", i*vertical_pro_wave.size()+j);
                imshow(title, cut_image);
            }
        }
        printf("\n");
        result_info_.push_back(row_result);
    }
    fflush(stdout);
}

void VisionModel::splitResult(Mat image, vector<int> horizontal_data, vector<int> vertical_data)
{
    vector<Wave> horizontal_pro_wave;
    vector<Wave> vertical_pro_wave;

    cutWave(horizontal_data, vertical_data, horizontal_pro_wave, vertical_pro_wave);

    if (SHOW_IMAGE)
    {
        drawHorizontalProjection("test1", horizontal_data, image, 10);
        drawVerticalProjection( vertical_data, image);
    }

    printf("horizontal_pro_wave.size(): %d\n", horizontal_pro_wave.size());
    printf("vertical_pro_wave.size(): %d\n", vertical_pro_wave.size());

    for (int i = horizontal_pro_wave.size() - 1; i >= 0; i--)
    {
        vector<int> row_result;
        for (int j = vertical_pro_wave.size() - 1; j >= 0; j--)
        {
            int y = horizontal_pro_wave[i].wave_start;
            int x = vertical_pro_wave[j].wave_start;

            int height = horizontal_pro_wave[i].wave_end - horizontal_pro_wave[i].wave_start;
            int width = vertical_pro_wave[j].wave_end - vertical_pro_wave[j].wave_start;

            Rect cut_rect(x, y, width, height);
            Mat cut_image = Mat(image, cut_rect);

            int result = matchResult(cut_image);
            printf("%5d", result);
            row_result.push_back(result);

            if (SHOW_IMAGE)
            {
                char title[16];
                sprintf(title, "%d", i*vertical_pro_wave.size()+j);
                imshow(title, cut_image);
            }
        }
        printf("\n");
        result_info_.push_back(row_result);
    }
    fflush(stdout);
}

int VisionModel::matchResult(Mat& image)
{
    int result = -1;
    static int t = 0;
    t++;
    Mat contours;
    Canny(image, contours, 110, 110);

    vector<Vec4i> lines;
    HoughLinesP(contours, lines, 1, CV_PI/180, 15, 15, 10);

    for (size_t i = 0; i < lines.size(); i++)
    {
        Mat dst;
        cvtColor(image, dst, CV_GRAY2BGR);
        Vec4i l = lines[i];
        line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);

        double k = 1.0 * (l[3] - l[1])/(l[2] - l[0]);
        char ti[32];
        sprintf(ti, "result/%d_%f.jpg", t, k);
        imwrite(ti, dst);
//        printf("test: %s\n", ti);
        if(k > 0.2 && k < 10)
        {
                return 0;
        }
        else
        {
            result = 1;
        }
    }

    return result;
}

double VisionModel::calculateSlope(vector<int> data)
{
    // calculate average
    double _x = (1 + data.size()) / 2;
    double _y = 0;
    double sum_x_y_i = 0;
    double sum_x_x_i = 0;
    for (int i = 0; i < data.size(); i++)
    {
        data[i] -= data[0];
        _y += data[i];
        sum_x_y_i += i *  data[i];
        sum_x_x_i += i * i;
    }

    _y = _y / data.size();

    return 0;
}

double VisionModel::whiteRatio(Mat image)
{
    Mat_<uchar>::iterator it = image.begin<uchar>();

    int sum = 0;
    int white_sum = 0;

    for (; it != image.end<uchar>(); it++)
    {
        sum++;
        if (*it > 0)
        {
            white_sum++;
        }
    }

    return 1.0 * white_sum / sum;
}

int VisionModel::horizontalProjection(Mat image, vector<int>& data, int cut)
{
    data.resize(image.rows, 0);

    for (int row = 0; row < image.rows; row++)
    {
        for (int col = 0; col < image.cols; col++)
        {
            if (image.at<uchar>(row, col) > 1)
                data[row]++;
        }
    }
    if (cut == -1)
        return -1;

    return filterData(data);
}

int VisionModel::verticalProjection(Mat image, vector<int>& data, int cut)
{
     data.resize(image.cols, 0);

    for (int col = 0; col < image.cols; col++)
    {
        for (int row = 0; row < image.rows; row++)
        {
            if (image.at<uchar>(row, col) > 1)
                data[col]++;
        }
    }

    if (cut == -1)
        return -1;

    return filterData(data);
}

vector<int> VisionModel::calculateValleyNum(vector<int>)
{

}

void VisionModel::drawHorizontalProjection(string name, vector<int> data_vector, Mat origion_image, int filter)
{
//    filter = 10;
    Mat projImg(origion_image.rows, origion_image.cols, CV_8U, Scalar(255));
    for (int i = 0; i < origion_image.rows; ++i)
    {
        line(projImg, Point(origion_image.cols -data_vector[i], i), Point(origion_image.cols - 1, i), Scalar::all(0));
        projImg.at<uchar>(i, origion_image.cols - filter) = 0;
    }
    imshow(name, projImg);
}

void VisionModel::drawVerticalProjection(vector<int> data_vector, Mat origion_image)
{
    printf("size: %d, cols: %d\n", data_vector.size(), origion_image.cols);
    Mat projImg(origion_image.rows, origion_image.cols, CV_8U, Scalar(255));
    for (int i = 0; i < origion_image.cols; ++i)
    {
        line(projImg, Point(i,  origion_image.rows - data_vector[i]), Point(i,  origion_image.rows - 1), Scalar::all(0));
    }

    imshow("vertical projImg", projImg);
}

int VisionModel::getFirstTrough(vector<int> data_vector, int& average)
{
    int sum_count = 0;
    int sum = 0;
    int max = 0;

    for (int i = 0; i < data_vector.size(); i++)
    {
        if (data_vector[i] > 10)
        {
            sum += data_vector[i];
            sum_count++;

            if (data_vector[i] > max)
                max = data_vector[i];
        }
    }

    int result = 0;
    average = sum/sum_count;
    if (max > 2 * average)
    {
        int k = (max + average) / 2;

        int k_max_site = 0;
        for (int i = 0; i < data_vector.size(); i++)
        {
            if (data_vector[i] > k)
            {
                k_max_site++;
            }
        }

        for (k_max_site; data_vector[k_max_site] > average; k_max_site++);

        result = k_max_site;
    }
    return result;
}
