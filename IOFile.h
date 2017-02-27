#pragma once

#ifndef CIOFILE_CLASS_DEF
#define CIOFILE_CLASS_DEF
#include <vector>
#include <fstream>
#include <algorithm>
#include "json.h"
using namespace std;

class CIOFile
{
public:
	CIOFile(){};
	int ParseJson(const string& file_path, Json::Value &root);
	// �ж��Ƿ���ڸ��ļ�
	int isFile(const string& file_path);
	
	// �����ע��ϢΪJson�ļ�
	int WriteKeyToJsonFile(const string& file_path, const string& image_name, const int scene_calss, const vector<int> attr);

	int ReadKeyFromJsonFile(const string& file_path, const string& image_name, int& scene_class, vector<int>& attr);
	void ReadTxtFile(const CString& path);
	void WriteXMLFile(const CString& path);
};

#endif