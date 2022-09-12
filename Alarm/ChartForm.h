//---------------------------------------------------------------------------

#ifndef ChartFormH
#define ChartFormH
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
//---------------------------------------------------------------------------
class TFormTok : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TSplitter *Splitter1;
	TStatusBar *StatusBar1;
	TPanel *Panel2;
	TChart *Chart1;
	TLineSeries *Series1;
	TLineSeries *Series2;
	TLineSeries *Series3;
	TAreaSeries *Series4;
	TAreaSeries *Series5;
	TAdvStringGrid *AdvStringGrid1;
	TSplitter *Splitter2;
	TLabel *Label1;
	TLabel *Label2;
	TSpeedButton *SpeedButton1;
	TSpeedButton *SpeedButton2;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall SpeedButton1Click(TObject *Sender);
	void __fastcall SpeedButton2Click(TObject *Sender);
private:	// User declarations
        TColor clPhase1,clPhase2,clPhase3,clDay,clNight,clEvening;

public:		// User declarations
	__fastcall TFormTok(TComponent* Owner);
		bool FlagOnce;
		AnsiString pp;
int RGBtoColor(short r,short b,short g);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormTok *FormTok;
//---------------------------------------------------------------------------
#endif
