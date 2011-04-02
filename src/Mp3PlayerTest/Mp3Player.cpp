
///////////////////////////////////////////////////////////
// Mp3Player.cpp
//
//  
//
// Created On:	2011年4月2日 10:14:32
// Original author:	xxzhu
///////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Mp3Player.h"


//////////////////////////////////////////////////////////////////////////

static
unsigned long const g_BitrateTable[3][15] = {
	/* MPEG-1 */
	{ 0,  32000,  64000,  96000, 128000, 160000, 192000, 224000,  /* Layer I   */
	256000, 288000, 320000, 352000, 384000, 416000, 448000 },
	{ 0,  32000,  48000,  56000,  64000,  80000,  96000, 112000,  /* Layer II  */
	128000, 160000, 192000, 224000, 256000, 320000, 384000 },
	{ 0,  32000,  40000,  48000,  56000,  64000,  80000,  96000,  /* Layer III */
	112000, 128000, 160000, 192000, 224000, 256000, 320000 },


// 	/* MPEG-2 LSF */
// 	{ 0,  32000,  48000,  56000,  64000,  80000,  96000, 112000,  /* Layer I   */
// 	128000, 144000, 160000, 176000, 192000, 224000, 256000 },
// 	{ 0,   8000,  16000,  24000,  32000,  40000,  48000,  56000,  /* Layers    */
// 	64000,  80000,  96000, 112000, 128000, 144000, 160000 } /* II & III  */
};

static
unsigned int const g_SampleFreqTable[3] = { 44100, 48000, 32000 };

//////////////////////////////////////////////////////////////////////////


CMp3Player::CMp3Player()
{
	m_hFile=INVALID_HANDLE_VALUE;
	m_hFileMap=NULL;
	m_pMapAddr=NULL;
	m_pId3v1Hdr=NULL;
	m_dwFileSize=0;
}


CMp3Player::~CMp3Player()
{

}


BOOL CMp3Player::LoadFile(const CString& strFilePath)
{

	/*
	 关闭上个被打开的文件
	*/
	CloseFile();


	/*
	 创建内存映射文件
	*/
	m_hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if( m_hFile==INVALID_HANDLE_VALUE )
	{
		m_strLastError.Format("%s error: CreateFile %s", __FUNCTIONW__, strFilePath);
		return FALSE;	
	}

	m_dwFileSize=::GetFileSize(m_hFile,NULL);
	if ( m_dwFileSize<=0 )
	{
		m_strLastError.Format("%s error: GetFileSize %s", __FUNCTIONW__, strFilePath);
		return FALSE;	
	}

	m_hFileMap = CreateFileMapping(m_hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if( m_hFileMap==NULL )
	{
		m_strLastError.Format("%s error: CreateFileMapping %s", __FUNCTIONW__, strFilePath);
		CloseHandle(m_hFile);
		m_hFile=INVALID_HANDLE_VALUE;
		return FALSE;

	}

	m_pMapAddr = (char*)MapViewOfFile(m_hFileMap, FILE_MAP_READ, 0, 0, 0);
	if( m_pMapAddr==NULL )
	{
		m_strLastError.Format("%s error: MapViewOfFile %s", __FUNCTIONW__, strFilePath);
		CloseHandle(m_hFileMap);
		m_hFileMap=NULL;
		CloseHandle(m_hFile);
		m_hFile=INVALID_HANDLE_VALUE;
		return FALSE;

	}

	
	/*
	 简单验证下是不是合法的mp3文件
	*/
	PID3V2HEADER pID3V2Hdr=(PID3V2HEADER)m_pMapAddr;
	if ( pID3V2Hdr->Magic[0]!='I' || pID3V2Hdr->Magic[1]!='D' || pID3V2Hdr->Magic[2]!='3' )
	{
		m_strLastError.Format("%s error: invalid mp3 file %s", __FUNCTIONW__, strFilePath);
		this->CloseFile();
		return FALSE;
	}



	m_strFilePath=strFilePath;
	m_strLastError.Format("%s load %s OK", __FUNCTIONW__, m_strFilePath);


	/*
	 检测TAG位置,这段代码不是必须的
	*/
	if ( m_dwFileSize>128+sizeof(tag_ID3V2HEADER) )
	{
		m_pId3v1Hdr=m_pMapAddr+m_dwFileSize-128;
		if ( m_pId3v1Hdr[0]!='T' || m_pId3v1Hdr[1]!='A' || m_pId3v1Hdr[2]!='G' )
		{
			m_strLastError.Format("%s load %s，no id3v1 header", __FUNCTIONW__, m_strFilePath);
			m_pId3v1Hdr=NULL;
		}
	}


	return TRUE;
}


void CMp3Player::CloseFile()
{
	if ( m_pMapAddr!=NULL )
	{
		UnmapViewOfFile(m_pMapAddr);
		m_pMapAddr=NULL;
	}

	if ( m_hFileMap!=NULL )
	{
		CloseHandle(m_hFileMap);
		m_hFileMap=NULL;
	}

	if ( m_hFile!=INVALID_HANDLE_VALUE )
	{
		::CloseHandle(m_hFile);
		m_hFile=INVALID_HANDLE_VALUE;
	}

	m_strLastError.Format("%s close %s OK", __FUNCTIONW__, m_strFilePath);
}


void CMp3Player::GetMp3Info()
{
	PID3V2HEADER pID3V2Hdr=(PID3V2HEADER)m_pMapAddr;
	PID3V2FRAME pID3V2Frame=(PID3V2FRAME)(pID3V2Hdr+1);

	if ( pID3V2Hdr->MajorVersion==1 )
	{
	}
	else if ( pID3V2Hdr->MajorVersion==2 )
	{
	}
	else if ( pID3V2Hdr->MajorVersion==3 )
	{
	}
	else if ( pID3V2Hdr->MajorVersion==4 )
	{
		DecodeID3V2Frame4();
	}
	else
	{
	}


}

void CMp3Player::DecodeID3V2Frame4()
{
	PID3V2HEADER pID3V2Hdr=(PID3V2HEADER)m_pMapAddr;
	PID3V2FRAME pID3V2Frame=(PID3V2FRAME)(pID3V2Hdr+1);
	CString*pstrDst=NULL;

	DWORD dwDataOffset=DecodeSyncSafeInteger4( pID3V2Hdr->Size[0], 
		pID3V2Hdr->Size[1], pID3V2Hdr->Size[2], pID3V2Hdr->Size[3] );
	DWORD dwFrameSize=DecodeSyncSafeInteger4( pID3V2Frame->Size[0], 
		pID3V2Frame->Size[1], pID3V2Frame->Size[2], pID3V2Frame->Size[3] );
		
	while ( dwFrameSize>0 && (DWORD)(pID3V2Frame+1)+dwFrameSize<=(DWORD)pID3V2Hdr+dwDataOffset )
	{
		
		if ( pID3V2Frame->dwMagic==DWORD('1EPT') )		//TPE1
		{
			pstrDst=&m_strArtist;

		}
		else if ( pID3V2Frame->dwMagic==DWORD('2TIT') )	//TIT2
		{
			pstrDst=&m_strTitle;
		}
		else if ( pID3V2Frame->dwMagic==DWORD('BLAT') )	//TALB
		{
			pstrDst=&m_strAlbum;
		}
		else if ( pID3V2Frame->dwMagic==DWORD('KCRT') )	//TRCK
		{
			pstrDst=&m_strTrackNum;
		}
		else if ( pID3V2Frame->dwMagic==DWORD('CRDT') )	//TDRC
		{
			pstrDst=&m_strYear;
		}
		else if ( pID3V2Frame->dwMagic==DWORD('NOCT') )	//TCON
		{
			pstrDst=&m_strGenre;
		}
		else if ( pID3V2Frame->dwMagic==DWORD('MMOC') )	//COMM
		{
			pstrDst=&m_strComment;
		}
		else
		{
			m_strLastError.Format("%s unkown fram magic", __FUNCTIONW__);
		}

		/*
		 还需要一个解码操作，第一个字符作为解码方法的标识
		*/
		unsigned char*pSrc=(unsigned char*)(pID3V2Frame+1);
		unsigned char*pDst=(unsigned char*)pstrDst->GetBuffer(dwFrameSize*2);	//足够大的缓冲区
		memset(pDst,0,dwFrameSize*2);
		id3_decode_str(pSrc+1,dwFrameSize-1,pDst,pSrc[0]);
		pstrDst->ReleaseBuffer();
		printf_s("%s\n",*pstrDst);

		pID3V2Frame=(PID3V2FRAME)( (char*)(pID3V2Frame+1)+dwFrameSize );
		dwFrameSize=DecodeSyncSafeInteger4( pID3V2Frame->Size[0], 
			pID3V2Frame->Size[1], pID3V2Frame->Size[2], pID3V2Frame->Size[3] );

	}//end while
}

unsigned int CMp3Player::id3_decode_str(unsigned char *str, unsigned int length, unsigned char *dest, unsigned int encoding)
{
	unsigned int i;


	switch(encoding)
	{
	case 0: // Latin1
		memmove(dest, str, length);
		dest[length] = 0;
		break;

	case 1: // UTF-16 [UTF-16] encoded Unicode [UNICODE] with BOM
		{

			unsigned char *ptr = str;
			int byteorder = (( str[0] << 8 ) | str[1] );

			if (byteorder == 0xFFFE) // LE
			{
				ptr += 2;
			}
			else if (byteorder == 0xFEFF)  // BE
			{
				for(i = 0; i < length - 1; i += 2)
				{
					unsigned char c;
					c = str[i];
					str[i] = str[i + 1];
					str[i + 1] = c;	
				}

				ptr += 2;
			}

			if(length > 5)
			{
				if(ptr[0] == 0 && ptr[1] == 0)
					ptr += 2;

			}



			char *out = (char*) malloc(length);
			if(out == 0)
				return 0;

			length = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, (LPCWSTR) ptr, -1, out, length, NULL, NULL);
			if(length == 0)
			{	
				free(out);
				return 0;
			}
			memcpy(dest, out, length);
			free(out);

		}
		break;

	case 2:
		{

			char *out = (char*) malloc(length + 1);
			if(out == 0)
				return 0;

			length = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, (LPCWSTR) str, -1, out, length, NULL, NULL);
			memcpy(dest, out, length);
			free(out);

		}
		break;

	case 3: // UTF-8 [UTF-8] encoded Unicode [UNICODE]. Terminated with $00.
		{
			int size = MultiByteToWideChar(CP_UTF8, 0, (char*) str, length, NULL, 0);
			if(size > 0)
			{
				WORD *wide = new WORD[size+1];
				if(!wide)
					return 0;


				if(MultiByteToWideChar(CP_UTF8, 0, (char*) str, length, (LPWSTR) wide, size))
				{
					wide[size]=0;
					length = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR) wide, size, (char*) dest, length, NULL, NULL);

				}
				
				if ( wide!=NULL )
				{
					delete []wide;
				}
			}
			else
				return 0;

		}
		break;

	}


	return length;


}


BOOL CMp3Player::GetMp3FrameHeaderInfo(byte*pMagic,MP3FRAMEHEADER&tMp3FrameHdr)
{
	int nMulti=0;
	DWORD dwMagic=0;

	if ( pMagic==NULL )
	{
		m_strLastError.Format("%s error: pMagic==NULL", __FUNCTIONW__);
		return NULL;
	}

	if ( m_pId3v1Hdr!=NULL )
	{
		if ( pMagic>=(byte*)m_pId3v1Hdr )
		{
			m_strLastError.Format("%s error: mp3 frame over", __FUNCTIONW__);
			return NULL;
		}
	}
	else
	{
		if ( pMagic>=(byte*)m_pMapAddr+m_dwFileSize )
		{
			m_strLastError.Format("%s error: mp3 frame over", __FUNCTIONW__);
			return NULL;
		}
	}

	if ( pMagic[0]!=0xFF )
	{
		printf_s("%s 出现错误,文件偏移：%08X 处第一字节不为0xFF\n",__FUNCTION__, pMagic-(byte*)m_pMapAddr);
		return FALSE;
	}
	tMp3FrameHdr.pAddr=(char*)pMagic;

	dwMagic = (pMagic[0]<<24) + (pMagic[1]<<16) + (pMagic[2]<<8) + pMagic[3];

	//00-MPEG 2.5   01-未定义     10-MPEG 2     11-MPEG 1
	tMp3FrameHdr.nMpegVersion = ( dwMagic>>19 ) & 0x3;
	if ( tMp3FrameHdr.nMpegVersion==0 ){		//MPEG 2.5
		tMp3FrameHdr.nMpegVersion=3;
	}else if ( tMp3FrameHdr.nMpegVersion==1 ){	//未定义
		tMp3FrameHdr.nMpegVersion=0;
	}else if ( tMp3FrameHdr.nMpegVersion==2 ){	//MPEG 2
		tMp3FrameHdr.nMpegVersion=2;
	}else if ( tMp3FrameHdr.nMpegVersion==3 ){	//MPEG 1
		tMp3FrameHdr.nMpegVersion=1;
	}

	//00-未定义     01-Layer 3    10-Layer 2    11-Layer 1
	tMp3FrameHdr.nLayer = ( dwMagic>>17 ) & 0x3;
	if ( tMp3FrameHdr.nLayer==0 ){				//未定义
		tMp3FrameHdr.nLayer=0;
	}else if ( tMp3FrameHdr.nLayer==1 ){		//Layer 3 
		tMp3FrameHdr.nLayer=3;
	}else if ( tMp3FrameHdr.nLayer==2 ){		//Layer 2
		tMp3FrameHdr.nLayer=2;
	}else if ( tMp3FrameHdr.nLayer==3 ){		//Layer 1
		tMp3FrameHdr.nLayer=1;
	}

	tMp3FrameHdr.bCrc = ( dwMagic>>16 ) & 0x1;

	tMp3FrameHdr.nBitrate = ( dwMagic>>12 ) & 0xF;
	tMp3FrameHdr.nBitrate=g_BitrateTable[tMp3FrameHdr.nLayer-1][tMp3FrameHdr.nBitrate];

	tMp3FrameHdr.nSamplingFreq = ( dwMagic>>10 ) & 0x3;
	tMp3FrameHdr.nSamplingFreq=g_SampleFreqTable[tMp3FrameHdr.nSamplingFreq];

	tMp3FrameHdr.nPadding = ( dwMagic>>9 ) & 0x1;
	tMp3FrameHdr.nMode = ( dwMagic>>6 ) & 0x3;
	
	if ( tMp3FrameHdr.nMpegVersion==1 )			//11-MPEG 1
	{
		if ( tMp3FrameHdr.nLayer==1 )			//Layer 1
		{
			nMulti=48;
		}
		else if ( tMp3FrameHdr.nLayer==2 )		//Layer 2
		{
			nMulti=144;
		}
		else if ( tMp3FrameHdr.nLayer==3 )		//Layer 3
		{
			nMulti=144;
		}
		else									//
		{
		}
	}
	else
	{
		//不处理
	}
	
	tMp3FrameHdr.nSize=nMulti*tMp3FrameHdr.nBitrate/tMp3FrameHdr.nSamplingFreq + tMp3FrameHdr.nPadding;
	//printf_s("帧大小：%08X\n",tMp3FrameHdr.nSize);
	return TRUE;
}


BOOL CMp3Player::GetFirstMp3Frame()
{
	PID3V2HEADER pID3V2Hdr=(PID3V2HEADER)m_pMapAddr;

	DWORD dwDataOffset=DecodeSyncSafeInteger4( pID3V2Hdr->Size[0], 
		pID3V2Hdr->Size[1], pID3V2Hdr->Size[2], pID3V2Hdr->Size[3] );

	byte* pMagic=(byte*)( (DWORD)(pID3V2Hdr+1)+dwDataOffset );
	return GetMp3FrameHeaderInfo(pMagic,m_tMp3FrameHdr);
}


BOOL CMp3Player::GetNextMp3Frame()
{
	byte* pMagic=(byte*)m_tMp3FrameHdr.pAddr+m_tMp3FrameHdr.nSize;
	return GetMp3FrameHeaderInfo(pMagic,m_tMp3FrameHdr);
}