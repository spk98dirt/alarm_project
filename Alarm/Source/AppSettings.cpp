//---------------------------------------------------------------------------

#pragma hdrstop

#include "AppSettings.h"
#include <System.IniFiles.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall TAppSettings::TAppSettings()
{
    FIniSettings = new TAppIniSettings();
}

__fastcall TAppSettings::~TAppSettings()
{
    delete FIniSettings;
}

String __fastcall TAppSettings::ReverseChar(String str)
{
  UnicodeString result = "";
  int k;
  for (int i=1; i<=str.Length(); i++)
   {
    k = -1;
    for (int j=0; j<10; j++)
     {
      if (UnicodeString(str[i])==MasCh[j])
       {
        k = j;
        break;
        }
      }
    if (k>=0)
     {
      result = result + MasD[k];
      } else {
      result = result + str[i];
      }
    }
  return result;
}

String __fastcall TAppSettings::DecodeStr(String str)
{
    Mas[0] = "$45";     MasCh[0] = "A";     MasD[0] = "0";
    Mas[1] = "$57";     MasCh[1] = "D";     MasD[1] = "1";
    Mas[2] = "$D4";     MasCh[2] = "B";     MasD[2] = "2";
    Mas[3] = "$2";      MasCh[3] = "L";     MasD[3] = "3";
    Mas[4] = "$1C";     MasCh[4] = "P";     MasD[4] = "4";
    Mas[5] = "$77";     MasCh[5] = "X";     MasD[5] = "5";
    Mas[6] = "$EE";     MasCh[6] = "Z";     MasD[6] = "6";
    Mas[7] = "$A5";     MasCh[7] = "W";     MasD[7] = "7";
    Mas[8] = "$F3";     MasCh[8] = "Q";     MasD[8] = "8";
    Mas[9] = "$7";      MasCh[9] = "R";     MasD[9] = "9";

    UnicodeString result = "";
    wchar_t ch;
    UnicodeString str_tmp = str;
    UnicodeString tmp;
    int i = 1;
    while (str_tmp.Length()>0)
    {
        tmp = str_tmp.SubString(1,count_digit);
        tmp = ReverseChar(tmp);
        int a,c,b;
        try
         {
          b = StrToInt(tmp);
          }
        catch (...)
         {
          b = 0;
          }
        int j = i - int(i / 10)*10;
        a = StrToInt(Mas[j]);
        c = b - a;
        ch = c;
        result = result + ch;
        str_tmp = str_tmp.SubString(count_digit+1,str_tmp.Length());
        i++;
    }
    return result;
}

void __fastcall TAppSettings::ParseIniFile(String inifile)
{
    TIniFile* ini = new TIniFile(inifile);
    try
    {
        FIniSettings->UsernameOracle = ini->ReadString("Connection","UsernameOracle", "ellips");
        String crypt_pass = ini->ReadString("Connection","CryptPasswordOracle", "");
        if(crypt_pass.IsEmpty())
            FIniSettings->PasswordOracle = ini->ReadString("Connection","PasswordOracle", "ellips");
        else
            FIniSettings->PasswordOracle = DecodeStr(crypt_pass);

        FIniSettings->HostOracle = ini->ReadString("Connection","HostOracle",     "localhost");
        FIniSettings->PortOracle = ini->ReadString("Connection","PortOracle",     "1521");
        FIniSettings->BaseOracle = ini->ReadString("Connection","BaseOracle",     "xe");
        FIniSettings->SchemaDatabase = ini->ReadString("Connection","SchemaDatabase", "");

    }
    __finally
    {
        delete ini;
    }

}


