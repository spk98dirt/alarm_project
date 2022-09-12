//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ChartForm1.h"
#include "MapForm1.h"

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

TFormTok1 *FormTok1;

bool FirstFlag = true;

void __fastcall TFormTok1::Init1()
{
if( !FirstFlag ){
//   FShow();
   return;
}
FirstFlag = false;

try
{
//	FormMap1->Global->SetProperties("Graf",FormTok1);
	FormMap1->Controllers->SetPropertyForm(FormTok1,"Graf","");
// Выстраивание контролов на верхней панели
	Panel8->Height               = Font->Size+50;

	Label1->Font->Size           = Font->Size;
	Label1->Font->Name           = Font->Name;
	Label1->Top                  = Panel8->Height - Label1->Height - 5 ;

	DateTimePicker1->Font->Size  = Font->Size;
	DateTimePicker1->Font->Name  = Font->Name;
	DateTimePicker1->Left        = Label1->Left + Label1->Width + 10;
	DateTimePicker1->Top         = Label1->Top;
	DateTimePicker1->Width       = Canvas->TextWidth("01.01.0001") + 30;

	Label2->Font->Size           = Font->Size;
	Label2->Font->Name           = Font->Name;
	Label2->Top                  = Label1->Top;
	Label2->Left                 = DateTimePicker1->Left + DateTimePicker1->Width + 10;

	DateTimePicker2->Font->Size  = Font->Size;
	DateTimePicker2->Font->Name  = Font->Name;
	DateTimePicker2->Left        = Label2->Left + Label2->Width + 10;
	DateTimePicker2->Top         = Label1->Top;
	DateTimePicker2->Width       = Canvas->TextWidth("01.01.0001") + 30;

	SpeedButton1->Left           = DateTimePicker2->Left + DateTimePicker2->Width + 10;
	SpeedButton1->Top            = Label1->Top;
	SpeedButton1->Caption        = ">>";
	SpeedButton1->Width          = DateTimePicker1->Height;
	SpeedButton1->Height         = SpeedButton1->Width;
	SpeedButton1->Font->Size     = Font->Size;

	CheckBox1->Font->Size        = Font->Size;
	CheckBox1->Font->Name        = Font->Name;
	CheckBox1->Left              = SpeedButton1->Left + SpeedButton1->Width + 50;
	CheckBox1->Top               = Label1->Top;
	CheckBox1->Width             = Canvas->TextWidth("A") + 25;

	CheckBox2->Font->Size        = Font->Size;
	CheckBox2->Font->Name        = Font->Name;
	CheckBox2->Left              = CheckBox1->Left + CheckBox1->Width + 20;
	CheckBox2->Top               = Label1->Top;
	CheckBox2->Width             = Canvas->TextWidth("A") + 25;

	CheckBox3->Font->Size        = Font->Size;
	CheckBox3->Font->Name        = Font->Name;
	CheckBox3->Left              = CheckBox2->Left + CheckBox2->Width + 20;
	CheckBox3->Top               = Label1->Top;
	CheckBox3->Width             = Canvas->TextWidth("A") + 25;

	CheckBox4->Font->Size        = Font->Size;
	CheckBox4->Font->Name        = Font->Name;
	CheckBox4->Left              = CheckBox3->Left + CheckBox3->Width + 50;
	CheckBox4->Top               = Label1->Top;
	CheckBox4->Width             = Canvas->TextWidth("U,В") + 30;




	Grid3->Font->Size    = Font->Size - 2;


	clIa     	         = (TColor)RGBStringToColor(FormMap1->Controllers->ReadProperty("Graf","ChartIA","0,0,215"));
	clIb     	         = (TColor)RGBStringToColor(FormMap1->Controllers->ReadProperty("Graf","ChartIB","0,215,0"));
	clIc      	         = (TColor)RGBStringToColor(FormMap1->Controllers->ReadProperty("Graf","ChartIC","0,215,215"));
	clSv     	         = (TColor)RGBStringToColor(FormMap1->Controllers->ReadProperty("Graf","ChartSvjaz","0,0,0"));
	clAl     	         = (TColor)RGBStringToColor(FormMap1->Controllers->ReadProperty("Graf","ChartAlarm","215,0,0"));
	clUa        	     = (TColor)RGBStringToColor(FormMap1->Controllers->ReadProperty("Graf","ChartUA","30,100,30"));
	clUb       		     = (TColor)RGBStringToColor(FormMap1->Controllers->ReadProperty("Graf","ChartUB","30,170,130"));
	clUc	             = (TColor)RGBStringToColor(FormMap1->Controllers->ReadProperty("Graf","ChartUC","100,166,30"));

	TDateTime d_on1,d_on2,d_off1,d_off2;

	Caption = " " + Caption + " " + FormMap1->nampp;

	if( !FormMap1->Global->Connection->Connected) FormMap1->Global->Connection->PerformConnect(true);
	UniQuery1->Connection = FormMap1->Global->Query->Connection;
	UniQuery2->Connection = FormMap1->Global->Query->Connection;
	UniQuery3->Connection = FormMap1->Global->Query->Connection;
	AdvGrid2Color();

	AdvStringGrid2->ColCount     = 6;
	AdvStringGrid2->FixedCols    = 0;
	AdvStringGrid2->FixedRows    = 1;
	AdvStringGrid2->ColWidths[0] = 250;
	AdvStringGrid2->ColWidths[1] = 130;
	AdvStringGrid2->ColWidths[2] = 0;//(AdvStringGrid2->Width-25)*0.05;//80;
	AdvStringGrid2->ColWidths[3] = 0;
	AdvStringGrid2->ColWidths[4] = 130;
	AdvStringGrid2->ColWidths[5] = 400;
	AdvStringGrid2->Cells[0][0]  = "Тревога";
	AdvStringGrid2->Cells[1][0]  = "Фиксация";
	AdvStringGrid2->Cells[2][0]  = "K";
	AdvStringGrid2->Cells[4][0]  = "Снятие";
	AdvStringGrid2->Cells[5][0]  = "Комментарий";

	Grid3->ColCount     = 2;
	Grid3->RowCount     = 6;
	Grid3->FixedCols    = 0;
	Grid3->FixedRows    = 1;
	Grid3->ColWidths[0] = Grid3->Width * 0.4;
	Grid3->ColWidths[1] = Grid3->Width * 0.55;
	Grid3->Cells[0][0]  = "Название параметра";
	Grid3->Cells[1][0]  = "Значение";
	Grid3->Cells[0][1]  = "Кто квитировал тревогу";
	Grid3->Cells[0][2]  = "Квитирование";
	Grid3->RowColor[2]  = (TColor)RGBStringToColor("240,240,240");
	Grid3->RowHeights[3] = 40;
	Grid3->AddScrollBar(1,3,true);
	Grid3->Cells[0][3]  = "Комментарий диспетчера";
	Grid3->Cells[0][4]  = "Снятие";
	Grid3->RowHeights[4] = 0;
	Grid3->RowColor[4]  = (TColor)RGBStringToColor("240,240,240");
	Grid3->RowHeights[5] = 30;
	Grid3->MergeCells(0,5,2,1);
	//Grid3->Cells[1][5]  =  "Квитирование";
	Grid3->Cells[0][5]  = "<CONTROL TYPE=\"BUTTON\" WIDTH=\"85\" VALUE=\"Квитировать\" ID=\"but\">    <CONTROL TYPE=\"BUTTON\" WIDTH=\"100\" VALUE=\"Квитировать все\" ID=\"butal\">";
	Panel6->Height = ClientHeight -  180;
	  int w = Width;
	  int h = Height;
	  int st = WindowState;

	Grafik = new TKGrafIU(AdvStringGrid1,Chart1,FormMap1->Global);
	vise = "Graf_IA,Graf_IB,Graf_IC,Graf_SV,Graf_GR,Graf_TZN";
	int Vis = 0;
	Vis = Graf_IA | Graf_IB | Graf_IC | Graf_SV | Graf_AV | Graf_GR | Graf_TVO | Graf_TZN ;
	dat1 = DateTimePicker1->DateTime.DateString();
	dat2 = DateTimePicker2->DateTime.DateString();


//	Grafik->GrafPaint(PP,dat1,dat2,false);
//	Grafik->GrafVisible(Vis);
//	TKControllers *Controllers = FormMap1->Controllers;
//	Timer1->Interval = StrToInt(Controllers->ReadProperty("Graf","ChartAuto","120000"));
//	FShow();

}
catch(...)
{
	Screen->Cursor = crDefault;
}
}

//---------------------------------------------------------------------------
__fastcall TFormTok1::TFormTok1(TComponent* Owner)
	: TForm(Owner)
{
   FlagOnce = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormTok1::FormShow(TObject *Sender)
{
  try
  {
//   if(FlagOnce)return;
//   FlagOnce = true;
   Top  = FormMap1->Y2;
   Left = FormMap1->X2;
//   Init1();
   FShow();
   // Находим в конфигурации координаты окна в процетах
	//FormMap1->Global->SetProperties("Graf",FormTok);
	//Font->Size = FormMap1->Global->IniFile->ReadInteger("AutoForm","FontSize", 12);
	//Font->Name = FormMap1->Controllers->ReadProperty("AutoForm","FontName","Arial");

}
catch(...)
{
	Screen->Cursor = crDefault;
}
}
//---------------------------------------------------------------------------


int TFormTok1::RGBtoColor(short r,short b,short g)
{
   return (int)(g<<16|b<<8|r);
}
//---------------------------------------------------------------------------

/**
* Выдаем график
*/
void __fastcall TFormTok1::FShow(){
   try {
	  FormMap1->ProgressShow("Строится график...",1);

	  cntrg = FormMap1->Controllers->FindByPP(PP);
	  if (cntrg == NULL )return;
	  Caption = "ПП "+PP+" ("+cntrg->ppAddress+")";
//	  ShowAlarm();
//	  Grafik->Alarm->Clear();
/*
for (int i = 0; i < CheckListBox1->Items->Count; i++)
{
	if (CheckListBox1->Checked[i] )
	{
		Grafik->Alarm->Add(CheckListBox1->Items->Strings[i]);
	}
}
*/

	  Screen->Cursor = crAppStart;
//	  TryStrToDateTime(dat1,DateTimePicker1->DateTime);
 //	  TryStrToDateTime(dat2,DateTimePicker1->DateTime);
//	  bool ck = CheckBox->Checked;
	/*double dn = d2 - d1;
	if ( dn >= 7.0 )
	{
	   ck = true;
	} */
	  dat1 = DateTimePicker1->Date.DateString() + " 12:00:00";
	  dat2 = DateTimePicker2->Date.DateString() + " 12:00:00";
	  CheckIAClick( FormTok1 );
	  Grafik->GrafPaint(PP,dat1,dat2,false);
//	  Grafik->GrafPaintAlarm(PP,dat1,dat2);
	  Grafik->GrafPaintParam(PP,dat1,dat2,false);
	  Screen->Cursor = crDefault;
	  FormMap1->ProgressClose();


}
catch(...)
{
	Timer1->Enabled = false;
	Screen->Cursor = crDefault;
}
}

//---------------------------------------------------------------------------


/*void __fastcall TFormTok::Chart1ClickSeries(TCustomChart *Sender, TChartSeries *Series,
		  int ValueIndex, TMouseButton Button, TShiftState Shift, int X,
		  int Y)
{
 try
 {
	AdvStringGrid1->ScrollInView(0,ValueIndex);
	TGridRect r;
	r.Left = AdvStringGrid1->FixedCols;
	r.Top = ValueIndex+1;
	r.Right = AdvStringGrid1->ColCount;
	r.Bottom = ValueIndex+1;
	AdvStringGrid1->Selection = r;
	Chart1->CancelMouse = false;
 }
 catch (...)
 {

 }
}  */
//---------------------------------------------------------------------------


void __fastcall TFormTok1::CheckIAClick(TObject *Sender){
   int	Vis = Graf_SV | Graf_AV | Graf_GR | Graf_TVO | Graf_TZN ;
   if( CheckBox1->Checked ){
	  Vis = Vis | Graf_IA;
	  if( CheckBox4->Checked )Vis = Vis | Graf_UA;
   }
   if( CheckBox2->Checked ){
	  Vis = Vis | Graf_IB;
	  if( CheckBox4->Checked )Vis = Vis | Graf_UB;
   }
   if( CheckBox3->Checked ){
	  Vis = Vis | Graf_IC;
	  if( CheckBox4->Checked )Vis = Vis | Graf_UC;
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


void __fastcall TFormTok1::AdvGrid2ClickCell(TObject *Sender, int ARow, int ACol)
{
SelectCell(ACol,ARow);
}
//---------------------------------------------------------------------------

void __fastcall TFormTok1::AdvSplitter1Click(TObject *Sender)
{
if (Panel1->Visible)
{
	Panel1->Visible = false;
}
else
{
	/*AdvSplitter1->Visible = false;
	AdvSplitter1->Visible = true;    */
	Panel1->Visible = true;
}
}

//---------------------------------------------------------------------------

void __fastcall TFormTok1::Panel1CanResize(TObject *Sender, int &NewWidth, int &NewHeight,
          bool &Resize)
{
	AdvGrid2->ColWidths[1] = AdvGrid2->Width - 25 - AdvGrid2->ColWidths[0];
}

//---------------------------------------------------------------------------


void __fastcall TFormTok1::AdvGrid2CustomCellDraw(TObject *Sender, TCanvas *Canvas,
          int ACol, int ARow, TGridDrawState AState, TRect &ARect, bool Printing)

{
try
 {
   if ((ARow >=0)&&(ACol == 0))
	{
		TColor cl = clWindow;
		InflateRect(ARect,-2,-2);
		Canvas->Pen->Color = cl;
		Canvas->Pen->Width = 3;
		Canvas->Brush->Color = cl;
		Canvas->Rectangle(ARect);
		cl = AdvGrid2->FontColors[ACol][ARow];
		//InflateRect(ARect,-5,-3);
		Canvas->Pen->Color = clGray;
		Canvas->Pen->Width = 2;
		Canvas->Brush->Color = cl;
		Canvas->Rectangle(ARect.Left+2,ARect.Top+2,ARect.Left-Font->Height+2,ARect.Top-Font->Height+2);
		TKController *pcnt = FormMap1->Controllers->FindByPP(AdvGrid2->Cells[ACol+1][ARow]);
		Canvas->Pen->Color = clWindow;
		Canvas->Brush->Color = clWindow;
		UnicodeString param = "";
		UnicodeString comm = "";
		if (pcnt != NULL)
		{
		   //	if ( pcnt->ArchiveValuesParam->Link)
			//{
				if( pcnt->ArchiveValuesParam->GetParamDiscret(PARAM_Guard) ==  FormMap1->b_guard)
				{	param = param + FormMap1->char_guard + " ";   comm = comm + FormMap1->char_guard + " "; }
				if( pcnt->ArchiveValuesParam->GetParamDiscret(PARAM_Switch) == FormMap1->b_switch )
				{	param = param + FormMap1->char_switch + " ";  comm = comm + FormMap1->char_switch + " ";}
				if( pcnt->ArchiveValuesParam->GetParamDiscret(PARAM_Contrl) == FormMap1->b_contrl)
				{	param = param + FormMap1->char_contrl + " ";  comm = comm + FormMap1->char_contrl + " ";}
		   //	}

			if ((pcnt->SwitchType.Pos("контактор") != 0) || (pcnt->SwitchType.Pos("Контактор") != 0))
			{
				comm = comm + "K";
				Canvas->Font->Color = clBlack;
				Canvas->Font->Style << fsBold;
				Canvas->Font->Size = Font->Size;
				Canvas->TextOutW(ARect.Left-Font->Height+3,ARect.Top,comm);
			}
			Canvas->Font->Color = clRed;
			Canvas->Font->Style << fsBold;
			Canvas->Font->Size = Font->Size;
			Canvas->TextOutW(ARect.Left-Font->Height+3,ARect.Top,param);
		}
	}
	}
	catch(...)
	{

	}
}

//---------------------------------------------------------------------------

void __fastcall TFormTok1::CheckBoxClick(TObject *Sender)
{
FShow();
}

//---------------------------------------------------------------------------

void __fastcall TFormTok1::SelectCell(int ACol, int ARow)
{
try
{
Timer1->Enabled = false;
PP = AdvGrid2->Cells[ACol][ARow];
FShow();

if (DateTimePicker2->Date < IncMinute(Now(),-30))	Timer1->Enabled = false;
else	Timer1->Enabled = true;
/*
if (FormMap1->Grid1->Visible)
{
	for (int i = 1; i <= FormMap1->Grid1->RowCount; i++)
	{
		if (FormMap1->Grid1->Cells[1][i] == punkt)
		{
			 FormMap1->Grid1->Row = i;
			 FormMap1->Grid1->Col = 1;
			 i = FormMap1->Grid1->RowCount + 1;
		}
	}
}
if (FormMap1->Grid2->Visible)
{
	for (int i = 0; i <= FormMap1->Grid2->RowCount; i++)
	{
	   for (int j = 0; j <= FormMap1->Grid2->ColCount; j++)
	   {
		if (FormMap1->Grid2->Cells[j][i] == punkt)
		{
			 FormMap1->Grid2->Row = i;
			 FormMap1->Grid2->Col = j;
			 i = FormMap1->Grid2->RowCount + 1;
			 j = FormMap1->Grid2->ColCount + 1;
		}
	   }
	}
}  */
}
catch(...)
{

}
}

//---------------------------------------------------------------------------



void __fastcall TFormTok1::Timer1Timer(TObject *Sender)
{
	int srow;
	srow = AdvStringGrid1->Row;
	dat1 = DateTimePicker1->Date.DateString();
	DateTimePicker2->DateTime = Now();
	dat2 = Now().DateString();// DateTimePicker2->Date.DateString();
	FShow();
	//if (srow != 1)
	//{
		AdvStringGrid1->Row = srow;
	//}
  //CheckIA->OnClick;
}
//---------------------------------------------------------------------------

void __fastcall TFormTok1::ToolButton3Click(TObject *Sender)
{
/*	dat1 = DateTimePicker1->Date.DateString();
	dat2 = DateTimePicker2->Date.DateString();
	if (DateTimePicker2->Date < IncMinute(Now(),-30))
		{
			ToolButton5->Enabled = false;
			Timer1->Enabled = false;
		}
	else
		{
			ToolButton5->Enabled = true;
			Timer1->Enabled = true;
		}

  FShow();
  CheckIA->OnClick;    */
}
//---------------------------------------------------------------------------

void __fastcall TFormTok1::FormClose(TObject *Sender, TCloseAction &Action)
{
Timer1->Enabled = false;
}
//---------------------------------------------------------------------------


void __fastcall TFormTok1::ClickAlarm()
{
try
{
   alarmcom = 0;
   Grid3->Cells[1][1] = "";
   Grid3->Cells[1][2] = "";
   Grid3->Cells[1][3] = "";
   Grid3->Cells[1][4] = "";
   FormMap1->UniQ2->Close();
   FormMap1->UniQ2->SQL->Clear();
   FormMap1->UniQ2->SQL->Add("SELECT t.*,c.id_object cid,c.V_ALARMCOMMENTTYPE,pp.v_pp,s.v_alarm_sprav_id");
   FormMap1->UniQ2->SQL->Add("FROM j_trevogi t,j_alarm_type_comment c,j_srpav_pp pp,alarm_sprav s");
   FormMap1->UniQ2->SQL->Add("WHERE t.id_object = :a1");
   FormMap1->UniQ2->SQL->Add(" AND pp.id_object = t.o_pp");
   FormMap1->UniQ2->SQL->Add(" AND t.o_alarm_sprav_id = s.id_object");
   FormMap1->UniQ2->SQL->Add(" AND t.o_alarmcommenttype = c.id_object(+)");
   FormMap1->UniQ2->Params->Items[0]->AsInteger = IdA;
   FormMap1->UniQ2->Open();
   if( !FormMap1->UniQ2->Eof )
   {
	   Grid3->Cells[1][1] = FormMap1->UniQ2->FieldByName("V_ALARM_USERNAME")->AsString;
	   if (FormMap1->UniQ2->FieldByName("v_date_time_kvit_trev")->AsString != "")
		{
			Grid3->Cells[1][2] = FormMap1->DateToString(FormMap1->UniQ2->FieldByName("v_date_time_kvit_trev")->AsDateTime);
			Grid3->Cells[1][3] = FormMap1->UniQ2->FieldByName("V_ALARMCOMMENTTYPE")->AsString;
			Grid3->RowHeights[5] = 0;
		}
	   else
		{
			Grid3->Cells[1][2] = "";
            Grid3->Cells[1][3] = "<CONTROL TYPE=\"COMBO\" WIDTH=\" "+ IntToStr(Grid3->ColWidths[1]-5)+"\" VALUE=\"\" ID=\"CO2\">";
			Grid3->RowHeights[5] = 30;
		}

	   if (FormMap1->UniQ2->FieldByName("v_date_time_sniat_trev")->AsString != "")
		Grid3->Cells[1][4] = FormMap1->DateToString(FormMap1->UniQ2->FieldByName("v_date_time_sniat_trev")->AsDateTime);
	   else
		Grid3->Cells[1][4] = "";
   }
   else
   {
	   Grid3->Cells[1][1] = "";
	   Grid3->RowHeights[5] = 0;
	   Grid3->Cells[1][2] = "";
	   Grid3->Cells[1][3] = "";
	   Grid3->Cells[1][4] = "";
   }

}
catch(...)
{
	Timer1->Enabled = false;
}
}
//---------------------------------------------------------------------------
void __fastcall TFormTok1::AdvStringGrid2ClickCell(TObject *Sender, int ARow, int ACol)

{
	IdA  =(int)AdvStringGrid2->Objects[0][ARow];
	ClickAlarm();
}
//---------------------------------------------------------------------------

void __fastcall TFormTok1::AdvStringGrid2SelectCell(TObject *Sender, int ACol, int ARow,
		  bool &CanSelect)
{
	IdA  =(int)AdvStringGrid2->Objects[0][ARow];
	ClickAlarm();
}
//---------------------------------------------------------------------------
 /*
* Отображение тревог
*/
void __fastcall TFormTok1::ShowAlarm()
{
   try
   {
   // Таблица с тревогами
   for( int i=1; i<AdvStringGrid2->RowCount; i++ )
		for(int j=0; j<AdvStringGrid2->ColCount; j++ )
		{
			AdvStringGrid2->Cells[j][i] = "";
			AdvStringGrid2->CellProperties[j][i]->BrushColor = clWhite;
		}
	  AdvStringGrid2->RowCount = 2;
	  FormMap1->UniQ->Close();
	  FormMap1->UniQ->SQL->Clear();

	  FormMap1->UniQ->SQL->Add("SELECT ( CASE WHEN t.v_alarm_check='Нет' THEN 1 WHEN t.v_alarm_check='Да' THEN 2 ELSE 1 END) AS kv,");
	  FormMap1->UniQ->SQL->Add("c.V_ALARMCOMMENTTYPE,t.id_object AS tid,pp.v_pp,s.v_alarm_sprav_name,t.* ");
	  FormMap1->UniQ->SQL->Add("FROM j_trevogi t,j_srpav_pp pp,alarm_sprav s,j_alarm_type_comment c ");
	  FormMap1->UniQ->SQL->Add(" WHERE pp.id_object = t.o_pp");
	  FormMap1->UniQ->SQL->Add(" AND t.o_alarm_sprav_id = s.id_object AND t.o_alarmcommenttype = c.id_object(+)");
	  FormMap1->UniQ->SQL->Add(" AND s.v_alarm_sprav_onoff = 'Да'");
	  FormMap1->UniQ->SQL->Add(" AND pp.v_pp = :a0");
	  FormMap1->UniQ->SQL->Add("AND ((t.V_DATE_TIME_VOZ_TREV>=:a1 AND t.V_DATE_TIME_SNIAT_TREV<=:a2) OR ");
	  FormMap1->UniQ->SQL->Add("(t.V_DATE_TIME_VOZ_TREV<=:a1 AND t.V_DATE_TIME_SNIAT_TREV>=:a2) OR");
	  FormMap1->UniQ->SQL->Add("(t.V_DATE_TIME_VOZ_TREV>=:a1 AND t.V_DATE_TIME_VOZ_TREV<=:a2 AND t.V_DATE_TIME_SNIAT_TREV>=:a2) OR");
	  FormMap1->UniQ->SQL->Add("(t.V_DATE_TIME_VOZ_TREV<=:a1 AND t.V_DATE_TIME_SNIAT_TREV>=:a1 AND t.V_DATE_TIME_SNIAT_TREV<=:a2) OR");
	  FormMap1->UniQ->SQL->Add("(t.V_DATE_TIME_VOZ_TREV<=:a2 AND t.V_DATE_TIME_SNIAT_TREV is NULL))");
	  FormMap1->UniQ->SQL->Add(" AND (t.v_tip_trevogi = 'Авария' OR t.v_tip_trevogi = 'Тревога')");
	  FormMap1->UniQ->SQL->Add(" ORDER BY kv,v_date_time_voz_trev DESC");
	  FormMap1->UniQ->Params->Items[0]->AsString = PP;
	  FormMap1->UniQ->Params->Items[1]->AsDateTime = DateTimePicker1->DateTime.FormatString("dd.mm.yy");
	  FormMap1->UniQ->Params->Items[2]->AsDateTime = (DateTimePicker2->DateTime+1).FormatString("dd.mm.yy");
	  int cnt1 = 0, cnt2 = 0;
	  UnicodeString bold = FormMap1->Controllers->ReadProperty("ALARM","BoldKvitir","0");
	  for( FormMap1->UniQ->Open(); !FormMap1->UniQ->Eof; FormMap1->UniQ->Next(),cnt1++ )
	  {
		 UnicodeString as1 = FormMap1->UniQ->FieldByName("v_alarm_check")->AsString;
		 UnicodeString as2 = FormMap1->UniQ->FieldByName("v_alarm_enable")->AsString;
// Устанавливаем цвет тревоги
		 AdvStringGrid2->RowCount      = cnt1+2;
		 AdvStringGrid2->Cells[0][cnt1+1] = FormMap1->UniQ->FieldByName("v_text_trevogi")->AsString;
		 int tid = FormMap1->UniQ->FieldByName("tid")->AsInteger;
		 AdvStringGrid2->Objects[0][cnt1+1] = (TObject *)tid;
		 AdvStringGrid2->Cells[1][cnt1+1] = FormMap1->DateToString(FormMap1->UniQ->FieldByName("v_date_time_voz_trev")->AsDateTime);
		 if (FormMap1->UniQ->FieldByName("v_date_time_sniat_trev")->AsString != "")
			AdvStringGrid2->Cells[4][cnt1+1] = FormMap1->DateToString(FormMap1->UniQ->FieldByName("v_date_time_sniat_trev")->AsDateTime);
		 else
			AdvStringGrid2->Cells[4][cnt1+1] = "";
		 AdvStringGrid2->Cells[5][cnt1+1] = FormMap1->UniQ->FieldByName("V_ALARMCOMMENTTYPE")->AsString;
		TColor cl;
			 // Проверка что тревога квитирована
		  if ((FormMap1->UniQ->FieldByName("v_date_time_kvit_trev")->AsAnsiString != "") && (FormMap1->UniQ->FieldByName("v_date_time_sniat_trev")->AsAnsiString != ""))
		  {
			 AdvStringGrid2->Cells[3][cnt1+1] = FormMap1->DateToString(FormMap1->UniQ->FieldByName("v_date_time_kvit_trev")->AsDateTime);
			 AdvStringGrid2->Cells[4][cnt1+1] = FormMap1->DateToString(FormMap1->UniQ->FieldByName("v_date_time_sniat_trev")->AsDateTime);
             if (bold == "1")
			 {
				AdvStringGrid2->CellProperties[0][cnt1+1]->FontStyle  >> fsBold;
				AdvStringGrid2->CellProperties[1][cnt1+1]->FontStyle  >> fsBold;
				AdvStringGrid2->CellProperties[2][cnt1+1]->FontStyle  >> fsBold;
				AdvStringGrid2->CellProperties[3][cnt1+1]->FontStyle  >> fsBold;
				AdvStringGrid2->CellProperties[4][cnt1+1]->FontStyle  >> fsBold;
				AdvStringGrid2->CellProperties[5][cnt1+1]->FontStyle  >> fsBold;
			 }
			 cl = (TColor)RGBStringToColor(FormMap1->Controllers->ReadProperty("ALARM","ColorNAktiv","208,208,208"));
		  }
		  if ((FormMap1->UniQ->FieldByName("v_date_time_kvit_trev")->AsAnsiString == "") && (FormMap1->UniQ->FieldByName("v_date_time_sniat_trev")->AsAnsiString != ""))
		  {
			 AdvStringGrid2->Cells[3][cnt1+1] = "";
			 AdvStringGrid2->Cells[4][cnt1+1] = FormMap1->DateToString(FormMap1->UniQ->FieldByName("v_date_time_sniat_trev")->AsDateTime);
             if (bold == "1")
			 {
				AdvStringGrid2->CellProperties[0][cnt1+1]->FontStyle  << fsBold;
				AdvStringGrid2->CellProperties[1][cnt1+1]->FontStyle  << fsBold;
				AdvStringGrid2->CellProperties[2][cnt1+1]->FontStyle  << fsBold;
				AdvStringGrid2->CellProperties[3][cnt1+1]->FontStyle  << fsBold;
				AdvStringGrid2->CellProperties[4][cnt1+1]->FontStyle  << fsBold;
				AdvStringGrid2->CellProperties[5][cnt1+1]->FontStyle  << fsBold;
			 }
			 cl = (TColor)RGBStringToColor(FormMap1->Controllers->ReadProperty("ALARM","ColorNKvitir","227,203,193"));
		  }
		  if ((FormMap1->UniQ->FieldByName("v_date_time_kvit_trev")->AsAnsiString == "") && (FormMap1->UniQ->FieldByName("v_date_time_sniat_trev")->AsAnsiString == ""))
		  {
			 AdvStringGrid2->Cells[3][cnt1+1] = "";
			 AdvStringGrid2->Cells[4][cnt1+1] = "";
			 if (bold == "1")
			 {
				AdvStringGrid2->CellProperties[0][cnt1+1]->FontStyle  << fsBold;
				AdvStringGrid2->CellProperties[1][cnt1+1]->FontStyle  << fsBold;
				AdvStringGrid2->CellProperties[2][cnt1+1]->FontStyle  << fsBold;
				AdvStringGrid2->CellProperties[3][cnt1+1]->FontStyle  << fsBold;
				AdvStringGrid2->CellProperties[4][cnt1+1]->FontStyle  << fsBold;
				AdvStringGrid2->CellProperties[5][cnt1+1]->FontStyle  << fsBold;
			 }
			 cl = (TColor)RGBStringToColor(FormMap1->Controllers->ReadProperty("ALARM","ColorAlarm","188,74,45"));
		  }
		  if ((FormMap1->UniQ->FieldByName("v_date_time_kvit_trev")->AsAnsiString != "") && (FormMap1->UniQ->FieldByName("v_date_time_sniat_trev")->AsAnsiString == ""))
		  {
			 AdvStringGrid2->Cells[3][cnt1+1] = FormMap1->DateToString(FormMap1->UniQ->FieldByName("v_date_time_kvit_trev")->AsDateTime);
			 AdvStringGrid2->Cells[4][cnt1+1] = "";
			 if (bold == "1")
			 {
				AdvStringGrid2->CellProperties[0][cnt1+1]->FontStyle  >> fsBold;
				AdvStringGrid2->CellProperties[1][cnt1+1]->FontStyle  >> fsBold;
				AdvStringGrid2->CellProperties[2][cnt1+1]->FontStyle  >> fsBold;
				AdvStringGrid2->CellProperties[3][cnt1+1]->FontStyle  >> fsBold;
				AdvStringGrid2->CellProperties[4][cnt1+1]->FontStyle  >> fsBold;
			 	AdvStringGrid2->CellProperties[5][cnt1+1]->FontStyle  >> fsBold;
			 }
			 cl = (TColor)RGBStringToColor(FormMap1->Controllers->ReadProperty("ALARM","ColorKvitir","218,216,127"));
		  }
		  AdvStringGrid2->RowColor[cnt1+1] = cl;
	  }
	  FormMap1->UniQ->Close();
	  LabelAlarm->Caption = " Тревоги ("+IntToStr(cnt1)+")";
	  LabelAlarm->Font->Style >> fsBold;
	  if (cnt1>0)
	  {
		  IdA  =(int)AdvStringGrid2->Objects[0][1];
		  ClickAlarm();
	  }
	  else
	  {
		  Grid3->Cells[1][1] = "";
		  Grid3->Cells[1][2] = "";
		  Grid3->Cells[1][3] = "";
		  Grid3->Cells[1][4] = "";
		  Grid3->RowHeights[5] = 0;
	  }

   }
   catch(Exception &e)
   {
	  Timer1->Enabled = false;
	  ShowMessage("Ошибка отображения аварий "+PP+" "+e.Message);
   }
}

void __fastcall TFormTok1::AdvStringGrid2DblClickCell(TObject *Sender, int ARow, int ACol)

{
/*DateTimePicker1->DateTime = AdvStringGrid1->Cells[1][ARow];
ClearPropertyAll();
ShowStatusPP();
ShowPP();*/
ShowAlarm();
}
//---------------------------------------------------------------------------

void __fastcall TFormTok1::Grid3GetEditorType(TObject *Sender, int ACol, int ARow,
          TEditorType &AEditor)
{
//AEditor = edRichEdit;
}
//---------------------------------------------------------------------------

void __fastcall TFormTok1::Grid3ControlClick(TObject *Sender, int ARow, int ACol, UnicodeString CtrlID,
          UnicodeString CtrlType, UnicodeString CtrlVal)
{
try
{
	  if (CtrlID == "but")
	  {
	  TDateTime dt;
	  dt = dt.CurrentDateTime();
	  FormMap1->Global->Query->Close();
	  FormMap1->Global->Query->SQL->Clear();

	  FormMap1->Global->Query->SQL->Add("UPDATE j_trevogi SET o_alarmcommenttype = :a0");
	  FormMap1->Global->Query->SQL->Add(",v_komment_dispetcher = :a1");
	  FormMap1->Global->Query->SQL->Add(",v_alarm_check = :a2");
	  FormMap1->Global->Query->SQL->Add(",v_date_time_kvit_trev = :a3");
	  FormMap1->Global->Query->SQL->Add(",v_alarm_username = :a4");
	  FormMap1->Global->Query->SQL->Add(",last_editing = :a5");
	  FormMap1->Global->Query->SQL->Add(" WHERE id_object = :a6");
	  FormMap1->Global->Query->Params->Items[0]->AsString    = alarmcom;
	  FormMap1->Global->Query->Params->Items[1]->AsString    = "";//Grid3->Cells[1][3];
	  FormMap1->Global->Query->Params->Items[2]->AsString    = "Да";
	  FormMap1->Global->Query->Params->Items[3]->AsDateTime  = dt;
	  FormMap1->Global->Query->Params->Items[4]->AsString    = FormMap1->Global->UserSem;
	  FormMap1->Global->Query->Params->Items[5]->AsString    =
		 dt.FormatString("dd.mm.yyyy hh:nn") + " " + getenv("USERNAME") + "@" + getenv("USERDOMAIN");
	  FormMap1->Global->Query->Params->Items[6]->AsInteger   = IdA;
	  FormMap1->Global->Query->ExecSQL();
	  ShowAlarm();
	  for (int i = 1; i <= AdvStringGrid2->RowCount; i++)
	  {
		if (IdA  == (int)AdvStringGrid2->Objects[0][i])
		{
		   AdvStringGrid2->SelectCells(0,i,AdvStringGrid2->ColCount,i);
		   break;
		}
	  }
	  ClickAlarm();
	  }
	  if (CtrlID == "butal")
	  {
		  /*UnicodeString st = "Квитировать все тревоги " + FormMap1->nampp + " № " + PP + "?";
		  if (MessageBox(NULL,st.w_str(),L"Квитирование тревог",MB_OKCANCEL | MB_DEFBUTTON2 | MB_DEFAULT_DESKTOP_ONLY | MB_TASKMODAL) == IDOK )
		{
		 TDateTime dt;
		 FormMap1->Global->Query->Close();
		 FormMap1->Global->Query->SQL->Clear();
		 FormMap1->Global->Query->SQL->Add("UPDATE j_trevogi SET v_komment_dispetcher = :a0");
		 FormMap1->Global->Query->SQL->Add(",v_alarm_check = :a1");
		 FormMap1->Global->Query->SQL->Add(",v_date_time_kvit_trev = :a2");
		 FormMap1->Global->Query->SQL->Add(",v_alarm_username = :a3");
		 FormMap1->Global->Query->SQL->Add(",last_editing = :a4");
		 FormMap1->Global->Query->SQL->Add(" WHERE v_alarm_check = :a5 and j_trevogi.o_pp=(SELECT j_srpav_pp.id_object FROM j_srpav_pp Where j_srpav_pp.v_pp = :a6)");
		 FormMap1->Global->Query->Params->Items[0]->AsString    = "Квитировано автоматически";
		 FormMap1->Global->Query->Params->Items[1]->AsString    = "Да";
		 FormMap1->Global->Query->Params->Items[2]->AsDateTime  = dt.CurrentDateTime();
		 FormMap1->Global->Query->Params->Items[3]->AsString    = FormMap1->Global->UserSem;
		 FormMap1->Global->Query->Params->Items[4]->AsString    =
			dt.FormatString("dd.mm.yyyy hh:nn") + " " + getenv("USERNAME") + "@" + getenv("USERDOMAIN");
		 FormMap1->Global->Query->Params->Items[5]->AsString    = "Нет";
		 FormMap1->Global->Query->Params->Items[6]->AsString    = PP;
		 FormMap1->Global->Query->ExecSQL();
		 ClickAlarm();
		 ShowAlarm();
		 FormMap1->ToolButton9->Click();
		}                                  */
		Application->CreateForm(__classid(TKvitVse), &KvitVse);
	  KvitVse->Top = (Screen->Height - KvitVse->Height) * 0.5;
	  KvitVse->Left  = (Screen->Width  - KvitVse->Width) * 0.5;
	  KvitVse->MaskEdit1->Text = "";
	  KvitVse->tip = false;
	  KvitVse->FormKvit = "FormTok";
	  KvitVse->Show();
      }
}
catch(...)
{
     Timer1->Enabled = false;
}
}
//---------------------------------------------------------------------------

void __fastcall TFormTok1::AdvGrid2Color()
{
	int j,max;
	int srow;
	srow = AdvGrid2->Row;
	AdvGrid2->RowCount = 1;
	for  (int i = 0; i<MAX_CONTROLLER; i++ )
	{
		cntrg = FormMap1->Controllers->Controllers[i];
		if ((cntrg != NULL) && (cntrg->ppStatus.Pos("Модем") != 0))
		{
		AdvGrid2->Cells[0][AdvGrid2->RowCount-1] = cntrg->Status;
		AdvGrid2->Cells[1][AdvGrid2->RowCount-1] = cntrg->PP;
		switch( cntrg->Status )
		{
		 case TKControllerStatus::csNone   :
			AdvGrid2->FontColors[0][AdvGrid2->RowCount-1] = FormMap1->PP_disable;
			break;
		 case csPowerOff   :
			AdvGrid2->FontColors[0][AdvGrid2->RowCount-1] = FormMap1->PP_off;
			break;
		 case csPowerOn    :
			AdvGrid2->FontColors[0][AdvGrid2->RowCount-1] = FormMap1->PP_on;
			break;
		 case csPowerNight :
			AdvGrid2->FontColors[0][AdvGrid2->RowCount-1] = FormMap1->PP_on1;
			break;
		 case csLinkNone   :
			AdvGrid2->FontColors[0][AdvGrid2->RowCount-1] = FormMap1->PP_nolink;
			break;
		 case csLinkError  :
			AdvGrid2->FontColors[0][AdvGrid2->RowCount-1] = FormMap1->PP_nolink;
			break;
		 case csAlarm      :
			AdvGrid2->FontColors[0][AdvGrid2->RowCount-1] = FormMap1->PP_alarm;
			break;
		 default           :
			AdvGrid2->FontColors[0][AdvGrid2->RowCount-1] = FormMap1->PP_disable;
		}
		if (cntrg->PP == PP) j = AdvGrid2->RowCount-1;
		if (max < cntrg->PP.Length()) max = cntrg->PP.Length();
		AdvGrid2->RowCount = AdvGrid2->RowCount + 1;
		}
	}
	AdvGrid2->RowCount = AdvGrid2->RowCount - 1;
	AdvGrid2->Width = max*Font->Size;
	AdvGrid2->ColWidths[0] = 20;
	AdvGrid2->ColWidths[1] = AdvGrid2->Width - 45;
	AdvGrid2->ScrollInView(0,j);
	if (srow != 0)
	{
		AdvGrid2->Row = srow;
	}
	else
	{
		TGridRect r;
		r.Left = AdvGrid2->FixedCols;
		r.Top = j;
		r.Right = AdvGrid2->ColCount;
		r.Bottom = j;
		AdvGrid2->Selection = r;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormTok1::AdvGrid2KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
switch( Key )
   {
	   case 38:
	   SelectCell(AdvGrid2->Col,AdvGrid2->Row-1);
	   break;
	   case 40:
	   SelectCell(AdvGrid2->Col,AdvGrid2->Row+1);
	   break;
   }
}
//---------------------------------------------------------------------------


void __fastcall TFormTok1::AdvSplitter2Click(TObject *Sender)
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






void __fastcall TFormTok1::Grid3ControlComboList(TObject *Sender, int ARow, int ACol,
          UnicodeString CtrlID, UnicodeString CtrlType, UnicodeString CtrlVal, TStringList *Values,
          bool &Edit, int &DropCount)
{
Values->Clear();
if (CtrlID == "CO2")
{
  Values->AddStrings(FormMap3->alarmcomment);
  Edit = false;
}
}
//---------------------------------------------------------------------------

void __fastcall TFormTok1::Grid3ControlComboSelect(TObject *Sender, int ARow, int ACol,
          int ItemIndex, UnicodeString CtrlID, UnicodeString CtrlValue)
{
  alarmcom = (int)FormMap3->alarmcomment->Objects[ItemIndex];
}
//---------------------------------------------------------------------------





void __fastcall TFormTok1::SpeedButton1Click(TObject *Sender)
{
//Перевыдать график

	FShow();
//CheckBox1->OnClick;

}
//---------------------------------------------------------------------------

