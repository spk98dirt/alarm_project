//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AlarmEditUnit1.h"

#define Log(message) fsSyntaxMemo3->Lines->Add(message)

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "fs_synmemo"
#pragma link "fs_iinterpreter"
#pragma link "fs_icpp"
#pragma link "DBAccess"
#pragma link "OracleUniProvider"
#pragma link "Uni"
#pragma link "UniProvider"
#pragma link "DBAccess"
#pragma link "OracleUniProvider"
#pragma link "Uni"
#pragma link "UniProvider"
#pragma resource "*.dfm"


TAlarmEditForm1 *AlarmEditForm1;
//---------------------------------------------------------------------------
__fastcall TAlarmEditForm1::TAlarmEditForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TAlarmEditForm1::InitSem(){
   try {
	  Global1    = new TKGlobalSem(UniConnection1);
	  AlarmSprav = new TKAlarmSprav;
	  fsSyntaxMemo1->Lines->Clear();
	  fsSyntaxMemo2->Lines->Clear();
	  fsSyntaxMemo3->Lines->Clear();

	  UnicodeString type = "";
	  AlarmSprav->IdObject = StrToIntDef(Global1->GetArg("-id"),-1);
// Читаем параметры тревоги из базы
	  Global1->Query->Close();
	  Global1->Query->SQL->Clear();
	  Global1->Query->SQL->Add("SELECT * FROM alarm_sprav WHERE id_object = :a1");
	  Global1->Query->Params->Items[0]->AsInteger = AlarmSprav->IdObject;
	  Global1->Query->Open();
	  if( !Global1->Query->Eof ){
		  type  = Global1->Query->FieldByName("v_alarm_type_event")->AsString;
		  AlarmSprav->Type      = Global1->Query->FieldByName("v_alarm_type_event")->AsString;
		  AlarmSprav->ShortName = Global1->Query->FieldByName("v_alarm_name")->AsString;
		  AlarmSprav->Name      = Global1->Query->FieldByName("v_alarm_sprav_name")->AsString;
		  AlarmSprav->Script    = Global1->Query->FieldByName("v_alarm_sprav_algoritm")->AsString;
		  AlarmSprav->AutoCheck = StrToIntDef(Global1->Query->FieldByName("v_alarm_sprav_time_kvit")->AsString,0);
	  }
	  Global1->Query->Close();
	  Log("Справочник тревог");
	  Log("ID="+IntToStr(AlarmSprav->IdObject));
	  Log("Name="+AlarmSprav->Name);
	  Log("Key="+AlarmSprav->ShortName);
	  Log("Type="+type);
	  fsSyntaxMemo2->Lines->Text = AlarmSprav->Script;



   } catch (Exception &e) {
	  Log("Ошибка инициализации БД "+e.Message);
   }
}



void __fastcall TAlarmEditForm1::InitParams(){
   try {
	   fsSyntaxMemo1->Lines->Add("/**");
	   fsSyntaxMemo1->Lines->Add("* Переменные, которые заполняет сервис");
	   fsSyntaxMemo1->Lines->Add("*/");
	   fsSyntaxMemo1->Lines->Add("");
	   fsSyntaxMemo1->Lines->Add("// Связь");
	   fsSyntaxMemo1->Lines->Add("bool Link = true;");
	   fsSyntaxMemo1->Lines->Add("// Напряжения");
	   fsSyntaxMemo1->Lines->Add("double Ua = 220.0;");
	   fsSyntaxMemo1->Lines->Add("double Ub = 220.0;");
	   fsSyntaxMemo1->Lines->Add("double Uc = 220.0;");
	   fsSyntaxMemo1->Lines->Add("// Границы напряжений");
	   fsSyntaxMemo1->Lines->Add("double Ua_min = 200.0;");
	   fsSyntaxMemo1->Lines->Add("double Ub_min = 200.0;");
	   fsSyntaxMemo1->Lines->Add("double Uc_min = 200.0;");
	   fsSyntaxMemo1->Lines->Add("double Ua_max = 240.0;");
	   fsSyntaxMemo1->Lines->Add("double Ub_max = 240.0;");
	   fsSyntaxMemo1->Lines->Add("double Uc_max = 240.0;");
	   fsSyntaxMemo1->Lines->Add("// Токи");
	   fsSyntaxMemo1->Lines->Add("double Ia = 0;");
	   fsSyntaxMemo1->Lines->Add("double Ib = 0;");
	   fsSyntaxMemo1->Lines->Add("double Ic = 0;");
	   fsSyntaxMemo1->Lines->Add("// Границы токов");
	   fsSyntaxMemo1->Lines->Add("double Ia_min = 0;");
	   fsSyntaxMemo1->Lines->Add("double Ib_min = 0;");
	   fsSyntaxMemo1->Lines->Add("double Ic_min = 0;");
	   fsSyntaxMemo1->Lines->Add("double Ia_max = 100.0;");
	   fsSyntaxMemo1->Lines->Add("double Ib_max = 100.0;");
	   fsSyntaxMemo1->Lines->Add("double Ic_max = 100.0;");
	   fsSyntaxMemo1->Lines->Add("// Ошибка симистора");
	   fsSyntaxMemo1->Lines->Add("bool SemA = false;");
	   fsSyntaxMemo1->Lines->Add("bool SemB = false;");
	   fsSyntaxMemo1->Lines->Add("bool SemC = false;");
	   fsSyntaxMemo1->Lines->Add("// Перегрузка (тепловая защита)");
	   fsSyntaxMemo1->Lines->Add("bool PerA = false;");
	   fsSyntaxMemo1->Lines->Add("bool PerB = false;");
	   fsSyntaxMemo1->Lines->Add("bool PerC = false;");
	   fsSyntaxMemo1->Lines->Add("// КЗ");
	   fsSyntaxMemo1->Lines->Add("bool KZA = false;");
	   fsSyntaxMemo1->Lines->Add("bool KZB = false;");
	   fsSyntaxMemo1->Lines->Add("bool KZC = false;");
	   fsSyntaxMemo1->Lines->Add("// Состояние фазы (true - включено)");
	   fsSyntaxMemo1->Lines->Add("bool StatA = false;");
	   fsSyntaxMemo1->Lines->Add("bool StatB = false;");
	   fsSyntaxMemo1->Lines->Add("bool StatC = false;");
	   fsSyntaxMemo1->Lines->Add("// Тумблер (false - включено)");
	   fsSyntaxMemo1->Lines->Add("bool Switch = true;");
	   fsSyntaxMemo1->Lines->Add("// Режим упрвления (true - диспетчерсок, false - автоматическое)");
	   fsSyntaxMemo1->Lines->Add("bool Contrl = true;");
	   fsSyntaxMemo1->Lines->Add("// Дверь (false - дверь закрыта)");
	   fsSyntaxMemo1->Lines->Add("bool Door = false;");
	   fsSyntaxMemo1->Lines->Add("// Охрана (false - под охраной)");
	   fsSyntaxMemo1->Lines->Add("bool Guard = false;");
	   fsSyntaxMemo1->Lines->Add("// Наличие блока БКС-12");
	   fsSyntaxMemo1->Lines->Add("bool BKS = false;");
	   fsSyntaxMemo1->Lines->Add("// Наличие напряжения на входах БКС-12");
	   fsSyntaxMemo1->Lines->Add("bool BKS01 = false;");
	   fsSyntaxMemo1->Lines->Add("bool BKS02 = false;");
	   fsSyntaxMemo1->Lines->Add("bool BKS03 = false;");
	   fsSyntaxMemo1->Lines->Add("bool BKS04 = false;");
	   fsSyntaxMemo1->Lines->Add("bool BKS05 = false;");
	   fsSyntaxMemo1->Lines->Add("bool BKS06 = false;");
	   fsSyntaxMemo1->Lines->Add("bool BKS07 = false;");
	   fsSyntaxMemo1->Lines->Add("bool BKS08 = false;");
	   fsSyntaxMemo1->Lines->Add("bool BKS09 = false;");
	   fsSyntaxMemo1->Lines->Add("bool BKS10 = false;");
	   fsSyntaxMemo1->Lines->Add("bool BKS11 = false;");
	   fsSyntaxMemo1->Lines->Add("bool BKS12 = false;");
	   fsSyntaxMemo1->Lines->Add("bool BKS13 = false;");
	   fsSyntaxMemo1->Lines->Add("bool BKS14 = false;");
	   fsSyntaxMemo1->Lines->Add("bool BKS15 = false;");
	   fsSyntaxMemo1->Lines->Add("bool BKS16 = false;");
	   fsSyntaxMemo1->Lines->Add("// Текущий режим по расписанию (1-день,2-ночь,3-вечер,4-утро)");
	   fsSyntaxMemo1->Lines->Add("int Mode   = 1;");
	   fsSyntaxMemo1->Lines->Add("// Активность фазы в ночном режиме (из XML)");
	   fsSyntaxMemo1->Lines->Add("bool PhaseA = true;");
	   fsSyntaxMemo1->Lines->Add("bool PhaseB = true;");
	   fsSyntaxMemo1->Lines->Add("bool PhaseC = true;");
	   fsSyntaxMemo1->Lines->Add("// Название тревоги");
	   fsSyntaxMemo1->Lines->Add("String Name = \""+AlarmSprav->Name+"\";");
	   fsSyntaxMemo1->Lines->Add("// Счетчик событий с первого Event=true");
	   fsSyntaxMemo1->Lines->Add("int Count   = 0;");
	   fsSyntaxMemo1->Lines->Add("// Счетчик минут с первого Event=true");
	   fsSyntaxMemo1->Lines->Add("int TimeOut = 0;");

	   fsSyntaxMemo1->Lines->Add("/**");
	   fsSyntaxMemo1->Lines->Add("* Переменные, которые возвращаются сервису");
	   fsSyntaxMemo1->Lines->Add("*/");
	   fsSyntaxMemo1->Lines->Add("");
	   fsSyntaxMemo1->Lines->Add("// Наличие аварии");
	   fsSyntaxMemo1->Lines->Add("bool Alarm = false;");
	   fsSyntaxMemo1->Lines->Add("// Возникновение события");
	   fsSyntaxMemo1->Lines->Add("bool Event = false;");
	   fsSyntaxMemo1->Lines->Add("String  AlarmType = \""+AlarmSprav->Type+"\";");

	   //	   fsSyntaxMemo1->Lines->Add("// Состояние (0-никакое,1-откл,2-");
//	   fsSyntaxMemo1->Lines->Add("int Stat = 0;");


   } catch (Exception &e) {
	  Log("Ошибка инициализации параметров "+e.Message);
   }

}

void __fastcall TAlarmEditForm1::Save(){
   try {
	  if( AlarmSprav->IdObject < 0 )return;
	  if( AlarmSprav->Script == fsSyntaxMemo2->Lines->Text )return;
	  int n = MessageDlg("Сохранить алгоритм в базу",
               mtConfirmation	, TMsgDlgButtons() << mbYes <<mbNo, 0);
      if( n != mrYes )    return;

	  Global1->Query->Close();
	  Global1->Query->SQL->Clear();
	  Global1->Query->SQL->Add("UPDATE alarm_sprav SET v_alarm_sprav_algoritm=:a0 WHERE id_object = :a1");
	  Global1->Query->Params->Items[0]->AsString = fsSyntaxMemo2->Lines->Text;
	  Global1->Query->Params->Items[1]->AsInteger = AlarmSprav->IdObject;
	  Global1->Query->ExecSQL();

   } catch (Exception &e) {
	  Log("Ошибка изаписи БД "+e.Message);
   }
}



void __fastcall TAlarmEditForm1::FormActivate(TObject *Sender)
{
   InitSem();

   InitParams();
}
//---------------------------------------------------------------------------

void __fastcall TAlarmEditForm1::ToolButton1Click(TObject *Sender)
{


	fsScript1->Clear();
	fsScript1->Lines->Clear();
    fsScript1->AddRTTI();
//	fsScript1->Lines->Add("{");
//	fsScript1->Lines->Add("ShowMessage(\"ok\");");
//	fsScript1->Lines->Add("}");

   fsScript1->Lines->Add("{");
   for( int i=0; i< fsSyntaxMemo1->Lines->Count; i++ ){
	  fsScript1->Lines->Add(fsSyntaxMemo1->Lines->Strings[i]);
   }
   for( int i=0; i< fsSyntaxMemo2->Lines->Count; i++ ){
	  fsScript1->Lines->Add(fsSyntaxMemo2->Lines->Strings[i]);
   }
   fsScript1->Lines->Add("}");

//   fsSyntaxMemo3->Lines->Text = fsScript1->Lines->Text;
   fsSyntaxMemo1->SetActiveLine(-1);
   fsSyntaxMemo2->SetActiveLine(-1);
   TDateTime t1,t2;
//   MilliSecondsBetween
   t1 = t1.CurrentDateTime();
   if(  !fsScript1->Run() )
   {
	   Log(fsScript1->ErrorMsg);
	   TPoint p = fsPosToPoint(fsScript1->ErrorPos);
//	   Log("x="+IntToStr((int)p.x)+",y="+IntToStr((int)p.y));
	   if( p.y > fsSyntaxMemo1->Lines->Count ){
		  fsSyntaxMemo2->SetActiveLine(p.y-fsSyntaxMemo1->Lines->Count-3);
	   }
	   else {
		  fsSyntaxMemo1->SetActiveLine(p.y-3);
	   }
   }
   else {
	  int ms = MilliSecondsBetween(t1.CurrentDateTime(),t1);
	  Log("");
	  Log("Время выполнения "+IntToStr(ms)+" мс");
	  UnicodeString name = fsScript1->Variables["Name"];
	  Variant event = fsScript1->Variables["Event"];
	  Variant type1 = fsScript1->Variables["AlarmType"];

	  if( event == 0 )Log("Event = false;");
	  else Log("Event = true;");
	  Variant alarm = fsScript1->Variables["Alarm"];
	  if( alarm == 0 )Log("Alarm = false;");
	  else {
		 Log("Alarm = true;");
		 Log("Name = \""+name+"\"");
	  }
	  Log("AlarmType = \""+type1+"\"");

   }
}
//---------------------------------------------------------------------------

void __fastcall TAlarmEditForm1::FormClose(TObject *Sender, TCloseAction &Action)

{
   Save();
}
//---------------------------------------------------------------------------

