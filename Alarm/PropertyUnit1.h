//---------------------------------------------------------------------------

#ifndef PropertyUnit1H
#define PropertyUnit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "AdvDateTimePicker.hpp"
#include <ComCtrls.hpp>
#include "AsdClass3.h"
#include "ChartForm.h"
#include "LogClass.h"

//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo1;
        TBitBtn *BitBtn1;
    TBitBtn *btnCancel;
        TCheckBox *CheckBox1;
        TAdvDateTimePicker *AdvDateTimePicker1;
        TListBox *ListBox1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
    TBitBtn *btnRequest;
        void __fastcall btnCancelClick(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall CheckBox1Click(TObject *Sender);
        void __fastcall ListBox1KeyPress(TObject *Sender, char &Key);
        void __fastcall btnRequestClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm2(TComponent* Owner);
        int  Row;
        bool ReturnFlag;
        int Id;
        int IdSpravAlarm,IdPP;
		AnsiString SpravAlarm,PP;
		TKGlobalSem *Global1;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
