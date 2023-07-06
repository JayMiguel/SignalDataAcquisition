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
	//声明Dump文件，异常时会自动生成。会自动加入.dmp文件名后缀
	void DeclarDumpFile(std::string dmpFileName = "");

private:
	static std::string					m_strDumpFile;
	static shared_ptr<CCreateDump*>    m_sptrInstance;
};


