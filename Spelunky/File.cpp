#include "stdafx.h"
#include "File.h"

namespace FileUtils
{

	static const unsigned BUF_LEN = 4096;

	File STDOUT(stdout);

	// Super hacky thing found at: http://mrpt.googlecode.com/svn-history/r2406/trunk/libs/base/src/system/os.cpp
	// The original code from this link actually had some annoying bugs I had to fix myself.
	//int vfscanf(FILE *file, const char *format, va_list args)
	//{
	//	int count = 0;
	//	const char *p = format;

	//	for (;;)
	//	{
	//		char c = *(p++);
	//		if (c == 0)
	//			break;

	//		// Count number of specifiers to pass to fscanf
	//		if (c == '%' && *p != '%')
	//			++count;
	//	}

	//	if (count <= 0)
	//		return 0;

	//	int retVal;

	//	// copy stack pointer
	//	_asm
	//	{
	//		mov esi, esp;
	//	}

	//	const int paramSize = sizeof(void *);

	//	// push variable parameters pointers on stack
	//	for (int i = ((int)count) - 1; i >= 0; --i)
	//	{
	//		Assert(paramSize == 4); // The 4 a few lines down must be
	//										   // modified to match the size of param_size
	//		_asm
	//		{
	//			mov eax, dword ptr[i];
	//			mov ecx, dword ptr[args];
	//			mov edx, dword ptr[ecx + eax * 4]; // 4 as in the size of void *
	//			push edx;
	//		}
	//	}

	//	int stackAdvance = (int)((2 + count) * paramSize);

	//	_asm
	//	{
	//		// now push on the fixed params
	//		mov eax, dword ptr[format];
	//		push eax;
	//		mov eax, dword ptr[file];
	//		push eax;

	//		// call fscanf, and move the result into ret_val
	//		call dword ptr[fscanf];
	//		mov retVal, eax;

	//		// restore stack pointer
	//		mov eax, dword ptr[stackAdvance];
	//		add esp, eax;
	//		//mov esp, esi;
	//	}

	//	return retVal;
	//}

	File::File()
		: fp(NULL)
	{
	}

	File::File(FILE *file)
		: fp(file)
	{
	}

	File::~File()
	{
		Clear();
	}

	bool File::Open(const std::wstring &fileName, FileAccess::Type access_type)
	{
		char buffer[80]{};
		wcstombs(buffer, fileName.c_str(), 79);

		bool result = true;
		switch (access_type)
		{
		case FileAccess::Read:
			fp = fopen(buffer, "r");
			break;
		case FileAccess::Write:
			fp = fopen(buffer, "w");
			break;
		case FileAccess::Append:
			fp = fopen(buffer, "a");
			break;
		}
		if (fp == nullptr)
		{
			result = false;
		}
		else
		{
			result = true;
		}
		return result;
		//Assert(fp);
	}

	void File::Close(void)
	{
		if (fp)
			fclose(fp);
	}

	void File::Clear(void)
	{
		Close();
	}

	void File::Write(const WCHAR *format, ...)
	{
		va_list args;
		va_start(args, format);
		vfwprintf(fp, format, args);
		va_end(args);
	}

	void File::Read(const WCHAR *format, ...)
	{
		int rc;
		va_list args;
		va_start(args, format);
		rc = vfwscanf(fp, format, args);
		va_end(args);

	}

	std::wstring File::GetLine(void)
	{
		std::wstring temp;

		WCHAR c;

		for (;;)
		{
			c = static_cast<WCHAR>(fgetwc(fp));
			if (feof(fp))
				break;
			else if (c == L'\n')
				break;
			temp += c;
		}

		return std::move(temp);
	}

	std::wstring File::GetLine(WCHAR delim)
	{
		std::wstring temp;

		WCHAR c;

		for (;;)
		{
			c = static_cast<WCHAR>(fgetwc(fp));
			if (feof(fp))
				break;
			else if (c == delim)
				break;
			temp += c;
		}

		return std::move(temp);
	}

	std::wstring File::GetLine(const WCHAR *delims)
	{
		std::wstring temp;

		WCHAR c;

		for (;;)
		{
			c = static_cast<WCHAR>(fgetwc(fp));
			if (feof(fp))
				break;
			for (const WCHAR *temp_delim = delims; *temp_delim; ++temp_delim)
				if (c == *temp_delim)
					return std::move(temp);
			temp += c;
		}

		return std::move(temp);
	}

	void File::SeekFromBeginOfFile(int offset)
	{
		fseek(fp, offset, SEEK_SET);
	}

	void File::SeekFromEndOfFile(int offset)
	{
		fseek(fp, offset, SEEK_END);
	}

	void File::SeekByOffset(int offset)
	{
		fseek(fp, offset, SEEK_CUR);
	}

	bool File::Validate(void) const
	{
		return fp && !feof(fp) ? true : false;
	}
}