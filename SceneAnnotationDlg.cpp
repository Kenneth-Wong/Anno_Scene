
// SceneAnnotationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SceneAnnotation.h"
#include "SceneAnnotationDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX }; 

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CSceneAnnotationDlg �Ի���



CSceneAnnotationDlg::CSceneAnnotationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSceneAnnotationDlg::IDD, pParent)
	, path(_T(""))
	, showIdx(0)
	, num_commit(0)
	, m_class_sel(0)
	, groupRadio(0)
	, p_scene(0)
	, p_attr(0)
	, json_start_id(0)
	, json_package_id(0)
	, json_image_num(0)
	, end_id(0)
	, sel_class(0)
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
	ON_BN_CLICKED(IDC_GROUP, &CSceneAnnotationDlg::OnBnClickedChangeGroup)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CSceneAnnotationDlg ��Ϣ�������

BOOL CSceneAnnotationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//ShowWindow(SW_MAXIMIZE); 
	ShowWindow(SW_NORMAL);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	m_pro.SetPos(0);
	GetDlgItem(IDC_STATIC4)->SetWindowText(L"(0/0)");



	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

/*
*  ��ȡconfig�ļ���������עͼ�������Ϣ
* json �ؼ��֣�
* image_num��package������ͼ����Ŀ
* start_id����һ��ͼ�����������ݼ��е�id
* package_id����������id
* type�� train/val/test
*/
int CSceneAnnotationDlg::ReadConfigFile(const CString& dir_path)
{
	//��ȡ�����ļ�
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
		json_image_num = root["image_num"].asInt();
		json_start_id = root["start_id"].asInt();
		if (!root["start_id"].isNull())
			json_start_id = root["start_id"].asInt();
		else
		{
			MessageBox(L"Parameter start_id not exist!");
			return 0;
		}
		if (!root["image_num"].isNull())
			json_image_num = root["image_num"].asInt();
		else
		{
			MessageBox(L"Parameter image_num not exist!");
			return 0;
		}
		if (!root["package_id"].isNull())
			json_package_id = root["package_id"].asInt();
		else
		{
			MessageBox(L"Parameter package_id not exist!");
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
* ��ȡԤ���ע�ļ����ֱ��ȡtrain��val��test�ĳ���������Ԥ���ע
* ����Ԥ�����ǰ20����ǩ������Ԥ�����ǰ10����ǩ
*/
void CSceneAnnotationDlg::ReadDataFiles(int TYPE)
{
	ifstream f;
	f.open(PLACES);
	uchar io;
	string e_name;
	string c_name; CString c;

	while (!f.eof())
	{
		f >> io >> e_name >> c_name;
		c = c_name.c_str();
		m_classes.push_back(scene(io, e_name, c));
	}
	f.close();
	WARN(m_classes.size() == 205, "Classes NUM is not 205!")


	f.open(ATTRIBUTE);
	while (!f.eof())
	{
		f >> e_name >> c_name;
		c = c_name.c_str();
		m_attributes.push_back(attribute(e_name, c));
	}
	f.close();
	WARN(m_attributes.size() == 102, "Attributes NUM is not 102!")

	vector< vector<char16_t> > *p[6];
	p[0] = &m_pr_scene_train;
	p[1] = &m_pr_scene_val;
	p[2] = &m_pr_scene_test;
	p[3] = &m_pr_attr_train;
	p[4] = &m_pr_attr_val;
	p[5] = &m_pr_attr_test;
	string path[6] = { TRAIN14_CLASS, VAL14_CLASS, TEST15_CLASS, TRAIN14_ATTR, VAL14_ATTR, TEST15_ATTR};
	int label,len;

	int k[2] = { TYPE, TYPE + 3 };
	for (int i = 0; i < 2; i++)
	{
		f.open(path[k[i]]);
		while (!f.eof())
		{
			vector<char16_t> tmp;
			len = k[i]>2 ? 10 : 20;
			for (int i = 0; i < len; i++)
			{
				f >> label;
				tmp.push_back((char16_t)label);
			}
			WARN(tmp.size() == len, "Something wrong when loading the data files!")
				(*(p[k[i]])).push_back(tmp);
		}
		f.close();
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSceneAnnotationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSceneAnnotationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// ͼ��Ŀ¼�ı�ʱһϵ�ж���
void CSceneAnnotationDlg::OnEnChangeEditbrowse()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemText(IDC_EDITBROWSE, path);

	
	/// Init all
	m_allImgVec.clear();
	m_allImgName.clear();
	state.clear();
	showIdx = 0;
	num_commit = 0;
	groupRadio = 0;
	
	// У���ļ�������
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
	state.resize(m_allImgVec.size(), (char)0);

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
			vector<int> attr;
			for (size_t i = 0; i < info["scene_attribute"].size(); i++)
			{
				attr.push_back(info["scene_attribute"][i].asInt());
			}
			m_annotated.insert(pair<CString, Annotation>(c_i_name, Annotation(scene, attr)));
		}
	}
	
	WARN(m_annotated.size() <= m_allImgVec.size(), "Annotation NUM is not compatible with that of Imags NUM!");

	// set state and get image's name
	for (size_t i = 0; i < m_allImgVec.size(); i++)
	{
		CString name = m_allImgVec[i];
		int n = name.ReverseFind('\\');
		name.Delete(0, n + 1);
		m_allImgName.push_back(name);
		if (m_annotated.count(name)>0)
			state[i] = (char)1;
	}

	end_id = json_start_id + json_image_num - 1;

	num_commit = m_annotated.size();

	if (json_type == "train")
	{
		p_scene = &m_pr_scene_train;
		p_attr = &m_pr_attr_train;
	}
	else if (json_type == "val")
	{
		p_scene = &m_pr_scene_val;
		p_attr = &m_pr_attr_val;
	}
	else if (json_type == "test")
	{
		p_scene = &m_pr_scene_test;
		p_attr = &m_pr_attr_test;
	}

	//set the progress bar
	m_pro.SetRange(0, m_allImgVec.size());
	m_pro.SetPos(num_commit);
	

	CString info;
	info.Format(L"(%d/%d)", num_commit, m_allImgVec.size());
	GetDlgItem(IDC_STATIC4)->SetWindowText(info);

	// ע����ʾ��ͼ�����ñ�ע��Ϣ�Ǵ�m_allImgVec��ȡ�ģ�
	// �� showIdx Ϊ������ȡ��ע��Ϣ����ȫ��������Ҫ����start_id
	DrawPic(showIdx);
	SetRadioState(showIdx);
	SetCheckBoxState(showIdx);
	SetRadioNames(showIdx+json_start_id);
	SetCheckNames(showIdx+json_start_id);
	showSamples(showIdx+json_start_id);
	
	
	GetDlgItem(IDC_STATIC6)->SetWindowText(state[showIdx] ? L"�ѱ�ע" : L"δ��ע");

}

void CSceneAnnotationDlg::SetRadioNames(int showIdx)
{
	for (int i = 0; i < 10; i++)
	{
		GetDlgItem(1018 + i)->SetWindowText(m_classes[(*p_scene)[showIdx][i + groupRadio]].c_name);
	}
}

void CSceneAnnotationDlg::SetCheckNames(int showIdx)
{
	for (int i = 0; i < 10; i++)
	{
		GetDlgItem(IDC_CHECK1 + i)->SetWindowText(m_attributes[(*p_attr)[showIdx][i]].c_name);
	}
}

void CSceneAnnotationDlg::showSamples(int showIdx)
{
	CString root = _T(SAMPLEROOT);
	for (int i = 0; i < NUM_SAMPLE / 2; i++)
	{
		int pr_label = (*p_scene)[showIdx][i + groupRadio];
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
	CString strTemp = strPath;//�����Ŀ¼·��
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_allImgVec.size() == 0)
		return;
	WARN(showIdx + 1 < m_allImgVec.size(), "This is the final image!");
	showIdx++;
	DrawPic(showIdx);
	groupRadio = 0;
	SetRadioState(showIdx);
	SetCheckBoxState(showIdx);
	SetRadioNames(showIdx + json_start_id);
	SetCheckNames(showIdx + json_start_id);
	showSamples(showIdx + json_start_id);
	
	GetDlgItem(IDC_STATIC6)->SetWindowText(state[showIdx] ? L"�ѱ�ע" : L"δ��ע");
}

// prev image
void CSceneAnnotationDlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_allImgVec.size() == 0)
		return;
	WARN(showIdx > 0, "This is the first image!");

	showIdx--;
	DrawPic(showIdx);
	groupRadio = 0;
	SetRadioState(showIdx);
	SetCheckBoxState(showIdx);
	SetRadioNames(showIdx + json_start_id);
	SetCheckNames(showIdx + json_start_id);
	showSamples(showIdx + json_start_id);
	GetDlgItem(IDC_STATIC6)->SetWindowText(state[showIdx] ? L"�ѱ�ע" : L"δ��ע");
}


void CSceneAnnotationDlg::OnStnClickedStatic3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

/*���涯����
* �����ļ�·����Annotation\\train2014(or val2014 or test2015).json
* ��������1���õ����ֵm_class_sel�����Ե�һ��ֵ��
        ��2���������ļ����鿴��ͼƬ�Ƿ��Ѿ�����ע�����ѱ�ע����ʾ����ע�����
		��3�������ѱ���ע���������Ȼ�ύ���Ի���ѯ���Ƿ�ȷ�ϣ���ȷ�����޸ı�עֵ
		��4����δ����ע���޸�״̬������עд��json�ļ���ͬʱ������һ��ͼ������Ѿ������һ��ͼ��������ʾ
		��5������Ѿ�������б�ע��������ʾ

*/
void CSceneAnnotationDlg::OnBnClickedEnsure()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//if (num_commit == m_allImgVec.size())
	//	return;

	
	num_commit = state[showIdx] ? num_commit : num_commit + 1; // ��������Ѿ���ע���򲻱䣬�����1
	state[showIdx] = (char)1;
	m_pro.SetPos(num_commit);
	CString info;
	info.Format(L"(%d/%d)", num_commit, m_allImgVec.size());
	GetDlgItem(IDC_STATIC4)->SetWindowText(info);

	string img_name = ((LPCSTR)CStringA(m_allImgName[showIdx]));
	if (m_class_sel == 10)
		sel_class = -1;
	else
		sel_class = (*p_scene)[showIdx + json_start_id][m_class_sel + groupRadio];
	vector<int> attr;
	for (int i = 0; i < 10; i++)
		if (((CButton*)GetDlgItem(IDC_CHECK1 + i))->GetCheck() == BST_CHECKED)
			attr.push_back((*p_attr)[showIdx + json_start_id][i]);

	// д���ļ��У�ͬʱ����m_annotated
	io->WriteKeyToJsonFile(save_annotated, img_name, sel_class, attr);
	if (m_annotated.count(m_allImgName[showIdx]) == 0)
		m_annotated.insert(pair<CString, Annotation>(m_allImgName[showIdx], Annotation(sel_class, attr)));
	else
	{
		m_annotated[m_allImgName[showIdx]].scene_id = sel_class;
		m_annotated[m_allImgName[showIdx]].attr = attr;
	}

	WARN(showIdx + 1 < m_allImgVec.size(), "This is the final image!");
	
	showIdx++;
	DrawPic(showIdx);
	groupRadio = 0;
	SetRadioState(showIdx);
	SetCheckBoxState(showIdx);
	SetRadioNames(showIdx + json_start_id);
	SetCheckNames(showIdx + json_start_id);
	showSamples(showIdx + json_start_id);

	GetDlgItem(IDC_STATIC6)->SetWindowText(state[showIdx] ? L"�ѱ�ע" : L"δ��ע");

	
}


//BOOL CSceneAnnotationDlg::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO:  �ڴ����ר�ô����/����û���
//	if (m_ACCcommit)
//	{
//		if (::TranslateAccelerator(m_hWnd, m_ACCcommit, pMsg))
//		{
//			OnBnClickedEnsure();//���ذ�ť����ĺ�����
//			return(TRUE);
//		}
//	}
//	return CDialogEx::PreTranslateMessage(pMsg);
//}


void CSceneAnnotationDlg::OnBnClickedRadio1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	groupRadio = 10 - groupRadio;
	SetRadioState(showIdx);
	SetRadioNames(showIdx + json_start_id);
	showSamples(showIdx + json_start_id);
}


void CSceneAnnotationDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_allImgVec.size() == 0)
		return;

	CPoint ptCursor=point;
	//GetCursorPos(&ptCursor);//��ȡ���λ��
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
	int pr_label = (*p_scene)[showIdx + json_start_id][pic_id / 2 + groupRadio];
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




void CSceneAnnotationDlg::SetRadioState(int showIdx)
{
	// ��ȡ��ǰͼƬ��
	CString key = m_allImgName[showIdx];

	if (m_annotated.size() == 0 || m_annotated.count(key) == 0) //�ޱ�ע��Ϣ
	{
		m_class_sel = 0;//default
	}
	else // �б�ע��Ϣ
	{
		Annotation anno = m_annotated[key];
		// ��ȡ��ѡ������������ǰ20�������е�id
		int sel_id;
		if (anno.scene_id == -1) //�ޱ�ǩ
			sel_id = -1;
		else{
			for (int i = 0; i < 20; i++)
			{
				if ((*p_scene)[showIdx + json_start_id][i] == anno.scene_id)
				{
					sel_id = i;
					break;
				}
			}
		}
		if (sel_id==-1)
		{
			m_class_sel = 10;// NONE
		}
		else
		{
			if (sel_id < 10) // ��ǩ��ǰʮ��֮һ
			{
				if (groupRadio == 0) // ��ǰ��ʾ���ǵ�һ��
					m_class_sel = sel_id;
				else m_class_sel = 100;// nothing
			}
			else // ��ǩ�Ǻ�ʮ��֮һ
			{
				if (groupRadio == 0) //��ǰ��ʾ��һ��
					m_class_sel = 100;
				else m_class_sel = sel_id - groupRadio; //��ʾ�ڶ���
			}
		}	
	}
	UpdateData(false);
}

void CSceneAnnotationDlg::SetCheckBoxState(int showIdx)
{
	CString key = m_allImgName[showIdx];
	for (int i = 0; i < 10; i++)
		((CButton*)GetDlgItem(IDC_CHECK1 + i))->SetCheck(BST_UNCHECKED);

	if (!(m_annotated.size() == 0 || m_annotated.count(key) == 0)) 
	{
		Annotation anno = m_annotated[key];
		// ��ȡ��ѡ������������ǰ20�������е�id
		for (int i = 0; i < anno.attr.size(); i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if ((*p_attr)[showIdx + json_start_id][j] == anno.attr[i])
				{
					((CButton*)GetDlgItem(IDC_CHECK1 + j))->SetCheck(BST_CHECKED);
					break;
				}
			}
		}
	}
	UpdateData(false);
}