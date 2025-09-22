#ifndef COMMONAPI_H
#define COMMONAPI_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <NvInfer.h>
#include <NvOnnxParser.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>

using namespace nvonnxparser;
using namespace nvinfer1;


typedef struct trtModel{
    size_t size = static_cast<size_t>(-1);
    char *model{nullptr};
}trtModel;

class Logger : public ILogger
{
    void  log(Severity severity, const char* msg) noexcept
    {
        if (severity <= Severity::kERROR)
        {
            std::cout << msg << std::endl;
        }
    }
};

class CommonAPI
{
public:
    static std::vector<std::string> readClassNames(std::string classNamePath);
    static trtModel load_model(const char* trt_name);
//    static float sigmoid_function(float a);
//    static void draw_pose_keyPoint(const float* data, cv::Mat &input_image)
};





#endif // COMMONAPI_H
