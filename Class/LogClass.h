//---------------------------------------------------------------------------

#ifndef LogClassH
#define LogClassH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.ComCtrls.hpp>
#include <stdio.h>

#define MAX_COUNT 256

//---------------------------------------------------------------------------
enum TKLogLevel
{
   llNone 	                  = 0x0000,   //Нет сообщений
   llError                    = 0x0001,   //Только ошибки
   llMessage                  = 0x0002,   //Сообщения
   llErrAndMess               = 0x0003,   //Ошибки и сообщения
   llDebug                    = 0x0004,   //Уровень отладки 1
   llErrorAndDebug            = 0x0005,   //Отладка и ошибки
   llMessageAndDebug          = 0x0006,   //Отладка и сообщения
   llErrorAndMessageAndDebug  = 0x0007,   //Отладка, ошибки и сообщения
   llAll                      = 0xffff    // Все
};


class TKLog : public TObject
{
private:
   TMemo           *Console;
   TProgressBar    *Bar;
   TCheckListBox   *LevelBox;
   void __fastcall LevelBoxClick(TObject *Sender);
public:
   TKLogLevel Level;
   int  MaxCount;

   bool WithDT;
   bool WithLogFile;

   AnsiString LogFile;
   TDateTime StartDT;
   TDateTime CurrentDT;
   __fastcall TKLog(TMemo *console, TProgressBar *bar, TCheckListBox *clb);
// Работа с консолью
   void __fastcall Clear();
   void __fastcall SetLevel( TKLogLevel level );
   void __fastcall Log( UnicodeString s, TKLogLevel level );
   void __fastcall Message( UnicodeString s );
   void __fastcall Error( UnicodeString s );
   void __fastcall Debug( UnicodeString s );
// Работа с баром
   void __fastcall StartBar(int start,int max);
   void __fastcall SetBar(int pos);
   void __fastcall StopBar();
};


class TKWatchdogTimer : public TThread
{
private:
//protected:
	void __fastcall Execute();
	TKLog *Log;
	TForm *Form;
	TDateTime DTLastNormal;
public:
	__fastcall TKWatchdogTimer(TKLog *log, TForm *form);
};
#endif
