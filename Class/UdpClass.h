//---------------------------------------------------------------------------

#ifndef UdpClassH
#define UdpClassH

#include <Classes.hpp>
#include <Controls.hpp>
#include <stdarg.h>
#include <inifiles.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPClient.hpp>
#include <IdUDPServer.hpp>
#include <System.DateUtils.hpp>




struct TKUdpClientItem
{
   int        Port;
   UnicodeString Host;
   int        TimeOut;
   TDateTime  LastTime;
   bool       IsReturn;
   bool       Enabled;
   UnicodeString RetString;
   int        Count;
};


/**
* ����� ��� ������ ����� ASD-�������� � �������� �� ����
*/
class TKUdpClient: public TObject
{
private:
   TIdUDPClient *UdpClient;
public:
   bool Status;
   TList *Clients;
   UnicodeString RetString;
   TStringList *RetStrings;
// �����������
   __fastcall TKUdpClient( TIdUDPClient *Udp);
   __fastcall TKUdpClient();
   void __fastcall AddClient(int port,UnicodeString ip,int ta);
   void __fastcall CheckConnects();
// �������� ������� ������ �������
   bool __fastcall Send(int num, UnicodeString s, bool ret );
   void __fastcall SendAll(UnicodeString s);
   void __fastcall Parse();
};

enum TKGuardToDoMode
{
   gmNone       = 0,     // ������ �� ������
   gmLogOnly	= 1,     // ������ �����������
   gmTerminate  = 2,     // ��������� ������� ����� ��������
   gmRestart    = 3      // ��������� ������� � ��������� ������
};

class TKGuardClient: public TObject
{
private:
   TIdUDPClient    *UdpGuard;
   int             TMRequest;
   UnicodeString   ExeName;
   UnicodeString   ExeFullName;
   DWORD           Pid;
   TKGuardToDoMode Mode;
   TDateTime       DTLastRequest;
   UnicodeString   CheckCmd;
   void __fastcall Send(UnicodeString cmd);
public:
   __fastcall TKGuardClient(UnicodeString host, int port, int tm, TKGuardToDoMode mode );
   __fastcall TKGuardClient(TIniFile *i_file, UnicodeString section );
   __fastcall TKGuardClient(TIniFile *i_file, UnicodeString section, bool flag );
   void __fastcall OK();
   void __fastcall Bay();
   void __fastcall Restart();
};

#endif
