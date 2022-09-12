#include "Funcs.h"

const count_digit = 3;
UnicodeString Mas[10];
UnicodeString MasCh[10];
UnicodeString MasD[10];


/**
* Декодирование строки
*/
UnicodeString __fastcall DecodeStr(UnicodeString str)
{
  Mas[0] = "$45";     MasCh[0] = "A";     MasD[0] = "0";
  Mas[1] = "$57";     MasCh[1] = "D";     MasD[1] = "1";
  Mas[2] = "$D4";     MasCh[2] = "B";     MasD[2] = "2";
  Mas[3] = "$2";      MasCh[3] = "L";     MasD[3] = "3";
  Mas[4] = "$1C";     MasCh[4] = "P";     MasD[4] = "4";
  Mas[5] = "$77";     MasCh[5] = "X";     MasD[5] = "5";
  Mas[6] = "$EE";     MasCh[6] = "Z";     MasD[6] = "6";
  Mas[7] = "$A5";     MasCh[7] = "W";     MasD[7] = "7";
  Mas[8] = "$F3";     MasCh[8] = "Q";     MasD[8] = "8";
  Mas[9] = "$7";      MasCh[9] = "R";     MasD[9] = "9";

  UnicodeString result = "";
  char ch;
  UnicodeString str_tmp = str;
  UnicodeString tmp;
  int i = 1;
  while (str_tmp.Length()>0)
   {
    tmp = str_tmp.SubString(1,count_digit);
    tmp = ReverseChar(tmp);
    int a,c,b;
    try
     {
      b = StrToInt(tmp);
      }
    catch (...)
     {
      b = 0;
      }
    int j = i - int(i / 10)*10;
    a = StrToInt(Mas[j]);
    c = b - a;
    ch = c;
    result = result + ch;
    str_tmp = str_tmp.SubString(count_digit+1,str_tmp.Length());
    i++;
    }
  return result;
}

/**
* Служебная функция для кодирования - декодирования
*/
UnicodeString __fastcall ReverseDigit(UnicodeString str)
{
  UnicodeString result = "";
  int k;
  for (int i=1; i<=str.Length(); i++)
   {
    k = -1;
    for (int j=0; j<10; j++)
     {
      if (UnicodeString(str[i])==MasD[j])
       {
        k = j;
        break;
        }
      }
    if (k>=0)
     {
      result = result + MasCh[k];
      } else {
      result = result + str[i];
      }
    }
  return result;
}

/**
* Служебная функция для кодирования - декодирования
*/
UnicodeString __fastcall ReverseChar(UnicodeString str)
{
  UnicodeString result = "";
  int k;
  for (int i=1; i<=str.Length(); i++)
   {
    k = -1;
    for (int j=0; j<10; j++)
     {
      if (UnicodeString(str[i])==MasCh[j])
       {
        k = j;
        break;
        }
      }
    if (k>=0)
     {
      result = result + MasD[k];
      } else {
      result = result + str[i];
      }
    }
  return result;
}

/**
* Преобразовать число к n разрядному числу в виде строки
*/
UnicodeString __fastcall FullN(int a,int n)
{
  UnicodeString result = "";
  result = IntToStr(a);
  UnicodeString tmp;
  if (result.Length()<n)
   {
    tmp = result;
    for (int i=1; i<=(n-tmp.Length());i++)
     {
      tmp = "0" + tmp;
      }
    result = tmp;
    }
  return result;
}


int RGBtoColor(short r,short b,short g)
{
   return (int)(g<<16|b<<8|r);
}

UnicodeString ColorToRGB(int color)
{
   int r = color&0xff;
   int g = (color>>8)&0xff;
   int b = (color>>16)&0xff;

   return IntToStr(r)+","+IntToStr(g)+","+IntToStr(b);
}

int RGBStringToColor( UnicodeString s)
{
   if( s == "" )return clBlack;
   TStringList *sl = new TStringList();
   sl->Delimiter=',';
   sl->DelimitedText = s;
   short r=0,b=0,g=0;
   if( sl->Count > 0 )r = StrToIntDef(sl->Strings[0],0);
   if( sl->Count > 1 )g = StrToIntDef(sl->Strings[1],0);
   if( sl->Count > 2 )b = StrToIntDef(sl->Strings[2],0);
   return RGBtoColor(r,g,b);
}

/**
* Запуск внешней команды
*
*@param Cmd - имя команды
*/
void __fastcall _Exec(UnicodeString Cmd )
{
   try
   {
//      UnicodeString Cmd
	  UnicodeString FullName = Cmd;

	  TStringList *l = new TStringList();
	  l->Delimiter = ' ';
	  l->DelimitedText = FullName;
	  UnicodeString ArgName = "";
	  FullName = l->Text;
	  if( l->Count > 0 )
	  {
		 UnicodeString CmdName  = l->Strings[0];
		 UnicodeString PathName = ExtractFilePath(l->Strings[0]);
//         UnicodeString FullName
		 for( int i=1; i<l->Count; i++ )
		 {
			ArgName = ArgName+" "+l->Strings[i];
		 }
		 ShellExecute(NULL,L"open",CmdName.w_str(),ArgName.w_str(),PathName.w_str(),SW_RESTORE);
	  }
	  delete l;
   }
   catch(...)
   {
	  ;
   }

}

/**
* Запуск внешней команды и возврат Hendla процесса
*
*@param Cmd - имя команды
*@return указатель на структуру SHELLEXECUTEINFO
*/
HANDLE __fastcall _ExecEx(UnicodeString Cmd )
{
   HANDLE proc = 0;

   try
   {
//      UnicodeString Cmd
	  UnicodeString FullName = Cmd;

	  TStringList *l = new TStringList();
	  l->Delimiter = ' ';
	  l->DelimitedText = FullName;
	  UnicodeString ArgName = "";
	  FullName = l->Text;
	  if( l->Count > 0 )
	  {
		 UnicodeString CmdName  = l->Strings[0];
		 UnicodeString PathName = ExtractFilePath(l->Strings[0]);
//         UnicodeString FullName
		 for( int i=1; i<l->Count; i++ )
		 {
			ArgName = ArgName+" "+l->Strings[i];
		 }

	  SHELLEXECUTEINFO shellExInfo;

	  shellExInfo.cbSize       = sizeof(SHELLEXECUTEINFO);
	  shellExInfo.fMask        = SEE_MASK_NOCLOSEPROCESS;
	  shellExInfo.hwnd         = NULL;
	  shellExInfo.lpVerb       = L"open";
	  shellExInfo.lpFile       = CmdName.w_str();
	  shellExInfo.lpParameters = ArgName.w_str();
	  shellExInfo.lpDirectory  = PathName.w_str();
	  shellExInfo.nShow        = SW_RESTORE;
	  shellExInfo.hInstApp     = NULL;

	  ShellExecuteEx(&shellExInfo); // start process
	  proc  = shellExInfo.hProcess;

	  }
	  delete l;
   }
   catch(...)
   {
	  ;
   }
   return proc;

}


/**
* Проверка запускаемого файла
*
*@param Cmd - имя команды
*/
bool __fastcall _CheckExeFile(UnicodeString Cmd )
{
   try
   {
	  bool ret = false;
	  UnicodeString dir = GetCurrentDir();
	  TStringList *l = new TStringList();
	  l->Delimiter = ' ';
	  l->DelimitedText = Cmd;
	  if( l->Count > 0 )
	  {
		 ret = FileExists(l->Strings[0]);
	  }
	  delete l;
	  return ret;
   }
   catch(...)
   {
	  ;
   }

}

/**
* Извлекаем имя файла из команды
*
*@param Cmd - имя команды
*/
UnicodeString __fastcall _ExtractFileName(UnicodeString Cmd )
{
   try
   {
	  UnicodeString s = "";
	  TStringList *l = new TStringList();
	  l->Delimiter = ' ';
	  l->DelimitedText = Cmd;
	  if( l->Count > 0 )s = l->Strings[0];
	  delete l;
	  return s;
   }
   catch(...)
   {
	  ;
   }

}




/**
* Проверка что такая команда уже запущена
*
*@param Cmd - имя команды
*/
bool __fastcall _CheckProcess(UnicodeString Cmd )
{
   try
   {
	  bool ret = false;
	  UnicodeString file = _ExtractFileName( Cmd ).UpperCase();
// Перебираем список процессов
	  TProcessEntry32 p;
	  p.dwSize = sizeof(Winapi::Tlhelp32::PROCESSENTRY32);
	  HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

	  for (bool loop = Process32First(h, &p); loop; loop = Process32Next(h, &p))   {
		 UnicodeString fname = UnicodeString(p.szExeFile).Trim().UpperCase();
		 if( file == fname ){
			ret = true;
			break;
		 }
	  }
	  return ret;
   }
   catch(...)
   {
	  ;
   }

}


/**
* Проверка, запущен ли процесс с таким же именем как текущий
*/
bool __fastcall _CheckProcess()
{
   bool flag = false;
   UnicodeString fname = "";
// Находим PID текущего процесса
   int pid1  = GetCurrentProcessId();

// Перебираем список процессов
   TProcessEntry32 p;
   p.dwSize = sizeof(Winapi::Tlhelp32::PROCESSENTRY32);
   HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

   for (bool loop = Process32First(h, &p); loop; loop = Process32Next(h, &p))
   {
// Если PID процесса совпадает с текущим
	  if( pid1 == p.th32ProcessID )
	  {
		 fname = UnicodeString(p.szExeFile).Trim().UpperCase();
		 break;
	  }
   }

// Снова перебираем список процесса
   for (bool loop = Process32First(h, &p); loop; loop = Process32Next(h, &p))
   {
      UnicodeString fname1 = UnicodeString(p.szExeFile).Trim().UpperCase();
//      Memo1->Lines->Add(fname1+" "+fname+" "+IntToStr(pid1)+" "+IntToStr(p.th32ProcessID));
// Если имя процесса совпадает, а PID не совпадает
      if( fname == fname1 && pid1 != p.th32ProcessID )
      {
         flag = true;
//         Memo1->Lines->Add("Совпало!");
         break;
      }
   }
   return flag;
}

/**
* Проверка, запущен ли процесс с таким же идентификатором как текущий
*/
bool __fastcall _IsExistProcess(int pid)
{
   bool flag = false;

// Перебираем список процессов
   TProcessEntry32 p;
   p.dwSize = sizeof(Winapi::Tlhelp32::PROCESSENTRY32);
   HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

   for (bool loop = Process32First(h, &p); loop; loop = Process32Next(h, &p))
   {
// Если PID процесса совпадает с текущим
	  if( pid == p.th32ProcessID ){ flag = true; break; }
   }
   return flag;
}



/**
* Закрываем указанный процесс
*
*@param Cmd - имя команды
*/
void __fastcall _KillProcess(UnicodeString Cmd )
{
   try
   {
	  UnicodeString FullName = Cmd;

	  TStringList *l = new TStringList();
	  l->Delimiter = ' ';
	  l->DelimitedText = FullName;
/*
	  for( int i=0; i<l->Count; i++ )
	  {
		 if( l->Strings[i] == "<user>" )l->Strings[i] = Global1->User->Login;
		 if( l->Strings[i] == "<passwd>" )l->Strings[i] = Global1->User->Password;
	  }
*/
	  FullName = l->Text;
	  if( l->Count > 0 )
	  {
		 UnicodeString CmdName  = ExtractFileName(l->Strings[0]);
		 CmdName             = CmdName.UpperCase();
// Перебираем список процессов
		TProcessEntry32 p;
        p.dwSize = sizeof(Winapi::Tlhelp32::PROCESSENTRY32);
        HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

        for (bool loop = Process32First(h, &p); loop; loop = Process32Next(h, &p))
		{
// Если PID процесса совпадает с текущим
            UnicodeString fname1 = UnicodeString(p.szExeFile).Trim().UpperCase();
			if( CmdName == fname1 )
			{
			   HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, p.th32ProcessID);
			   TerminateProcess(hProcess ,0);

			}

   }
	  }
//      delete l;
   }
   catch(...)
   {
	  ;
   }

}


/**
* Закрываем указанный процесс
*
*@param Cmd - имя команды
*/
void __fastcall _KillProcess1(UnicodeString Cmd )
{
   try
   {
	  UnicodeString FullName = Cmd;

	  int pid1  = GetCurrentProcessId();

	  TStringList *l = new TStringList();
	  l->Delimiter = ' ';
	  l->DelimitedText = FullName;
/*
	  for( int i=0; i<l->Count; i++ )
	  {
		 if( l->Strings[i] == "<user>" )l->Strings[i] = Global1->User->Login;
		 if( l->Strings[i] == "<passwd>" )l->Strings[i] = Global1->User->Password;
	  }
*/
	  FullName = l->Text;
	  if( l->Count > 0 )
	  {
		 UnicodeString CmdName  = ExtractFileName(l->Strings[0]);
		 CmdName             = CmdName.UpperCase();
// Перебираем список процессов
		TProcessEntry32 p;
        p.dwSize = sizeof(Winapi::Tlhelp32::PROCESSENTRY32);
        HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

        for (bool loop = Process32First(h, &p); loop; loop = Process32Next(h, &p))
		{
// Если PID процесса совпадает с текущим
            UnicodeString fname1 = UnicodeString(p.szExeFile).Trim().UpperCase();
			if( CmdName == fname1 )
			{
			   if (pid1 != p.th32ProcessID)
			   {
			   HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, p.th32ProcessID);
			   TerminateProcess(hProcess ,0);
               }
			}

   }
	  }
//      delete l;
   }
   catch(...)
   {
	  ;
   }

}


/**
* Получения значения атрибута по названию
*
* @param node XML нода, где нужно искать атрибут
* @param attr имя атрибута
* @param def значения атрибута, если ничего не найдено или атрибут нулевой
* @return возвращается значение атрибута
*/
UnicodeString __fastcall XMLNodeAttribute( _di_IXMLNode node,UnicodeString attr,UnicodeString def )
{
   UnicodeString var = node->Attributes[WideString(attr)];
   if( var != "" )return var;
   else return def;
}

/**
* Получения значения атрибута по названию
*
* @param node XML нода, где нужно искать атрибут
* @param attr имя атрибута
* @param def значения атрибута, если ничего не найдено или атрибут нулевой
* @return возвращается значение атрибута
*/
int __fastcall XMLNodeIntAttribute( _di_IXMLNode node,UnicodeString attr, int def )
{
   int ret;
   UnicodeString var = node->Attributes[WideString(attr)];
   if( var!="" )ret = StrToIntDef( UnicodeString( var ), def );
   else ret = def;
   return ret;
}


//      TerminateProcess(GetCurrentProcess(), 0);

