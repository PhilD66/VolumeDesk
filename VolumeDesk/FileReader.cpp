#include "stdafx.h"
#include <stdio.h>
//#include <cstdio.h>
#include "FileReader.hpp"

#define SKULL_SLICE_FILES 113


FileReader::FileReader() {

}

int FileReader::LoadSkullDemo(int * pDimX, int * pDimY, int * pDimZ, float **pVolArray)
{
    int result  = 1;
    char * pFileNameTemplate = "C:\\Users\\Phil\\source\\repos\\Sample Data\\Skull\\CThead.%d";
	const int MaxFilePath = 256;
    char filePath[MaxFilePath];
    FILE * pSliceFile;
    int		readSize	= 256;
    int		pos			= 0;
    unsigned short	*pLineData	= new unsigned short [256];
    unsigned char	*pUpper;
    unsigned char	*pLower;

    *pDimX	= 256;
    *pDimY	= 256;
    *pDimZ	= SKULL_SLICE_FILES;

    *pVolArray   = NULL;
    *pVolArray	= new float [*pDimX * *pDimY * *pDimZ];
    float	*pData	= *pVolArray;

    for ( int slice = 1; (slice <= SKULL_SLICE_FILES) && (result == 1); slice++ )
    {
        sprintf_s(filePath, MaxFilePath, pFileNameTemplate, slice);
        if ( fopen_s(&pSliceFile, filePath, "r") != EINVAL )
        {
            for ( int line = 0; line < 256; line++ )
            {
                fread( pLineData, sizeof(unsigned short), readSize, pSliceFile );

                //	Now convert the short data into doubles.
                for ( int iShort = 0; iShort < 256; iShort++ )
                {
                    pUpper	= (unsigned char *)&pLineData[iShort];
                    pLower	= pUpper	+ 1;
                    pData[pos++]	= (float)(((unsigned short)*pUpper << 8) + (unsigned short)*pLower);
                }
            }

            fclose(pSliceFile);

        }
        else {
            result = 0;
        }
    }

    //  Delete the allocated memory if there's a problem.
    if ( (result == 0) && (pVolArray != NULL) ) {
        delete [] pVolArray;
    }

    return result;
}

