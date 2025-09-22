#include "Yolov5.h"
#include "CommonAPI.h"

Yolov5::Yolov5(QString modePath, QString labeTxt)
{
    qDebug() << "modePaht : " << modePath;
    qDebug() << "labeTxt : " << labeTxt;
      // init the model
    label_path = labeTxt.toLocal8Bit().toStdString();
    model_path = modePath.toLocal8Bit().toStdString();
    labels = CommonAPI::readClassNames(label_path);


    trtModel trt_model = CommonAPI::load_model(model_path.c_str());

     m_runtime.reset(createInferRuntime(m_logger));
     m_engine.reset(m_runtime->deserializeCudaEngine(trt_model.model, trt_model.size));
     m_context.reset(m_engine->createExecutionContext());
     cudaStreamCreate(&stream);

     int32_t num =  m_engine->getNbBindings();

     for(int i = 0; i < num; i++)
     {
            std::cout << "the name is " << m_engine->getBindingName(i) << std::endl;
            if(m_engine->bindingIsInput(i))
            {
                 Dims inputDims = m_engine->getBindingDimensions(i);
                 for(int j = 0; j < inputDims.nbDims; j++)
                 {
                     std::cout << inputDims.d[j];
                     inputsize *= inputDims.d[j];
                     if(j == 2)
                     {
                         input_w = inputDims.d[j];
                     }
                     if(j == 3)
                     {
                         input_h = inputDims.d[j];
                     }
                     if(j == inputDims.nbDims - 1)
                         break;
                     std::cout << "x";
                 }
                 std::cout << std::endl;
            }else
            {
                Dims outputDims = m_engine->getBindingDimensions(i);
                for(int j = 0; j < outputDims.nbDims; j++)
                {
                    std::cout << outputDims.d[j];
                    outputsize *= outputDims.d[j];
                    if(j == 1)
                    {
                        out_num = outputDims.d[j];
                    }
                    if(j == 2)
                    {
                        out_ch = outputDims.d[j];
                    }
                    if(j == outputDims.nbDims - 1)
                        break;
                    std::cout << "x";
                }
                std::cout << std::endl;
            }
     }

     std::cout << "inputSize is " << inputsize << std::endl;
     std::cout << "outputSize is " << outputsize << std::endl;

     // cudaMallocHost
    cudaMallocHost(&input, inputsize*sizeof(float));
    cudaMallocHost(&output, outputsize * sizeof(float));

    cudaMalloc(&buffers[0], inputsize * sizeof(float));
    cudaMalloc(&buffers[1], outputsize * sizeof(float));
}

cv::Mat Yolov5::preprocess(const cv::Mat& image)
{
       int w = image.cols;
        int h = image.rows;

        int _max = std::max(h,w);

        cv::Mat image_m = cv::Mat::zeros(cv::Size(_max, _max), CV_8UC3);
        cv::Rect roi(0,0,w,h);
        image.copyTo(image_m(roi));


        x_factor = image_m.cols / static_cast<float>(input_w);
        y_factor = image_m.rows / static_cast<float>(input_h);

        cv::Mat blob = cv::dnn::blobFromImage(image_m, 1.0/255.0, cv::Size(input_w, input_h),
                                              cv::Scalar(0,0,0), true, true);

        return blob;
}
void Yolov5::postprocess(cv::Mat& image)
{
    float *pdata = output;

        std::vector<cv::Rect> boxes;
        std::vector<int> classIds;
        std::vector<float> confidences;

        // 1x25200x85

        cv::Mat det_output(out_num, out_ch, CV_32F, (float*)pdata);

        for(int i = 0; i < det_output.rows; i++)
        {
            float conf = det_output.at<float>(i,4);
            if(conf < 0.45)
            {
                continue;
            }

            cv::Mat classes_scores = det_output.row(i).colRange(5, out_ch);
            cv::Point classIdPoint;
            double score;
            cv::minMaxLoc(classes_scores, 0, &score, 0, &classIdPoint);

            // 置信度0-1之间
            if( score > 0.25)
            {
                float cx = det_output.at<float>(i,0);
                float cy = det_output.at<float>(i,1);
                float ow = det_output.at<float>(i,2);
                float oh = det_output.at<float>(i,3);

                int x = static_cast<int>((cx - 0.5*ow) * x_factor);
                int y = static_cast<int>((cy - 0.5*oh) * y_factor);
                int width = static_cast<int>(ow*x_factor);
                int height = static_cast<int>(oh*y_factor);

                cv::Rect box;
                box.x = x;
                box.y = y;
                box.width = width;
                box.height = height;

                boxes.push_back(box);
                classIds.push_back(classIdPoint.x);
                confidences.push_back(score);
            }
        }

        // NMS
        std::vector<int> indexes;
        cv::dnn::NMSBoxes(boxes, confidences, (float)(0.25), (float)(0.45), indexes);
        for(size_t i = 0; i < indexes.size(); i++)
        {
//            std::cout << "the i is " << i << std::endl;
            int idx = indexes[i];
            int cid = classIds[idx];
            cv::rectangle(image, boxes[idx], cv::Scalar(0,0,255), 2, 8,0);
            cv::putText(image, cv::format("%s_%.2f", labels[cid].c_str(), confidences[idx]) , boxes[idx].tl(),
                        cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0,255,0), 2, 8);
        }

        float t = (end_time - start_time) / static_cast<float>(cv::getTickFrequency());
        cv::putText(image, cv::format("FPS: %.2f", 1.0/t), cv::Point(20,40), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(255, 0, 0), 2, 8);

}
void Yolov5::infer(const cv::Mat& image)
{
    memcpy(input, image.ptr<float>(), inputsize * sizeof(float));

    start_time = cv::getTickCount();
    cudaMemcpyAsync(buffers[0], input, inputsize*sizeof(float), cudaMemcpyHostToDevice, stream);
    m_context->enqueueV2(buffers, stream, nullptr);
    cudaStreamSynchronize(stream);
    cudaMemcpyAsync(output, buffers[1], outputsize*sizeof(float), cudaMemcpyDeviceToHost, stream);

    end_time = cv::getTickCount();

//    float t = (end_time - start_time) / static_cast<float>(cv::getTickFrequency());
//    std::cout << "the fps is " << 1.0/t << std::endl;

}

void Yolov5::imageHandleInference(cv::Mat &image)
{
//  qDebug() << "inference..";
  cv::Mat blob =  this->preprocess(image);
  this->infer(blob);
  this->postprocess(image);
}
