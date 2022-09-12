//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "mmsystem.h"
#include "math.h"
#include "time.h"
#include "AlarmUnit1.h"
#include "AppHelper.h"
#include "AlarmEvent.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "Uni"
#pragma link "OracleUniProvider"
#pragma link "UniProvider"
#pragma link "AdvEdit"
#pragma link "HTMListB"
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "BaseGrid"
#pragma link "AdvToolBtn"
#pragma link "AdvUtil"
#pragma resource "*.dfm"
TForm1 *Form1;
extern TForm2 *Form2;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
		: TForm(Owner)
{
   FlagBeep   = true;
   CheckCount = 0;
   FAlarmEvents = new TList();
}
//---------------------------------------------------------------------------

/**
* Инициализация подключения к системе Катюша
*/
void __fastcall TForm1::InitSem()
{
	try
    {
	    Global        = new TKGlobalSem(UniConnection1);
	    Controllers   = new TKControllers(Global);

	    TimeOutPoll  = Controllers->ReadPropertyInt("ALARM","TimeOutPoll",60);
	    TimeOutBeep  = Controllers->ReadPropertyInt( "ALARM","TimeOutBeep",60);
	    Top          = (Screen->Height - Height) * Controllers->ReadPropertyInt("ALARM","CenterY",50)/100;
	    Left         = (Screen->Width - Width) * Controllers->ReadPropertyInt("ALARM","CenterX", 50)/100;

        // Проверяем, является ли текущий пользователь членом группы с расширенными правами

	    IsAdmin = false;
	    AnsiString adm_g = Controllers->ReadProperty( "ALARM","AdminGroup","xxx");
	}
	catch(Exception &ex)
    {
	   ShowMessage("Ошибка инициализации "+ ex.Message);
	   Application->Terminate();
	}
}
//---------------------------------------------------------------------------

/**
* Инициализация журналов сем
*/
void __fastcall TForm1::InitJournals()
{
// Открываем журнал тревог
 /*  j_alarm = new TKJournal(Global1, "Alarms");
   j_alarm->GetListParameter();
   j_alarm->AddParamToList("Rayon");   */
}
//---------------------------------------------------------------------------

/**
* Загрузка тревог из журнала
*/
void __fastcall TForm1::LoadAlarmFromSem()
{
    // Сохраняем текущую позицию GRID и запоменаем текущую строку
    GridTopRow   = AdvStringGrid1->TopRow;
    GridRowCount = AdvStringGrid1->RowCount;
    GridRow      = AdvStringGrid1->Row;
    int Row1     = AdvStringGrid1->FixedRows;
    AdvStringGrid1->ShowSelection = true;

    if( AdvStringGrid1->Row >= AdvStringGrid1->FixedRows &&
	   AdvStringGrid1->Row < AdvStringGrid1->RowCount )
	   CurrentTid = (int)AdvStringGrid1->Objects[0][AdvStringGrid1->Row];
    else
	   CurrentTid = -1;

    ReloadAlarmEvents(TDateTime::CurrentDateTime());

    FlagPauseTimer  = true;
    bool AlarmEmpty = false;
    AlarmSoundFlag  = aAlarmNone;
    
    // Очищаем GRID
    for(int i=AdvStringGrid1->FixedRows; i < AdvStringGrid1->RowCount; i++)
	    for(int j=0; j < AdvStringGrid1->ColCount; j++)
	    {
		    AdvStringGrid1->Cells[j][i]   = "";
		    AdvStringGrid1->CellProperties[j][i]->BrushColor = clWhite;
		    AdvStringGrid1->Objects[j][i] = NULL;
	    }

    // Заполняем GRID
    int fix = 1;
    AdvStringGrid1->FixedRows = 0;

    for(int i=0; i < FAlarmEvents->Count; i++)
    {
        TAlarmEvent* ev = (TAlarmEvent*)FAlarmEvents->Items[i];
	    AdvStringGrid1->RowCount = fix+i+1;

	    AdvStringGrid1->Objects[0][i+fix] = (TObject*)ev;

        // Проверка текущей строки
	    if(ev->Id == CurrentTid)
            Row1 = i + fix;

	    AdvStringGrid1->Cells[0][i+fix] = ev->Pp;
	    AdvStringGrid1->Cells[1][i+fix] = ev->AlarmDesc;
	    AdvStringGrid1->Cells[2][i+fix] = ev->AlarmDate.FormatString("dd/mm hh:nn");
	    AdvStringGrid1->Cells[3][i+fix] = ev->Kolvo;

        // Проверка что тревога квитирована
	    if(ev->AlarmCheck)
        {
            AdvStringGrid1->AddCheckBox(4,i+fix,true,false);
        }
	    else
        {
            AdvStringGrid1->AddCheckBox(4,i+fix,false,false);
            AlarmSoundFlag = aAlarmYes;
        }

        // Проверка, что тревога снята
        if(ev->AlarmEnable)
        {
            AdvStringGrid1->Cells[5][i+fix]
			    = ev->AlarmReleased.FormatString("dd/mm hh:nn"); //Sql1->FieldByName("v_date_time_sniat_trev")->AsDateTime.FormatString("dd/mm hh:nn");
	    }

        // Устанавливаем свойства таблицы
	    for( int j=0; j<AdvStringGrid1->ColCount; j++ )
	    {
		    AdvStringGrid1->CellProperties[j][i+fix]->ReadOnly   = true;
		    if(!ev->AlarmCheck)
		    {
			    AdvStringGrid1->CellProperties[j][i+fix]->BrushColor = (TColor)RGBStringToColor(Controllers->ReadProperty("Alarm","ColorAlarm","188,74,45"));
			    AdvStringGrid1->CellProperties[j][i+fix]->FontStyle  << fsBold;
		    }
		    else
		    {
			    AdvStringGrid1->CellProperties[j][i+fix]->BrushColor = (TColor)RGBStringToColor(Controllers->ReadProperty("Alarm","ColorKvitir","218,216,127"));
			    AdvStringGrid1->CellProperties[j][i+fix]->FontStyle  >> fsBold;
		    }
	    }
    }

    // Устанавливаем свойства заголовка таблицы
    if(FAlarmEvents->Count == 0)
    {
	    AdvStringGrid1->FixedRows = 0;
	    AdvStringGrid1->RowCount  = fix+1;
	    AdvStringGrid1->FixedRows = fix;

	    for( int j=0; j<AdvStringGrid1->ColCount; j++ )
	    {
            AdvStringGrid1->CellProperties[j][1]->BrushColor = clWhite;
		    AdvStringGrid1->CellProperties[j][1]->FontStyle  = TFontStyles();
	    }
	    AdvStringGrid1->Cells[2][1] = "Тревоги отсутвуют";
	    AlarmEmpty = true;
    }
    else
    {
	    AdvStringGrid1->FixedRows = fix;
    }

    // Проверяем, сдвинулась ли текущая строка
    if( AdvStringGrid1->RowCount == GridRowCount || GridRow == Row1 )
    {
	    AdvStringGrid1->TopRow = GridTopRow;
	    AdvStringGrid1->Row    = Row1;
    }
    else
    {
	    AdvStringGrid1->Row    = Row1;
    }

    if(FlagStayOnTop)
        Form1->FormStyle = fsStayOnTop;
    FlagPauseTimer = false;
}
//---------------------------------------------------------------------------

/**
* Установка ширины столюцов грида
* работает при изменении размера
*/
void __fastcall TForm1::InitWidthColumns()
{
    try
    {
        int n00 = 50;

        int x0 = 15;
        int x1 = 52;
        int x2 = 20;
        int x3 =  0;
        int x4 = 10;
        int x5 =  0;

        int n0 = (AdvStringGrid1->Width-15)*x0/100;
        int n1 = (AdvStringGrid1->Width-15)*x1/100;
        int n2 = (AdvStringGrid1->Width-15)*x2/100;
        int n3 = (AdvStringGrid1->Width-15)*x3/100;
        int n4 = (AdvStringGrid1->Width-15)*x4/100;
        int n5 = (AdvStringGrid1->Width-15)*x5/100;

        AdvStringGrid1->ColWidths[0] = n0;
        AdvStringGrid1->ColWidths[1] = n1;
        AdvStringGrid1->ColWidths[2] = n2;
        AdvStringGrid1->ColWidths[3] = n3;
        AdvStringGrid1->ColWidths[4] = n4;
        AdvStringGrid1->ColWidths[5] = n5;

        AdvStringGrid1->DefaultRowHeight = GridRowHeight;
        AdvStringGrid1->RowHeights[0]    = GridRowHeight*2;
        AdvStringGrid1->Font->Size       = GridFontSize;
        AdvStringGrid1->FixedFont->Size  = GridFontSize;
    }
    catch(Exception &ex)
    {
	    ShowMessage("Ошибка "+ ex.Message);
    }
}
//---------------------------------------------------------------------------

/**
* Инициализация всех контролов
*/
void __fastcall TForm1::InitControls()
{
    if(apphelper->MaximizedForm)
        Form1->WindowState = wsMaximized;

    GridRowHeight = Controllers->ReadPropertyInt( "ALARM","AlarmRowHeight", 20);
    GridFontSize  = Controllers->ReadPropertyInt( "ALARM","AlarmFontSize", 10);

    InitWidthColumns();
    AdvStringGrid1->Cells[0][0]   = "Пункт\rпитания";
    AdvStringGrid1->Cells[1][0]   = "Текст тревоги";
    AdvStringGrid1->Cells[2][0]   = "Возникновение\rтревоги";
    AdvStringGrid1->Cells[3][0]   = "Кол-во\rсобытий";
    AdvStringGrid1->Cells[4][0]   = "К";
    AdvStringGrid1->Cells[5][0]   = "Снятие\rтревоги";
    
    for(int i=0; i< AdvStringGrid1->ColCount; i++)
	    AdvStringGrid1->CellProperties[i][0]->Alignment = taCenter;

    if( Controllers->ReadPropertyInt( "ALARM","StayOnTop",0) == 1 )
        FlagStayOnTop = true;
    else
        FlagStayOnTop = false;

    // Параметры протокола отображения на АСД ПП
    ClientSocket1->Host       = Controllers->ReadProperty( "ASD","AsdServerHost","");
    ClientSocket1->Port       = Controllers->ReadPropertyInt( "ASD","AsdServerPort",20001);
    if( Controllers->ReadPropertyInt( "ASD","AsdServerEnable",0) == 1 )
        FlagShowMap = true;
    else
        FlagShowMap          = false;

    ShowMapSecret             = Controllers->ReadProperty( "ASD","AsdServerSecret","");
    cmdApplication            = Controllers->ReadProperty( "ALARM","AlarmApplicationExec","");
    for(int i=1; i<=10; i++)
        AddButton(i);

    // Открываем список контроллеров
	ProgressBar1->Parent = StatusBar1;
		ProgressBar1->Top    = 2;
		ProgressBar1->Height = StatusBar1->Height - 4;;
		ProgressBar1->Left   = 2;
		ProgressBar1->Width  = StatusBar1->Width-4;

// Настройка отладки
   Controllers->InitControllers(Sql1);
}
//---------------------------------------------------------------------------

/**
* Проигрыванием файла
*
*@param File - имя файла
*/
void __fastcall TForm1::PlayFile(AnsiString File)
{
   UnicodeString s1 = GetCurrentDir()+"\\"+File;
   if( FileExists( s1 ) )
   {
	  DWORD fdwSound = SND_ASYNC |  SND_FILENAME;
	  PlaySound(s1.w_str(),NULL, fdwSound);
   }
}
//---------------------------------------------------------------------------

/**
* Показать ПП на карте
*
* @param pp - Пунkт питания
*/
void __fastcall TForm1::ShowOnMap(AnsiString pp)
{
   if( !FlagShowMap )return;
   
    try
    {
	    ClientSocket1->Active = true;
	    ClientSocket1->Send(ShowMapSecret+"|ShowPP|"+pp);
    }
    catch(...)
    {
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Exec(AnsiString InitCmd, AnsiString ErrorMsg )
{
    try
    {
	    UnicodeString FullName = Controllers->ReadProperty("ALARM", InitCmd, "");
	    TStringList *l = new TStringList();
	    l->Delimiter = ' ';
	    l->DelimitedText = FullName;
	    for( int i=0; i<l->Count; i++ )
	    {
		    if(l->Strings[i] == "<user>" )
                l->Strings[i] = apphelper->LoginInfo->Name;
		    if(l->Strings[i] == "<passwd>" )
                l->Strings[i] = apphelper->LoginInfo->Password;
	    }
        
	    FullName = l->Text;
	    if( l->Count > 0 )
	    {
		    UnicodeString CmdName  = ExtractFileName(l->Strings[0]);
		    UnicodeString PathName = ExtractFilePath(l->Strings[0]);
            
		    ShellExecute(NULL,L"open",CmdName.w_str(),FullName.w_str(),PathName.w_str(),SW_RESTORE);
	    }
	    delete l;
    }
    catch(...)
    {
        ShowMessage(ErrorMsg);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AlarmButtonClick(TObject *Sender)
{
    TSpeedButton *sb = (TSpeedButton *)Sender;
    Exec(sb->Name,"Ошибка запуска "+sb->Name);
}
//---------------------------------------------------------------------------

/**
* Добавить кнопочку
*/
void __fastcall TForm1::AddButton(int n)
{
   AnsiString str_n = IntToStr(n);
   try
   {
// Проверяем кнопочки
	  if( Controllers->ReadProperty( "ALARM","AlarmButton"+str_n,0) == 1 )
	  {
		 TSpeedButton *sb  = new TSpeedButton(Form1);
		 sb->Name          = "AlarmButtonExec"+str_n;
		 sb->Parent        = ToolBar1;
		 sb->Glyph->LoadFromFile(GetCurrentDir()+"\\"+Controllers->ReadProperty( "ALARM","AlarmButtonIcon"+str_n,""));
		 sb->Left          = ToolButton1->Left + ToolButton1->Width + (n-1)*32;
		 sb->Width         = 32;
		 sb->Height        = 30;
		 sb->ShowHint      = true;
		 sb->Hint          = Controllers->ReadProperty( "ALARM","AlarmButtonHint"+str_n,"");
		 sb->Flat          = true;
		 sb->OnClick       = AlarmButtonClick;
	  }
   }
   catch(...)   {ShowMessage("Ошибка создания кнопки AlarmButton"+str_n);}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormActivate(TObject *Sender)
{
    if( FlagOnce )
    return;

    Screen->Cursor = crHourGlass;
    try
    {
        InitSem();
        InitControls();
        InitJournals();
        LoadAlarmFromSem();
        FlagOnce = true;
        FlagPauseTimer = false;
        Timer1->Enabled = true;
        Form1->Refresh();
        Form1->Width++;
        nampp =  Controllers->ReadProperty("AutoForm","NamePP","ПП");
    }
    __finally
    {
        Screen->Cursor = crDefault;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormResize(TObject *Sender)
{
    InitWidthColumns();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AdvStringGrid1RowChanging(TObject *Sender, int OldRow, int NewRow,
          bool &Allow)
{
    if(NewRow < 1)
    {
        Allow = false;
    }
    else
    {
        AdvStringGrid1->SelectionRectangle = true;
        AdvStringGrid1->ShowSelection = true;
        //AdvStringGrid1->SelectionColor = AdvStringGrid1->CellProperties[0][NewRow]->BrushColor;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
    if(FlagPauseTimer)
        return;

    CheckCount++;
    TDateTime d = CheckImportRT();
    if( d != RtPollDate || CheckCount >= 20)
    {
	    RtPollDate = d;
	    CheckCount=0;
	    LoadAlarmFromSem();
    }

    if( TimerCount%TimeOutBeep == 0 && FlagBeep )
    {
        switch( AlarmSoundFlag )
	    {
		    case aAlarmYes:
			    PlayFile(Controllers->ReadProperty( "ALARM","AlarmSound",""));
			    break;
		    case aAlarmNew:
			    PlayFile(Controllers->ReadProperty( "ALARM","AlarmSoundNew",""));
			    break;
        }
    }
    TimerCount++;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AdvStringGrid1DblClickCell(TObject *Sender, int ARow, int ACol)
{
    Row = AdvStringGrid1->RealRow;
    if (!(Row == 0))
    {
        FormStyle = Vcl::Forms::fsNormal;
        Application->CreateForm(__classid(TFormTok), &FormTok);

        TAlarmEvent* ev = (TAlarmEvent*)AdvStringGrid1->Objects[0][Row];

        FormTok->punkt  = ev->Pp;
        FormTok->DateTimePicker1->DateTime =  ev->AlarmDate;
        FormTok->DateTimePicker2->DateTime =  ev->AlarmDate;

        if(Row < AdvStringGrid1->FixedRows)
            return;
            
        if(FormTok->Visible)
            FormTok->Close();
        Timer1->Enabled = false;

        if(FormTok->punkt == "") 
            return;
            
        // Ищем ПП в списке контроллеров
        for( int i=0; i<MAX_CONTROLLER; i++)
        {
	        TKController *cnt = Controllers->Controllers[i];
	        if( cnt == NULL )continue;
	        if( cnt->PP == FormTok->punkt )
		        break;
        }
        FormTok->Show();
        Timer1->Enabled = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AdvStringGrid1ClickCell(TObject *Sender, int ARow, int ACol)
{
    // Проверка что строка в заголовке
    if(ARow < 1 )return;
    if(ACol == 0 )  Row = ARow;
    if(ACol!=4)return;

    bool ck;
    // Проверка состояния чекбокса

    FlagPauseTimer = true;

    TAlarmEvent* ev = (TAlarmEvent*)AdvStringGrid1->Objects[0][ARow];
    Form2->Id   = ev->Id;
    TFormStyle st = Form1->FormStyle;
    Form1->FormStyle = Vcl::Forms::fsNormal;
    Form2->ShowModal();
    Form1->FormStyle = st;

    if(Form2->ReturnFlag)
        LoadAlarmFromSem();
    FlagPauseTimer = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton2Click(TObject *Sender)
{
    AdvEdit1->Visible = true;
	AdvEdit1->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AdvEdit1KeyPress(TObject *Sender, System::WideChar &Key)
{
    if(Key == 13)
    {
	    AdvEdit1->Visible = false;
	    if( AdvEdit1->Text == "12345" )
	    {
            AdvEdit1->Text = "";
		    TDateTime dt;
		    Sql2->Close();
		    Sql2->SQL->Clear();

		    Sql2->SQL->Add("UPDATE j_trevogi SET v_komment_dispetcher = :a0");
		    Sql2->SQL->Add(",v_alarm_check = :a1");
		    Sql2->SQL->Add(",v_date_time_kvit_trev = :a2");
		    Sql2->SQL->Add(",v_alarm_username = :a3");
		    Sql2->SQL->Add(",last_editing = :a4");
		    Sql2->SQL->Add(" WHERE v_alarm_check = :a5");
		    Sql2->Params->Items[0]->AsString    = "Квитировано автоматически";
		    Sql2->Params->Items[1]->AsString    = "Да";
		    Sql2->Params->Items[2]->AsDateTime  = dt.CurrentDateTime();
		    Sql2->Params->Items[3]->AsString    = apphelper->LoginInfo->Name;
		    Sql2->Params->Items[4]->AsString    =
			dt.FormatString("dd.mm.yyyy hh:nn") + " " + getenv("USERNAME") + "@" + getenv("USERDOMAIN");
		    Sql2->Params->Items[5]->AsString    = "Нет";
		    Sql2->ExecSQL();
		    LoadAlarmFromSem();
	    }//end if
        else 
            ShowMessage("Пароль неправильный");
    }
    else if(Key == 27)
    {
        AdvEdit1->Text = "";
        AdvEdit1->Visible = false;
        AdvStringGrid1->SetFocus();	
    }    
}
//---------------------------------------------------------------------------

/**
* Проверка наличия нового опроса RT
*
* @return - Дата и время последнего опроса
*/
TDateTime __fastcall TForm1::CheckImportRT()
{
   try
   {
	  TDateTime d = 0;
	  Sql1->Close();
	  Sql1->SQL->Clear();
	  Sql1->SQL->Add("SELECT * FROM j_rt_poll WHERE v_rt_poll_name = \'Alarm\'");
	  Sql1->Open();
	  if( !Sql1->Eof )d = Sql1->FieldByName("v_rt_poll_time")->AsDateTime;
	  Sql1->Close();
	  return d;
   }
   catch(...)
   {
	  StatusBar1->SimpleText = "Ошибка опроса базы";
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton3Click(TObject *Sender)
{
	if( FlagBeep )
	{
		SpeedButton4->Visible = false;
		SpeedButton3->Visible = true;
	}
   else
	{
		SpeedButton3->Visible = false;
		SpeedButton4->Visible = true;
	}
	FlagBeep = !FlagBeep;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ClearAlarmEvents()
{
    for(int i=0; i < FAlarmEvents->Count; i++)
    {
        TAlarmEvent* ev = (TAlarmEvent*)FAlarmEvents->Items[i];
        delete ev;
    }
    FAlarmEvents->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
    ClearAlarmEvents();
    delete FAlarmEvents;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ReloadAlarmEvents(TDateTime act_dt)
{
    ClearAlarmEvents();
    apphelper->DbHelper->ReloadAlarmEvents(FAlarmEvents, act_dt);
}
//---------------------------------------------------------------------------

