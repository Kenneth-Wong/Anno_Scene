
// SceneAnnotationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SceneAnnotation.h"
#include "SceneAnnotationDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CString getName(CString path);

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX }; 

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSceneAnnotationDlg 对话框



CSceneAnnotationDlg::CSceneAnnotationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSceneAnnotationDlg::IDD, pParent)
	, path(_T(""))
	, showIdx(0)
	, num_commit(0)
	, m_class_sel(0)
	, groupRadio(0)
	, p_pr(0)
	//, p_scene(0)
	//, p_attr(0)
	, json_start_id(0)
	, json_package_id(0)
	, json_image_num(0)
	, end_id(0)
	, sel_class(0)
	, m_language(0)
	, m_globalIdx(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_ACCcommit = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));

	io = new CIOFile();
}

void CSceneAnnotationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_pro);
	DDX_Radio(pDX, IDC_RADIO1, m_class_sel);
	DDX_Radio(pDX, IDC_RADIO13, m_language);
	DDX_Text(pDX, IDC_EDIT1, m_globalIdx);
}

BEGIN_MESSAGE_MAP(CSceneAnnotationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDITBROWSE, &CSceneAnnotationDlg::OnEnChangeEditbrowse)
	ON_BN_CLICKED(IDC_BUTTON3, &CSceneAnnotationDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CSceneAnnotationDlg::OnBnClickedButton2)
	ON_STN_CLICKED(IDC_STATIC3, &CSceneAnnotationDlg::OnStnClickedStatic3)
	ON_BN_CLICKED(IDC_ENSURE, &CSceneAnnotationDlg::OnBnClickedEnsure)
	ON_BN_CLICKED(IDC_RADIO1, &CSceneAnnotationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CSceneAnnotationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CSceneAnnotationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO4, &CSceneAnnotationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO5, &CSceneAnnotationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO6, &CSceneAnnotationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO7, &CSceneAnnotationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO8, &CSceneAnnotationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO9, &CSceneAnnotationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO10, &CSceneAnnotationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO11, &CSceneAnnotationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO12, &CSceneAnnotationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_GROUP, &CSceneAnnotationDlg::OnBnClickedChangeGroup)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDOK, &CSceneAnnotationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSceneAnnotationDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO13, &CSceneAnnotationDlg::OnBnClickedRadio13)
	ON_BN_CLICKED(IDC_RADIO14, &CSceneAnnotationDlg::OnBnClickedRadio13)
	ON_BN_CLICKED(IDC_GO, &CSceneAnnotationDlg::OnBnClickedGo)
END_MESSAGE_MAP()


// CSceneAnnotationDlg 消息处理程序

BOOL CSceneAnnotationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//ShowWindow(SW_MAXIMIZE); 
	ShowWindow(SW_NORMAL);

	// TODO:  在此添加额外的初始化代码

	/****************************************************************/
	// 加载places和attribute标签
	ifstream f;
	f.open(PLACES);
	uchar io;
	string e_name;
	string c_name;
	CString c;
	m_classes.swap(vector<scene>());
	m_classes.reserve(205);
	for (int i = 0; i < 205; i++)
	{
		f >> io >> e_name >> c_name;

		c = c_name.c_str();
		m_classes.push_back(scene(io, e_name, c));

	}
	f.close();
	//WARN(m_classes.size() == 205, "Classes NUM is not 205!");

	f.open(ATTRIBUTE);
	m_attributes.swap(vector<attribute>());
	m_attributes.reserve(102);
	for (int i = 0; i < 102; i++)
	{
		f >> e_name >> c_name;
		c = c_name.c_str();
		m_attributes.push_back(attribute(e_name, c));
	}
	f.close();
	/****************************************************************/

	//WARN(m_attributes.size() == 102, "Attributes NUM is not 102!");
	m_pro.SetPos(0);
	GetDlgItem(IDC_STATIC4)->SetWindowText(L"(0/0)");



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

/*
*  读取config文件，包含标注图像包的信息
* json 关键字：
* image_num：package包含的图像数目
* start_id：第一张图像在整个数据集中的id
* package_id：整个包的id
* type： train/val/test
*/
int CSceneAnnotationDlg::ReadConfigFile(const CString& dir_path)
{
	//读取配置文件
	ifstream ifs;
	CString config_json = dir_path + _T("\\image_detail.json");
	ifs.open(config_json);
	if (!ifs)
	{
		MessageBox(L"Config file not exist!");
		return 0;
	}
	Json::Reader j_reader;
	Json::Value root;
	if (j_reader.parse(ifs, root))
	{

		if (!root["image_num"].isNull())
			json_image_num = root["image_num"].asInt();
		else
		{
			MessageBox(L"Parameter image_num not exist!");
			return 0;
		}
		if (!root["type"].isNull())
			json_type = root["type"].asString();
		else
		{
			MessageBox(L"Parameter type not exist!");
			return 0;
		}
		return 1;
	}
	else{
		MessageBox(L"Read config file failed!");
		return 0;
	}
}

/*
* 读取预测标注文件，分别读取train，val，test的场景和属性预测标注
* 场景预测给出前20个标签，属性预测给出前10个标签
*/
void CSceneAnnotationDlg::ReadDataFiles(int TYPE)
{
	
	map< CString, Prediction> *p[3];
	p[0] = &m_pr_train;
	p[1] = &m_pr_val;
	p[2] = &m_pr_test;
	if ((*p[TYPE]).size() != 0) // 如果已经加载，则不用重新加载
		return;

	ifstream f, f2; 
	string path[6] = { TRAIN14_CLASS, VAL14_CLASS, TEST15_CLASS, TRAIN14_ATTR, VAL14_ATTR, TEST15_ATTR};
	char16_t label;
	string img_name, img_name2; 
	CString IMG_NAME;

	int num[3] = { TRAIN_NUM, VAL_NUM, TEST_NUM };

	f.open(path[TYPE]);
	f2.open(path[TYPE + 3]);
	for (int q = 0; q < num[TYPE]; q++)
	{
		f >> img_name; 
		f2 >> img_name2;
		assert(img_name == img_name2);
		IMG_NAME = img_name.c_str();
		vector<char16_t> scene_tmp, attr_tmp;
		for (int j = 0; j < 20; j++)
		{
			f >> label;
			scene_tmp.push_back(label);
		}
		for (int j = 0; j < 10; j++)
		{
			f2 >> label;
			attr_tmp.push_back(label);
		}
		(*p[TYPE]).insert(pair<CString, Prediction>(IMG_NAME, Prediction(scene_tmp, attr_tmp)));
	}
#ifdef DEBUG
	MessageBox(L"Completed");
#endif	
}

void CSceneAnnotationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSceneAnnotationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSceneAnnotationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 图像目录改变时一系列动作
void CSceneAnnotationDlg::OnEnChangeEditbrowse()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDITBROWSE, path);

	
	/// Init all
	m_allImgVec.clear();
	m_allImgName.clear();
	m_NoneAnnotated.clear();
	m_annotated.clear();
	//state.clear();
	showIdx = 0;
	m_globalIdx = _T("");
	UpdateData(false);
	num_commit = 0;
	groupRadio = 0;
	
	// 校验文件夹命名
	CString dir = path;
	dir.Delete(0, dir.ReverseFind('\\') + 1);
	WARN(dir.ReverseFind('_') != -1, "Invalid direction name!");
	int x = dir.ReverseFind('_');
	CString prefix = dir.Left(x);
	WARN(prefix == "train" || prefix == "val" || prefix == "test", "Invalid direction name!");
	
		
	ScanDiskFile(path);

	WARN(ReadConfigFile(path), "Some problems when reading config file. Please have a check!");
	
	WARN(!(m_allImgVec.size() == 0), "No images in this directory!");

	WARN(json_image_num == m_allImgVec.size(), "Images NUM is not compatible with declaration in config file!");
	
	WARN(((LPCSTR)CStringA(prefix)) == json_type, "Direction name is not compatible with declaration in config file!");

	ReadDataFiles((prefix=="train"?TRAIN:(prefix=="val"?VAL:TEST)));

	

	// initialize the image state
	//state.resize(m_allImgVec.size(), (char)0);

	// load annoted_file
	save_annotated = SAVEROOT; 
	save_annotated += ((LPCSTR)CStringA(dir));
	save_annotated += ".json";

	Json::Value root;
	if (io->ParseJson(save_annotated, root))
	{
		Json::Value::Members mem = root.getMemberNames();
		for (auto iter = mem.begin(); iter != mem.end(); iter++)
		{
			string i_name = *iter;
			CString c_i_name(i_name.c_str());
			Json::Value info = root[*iter];
			int scene = info["scene_class"].asInt();
			vector<char16_t> attr;
			for (size_t i = 0; i < info["scene_attribute"].size(); i++)
			{
				attr.push_back((char16_t)info["scene_attribute"][i].asInt());
			}
			m_annotated.insert(pair<CString, Annotation>(c_i_name, Annotation((char16_t)scene, attr)));
		}
	}
	
	WARN_NO_RETURN(m_annotated.size() <= m_allImgVec.size(), "NOTE: Annotation NUM is larger than that of Images NUM!");
	
	// set state and get image's name
	for (size_t i = 0; i < m_allImgVec.size(); i++)
	{
		CString name = getName(m_allImgVec[i]);
		m_allImgName.push_back(name);
		if (m_annotated.count(name) == 0)
			m_NoneAnnotated.insert(name);
	}

	num_commit = m_annotated.size() <= m_allImgVec.size() ? m_annotated.size() : m_allImgVec.size();


	p_pr = json_type == "train" ? &m_pr_train : (json_type == "val" ? &m_pr_val : (json_type == "test" ? &m_pr_test : 0));
	WARN(p_pr, "Parameter json_type in illegal!");


	//set the progress bar
	m_pro.SetRange(0, m_allImgVec.size());
	m_pro.SetPos(num_commit);
	

	CString info;
	info.Format(L"(%d/%d)", num_commit, m_allImgVec.size());
	GetDlgItem(IDC_STATIC4)->SetWindowText(info);

	m_globalIdx.Format(L"%d", showIdx + 1);
	UpdateData(false);
	GetDlgItem(IDC_IMGNAME)->SetWindowText(m_allImgName[showIdx]);

//MessageBox(L"OK!");

	// 如果全部标注，则根据showidx显示一张
	CString key;
	if (m_NoneAnnotated.size() == 0)
	{
		key = m_allImgName[showIdx];
		DrawPic(showIdx);
		SetRadioState(key);
		SetCheckBoxState(key);
		SetRadioNames(key);
		SetCheckNames(key);
		showSamples(key);
	}
	else
	{
		key = *m_NoneAnnotated.begin();
		vector<CString>::iterator iter = find(m_allImgName.begin(), m_allImgName.end(), key);
		if (iter != m_allImgName.end())
			showIdx = iter - m_allImgName.begin();
		DrawPic(showIdx);
		SetRadioState(key);
		SetCheckBoxState(key);
		SetRadioNames(key);
		SetCheckNames(key);
		showSamples(key);
	}

	
	GetDlgItem(IDC_STATIC6)->SetWindowText(m_annotated.count(key) ? L"已标注" : L"未标注");

}

void CSceneAnnotationDlg::SetRadioNames(CString key)
{
	if (!m_language)
	{
		for (int i = 0; i < 10; i++)
		{
			GetDlgItem(1018 + i)->SetWindowText(m_classes[(*p_pr)[key].scene[i + groupRadio]].c_name);
		}
	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			CString ename(m_classes[(*p_pr)[key].scene[i + groupRadio]].e_name.c_str());
			GetDlgItem(1018 + i)->SetWindowText(ename);
		}
	}
}

void CSceneAnnotationDlg::SetCheckNames(CString key)
{
	if (!m_language)
	{
		for (int i = 0; i < 10; i++)
		{
			GetDlgItem(IDC_CHECK1 + i)->SetWindowText(m_attributes[(*p_pr)[key].attr[i]].c_name);
		}
	}
	else
	{
		
		for (int i = 0; i < 10; i++)
		{
			GetDlgItem(IDC_CHECK1 + i)->SetWindowText(CString(m_attributes[(*p_pr)[key].attr[i]].e_name.c_str()));
		}
	}
}

void CSceneAnnotationDlg::showSamples(CString key)
{
	CString root = _T(SAMPLEROOT);
	for (int i = 0; i < NUM_SAMPLE / 2; i++)
	{
		int pr_label = (*p_pr)[key].scene[i + groupRadio];
		char p[5];
		sprintf(p, "%d", pr_label);
		CString path = root + p + _T("\\*.*");
		CString strFile, suffix;
		CFileFind find;
		BOOL isFind = find.FindFile(path);
		int id = i * 2;
		while (isFind)
		{
			isFind = find.FindNextFileW();
			if (find.IsDots())
				continue;
			else{
				strFile = root+p+_T("\\")+find.GetFileName();
				string str = ((LPCSTR)CStringA(strFile));
				IplImage* img;
				img = cvLoadImage(str.c_str(), 1);
				DrawPicToHDC(img, IDC_PIC1+id );
				cvReleaseImage(&img);
				id++;
			}
			
		}
	}
}

void CSceneAnnotationDlg::DrawPic(int showIdx)
{
	string str = ((LPCSTR)CStringA(m_allImgVec[showIdx]));
	IplImage* img;
	img = cvLoadImage(str.c_str(), 1);
	DrawPicToHDC(img, IDC_PICTURE);
	cvReleaseImage(&img);
}

void CSceneAnnotationDlg::DrawPicToHDC(IplImage* img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);

	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);

}



void CSceneAnnotationDlg::ScanDiskFile(const CString& strPath)
{
	CFileFind find;
	CString strTemp = strPath;//传入的目录路径
	CString strDirectory = strPath + _T("\\") + _T("*.*");
	CString strFile,suffix;


	BOOL isFind = find.FindFile(strDirectory);
	while (isFind)
	{
		isFind = find.FindNextFileW();
		if (find.IsDots())
			continue;
		else if (find.IsDirectory())
		{
			strFile = find.GetFileName();
			ScanDiskFile(strTemp+ _T("\\") + strFile);
		}
		else
		{
			strFile = find.GetFileName();
			suffix = strFile;
			int dots = suffix.Find('.');
			suffix.Delete(0, dots + 1);
			if (!suffix.CompareNoCase(_T("jpg")) || !suffix.CompareNoCase(_T("jpeg")) || !suffix.CompareNoCase(_T("png")))
			{
				m_allImgVec.push_back(strTemp + _T("\\") + strFile);
			}
		}
	}
}

// next image
void CSceneAnnotationDlg::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_allImgVec.size() == 0)
		return;
	ShowNextImage();
}

// prev image
void CSceneAnnotationDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_allImgVec.size() == 0)
		return;
	WARN(showIdx > 0, "This is the first image!");

	showIdx--;
	DrawPic(showIdx);
	groupRadio = 0;
	CString key = m_allImgName[showIdx];
	SetRadioState(key);
	SetCheckBoxState(key);
	SetRadioNames(key);
	SetCheckNames(key);
	showSamples(key);
	m_globalIdx.Format(L"%d", showIdx + 1);
	UpdateData(false);
	GetDlgItem(IDC_IMGNAME)->SetWindowText(key);
	GetDlgItem(IDC_STATIC6)->SetWindowText(m_annotated.count(key) ? L"已标注" : L"未标注");
}


void CSceneAnnotationDlg::OnStnClickedStatic3()
{
	// TODO:  在此添加控件通知处理程序代码
}

/*保存动作：
* 保存文件路径：Annotation\\train2014(or val2014 or test2015).json
* 动作：（1）得到类别值m_class_sel和属性的一组值。
        （2）读入结果文件，查看此图片是否已经被标注，若已标注，显示被标注的类别
		（3）若在已被标注的情况下仍然提交，对话框询问是否确认，若确认则修改标注值
		（4）若未被标注，修改状态，将标注写入json文件，同时跳到下一张图。如果已经是最后一张图，给出提示
		（5）如果已经完成所有标注，给出提示

*/
void CSceneAnnotationDlg::OnBnClickedEnsure()
{
	// TODO:  在此添加控件通知处理程序代码
	//if (num_commit == m_allImgVec.size())
	//	return;

	CString key = m_allImgName[showIdx];
	num_commit = m_annotated.count(key) ? num_commit : num_commit + 1; // 如果本来已经标注，则不变，否则加1
	m_pro.SetPos(num_commit);
	CString info;
	info.Format(L"(%d/%d)", num_commit, m_allImgVec.size());
	GetDlgItem(IDC_STATIC4)->SetWindowText(info);

	string img_name = (LPCSTR)CStringA(key);
	if (m_class_sel == 10)
		sel_class = NOSCENE;
	else if (m_class_sel == 11)
		sel_class = NOLABEL;
	else
		sel_class = (*p_pr)[key].scene[m_class_sel + groupRadio];
	vector<char16_t> attr;
	for (int i = 0; i < 10; i++)
		if (((CButton*)GetDlgItem(IDC_CHECK1 + i))->GetCheck() == BST_CHECKED)
			attr.push_back((*p_pr)[key].attr[i]);

	// 写到文件中，同时更新m_annotated和m_NoneAnnotated
	io->WriteKeyToJsonFile(save_annotated, img_name, sel_class, attr);
	if (m_annotated.count(key) == 0)
	{
		WARN(m_NoneAnnotated.count(key), "Problem: This image is not in both Annotated and NONE-Annotated vector!");
		m_annotated.insert(pair<CString, Annotation>(key, Annotation(sel_class, attr)));
		m_NoneAnnotated.erase(key);
	}
	else
	{
		WARN(!m_NoneAnnotated.count(key), "Problem: This image is in both Annotated and NONE-Annotated vector!");
		m_annotated[key].scene_id = sel_class;
		m_annotated[key].attr = attr;
	}

	ShowNextImage();

	
}


//BOOL CSceneAnnotationDlg::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO:  在此添加专用代码和/或调用基类
//	if (m_ACCcommit)
//	{
//		if (::TranslateAccelerator(m_hWnd, m_ACCcommit, pMsg))
//		{
//			OnBnClickedEnsure();//隐藏按钮处理的函数；
//			return(TRUE);
//		}
//	}
//	return CDialogEx::PreTranslateMessage(pMsg);
//}


void CSceneAnnotationDlg::OnBnClickedRadio1()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true); // update m_class_sel

#ifdef DEBUG
	switch (m_class_sel)
	{
	case 0:
		MessageBox(L"1 is selected!");
		break;
	case 1:
		MessageBox(L"2 is selected!");
		break;
	case 10:
		MessageBox(L"none is selected!");
		break;
	default:
		MessageBox(L"other is selected!");
	}
#endif

}


void CSceneAnnotationDlg::OnBnClickedChangeGroup()
{
	// TODO:  在此添加控件通知处理程序代码
	groupRadio = 10 - groupRadio;
	CString key = m_allImgName[showIdx];
	SetRadioState(key);
	SetRadioNames(key);
	showSamples(key);
}


void CSceneAnnotationDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_allImgVec.size() == 0)
		return;

	CPoint ptCursor=point;
	//GetCursorPos(&ptCursor);//获取鼠标位置
	CRect rc;
	int pic_id = -1;
	for (int i = 0; i < NUM_SAMPLE; i++)
	{
		GetDlgItem(IDC_PIC1 + i)->GetWindowRect(&(rc));
		ScreenToClient(&rc);
		if (rc.PtInRect(ptCursor))
		{
			pic_id = i;
			break;
		}
	}
	if (pic_id == -1)
		return;
	CString root = _T(SAMPLEROOT);
	int pr_label = (*p_pr)[m_allImgName[showIdx]].scene[pic_id / 2 + groupRadio];
	char p[5];
	sprintf(p, "%d", pr_label);
	CString path = root + p + _T("\\*.*");
	CString strFile, suffix;
	CFileFind find;
	BOOL isFind = find.FindFile(path);
	bool find_first = 0;
	while (isFind)
	{
		isFind = find.FindNextFileW();
		if (find.IsDots())
			continue;
		else{
			if (!find_first)
			{
				find_first = 1;
				if (pic_id % 2)
					continue;
			}
			strFile = root + p + _T("\\") + find.GetFileName();
			string str = ((LPCSTR)CStringA(strFile));
			Mat sample = imread(str.c_str(), 1);
			imshow(str.c_str(), sample);
			waitKey(0);
			return;
		}

	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}




void CSceneAnnotationDlg::SetRadioState(CString key)
{
	// 获取当前图片名


	if (m_annotated.size() == 0 || m_annotated.count(key) == 0) //无标注信息
	{
		m_class_sel = 0;//default
	}
	else // 有标注信息
	{
		Annotation anno = m_annotated[key];
		// 读取所选的类别和属性在前20个属性中的id
		int sel_id = -3;
		if (anno.scene_id == NOSCENE) //非场景
			sel_id = NOSCENE;
		else if (anno.scene_id == NOLABEL) //无合适
			sel_id = NOLABEL;
		else{
			for (int i = 0; i < 20; i++)
			{
				if ((*p_pr)[key].scene[i] == anno.scene_id)
				{
					sel_id = i;
					break;
				}
			}
		}
		if (sel_id==NOSCENE)
		{
			m_class_sel = 10;// NONE
		}
		else if (sel_id == NOLABEL)
		{
			m_class_sel = 11;
		}
		else if (sel_id == -3) // json BUG *********************************
		{
			m_class_sel = 0;
		}
		else
		{
			if (sel_id < 10) // 标签是前十个之一
			{
				if (groupRadio == 0) // 当前显示的是第一组
					m_class_sel = sel_id;
				else m_class_sel = 100;// nothing
			}
			else // 标签是后十个之一
			{
				if (groupRadio == 0) //当前显示第一组
					m_class_sel = 100;
				else m_class_sel = sel_id - groupRadio; //显示第二组
			}
		}	
	}
	UpdateData(false);
}

void CSceneAnnotationDlg::SetCheckBoxState(CString key)
{
	for (int i = 0; i < 10; i++)
		((CButton*)GetDlgItem(IDC_CHECK1 + i))->SetCheck(BST_UNCHECKED);

	if (!(m_annotated.size() == 0 || m_annotated.count(key) == 0)) 
	{
		Annotation anno = m_annotated[key];
		// 读取所选的类别和属性在前20个属性中的id
		for (int i = 0; i < anno.attr.size(); i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if ((*p_pr)[key].attr[j] == anno.attr[i])
				{
					((CButton*)GetDlgItem(IDC_CHECK1 + j))->SetCheck(BST_CHECKED);
					break;
				}
			}
		}
	}
	UpdateData(false);
}

void CSceneAnnotationDlg::ShowNextImage()
{
	CString key;
	if (m_NoneAnnotated.size() == 0) // 没有未标注的图像，正常往后访问
	{
		if (showIdx + 1 >= m_allImgVec.size())
		{
			GetDlgItem(IDC_STATIC6)->SetWindowText(m_annotated.count(m_allImgName[showIdx]) ? L"已标注" : L"未标注");
			MessageBox(L"This is the final image!");
			return;
		}
		
		showIdx++;
		DrawPic(showIdx);
		groupRadio = 0;
		key = m_allImgName[showIdx];
		SetRadioState(key);
		SetCheckBoxState(key);
		SetRadioNames(key);
		SetCheckNames(key);
		showSamples(key);

		m_globalIdx.Format(L"%d", showIdx + 1);
		UpdateData(false);
		GetDlgItem(IDC_IMGNAME)->SetWindowText(key);
	}
	else
	{
		//判断当前显示图像是否已标注
		// 若当前已经被标注，则从未标注的头部取一张显示
		if (m_annotated.count(m_allImgName[showIdx]))
		{
			key = *m_NoneAnnotated.begin();
			vector<CString>::iterator iter = find(m_allImgName.begin(), m_allImgName.end(), key);
			if (iter != m_allImgName.end())
				showIdx = iter - m_allImgName.begin();
			else{
				MessageBox(L"ERROR!");
				return;
			}
			DrawPic(showIdx);
			SetRadioState(key);
			SetCheckBoxState(key);
			SetRadioNames(key);
			SetCheckNames(key);
			showSamples(key);
			m_globalIdx.Format(L"%d", showIdx + 1);
			UpdateData(false);
			GetDlgItem(IDC_IMGNAME)->SetWindowText(key);
		}
		// 若当前未被标注，则先找到他在未标注集合里的位置，取他的下一张显示
		else
		{
			key = m_allImgName[showIdx];
			set<CString>::iterator next = m_NoneAnnotated.find(key);
			next++;
			if (next == m_NoneAnnotated.end())
			{
				MessageBox(L"This is the last none-annotated image!");
				return;
			}
			else
			{
				vector<CString>::iterator v_iter = find(m_allImgName.begin(), m_allImgName.end(), *next);
				if (v_iter != m_allImgName.end())
					showIdx = v_iter - m_allImgName.begin();
				else{
					MessageBox(L"ERROR!");
					return;
				}
				DrawPic(showIdx);
				key = m_allImgName[showIdx];
				SetRadioState(key);
				SetCheckBoxState(key);
				SetRadioNames(key);
				SetCheckNames(key);
				showSamples(key);
				m_globalIdx.Format(L"%d", showIdx + 1);
				UpdateData(false);
				GetDlgItem(IDC_IMGNAME)->SetWindowText(key);
			}
		}
	}


	GetDlgItem(IDC_STATIC6)->SetWindowText(m_annotated.count(key) ? L"已标注" : L"未标注");
}

static CString getName(CString path)
{
	CString name = path;
	name.Delete(0, name.ReverseFind('\\') + 1);
	return name;
}

void CSceneAnnotationDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	// 将缓存中的内容全部写到文件上

	if (!io->isFile(save_annotated)) // 如果不存在文件（被误删）
	{
		if (m_annotated.size() != 0)
		{
			ofstream ofs;
			ofs.open(save_annotated);
			Json::Value root;
			Json::StyledWriter writer;
			for (map<CString, Annotation>::iterator iter = m_annotated.begin(); iter != m_annotated.end(); iter++)
			{
				Json::Value info;
				info["scene_class"] = Json::Value((*iter).second.scene_id);
				if ((*iter).second.attr.size() == 0)
					info["scene_attribute"].resize(0);
				else
				{
					vector<char16_t> tmp = (*iter).second.attr;
					sort(tmp.begin(), tmp.end());
					for (size_t i = 0; i < tmp.size(); i++)
						info["scene_attribute"].append(tmp[i]);
				}
				root[(LPCSTR)CStringA((*iter).first)] = info;
			}
			ofs << writer.write(root);
			ofs.close();
		}
	}
	else // 否则，先读取原有标注信息，判断是否有原来不存在的条目，如果有，重写，否则不做任何动作
	{
		Json::Value root;
		io->ParseJson(save_annotated, root);
		bool change = false;
		for (map<CString, Annotation>::iterator iter = m_annotated.begin(); iter != m_annotated.end(); iter++)
		{
			if (root[(LPCSTR)CStringA((*iter).first)].isNull())
			{
				Json::Value info;
				info["scene_class"] = Json::Value((*iter).second.scene_id);
				if ((*iter).second.attr.size() == 0)
					info["scene_attribute"].resize(0);
				else
				{
					vector<char16_t> tmp = (*iter).second.attr;
					sort(tmp.begin(), tmp.end());
					for (size_t i = 0; i < tmp.size(); i++)
						info["scene_attribute"].append(tmp[i]);
				}
				root[(LPCSTR)CStringA((*iter).first)] = info;
				change = true;
			}
		}
		if (change)
		{
			ofstream ofs;
			Json::StyledWriter writer;
			ofs.open(save_annotated);
			ofs << writer.write(root);
			ofs.close();
		}
	}
	CDialogEx::OnOK();
}


void CSceneAnnotationDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	if (MessageBox(L"Are you sure to exit without saving?", L"WARN", MB_YESNO) == IDNO)
		return;
	CDialogEx::OnCancel();
}


void CSceneAnnotationDlg::OnBnClickedRadio13()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (!p_pr)
		return;
	SetRadioNames(m_allImgName[showIdx]);
	SetCheckNames(m_allImgName[showIdx]);
}


void CSceneAnnotationDlg::OnBnClickedGo()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_allImgVec.size() == 0)
		return;
	UpdateData(true);
	
	// check
	for (int i = 0; i < m_globalIdx.GetLength(); i++)
	{
		if (i == 0)
		{
			WARN(m_globalIdx[i]>'0'&&m_globalIdx[i] <= '9', "Invalid Number!");
		}
		else
		{
			WARN(m_globalIdx[i]>='0'&&m_globalIdx[i] <= '9', "Invalid Number!");
		}		
	}
	int idx=atoi((LPCSTR)CStringA(m_globalIdx))-1;
	WARN(idx >= 0 && idx < m_allImgVec.size(), "Invalid Number!");

	showIdx = idx;
	DrawPic(showIdx);
	CString key = m_allImgName[showIdx];
	SetRadioState(key);
	SetCheckBoxState(key);
	SetRadioNames(key);
	SetCheckNames(key);
	showSamples(key);
	GetDlgItem(IDC_IMGNAME)->SetWindowText(key);
	GetDlgItem(IDC_STATIC6)->SetWindowText(m_annotated.count(key) ? L"已标注" : L"未标注");
}
