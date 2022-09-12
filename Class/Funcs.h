#ifndef FuncsH
#define FuncsH
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <winapi.tlhelp32.hpp>
#include <msxmldom.hpp>
#include <XMLDoc.hpp>
#include <xmldom.hpp>




//extern const count_digit = 3;
//extern UnicodeString Mas[10];
//extern UnicodeString MasCh[10];
//extern UnicodeString MasD[10];

UnicodeString __fastcall DecodeStr(UnicodeString str);
UnicodeString __fastcall ReverseDigit(UnicodeString str);
UnicodeString __fastcall ReverseChar(UnicodeString str);
UnicodeString __fastcall FullN(int a,int n);

void __fastcall _Exec(UnicodeString Cmd );
HANDLE  __fastcall _ExecEx(UnicodeString Cmd );

void __fastcall _KillProcess(UnicodeString Cmd );
void __fastcall _KillProcess1(UnicodeString Cmd );
void __fastcall _KillProcess(int pid );
bool __fastcall _IsExistProcess( int pid );
bool __fastcall _CheckExeFile(UnicodeString Cmd );

bool __fastcall _CheckProcess();
bool __fastcall _CheckProcess( UnicodeString Cmd );
UnicodeString __fastcall _ExtractFileName(UnicodeString Cmd );

int RGBtoColor(short r,short b,short g);
UnicodeString ColorToRGB(int color);
int RGBStringToColor( UnicodeString s);

UnicodeString __fastcall XMLNodeAttribute( _di_IXMLNode node,UnicodeString attr,UnicodeString def );
int __fastcall XMLNodeIntAttribute( _di_IXMLNode node,UnicodeString attr, int def );

#endif
