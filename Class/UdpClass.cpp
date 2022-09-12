#include <vcl.h>
#include <locale.h>
#include "UdpClass.h"

/******************************************************************************
* ������ ������ �������� �������������� UODCONTROL
******************************************************************************/


/**
* ����������� ������
*
*@param Udp - TIdUDPServer * ��� TIdUDPClient *
*/
__fastcall TKUdpClient::TKUdpClient(  TIdUDPClient *Udp): TObject()
{
// ���������, �������� �� ����� �������� ��� ��������
   Clients               = new TList();
   UdpClient             = Udp;
// ������ INI-����
   Udp->Active           = false;
   ((TIdUDPClient *)Udp)->BufferSize  = 131072;
   RetStrings = new TStringList();
}

/**
* ����������� ������
*
*/
__fastcall TKUdpClient::TKUdpClient(): TObject()
{
   TIdUDPClient *Udp = new TIdUDPClient(Application);
// ���������, �������� �� ����� �������� ��� ��������
   Clients               = new TList();
   UdpClient             = Udp;
// ������ INI-����
   Udp->Active           = false;
   ((TIdUDPClient *)Udp)->BufferSize  = 131072;
   RetStrings = new TStringList();
}



/**
* �������� ������� � ������
*/
void __fastcall TKUdpClient::AddClient(int port, UnicodeString ip, int timeout)
{
   try
   {
// �������� ������� � ������
      TKUdpClientItem *up = NULL;
      for(int i=0; i<Clients->Count; i++ )
      {
		 up = (TKUdpClientItem *)Clients->Items[i];
         if( up->Port == port && up->Host == ip )return;
      }
// ��������� ������� � ������
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
* ������� ������� �������
*
*@param num - ����� UDP �������
*@param s - �������
*@param ret_flag - true ���� ������� �������� � false ���� �� �������
*@return - true ���� ������ ���������
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
* ������� ������� ���� �����������
*
*@param s - �������
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
* ��������� ��� ���������� ������
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
// ������� ���������
         UnicodeString s;
         try
         {
			UdpClient->Send("PING");
			s = UdpClient->ReceiveString(to);
         }
         catch(...){ flag = true; }
         if( !flag && s!= "OK" )flag = true;
 // ���������� ��������
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
* ���������� ���������
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
******** ����� TKGuardClient **************************************************
******************************************************************************/

/**
* �����������
*
*@param host - IP-����� ��� ��� Guard �������
*@param port - UDP ����
*@param tm   - ����������� �������� � �������� ����� ��������� ������
*@param mode - ��� ������, ���� ������� �����
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
// ��
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
* ����������� � INI-������
*
*@param i_file - INI-����
*@param section - ������ INI-�����
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
// ��
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
* ����������� � INI-������
*
*@param i_file - INI-����
*@param section - ������ INI-�����
*/
__fastcall TKGuardClient::TKGuardClient(TIniFile *IniFile, UnicodeString Section, bool flag)
{
	UdpGuard = new TIdUDPClient( Application );
	UdpGuard->Host = IniFile->ReadString("AsdGuard","Host", "localhost");;
	UdpGuard->Port = IniFile->ReadInteger("AsdGuard","Port", 20013);
	TMRequest      = IniFile->ReadInteger("AsdGuard","IntervalSend", 5);
	Mode           = (TKGuardToDoMode)IniFile->ReadInteger(Section,"GuardMode", (int)gmTerminate);
	Pid            = GetCurrentProcessId();
// ��
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
 * ������� ������� ��� ��� ���������
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
 * ������� ������� ��� ��� ���������
 */
 void __fastcall TKGuardClient::OK()  { Send("OK");  }
 void __fastcall TKGuardClient::Bay() { Send("BAY"); }
 void __fastcall TKGuardClient::Restart() { Send("RESTART"); }





