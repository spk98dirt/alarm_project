//---------------------------------------------------------------------------

#pragma hdrstop

#include "LogClass.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)



/**
* Конструктор
*
*@param console - консоль
*@param bar     - бар
*@param clb     - чек лист бокс настройки уровня отладки
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
		LevelBox->Items->Add("Ошибки");
		LevelBox->Items->Add("Сообщения");
		LevelBox->Items->Add("Отладка");
		LevelBox->OnClick = LevelBoxClick;
	}
*/
	SetLevel(llError);

	WithDT      = false;
	WithLogFile = false;
	LogFile     = "logfile.txt";
	MaxCount    = MAX_COUNT;
}

// Работа с консолью
/**
* Очстить консоль
*/
void __fastcall TKLog::Clear()
{
    Console->Lines->Clear();
}

/**
*   Установить уровень выдачи сообщений
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
* Выдать сообщение
*
*@param s строка
*@param level тип сообщения
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
* Выдать сообщение
*
*@param s строка
*/
void __fastcall TKLog::Message( UnicodeString s ){ Log(s,llMessage); }

/**
* Выдать ошибку
*
*@param s строка
*/
void __fastcall TKLog::Error( UnicodeString s ){ Log(s,llError); }

/**
* Выдать отладку
*
*@param s строка
*/
void __fastcall TKLog::Debug( UnicodeString s ){ Log(s,llDebug); }

// Работа с баром
/**
* Начать работу с баром
*
*@param start - начальное положение
*@param max - максимальное положение
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
* Установить позициб бара
*/
void __fastcall TKLog::SetBar(int pos)
{
	CurrentDT       = CurrentDT.CurrentDateTime();
	if( Bar == NULL )return;
	Bar->Position   = pos;
	Application->ProcessMessages();
}

/**
* Закончить работу с баром
*/
void __fastcall TKLog::StopBar()
{
	CurrentDT       = CurrentDT.CurrentDateTime();
	if( Bar == NULL )return;
	Bar->Visible = false;
}


/**
* Событие на изменение уровня отлажки
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
******* Сторожевой таймер на зависание ***************************************
*****************************************************************************/

/**
* Конструктор
*
*@param log - Класс куда выводить сообщения
*@param form - форма которую проверять на зависание
*/
__fastcall TKWatchdogTimer::TKWatchdogTimer(TKLog *log, TForm *form)
   : TThread(false)
{
	Log  = log;
	Form = form;
	DTLastNormal = DTLastNormal.CurrentDateTime();
}


/**
* Запуск потока
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
// Безконечный цикл
	for( int i=0; i<100; i++ )
	{
// Проверяем состояние окна
		DWORD_PTR dwResult;
		if(!SendMessageTimeout(Form->Handle, WM_NULL, 0, 0,
							SMTO_ABORTIFHUNG|SMTO_BLOCK, 500,
							&dwResult))
		{
			if( Log != NULL )Log->Error("Окно зависло" );
		}
		else
		{
			if( Log != NULL )Log->Error("Все фурычит" );

        }
		TDateTime dt = dt.CurrentDateTime();
		Sleep(10000);

	}
	Log->Message("Хана мне!");
	Application->Terminate();
	TerminateProcess(GetCurrentProcess(), 0);
}
