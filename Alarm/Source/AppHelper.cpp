//---------------------------------------------------------------------------

#pragma hdrstop

#include "AppHelper.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TAppHelper* apphelper;

__fastcall TAppHelper::TAppHelper()
{
    FAppSettings = new TAppSettings();
    FLoginInfo = new TLoginInfo("", "");
    FDbHelper = new TDbHelper();
    FMaximizedForm = false;
}

__fastcall TAppHelper::~TAppHelper()
{
    delete FDbHelper;
    delete FAppSettings;
    delete FLoginInfo;
}

void __fastcall TAppHelper::Initialize()
{
    String tmp_name, tmp_pass;

    int n = ParamCount();
    for (int i = 1; i <= n; i++)
    {
        String s = ParamStr(i);
        if (s.Pos("-config=") == 1)
            FConfigFileName = GetCurrentDir() + "\\" + s.SubString(9, s.Length()-8);
        if(s == "-u" && i < n)
            tmp_name = ParamStr(i + 1);
        if(s == "-p" && i < n)
            tmp_pass = ParamStr(i + 1);
        if(ParamStr(i) == "-max")
            FMaximizedForm = true;
    }

    if (FConfigFileName.IsEmpty())
        throw Exception("Не указан файл конфигурации!");
    if (!FileExists(FConfigFileName))
        throw Exception("Не найден файл конфигурации: " + FConfigFileName);

    if(!tmp_name.IsEmpty())
    {
        FLoginInfo->Name = tmp_name;
        FLoginInfo->Password = tmp_pass;
    }

    FAppSettings->ParseIniFile(FConfigFileName);
    FDbHelper->SetConnProperties(FAppSettings->IniSettings);
    FDbHelper->CheckConnection();
}


