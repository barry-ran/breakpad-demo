// BreakpadDemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <locale.h> 

#include "client/windows/handler/exception_handler.h"
#include "client/windows/sender/crash_report_sender.h"

std::wstring g_security_url = L"";

std::wstring StringToWstring(const std::string str)
{// string转wstring
	unsigned len = str.size() * 2;// 预留字节数
	setlocale(LC_CTYPE, "");     //必须调用此函数
	wchar_t *p = new wchar_t[len];// 申请一段内存存放转换后的字符串
	mbstowcs(p, str.c_str(), len);// 转换
	std::wstring str1(p);
	delete[] p;// 释放申请的内存
	return str1;
}

// 程序崩溃回调函数;
bool callback(const wchar_t *dump_path, const wchar_t *id,
	void *context, EXCEPTION_POINTERS *exinfo,
	MDRawAssertionInfo *assertion,
	bool succeeded)
{
	if (succeeded)
	{
		std::cout << "create dump file success" << std::endl;;

		std::cout << "start upload dump file:" << std::endl;;
		wprintf(id);
		std::cout << std::endl;
		std::cout << std::endl;


		google_breakpad::CrashReportSender sender(L"crash.checkpoint");

		std::wstring filename = dump_path;
		filename += L"\\";
		filename += id;
		filename += L".dmp";

		std::map<std::wstring, std::wstring> files;
		// 这里一定是upload_file_minidump
		files.insert(std::make_pair(L"upload_file_minidump", filename));

		// At this point you may include custom data to be part of the crash report.
		std::map<std::wstring, std::wstring> userCustomData;
		userCustomData.insert(std::make_pair(L"desc", L"Hello Crash"));

		// url可在sentry项目里参照Security Headers中填写
		// 1. security改为minidump
		// 2. api前面保留一个/
		google_breakpad::ReportResult re = sender.SendCrashReport(g_security_url, userCustomData, files, 0);

		std::cout << "upload dump file result " << (int)re << std::endl;
	}
	else
	{
		std::cout << "create dump file failed" << std::endl;
	}
	getchar();
	return succeeded;
}

int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		std::cout << "需要一个参数指定security url，例如:" << std::endl;
		std::cout << "http://10.224.18.144:9000/api/3/minidump/?sentry_key=3edeb8d3f27c4eb38e4220232f1c276f" << std::endl;
		getchar();
		return 1;
	}
	g_security_url = StringToWstring(argv[1]);
	// 创建捕捉程序异常对象;
	google_breakpad::ExceptionHandler eh(L".", NULL, callback, NULL,
		google_breakpad::ExceptionHandler::HANDLER_ALL);

    std::cout << "start test crash" << std::endl;;

	int *n = NULL;
	*n = 1;

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
