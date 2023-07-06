#include "CCreateDump.h"

#include <QString>
#include <Windows.h>
#include "CCreateDump.h"
#include <DbgHelp.h>
#pragma comment(lib,  "dbghelp.lib")

std::shared_ptr<CCreateDump*>  CCreateDump::m_sptrInstance = make_shared<CCreateDump*>();
std::string CCreateDump::m_strDumpFile = "";

CCreateDump::CCreateDump()
{
}

CCreateDump::~CCreateDump(void)
{

}

long CCreateDump::UnhandleExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo)
{
	// ��const char*תΪQString����
	QString qsDumpFile = QString::fromUtf8(m_strDumpFile.c_str());

	// ��QString����תΪWCHAR*���͵Ŀ��ַ���
	QByteArray baDumpFile = qsDumpFile.toUtf8();
	const char* cstrDumpFile = baDumpFile.constData();
	WCHAR wstrDumpFile[MAX_PATH];
	MultiByteToWideChar(CP_UTF8, 0, cstrDumpFile, -1, wstrDumpFile, MAX_PATH);
	LPCWSTR lpwstrDumpFile = reinterpret_cast<LPCWSTR>(wstrDumpFile);

	HANDLE hFile = CreateFile(lpwstrDumpFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION   ExInfo;
		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = ExceptionInfo;
		ExInfo.ClientPointers = FALSE;
		//   write   the   dump
		BOOL   bOK = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL);
		CloseHandle(hFile);
		if (!bOK)
		{
			DWORD dw = GetLastError();
			//дdump�ļ�������,�쳣����windows����
			return EXCEPTION_CONTINUE_SEARCH;
		}
		else
		{    //���쳣������
			return EXCEPTION_EXECUTE_HANDLER;
		}
	}
	else
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}
}

void CCreateDump::DeclarDumpFile(std::string dmpFileName)
{
	SYSTEMTIME syt;
	GetLocalTime(&syt);
	char szTime[MAX_PATH];
	sprintf_s(szTime, MAX_PATH, "%04d-%02d-%02dT%02d-%02d-%02d", syt.wYear, syt.wMonth, syt.wDay, syt.wHour, syt.wMinute, syt.wSecond);
	m_strDumpFile = dmpFileName + "_" + std::string(szTime);
	m_strDumpFile += std::string(".dmp");
	SetUnhandledExceptionFilter(UnhandleExceptionFilter);
}

CCreateDump* CCreateDump::Instance()
{
	if (*m_sptrInstance == NULL)
	{
		m_sptrInstance = make_shared<CCreateDump*>(new CCreateDump);
	}
	return *m_sptrInstance;
}