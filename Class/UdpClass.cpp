#include <vcl.h>
#include <locale.h>
#include "UdpClass.h"

/******************************************************************************
* Методы класса сетевого взаимодействия UODCONTROL
******************************************************************************/


/**
* Конструктор класса
*
*@param Udp - TIdUDPServer * или TIdUDPClient *
*/
__fastcall TKUdpClient::TKUdpClient(  TIdUDPClient *Udp): TObject()
{
// Проверяем, является ли класс сервером или клиентом
   Clients               = new TList();
   UdpClient             = Udp;
// Парсим INI-файл
   Udp->Active           = false;
   ((TIdUDPClient *)Udp)->BufferSize  = 131072;
   RetStrings = new TStringList();
}

/**
* Конструктор класса
*
*/
__fastcall TKUdpClient::TKUdpClient(): TObject()
{
   TIdUDPClient *Udp = new TIdUDPClient(Application);
// Проверяем, является ли класс сервером или клиентом
   Clients               = new TList();
   UdpClient             = Udp;
// Парсим INI-файл
   Udp->Active           = false;
   ((TIdUDPClient *)Udp)->BufferSize  = 131072;
   RetStrings = new TStringList();
}



/**
* Добавить клиента в список
*/
void __fastcall TKUdpClient::AddClient(int port, UnicodeString ip, int timeout)
{
   try
   {
// Проверка клиента в списке
      TKUdpClientItem *up = NULL;
      for(int i=0; i<Clients->Count; i++ )
      {
		 up = (TKUdpClientItem *)Clients->Items[i];
         if( up->Port == port && up->Host == ip )return;
      }
// Добавляем клиента в список
	  up = new TKUdpClientItem;
      up->Port     = port;
      up->Host     = ip;
      up->TimeOut  = timeout;
      up->LastTime = 0;
	  up->Enabled  = true;
      Clients->Add((void *)up);
   }
   catch(...)
   {
   }
}

/**
* Послать команду серверу
*
*@param num - номер UDP клиента
*@param s - команда
*@param ret_flag - true если требует возврата и false если не требует
*@return - true если строка вернулась
*/
bool __fastcall TKUdpClient::Send(int num, UnicodeString s, bool ret_flag )
{
   bool ret = false;
   try
   {
	 if( num <0 || num >= Clients->Count )return ret;
	 //TIdTextEncoding *code;
	 TKUdpClientItem *up = (TKUdpClientItem *)Clients->Items[num];
	 UdpClient->Port = up->Port;
	 UdpClient->Host = up->Host;
	 UdpClient->Active = true;


	 UdpClient->Send(s, IndyTextEncoding_OSDefault());
	 if( ret_flag )
	 {
//		code->u
//		TEncoding code;
//		TIdBytes ret_buf;
//		int nn = UdpClient->ReceiveBuffer(ret_buf,up->TimeOut);
//		RetString = BytesToStringRaw(ret_buf);
		RetString = UdpClient->ReceiveString(up->TimeOut, IndyTextEncoding_OSDefault());
		Parse();
	 }
	 UdpClient->Active        = false;
	 ret = true;
   }
   catch(...)
   {
   }
   return ret;
}


/**
* Послать команду всем соедтнениям
*
*@param s - команда
*/
void __fastcall TKUdpClient::SendAll(UnicodeString s)
{
   try
   {
	 for( int i=0; i <  Clients->Count; i++)
		 Send(i,s,false);
	}
   catch(...)
   {
   }
}


/**
* Проверить что соединения живыве
*/
void __fastcall TKUdpClient::CheckConnects()
{
   try
   {
	  for( int i=Clients->Count-1; i>=0; i-- )
      {
		 TKUdpClientItem *up = (TKUdpClientItem *)Clients->Items[i];
		 UdpClient->Port = up->Port;
		 UdpClient->Host = up->Host;
		 UdpClient->Active = true;
         int to = 1000;
         bool flag = false;
// Попытка коннеткта
         UnicodeString s;
         try
         {
			UdpClient->Send("PING");
			s = UdpClient->ReceiveString(to);
         }
         catch(...){ flag = true; }
         if( !flag && s!= "OK" )flag = true;
 // Соединение потеряно
         if( flag )
         {
            delete up;
			Clients->Delete(i);
         }//end if
      }//end for i
   }
   catch(...)
   {
   }
}

/**
* Распарсить результат
*/
void __fastcall TKUdpClient::Parse()
{
   try
   {
	   RetStrings->Clear();
	   RetStrings->Delimiter = ';';
	   RetStrings->DelimitedText = RetString;
   }
   catch(...)
   {
	   ;
   }
}

/******************************************************************************
******** Класс TKGuardClient **************************************************
******************************************************************************/

/**
* Клнструктор
*
*@param host - IP-адрес или имя Guard сервера
*@param port - UDP порт
*@param tm   - минимальный интервал в секундах между посылками пакета
*@param mode - что делать, если процесс завис
*/
__fastcall TKGuardClient::TKGuardClient(UnicodeString host, int port, int tm, TKGuardToDoMode mode )
{
	UdpGuard = new TIdUDPClient( Application );
	UdpGuard->Host = host;
	UdpGuard->Port = port;
	TMRequest      = tm;
	Mode           = mode;
	CheckCmd       = "";
	Pid            = GetCurrentProcessId();
// Пр
	ExeFullName    = Application->ExeName;
	for( int i=1; i<=ParamCount(); i++ )
	{
	   ExeFullName = ExeFullName+" "+ParamStr(i);
//	   int n=i;
	}
//ExtractFilePath(Application->ExeName);
	ExeName        = ExtractFileName(Application->ExeName);
	DTLastRequest  = 0;
	Send("HI");
}

/**
* Конструктор с INI-файлом
*
*@param i_file - INI-файл
*@param section - секция INI-файла
*/
__fastcall TKGuardClient::TKGuardClient(TIniFile *IniFile, UnicodeString Section)
{
	UdpGuard = new TIdUDPClient( Application );
	UdpGuard->Host = IniFile->ReadString("AsdGuard","Host", "localhost");;
	UdpGuard->Port = IniFile->ReadInteger("AsdGuard","Port", 20013);
	TMRequest      = IniFile->ReadInteger("AsdGuard","IntervalSend", 5);
	Mode           = (TKGuardToDoMode)IniFile->ReadInteger(Section,"GuardMode", (int)gmTerminate);
	CheckCmd       = IniFile->ReadString(Section,"GuardCkeckCmd", "");;
	Pid            = GetCurrentProcessId();
// Пр
	ExeFullName    = Application->ExeName;
	for( int i=1; i<=ParamCount(); i++ )
	{
	   ExeFullName = ExeFullName+" "+ParamStr(i);
//	   int n=i;
	}
//ExtractFilePath(Application->ExeName);
	ExeName        = ExtractFileName(Application->ExeName);
	DTLastRequest  = 0;
	Send("HI");
}

/**
* Конструктор с INI-файлом
*
*@param i_file - INI-файл
*@param section - секция INI-файла
*/
__fastcall TKGuardClient::TKGuardClient(TIniFile *IniFile, UnicodeString Section, bool flag)
{
	UdpGuard = new TIdUDPClient( Application );
	UdpGuard->Host = IniFile->ReadString("AsdGuard","Host", "localhost");;
	UdpGuard->Port = IniFile->ReadInteger("AsdGuard","Port", 20013);
	TMRequest      = IniFile->ReadInteger("AsdGuard","IntervalSend", 5);
	Mode           = (TKGuardToDoMode)IniFile->ReadInteger(Section,"GuardMode", (int)gmTerminate);
	Pid            = GetCurrentProcessId();
// Пр
	ExeFullName    = Application->ExeName;
	for( int i=1; i<=ParamCount(); i++ )
	{
	   ExeFullName = ExeFullName+" "+ParamStr(i);
//	   int n=i;
	}
//ExtractFilePath(Application->ExeName);
	ExeName        = ExtractFileName(Application->ExeName);
	DTLastRequest  = 0;
	if( flag )Send("HI");
}



 /**
 * Функция посылка что все нормально
 */
 void __fastcall TKGuardClient::Send(UnicodeString cmd)
 {
	 try
	 {

		 TDateTime dt = dt.CurrentDateTime();
		 if(SecondsBetween( dt, DTLastRequest ) <= TMRequest )return;
		 DTLastRequest = dt;
		 UdpGuard->Connect();
		 UnicodeString s = cmd+";"+
			 IntToStr((int)Pid)+";"+
			 ExeName+";\""+
			 ExeFullName+"\";"+
			 IntToStr((int)Mode);
		 if( CheckCmd != "" )s = s+";"+CheckCmd;
		 UdpGuard->Send(s);
		 UdpGuard->Disconnect();
	 }
	 catch(...)
	 {
		 ;
     }
 }


 /**
 * Функция посылка что все нормально
 */
 void __fastcall TKGuardClient::OK()  { Send("OK");  }
 void __fastcall TKGuardClient::Bay() { Send("BAY"); }
 void __fastcall TKGuardClient::Restart() { Send("RESTART"); }





