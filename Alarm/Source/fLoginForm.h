//---------------------------------------------------------------------------

#ifndef fLoginFormH
#define fLoginFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "AppHelper.h"
//---------------------------------------------------------------------------
class TLoginForm : public TForm
{
__published:	// IDE-managed Components
    TBevel *Bevel1;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *edtPassword;
    TButton *btnOk;
    TButton *btnCancel;
    TComboBox *cmbLogins;
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall edtPasswordChange(TObject *Sender);
    void __fastcall cmbLoginsKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall edtPasswordKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall FormShow(TObject *Sender);


private:	// User declarations
    TAppHelper* apphlpr;
public:		// User declarations
    __fastcall TLoginForm(TComponent* Owner, TAppHelper* apphelper);
};
//---------------------------------------------------------------------------
#endif
