/*******************************************************************
; Author:	朱星星
; Web	:	http://hi.baidu.com/asmcvc
; E-mail:	asmcvc@163.com
; Date	:	2009.03.11
; 说明	:	经罗云彬的汇编版本“水波效果”改写
;********************************************************************/

#define		F_WO_ACTIVE			0001
#define		F_WO_NEED_UPDATE	0002
#define		F_WO_EFFECT			0004
#define		F_WO_ELLIPSE		0008

typedef struct _WAVE_OBJECT
{
	DWORD	hWnd;
	DWORD	dwFlag;				//见 F_WO_xxx 组合
	// ;********************************************************************
	DWORD	hDcRender;
	DWORD	hBmpRender;
	DWORD	lpDIBitsSource;		//原始像素数据
	DWORD	lpDIBitsRender;		//用于显示到屏幕的像素数据
	DWORD	lpWave1;			//水波能量数据缓冲1
	DWORD	lpWave2;			//水波能量数据缓冲2
	// ;********************************************************************
	DWORD	dwBmpWidth;
	DWORD	dwBmpHeight;
	DWORD	dwDIByteWidth;		// = (dwBmpWidth * 3 + 3) and ~3
	DWORD	dwWaveByteWidth;	//= dwBmpWidth * 4
	DWORD	dwRandom;
	// ;********************************************************************
	// ; 特效参数
	// ;********************************************************************
	DWORD	dwEffectType;
	DWORD	dwEffectParam1;
	DWORD	dwEffectParam2;
	DWORD	dwEffectParam3;
	// ;********************************************************************
	// ; 用于行船特效
	// ;********************************************************************
	DWORD	dwEff2X;
	DWORD	dwEff2Y;
	DWORD	dwEff2XAdd;
	DWORD	dwEff2YAdd;
	DWORD	dwEff2Flip;
	// ;********************************************************************
	BITMAPINFO	stBmpInfo;
}WAVE_OBJECT,*PWAVE_OBJECT;

#ifdef __cplusplus
extern "C" {
#endif

// lpWaveObject --> 指向一个空的 WAVE_OBJECT 结构
// hWnd --> 要绘画水波效果的窗口，渲染后的图片将画到窗口的客户区中
// hBmp --> 背景图片，绘画的范围大小同背景图片大小
// dwTime --> 刷新的时间间隔（毫秒），建议值：10～30
// dwType --> =0 表示圆形水波，=1表示椭圆型水波（用于透视效果）
// 返回值：eax = 0（成功，对象被初始化），eax = 1（失败）
DWORD __stdcall _WaveInit(PWAVE_OBJECT lpWaveObject,HWND hWnd,HBITMAP hBmp,DWORD dwTime,DWORD dwType);

// dwPosX,dwPosY --> 扔下石头的位置
// dwStoneSize --> 石头的大小，即初始点的大小，建议值：0～5
// dwStoneWeight --> 石头的重量，决定了波最后扩散的范围大小，建议值：10～1000
 DWORD __stdcall _WaveDropStone(PWAVE_OBJECT lpWaveObject,DWORD dwPosX,DWORD dwPosY,DWORD dwStoneSize,DWORD dwStoneWeight);

// dwParam1,dwParam2,dwParam3 --> 效果参数，对不同的特效类型参数含义不同
// dwEffectType --> 特效类型
// 0 --> 关闭特效
// 1 --> 下雨，Param1＝密集速度（0最密，越大越稀疏），建议值：0～30
//             Param2＝最大雨点直径，建议值：0～5
//             Param3＝最大雨点重量，建议值：50～250
// 2 --> 汽艇，Param1＝速度（0最慢，越大越快），建议值：0～8
//             Param2＝船大小，建议值：0～4
//            Param3＝水波扩散的范围，建议值：100～500
// 3 --> 风浪，Param1＝密度（越大越密），建议值：50～300
//             Param2＝大小，建议值：2～5
//             Param3＝能量，建议值：5～10
DWORD __stdcall _WaveEffect(PWAVE_OBJECT lpWaveObject,DWORD dwEffectType,DWORD dwParam1,DWORD dwParam2,DWORD dwParam3);

//窗口客户区强制更新（用于在窗口的WM_PAINT消息中强制更新客户端）
DWORD __stdcall _WaveUpdateFrame(PWAVE_OBJECT lpWaveObject,HDC hDc,BOOL bForceUpdate);

//使用完毕后，必须将水波对象释放（本函数将释放申请的缓冲区内存等资源）
DWORD __stdcall _WaveFree(PWAVE_OBJECT lpWaveObject);

#ifdef __cplusplus
}
#endif
