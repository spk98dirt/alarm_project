#include "class/TkConfig.h"
#include "class/TKInterpreter.h"
#include "../../sem3/application/form/Progress.h"
#include <class/TkGlobal.h>
#include <class/TkListScenario.h>
#include <class/TkJournal.h>
#include <class/TkScenario.h>
#include "MainForm.h"
extern TForm1 *Form1;

#define WITHOUT_SCRIPT 1

bool Flag_message;



bool __fastcall ConnectBase()
{
/*
  bool f = true;
  if (Global1 == NULL)
   {
    AnsiString ini_file = "cbk.ini";
    AnsiString tmp_str = GetValueProgramParam("-config");
    if (!tmp_str.IsEmpty()) ini_file = tmp_str;
    Global1 = new TKGlobal(Application, ini_file );


    bool f = true;

    f = Global1->ConnectToBase();

    if (!f)
     {
      ConnectOracleForm->ShowModal();
      try
       {
        f = ConnectOracleForm->OraSession1->Connected;
        }
      catch (...)
       {
        f = false;
        }
      }
    }
  return f;
*/
}

/**
* Выполнение программы
*/
void __fastcall RunScript1(AnsiString script)
{
  bool f;
  TKInterpreter *interp;

//Запуск интерпретатора
      interp = new TKInterpreter(script);
      interp->Journal = NULL;
      interp->Form = NULL;
//Вместо ifdef
      interp->WithFolderScript = false;

//      FormScript1->ClientSocket1->Host = "localhost";
//      FormScript1->ClientSocket1->Port = 3000;
//      FormScript1->FlagExit = false;
//      interp->DebugSocket = FormScript1->ClientSocket1;
      interp->DebugSocket = NULL;
//      interp->DebugSocket->OnRead = interp->ReadSocket;
//      FormScript1->interp = interp;
      interp->m_main();
      delete interp;
}




/**
* Выполнение программы
*/
void __fastcall RunScript(AnsiString script)
{
  bool f;
  AnsiString user;
  AnsiString pswd;
  AnsiString cpswd;
  AnsiString fd="";
  int id_user;
  AnsiString ini_file;
  AnsiString tmp_str;
  TKInterpreter *interp;
  AnsiString ScriptsNames;
  AnsiString ScriptName;
  TStringList *args;
  ScriptName = script;

//Запуск интерпретатора
      interp = new TKInterpreter(ScriptName);
      interp->Journal = NULL;
      interp->Form = NULL;
//Вместо ifdef
      interp->WithFolderScript = false;

//Флаг Без отладки
      if (!Trim(fd).IsEmpty()) interp->fnodebug = true;

      Form1->FlagExit = false;
      interp->DebugSocket = NULL;
//      interp->DebugSocket->OnRead = interp->ReadSocket;
      interp->m_main();
      delete interp;
}

