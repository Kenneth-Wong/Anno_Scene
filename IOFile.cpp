#include "stdafx.h"
#include "IOFile.h"




int CIOFile::isFile(const string& file_path)
{
	ifstream ifs;
	ifs.open(file_path);
	if (!ifs)
		return 0;
	else
		return 1;
}
int CIOFile::ParseJson(const string& file_path, Json::Value &root)
{
	if (!isFile(file_path))
		return 0;
	ifstream ifs;
	ifs.open(file_path);
	Json::Reader reader;
	if (reader.parse(ifs, root))
		return 1;
	else return 0;
}

int CIOFile::ReadKeyFromJsonFile(const string& file_path, const string& image_name, int& scene_class, vector<int>& attr)
{
	if (!isFile(file_path))
		return 0; // file not exists
	Json::Value root;
	if (ParseJson(file_path, root))
	{
		if (!root[image_name].isNull())
		{
			Json::Value info = root[image_name];
			scene_class = info["scene_class"].asInt();
			for (size_t i = 0; i < info["scene_attribute"].size(); i++)
			{
				attr.push_back(info["scene_attribute"][i].asInt());
			}
			return 1; // success
		}
		else
			return -1; // key not exists
	}
	else
		return -2; // file is wrong
	
}
int CIOFile::WriteKeyToJsonFile(const string& file_path, const string& image_name, const int scene_calss, const vector<int> attr)
{
	ofstream ofs;
	int o_class;
	vector<int> o_attr;
	int res = ReadKeyFromJsonFile(file_path, image_name, o_class, o_attr);

	if (res == 0) // file not exists, wirte it directly.
	{
		ofs.open(file_path);
		Json::Value root, info;
		Json::StyledWriter writer;
		info["scene_class"] = Json::Value(scene_calss);
		if (attr.size() == 0)
			info["scene_attribute"].resize(0);
		else
		{
			vector<int> tmp = attr;
			sort(tmp.begin(), tmp.end());
			for (size_t i = 0; i < tmp.size(); i++)
				info["scene_attribute"].append(tmp[i]);
		}
		
		root[image_name] = info;
		ofs << writer.write(root);
		ofs.close();
	}	
	else if (res == -1) // key not exists, add the key 
	{
		Json::Value root, info;
		Json::StyledWriter writer;
		ParseJson(file_path, root);
		ofs.open(file_path);
		info["scene_class"] = Json::Value(scene_calss);
		if (attr.size() == 0)
			info["scene_attribute"].resize(0);
		else
		{
			vector<int> tmp = attr;
			sort(tmp.begin(), tmp.end());
			for (size_t i = 0; i < tmp.size(); i++)
				info["scene_attribute"].append(tmp[i]);
		}
		root[image_name] = info;
		ofs << writer.write(root);
		ofs.close();
	}
	else if (res == 1) // key exists
	{
		if (MessageBox(NULL, L"Are you sure to rectify this image annotation?", L"NOTE", MB_YESNO) == IDYES)
		{
			// note: 必须先读取文件，再调用open。open会使原来内容全部抹掉，导致parse失败
			Json::Value root;
			ParseJson(file_path, root);
			ofs.open(file_path);
			Json::StyledWriter writer;
			Json::Value& info = root[image_name];
			info.removeMember("scene_class");
			info["scene_calss"] = Json::Value(scene_calss);
			info["scene_attribute"].clear();
			if (attr.size() == 0)
				info["scene_attribute"].resize(0);
			else
			{
				vector<int> tmp = attr;
				sort(tmp.begin(), tmp.end());
				for (size_t i = 0; i < tmp.size(); i++)
					info["scene_attribute"].append(tmp[i]);
			}
			ofs << writer.write(root);
			ofs.close();
		}
		
	}
	return 1;
		
}