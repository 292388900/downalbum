
#include "Garbage.h"
using namespace Star::Encrypt;
//////////////////////////////////////////////////////////////////////////

#define GetMemberFuncAddr_VC8(FuncAddr,FuncType)\
{	\
	__asm{mov eax,offset FuncType};\
	__asm{mov FuncAddr, eax};\
}

void Garbage::Encode(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	typedef void(*dump)(unsigned char*pIn,unsigned char*pOut,int nSize);

	dump func[23]={};
	dump pAfunc=NULL;

	GetMemberFuncAddr_VC8(pAfunc,Dump5);
	func[1]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump2);
	func[2]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump7);
	func[3]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump3);
	func[4]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump8);
	func[5]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump1);
	func[6]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump4);
	func[7]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump6);
	func[8]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump9);
	func[9]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump20);
	func[10]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump21);
	func[11]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump19);
	func[12]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump17);
	func[13]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump10);
	func[14]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump12);
	func[15]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump15);
	func[16]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump11);
	func[17]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump10);
	func[18]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump16);
	func[19]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump18);
	func[20]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump14);
	func[21]=pAfunc;
	GetMemberFuncAddr_VC8(pAfunc,Dump13);
	func[22]=pAfunc;
	
	int n=sizeof(func)/sizeof(dump);
	for (int i=1;i<n;i++)
	{
		func[i](pIn,pOut,nSize);
	}
}

//////////////////////////////////////////////////////////////////////////
void Garbage::Dump1(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}

	char v1;
	char v2;
	for (int i=0;i<nSize;i++)
	{
		v1=pIn[i];
		v2=((v1 << 4 ^ v1 >> 5) + v1);
		pOut[i]=v2;
	}
}

void Garbage::Dump2(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=2)
	{
		return;
	}
	
	for (int i=0;i<nSize-1;i++)
	{
		pOut[i+1]=pIn[i]^pIn[i+1];
	}
	pOut[0]=pIn[0]^0x89;
}

void Garbage::Dump3(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}
	for (int i=0;i<nSize-1;i++)
	{
		pOut[i+1]=pIn[i]+pIn[i+1];
	}
	pOut[0]=pIn[0]+0x56;
}

void Garbage::Dump4(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}

	for (int i=0;i<nSize-1;i++)
	{
		pOut[i+1]=pIn[i]-pIn[i+1];
	}
	pOut[0]=pIn[0]-0x35;
}

void Garbage::Dump5(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}

	for (int i=0;i<nSize-1;i++)
	{
		pOut[i+1]=pIn[i+1]-pIn[i];
	}
	pOut[0]=0x93-pIn[0];
}

void Garbage::Dump6(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}
	for (int i=0;i<nSize-1;i++)
	{
		pOut[i+1]=pIn[i]*pIn[i+1];
	}
	pOut[0]=pIn[0]*0x35;
}

void Garbage::Dump7(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}
	for (int i=0;i<nSize-1;i++)
	{
		if (pIn[i+1]!=0)
			pOut[i+1]=pIn[i]/pIn[i+1];
		else
			pOut[i+1]=pIn[i]-pIn[i+1];
	}
	pOut[0]=pIn[0]/0x18;
}

void Garbage::Dump8(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}
	memcpy(pOut,pIn,nSize);
	int i,j;
	unsigned char temp; 
	for(i=0;i<nSize-1;i++) 
	{
		for(j=i+1;j<nSize;j++) /*注重循环的上下限*/ 
		{
			if(pOut[i]>pOut[j]) 
			{ 
				temp=pOut[i]; 
				pOut[i]=pOut[j]; 
				pOut[j]=temp; 
			}
		}
	}
}

void Garbage::Dump9(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}
	memcpy(pOut,pIn,nSize);
	int i,j;
	unsigned char temp; 
	for(i=0;i<nSize-1;i++) 
	{
		for(j=i+1;j<nSize;j++) /*注重循环的上下限*/ 
		{
			if(pOut[i]<pOut[j]) 
			{ 
				temp=pOut[i]; 
				pOut[i]=pOut[j]; 
				pOut[j]=temp; 
			}
		}
	}

}
		/*****************************/
		/*将des加密表数据掺杂其中迷惑*/
		/*****************************/

void Garbage::Dump10(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}
	// initial permutation IP
	const static unsigned char IP_Table[64] = {
		58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17,  9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
	};

	memcpy(pOut,pIn,nSize);
	nSize=min( nSize,sizeof(IP_Table) );
	for (int i=0;i<nSize;i++)
	{
		pOut[i]=pOut[i]^IP_Table[i];
	}
}

void Garbage::Dump11(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}
	// final permutation IP^-1 
	const static char IPR_Table[64] = {
		40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
		38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
		36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
		34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25
	};

	memcpy(pOut,pIn,nSize);
	nSize=min( nSize,sizeof(IPR_Table) );
	for (int i=0;i<nSize;i++)
	{
		pOut[i]=pOut[i]^IPR_Table[i];
	}
}

void Garbage::Dump12(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}
	// expansion operation matrix
	static const char E_Table[48] = {
		32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
		8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
		16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
		24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1
	};

	memcpy(pOut,pIn,nSize);
	nSize=min( nSize,sizeof(E_Table) );
	for (int i=0;i<nSize;i++)
	{
		pOut[i]=pOut[i]^E_Table[i];
	}
}

void Garbage::Dump13(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}
	// 32-bit permutation function P used on the output of the S-boxes 
	const static char P_Table[32] = {
		16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10,
		2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25
	};

	memcpy(pOut,pIn,nSize);
	nSize=min( nSize,sizeof(P_Table) );
	for (int i=0;i<nSize;i++)
	{
		pOut[i]=pOut[i]^P_Table[i];
	}
}

void Garbage::Dump14(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}
	// permuted choice table (key) 
	const static char PC1_Table[56] = {
		57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
		10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
		63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
		14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4
	};

	memcpy(pOut,pIn,nSize);
	nSize=min( nSize,sizeof(PC1_Table) );
	for (int i=0;i<nSize;i++)
	{
		pOut[i]=pOut[i]^PC1_Table[i];
	}
}

void Garbage::Dump15(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}

	// permuted choice key (table) 
	const static unsigned char PC2_Table[48] = {
		14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
		23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
		41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
		44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
	};

	memcpy(pOut,pIn,nSize);
	nSize=min( nSize,sizeof(PC2_Table) );
	for (int i=0;i<nSize;i++)
	{
		pOut[i]=pOut[i]^PC2_Table[i];
	}
}

		/**********************************/
		/*将Rijndael加密表数据掺杂其中迷惑*/
		/**********************************/

void Garbage::Dump16(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}
	const static unsigned char sm_alog[256] =
	{
		1, 3, 5, 15, 17, 51, 85, 255, 26, 46, 114, 150, 161, 248, 19, 53, 
		95, 225, 56, 72, 216, 115, 149, 164, 247, 2, 6, 10, 30, 34, 102, 170, 
		229, 52, 92, 228, 55, 89, 235, 38, 106, 190, 217, 112, 144, 171, 230, 49, 
		83, 245, 4, 12, 20, 60, 68, 204, 79, 209, 104, 184, 211, 110, 178, 205, 
		76, 212, 103, 169, 224, 59, 77, 215, 98, 166, 241, 8, 24, 40, 120, 136, 
		131, 158, 185, 208, 107, 189, 220, 127, 129, 152, 179, 206, 73, 219, 118, 154, 
		181, 196, 87, 249, 16, 48, 80, 240, 11, 29, 39, 105, 187, 214, 97, 163, 
		254, 25, 43, 125, 135, 146, 173, 236, 47, 113, 147, 174, 233, 32, 96, 160, 
		251, 22, 58, 78, 210, 109, 183, 194, 93, 231, 50, 86, 250, 21, 63, 65, 
		195, 94, 226, 61, 71, 201, 64, 192, 91, 237, 44, 116, 156, 191, 218, 117, 
		159, 186, 213, 100, 172, 239, 42, 126, 130, 157, 188, 223, 122, 142, 137, 128, 
		155, 182, 193, 88, 232, 35, 101, 175, 234, 37, 111, 177, 200, 67, 197, 84, 
		252, 31, 33, 99, 165, 244, 7, 9, 27, 45, 119, 153, 176, 203, 70, 202, 
		69, 207, 74, 222, 121, 139, 134, 145, 168, 227, 62, 66, 198, 81, 243, 14, 
		18, 54, 90, 238, 41, 123, 141, 140, 143, 138, 133, 148, 167, 242, 13, 23, 
		57, 75, 221, 124, 132, 151, 162, 253, 28, 36, 108, 180, 199, 82, 246, 1
	};

	memcpy(pOut,pIn,nSize);
	nSize=min( nSize,sizeof(sm_alog) );
	for (int i=0;i<nSize;i++)
	{
		pOut[i]=pOut[i]^sm_alog[i];
	}
}

void Garbage::Dump17(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}
	const static unsigned char sm_log[256] =
	{
		0, 0, 25, 1, 50, 2, 26, 198, 75, 199, 27, 104, 51, 238, 223, 3, 
		100, 4, 224, 14, 52, 141, 129, 239, 76, 113, 8, 200, 248, 105, 28, 193, 
		125, 194, 29, 181, 249, 185, 39, 106, 77, 228, 166, 114, 154, 201, 9, 120, 
		101, 47, 138, 5, 33, 15, 225, 36, 18, 240, 130, 69, 53, 147, 218, 142, 
		150, 143, 219, 189, 54, 208, 206, 148, 19, 92, 210, 241, 64, 70, 131, 56, 
		102, 221, 253, 48, 191, 6, 139, 98, 179, 37, 226, 152, 34, 136, 145, 16, 
		126, 110, 72, 195, 163, 182, 30, 66, 58, 107, 40, 84, 250, 133, 61, 186, 
		43, 121, 10, 21, 155, 159, 94, 202, 78, 212, 172, 229, 243, 115, 167, 87, 
		175, 88, 168, 80, 244, 234, 214, 116, 79, 174, 233, 213, 231, 230, 173, 232, 
		44, 215, 117, 122, 235, 22, 11, 245, 89, 203, 95, 176, 156, 169, 81, 160, 
		127, 12, 246, 111, 23, 196, 73, 236, 216, 67, 31, 45, 164, 118, 123, 183, 
		204, 187, 62, 90, 251, 96, 177, 134, 59, 82, 161, 108, 170, 85, 41, 157, 
		151, 178, 135, 144, 97, 190, 220, 252, 188, 149, 207, 205, 55, 63, 91, 209, 
		83, 57, 132, 60, 65, 162, 109, 71, 20, 42, 158, 93, 86, 242, 211, 171, 
		68, 17, 146, 217, 35, 32, 46, 137, 180, 124, 184, 38, 119, 153, 227, 165, 
		103, 74, 237, 222, 197, 49, 254, 24, 13, 99, 140, 128, 192, 247, 112, 7
	};

	memcpy(pOut,pIn,nSize);
	nSize=min( nSize,sizeof(sm_log) );
	for (int i=0;i<nSize;i++)
	{
		pOut[i]=pOut[i]^sm_log[i];
	}
}

void Garbage::Dump18(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}
	const static unsigned char sm_S[256] =
	{
		99, 124, 119, 123, -14, 107, 111, -59, 48, 1, 103, 43, -2, -41, -85, 118, 
		-54, -126, -55, 125, -6, 89, 71, -16, -83, -44, -94, -81, -100, -92, 114, -64, 
		-73, -3, -109, 38, 54, 63, -9, -52, 52, -91, -27, -15, 113, -40, 49, 21, 
		4, -57, 35, -61, 24, -106, 5, -102, 7, 18, -128, -30, -21, 39, -78, 117, 
		9, -125, 44, 26, 27, 110, 90, -96, 82, 59, -42, -77, 41, -29, 47, -124, 
		83, -47, 0, -19, 32, -4, -79, 91, 106, -53, -66, 57, 74, 76, 88, -49, 
		-48, -17, -86, -5, 67, 77, 51, -123, 69, -7, 2, 127, 80, 60, -97, -88, 
		81, -93, 64, -113, -110, -99, 56, -11, -68, -74, -38, 33, 16, -1, -13, -46, 
		-51, 12, 19, -20, 95, -105, 68, 23, -60, -89, 126, 61, 100, 93, 25, 115, 
		96, -127, 79, -36, 34, 42, -112, -120, 70, -18, -72, 20, -34, 94, 11, -37, 
		-32, 50, 58, 10, 73, 6, 36, 92, -62, -45, -84, 98, -111, -107, -28, 121, 
		-25, -56, 55, 109, -115, -43, 78, -87, 108, 86, -12, -22, 101, 122, -82, 8, 
		-70, 120, 37, 46, 28, -90, -76, -58, -24, -35, 116, 31, 75, -67, -117, -118, 
		112, 62, -75, 102, 72, 3, -10, 14, 97, 53, 87, -71, -122, -63, 29, -98, 
		-31, -8, -104, 17, 105, -39, -114, -108, -101, 30, -121, -23, -50, 85, 40, -33, 
		-116, -95, -119, 13, -65, -26, 66, 104, 65, -103, 45, 15, -80, 84, -69, 22
	};
	memcpy(pOut,pIn,nSize);
	nSize=min( nSize,sizeof(sm_S) );
	for (int i=0;i<nSize;i++)
	{
		pOut[i]=pOut[i]^sm_S[i];
	}
}

void Garbage::Dump19(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}
	const static unsigned char sm_Si[256] =
	{
		82, 9, 106, -43, 48, 54, -91, 56, -65, 64, -93, -98, -127, -13, -41, -5, 
		124, -29, 57, -126, -101, 47, -1, -121, 52, -114, 67, 68, -60, -34, -23, -53, 
		84, 123, -108, 50, -90, -62, 35, 61, -18, 76, -107, 11, 66, -6, -61, 78, 
		8, 46, -95, 102, 40, -39, 36, -78, 118, 91, -94, 73, 109, -117, -47, 37, 
		114, -8, -10, 100, -122, 104, -104, 22, -44, -92, 92, -52, 93, 101, -74, -110, 
		108, 112, 72, 80, -3, -19, -71, -38, 94, 21, 70, 87, -89, -115, -99, -124, 
		-112, -40, -85, 0, -116, -68, -45, 10, -9, -28, 88, 5, -72, -77, 69, 6, 
		-48, 44, 30, -113, -54, 63, 15, 2, -63, -81, -67, 3, 1, 19, -118, 107, 
		58, -111, 17, 65, 79, 103, -36, -22, -105, -14, -49, -50, -16, -76, -26, 115, 
		-106, -84, 116, 34, -25, -83, 53, -123, -30, -7, 55, -24, 28, 117, -33, 110, 
		71, -15, 26, 113, 29, 41, -59, -119, 111, -73, 98, 14, -86, 24, -66, 27, 
		-4, 86, 62, 75, -58, -46, 121, 32, -102, -37, -64, -2, 120, -51, 90, -12, 
		31, -35, -88, 51, -120, 7, -57, 49, -79, 18, 16, 89, 39, -128, -20, 95, 
		96, 81, 127, -87, 25, -75, 74, 13, 45, -27, 122, -97, -109, -55, -100, -17, 
		-96, -32, 59, 77, -82, 42, -11, -80, -56, -21, -69, 60, -125, 83, -103, 97, 
		23, 43, 4, 126, -70, 119, -42, 38, -31, 105, 20, 99, 85, 33, 12, 125
	};

	memcpy(pOut,pIn,nSize);
	nSize=min( nSize,sizeof(sm_Si) );
	for (int i=0;i<nSize;i++)
	{
		pOut[i]=pOut[i]^sm_Si[i];
	}
}

void Garbage::Dump20(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}
	const static int sm_T1[256] =
	{
		-966564955, -126059388, -294160487, -159679603, 
		-855539, -697603139, -563122255, -1849309868, 
		1613770832, 33620227, -832084055, 1445669757, 
		-402719207, -1244145822, 1303096294, -327780710, 
		-1882535355, 528646813, -1983264448, -92439161, 
		-268764651, -1302767125, -1907931191, -68095989, 
		1101901292, -1277897625, 1604494077, 1169141738, 
		597466303, 1403299063, -462261610, -1681866661, 
		1974974402, -503448292, 1033081774, 1277568618, 
		1815492186, 2118074177, -168298750, -2083730353, 
		1748251740, 1369810420, -773462732, -101584632, 
		-495881837, -1411852173, 1647391059, 706024767, 
		134480908, -1782069422, 1176707941, -1648114850, 
		806885416, 932615841, 168101135, 798661301, 
		235341577, 605164086, 461406363, -538779075, 
		-840176858, 1311188841, 2142417613, -361400929, 
		302582043, 495158174, 1479289972, 874125870, 
		907746093, -596742478, -1269146898, 1537253627, 
		-1538108682, 1983593293, -1210657183, 2108928974, 
		1378429307, -572267714, 1580150641, 327451799, 
		-1504488459, -1177431704, 0, -1041371860, 
		1075847264, -469959649, 2041688520, -1235526675, 
		-731223362, -1916023994, 1740553945, 1916352843, 
		-1807070498, -1739830060, -1336387352, -2049978550, 
		-1143943061, -974131414, 1336584933, -302253290, 
		-2042412091, -1706209833, 1714631509, 293963156, 
		-1975171633, -369493744, 67240454, -25198719, 
		-1605349136, 2017213508, 631218106, 1269344483, 
		-1571728909, 1571005438, -2143272768, 93294474, 
		1066570413, 563977660, 1882732616, -235539196, 
		1673313503, 2008463041, -1344611723, 1109467491, 
		537923632, -436207846, -34344178, -1076702611, 
		-2117218996, 403442708, 638784309, -1007883217, 
		-1101045791, 899127202, -2008791860, 773265209, 
		-1815821225, 1437050866, -58818942, 2050833735, 
		-932944724, -1168286233, 840505643, -428641387, 
		-1067425632, 427917720, -1638969391, -1545806721, 
		1143087718, 1412049534, 999329963, 193497219, 
		-1941551414, -940642775, 1807268051, 672404540, 
		-1478566279, -1134666014, 369822493, -1378100362, 
		-606019525, 1681011286, 1949973070, 336202270, 
		-1840690725, 201721354, 1210328172, -1201906460, 
		-1614626211, -1110191250, 1135389935, -1000185178, 
		965841320, 831886756, -739974089, -226920053, 
		-706222286, -1949775805, 1849112409, -630362697, 
		26054028, -1311386268, -1672589614, 1235855840, 
		-663982924, -1403627782, -202050553, -806688219, 
		-899324497, -193299826, 1202630377, 268961816, 
		1874508501, -260540280, 1243948399, 1546530418, 
		941366308, 1470539505, 1941222599, -1748580783, 
		-873928669, -1579295364, -395021156, 1042226977, 
		-1773450275, 1639824860, 227249030, 260737669, 
		-529502064, 2084453954, 1907733956, -865704278, 
		-1874310952, 100860677, -134810111, 470683154, 
		-1033805405, 1781871967, -1370007559, 1773779408, 
		394692241, -1715355304, 974986535, 664706745, 
		-639508168, -336005101, 731420851, 571543859, 
		-764843589, -1445340816, 126783113, 865375399, 
		765172662, 1008606754, 361203602, -907417312, 
		-2016489911, -1437248001, 1344809080, -1512054918, 
		59542671, 1503764984, 160008576, 437062935, 
		1707065306, -672733647, -2076032314, -798463816, 
		-2109652541, 697932208, 1512910199, 504303377, 
		2075177163, -1470868228, 1841019862, 739644986
	};
	memcpy(pOut,pIn,nSize);
	nSize=min( nSize,sizeof(sm_T1) );
	for (int i=0;i<nSize;i++)
	{
		pOut[i]=pOut[i]^sm_T1[i];
	}
}

void Garbage::Dump21(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}

	for (int i=0;i<nSize-1;i++)
	{
		pOut[i]=((pIn[i] & 0x03) << 4) | (pIn[i+1] >> 4);
	}
}

void Garbage::Dump22(unsigned char*pIn,unsigned char*pOut,int nSize)
{
	if (pIn==NULL || pOut==NULL || nSize<=0)
	{
		return;
	}

	for (int i=0;i<nSize-2;i++)
	{
		pOut[i]=((pIn[i+1] & 0x0F) << 2) | (pIn[i+2] >> 6);
	}
}
