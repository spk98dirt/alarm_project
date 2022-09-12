//---------------------------------------------------------------------------

#ifndef ChartForm1H
#define ChartForm1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.DateUtils.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "BaseGrid.hpp"
#include <Vcl.Grids.hpp>
#include <Vcl.Buttons.hpp>
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "Uni.hpp"
#include <Data.DB.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ImgList.hpp>
#include "..\Class\SemClass.h"
#include "..\Class\Funcs.h"
#include <VCLTee.BubbleCh.hpp>
#include "AdvSplitter.hpp"
#include "AdvOfficeButtons.hpp"
#include "..\Class\AsdClass3.h"
#include <Vcl.CheckLst.hpp>
#include "AdvDateTimePicker.hpp"
#include <VCLTee.GanttCh.hpp>
#include "AdvCombo.hpp"
#include "htmcombo.hpp"
#include "clisted.hpp"
#include "AdvUtil.hpp"
#include <System.ImageList.hpp>
#include <VclTee.TeeGDIPlus.hpp>
//#include "KvitForm.h"
//#include "MapForm1.h"
//---------------------------------------------------------------------------
class TFormTok1 : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TChart *Chart1;
	TAdvStringGrid *AdvStringGrid1;
	TUniQuery *UniQuery1;
	TPanel *Panel1;
	TImageList *ImageList1;
	TUniQuery *UniQuery2;
	TUniQuery *UniQuery3;
	TPanel *Panel3;
	TAdvSplitter *AdvSplitter1;
	TAdvSplitter *AdvSplitter2;
	TAdvStringGrid *AdvGrid2;
	TTimer *Timer1;
	TGanttSeries *Series1;
	TPanel *Panel5;
	TPanel *Panel6;
	TPanel *Panel7;
	TLabel *LabelAlarm;
	TAdvStringGrid *Grid3;
	TAdvStringGrid *AdvStringGrid2;
	TAdvSplitter *AdvSplitter3;
	TPanel *Panel8;
	TPanel *Panel9;
	TLabel *Label1;
	TLabel *Label2;
	TCheckBox *CheckBox1;
	TSpeedButton *SpeedButton1;
	TCheckBox *CheckBox2;
	TCheckBox *CheckBox3;
	TCheckBox *CheckBox4;
	TDateTimePicker *DateTimePicker1;
	TDateTimePicker *DateTimePicker2;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CheckIAClick(TObject *Sender);
	void __fastcall AdvGrid2ClickCell(TObject *Sender, int ARow, int ACol);
	void __fastcall AdvSplitter1Click(TObject *Sender);
	void __fastcall Panel1CanResize(TObject *Sender, int &NewWidth, int &NewHeight,
          bool &Resize);
	void __fastcall AdvGrid2CustomCellDraw(TObject *Sender, TCanvas *Canvas, int ACol,
          int ARow, TGridDrawState AState, TRect &ARect, bool Printing);
	void __fastcall CheckBoxClick(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall ToolButton3Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall AdvStringGrid2ClickCell(TObject *Sender, int ARow, int ACol);
	void __fastcall AdvStringGrid2SelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall AdvStringGrid2DblClickCell(TObject *Sender, int ARow, int ACol);
	void __fastcall Grid3GetEditorType(TObject *Sender, int ACol, int ARow, TEditorType &AEditor);
	void __fastcall Grid3ControlClick(TObject *Sender, int ARow, int ACol, UnicodeString CtrlID,
          UnicodeString CtrlType, UnicodeString CtrlVal);
	void __fastcall AdvGrid2KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall AdvSplitter2Click(TObject *Sender);
	void __fastcall Grid3ControlComboList(TObject *Sender, int ARow, int ACol, UnicodeString CtrlID,
          UnicodeString CtrlType, UnicodeString CtrlVal, TStringList *Values,
          bool &Edit, int &DropCount);
	void __fastcall Grid3ControlComboSelect(TObject *Sender, int ARow, int ACol, int ItemIndex,
          UnicodeString CtrlID, UnicodeString CtrlValue);
	void __fastcall SpeedButton1Click(TObject *Sender);
private:	// User declarations
		TColor clIa,clIb,clIc,clSv,clPhase44,clAl,clDay,clNight,clEvening,clUa,clUb,clUc;
//		TDateTime d1,d2;
		int alarmcom;
public:		// User declarations
	__fastcall TFormTok1(TComponent* Owner);
	void __fastcall ClickAlarm();
	void __fastcall ShowAlarm();
	void __fastcall Init1();
  //		void __fastcall Button1Click(TObject *Sender);
		bool FlagOnce;
		UnicodeString PP,dat1,dat2,vise;
    int RGBtoColor(short r,short b,short g);
	void __fastcall FShow();
	void __fastcall AdvGrid2Color();
	void __fastcall SelectCell(int ACol, int ARow);
	int WAl;
	int IdA;
	TKGrafIU *Grafik;

	TKController *cntrg;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTok1 *FormTok1;
//---------------------------------------------------------------------------
#endif
