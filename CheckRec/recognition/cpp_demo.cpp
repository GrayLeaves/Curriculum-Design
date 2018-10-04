#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include "classification-c.h"
using namespace std;
#pragma warning(disable : 4996)
#pragma comment(lib, "classification_dll.lib")

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

int main()
{
	string picture = "C:\\Users\\wuzhenguang\\Pictures\\�ڵװ�����֤��\\4SU5G_11586.png";
	//��ֹcaffe������Ϣ
	disableErrorOutput();

	//ע��Ŀ¼�����Թ����ϼ�Ŀ¼��
	Classifier* classif = createClassifier("deploy.prototxt", "_iter_122659.caffemodel");
	//const char* imageFile = "4SU5G_11586.png";
	
	/*cout << "��������ʶ����ͼƬ�ļ������ںڰ���֤��Ŀ¼����������չ��:" << endl;
	string fileName;
	cin >> fileName;
	string picture = path + fileName + string(".png");
	cout << picture << endl;*/
	vector<string> labelMap = loadCodeMap("label-map.txt");
	vector<char> data = readFile(picture.c_str());
	//vector<char> data = readFile(imageFile);
	if (data.empty()){
		printf("�ļ�������ô�� %s\n", picture.c_str());

		releaseClassifier(classif);
		system("pause");
		return -1;
	}

	forward(classif, &data[0], data.size());

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
		float value = lin[predict_label];

		if (predict_label != blank_label && predict_label != prev_label){
			result = result + getLabel(labelMap, predict_label);
		}
		
		result_raw = result_raw + getLabel(labelMap, predict_label);
		prev_label = predict_label;
	}

	releaseBlobData(premuted_fc);
	printf("ʶ���Ľ����ǣ�\n%s\n", result.c_str());
	//printf("ʶ���Ľ����ǣ�\n%s\n%s\n", result.c_str(), result_raw.c_str());
	releaseClassifier(classif);

	system("pause");
	return 0;
}
