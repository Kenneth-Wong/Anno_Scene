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
	// 判断是否存在该文件
	int isFile(const string& file_path);
	
	// 保存标注信息为Json文件
	int WriteKeyToJsonFile(const string& file_path, const string& image_name, const int scene_calss, const vector<int> attr);

	int ReadKeyFromJsonFile(const string& file_path, const string& image_name, int& scene_class, vector<int>& attr);
	void ReadTxtFile(const CString& path);
	void WriteXMLFile(const CString& path);
};

#endif