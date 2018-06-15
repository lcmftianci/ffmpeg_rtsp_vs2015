#include"base64.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char myBase64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
const char myBase64Map[256] =
{
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 62, 255, 255, 255, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 255, 255,
	255, 254, 255, 255, 255, 0, 1, 2, 3, 4, 5, 6,
	7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
	19, 20, 21, 22, 23, 24, 25, 255, 255, 255, 255, 255,
	255, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
	37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
	49, 50, 51, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255
};

static const char*g_pCodes="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
static const unsigned char g_pMap[256]={255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,62,255,255,255,63,
52,53,54,55,56,57,58,59,60,61,255,255,
255,254,255,255,255,0,1,2,3,4,5,6,
7,8,9,10,11,12,13,14,15,16,17,18,
19,20,21,22,23,24,25,255,255,255,255,255,
255,26,27,28,29,30,31,32,33,34,35,36,
37,38,39,40,41,42,43,44,45,46,47,48,
49,50,51,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255
};

CBase64::CBase64()
{
}

CBase64::~CBase64()
{
}

bool CBase64::Encode(const unsigned char*pIn,unsigned long uInLen,string&strOut)
{
unsigned long i,len2,leven;
strOut="";

//ASSERT((pIn!=NULL)&&(uInLen!=0)&&(pOut!=NULL)&&(uOutLen!=NULL));

len2=((uInLen+2)/3)<<2;
//if((*uOutLen)<(len2+1))returnfalse;

//p=pOut;
leven=3*(uInLen/3);
for(i=0;i<leven;i+=3)
{
strOut+=g_pCodes[pIn[0]>>2];
strOut+=g_pCodes[((pIn[0]&3)<<4)+(pIn[1]>>4)];
strOut+=g_pCodes[((pIn[1]&0xf)<<2)+(pIn[2]>>6)];
strOut+=g_pCodes[pIn[2]&0x3f];
pIn+=3;
}

if(i<uInLen)
{
unsigned char a=pIn[0];
unsigned char b=((i+1)<uInLen)?pIn[1]:0;
unsigned char c=0;

strOut+=g_pCodes[a>>2];
strOut+=g_pCodes[((a&3)<<4)+(b>>4)];
strOut+=((i+1)<uInLen)?g_pCodes[((b&0xf)<<2)+(c>>6)]:'=';
strOut+='=';
}

//*p=0;//AppendNULLbyte
//*uOutLen=p-pOut;
return true;
}

bool CBase64::Decode(const string& strIn, unsigned char * pOut, unsigned long *uOutLen)
{
 unsigned long t, x, y, z;
 unsigned char c;
 unsigned long g = 3;
 for(x = y = z = t = 0;x < strIn.length();x++)
{
 c = g_pMap[strIn[x]];
 if(c == 255) continue;
 if(c == 254) {c = 0; g--;}

 t = (t << 6) | c;

 if(++y == 4)
 {
  if((z + g) < *uOutLen) {return false;}
  pOut[z++] = (unsigned char)((t>>16)&255);
  if(g > 1) pOut[z++] = (unsigned char)((t>>8)&255);
  if(g > 2) pOut[z++] = (unsigned char)(t&255);
  y = t = 0;
 }
}
*uOutLen = z;
return true;
}

int CBase64::EncodeBase64(const unsigned char* pSrc, unsigned char* pDst, int nSrcLen)
{
	if (nullptr == pSrc || 0 == nSrcLen)
		return -1;

	unsigned char c1, c2, c3;
	unsigned int nDstLen = 0;
	unsigned int nDiv = nSrcLen / 3;
	unsigned int nMod = nSrcLen % 3;

	for (int i = 0; i < nDiv; i++)
	{
		c1 = *pSrc++;
		c2 = *pSrc++;
		c3 = *pSrc++;

		*pDst++ = myBase64Table[c1 >> 2];
		*pDst++ = myBase64Table[((c1 << 4) | c2 >> 4) & 0x3f];
		*pDst++ = myBase64Table[((c2 << 2) | c3 >> 6) & 0x3f];
		*pDst++ = myBase64Table[c3 & 0x3f];
		nDstLen += 4;
	}

	if (1 == nMod)
	{
		*pDst++ = myBase64Table[(*pSrc) >> 2];
		*pDst++ = myBase64Table[((*pSrc) << 4) & 0x3f];
		*pDst++ = '=';
		*pDst++ = '=';
		nDstLen += 4;
	}
	else if (2 == nMod)
	{
		c1 = *pSrc++;
		c2 = *pSrc++;

		*pDst++ = myBase64Table[c1 >> 2];
		*pDst++ = myBase64Table[((c1 << 4) | c2 >> 4) & 0x3f];
		*pDst++ = myBase64Table[c2 >> 2];
		*pDst++ = '=';
		nDstLen += 4;
	}

	*pDst = '\0';
	return nDstLen;
}

int CBase64::DecodeBase64(const unsigned char* pSrc, unsigned char* pDst, int nSrcLen)
{
	if (nullptr == pSrc || 0 == nSrcLen)
		return -1;

	unsigned int nDstLen = 0;
	unsigned char pc; // now char ptr  
	unsigned long ncode = 0;
	unsigned int dc = 0; //'=' count  
	unsigned int nc = 0; // get char count  
	for (int i = 0; i < nSrcLen; i++)
	{
		pc = myBase64Map[*pSrc++];

		if (pc == 0xff)continue;
		if (pc == 0xfe){ pc = 0; dc++; }

		ncode = (ncode << 6) | pc;

		if (++nc == 4)
		{
			*pDst++ = (unsigned char)(ncode >> 16 & 0xff);
			nDstLen++;
			if (dc < 2)
			{
				*pDst++ = (unsigned char)((ncode >> 8) & 0xff);
				nDstLen++;
			}
			if (dc < 1)
			{
				*pDst++ = (unsigned char)(ncode & 0xff);
				nDstLen++;
			}
			nc = 0;
			ncode = 0;
		}
	}
	return nDstLen;
}

 
string CBase64::base64Decode(char * in, int inlen)
{
    unsigned char* out = (unsigned char *)malloc(inlen);
    unsigned long t, x, y, z;
    unsigned char c;
    int g = 3;
 
    for (x = y = z = t = 0; x<inlen;)
        {
            c = g_pMap[in[x++]];
#if 0		//如果是换行获取空格时程序就跳出了
            if (c == 255) 
				return "";
            if (c == 253) 
				continue;
            if (c == 254)
              {
                    c = 0;
                    g--;
             }
#endif
            t = (t<<6)|c;
            if (++y == 4)
                {
                    out[z++] = (unsigned char)((t>>16)&255);
                    if (g > 1) out[z++] = (unsigned char)((t>>8)&255);
                    if (g > 2) out[z++] = (unsigned char)(t&255);
                    y = t = 0;
                }
        }
    //memcpy(in,out,z);
	string strOut = "";
	strOut += (const char*)out;
    free(out);
	return strOut;
}


bool CBase64::base64Encode(char *bindata, size_t inlen, char **out, size_t *outlen)
{
	size_t _outlen = *outlen;
	char *_out = NULL;
	size_t out_pos = 0;
	if(NULL == *out)
	{
		_outlen = (inlen / 3 + (inlen%3 != 0)) * 4 + 1;
		_out = (char *)malloc(_outlen);
	}
	else
	{
		_outlen = *outlen;
	    _out = *out;
	}
	memset(_out,'=',_outlen);
	_out[_outlen-1] = 0;
	int bits_collected = 0;
	int accumulator = 0;
	for(int i = 0; i < inlen; i++)
	{
		accumulator = (accumulator << 8) | (bindata[i] & 0xffu);
		bits_collected += 8;
		while (bits_collected >= 6)
		{
			bits_collected -= 6;
			_out[out_pos++] = g_pCodes[(accumulator >> bits_collected) & 0x3fu];
		}
	}
	if(bits_collected >= 6)
	{
		if(NULL == *out)
		{
			free(_out);
		}
		return NULL;
	}								

	if(bits_collected >0)
	{
		// Any trailing bits that are missing.
		accumulator <<= 6 - bits_collected;
		_out[out_pos++] = g_pCodes[accumulator & 0x3fu];
	}								
	*outlen = _outlen;
	*out = _out;
//	return _out;
	return true;
}
