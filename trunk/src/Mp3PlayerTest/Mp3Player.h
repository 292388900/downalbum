
///////////////////////////////////////////////////////////
// Mp3Player.h
//
//  
//
// Created On:	2011年4月2日 10:14:23
// Original author:	xiaolin
///////////////////////////////////////////////////////////

#pragma once
#pragma pack(1)

#define GetBit(value, pos) ( (value >> pos) & 0x01)
#define DecodeSyncSafeInteger4(value1, value2, value3, value4) (((value1 & 0x7F) << 21) | ((value2 & 0x7F) << 14) | ((value3 & 0x7F) << 7) | (value4 & 0x7F))
#define DecodeSyncSafeInteger3(value1, value2, value3) (((value1 & 0x7F) << 14) | ((value2 & 0x7F) << 7) | (value3 & 0x7F))


/*
 ID3V2头结构
*/
typedef struct tag_ID3V2HEADER
{
	char Magic[3];				//必须为ID3
	byte MajorVersion;			//主版本号
	byte RevisionNumber;		//副版本号，一般为0
	byte Flags;					//标记
	byte Size[4];				//不含此头的标签大小，不是一个大尾或小尾的双字，需要计算后得到
}ID3V2HEADER,*PID3V2HEADER;


/*
 ID3V2帧结构
*/
typedef struct tag_ID3V2FRAME
{
	DWORD dwMagic;				//是一些字符含义
	byte  Size[4];				//不含此头的帧大小，不是一个大尾或小尾的双字，需要计算后得到
	WORD  wFlags;				//标记
}ID3V2FRAME,*PID3V2FRAME;


/*
 mp3帧头结构，只提取一些有用的信息，其他的忽略
*/
typedef struct tag_MP3FRAMEHEADER
{
	char* pAddr;
	int   nMpegVersion;			//00-MPEG 2.5   01-未定义     10-MPEG 2     11-MPEG 1
	int   nLayer;				//00-未定义     01-Layer 3    10-Layer 2    11-Layer 1
	bool  bCrc;					//CRC校验
	int   nBitrate;				//位率
	int   nSamplingFreq;		//采样频率
	int   nPadding;				//帧长调节
	int   nMode;				//声道模式,表示声道:00-立体声Stereo;01-Joint Stereo;10-双声道;11-单声道

	int   nSize;				//帧长，计算得到,计算公式：
/*
. mpeg1.0		layer1 :	帧长= (48*bitrate)/sampling_freq + padding
				layer2&3:	帧长= (144*bitrate)/sampling_freq + padding
. mpeg2.0       layer1 :	帧长= (24*bitrate)/sampling_freq + padding
				layer2&3 :	帧长= (72*bitrate)/sampling_freq + padding
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
	//BOOL GetID3V1Info();	//这个就不需要了，很多文件的这个结构都没有信息

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
	char*	m_pId3v1Hdr;		//ID3V1的结构头位置，在倒数128字节处
	DWORD	m_dwFileSize;		//整个文件大小

	/*
	 关于mp3的一些信息
	*/
	CString m_strTitle;			//名称
	CString m_strArtist;		//音乐家
	CString m_strAlbum;			//专辑
	CString m_strYear;			//时间
	CString m_strComment;		//注释备注信息
	CString m_strTrackNum;		//音轨（曲号）保留位，有时没有
	CString m_strGenre;			//音乐风格

	MP3FRAMEHEADER m_tMp3FrameHdr;

	/*
	 错误信息
	*/
	CString m_strLastError;
};

/*

【公式 1】
帧大小可以用如下公式计算： 
Frame Size = ( (Samples Per Frame / 8 * Bitrate) / Sampling Rate) + Padding Size 
帧大小 = （（每帧的采样数  /8 * 比特率） /采样率）+ 填充大小

【公式 2】
用如下公式可以计算 MP3的播放时间，以秒为单位： 
CBR Duration = File Size / Bitrate * 8 
播放时间 = 文件大小 / 比特率 * 8

*/
