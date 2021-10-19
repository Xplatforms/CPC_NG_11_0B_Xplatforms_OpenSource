// CPC_NG_11_0B_Xplatforms_OpenSource.cpp : Hiermit werden die exportierten Funktionen für die DLL definiert.
//

#include "pch.h"
#include "framework.h"
#include "CPC_NG_11_0B_Xplatforms_OpenSource.h"

typedef INT8 byte;

static byte GetBit(byte data, int bitNumber)
{
	byte result = 0xFF;

	if (bitNumber <= 7)
		result = (byte)((data >> bitNumber) & 1);

	return result;
}

UINT32 CPCNG_GenSeedKeyLevel(UINT32 seedRequest, int level)
{
	int * shifttable = nullptr;
	UINT32 * hashtable = nullptr;

	if (level == 0x0B)
	{
		shifttable = new int[12]
		{
		0, 3, 1, 3, 2, 1, 3, 1, 7, 5, 2, 0
		};
		hashtable = new UINT32[8]{
		0xE3288EC6, 0x2E789639, 0xE6158894, 0x01506AF7, 0x89E648EA, 0xA84371AA, 0xF19D2734, 0x493DA574
		};
	}
	else if (level == 0x11)
	{
		shifttable = new int[12]
		{
		3, 1, 0, 2, 1, 2, 4, 2, 6, 7, 1, 5
		};
		hashtable = new UINT32[8]{
		0xE204E889, 0x13393EA3, 0xEA851438, 0xF3513BEF, 0xA37A2732, 0xAEBAE767, 0x1488EFF5, 0x2964C776
		};
	}
	else
		return 0;

	byte * seed = new byte[4]{ (byte)(seedRequest >> 24), (byte)(seedRequest >> 16), (byte)(seedRequest >> 8), (byte)seedRequest };

	byte n = (byte)(seed[3 - shifttable[1]] ^ seed[3 - shifttable[0]]);

	int a = GetBit(seed[3 - shifttable[2]], shifttable[6]);
	int b = GetBit(seed[3 - shifttable[3]], shifttable[7]);
	int c = GetBit(n, shifttable[8]);

	int index = (a << 2) | (b << 1) | c;

	UINT32 x = hashtable[index] ^ seedRequest;

	a = GetBit(seed[3 - shifttable[4]], shifttable[9]);
	b = GetBit(n, shifttable[10]);
	c = GetBit((byte)(x >> (8 * shifttable[5])), shifttable[11]);

	index = (a << 2) | (b << 1) | c;

	UINT32 y = (hashtable[index] >> 8) | (hashtable[index] << 24);

	UINT32 key = x ^ y;

	return key;
}



CPCNG110BXPLATFORMSOPENSOURCE_API const char * __cdecl GetECUName(void)
{
	return "CPC_NG";
}

CPCNG110BXPLATFORMSOPENSOURCE_API const char * GetComment(void)
{
	return "OpenSource CPC_NG SeedKey Generator library for Access Levels: 0x11 & 0x0B. Author: <a href='https://github.com/Xplatforms/mbseedkey'>Xplatforms</a>";
}

CPCNG110BXPLATFORMSOPENSOURCE_API int GetConfiguredAccessTypes(int * data)
{
	if (data == nullptr)return 0;
	data[0] = 0x11;
	data[1] = 0x0B;
	return 2;
}

CPCNG110BXPLATFORMSOPENSOURCE_API int GetSeedLength(int)
{
	return 4;
}

CPCNG110BXPLATFORMSOPENSOURCE_API int GetKeyLength(int)
{
	return 4;
}

UINT32 deserialize_uint32(const unsigned char *buffer)
{
	UINT32 value = 0;

	value |= buffer[0] << 24;
	value |= buffer[1] << 16;
	value |= buffer[2] << 8;
	value |= buffer[3];
	return value;

}

CPCNG110BXPLATFORMSOPENSOURCE_API int GenerateKeyExOpt(const unsigned char* ipSeedArray,
	unsigned int iSeedArraySize,
	const unsigned int iSecurityLevel,
	const char * ipVariant,
	const char* ipOptions,
	unsigned char* iopKeyArray,
	unsigned int iMaxKeyArraySize,
	unsigned int& oActualKeyArraySize)
{

	UINT32 seed32 = deserialize_uint32(ipSeedArray);

	byte key[8] = { 0 };
	UINT32 tmp_key = 0;

	switch (iSecurityLevel)
	{
	case 0x11:
		tmp_key = CPCNG_GenSeedKeyLevel(seed32, 11);
		break;
	case 0x0B:
		tmp_key = CPCNG_GenSeedKeyLevel(seed32, 17);
		break;
	default:
		tmp_key = CPCNG_GenSeedKeyLevel(seed32, 11);
	}



	key[0] = (byte)(tmp_key >> 24);
	key[1] = (byte)(tmp_key >> 16);
	key[2] = (byte)(tmp_key >> 8);
	key[3] = (byte)(tmp_key) & 0xFF;
	
	memcpy(iopKeyArray, key, 4);
	oActualKeyArraySize = 4;

	return 4;
}