/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "cmock.h"
#include "mock_common.h"

static const char* CMockString_buf = "buf";
static const char* CMockString_format = "format";
static const char* CMockString_fpath = "fpath";
static const char* CMockString_log_data = "log_data";
static const char* CMockString_path = "path";

typedef struct _CMOCK_fpath_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  char ExpectAnyArgsBool;
  const char* Expected_path;
  char* Expected_buf;
  char ReturnThruPtr_buf_Used;
  char* ReturnThruPtr_buf_Val;
  size_t ReturnThruPtr_buf_Size;

} CMOCK_fpath_CALL_INSTANCE;

typedef struct _CMOCK_log_data_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  char ExpectAnyArgsBool;
  const char* Expected_format;

} CMOCK_log_data_CALL_INSTANCE;

static struct mock_commonInstance
{
  char fpath_IgnoreBool;
  CMOCK_MEM_INDEX_TYPE fpath_CallInstance;
  char log_data_IgnoreBool;
  CMOCK_MEM_INDEX_TYPE log_data_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void mock_common_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_MEM_INDEX_TYPE call_instance;
  call_instance = Mock.fpath_CallInstance;
  if (Mock.fpath_IgnoreBool)
    call_instance = CMOCK_GUTS_NONE;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_fpath);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  call_instance = Mock.log_data_CallInstance;
  if (Mock.log_data_IgnoreBool)
    call_instance = CMOCK_GUTS_NONE;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_log_data);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
}

void mock_common_Init(void)
{
  mock_common_Destroy();
}

void mock_common_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

void fpath(const char* path, char* buf)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_fpath_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_fpath);
  cmock_call_instance = (CMOCK_fpath_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.fpath_CallInstance);
  Mock.fpath_CallInstance = CMock_Guts_MemNext(Mock.fpath_CallInstance);
  if (Mock.fpath_IgnoreBool)
  {
    UNITY_CLR_DETAILS();
    return;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (!cmock_call_instance->ExpectAnyArgsBool)
  {
  {
    UNITY_SET_DETAILS(CMockString_fpath,CMockString_path);
    UNITY_TEST_ASSERT_EQUAL_STRING(cmock_call_instance->Expected_path, path, cmock_line, CMockStringMismatch);
  }
  {
    UNITY_SET_DETAILS(CMockString_fpath,CMockString_buf);
    UNITY_TEST_ASSERT_EQUAL_STRING(cmock_call_instance->Expected_buf, buf, cmock_line, CMockStringMismatch);
  }
  }
  if (cmock_call_instance->ReturnThruPtr_buf_Used)
  {
    UNITY_TEST_ASSERT_NOT_NULL(buf, cmock_line, CMockStringPtrIsNULL);
    memcpy((void*)buf, (void*)cmock_call_instance->ReturnThruPtr_buf_Val,
      cmock_call_instance->ReturnThruPtr_buf_Size);
  }
  UNITY_CLR_DETAILS();
}

void CMockExpectParameters_fpath(CMOCK_fpath_CALL_INSTANCE* cmock_call_instance, const char* path, char* buf);
void CMockExpectParameters_fpath(CMOCK_fpath_CALL_INSTANCE* cmock_call_instance, const char* path, char* buf)
{
  cmock_call_instance->Expected_path = path;
  cmock_call_instance->Expected_buf = buf;
  cmock_call_instance->ReturnThruPtr_buf_Used = 0;
}

void fpath_CMockIgnore(void)
{
  Mock.fpath_IgnoreBool = (char)1;
}

void fpath_CMockStopIgnore(void)
{
  Mock.fpath_IgnoreBool = (char)0;
}

void fpath_CMockExpectAnyArgs(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_fpath_CALL_INSTANCE));
  CMOCK_fpath_CALL_INSTANCE* cmock_call_instance = (CMOCK_fpath_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.fpath_CallInstance = CMock_Guts_MemChain(Mock.fpath_CallInstance, cmock_guts_index);
  Mock.fpath_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  cmock_call_instance->ExpectAnyArgsBool = (char)1;
}

void fpath_CMockExpect(UNITY_LINE_TYPE cmock_line, const char* path, char* buf)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_fpath_CALL_INSTANCE));
  CMOCK_fpath_CALL_INSTANCE* cmock_call_instance = (CMOCK_fpath_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.fpath_CallInstance = CMock_Guts_MemChain(Mock.fpath_CallInstance, cmock_guts_index);
  Mock.fpath_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  CMockExpectParameters_fpath(cmock_call_instance, path, buf);
}

void fpath_CMockReturnMemThruPtr_buf(UNITY_LINE_TYPE cmock_line, char* buf, size_t cmock_size)
{
  CMOCK_fpath_CALL_INSTANCE* cmock_call_instance = (CMOCK_fpath_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.fpath_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringPtrPreExp);
  cmock_call_instance->ReturnThruPtr_buf_Used = 1;
  cmock_call_instance->ReturnThruPtr_buf_Val = buf;
  cmock_call_instance->ReturnThruPtr_buf_Size = cmock_size;
}

void log_data(const char* format, ...)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_log_data_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_log_data);
  cmock_call_instance = (CMOCK_log_data_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.log_data_CallInstance);
  Mock.log_data_CallInstance = CMock_Guts_MemNext(Mock.log_data_CallInstance);
  if (Mock.log_data_IgnoreBool)
  {
    UNITY_CLR_DETAILS();
    return;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (!cmock_call_instance->ExpectAnyArgsBool)
  {
  {
    UNITY_SET_DETAILS(CMockString_log_data,CMockString_format);
    UNITY_TEST_ASSERT_EQUAL_STRING(cmock_call_instance->Expected_format, format, cmock_line, CMockStringMismatch);
  }
  }
  UNITY_CLR_DETAILS();
}

void CMockExpectParameters_log_data(CMOCK_log_data_CALL_INSTANCE* cmock_call_instance, const char* format);
void CMockExpectParameters_log_data(CMOCK_log_data_CALL_INSTANCE* cmock_call_instance, const char* format)
{
  cmock_call_instance->Expected_format = format;
}

void log_data_CMockIgnore(void)
{
  Mock.log_data_IgnoreBool = (char)1;
}

void log_data_CMockStopIgnore(void)
{
  Mock.log_data_IgnoreBool = (char)0;
}

void log_data_CMockExpectAnyArgs(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_log_data_CALL_INSTANCE));
  CMOCK_log_data_CALL_INSTANCE* cmock_call_instance = (CMOCK_log_data_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.log_data_CallInstance = CMock_Guts_MemChain(Mock.log_data_CallInstance, cmock_guts_index);
  Mock.log_data_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  cmock_call_instance->ExpectAnyArgsBool = (char)1;
}

void log_data_CMockExpect(UNITY_LINE_TYPE cmock_line, const char* format)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_log_data_CALL_INSTANCE));
  CMOCK_log_data_CALL_INSTANCE* cmock_call_instance = (CMOCK_log_data_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.log_data_CallInstance = CMock_Guts_MemChain(Mock.log_data_CallInstance, cmock_guts_index);
  Mock.log_data_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  CMockExpectParameters_log_data(cmock_call_instance, format);
}
