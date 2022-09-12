//---------------------------------------------------------------------------

#ifndef AppHelperH
#define AppHelperH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "AppSettings.h"
#include "DbHelper.h"

class TAppHelper
{
private:
    TAppSettings* FAppSettings;
    TLoginInfo* FLoginInfo;
    TDbHelper* FDbHelper;
    String FConfigFileName;
    bool FMaximizedForm;
public:
    __property TLoginInfo* LoginInfo = { read = FLoginInfo };
    __property TAppSettings* AppSettings = { read = FAppSettings };
    __property TDbHelper* DbHelper = { read = FDbHelper };
    __property String ConfigFileName = { read = FConfigFileName };
    __property bool MaximizedForm = { read = FMaximizedForm };

    void __fastcall Initialize();

    __fastcall TAppHelper();
    __fastcall ~TAppHelper();
};

extern TAppHelper* apphelper;

#endif
