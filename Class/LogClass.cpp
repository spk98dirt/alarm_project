//---------------------------------------------------------------------------

#pragma hdrstop

#include "LogClass.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)



/**
* �����������
*
*@param console - �������
*@param bar     - ���
*@param clb     - ��� ���� ���� ��������� ������ �������
*/
__fastcall TKLog::TKLog(TMemo *console, TProgressBar *bar, TCheckListBox *clb)
{
	Console  = console;
	Bar      = bar;
	LevelBox = clb;
	StartDT  = StartDT.CurrentDateTime();
/*
	if( LevelBox != NULL )
	{
		LevelBox->Items->Clear();
		LevelBox->Items->Add("������");
		LevelBox->Items->Add("���������");
		LevelBox->Items->Add("�������");
		LevelBox->OnClick = LevelBoxClick;
	}
*/
	SetLevel(llError);

	WithDT      = false;
	WithLogFile = false;
	LogFile     = "logfile.txt";
	MaxCount    = MAX_COUNT;
}

// ������ � ��������
/**
* ������� �������
*/
void __fastcall TKLog::Clear()
{
    Console->Lines->Clear();
}

/**
*   ���������� ������� ������ ���������
*/
void __fastcall TKLog::SetLevel( TKLogLevel level )
{
	Level = level;
/*
	if( LevelBox == NULL )return;
	if( level & llError   )LevelBox->Checked[0] = true;
	else                   LevelBox->Checked[0] = false;
	if( level & llMessage )LevelBox->Checked[1] = true;
	else                   LevelBox->Checked[1] = false;
	if( level & llDebug   )LevelBox->Checked[2] = true;
	else                   LevelBox->Checked[2] = false;
*/
}

/**
* ������ ���������
*
*@param s ������
*@param level ��� ���������
*/
void __fastcall TKLog::Log( UnicodeString s, TKLogLevel level )
{

	CurrentDT     = CurrentDT.CurrentDateTime();
	AnsiString ds = CurrentDT.FormatString("dd.mm.yy hh:nn:ss: ");
	AnsiString s1 = s;
	if( level & Level)
	{
	   if( Console!=NULL )
	   {
		   if( WithDT )Console->Lines->Add(ds+s);
		   else Console->Lines->Add(s);
		   while( Console->Lines->Count > MaxCount )Console->Lines->Delete(0);
	   }
	   if( WithLogFile )
	   {
		   FILE *f = fopen(LogFile.c_str(),"a+");
		   if( s1 == "" )fprintf(f,"\n");
		   else fprintf(f,"%s%s\n",ds.c_str(),s1.c_str());
		   fclose(f);
       }
	}
}

/**
* ������ ���������
*
*@param s ������
*/
void __fastcall TKLog::Message( UnicodeString s ){ Log(s,llMessage); }

/**
* ������ ������
*
*@param s ������
*/
void __fastcall TKLog::Error( UnicodeString s ){ Log(s,llError); }

/**
* ������ �������
*
*@param s ������
*/
void __fastcall TKLog::Debug( UnicodeString s ){ Log(s,llDebug); }

// ������ � �����
/**
* ������ ������ � �����
*
*@param start - ��������� ���������
*@param max - ������������ ���������
*/
void __fastcall TKLog::StartBar(int start,int max)
{
	CurrentDT       = CurrentDT.CurrentDateTime();
	if( Bar == NULL )return;
	Bar->Min     = start;
	Bar->Max     = max;
	Bar->Visible = true;
}

/**
* ���������� ������� ����
*/
void __fastcall TKLog::SetBar(int pos)
{
	CurrentDT       = CurrentDT.CurrentDateTime();
	if( Bar == NULL )return;
	Bar->Position   = pos;
	Application->ProcessMessages();
}

/**
* ��������� ������ � �����
*/
void __fastcall TKLog::StopBar()
{
	CurrentDT       = CurrentDT.CurrentDateTime();
	if( Bar == NULL )return;
	Bar->Visible = false;
}


/**
* ������� �� ��������� ������ �������
*/
void __fastcall TKLog::LevelBoxClick(TObject *Sender)
{
   TCheckListBox *clb = (TCheckListBox *)Sender;
   if( clb == NULL )return;
   if( clb->ItemIndex < 3 )return;
   Level = llNone;
   if( clb->Checked[0] )Level = Level&llError;
   if( clb->Checked[1] )Level = Level&llMessage;
   if( clb->Checked[2] )Level = Level&llDebug;
}

/*****************************************************************************
******* ���������� ������ �� ��������� ***************************************
*****************************************************************************/

/**
* �����������
*
*@param log - ����� ���� �������� ���������
*@param form - ����� ������� ��������� �� ���������
*/
__fastcall TKWatchdogTimer::TKWatchdogTimer(TKLog *log, TForm *form)
   : TThread(false)
{
	Log  = log;
	Form = form;
	DTLastNormal = DTLastNormal.CurrentDateTime();
}


/**
* ������ ������
*/
void __fastcall TKWatchdogTimer::Execute()
{
/*
	Form->
if (!IsWindow(hWnd))
		return SetLastError(ERROR_INVALID_PARAMETER), FALSE;

	DWORD_PTR dwResult;
	if (!SendMessageTimeout(hWnd, WM_NULL, 0, 0,
							SMTO_ABORTIFHUNG|SMTO_BLOCK, 500,
							&dwResult))
		*pbHung = TRUE;

	return TRUE;
*/
// ����������� ����
	for( int i=0; i<100; i++ )
	{
// ��������� ��������� ����
		DWORD_PTR dwResult;
		if(!SendMessageTimeout(Form->Handle, WM_NULL, 0, 0,
							SMTO_ABORTIFHUNG|SMTO_BLOCK, 500,
							&dwResult))
		{
			if( Log != NULL )Log->Error("���� �������" );
		}
		else
		{
			if( Log != NULL )Log->Error("��� �������" );

        }
		TDateTime dt = dt.CurrentDateTime();
		Sleep(10000);

	}
	Log->Message("���� ���!");
	Application->Terminate();
	TerminateProcess(GetCurrentProcess(), 0);
}
