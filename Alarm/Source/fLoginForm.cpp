//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "fLoginForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TLoginForm::TLoginForm(TComponent* Owner, TAppHelper* apphelper)
    : TForm(Owner)
{
    apphlpr = apphelper;

    apphelper->DbHelper->GetLogins(cmbLogins->Items);
    if(!apphelper->LoginInfo->Name.IsEmpty())
    {
        int idx = cmbLogins->Items->IndexOf(apphelper->LoginInfo->Name);
        if(idx >= 0)
            cmbLogins->ItemIndex = idx;

        edtPassword->Text = apphelper->LoginInfo->Password;
    }
    else
        edtPassword->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TLoginForm::btnOkClick(TObject *Sender)
{
    String login = cmbLogins->Text;
    String psw = edtPassword->Text;
    if(!apphlpr->DbHelper->IsPswValid(login, psw))
    {
        String wrn = "Неправильное имя пользователя или пароль!";

        MessageBoxW(Handle, wrn.w_str(), L"Внимание:", MB_ICONWARNING);
        ModalResult = mrNone;
    }
    else
        ModalResult = mrOk;

    apphlpr->LoginInfo->Name = login;
    apphlpr->LoginInfo->Password = psw;
}
//---------------------------------------------------------------------------

void __fastcall TLoginForm::edtPasswordChange(TObject *Sender)
{
    btnOk->Enabled = !edtPassword->Text.IsEmpty();
}
//---------------------------------------------------------------------------

void __fastcall TLoginForm::cmbLoginsKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
    if (Key==13)
        btnOkClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TLoginForm::edtPasswordKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
    if (Key==13)
        btnOkClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TLoginForm::FormShow(TObject *Sender)
{
    if(cmbLogins->ItemIndex < 0)
        cmbLogins->SetFocus();
    else
        edtPassword->SetFocus();
}
//---------------------------------------------------------------------------

