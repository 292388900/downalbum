/*******************************************************************
; Author:	������
; Web	:	http://hi.baidu.com/asmcvc
; E-mail:	asmcvc@163.com
; Date	:	2009.03.11
; ˵��	:	�����Ʊ�Ļ��汾��ˮ��Ч������д
;********************************************************************/

#define		F_WO_ACTIVE			0001
#define		F_WO_NEED_UPDATE	0002
#define		F_WO_EFFECT			0004
#define		F_WO_ELLIPSE		0008

typedef struct _WAVE_OBJECT
{
	DWORD	hWnd;
	DWORD	dwFlag;				//�� F_WO_xxx ���
	// ;********************************************************************
	DWORD	hDcRender;
	DWORD	hBmpRender;
	DWORD	lpDIBitsSource;		//ԭʼ��������
	DWORD	lpDIBitsRender;		//������ʾ����Ļ����������
	DWORD	lpWave1;			//ˮ���������ݻ���1
	DWORD	lpWave2;			//ˮ���������ݻ���2
	// ;********************************************************************
	DWORD	dwBmpWidth;
	DWORD	dwBmpHeight;
	DWORD	dwDIByteWidth;		// = (dwBmpWidth * 3 + 3) and ~3
	DWORD	dwWaveByteWidth;	//= dwBmpWidth * 4
	DWORD	dwRandom;
	// ;********************************************************************
	// ; ��Ч����
	// ;********************************************************************
	DWORD	dwEffectType;
	DWORD	dwEffectParam1;
	DWORD	dwEffectParam2;
	DWORD	dwEffectParam3;
	// ;********************************************************************
	// ; �����д���Ч
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

// lpWaveObject --> ָ��һ���յ� WAVE_OBJECT �ṹ
// hWnd --> Ҫ�滭ˮ��Ч���Ĵ��ڣ���Ⱦ���ͼƬ���������ڵĿͻ�����
// hBmp --> ����ͼƬ���滭�ķ�Χ��Сͬ����ͼƬ��С
// dwTime --> ˢ�µ�ʱ���������룩������ֵ��10��30
// dwType --> =0 ��ʾԲ��ˮ����=1��ʾ��Բ��ˮ��������͸��Ч����
// ����ֵ��eax = 0���ɹ������󱻳�ʼ������eax = 1��ʧ�ܣ�
DWORD __stdcall _WaveInit(PWAVE_OBJECT lpWaveObject,HWND hWnd,HBITMAP hBmp,DWORD dwTime,DWORD dwType);

// dwPosX,dwPosY --> ����ʯͷ��λ��
// dwStoneSize --> ʯͷ�Ĵ�С������ʼ��Ĵ�С������ֵ��0��5
// dwStoneWeight --> ʯͷ�������������˲������ɢ�ķ�Χ��С������ֵ��10��1000
 DWORD __stdcall _WaveDropStone(PWAVE_OBJECT lpWaveObject,DWORD dwPosX,DWORD dwPosY,DWORD dwStoneSize,DWORD dwStoneWeight);

// dwParam1,dwParam2,dwParam3 --> Ч���������Բ�ͬ����Ч���Ͳ������岻ͬ
// dwEffectType --> ��Ч����
// 0 --> �ر���Ч
// 1 --> ���꣬Param1���ܼ��ٶȣ�0���ܣ�Խ��Խϡ�裩������ֵ��0��30
//             Param2��������ֱ��������ֵ��0��5
//             Param3������������������ֵ��50��250
// 2 --> ��ͧ��Param1���ٶȣ�0������Խ��Խ�죩������ֵ��0��8
//             Param2������С������ֵ��0��4
//            Param3��ˮ����ɢ�ķ�Χ������ֵ��100��500
// 3 --> ���ˣ�Param1���ܶȣ�Խ��Խ�ܣ�������ֵ��50��300
//             Param2����С������ֵ��2��5
//             Param3������������ֵ��5��10
DWORD __stdcall _WaveEffect(PWAVE_OBJECT lpWaveObject,DWORD dwEffectType,DWORD dwParam1,DWORD dwParam2,DWORD dwParam3);

//���ڿͻ���ǿ�Ƹ��£������ڴ��ڵ�WM_PAINT��Ϣ��ǿ�Ƹ��¿ͻ��ˣ�
DWORD __stdcall _WaveUpdateFrame(PWAVE_OBJECT lpWaveObject,HDC hDc,BOOL bForceUpdate);

//ʹ����Ϻ󣬱��뽫ˮ�������ͷţ����������ͷ�����Ļ������ڴ����Դ��
DWORD __stdcall _WaveFree(PWAVE_OBJECT lpWaveObject);

#ifdef __cplusplus
}
#endif
