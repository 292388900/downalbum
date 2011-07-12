/*
   minizip.c
   Version 1.01e, February 12th, 2005

   Copyright (C) 1998-2005 Gilles Vollant

   minizip 간략화버전
   작성자 : fehead.tistory.com
   사용법 : minizip.exe 생성할압축파일명 압축할파일1 압축할파일2 ... 압축할파일N
   예제   : minizip.exe test.zip test.txt text2.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <direct.h>
#include <io.h>

#include "minizip/zip.h"
#include "minizip/iowin32.h"

uLong filetime(
    const char *f,                /* name of file to get info on */
    tm_zip *tmzip,             /* return value: access, modific. and creation times */
    uLong *dt              /* dostime */
    )
{
  int ret = 0;
  {
      FILETIME ftLocal;
      HANDLE hFind;
      WIN32_FIND_DATA  ff32;

      hFind = FindFirstFile(f,&ff32);
      if (hFind != INVALID_HANDLE_VALUE)
      {
        FileTimeToLocalFileTime(&(ff32.ftLastWriteTime),&ftLocal);
        FileTimeToDosDateTime(&ftLocal,((LPWORD)dt)+1,((LPWORD)dt)+0);
        FindClose(hFind);
        ret = 1;
      }
  }
  return ret;
}

int main(int argc, char * argv[] )
{
    char    filename_try[128] = {'\0', };
    strncpy( filename_try, argv[1], sizeof(filename_try) );
    filename_try[sizeof(filename_try)-1] = 0;

    zlib_filefunc_def ffunc;
    fill_win32_filefunc(&ffunc);
    zipFile zf = zipOpen2(filename_try, 0, NULL, &ffunc);

    if (zf == NULL)
    {
        printf("error opening %s\n",filename_try);
        return 1;
    }
    printf("creating %s\n",filename_try);

    char    buf[4086];
    int err = ZIP_OK;
    for (int i = 2 ;(i<argc) && (err==ZIP_OK);i++)
    {
        int size_read;
        const char* filenameinzip = argv[i];
        zip_fileinfo zi = { 0, };

        filetime(filenameinzip, &zi.tmz_date, &zi.dosDate);

        err = zipOpenNewFileInZip3(zf, filenameinzip,&zi,
                         NULL,0,NULL,0,NULL /* comment*/,
                         Z_DEFLATED,
                         Z_DEFAULT_COMPRESSION, 0,
                         /* -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, */
                         -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                         NULL, 0);

        if (err != ZIP_OK)
        {
            printf("error in opening %s in zipfile\n",filenameinzip);
            return 1;
        }

        FILE * fin = fopen(filenameinzip,"rb");
        if (fin==NULL)
        {
            err=ZIP_ERRNO;
            printf("error in opening %s for reading\n",filenameinzip);
            return 1;
        }

        do
        {
            err = ZIP_OK;
            size_read = (int)fread(buf,1,sizeof(buf),fin);
            if (size_read < sizeof(buf))
                if (feof(fin)==0)
            {
                printf("error in reading %s\n",filenameinzip);
                err = ZIP_ERRNO;
            }

            if (size_read>0)
            {
                err = zipWriteInFileInZip(zf,buf,size_read);
                if (err<0)
                {
                    printf("error in writing %s in the zipfile\n",
                                     filenameinzip);
                }

            }
        } while ((err == ZIP_OK) && (size_read>0));

        fclose(fin);

        zipCloseFileInZip(zf);
    }
    
    zipClose(zf,NULL);

    return 0;
}
