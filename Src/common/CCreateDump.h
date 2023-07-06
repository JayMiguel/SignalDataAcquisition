#pragma once
#pragma execution_character_set("utf-8")

#include <string>
#include <memory>
using namespace std;

class CCreateDump
{
public:
	CCreateDump();
	~CCreateDump(void);
	static CCreateDump* Instance();
	static long __stdcall UnhandleExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo);
	//����Dump�ļ����쳣ʱ���Զ����ɡ����Զ�����.dmp�ļ�����׺
	void DeclarDumpFile(std::string dmpFileName = "");

private:
	static std::string					m_strDumpFile;
	static shared_ptr<CCreateDump*>    m_sptrInstance;
};


