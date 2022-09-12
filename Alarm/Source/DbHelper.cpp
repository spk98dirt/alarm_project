//---------------------------------------------------------------------------

#pragma hdrstop

#include "DbHelper.h"
#include "AlarmEvent.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall TDbHelper::TDbHelper()
{
    FConn = new TUniConnection(NULL);
}

__fastcall TDbHelper::~TDbHelper()
{
    delete FConn;
}

void __fastcall TDbHelper::SetConnProperties(TAppIniSettings* ini_settings)
{
    FIniSettings = ini_settings;
    if(FConn->Connected)
        FConn->Connected = false;


    FConn->ProviderName = "Oracle";
    FConn->Port         = StrToIntDef(FIniSettings->PortOracle, 1521);
    FConn->Username     = FIniSettings->UsernameOracle;
    FConn->Password     = FIniSettings->PasswordOracle;
    FConn->Database     = FIniSettings->BaseOracle;
    FConn->Server       = FIniSettings->HostOracle + ":" + FIniSettings->PortOracle + ":" + FIniSettings->BaseOracle;
    FConn->SpecificOptions->Values["Direct"] = "True";
    FConn->LoginPrompt = false;
}

bool __fastcall TDbHelper::CheckConnection()
{
//    FConn->PerformConnect(true);
    FConn->Connected = true;

    TUniQuery* q = new TUniQuery(NULL);
    try
    {
        q->Connection = FConn;
        q->SQL->Text = "ALTER SESSION SET NLS_NUMERIC_CHARACTERS = ', '";
        q->ExecSQL();
        q->Close();

        if(!FIniSettings->SchemaDatabase.IsEmpty())
        {
            q->SQL->Text = "ALTER SESSION SET CURRENT_SCHEMA=" + FIniSettings->SchemaDatabase;
            q->ExecSQL();
        }
    }
    __finally
    {
        delete q;
    }

    return FConn->Connected;
}

void __fastcall TDbHelper::GetLogins(TStrings *sl)
{
    sl->Clear();

    TUniQuery* q = new TUniQuery(NULL);
    try
    {
        q->Connection = FConn;
        q->SQL->Text = "SELECT login FROM Users ORDER BY login";
        q->Open();
        while(!q->Eof)
        {
            sl->Add(q->FieldByName("login")->AsString);
            q->Next();
        }
    }
    __finally
    {
        delete q;
    }
}

bool __fastcall TDbHelper::IsPswValid(String login, String passw)
{
    TUniQuery* q = new TUniQuery(NULL);
    try
    {
        q->Connection = FConn;
        q->SQL->Text = "SELECT count(*) as cnt FROM Users WHERE login = :a1 and parol = :a2";
        q->Params->Items[0]->AsString = login;
        q->Params->Items[1]->AsString = passw;
        q->Open();

        return q->FieldByName("cnt")->AsInteger == 1;
    }
    __finally
    {
        delete q;
    }
}

void __fastcall TDbHelper::ReloadAlarmEvents(TList *sl, TDateTime act_dt)
{
    TUniQuery* q = new TUniQuery(NULL);
    try
    {
        q->Connection = FConn;

        q->SQL->Add(" SELECT t.id_object tid,pp.v_pp,pp.v_name_pp_cascad,s.v_alarm_sprav_name,t.*");
        q->SQL->Add(" FROM j_trevogi t,j_srpav_pp pp,journal_1 r,alarm_sprav s");
        q->SQL->Add(" WHERE pp.id_object = t.o_pp");
        q->SQL->Add(" AND pp.o_rayon = r.id_object(+)");
        q->SQL->Add(" AND t.o_alarm_sprav_id = s.id_object");
        q->SQL->Add("AND s.v_alarm_sprav_onoff = 'Да' AND v_date_time_sniat_trev IS NULL");
        q->SQL->Add("AND ( v_tip_trevogi = 'Тревога' OR v_tip_trevogi = 'Авария' )");

        q->SQL->Add(" AND t.id_object NOT IN");
        q->SQL->Add("( SELECT t1.id_object FROM j_trevogi t1, j_Alarm_Exception e1");
        q->SQL->Add(" WHERE t1.o_pp = e1.o_pp AND t1.o_alarm_sprav_id = e1.o_alarm_sprav_id");
        q->SQL->Add(" AND e1.v_alarmexception_dateend > :a1 )");
        q->SQL->Add(" ORDER BY v_alarm_check DESC,v_date_time_voz_trev DESC");
        q->Params->Items[0]->AsDateTime = act_dt;

        q->Open();
        while(!q->Eof)
        {
            TAlarmEvent* ev = new TAlarmEvent();
            ev->Id = q->FieldByName("tid")->AsInteger;
            ev->Pp = q->FieldByName("v_pp")->AsString;
            ev->AlarmDesc = q->FieldByName("v_text_trevogi")->AsString;
            ev->AlarmCheck = q->FieldByName("v_alarm_check")->AsString == "Да";
            ev->AlarmCheck = q->FieldByName("v_alarm_enable")->AsString == "Да";
            ev->AlarmDate = q->FieldByName("v_date_time_voz_trev")->AsDateTime;
            ev->Kolvo = q->FieldByName("v_kolvo_sob_trev")->AsString;
            ev->AlarmReleased = q->FieldByName("v_date_time_voz_trev")->AsDateTime;

            sl->Add(ev);

            q->Next();
        }
    }
    __finally
    {
        delete q;
    }

}

