/*
==============================================================

	Half-Life MDL Decompiler
	2002, Kratisto. Based on code from HL SDK

==============================================================
*/

#pragma once

#ifndef _MDLDECOMPILER_H
#define _MDLDECOMPILER_H

//
#define STUDIO_VERSION	10
#define IDSTUDIOHEADER	(('T'<<24)+('S'<<16)+('D'<<8)+'I') // little-endian "IDST"
#define IDSTUDIOSEQHEADER	(('Q'<<24)+('S'<<16)+('D'<<8)+'I') // little-endian "IDSQ"
typedef unsigned char byte;
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#ifdef MDLDEC_MS3DPLUGIN

#include "../plugin/stdafx.h"
#include "../plugin/DlgLog.h"
#include "../plugin/msPlugInImpl.h"

#else

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <direct.h>
#else
#include <limits.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef _MAX_PATH
#define _MAX_PATH PATH_MAX
#endif

typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;
typedef uint32_t ULONG;

typedef struct __attribute__((packed))
{
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER;

typedef struct __attribute__((packed))
{
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;

typedef struct __attribute__((packed))
{
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD;

#ifndef BI_RGB
#define BI_RGB 0L
#endif

#ifndef MAKEWORD
#define MAKEWORD(a, b) ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#endif

#define _mkdir(path) mkdir(path, 0755)

#endif

#endif

#ifndef PATH_SEPARATOR
#ifdef _WIN32
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif
#endif


#include "./hlsdk/mathlib.h"
#include "./hlsdk/studio.h"
#include "./hlsdk/activity.h"


#define MDLDEC_VERSION				'1.01'
#define MDLDEC_MSG_INFO					0	
#define MDLDEC_MSG_WARNING				1
#define MDLDEC_MSG_ERROR				2


#define PATHSEPARATOR(c) ((c) == '\\' || (c) == '/')
void MyExtractFileBase (char *path, char *dest);
void MyExtractFilePath (char *path, char *dest);

class CMDLDecompiler
{

public:
							CMDLDecompiler();
	virtual					~CMDLDecompiler();
	bool					LoadModel( char *modelname );
	char					DestPath[_MAX_PATH];
	void					DumpInfo();
	void					QC_GenerateScript();
	void					SMD_GenerateReferences();
	void					SMD_GenerateSequences();
	void					BMP_GenerateTextures();
	void					FixRepeatedSequenceNames();	
	void					LogMessage ( int type, const char *msg, ...);

#ifdef MDLDEC_MS3DPLUGIN
	DlgLog					*pLogDialog; //hack
#endif


private:

	bool					ModelLoaded;
	char					ModelName[64];
	char					ModelPath[_MAX_PATH];

	FILE					*qcfile;

	byte					*CoreFile;
	int						CoreFileLength;
	studiohdr_t				*m_pstudiohdr;

	byte					*SequenceFiles[32];
	int						SequenceFilesLength[32];
	studioseqhdr_t			*m_panimhdr[32];

	byte					*TextureFile;
	int						TextureFileLength;
	studiohdr_t				*m_ptexturehdr;

	void					QC_WriteBodyModels();
	void					QC_WriteTextureGroups();
	void					QC_WriteAttachments();
	void					QC_WriteBoneControllers();
	void					QC_WriteHitBoxes();
	void					QC_WriteSequences();
	int						QC_TranslateActivityFlag(const int aflag, char *string);
	void					QC_TranslateMotionFlag(const int mflag, char *string, bool addtostring);

	void					SMD_WriteNodes(FILE *smdfile);
	void					SMD_WriteTriangle(FILE *smd, mstudiomodel_t *pmodel, mstudiotexture_t *ptexture, short *pA, short *pB, short *pC);
	void					SMD_WriteFrame(FILE *smd, int frame, mstudioanim_t *panim, mstudioseqdesc_t *pseqdesc);

	void					BMP_WriteTexture(byte *pBitData, byte *pColorData, char *TextureName, mstudiotexture_t *pTexture);
	float					g_bonetransform[MAXSTUDIOBONES][3][4];	// bone transformation matrix
	float					g_normtransform[MAXSTUDIOBONES][3][4];
};


#endif
