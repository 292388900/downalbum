#ifndef _CkCgi_H
#define _CkCgi_H
#include "Chilkat_C.h"

HCkCgi CkCgi_Create(void);
void CkCgi_Dispose(HCkCgi handle);
unsigned long CkCgi_getAsyncBytesRead(HCkCgi cHandle);
BOOL CkCgi_getAsyncInProgress(HCkCgi cHandle);
unsigned long CkCgi_getAsyncPostSize(HCkCgi cHandle);
BOOL CkCgi_getAsyncSuccess(HCkCgi cHandle);
void CkCgi_getDebugLogFilename(HCkCgi cHandle, HCkString retval);
void CkCgi_putDebugLogFilename(HCkCgi cHandle, const char *newVal);
int CkCgi_getHeartbeatMs(HCkCgi cHandle);
void CkCgi_putHeartbeatMs(HCkCgi cHandle, int newVal);
int CkCgi_getIdleTimeoutMs(HCkCgi cHandle);
void CkCgi_putIdleTimeoutMs(HCkCgi cHandle, int newVal);
void CkCgi_getLastErrorHtml(HCkCgi cHandle, HCkString retval);
void CkCgi_getLastErrorText(HCkCgi cHandle, HCkString retval);
void CkCgi_getLastErrorXml(HCkCgi cHandle, HCkString retval);
int CkCgi_getNumParams(HCkCgi cHandle);
int CkCgi_getNumUploadFiles(HCkCgi cHandle);
int CkCgi_getReadChunkSize(HCkCgi cHandle);
void CkCgi_putReadChunkSize(HCkCgi cHandle, int newVal);
unsigned long CkCgi_getSizeLimitKB(HCkCgi cHandle);
void CkCgi_putSizeLimitKB(HCkCgi cHandle, unsigned long newVal);
BOOL CkCgi_getStreamToUploadDir(HCkCgi cHandle);
void CkCgi_putStreamToUploadDir(HCkCgi cHandle, BOOL newVal);
void CkCgi_getUploadDir(HCkCgi cHandle, HCkString retval);
void CkCgi_putUploadDir(HCkCgi cHandle, const char *newVal);
BOOL CkCgi_getUtf8(HCkCgi cHandle);
void CkCgi_putUtf8(HCkCgi cHandle, BOOL newVal);
void CkCgi_getVersion(HCkCgi cHandle, HCkString retval);
void CkCgi_AbortAsync(HCkCgi cHandle);
BOOL CkCgi_GetEnv(HCkCgi cHandle, const char *varName, HCkString str);
BOOL CkCgi_GetParam(HCkCgi cHandle, const char *paramName, HCkString str);
BOOL CkCgi_GetParamName(HCkCgi cHandle, int idx, HCkString str);
BOOL CkCgi_GetParamValue(HCkCgi cHandle, int idx, HCkString str);
void CkCgi_GetRawPostData(HCkCgi cHandle, HCkByteData byteData);
BOOL CkCgi_GetUploadData(HCkCgi cHandle, int idx, HCkByteData byteData);
BOOL CkCgi_GetUploadFilename(HCkCgi cHandle, int idx, HCkString str);
unsigned long CkCgi_GetUploadSize(HCkCgi cHandle, int idx);
BOOL CkCgi_IsGet(HCkCgi cHandle);
BOOL CkCgi_IsHead(HCkCgi cHandle);
BOOL CkCgi_IsPost(HCkCgi cHandle);
BOOL CkCgi_IsUpload(HCkCgi cHandle);
BOOL CkCgi_ReadRequest(HCkCgi cHandle);
BOOL CkCgi_SaveLastError(HCkCgi cHandle, const char *filename);
void CkCgi_SleepMs(HCkCgi cHandle, int millisec);
const char *CkCgi_getEnv(HCkCgi cHandle, const char *varName);
const char *CkCgi_getParam(HCkCgi cHandle, const char *paramName);
const char *CkCgi_getParamName(HCkCgi cHandle, int idx);
const char *CkCgi_getParamValue(HCkCgi cHandle, int idx);
const char *CkCgi_getUploadFilename(HCkCgi cHandle, int idx);
const char *CkCgi_lastErrorHtml(HCkCgi cHandle);
const char *CkCgi_lastErrorText(HCkCgi cHandle);
const char *CkCgi_lastErrorXml(HCkCgi cHandle);
const char *CkCgi_version(HCkCgi cHandle);
#endif
