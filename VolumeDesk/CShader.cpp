#include "stdafx.h"
#include <stdio.h>
#include "CShader.hpp"

CShader::CShader() {
    pShaderCode = NULL;
}

CShader::~CShader() {
    if ( pShaderCode != NULL ) {
        delete [] pShaderCode;
    }
}

int CShader::GetFileSize(const char *pName)
{
	const size_t cSize = strlen(pName) + 1;
	wchar_t* wc = new wchar_t[cSize];
	size_t nConverted;
	mbstowcs_s(&nConverted, wc, cSize, pName, cSize);

	WIN32_FILE_ATTRIBUTE_DATA fad;
	if (!GetFileAttributesEx(wc, GetFileExInfoStandard, &fad))
	{
		delete[] wc;
		return -1; // error condition, could call GetLastError to find out more
	}
	LARGE_INTEGER size;
	size.HighPart = fad.nFileSizeHigh;
	size.LowPart = fad.nFileSizeLow;
	delete[] wc;
	return (int)size.QuadPart;
}

int CShader::LoadFile(char *pFilePath)
{
    FILE *pFile;
	size = 0;

    if ( (fopen_s(&pFile, pFilePath, "r") != EINVAL) && (pFile != NULL) ) {

		fseek(pFile, 0L, SEEK_END);
        size = ftell(pFile);

        pShaderCode = new char [size + 1];
        fseek(pFile, 0L, SEEK_SET);

        if ( fread( pShaderCode, sizeof(char), size, pFile ) ) {

        }

		// Remove garbage from the end of the file.
		while ( ((UINT)pShaderCode[size] > 127) && (size > 0) )
			pShaderCode[size--] = 0;

        fclose(pFile);
        return size;
    } else {
        return 0;
    }
}

char * CShader::getShaderCode() {
    return pShaderCode;
}

int CShader::getFileSize() {
	return size;
}

