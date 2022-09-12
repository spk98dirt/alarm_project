//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Connect.h"
#include "PropertyUnit1.h"
#include "AlarmUnit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "Uni"
#pragma link "OracleUniProvider"
#pragma link "UniProvider"
#pragma resource "*.dfm"
TConnectForm10 *ConnectForm10;
//---------------------------------------------------------------------------
__fastcall TConnectForm10::TConnectForm10(TComponent* Owner)
        : TForm(Owner)
{
   int a=1;
}
//---------------------------------------------------------------------------

/**
* Вывод формы с учетом конфигурационного файла
*/
void __fastcall TConnectForm10::FormShow(TObject *Sender)
{
   Font->Size = Form1->GridFontSize;
   Label1->Left = 8;
   Label1->Top = 16;
   ComboLog->Top = 16;
   Label2->Left = 8;
   EditPas->Top = ComboLog->Top*2 + ComboLog->Height;
   Label2->Top =  EditPas->Top ;
   ComboLog->Left = Label1->Width + 24;
   EditPas->Left = ComboLog->Left;
   Global1->Query->SQL->Clear();
   Global1->Query->SQL->Add("SELECT id_user, login, fio FROM Users ORDER BY login");

   isCancel = false;
   EditPas->Clear();
   ComboLog->Clear();
   for(Global1->Query->Open();!Global1->Query->Eof;Global1->Query->Next())
	  ComboLog->Items->Add( Global1->Query->FieldByName("login")->AsString);
   ComboLog->ItemIndex = 0;
   ComboLog->SetFocus();
   Global1->Query->Close();
   Global1->Query->SQL->Clear();
}

//---------------------------------------------------------------------------

/**
* Подтверждение регистрации
*/
void __fastcall TConnectForm10::ButOKClick(TObject *Sender)
{
  AnsiString s = ComboLog->Text;
//  ShowMessage(EditPas->Text);
  Global1->Query->SQL->Clear();
  Global1->Query->SQL->Add("SELECT id_user, login, fio, parol FROM Users ");
  Global1->Query->SQL->Add("WHERE login = :a1 and parol = :a2 ");
  Global1->Query->Params->Items[0]->AsString = ComboLog->Text;
  Global1->Query->Params->Items[1]->AsString = EditPas->Text;
  Global1->Query->Open();
  if (!Global1->Query->Eof)
  {
	Close();
  }
  else
  {
	ComboLog->Text = s;
	EditPas->Clear();
	if (!s.IsEmpty() )
		EditPas->SetFocus();
	else ComboLog->SetFocus();
  }
}

//---------------------------------------------------------------------------

/**
*
*/
void __fastcall TConnectForm10::ButCancelClick(TObject *Sender)
{
  isCancel = true;
  Close();
}

//---------------------------------------------------------------------------

/**
*
*/
void __fastcall TConnectForm10::EditPasKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key==13) ButOKClick(Sender); // Нажатие Enter
}

//---------------------------------------------------------------------------

/**
*
*/
void __fastcall TConnectForm10::EditLogKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key==13) ButOKClick(Sender); // Нажатие Enter
}
//---------------------------------------------------------------------------

