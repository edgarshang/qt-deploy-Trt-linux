#ifndef YOLOV5_H
#define YOLOV5_H
#include "Interface.h"
#include <NvInfer.h>
#include <NvOnnxParser.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include "CommonAPI.h"

class Yolov5 : public Inference
{
public:
    Yolov5(QString modePath, QString labeTxt);
    virtual void imageHandleInference(cv::Mat &image);


private:
    std::string model_path;
    std::string label_path;
    std::vector<std::string> labels;


    std::vector<std::string> input_node_names;
    std::vector<std::string> output_node_names;

    std::unique_ptr<nvinfer1::ICudaEngine> m_engine;
    std::unique_ptr<nvinfer1::IRuntime> m_runtime;
    std::unique_ptr<nvinfer1::IExecutionContext> m_context;
    Logger m_logger;
    cudaStream_t stream;

    int inputsize = 1;
    int outputsize = 1;
    float *input = nullptr;
    float *output = nullptr;
    void *buffers[2] = {NULL, NULL};
    float x_factor = 0.0;
    float y_factor = 0.0;
    int input_w = 0;
    int input_h = 0;
    int out_num = 0;
    int out_ch = 0;
    int64_t start_time;
    int64_t end_time;


private:
    cv::Mat preprocess(const cv::Mat& image);
    void postprocess(cv::Mat& image);
    void infer(const cv::Mat& image);

//    IBuilder *m_builder =  nullptr;
};

#endif // YOLOV5_H
