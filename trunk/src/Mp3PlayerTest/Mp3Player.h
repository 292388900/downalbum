
///////////////////////////////////////////////////////////
// Mp3Player.h
//
//  
//
// Created On:	2011��4��2�� 10:14:23
// Original author:	xiaolin
///////////////////////////////////////////////////////////

#pragma once
#pragma pack(1)

#define GetBit(value, pos) ( (value >> pos) & 0x01)
#define DecodeSyncSafeInteger4(value1, value2, value3, value4) (((value1 & 0x7F) << 21) | ((value2 & 0x7F) << 14) | ((value3 & 0x7F) << 7) | (value4 & 0x7F))
#define DecodeSyncSafeInteger3(value1, value2, value3) (((value1 & 0x7F) << 14) | ((value2 & 0x7F) << 7) | (value3 & 0x7F))


/*
 ID3V2ͷ�ṹ
*/
typedef struct tag_ID3V2HEADER
{
	char Magic[3];				//����ΪID3
	byte MajorVersion;			//���汾��
	byte RevisionNumber;		//���汾�ţ�һ��Ϊ0
	byte Flags;					//���
	byte Size[4];				//������ͷ�ı�ǩ��С������һ����β��Сβ��˫�֣���Ҫ�����õ�
}ID3V2HEADER,*PID3V2HEADER;


/*
 ID3V2֡�ṹ
*/
typedef struct tag_ID3V2FRAME
{
	DWORD dwMagic;				//��һЩ�ַ�����
	byte  Size[4];				//������ͷ��֡��С������һ����β��Сβ��˫�֣���Ҫ�����õ�
	WORD  wFlags;				//���
}ID3V2FRAME,*PID3V2FRAME;


/*
 mp3֡ͷ�ṹ��ֻ��ȡһЩ���õ���Ϣ�������ĺ���
*/
typedef struct tag_MP3FRAMEHEADER
{
	char* pAddr;
	int   nMpegVersion;			//00-MPEG 2.5   01-δ����     10-MPEG 2     11-MPEG 1
	int   nLayer;				//00-δ����     01-Layer 3    10-Layer 2    11-Layer 1
	bool  bCrc;					//CRCУ��
	int   nBitrate;				//λ��
	int   nSamplingFreq;		//����Ƶ��
	int   nPadding;				//֡������
	int   nMode;				//����ģʽ,��ʾ����:00-������Stereo;01-Joint Stereo;10-˫����;11-������

	int   nSize;				//֡��������õ�,���㹫ʽ��
/*
. mpeg1.0		layer1 :	֡��= (48*bitrate)/sampling_freq + padding
				layer2&3:	֡��= (144*bitrate)/sampling_freq + padding
. mpeg2.0       layer1 :	֡��= (24*bitrate)/sampling_freq + padding
				layer2&3 :	֡��= (72*bitrate)/sampling_freq + padding
*/


}MP3FRAMEHEADER,*PMP3FRAMEHEADER;



class CMp3Player
{
public:
	CMp3Player();
	~CMp3Player();

public:
	BOOL LoadFile(const CString& strFilePath);
	void CloseFile();
	void GetMp3Info();
	//BOOL GetID3V1Info();	//����Ͳ���Ҫ�ˣ��ܶ��ļ�������ṹ��û����Ϣ

	BOOL GetFirstMp3Frame();
	BOOL GetNextMp3Frame();
protected:

private:
	void DecodeID3V2Frame4();
	unsigned int id3_decode_str(unsigned char *str, unsigned int length, unsigned char *dest, unsigned int encoding);
	BOOL GetMp3FrameHeaderInfo(byte*pMagic,MP3FRAMEHEADER&tMp3FrameHdr);
private:

	CString m_strFilePath;

	HANDLE  m_hFile;
	HANDLE  m_hFileMap;
	char*	m_pMapAddr;
	char*	m_pId3v1Hdr;		//ID3V1�Ľṹͷλ�ã��ڵ���128�ֽڴ�
	DWORD	m_dwFileSize;		//�����ļ���С

	/*
	 ����mp3��һЩ��Ϣ
	*/
	CString m_strTitle;			//����
	CString m_strArtist;		//���ּ�
	CString m_strAlbum;			//ר��
	CString m_strYear;			//ʱ��
	CString m_strComment;		//ע�ͱ�ע��Ϣ
	CString m_strTrackNum;		//���죨���ţ�����λ����ʱû��
	CString m_strGenre;			//���ַ��

	MP3FRAMEHEADER m_tMp3FrameHdr;

	/*
	 ������Ϣ
	*/
	CString m_strLastError;
};

/*

����ʽ 1��
֡��С���������¹�ʽ���㣺 
Frame Size = ( (Samples Per Frame / 8 * Bitrate) / Sampling Rate) + Padding Size 
֡��С = ����ÿ֡�Ĳ�����  /8 * �����ʣ� /�����ʣ�+ ����С

����ʽ 2��
�����¹�ʽ���Լ��� MP3�Ĳ���ʱ�䣬����Ϊ��λ�� 
CBR Duration = File Size / Bitrate * 8 
����ʱ�� = �ļ���С / ������ * 8

*/
