//---------------------------------------------------------------------------

#ifndef AlarmEditUnit1H
#define AlarmEditUnit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "fs_synmemo.hpp"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include "fs_iinterpreter.hpp"
#include "DBAccess.hpp"
#include "OracleUniProvider.hpp"
#include "Uni.hpp"
#include "UniProvider.hpp"
#include <Data.DB.hpp>
#include <Vcl.ImgList.hpp>
#include "fs_icpp.hpp"


#include "SemClass.h"
#include "AsdClass3.h"
#include "DBAccess.hpp"
#include "OracleUniProvider.hpp"
#include "Uni.hpp"
#include "UniProvider.hpp"
#include <Data.DB.hpp>
//---------------------------------------------------------------------------
class TAlarmEditForm1 : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TPanel *Panel1;
	TSplitter *Splitter1;
	TPanel *Panel2;
	TfsSyntaxMemo *fsSyntaxMemo1;
	TfsSyntaxMemo *fsSyntaxMemo2;
	TLabel *Label2;
	TLabel *Label1;
	TLabel *Label3;
	TfsSyntaxMemo *fsSyntaxMemo3;
	TfsScript *fsScript1;
	TToolButton *ToolButton1;
	TUniConnection *UniConnection1;
	TOracleUniProvider *OracleUniProvider1;
	TImageList *ImageList1;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall ToolButton1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
    TKAlarmSprav *AlarmSprav;
public:		// User declarations
	TKGlobalSem *Global1;
	__fastcall TAlarmEditForm1(TComponent* Owner);
	void __fastcall InitParams();
	void __fastcall InitSem();
	void __fastcall Save();
};
//---------------------------------------------------------------------------
extern PACKAGE TAlarmEditForm1 *AlarmEditForm1;
//---------------------------------------------------------------------------
#endif
