#include "stdafx.h"
#include "ZipFile.h"
#if 0
#include "zlib/zlib.h"

#pragma comment(lib,"zlib/zlib.lib")  
#else
#include "zip.h"
#endif

CZipFile::CZipFile()
{
}


CZipFile::~CZipFile()
{
}

int CZipFile::Zip(const CString &srcFilePath, const CString &outFilePath)
{
#if 0
	HANDLE hFile, hFileToWrite;

	// ��Ҫ����ѹ�����ļ�  
	hFile = CreateFile(srcFilePath, // file name  
		GENERIC_READ, // open for reading  
		FILE_SHARE_READ, // share for reading  
		NULL, // no security  
		OPEN_EXISTING, // existing file only  
		FILE_ATTRIBUTE_NORMAL, // normal file  
		NULL
		); // no attr. template  
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox("Could not open file to read"); // process error  
		return -1;
	}

	HANDLE hMapFile, hMapFileToWrite;
	// ����һ���ļ�ӳ��  
	hMapFile = CreateFileMapping(hFile, // Current file handle.  
		NULL, // Default security.  
		PAGE_READONLY, // Read/write permission.  
		0, // Max. object size.  
		0, // Size of hFile.  
		_T("ZipTestMappingObjectForRead")
		); // Name of mapping object.  
	if (hMapFile == NULL)
	{
		//AfxMessageBox("Could not create file mapping object");
		CloseHandle(hFile);
		return -2;
	}


	LPVOID lpMapAddress, lpMapAddressToWrite;
	// ����һ���ļ�ӳ�����ͼ������Ϊsource  
	lpMapAddress = MapViewOfFile(hMapFile, // Handle to mapping object.  
		FILE_MAP_READ, // Read/write permission  
		0, // Max. object size.  
		0, // Size of hFile.  
		0); // Map entire file.  
	if (lpMapAddress == NULL)
	{
		//AfxMessageBox("Could not map view of file");
		CloseHandle(hFile);
		CloseHandle(hMapFile);
		return -3;
	}


	DWORD dwFileLength, dwFileLengthToWrite;
	DWORD m_dwSourceFileLength;
	dwFileLength = GetFileSize(hFile, NULL);    //��ȡ�ļ��Ĵ�С  

	m_dwSourceFileLength = dwFileLength;

	// ��Ϊѹ������������������������0.1% + 12 Ȼ��һ��DWORD ��������ѹ��ǰ�Ĵ�С��  
	// ��ѹ����ʱ���ã���Ȼ�����Ա���������Ϣ�������ò���  
	dwFileLengthToWrite = (DWORD)( (double)dwFileLength*1.001 + 12 + sizeof(DWORD) );


	//�����Ǵ���һ���ļ�����������ѹ������ļ�  
	hFileToWrite = CreateFile(outFilePath, // demoFile.rar  
		GENERIC_WRITE | GENERIC_READ, // open for writing  
		0, // do not share  
		NULL, // no security  
		CREATE_ALWAYS, // overwrite existing  
		FILE_ATTRIBUTE_NORMAL, // normal file  
		NULL); // no attr. template  
	if (hFileToWrite == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox("Could not open file to write"); // process error  
		CloseHandle(hFile);
		CloseHandle(hMapFile);
		UnmapViewOfFile(lpMapAddress);
		return -4;
	}

	//Ϊ����ļ�  ����һ���ļ�ӳ��  
	hMapFileToWrite = CreateFileMapping(hFileToWrite, // Current file handle.  
		NULL, // Default security.  
		PAGE_READWRITE, // Read/write permission.  
		0, // Max. object size.  
		dwFileLengthToWrite, // Size of hFile.  
		_T("ZipTestMappingObjectForWrite")); // Name of mapping object.  
	if (hMapFileToWrite == NULL)
	{
		//AfxMessageBox("Could not create file mapping object for write");
		CloseHandle(hFile);
		CloseHandle(hMapFile);
		UnmapViewOfFile(lpMapAddress);
		CloseHandle(hFileToWrite);
		return -5;
	}

	//Ϊ����ļ� ����һ���ļ�ӳ����ͼ  
	lpMapAddressToWrite = MapViewOfFile(hMapFileToWrite, //Handle to mapping  
		FILE_MAP_WRITE, // Read/write permission 0, // Max. object size.  
		0, // Size of hFile.  
		0,
		0
		); // Map entire file.  
	if (lpMapAddressToWrite == NULL)
	{
		//AfxMessageBox("Could not map view of file");
		CloseHandle(hFile);
		CloseHandle(hMapFile);
		UnmapViewOfFile(lpMapAddress);
		CloseHandle(hFileToWrite);
		CloseHandle(hMapFileToWrite);
		return -6;
	}

	//�����ǽ�ѹ��ǰ�Ĵ�С�������ļ��ĵ�һ��DWORD ����  
	LPVOID pBuf = lpMapAddressToWrite;
	(*(DWORD*)pBuf) = dwFileLength;
	pBuf = (DWORD*)pBuf + 1;

	//�����������Ҫ�ģ�zlib �����ṩ��һ����������Դ���������ѹ����Ŀ�Ļ���  
	//ԭ�����£�  
	//int compress (Bytef *dest, uLongf *destLen, const Bytef*source, uLong sourceLen);  
	//����destLen ����ʵ��ѹ������ļ���С��  
	compress((Bytef*)pBuf, &dwFileLengthToWrite, (Bytef*)lpMapAddress, dwFileLength);

	UnmapViewOfFile(lpMapAddress);
	CloseHandle(hMapFile);
	CloseHandle(hFile);
	UnmapViewOfFile(lpMapAddressToWrite);
	CloseHandle(hMapFileToWrite);

	//���ｫ�ļ���С��������һ��  
	SetFilePointer(hFileToWrite, dwFileLengthToWrite + sizeof(DWORD), NULL, FILE_BEGIN);
	SetEndOfFile(hFileToWrite);
	CloseHandle(hFileToWrite);
#else

	HZIP hz = CreateZip(outFilePath, 0);
	ZRESULT zr = ZipAdd(hz, _T("1.txt"), srcFilePath);
	CloseZip(hz);
	if (zr != 0)
		return zr;
#endif
	return 0;
}

int CZipFile::UnZip(const CString &srcFilePath, const CString &outFilePath)
{
	return 0;
}