#ifndef RECOGNITION_H
#define RECOGNITION_H

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include "classification-c.h"
using namespace std;
//#pragma warning(disable : 4996)
//#pragma comment(lib, "classification_dll.lib")

vector<char> readFile(const char* file){
    vector<char> data;
    FILE* f = fopen(file, "rb");
    if (!f) return data;

    int len = 0;
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (len > 0){
        data.resize(len);
        fread(&data[0], 1, len, f);
    }
    fclose(f);
    return data;
}

vector<string> loadCodeMap(const char* file){
    ifstream infile(file);
    string line;
    vector<string> out;
    while (std::getline(infile, line)){
        out.push_back(line);
    }
    return out;
}

string getLabel(const vector<string>& labelMap, int index){
    if (index < 0 || index >= labelMap.size())
        return "*";

    return labelMap[index];
}

string recognition(string& picture)
{
    //string picture = "C:\\Users\\wuzhenguang\\Pictures\\黑底白字验证码\\4SU5G_11586.png";
    //禁止caffe输出信息
    disableErrorOutput();
    vector<char> data = readFile(picture.c_str());
    //注意目录是相对工程上级目录的
    Classifier* classif = createClassifier("deploy.prototxt", "_iter_122659.caffemodel");
    forward(classif, &data[0], data.size());
    vector<string> labelMap = loadCodeMap("label-map.txt");

    BlobData* premuted_fc = getBlobData(classif, "premuted_fc");
    float* ptr = premuted_fc->list;
    int blank_label = 32;
    int time_step = 19;
    int alphabet_size = 33;
    int prev_label = blank_label;
    string result, result_raw;

    for (int i = 0; i < time_step; ++i){
        float* lin = ptr + i * alphabet_size;
        int predict_label = std::max_element(lin, lin + alphabet_size) - lin;
        //float value = lin[predict_label];

        if (predict_label != blank_label && predict_label != prev_label){
            result = result + getLabel(labelMap, predict_label);
        }

        result_raw = result_raw + getLabel(labelMap, predict_label);
        prev_label = predict_label;
    }

    releaseBlobData(premuted_fc);
    //printf("识别的结果是：\n%s\n", result.c_str());
    //printf("识别的结果是：\n%s\n%s\n", result.c_str(), result_raw.c_str());
    releaseClassifier(classif);

    return result;
}
#endif // RECOGNITION_H
