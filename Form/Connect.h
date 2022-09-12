//---------------------------------------------------------------------------

#ifndef ConnectH
#define ConnectH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "Uni.hpp"
#include <Data.DB.hpp>
#include "OracleUniProvider.hpp"
#include "UniProvider.hpp"
#include "ChartFormAlarm.h"

//---------------------------------------------------------------------------
class TConnectForm10 : public TForm
{
__published:	// IDE-managed Components
        TBevel *Bevel1;
        TLabel *Label1;
        TLabel *Label2;
		TEdit *EditPas;
        TButton *ButOK;
        TButton *ButCancel;
		TComboBox *ComboLog;
		void __fastcall FormShow(TObject *Sender);
        void __fastcall ButOKClick(TObject *Sender);
        void __fastcall ButCancelClick(TObject *Sender);
        void __fastcall EditPasKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall EditLogKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations

public:		// User declarations
		__fastcall TConnectForm10(TComponent* Owner);
		bool isCancel;
			TKGlobalSem *Global1;
};
//---------------------------------------------------------------------------
extern PACKAGE TConnectForm10 *ConnectForm10;
//---------------------------------------------------------------------------
#endif
