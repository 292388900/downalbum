#include "stdafx.h"

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将它更改为适合 Windows Me 或更高版本的相应值。
#endif

#include <windows.h>
#include <winerror.h>
#include <winioctl.h>
#include <tchar.h>
#include <stdio.h>
#include "diskid.h"
//
// IDENTIFY data (from ATAPI driver source)
//

#pragma pack(1)

typedef struct _IDENTIFY_DATA {
	USHORT GeneralConfiguration;            // 00 00
	USHORT NumberOfCylinders;               // 02  1
	USHORT Reserved1;                       // 04  2
	USHORT NumberOfHeads;                   // 06  3
	USHORT UnformattedBytesPerTrack;        // 08  4
	USHORT UnformattedBytesPerSector;       // 0A  5
	USHORT SectorsPerTrack;                 // 0C  6
	USHORT VendorUnique1[3];                // 0E  7-9
	USHORT SerialNumber[10];                // 14  10-19
	USHORT BufferType;                      // 28  20
	USHORT BufferSectorSize;                // 2A  21
	USHORT NumberOfEccBytes;                // 2C  22
	USHORT FirmwareRevision[4];             // 2E  23-26
	USHORT ModelNumber[20];                 // 36  27-46
	UCHAR  MaximumBlockTransfer;            // 5E  47
	UCHAR  VendorUnique2;                   // 5F
	USHORT DoubleWordIo;                    // 60  48
	USHORT Capabilities;                    // 62  49
	USHORT Reserved2;                       // 64  50
	UCHAR  VendorUnique3;                   // 66  51
	UCHAR  PioCycleTimingMode;              // 67
	UCHAR  VendorUnique4;                   // 68  52
	UCHAR  DmaCycleTimingMode;              // 69
	USHORT TranslationFieldsValid:1;        // 6A  53
	USHORT Reserved3:15;
	USHORT NumberOfCurrentCylinders;        // 6C  54
	USHORT NumberOfCurrentHeads;            // 6E  55
	USHORT CurrentSectorsPerTrack;          // 70  56
	ULONG  CurrentSectorCapacity;           // 72  57-58
	USHORT CurrentMultiSectorSetting;       //     59
	ULONG  UserAddressableSectors;          //     60-61
	USHORT SingleWordDMASupport : 8;        //     62
	USHORT SingleWordDMAActive : 8;
	USHORT MultiWordDMASupport : 8;         //     63
	USHORT MultiWordDMAActive : 8;
	USHORT AdvancedPIOModes : 8;            //     64
	USHORT Reserved4 : 8;
	USHORT MinimumMWXferCycleTime;          //     65
	USHORT RecommendedMWXferCycleTime;      //     66
	USHORT MinimumPIOCycleTime;             //     67
	USHORT MinimumPIOCycleTimeIORDY;        //     68
	USHORT Reserved5[2];                    //     69-70
	USHORT ReleaseTimeOverlapped;           //     71
	USHORT ReleaseTimeServiceCommand;       //     72
	USHORT MajorRevision;                   //     73
	USHORT MinorRevision;                   //     74
	USHORT Reserved6[50];                   //     75-126
	USHORT SpecialFunctionsEnabled;         //     127
	USHORT Reserved7[128];                  //     128-255
} IDENTIFY_DATA, *PIDENTIFY_DATA;

#pragma pack()


static char *ConvertToString (DWORD diskdata [256], int firstIndex, int lastIndex, char* buf)
{
	int index = 0;
	int position = 0;
	char ch = 0;

	//  each integer has two characters stored in it backwards
	for (index = firstIndex; index <= lastIndex; index++)
	{
		//  get high byte for 1st character
		ch = (char) (diskdata [index] / 256);
		if ( ch!=0 && ch!=0x20 ){
			buf [position++] = ch;
		}

		//  get low byte for 2nd character
		ch = (char) (diskdata [index] % 256);
		if ( ch!=0 && ch!=0x20 ){
			buf [position++] = ch;
		}
	}

	//  end the string 
	buf[position] = '\0';

	return buf;
}



int ReadPhysicalDriveInNTUsingSmart (char* diskid, int len)
{
	int nError = -1;

	//for ( int drive = 0; drive < 16; drive++ )
	{
		HANDLE hPhysicalDriveIOCTL = 0;

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		// 仅取第一块硬盘的编号即可
		TCHAR driveName[] = _T("\\\\.\\PhysicalDrive0");

		//  Windows NT, Windows 2000, Windows Server 2003, Vista
		hPhysicalDriveIOCTL = CreateFile (driveName,
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 
			NULL, OPEN_EXISTING, 0, NULL);

		if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE){
			nError = 1;
			//strncpy(diskid, "CreateFile err!", len);
		}else{
			GETVERSIONINPARAMS GetVersionParams = {0};
			DWORD cbBytesReturned = 0;

			// Get the version, etc of PhysicalDrive IOCTL

			if ( !DeviceIoControl(hPhysicalDriveIOCTL, SMART_GET_VERSION, NULL, 0, &GetVersionParams, sizeof (GETVERSIONINPARAMS), &cbBytesReturned, NULL) ){         
				nError = 2;
				//strncpy(diskid, "SMART_GET_VER err!", len);
			}else{
				// Print the SMART version
				// PrintVersion (& GetVersionParams);
				// Allocate the command buffer
				ULONG CommandSize = sizeof(SENDCMDINPARAMS) + IDENTIFY_BUFFER_SIZE;
				PSENDCMDINPARAMS Command = (PSENDCMDINPARAMS) new BYTE[CommandSize];
				// Retrieve the IDENTIFY data
				// Prepare the command
				#define ID_CMD          0xEC            // Returns ID sector for ATA
				Command -> irDriveRegs.bCommandReg = ID_CMD;
				DWORD BytesReturned = 0;
				if ( ! DeviceIoControl (hPhysicalDriveIOCTL, SMART_RCV_DRIVE_DATA, Command, sizeof(SENDCMDINPARAMS), Command, CommandSize, &BytesReturned, NULL) ){
					nError = 3;
					//strncpy(diskid, "SMART_RCV err!", len);
				}else{
					// Print the IDENTIFY data
					DWORD diskdata [256];
					USHORT *pIdSector = (USHORT *)
						(PIDENTIFY_DATA) ((PSENDCMDOUTPARAMS) Command) -> bBuffer;

					for (int ijk = 0; ijk < 256; ijk++)
						diskdata [ijk] = pIdSector [ijk];

					//10, 19,  ~  27, 46
					char serialNumber [MAX_PATH] = {0};
					char modelNumber [MAX_PATH] = {0};
					ConvertToString (diskdata, 10, 19, serialNumber);	//序列号
					//ConvertToString (diskdata, 27, 46, modelNumber);	//模型号
					lstrcpy(diskid,serialNumber);
					//lstrcat(diskid,modelNumber);

					//_snprintf(diskid, len-1, "%s - %s", serialNumber, modelNumber);

					nError = 0;
				}
				// Done
				CloseHandle (hPhysicalDriveIOCTL);
				delete []Command;
		 }
		}
	}

	return nError;
}

//
//void main()
//{
//	char diskid[MAX_PATH] = {0};
//	ReadPhysicalDriveInNTUsingSmart(diskid,MAX_PATH);
//	printf_s("%s\n",diskid);
//	system("pause");
//}