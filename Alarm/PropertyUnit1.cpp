//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PropertyUnit1.h"
#include "AlarmUnit1.h"
#include "Connect.h"
#include "SemClass.h"
#include "AppHelper.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvDateTimePicker"
#pragma resource "*.dfm"
extern TForm1 *Form1;
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm2::BitBtn1Click(TObject *Sender)
{
   ReturnFlag = true;
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::btnCancelClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormShow(TObject *Sender)
{
   TDateTime d1;
   ReturnFlag = false;
   Global1 = Form1->Global;
   d1 = d1.CurrentDateTime();
   TDateTime t1 = TimeOf(d1);
   TDateTime t2 = TDateTime(8,0,0,0);
   if( t1 < t2 )AdvDateTimePicker1->DateTime = DateOf( d1 ) + t2;
   else AdvDateTimePicker1->DateTime = IncDay(DateOf( d1 )) + t2;

//   AdvDateTimePicker1->DateTime = IncMonth(d1.CurrentDateTime(),3);
   AdvDateTimePicker1->Enabled  = false;
// Начальная инициализация контролов
   CheckBox1->Checked           = false;
   AdvDateTimePicker1->Enabled  = false;
   Memo1->Lines->Clear();
   ListBox1->ItemIndex          = -1;
// Считываем все атрибуты тревоги напрямую из базы

   Global1->Query->Close();
   Global1->Query->SQL->Clear();
   Global1->Query->SQL->Add("SELECT t.*,c.id_object cid,pp.v_pp,s.v_alarm_sprav_id");
   Global1->Query->SQL->Add("FROM j_trevogi t,j_alarm_type_comment c,j_srpav_pp pp,alarm_sprav s");
   Global1->Query->SQL->Add("WHERE t.id_object = :a1");
   Global1->Query->SQL->Add(" AND pp.id_object = t.o_pp");
   Global1->Query->SQL->Add(" AND t.o_alarm_sprav_id = s.id_object");
   Global1->Query->SQL->Add(" AND t.o_alarmcommenttype = c.id_object(+)");
//   Global1->Query->SQL->Add(" ORDER BY ");
   Global1->Query->Params->Items[0]->AsInteger = Id;
   Global1->Query->Open();
   if( !Global1->Query->Eof )
   {
	  int cid = Global1->Query->FieldByName("cid")->AsInteger;
	  for( int i=0; i<ListBox1->Items->Count; i++ )
      {
         if( cid == (int)ListBox1->Items->Objects[i] )
         {
            ListBox1->ItemIndex = i;
            break;
         }
      }
	  Label3->Caption = "Тревога на ПП: "+Global1->Query->FieldByName("v_pp")->AsString;
	  Label4->Caption = "Дата возникновения: "+Global1->Query->FieldByName("v_date_time_voz_trev")->AsDateTime.FormatString("dd.mm.yy hh:nn");
	  Label5->Caption = Global1->Query->FieldByName("v_text_trevogi")->AsString;
	  Memo1->Text  = Global1->Query->FieldByName("V_komment_dispetcher")->AsString;

	  IdSpravAlarm = Global1->Query->FieldByName("o_alarm_sprav_id")->AsInteger;
	  IdPP         = Global1->Query->FieldByName("o_pp")->AsInteger;
	  SpravAlarm   = Global1->Query->FieldByName("v_alarm_sprav_id")->AsString;
	  PP           = Global1->Query->FieldByName("v_pp")->AsString;
	  AnsiString s_c1 = Global1->Query->FieldByName("v_alarm_check")->AsString;
      if( s_c1 == "Да" )
      {
         ListBox1->Enabled = false;
         Memo1->Enabled    = false;
         Label1->Enabled   = false;
         BitBtn1->Caption  = "Отложить";
         BitBtn1->Enabled  = false;
	  }
      else
      {
         ListBox1->Enabled = true;
         Memo1->Enabled    = true;
		 Label1->Enabled   = true;
		 BitBtn1->Caption  = "Квитировать";
	  }

   }
   if(Form1->cmdApplication == "" )
   {
      btnRequest->Visible = false;
   }
   else
   {
      btnRequest->Visible = true;
   }
   Global1->Query->Close();
   }

//---------------------------------------------------------------------------

void __fastcall TForm2::FormClose(TObject *Sender, TCloseAction &Action)
{
   TDateTime dt;
   dt = dt.CurrentDateTime();

// Сохраняем квитирование в базу

   if( ReturnFlag )
   {
	  Global1->Query->Close();
	  Global1->Query->SQL->Clear();

	  Global1->Query->SQL->Add("UPDATE j_trevogi SET o_alarmcommenttype = :a1");
	  Global1->Query->SQL->Add(",v_komment_dispetcher = :a2");
	  Global1->Query->SQL->Add(",v_alarm_check = :a3");
	  Global1->Query->SQL->Add(",v_date_time_kvit_trev = :a4");
	  Global1->Query->SQL->Add(",v_alarm_username = :a5");
	  Global1->Query->SQL->Add(",last_editing = :a6");
	  Global1->Query->SQL->Add(" WHERE id_object = :a7");
      if( ListBox1->ItemIndex >= 0 )
		 Global1->Query->Params->Items[0]->AsInteger = (int)ListBox1->Items->Objects[ListBox1->ItemIndex];
      else
		 Global1->Query->Params->Items[0]->AsString = "";
	  Global1->Query->Params->Items[1]->AsString    = Memo1->Text;
	  Global1->Query->Params->Items[2]->AsString    = "Да";
	  Global1->Query->Params->Items[3]->AsDateTime  = dt;
	  Global1->Query->Params->Items[4]->AsString    = apphelper->LoginInfo->Name; //ConnectForm10->ComboLog->Text;//  Global1->User->Login;
	  Global1->Query->Params->Items[5]->AsString    =
		 dt.FormatString("dd.mm.yyyy hh:nn") + " " + getenv("USERNAME") + "@" + getenv("USERDOMAIN");
	  Global1->Query->Params->Items[6]->AsInteger   = Id;
	  Global1->Query->ExecSQL();
// Если стоит галка - добавмить в журнал исключений
	  if( CheckBox1->Checked )
	  {
		 TDateTime dtt    = dtt.CurrentDateTime();

//Генерим номер строки и код ее объекта
	int numLine     = ((TKGlobalSem *)Global1)->AutoNum(GEN_LINES);
	int orderLine   = ((TKGlobalSem *)Global1)->AutoNum(GEN_LINE_NUMBER);
//Для новой строки новый ID_OBJECT
	int numObject   = ((TKGlobalSem *)Global1)->AutoNum(GEN_OBJECTS);
	Global1->Query->Close();
	Global1->Query->SQL->Clear();
	Global1->Query->SQL->Add("INSERT INTO j_Alarm_Exception (");
//Строка [1] Сюда пишем поля
	Global1->Query->SQL->Add("ID_LINE,DT_LINE,ORDER_LINE,ID_OBJECT,STATUS_LINE,ID_USER,LAST_EDITING,");
	Global1->Query->SQL->Add("o_Alarm_sprav_id,o_pp,v_AlarmException_DateEnd,v_AlarmException_UserName,v_AlarmException_DateSet");
	Global1->Query->SQL->Add(") VALUES(");
//Строка [3] Сюда пишем значения
	Global1->Query->SQL->Add(":a0,:a1,:a2,:a3,:a4,:a5,:a6,:a7,:a8,:a9,:a10,:a11");
	Global1->Query->SQL->Add(")");
	Global1->Query->Params->Items[0]->AsInteger  = numLine;
	Global1->Query->Params->Items[1]->AsDateTime = dtt;
	Global1->Query->Params->Items[2]->AsInteger  = orderLine;
	Global1->Query->Params->Items[3]->AsInteger  = numObject;
	Global1->Query->Params->Items[4]->AsInteger  = 0;
	Global1->Query->Params->Items[5]->AsInteger  = 0;
	Global1->Query->Params->Items[6]->AsString   = dtt.FormatString("dd.mm.yyyy hh:nn") + " " + getenv("USERNAME") + "@" + getenv("USERDOMAIN");
	Global1->Query->Params->Items[7]->AsInteger  = IdSpravAlarm;
	Global1->Query->Params->Items[8]->AsInteger  = IdPP;
	Global1->Query->Params->Items[9]->AsDateTime = AdvDateTimePicker1->DateTime;
	Global1->Query->Params->Items[10]->AsAnsiString = apphelper->LoginInfo->Name; // ConnectForm10->ComboLog->Text;
	Global1->Query->Params->Items[11]->AsDateTime = dt;
	Global1->Query->ExecSQL();
	  }
   }
}

//---------------------------------------------------------------------------

void __fastcall TForm2::FormActivate(TObject *Sender)
{
   ListBox1->Items->Clear();
   Global1->Query->Close();
   Global1->Query->SQL->Clear();
   Global1->Query->SQL->Add("SELECT * FROM j_alarm_type_comment ORDER BY v_alarmcommenttype");
   Global1->Query->Open();
   while( !Global1->Query->Eof )
   {
	  ListBox1->Items->Add(Global1->Query->FieldByName("v_alarmcommenttype")->AsString);
      ListBox1->Items->Objects[ListBox1->Items->Count-1] =
		 (TObject *)Global1->Query->FieldByName("id_object")->AsInteger;
	  Global1->Query->Next();
   }
// Формируем размер LISTBOX в зависимости от числа строк
Font->Size  		= Form1->GridFontSize;
//Font->Name 			= Global1->IniFile->ReadString("AsdPpProperty","FontName","Arial");

BitBtn1->Font->Size = Font->Size;
BitBtn1->Font->Name = Font->Name;
btnCancel->Font->Size = Font->Size;
btnCancel->Font->Name = Font->Name;
btnRequest->Font->Size = Font->Size;
btnRequest->Font->Name = Font->Name;
AdvDateTimePicker1->Font->Size = Font->Size - 1;


CheckBox1->Width    = Form2->Canvas->TextWidth(CheckBox1->Caption)+ 20;
AdvDateTimePicker1->Left  = CheckBox1->Left + CheckBox1->Width + 8;
AdvDateTimePicker1->Width = Form2->Canvas->TextWidth("99.99.9999 99:99:99") + 40;
Form2->ClientWidth        = AdvDateTimePicker1->Left + AdvDateTimePicker1->Width;

BitBtn1->Width   	      = Form2->Canvas->TextWidth(BitBtn1->Caption)+ 12;
btnCancel->Width   	      = Form2->Canvas->TextWidth(btnCancel->Caption)+ 12;
btnRequest->Width   	      = Form2->Canvas->TextWidth(btnRequest->Caption)+ 12;
btnCancel->Left    	      = Form2->ClientWidth - btnCancel->Width;
BitBtn1->Left    	      = btnCancel->Left - BitBtn1->Width - 10;
btnRequest->Left             = 0;


ListBox1->Height    = ListBox1->Count*(ListBox1->Font->Height*(-1.3));
ListBox1->Width     = Form2->ClientWidth;
Memo1->Width        = Form2->ClientWidth;
BitBtn1->Height     = BitBtn1->Font->Size*2;
btnRequest->Height  	= BitBtn1->Font->Size*2;
btnCancel->Height  	= BitBtn1->Font->Size*2;
Label3->Top     	= 8;
Label4->Top	    	= Label3->Top + Label3->Height + 8;
Label5->Top     	= Label4->Top + Label4->Height + 8;
Label2->Top     	= Label5->Top + Label5->Height + 8;
ListBox1->Top   	= Label2->Top + Label2->Height + 8;
Label1->Top     	= ListBox1->Top + ListBox1->Height + 8;
Memo1->Top      	= Label1->Top + Label1->Height + 8;
CheckBox1->Top  	= Memo1->Top + Memo1->Height + 8;
AdvDateTimePicker1->Top = Memo1->Top + Memo1->Height + 8;
BitBtn1->Top    	= AdvDateTimePicker1->Top + AdvDateTimePicker1->Height + 8;
btnCancel->Top    	= AdvDateTimePicker1->Top + AdvDateTimePicker1->Height + 8;
btnRequest->Top    	= AdvDateTimePicker1->Top + AdvDateTimePicker1->Height + 8;
Form2->ClientHeight = btnRequest->Top + btnRequest->Height;
Form2->Left     	= Form1->Left+Form1->Width/2-Form2->Width/2;
Form2->Top      	= Form1->Top+20;



}
//---------------------------------------------------------------------------

void __fastcall TForm2::CheckBox1Click(TObject *Sender)
{
    if( CheckBox1->Checked )
    {
        AdvDateTimePicker1->Enabled = true;
        BitBtn1->Enabled            = true;
    }
    else
    {
        AdvDateTimePicker1->Enabled = false;
        BitBtn1->Enabled            = ListBox1->Enabled;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm2::ListBox1KeyPress(TObject *Sender, char &Key)
{
    if(Key == 27)
        btnCancel->Click();
}
//---------------------------------------------------------------------------

void __fastcall TForm2::btnRequestClick(TObject *Sender)
{
    try
    {
	    UnicodeString FullName = Form1->cmdApplication;
        TStringList *l = new TStringList();
        try
        {
            l->Delimiter = ' ';
            l->DelimitedText = FullName;

            for( int i=0; i<l->Count; i++ )
            {
                if	( l->Strings[i] == "<user>" )		l->Strings[i] = apphelper->LoginInfo->Name;
                if	( l->Strings[i] == "<passwd>" )		l->Strings[i] = apphelper->LoginInfo->Password;
                if	( l->Strings[i] == "<tid>" )		l->Strings[i] = IntToStr(Id);
            }

            FullName = l->Text;
            if( l->Count > 0 )
            {
                UnicodeString CmdName  = ExtractFileName(l->Strings[0]);
                UnicodeString PathName = ExtractFilePath(l->Strings[0]);
                ShellExecute(NULL,L"open",CmdName.w_str(),FullName.w_str(),PathName.w_str(),SW_RESTORE);
            }
        }
        __finally
        {
            delete l;
        }
    }
    catch(...)
    {
        ShowMessage("Ошибка запуска скрипта формирования заявки");
    }
}
//---------------------------------------------------------------------------

