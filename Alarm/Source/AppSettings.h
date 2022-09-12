//---------------------------------------------------------------------------

#ifndef AppSettingsH
#define AppSettingsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "AppIniSettings.h"

// настройки приложения
class TAppSettings
{
private:
	String Mas[10];
	String MasCh[10];
	String MasD[10];
    int count_digit;
    TAppIniSettings* FIniSettings;
	String __fastcall DecodeStr(String str);
	String __fastcall ReverseChar(String str);
public:
    __property TAppIniSettings* IniSettings = { read = FIniSettings };

    void __fastcall ParseIniFile(String inifile);

    __fastcall TAppSettings();
    __fastcall ~TAppSettings();
};

class TLoginInfo
{
private:
    String FName;
    String FPassword;
public:
    __property String Name = { read = FName, write = FName };
    __property String Password = { read = FPassword, write = FPassword };

	inline __fastcall TLoginInfo(String login, String psw) { FName = login, FPassword = psw; };
	inline static TLoginInfo* __fastcall Get(String login, String psw) { return new TLoginInfo(login, psw); };
};


#endif
