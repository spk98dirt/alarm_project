//---------------------------------------------------------------------------

#ifndef AlarmUnit1H
#define AlarmUnit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

//#include "..\Form\Connect.h"
#include "..\Form\ChartFormAlarm.h"

#include "AsdClass3.h"
#include "SemClass.h"
#include "PropertyUnit1.h"

#include <inifiles.hpp>
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "Uni.hpp"
#include <Data.DB.hpp>
#include "Funcs.h"
#include "OracleUniProvider.hpp"
#include "UniProvider.hpp"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Soap.InvokeRegistry.hpp>
#include <Soap.Rio.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPClient.hpp>
#include <IdCoder.hpp>
#include <IdCoder3to4.hpp>
#include <IdCoderMIME.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.Buttons.hpp>
#include "AdvEdit.hpp"
#include "HTMListB.hpp"
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "BaseGrid.hpp"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ToolWin.hpp>
#include "AdvToolBtn.hpp"
#include <Vcl.ActnCtrls.hpp>
#include <Vcl.ActnMan.hpp>
#include <Vcl.ImgList.hpp>
#include "AdvUtil.hpp"
#include <System.ImageList.hpp>


enum TAlarmSound
{
   aAlarmNone   = 0,
   aAlarmYes    = 1,
   aAlarmNew    = 3
};

class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TIdUDPClient *ClientSocket1;
	TUniConnection *UniConnection1;
	TOracleUniProvider *OracleUniProvider1;
	TUniQuery *Sql2;
	TStatusBar *StatusBar1;
	TAdvStringGrid *AdvStringGrid1;
	TTimer *Timer1;
	TUniQuery *Sql1;
	TAdvEdit *AdvEdit1;
	TToolBar *ToolBar1;
	TSpeedButton *SpeedButton1;
	TSpeedButton *SpeedButton2;
	TToolButton *ToolButton1;
	TImageList *ImageList1;
	TProgressBar *ProgressBar1;
	TSpeedButton *SpeedButton3;
	TToolButton *ToolButton3;
	TSpeedButton *SpeedButton4;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall AdvStringGrid1RowChanging(TObject *Sender, int OldRow, int NewRow,
          bool &Allow);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall AdvStringGrid1DblClickCell(TObject *Sender, int ARow, int ACol);
	void __fastcall AdvStringGrid1ClickCell(TObject *Sender, int ARow, int ACol);
	void __fastcall SpeedButton2Click(TObject *Sender);
	void __fastcall AdvEdit1KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall SpeedButton3Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);


private:	// User declarations
    TList* FAlarmEvents;

    void __fastcall ReloadAlarmEvents(TDateTime act_dt);
    void __fastcall ClearAlarmEvents();
public:		// User declarations
	AnsiString ShowMapSecret;
	bool FlagOnce;
	bool FlagStayOnTop;
	bool FlagPauseTimer;
	bool FlagShowMap;
	bool FlagBeep;
	int  TimeOutPoll;
  	int  TimeOutBeep;
	int  TimerCount;
	int  GridRowHeight;
	int  GridFontSize;
	int  GridRow;
	int  GridTopRow;
	int  GridRowCount;
	int  CurrentTid;
	TAlarmSound AlarmSoundFlag;
	TDateTime RtPollDate;

	TDateTime __fastcall CheckImportRT();

	void __fastcall InitSem();
	void __fastcall InitJournals();
	void __fastcall LoadAlarmFromSem();
	void __fastcall InitControls();
	void __fastcall InitWidthColumns();
	void __fastcall PlayFile( AnsiString File );
	void __fastcall ShowOnMap(AnsiString pp);
	void __fastcall Exec(AnsiString IniCmd, AnsiString ErrorMsg );
	void __fastcall AlarmButtonClick(TObject *Sender);
	void __fastcall AddButton(int n);
	__fastcall TForm1(TComponent* Owner);
//	TIniFile *iniFile;
	bool IsAdmin;
	int Row;
	AnsiString cmdApplication;
	TKControllers *Controllers;
	TKGlobalSem *Global;
	UnicodeString nampp;    // наименование пп/пв
	int CheckCount;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------

#endif

