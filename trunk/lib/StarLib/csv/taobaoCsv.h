
#include <vector>
using namespace std;

class CTaobaoCsv
{
public:
	CTaobaoCsv();
	~CTaobaoCsv();
public:
	bool SaveFile(CString strFileName);
	int  AddItem();
protected:
private:
	int m_nItemIndex;
	vector<vector<CString>>m_vtItems;
	static int m_nColumns;	//ÁÐÊý
	static CString m_strColumnNames[];
	enum COLUMN{
		baobeimingcheng,
		baobeileimu,
		dianpuleimu,
		xinjiuchengdu,
		sheng,
		chengshi,
		chushoufangshi,
		baobeijiage,
		jiajiafudu,
		baobeishuliang,
		youxiaoqi,
		yunfeichengdan,
		pingyou,
		ems,
		kuaidi,
		fukuanfangshi,
		zhifubao,
		fapiao,
		baoxiu,
		zidongchongfa,
		fangrucangku,
		chuchuangtuijian,
		kaishishijian,
		xinqinggushi,
		baobeimiaoshu,
		baobeitupian,
		baobeishuxing,
		tuangoujia,
		zuixiaotuangoujianshu,
		youfeimubanID,
		huiyuandazhe,
		xiugaishijian,
		shangchuanzhuangtai,
		tupianzhuangtai,
		fandianbili,
		xintupian,
		shipin,
		xiaoshoushuxingzuhe,
		yonghushuruIDchuan,
		yonghushuruming_zhidui,
		shangjiabianma,
		xiaoshoushuxingbieming,
		daichongleixing,
		baobeibianhao,
		shuziID
	};
	void Set(CTaobaoCsv::COLUMN nColumn,CString str);
};
