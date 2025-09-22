#include "CommonAPI.h"

std::vector<std::string> CommonAPI::readClassNames(std::string classNamePath)
{
    std::vector<std::string> classNames;
    std::ifstream fp(classNamePath);
    if (!fp.is_open())
    {
        printf("could not open file...\n");
        exit(-1);
    }

    std::string name;
    while (!fp.eof())
    {
        getline(fp, name);
        if (name.length())
        {
            classNames.push_back(name);
        }
    }
    fp.close();

    return classNames;
}

trtModel CommonAPI::load_model(const char* trt_name)
{
    trtModel trt_model;
    std::ifstream infile(trt_name, std::ios::binary);
    if (infile.good())
    {
        infile.seekg(0, infile.end);
        trt_model.size = static_cast<size_t>(infile.tellg());
        infile.seekg(0, infile.beg);
        trt_model.model = static_cast<char*>(malloc(trt_model.size*sizeof(char)));
        infile.read(trt_model.model, static_cast<std::streamsize>(trt_model.size));
        infile.close();
    }
    return trt_model;
}
