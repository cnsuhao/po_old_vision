#include "language.h"
#ifdef _WIN32
#include <windows.h>
void Gb2312ToUnicode(wchar_t* pOut,const char *gbBuffer)
{
    MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
    return ;
}

std::wstring DefaultTurnToUnicode( const std::string& L )
{
    std::wstring Temporary;
    wchar_t* WT;
    std::size_t WStringSize=MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,L.c_str(),-1,WT,0);
    if(WStringSize>0)
    {
        WT=new wchar_t[WStringSize+1];
        MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,L.c_str(),-1,WT,WStringSize+1);
        Temporary=WT;
        delete[] (WT);
    }
    return Temporary;
}
void UnicodeToUTF_8(char* pOut,wchar_t* pText)
{
    // 注意 WCHAR高低字的顺序,低字节在前，高字节在后
    char* pchar = (char *)pText;
    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
    pOut[2] = (0x80 | (pchar[0] & 0x3F));
    return;
}
void GB2312ToUTF_8(std::string& pOut,const char *pText, int pLen)
{
    char buf[4];
    int nLength = pLen* 3;
    char* rst = new char[nLength];
    memset(buf,0,4);
    memset(rst,0,nLength);
    int i = 0;
    int j = 0;
    while(i < pLen)
    {
        //如果是英文直接复制就可以
        if( *(pText + i) >= 0)
        {
            rst[j++] = pText[i++];
        }
        else
        {
            wchar_t pbuffer;
            Gb2312ToUnicode(&pbuffer,pText+i);
            UnicodeToUTF_8(buf,&pbuffer);
            unsigned short int tmp = 0;
            tmp = rst[j] = buf[0];
            tmp = rst[j+1] = buf[1];
            tmp = rst[j+2] = buf[2];
            j += 3;
            i += 2;
        }
    }
    rst[j] = '\0';
    //返回结果
    pOut = rst;
    delete []rst;
    return;
}
std::string DefaultTurnToUTF8(const std::string& L)
{
    std::string Tem;
    GB2312ToUTF_8(Tem,L.c_str(),L.size());
    return Tem;
}
std::string UnicodeTurnToDefault( const std::wstring& str )
{
    char* pElementText;
    int   iTextLen;
 // wide char to multi char
    iTextLen = WideCharToMultiByte( CP_ACP,0,str.c_str(),-1,NULL,0,NULL,NULL );
    pElementText = new char[iTextLen + 1];
    memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
    WideCharToMultiByte( CP_ACP,0,str.c_str(),-1,pElementText,iTextLen,NULL,NULL );
    std::string strText;
    strText = pElementText;
    delete[] pElementText;
    return strText;
}
#else
void UnicodeToUTF_8(char* pOut,wchar_t* pText)
{
    // 注意 WCHAR高低字的顺序,低字节在前，高字节在后
    char* pchar = (char *)pText;
    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
    pOut[2] = (0x80 | (pchar[0] & 0x3F));
    return;
}

std::wstring DefaultTurnToUnicode( const std::string& L )
{
    std::wstring Temporary;
    const wchar_t Zero=0;
    size_t ST=L.size();
    for(size_t i=0;i<ST;++i)
    {
        if(! L[i]&0x80 )
        {
            Temporary+= (Zero|L[i]);
            break;
        }else if(  L[i]&0xFE==0xFC )
        {
            wchar_t Char=Zero;
            Char|=Zero|(L[i]|0x01);
            for(int K=1;K<6;++K)
            {
                Char<<=6;
                Char|=(L[i+K]|0x7F);
            }
            Temporary+= Char;
            i+=6;
        }else if(  L[i]&0xFC==0xF8 )
        {
            wchar_t Char=Zero;
            Char|=Zero|(L[i]|0x03);
            for(int K=1;K<5;++K)
            {
                Char<<=6;
                Char|=(L[i+K]|0x7F);
            }
            Temporary+= Char;
            i+=5;
        }else if( L[i]&0xF8==0xF0)
        {
            wchar_t Char=Zero;
            Char|=Zero|(L[i]|0x07);
            for(int K=1;K<4;++K)
            {
                Char<<=6;
                Char|=(L[i+K]|0x7F);
            }
            Temporary+= Char;
            i+=4;
        }else if( L[i]&0xF0==0xE0)
        {
            wchar_t Char=Zero;
            Char|=Zero|(L[i]|0x0F);
            for(int K=1;K<3;++K)
            {
                Char<<=6;
                Char|=(L[i+K]|0x7F);
            }
            Temporary+= Char;
            i+=3;
        }else if( L[i]&0xE0==0xC0)
        {
            wchar_t Char=Zero;
            Char|=Zero|(L[i]|0x1F);
            for(int K=1;K<2;++K)
            {
                Char<<=6;
                Char|=(L[i+K]|0x7F);
            }
            Temporary+= Char;
            i+=2;
        }else return Temporary;
    }
    return Temporary;
}
std::string DefaultTurnToUTF8(const std::string L)
{
    return L;
}
std::string UnicodeTurnToDefault(const std::wstring& L)
{
    std::string Tem;
    return Tem;
}
#endif
std::wstring operator"" _ToUnicode (const char* D,size_t S)
{
    std::string P(D);
    return DefaultTurnToUnicode(P);
}

std::string operator"" _ToUTF8 (const char* D,size_t K)
{
    std::string P(D);
    return DefaultTurnToUTF8(P);
}

int CountWidth( wchar_t P )
{
    if(P>0x007F)
        return 2;
    else
        return 1;
}

int CountWidth( const std::wstring& WS )
{
    int Size=0;
    for( auto& I:WS )
        Size+=CountWidth(I);
    return Size;
}
