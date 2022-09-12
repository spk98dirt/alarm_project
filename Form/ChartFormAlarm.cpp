//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ChartFormAlarm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "BaseGrid"
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "Uni"
#pragma link "AdvSplitter"
#pragma link "AdvOfficeButtons"
#pragma link "AdvDateTimePicker"
#pragma link "AdvCombo"
#pragma link "htmcombo"
#pragma link "clisted"
#pragma link "AdvUtil"
#pragma resource "*.dfm"
#include <ComObj.hpp>

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
  try
  {
   if(FlagOnce)return;
   FlagOnce = true;
   Top  = Form1->Top;
   Left = Form1->Left;
   	FShow();
}
catch(...)
{
	Screen->Cursor = crDefault;
}
}
//---------------------------------------------------------------------------

void __fastcall TFormTok::SpeedButton1Click(TObject *Sender)
{
Grafik->GrafToExcel();
}
//---------------------------------------------------------------------------

int TFormTok::RGBtoColor(short r,short b,short g)
{
   return (int)(g<<16|b<<8|r);
}
//---------------------------------------------------------------------------
void __fastcall TFormTok::FShow()
{
try
{
cntrg = Form1->Controllers->FindByPP(punkt);
	  if (cntrg != NULL )
	  {
		  LabelPP->Caption = "  "+/*"№ " + FormMap1->nampp + ":" + */punkt;
		  LabelAD->Caption = cntrg->ppAddress;
	  }
Grafik->Alarm->Clear();
for (int i = 0; i < CheckListBox1->Items->Count; i++)
{
	if (CheckListBox1->Checked[i] )
	{
		Grafik->Alarm->Add(CheckListBox1->Items->Strings[i]);
	}
}

	Screen->Cursor = crAppStart;
	TryStrToDateTime(dat1,d1);
	TryStrToDateTime(dat2,d2);
	bool ck = CheckBox->Checked;
	/*double dn = d2 - d1;
	if ( dn >= 7.0 )
	{
	   ck = true;
	} */
	Grafik->GrafPaint(punkt,dat1,dat2,ck);
	Grafik->GrafPaintAlarm(punkt,dat1,dat2);
	Grafik->GrafPaintParam(punkt,dat1,dat2,ComboBox1->ItemIndex);
	Screen->Cursor = crDefault;

}
catch(...)
{
	Screen->Cursor = crDefault;
}
}
//---------------------------------------------------------------------------

void __fastcall TFormTok::FormActivate(TObject *Sender)
{
try
{
	Form1->Global->SetProperties("Graf",FormTok);
    //FormTok->WindowState = wsNormal;
    //FormTok->FormStyle = Vcl::Forms::fsNormal;

	LabelPP->Font->Size = Font->Size + 5;
	LabelAD->Font->Size = Font->Size + 5;
	clIa     	         = (TColor)RGBStringToColor(Form1->Controllers->ReadProperty("Graf","ChartIA","0,0,215"));
	clIb     	         = (TColor)RGBStringToColor(Form1->Controllers->ReadProperty("Graf","ChartIB","0,215,0"));
	clIc      	         = (TColor)RGBStringToColor(Form1->Controllers->ReadProperty("Graf","ChartIC","0,215,215"));
	clSv     	         = (TColor)RGBStringToColor(Form1->Controllers->ReadProperty("Graf","ChartSvjaz","0,0,0"));
	clAl     	         = (TColor)RGBStringToColor(Form1->Controllers->ReadProperty("Graf","ChartAlarm","215,0,0"));
	clUa        	     = (TColor)RGBStringToColor(Form1->Controllers->ReadProperty("Graf","ChartUA","30,100,30"));
	clUb       		     = (TColor)RGBStringToColor(Form1->Controllers->ReadProperty("Graf","ChartUB","30,170,130"));
	clUc	             = (TColor)RGBStringToColor(Form1->Controllers->ReadProperty("Graf","ChartUC","100,166,30"));
	CheckIA->Font->Color = clIa;
	CheckIB->Font->Color = clIb;
	CheckIC->Font->Color = clIc;
	CheckUa->Font->Color = clUa;
	CheckUb->Font->Color = clUb;
	CheckUc->Font->Color = clUc;
	CheckSV->Font->Color = clSv;
	CheckAL->Font->Color = clAl;

	TDateTime d_on1,d_on2,d_off1,d_off2;

	Caption = " " + Caption + " " + Form1->nampp;
	Label2->Caption = "    с  ";
	Label3->Caption = "    по  ";
	d1 = DateTimePicker1->Date;
	d2 = DateTimePicker2->Date;

	DateTimePicker1->Left = Label2->Left + Label2->Width + 5;
	Label3->Left = DateTimePicker1->Left + DateTimePicker1->Width + 5;
	DateTimePicker2->Left = Label3->Left + Label3->Width + 5;
	if( !Form1->Global->Connection->Connected) Form1->Global->Connection->PerformConnect(true);
	UniQuery1->Connection = Form1->Global->Query->Connection;
	UniQuery2->Connection = Form1->Global->Query->Connection;
	UniQuery3->Connection = Form1->Global->Query->Connection;
	AdvGrid2Color();

	CheckListBox1->Items->Clear();
	UniQuery1->Close();
	UniQuery1->SQL->Clear();
	UniQuery1->SQL->Add("SELECT DISTINCT V_ALARM_GROUP_EVENT ");
	UniQuery1->SQL->Add("FROM alarm_sprav ");
	UniQuery1->SQL->Add("WHERE V_ALARM_TYPE_EVENT = 'Тревога' AND V_ALARM_SPRAV_ONOFF = 'Да' Order by V_ALARM_GROUP_EVENT");
	for(UniQuery1->Open();!UniQuery1->Eof; UniQuery1->Next() )
	{
		 CheckListBox1->Items->Add(UniQuery1->FieldByName("V_ALARM_GROUP_EVENT")->AsString);
	}
	for(int i=0; i<CheckListBox1->Items->Count;i++ )
	{
		 CheckListBox1->Checked[i] = true;
	}	Panel6->Height = ClientHeight - 50;

	Grafik = new TKGrafIU(AdvStringGrid1,Chart1,Form1->Global);
	vise = "Graf_IA,Graf_IB,Graf_IC,Graf_SV,Graf_GR,Graf_TZN";
	int Vis = 0;
	  if (vise.Pos("Graf_IA") != 0)
	  {
		Vis = Graf_IA;
		CheckIA->Checked = true;
	  }
	  if (vise.Pos("Graf_IB") != 0)
	  {
		Vis = Vis | Graf_IB;
		CheckIB->Checked = true;
	  }
	  if (vise.Pos("Graf_IC") != 0)
	  {
		Vis = Vis | Graf_IC;
		CheckIC->Checked = true;
	  }
	  if (vise.Pos("Graf_UA") != 0)
	  {
		Vis = Vis | Graf_UA;
		CheckUa->Checked = true;
	  }
	  if (vise.Pos("Graf_UB") != 0)
	  {
		Vis = Vis | Graf_UB;
		CheckUb->Checked = true;
	  }
	  if (vise.Pos("Graf_UC") != 0)
	  {
		Vis = Vis | Graf_UC;
		CheckUc->Checked = true;
	  }
	  if (vise.Pos("Graf_SV") != 0)
	  {
		Vis = Vis | Graf_SV;
		CheckSV->Checked = true;
	  }
	  if (vise.Pos("Graf_AV") != 0)
	  {
		Vis = Vis | Graf_AV;
		CheckAL->Checked = true;
	  }
	  if (vise.Pos("Graf_GR") != 0)
	  {
		Vis = Vis | Graf_GR;
		CheckGr->Checked = true;
	  }
	  if (vise.Pos("Graf_TVO") != 0)
	  {
		Vis = Vis | Graf_TVO;
		CheckTVO->Checked = true;
	  }
	   if (vise.Pos("Graf_TZN") != 0)
	  {
		Vis = Vis | Graf_TZN;
		CheckTZN->Checked = true;
	  }
	dat1 = d1.DateString();
	dat2 = d2.DateString();
	Grafik->GrafPaint(punkt,dat1,dat2,CheckBox->Checked);
	Grafik->GrafVisible(Vis);
	TKControllers *Controllers;
	Controllers = new TKControllers(Form1->Global);
	FShow();
	if (DateTimePicker2->Date < IncMinute(Now(),-30))
	{
		ToolButton5->Enabled = false;
	}
	else
	{
		ToolButton5->Enabled = true;
	}

}
catch(...)
{
	Screen->Cursor = crDefault;
}
}
//---------------------------------------------------------------------------


void __fastcall TFormTok::CheckIAClick(TObject *Sender)
{
int Vis = 0;
if (CheckIA->Checked)
{
	Vis = Graf_IA;
}
if (CheckIB->Checked)
{
	Vis = Vis | Graf_IB;
}
if (CheckIC->Checked)
{
	Vis = Vis | Graf_IC;
}
if (CheckAL->Checked)
{
	Vis = Vis | Graf_AV;
}
if (CheckSV->Checked)
{
	Vis = Vis | Graf_SV;
}
if (CheckUa->Checked)
{
	Vis = Vis | Graf_UA;
}
if (CheckUb->Checked)
{
	Vis = Vis | Graf_UB;
}
if (CheckUc->Checked)
{
	Vis = Vis | Graf_UC;
}
if (CheckGr->Checked)
{
	Vis = Vis | Graf_GR;
}
if (CheckTVO->Checked)
{
	Vis = Vis | Graf_TVO;
}
if (CheckTZN->Checked)
{
	Vis = Vis | Graf_TZN;
}

if (CheckAa->Checked)
{
	Vis = Vis | Graf_AA;
}
if (CheckAb->Checked)
{
	Vis = Vis | Graf_AB;
}
if (CheckAc->Checked)
{
	Vis = Vis | Graf_AC;
}



Grafik->GrafVisible(Vis);
/*if (CheckIA->Checked)
{
   Series1->Visible = true;
   AdvStringGrid1->ColWidths[1] = 35;
}
else
{
   Series1->Visible = false;
   AdvStringGrid1->ColWidths[1] = 0;
}     */
}
//---------------------------------------------------------------------------







void __fastcall TFormTok::CheckBoxClick(TObject *Sender)
{
FShow();
}

//---------------------------------------------------------------------------

void __fastcall TFormTok::SelectCell(int ACol, int ARow)
{
}

//---------------------------------------------------------------------------


void __fastcall TFormTok::ToolButton5Click(TObject *Sender)
{
if (ToolButton5->ImageIndex == 3)
{

ToolButton5->ImageIndex = 4;
}
else
{

ToolButton5->ImageIndex = 3;
}
}

//---------------------------------------------------------------------------




void __fastcall TFormTok::CheckListBox1Click(TObject *Sender)
{
try
{
Grafik->Alarm->Clear();
for (int i = 0; i < CheckListBox1->Items->Count; i++)
{
	if (CheckListBox1->Checked[i] )
	{
		Grafik->Alarm->Add(CheckListBox1->Items->Strings[i]);
	}
}
	Screen->Cursor = crAppStart;
	TryStrToDateTime(dat1,d1);
	TryStrToDateTime(dat2,d2);
	Grafik->GrafPaintAlarm(punkt,dat1,dat2);
	Screen->Cursor = crDefault;
}
catch(...)
{
	Screen->Cursor = crDefault;
}

}

void __fastcall TFormTok::Grid3GetEditorType(TObject *Sender, int ACol, int ARow,
          TEditorType &AEditor)
{
//AEditor = edRichEdit;
}
//---------------------------------------------------------------------------


void __fastcall TFormTok::ComboBox1Click(TObject *Sender)
{
/*
1 		Межфазное замыкание ф. A
2 		Межфазное замыкание ф. B
3 		Межфазное замыкание ф. C
256 	КЗ ф. A
512 	КЗ ф. B
1024 	КЗ ф. C
16 		Перегрузка по току ф. A
32 		Перегрузка по току ф. B
64 		Перегрузка по току ф. C

128 	Охрана
16 		Тумблер
64		Дверь
*/

Screen->Cursor = crAppStart;
TryStrToDateTime(dat1,d1);
TryStrToDateTime(dat2,d2);
Grafik->GrafPaintParam(punkt,dat1,dat2,ComboBox1->ItemIndex);
Screen->Cursor = crDefault;

}
//---------------------------------------------------------------------------
void __fastcall TFormTok::AdvGrid2Color()
{

}
//---------------------------------------------------------------------------

void __fastcall TFormTok::AdvOfficeCheckBox1Click(TObject *Sender)
{
if (AdvOfficeCheckBox1->Checked)
{
	dat1 = DateTimePicker1->Date.DateString() + " 12:00:00";
	dat2 = DateTimePicker2->Date.DateString() + " 12:00:00";
}
else
{
	dat1 = DateTimePicker1->Date.DateString();
	dat2 = DateTimePicker2->Date.DateString();
}

if (DateTimePicker2->Date < IncMinute(Now(),-30))
{
	ToolButton5->Enabled = false;

}
else
{
	ToolButton5->Enabled = true;
}
FShow();
CheckIA->OnClick;
}

//---------------------------------------------------------------------------

void __fastcall TFormTok::FormResize(TObject *Sender)
{
   Panel6->Height = ClientHeight -  50;
   Panel9->Height = Panel2->Height	- ToolBar1->Height - Panel4->Height;
}

//---------------------------------------------------------------------------

void __fastcall TFormTok::AdvSplitter2Click(TObject *Sender)
{
try
{
if (Panel3->Visible)
{
	Panel3->Visible = false;
}
else
{
	AdvSplitter2->Visible = false;
	Panel3->Visible = true;
	Panel3->Width = 250;
	AdvSplitter2->Visible = true;
}
}
catch(...)
{

}
}

//---------------------------------------------------------------------------

void __fastcall TFormTok::ToolButton1Click(TObject *Sender)
{
Grafik->GrafToExcel();
}

//---------------------------------------------------------------------------

void __fastcall TFormTok::ToolButton2Click(TObject *Sender)
{
	TBitmap *bm          = new TBitmap();
	TCanvas *canv        = Canvas;
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

