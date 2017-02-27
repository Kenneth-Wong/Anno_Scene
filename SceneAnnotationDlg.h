
// SceneAnnotationDlg.h : 头文件
//
#include <vector>
#include "CvvImage.h"
#include "afxcmn.h"
#include <map>
#include <iostream>
#include <fstream>
#include "json.h"
#include "config.h"
#include "IOFile.h"
using namespace std;
using namespace cv;
#pragma once

#define NUM_SAMPLE 20

#define WARN(c, m) \
if(!(c))\
{\
	MessageBox(_T(m));\
	return;\
};

	

struct SceneClass{
	uchar io;
	string e_name;
	CString c_name;
public:
	SceneClass(){}
	SceneClass(uchar io_, string e_name_, CString c_name_) :io(io_), e_name(e_name_), c_name(c_name_){}
};
typedef SceneClass scene;

struct SceneAttribute{
	string e_name;
	CString c_name;
public:
	SceneAttribute(){}
	SceneAttribute(string e_name_, CString c_name_) :e_name(e_name_), c_name(c_name_){}
};
typedef SceneAttribute attribute;

struct Annotation{
	int scene_id;
	vector<int> attr;
public:
	Annotation(){}
	Annotation(int s_, const vector<int> a_) :scene_id(s_), attr(a_){}
};

// CSceneAnnotationDlg 对话框
class CSceneAnnotationDlg : public CDialogEx
{
// 构造
public:
	CSceneAnnotationDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SCENEANNOTATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditbrowse();
	CString path;
	LPCWSTR image_name;
	void DrawPicToHDC(IplImage* img, UINT ID);
	void DrawPic(int showIdx);
	void ScanDiskFile(const CString& strPath);
	void ReadDataFiles(int TYPE);
	int ReadConfigFile(const CString& dir_path);
	void SetRadioNames(int showIdx);
	void SetCheckNames(int showIdx);
	void showSamples(int showIdx);
	vector<CString> m_allImgVec;
	vector<CString> m_allImgName;
	unsigned int showIdx;
	int groupRadio;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	CProgressCtrl m_pro;
	afx_msg void OnStnClickedStatic3();
	afx_msg void OnBnClickedEnsure();
	int num_commit;
	vector<char> state;
	HACCEL m_ACCcommit;
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	vector<attribute> m_attributes;
	vector<scene> m_classes;
	vector< vector<char16_t> > m_pr_scene_train, m_pr_scene_val, m_pr_scene_test;
	vector< vector<char16_t> > m_pr_attr_train, m_pr_attr_val, m_pr_attr_test;
	vector <vector<char16_t> > *p_scene, *p_attr;
	ofstream ofs;
	ifstream ifs;
	int m_class_sel;//选择的控件id
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedChangeGroup();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	int json_start_id;
	int json_package_id;
	int json_image_num;
	string json_type;
	string save_annotated;
	int end_id;
	CIOFile* io;
	map<CString, Annotation> m_annotated;
	void SetRadioState(int showIdx);
	void SetCheckBoxState(int showIdx);
	int sel_class;//真正的场景id
};
