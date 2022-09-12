//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ChartForm.h"
#include "AlarmUnit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "BaseGrid"
#pragma resource "*.dfm"
TFormTok *FormTok;
//---------------------------------------------------------------------------
__fastcall TFormTok::TFormTok(TComponent* Owner)
	: TForm(Owner)
{
   FlagOnce = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormTok::FormShow(TObject *Sender)
{
   TDateTime d_on1,d_on2,d_off1,d_off2;
   if(FlagOnce)return;
   FlagOnce = true;
   TDateTime d1,d2;
   d2 = d2.CurrentDateTime();
   d1 = IncDay(d2,-1);
 //  Form1->Controllers->GetShedule(Form1->Global1->Query,d1.CurrentDateTime(),&d_off1,&d_on2,&d_off2);
   Caption = "Просмотр токов ПП "+pp+" с "+d1.FormatString("dd.mm.yyyy")+" по "+d2.FormatString("dd.mm.yyyy");
   Label2->Caption = "ПП "+pp+" с "+d1.FormatString("dd.mm.yyyy hh:nn")+" по "+d2.FormatString("dd.mm.yyyy hh:nn");

   Chart1->Series[0]->Clear();
   Chart1->Series[1]->Clear();
   Chart1->Series[2]->Clear();
   Chart1->Series[3]->Clear();
   Chart1->Series[4]->Clear();
   AdvStringGrid1->RowCount  = 1;
   AdvStringGrid1->FixedRows = 0;
// Проверяем соединение к ораклу и если нет восстанавливаем
   if( !Form1->Global1->Connection->Connected )Form1->Global1->Connection->PerformConnect(true);
// Формируем SQL запрос к базе
   Form1->Global1->Query->Close();
   Form1->Global1->Query->SQL->Clear();
   Form1->Global1->Query->SQL->Add("SELECT * FROM pollservicearchive");
//   Form1->Global1->Query->SQL->Add("WHERE arh_pp=:a1 AND arh_dt >= :a2 ");
   Form1->Global1->Query->SQL->Add("WHERE arh_pp=:a1 AND arh_dt >= :a2 AND arh_dt < :a3");
   Form1->Global1->Query->SQL->Add("ORDER BY arh_dt");
   Form1->Global1->Query->Params->Items[0]->AsString = pp;
   Form1->Global1->Query->Params->Items[1]->AsDateTime = d1;
   Form1->Global1->Query->Params->Items[2]->AsDateTime = d2;
   double imax = 10;
   int i =0;
   for( i=1,Form1->Global1->Query->Open(); !Form1->Global1->Query->Eof;i++, Form1->Global1->Query->Next())
   {
	   TDateTime dt = Form1->Global1->Query->FieldByName("arh_dt")->AsDateTime;
	   double ia    = Form1->Global1->Query->FieldByName("arh_ia")->AsFloat;
	   double ib    = Form1->Global1->Query->FieldByName("arh_ib")->AsFloat;
	   double ic    = Form1->Global1->Query->FieldByName("arh_ic")->AsFloat;
	   double m     = Form1->Global1->Query->FieldByName("arh_mode")->AsFloat;

	   if( imax < ia )imax = ia;
	   if( imax < ib )imax = ib;
	   if( imax < ic )imax = ic;
	   AdvStringGrid1->RowCount++;
	   AdvStringGrid1->Cells[0][i] = dt.FormatString("dd.mm.yy hh:nn:ss");
	   AdvStringGrid1->Cells[1][i] = FloatToStr(ia);
	   AdvStringGrid1->Cells[2][i] = FloatToStr(ib);
	   AdvStringGrid1->Cells[3][i] = FloatToStr(ic);
	   AdvStringGrid1->CellProperties[1][i]->FontColor = clPhase1;
	   AdvStringGrid1->CellProperties[2][i]->FontColor = clPhase2;
	   AdvStringGrid1->CellProperties[3][i]->FontColor = clPhase3;


	   Chart1->Series[2]->AddXY(dt,ia,dt.FormatString("hh:NN"),clPhase1);
	   Chart1->Series[3]->AddXY(dt,ib,dt.FormatString("hh:NN"),clPhase2);
	   Chart1->Series[4]->AddXY(dt,ic,dt.FormatString("hh:NN"),clPhase3);

	   double x1    = -100;
	   double x2    = -100;
	   TColor cl    = (TColor)RGBtoColor(0xff,0xff,0x99);

	  /* if( Form1->ModeRT == mrtDay )
	   {
		   if(  dt < Form1->t11 || dt > Form1->t12 )x1 = 100;
	   }
	   else
	   {
		   if(  dt > Form1->t21 && dt < Form1->t22 )x1 = 100;
		   if(  dt > Form1->t31 && dt < Form1->t32 )x2 = 100;
	   }   */
	   x1 = 100;
	   x2 = 100;
	   if( x2 > 0 )
	   {
		  for( int j=0; j<AdvStringGrid1->ColCount; j++)
			 AdvStringGrid1->CellProperties[j][i]->BrushColor = (TColor)RGBtoColor(0xcc,0xcc,0x99);
	   }
	   else
	   {
		  if( x1 > 0 )
		  {
			 for( int j=0; j<AdvStringGrid1->ColCount; j++)
				AdvStringGrid1->CellProperties[j][i]->BrushColor = (TColor)RGBtoColor(0xff,0xff,0x99);
		  }
		  else
		  {
 			 for( int j=0; j<AdvStringGrid1->ColCount; j++)
				AdvStringGrid1->CellProperties[j][i]->BrushColor = clWhite;

          }
	   }


	   TAreaSeries *as = (TAreaSeries *)Chart1->Series[0];
	   as->AddXY(dt,x1,dt.FormatString("hh:NN"),(TColor)RGBtoColor(0xff,0xff,0x99));
	   as = (TAreaSeries *)Chart1->Series[1];
	   as->AddXY(dt,x2,dt.FormatString("hh:NN"),(TColor)RGBtoColor(0xcc,0xcc,0x99));
//	   i++;
   }


   Form1->Global1->Query->Close();
   Chart1->Axes->Left->Automatic = false;
   Chart1->Axes->Left->Maximum = imax;
   if( AdvStringGrid1->RowCount > 1 )AdvStringGrid1->FixedRows = 1;
   FlagOnce = false;


}
//---------------------------------------------------------------------------
void __fastcall TFormTok::FormCreate(TObject *Sender)
{
	AdvStringGrid1->ColWidths[0] = 120;
	AdvStringGrid1->ColWidths[1] = 40;
	AdvStringGrid1->ColWidths[2] = 40;
	AdvStringGrid1->ColWidths[3] = 40;
	AdvStringGrid1->Cells[0][0]  = "Время";
	AdvStringGrid1->Cells[1][0]  = "IA";
	AdvStringGrid1->Cells[2][0]  = "IB";
	AdvStringGrid1->Cells[3][0]  = "IC";
	AdvStringGrid1->ColCount     = 4;
	clDay                        = (TColor)RGBtoColor(0xff,0xff,0xff);
	clEvening                    = (TColor)RGBtoColor(0xff,0xff,0x99);
	clNight                      = (TColor)RGBtoColor(0xcc,0xcc,0x99);
	clPhase1                     = (TColor)RGBtoColor(0x00,0x00,0x66);
	clPhase2                     = (TColor)RGBtoColor(0x00,0x66,0x00);
	clPhase3                     = (TColor)RGBtoColor(0x66,0x00,0x00);
	Chart1->Series[2]->Color     = clPhase1;
	Chart1->Series[3]->Color     = clPhase2;
	Chart1->Series[4]->Color     = clPhase3;
	AdvStringGrid1->CellProperties[1][0]->FontColor = clPhase1;
	AdvStringGrid1->CellProperties[2][0]->FontColor = clPhase2;
	AdvStringGrid1->CellProperties[3][0]->FontColor = clPhase3;


}
//---------------------------------------------------------------------------

void __fastcall TFormTok::SpeedButton1Click(TObject *Sender)
{
   //Form1->CopyClipboard(Label2->Caption,Label1->Caption,AdvStringGrid1);
}
//---------------------------------------------------------------------------

void __fastcall TFormTok::SpeedButton2Click(TObject *Sender)
{
	TBitmap *bm          = new TBitmap();
	TCanvas *canv        = FormTok->Canvas;
	TRect rect           = canv->ClipRect;
	int w                = Chart1->Width;
	int h                = Panel2->Height;
	int l                = Panel2->Left;
	int t                = Panel2->Top;
	bm->Width            = w;
	bm->Height           = h;

	bm->Canvas->CopyMode = cmSrcCopy;
	canv->CopyMode       = cmSrcCopy;
	bm->Canvas->CopyRect(Rect(0,0,w,h),canv,Rect(l,t,l+w,t+h));
	Clipboard()->Assign( bm );
	delete bm;

}
//---------------------------------------------------------------------------
int TFormTok::RGBtoColor(short r,short b,short g)
{
   return (int)(g<<16|b<<8|r);
}
//---------------------------------------------------------------------------
