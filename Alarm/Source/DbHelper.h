//---------------------------------------------------------------------------

#ifndef DbHelperH
#define DbHelperH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Data.DB.hpp>
#include "Uni.hpp"
#include "AppIniSettings.h"

class TDbHelper
{
private:
    TUniConnection *FConn;
    TAppIniSettings* FIniSettings;
public:
    bool __fastcall CheckConnection();
    void __fastcall SetConnProperties(TAppIniSettings* ini_settings);

    void __fastcall GetLogins(TStrings *sl);
    bool __fastcall IsPswValid(String login, String passw);
    void __fastcall ReloadAlarmEvents(TList *sl, TDateTime act_dt);

    __fastcall TDbHelper();
    __fastcall ~TDbHelper();
};

#endif
