
#pragma once
#include <afx.h>

/*调用示例

char szBuff[]="123456789";
char szOut1[20]={0};
char szOut2[20]={0};
//------------------------------------------------------------------------
Encrypt::Garbage::Encode((BYTE*)szBuff,(BYTE*)szOut1,sizeof(szBuff));
Encrypt::Garbage::Encode((BYTE*)szBuff,(BYTE*)szOut2,sizeof(szBuff));
//------------------------------------------------------------------------
*/

namespace Star
{
	namespace Encrypt
	{
		namespace Garbage
		{
			void Dump1(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump2(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump3(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump4(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump5(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump6(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump7(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump8(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump9(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump10(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump11(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump12(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump13(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump14(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump15(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump16(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump17(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump18(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump19(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump20(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump21(unsigned char*pIn,unsigned char*pOut,int nSize);
			void Dump22(unsigned char*pIn,unsigned char*pOut,int nSize);
			/****************/
			/*随机组合Dump%d*/
			/****************/
			void Encode(unsigned char*pIn,unsigned char*pOut,int nSize);

		}
	}
}