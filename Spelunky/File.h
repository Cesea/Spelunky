#ifndef FILE_H
#define FILE_H

namespace FileUtils
{
	namespace FileAccess
	{
		enum Type
		{
			Read,  // Read only
			Write, // Clear file contents
			Append // Write to end of file
		};
	};

	class File
	{
	public:
		File();
		File(FILE *file);
		~File();

		void Open(const char *fileName, FileAccess::Type accessType);
		void Close(void); // Close file pointer
		void Clear(void); // Close file pointer (if open) and clear file name
		void Write(const WCHAR *format, ...);
		void Read(const WCHAR *format, ...);
		std::wstring GetLine(void);        // Retrieve one line delimited by '\n'
		std::wstring GetLine(WCHAR delim);  // Retrieve one line delimited by delim
		std::wstring GetLine(const WCHAR *delims);  // Retrieve one line delimited by chars within delims

		void SeekFromBeginOfFile(int offset);
		void SeekFromEndOfFile(int offset);
		void SeekByOffset(int offset);

		bool Validate(void) const;

	private:
		FILE *fp; // Pointer to file
	};

	extern File STDOUT;
}
#endif
