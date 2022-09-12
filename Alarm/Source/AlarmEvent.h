//---------------------------------------------------------------------------

#ifndef AlarmEventH
#define AlarmEventH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>

// информация о событии
class TAlarmEvent
{
private:
    int FId;                // j_trevogi.id_object
    String FPp;             // j_srpav_pp.v_pp
    String FAlarmDesc;      // j_trevogi.v_text_trevogi
    TDateTime FAlarmDate;   // j_trevogi.v_date_time_voz_trev
    String FKolvo;          // j_trevogi.v_kolvo_sob_trev
    bool FAlarmCheck;       // j_trevogi.v_alarm_check
    bool FAlarmEnable;      // j_trevogi.v_alarm_enable
    TDateTime FAlarmReleased;    // j_trevogi.v_date_time_sniat_trev
public:
    __property int Id = { read = FId, write = FId };
    __property String Pp = { read = FPp, write = FPp };
    __property String AlarmDesc = { read = FAlarmDesc, write = FAlarmDesc };
    __property TDateTime AlarmDate = { read = FAlarmDate, write = FAlarmDate };
    __property String Kolvo = { read = FKolvo, write = FKolvo };
    __property bool AlarmCheck = { read = FAlarmCheck, write = FAlarmCheck };
    __property bool AlarmEnable = { read = FAlarmEnable, write = FAlarmEnable };
    __property TDateTime AlarmReleased = { read = FAlarmReleased, write = FAlarmReleased };
};

#endif
