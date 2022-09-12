//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SemClass.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

/*****************************************************************************
* ����� TKGlobalSem
*****************************************************************************/

/**
* ����������� ������ TKGlobalSem
*
* @param connection - ���������� � �� ������� ����� ������� � ����������
*/
__fastcall TKGlobalSem::TKGlobalSem(TUniConnection *connection )
{
    // ��������� ������������� ��������� ����������
    count_digit           = 3;
    UnicodeString file    = "";
	UserSem               = "";
	PasswordSem           = "";
	Journals              = new TKJournalSemList();
	Connection            = connection;
	Connection->Connected = false;

    // �������� ������ ����������
	int n                 = ParamCount();
    for( int i=1; i<=n; i++ )
    {
        UnicodeString s = ParamStr(i);
        if( s.Pos("-config=") == 1 )file    = GetCurrentDir()+"\\"+s.SubString(9,s.Length()-8);
        if( s == "-u" && i < n )UserSem     = ParamStr(i+1);
        if( s == "-p" && i < n )PasswordSem = ParamStr(i+1);
    }
    if( !FileExists(file) )
        throw Exception("�� ������ ���� ������������ "+file);

	IniFile = new TMemIniFile(file);

    // ������ ��������� �������
	UnicodeString s_user1 = IniFile->ReadString("Connection","UsernameOracle", "ellips");
	  //	UnicodeString s_pass1 = IniFile->ReadString("Connection","PasswordOracle", "ellips");

    UnicodeString s_pass1 = IniFile->ReadString("Connection","CryptPasswordOracle", "");
    if( s_pass1 == "" )
        s_pass1 = IniFile->ReadString("Connection","PasswordOracle", "ellips");
    else
        s_pass1 = DecodeStr( s_pass1 );

    UnicodeString s_host1 = IniFile->ReadString("Connection","HostOracle",     "localhost");
    UnicodeString s_port1 = IniFile->ReadString("Connection","PortOracle",     "1521");
    UnicodeString s_base1 = IniFile->ReadString("Connection","BaseOracle",     "xe");
    UnicodeString s_schm1 = IniFile->ReadString("Connection","SchemaDatabase", "");

    // ������� ����������� � ORACLE
    Connection->ProviderName = "Oracle";
    Connection->Port         = StrToIntDef(s_port1,1521);
    Connection->Username     = s_user1;
    Connection->Password     = s_pass1;
    Connection->Database     = s_base1;
    Connection->Server       = s_host1+":"+s_port1+":"+s_base1;

	    /*
		UnicodeString st = "����: "+GetCurrentDir()+"\\"+file+"\n���������: "+"Oracle\n����: "+s_port1+"\n������������: "+s_user1+"\n������: "+s_pass1+"\n����:����:���� "+s_host1+" : "+s_port1+" : "+s_base1;
		ShowMessage(st);
		*/
		try {
		   Connection->PerformConnect(true);
		   }
		catch(...){
		}
/*
		if( !Connection->Connected )
		{
			 throw Exception("�� ���� ������������ � �� "+file);
		}
*/
// ����������� ����������� � ��
		Query = new TUniQuery(Application);
		Query->Connection = Connection;
		if( Connection->Connected ){

		   Query->SQL->Clear();
		   Query->SQL->Add("ALTER SESSION SET NLS_NUMERIC_CHARACTERS = ', '");
		   Query->ExecSQL();
		   Query->Close();
//������ ������� ����� �� ��������� ����� ELLIPS
		   Query->SQL->Clear();
		   if( s_schm1 != "" ) 	{
			  UnicodeString str_temp = "ALTER SESSION SET CURRENT_SCHEMA=" + s_schm1;
			  Query->SQL->Add(str_temp);
			  Query->ExecSQL();
			  Query->Close();
		   }
//	}
		   Statistic = new TKPerfStatistic();
		}
}


bool __fastcall TKGlobalSem::CheckConnect()
{
	if( !Connection->Connected )
	{
		try {
		   Connection->PerformConnect(true);
		}
		catch(...){}

	}
	return Connection->Connected;
}

UnicodeString __fastcall TKGlobalSem::ReadProperty(UnicodeString Selection, UnicodeString name, UnicodeString value)
{
   UnicodeString ret;
   if( CheckOfExistsTable("j_param_asd") )
   {
	  Query->SQL->Clear();
	  Query->SQL->Add("SELECT * FROM j_param_asd WHERE v_razdelasd=:a1 AND v_paramasd=:a2");
	  Query->Params->Items[0]->AsString = Selection;
	  Query->Params->Items[1]->AsString = name;
	  Query->Open();
	  if( !Query->Eof )
	  {
		 ret = Query->FieldByName("v_valueasd")->AsString;
		 Query->Close();
		 return ret;
	  }
   }
   ret = IniFile->ReadString(Selection,name,value);
   return ret;
}

void __fastcall TKGlobalSem::SetProperties(UnicodeString Selection, TForm *Form1)
{
    if (ReadProperty(Selection,"AllScreen", "0") == "1")
	{
		ShowWindow(Form1->Handle,SW_SHOWMINIMIZED);
	  	ShowWindow(Form1->Handle,SW_SHOWMAXIMIZED);
//Form1->WindowState = wsMaximized;
	}
	else
	{
		if (ReadProperty(Selection,"FormHeight", "0") != "0")
		{
			Form1->Height = StrToInt(ReadProperty(Selection,"FormHeight", "0"));
		}
		if (ReadProperty(Selection,"FormWidth", "0") != "0")
		{
			Form1->Width = StrToInt(ReadProperty(Selection,"FormWidth", "0"));
		}
		Form1->Top  = (Screen->Height - Form1->Height) * StrToInt(ReadProperty(Selection,"CenterY","50"))/100;
		Form1->Left = (Screen->Width - Form1->Width) * StrToInt(ReadProperty(Selection,"CenterX", "50"))/100;
	}

	Form1->Font->Size = StrToInt(ReadProperty(Selection,"FontSize", "10"));// IniFile->ReadInteger(Selection,"FontSize", 10);
	Form1->Font->Name = ReadProperty(Selection,"FontName","Arial");//IniFile->ReadString(Selection,"FontName","Arial");
	Form1->Caption = ReadProperty(Selection,"Caption","");//IniFile->ReadString(Selection,"Caption","");

	if (ReadProperty(Selection,"FormStyle", "0") == "1")
	{
		Form1->FormStyle = fsStayOnTop;
	}
	else
	{
		Form1->FormStyle = fsNormal;
	}
	if (ReadProperty(Selection,"Icon", "") != "")
	{
		Form1->Icon->LoadFromFile(ReadProperty(Selection,"Icon", ""));
	}
	/*

this.WindowState = FormWindowState.Maximized;

# �� ���� �����
AllScreen=0
	*/
}

/**
* ���������� ���������� ������ ����� sequence
*
* @param seq - ��� ����������
*/
int __fastcall TKGlobalSem::AutoNum(  AnsiString seq )
{
	int num = -1;
	if (!seq.IsEmpty())
	{
// ���� ��������
		Query->Close();
		Query->SQL->Clear();
		Query->SQL->Add("SELECT "+seq+".NEXTVAL FROM dual");
		Query->Open();
		if(!Query->Eof)num = Query->Fields->Fields[0]->AsInteger;
		Query->Close();
	}
	return num;
}


/**
* �������� ������������� �������
*
* @param tab_name - ��� �������
* @return - true ���� ����������
*/
bool __fastcall TKGlobalSem::CheckOfExistsTable( UnicodeString tab_name )
{
	bool flag = false;
	Query->Close();
	Query->SQL->Clear();
	Query->SQL->Add("SELECT * FROM user_tables WHERE table_name = :a1");
	Query->Params->Items[0]->AsString = tab_name.UpperCase();
	Query->Open();
	if( !Query->Eof )flag = true;
	Query->Close();
	return flag;
}

/**
* �������� ������������� ������� � �������
*
* @param tab_name - ��� �������
* @param col_name - ��� �������
* @return - true ���� ����������
*/
bool __fastcall TKGlobalSem::CheckOfExistsColumn( UnicodeString tab_name,UnicodeString col_name )
{
	bool flag = false;
	Query->Close();
	Query->SQL->Clear();
	Query->SQL->Add("SELECT * FROM user_tab_columns WHERE table_name = :a1 AND column_name=:a2");
	Query->Params->Items[0]->AsString = tab_name.UpperCase();
	Query->Params->Items[1]->AsString = col_name.UpperCase();
	Query->Open();
	if( !Query->Eof ){
//	   int n = Query->Fields->Fields[0]->AsInteger;
//	   UnicodeString s = Query->FieldByName("column_name")->AsString;
	   flag = true;
    }
	Query->Close();
	return flag;
}


/**
* �������� �������, ���� �� ����������
*
* @param tab_name - ��� �������
* @return - true ���� ������� ����� �������
*/
bool __fastcall TKGlobalSem::CreateTable( UnicodeString tab_name, TStringList *fields )
{
	if( CheckOfExistsTable(tab_name) )return false;
	Query->Close();
	Query->SQL->Clear();
	Query->SQL->Add("CREATE TABLE "+tab_name+" (");
	for (int i=0; i<(fields->Count-1); i++) Query->SQL->Add(fields->Strings[i]+",");
	Query->SQL->Add(fields->Strings[fields->Count-1]);
	Query->SQL->Add(")");
	Query->ExecSQL();
	return true;
}


/**
* ���������� ������� � ������� � ��������� �������������
*
* @param tab_name - ��� �������
* @param col_name - ��� �������
* @param col_str - ��� ������ � ������ ��������������
* @return - true ���� ������� ����� �������
*/
bool __fastcall TKGlobalSem::AddColumnToTable( UnicodeString tab_name,UnicodeString col_name, UnicodeString col_str )
{
	if( CheckOfExistsColumn(tab_name,col_name) )return false;
	Query->Close();
	Query->SQL->Clear();
	Query->SQL->Add("ALTER TABLE "+tab_name+" ADD "+col_name+" "+col_str);
	Query->ExecSQL();
	return true;
}

/**
* �������� ������������� �����������������
*
* @param seq_name - ��� ����������
* @return - true ���� ����������
*/
bool __fastcall TKGlobalSem::CheckOfExistsSequence( UnicodeString seq_name )
{
	bool flag = false;
	Query->Close();
	Query->SQL->Clear();
	Query->SQL->Add("SELECT * FROM user_sequences WHERE sequence_name = :a1");
	Query->Params->Items[0]->AsString = seq_name.UpperCase();
	Query->Open();
	if( !Query->Eof )flag = true;
	Query->Close();
	return flag;
}

/**
* �������� ����������
*
* @param seq_name - ��� ����������
* @return - true ���� ������ ����� ���������
*/
bool __fastcall TKGlobalSem::CreateSequence( UnicodeString seq_name )
{
	if( CheckOfExistsSequence(seq_name) )return false;
	Query->Close();
	Query->SQL->Clear();
	Query->SQL->Add("CREATE SEQUENCE "+seq_name+" START WITH 1 INCREMENT BY 1 NOCYCLE NOCACHE ORDER");
	Query->ExecSQL();
	return true;
}

/**
* �������� ������������� �������
*
* @param index_name - ��� ����������
* @return - true ���� ����������
*/
bool __fastcall TKGlobalSem::CheckOfExistsIndex( UnicodeString index_name )
{
	bool flag = false;
	Query->Close();
	Query->SQL->Clear();
	Query->SQL->Add("SELECT * FROM user_indexes WHERE index_name = :a1");
	Query->Params->Items[0]->AsString = index_name.UpperCase();
	Query->Open();
	if( !Query->Eof )flag = true;
	Query->Close();
	return flag;
}

/**
* �������� �������
*
*/
bool __fastcall TKGlobalSem::CreateIndex( UnicodeString tab_name,UnicodeString field_name,UnicodeString index_name,bool is_unique )
{
//	UnicodeString index_name = tab_name+"_"+field_name;
	if( CheckOfExistsIndex(index_name) )return false;
	Query->Close();
	Query->SQL->Clear();
	if( is_unique )Query->SQL->Add("CREATE UNIQUE INDEX "+index_name);
	else Query->SQL->Add("CREATE INDEX "+index_name);
	Query->SQL->Add(" ON "+tab_name+"("+field_name+")");
	Query->ExecSQL();
	return true;
}

/**
* �������� �������
*
*/
bool __fastcall TKGlobalSem::CreateIndex( UnicodeString tab_name,UnicodeString field_name,bool is_unique )
{
	UnicodeString index_name = tab_name+"_"+field_name;
	return CreateIndex(tab_name,field_name,index_name,is_unique);
}




/**
* ������������� ������
*/
UnicodeString __fastcall TKGlobalSem::DecodeStr(UnicodeString str)
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
  wchar_t ch;
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
* ��������� ������� ��� ����������� - �������������
*/
UnicodeString __fastcall TKGlobalSem::ReverseDigit(UnicodeString str)
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
* ��������� ������� ��� ����������� - �������������
*/
UnicodeString __fastcall TKGlobalSem::ReverseChar(UnicodeString str)
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
* ��������� �������� ��������� ������
*/
bool __fastcall TKGlobalSem::CheckArg(UnicodeString str)
{
	int n                 = ParamCount();
	for( int i=1; i<=n; i++ )
	{
		if( ParamStr(i) == str )
		{
		   return true;
		}
	}
	return false;
}

/**
* ������� �������� ��������� ��������� ������ ��������� �� ������
*/
UnicodeString __fastcall TKGlobalSem::GetArg(UnicodeString str)
{
	int n                 = ParamCount();
	for( int i=1; i<=n; i++ )
		if( ParamStr(i) == str && i < n)return ParamStr(i+1);
	return "";
}


/**
* ������������� ����� � n ���������� ����� � ���� ������
*/
UnicodeString __fastcall TKGlobalSem::FullN(int a,int n)
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

/*****************************************************************************
* ����� TKValueSem
*****************************************************************************/
/**
* ����������� TKValueSem
*/
__fastcall TKValueSem::TKValueSem(): TObject()
{
	Type       = psdtUnknown;
	IdObject   = -1;
	AsVariant  = "";
	AsString   = "";
	AsFloat    = 0;
	AsInteger  = 0;
	AsDateTime = 0;
	AsTime     = 0;
	AsDate     = 0;
}

/**
* ��������� �������� int
*/
void __fastcall TKValueSem::Set(int val)
{
	Type       = psdtInteger;
	AsInteger  = val;
	AsVariant  = val;
	AsString   = IntToStr(val);
	AsFloat    = val;
	AsDateTime = val;
	AsTime     = AsDateTime;
	AsDate     = AsDateTime;

}

/**
* ��������� �������� double
*/
void __fastcall TKValueSem::Set(double val)
{
	Type       = psdtFloat;
	AsFloat    = val;
	AsInteger  = val;
	AsVariant  = val;
	AsString   = FloatToStr(val);
	AsDateTime = val;
	AsTime     = AsDateTime;
	AsDate     = AsDateTime;
}

/**
* ��������� �������� TDateTime
*/
void __fastcall TKValueSem::Set(TDateTime val)
{
	Type       = psdtDateTime;
	AsFloat    = val;
	AsInteger  = (int)val;
	AsVariant  = val;
	AsString   = val.FormatString("dd.mm.yyyy hh:nn:ss");
	AsDateTime = val;
	AsTime     = AsDateTime;
	AsDate     = AsDateTime;
}

/**
* ��������� �������� String
*/
void __fastcall TKValueSem::Set(UnicodeString val)
{
	Type       = psdtString;
	AsFloat    = StrToFloatDef(val,0);
	AsInteger  = StrToIntDef(val,0);
	AsVariant  = val;
	AsString   = val;
	AsDateTime = StrToDateTimeDef(val,0);
	AsTime     = AsDateTime;
	AsDate     = AsDateTime;
}





/*****************************************************************************
* ����� TKParamSem, TKParamSemItem � TKParamSemList
*****************************************************************************/
/**
* ����������� TKParamSem
*/
__fastcall TKParamSem::TKParamSem(TObject *journal)
      :TObject()
{
	Journal = journal;
	Num     = -1;
	Key     = "";
	Name    = "";
//	Type    = type;
}

/**
* ����������� TKParamSemItem
*/
__fastcall TKParamSemItem::TKParamSemItem(TCollection *Collection)
   : TCollectionItem(Collection), FValue(0) { }

/**
* ����������� TKParamSemList
*/
__fastcall TKParamSemList::TKParamSemList()
	: TCollection(__classid(TKParamSemItem)) { }

/**
* �������� ������� � ������
*/
void __fastcall TKParamSemList::Add(TKParamSem *AValue)
{
	TKParamSemItem *Item = (TKParamSemItem *) TCollection::Add();
	Item->Value = AValue;
}

/**
* ������� ������� �� ������
*/
void __fastcall TKParamSemList::Delete(int Index)
{
	if( Index < 0 && Index >= TCollection::Count )return;
	delete Items[Index];
	TCollection::Delete(Index);
}

/**
* �������� ������� �� ������
*/
TKParamSem * __fastcall TKParamSemList::GetItem(int Index)
{
	TKParamSemItem *Item = (TKParamSemItem *) TCollection::GetItem(Index);
	return Item->Value;
}

/*
* ��������� ������� � ������
*/
void __fastcall TKParamSemList::SetItem(int Index, TKParamSem * AValue)
{
	TKParamSemItem *Item = (TKParamSemItem *) TCollection::GetItem(Index);
	Item->Value = AValue;
}

/**
* ����� �������� �� �����
* @param key - ���-���� ���������
*/
TKParamSem * __fastcall TKParamSemList::FindByKey( UnicodeString key )
{
   for( int i=0; i<Count; i++ )
   {
	   TKParamSem *par = Items[i];
	   if( par->Key == key )return par;
   }
   return NULL;
}


/*****************************************************************************
* ����� TKParamJournal, TKParamJournalItem � TKParamJotnalList
*****************************************************************************/
/**
* ����������� TKJournalSemItem
*/
__fastcall TKJournalSemItem::TKJournalSemItem(TCollection *Collection)
   : TCollectionItem(Collection), FValue(0) { }

/**
* ����������� TKJournalSemList
*/
__fastcall TKJournalSemList::TKJournalSemList()
	: TCollection(__classid(TKJournalSemItem)) { }

/**
* �������� ������� � ������
*/
void __fastcall TKJournalSemList::Add(TKJournalSem *AValue)
{
	TKJournalSemItem *Item = (TKJournalSemItem *) TCollection::Add();
	Item->Value = AValue;
}

/**
* ������� ������� �� ������
*/
void __fastcall TKJournalSemList::Delete(int Index)
{
	if( Index < 0 && Index >= TCollection::Count )return;
	delete Items[Index];
	TCollection::Delete(Index);
}

/**
* �������� ������� �� ������
*/
TKJournalSem * __fastcall TKJournalSemList::GetItem(int Index)
{
	TKJournalSemItem *Item = (TKJournalSemItem *) TCollection::GetItem(Index);
	return Item->Value;
}

/*
* ��������� ������� � ������
*/
void __fastcall TKJournalSemList::SetItem(int Index, TKJournalSem * AValue)
{
	TKJournalSemItem *Item = (TKJournalSemItem *) TCollection::GetItem(Index);
	Item->Value = AValue;
}

/**
* ����� ������ �� �����
* @param key - ���-���� ���������
*/
TKJournalSem * __fastcall TKJournalSemList::FindByKey( UnicodeString key )
{
   for( int i=0; i<Count; i++ )
   {
	   TKJournalSem *j = Items[i];
	   if( j->Key == key )return j;
   }
   return NULL;
}



/**
* ���������� ������ TKJornalSem
* @param key - ���-���� �������
* @param global - ��������� � ����������� �����������
*/
__fastcall TKJournalSem::TKJournalSem( UnicodeString key, TObject *global )
{
//   try
//   {
// ������������� ����������
	   int id_param_pk = -1;
	   ParamPK         = NULL;
	   Global          = global;
	   Key             = "";
	   Params          = new TKParamSemList();
	   Sql             = new TUniQuery(Application);
	   QueryParams     = new TStringList();
	   Sql->Connection = ((TKGlobalSem *)Global)->Connection;
	   if( !((TKGlobalSem *)Global)->Connection->Connected )
	      ((TKGlobalSem *)Global)->Connection->Connect();
// ��������� ��������� �������
	   Sql->Close();
	   Sql->SQL->Clear();
	   Sql->SQL->Add("SELECT m.id_magazine,m.name,m.name_table,p.id_parameter FROM magazine m,param p");
	   Sql->SQL->Add("WHERE m.id_type_object = p.id_type_object(+)");
	   Sql->SQL->Add("AND m.key_name = :a1 AND p.id_input(+) = :a2" );
	   Sql->Params->Items[0]->AsString  = key;
	   Sql->Params->Items[1]->AsInteger = (int)pstPK;
	   Sql->Open();
	   if( !Sql->Eof )
	   {
		   Num         = Sql->FieldByName("id_magazine")->AsInteger;
		   Name        = Sql->FieldByName("name")->AsString;
		   TableName   = Sql->FieldByName("name_table")->AsString;
		   Key         = key;
		   id_param_pk = Sql->FieldByName("id_parameter")->AsInteger;
	   }
	   Sql->Close();
	   if( Key == "" )return;
// ��������� ��������� �������
	   Sql->Close();
	   Sql->SQL->Clear();
	   Sql->SQL->Add("SELECT p.id_input,p.id_parameter_type,p.format,p.round_val,pm.* FROM param p,param_in_magazine pm");
	   Sql->SQL->Add("WHERE p.id_parameter = pm.id_parameter");
	   Sql->SQL->Add("AND pm.id_magazine = :a1" );
	   Sql->Params->Items[0]->AsString  = Num;
	   for(Sql->Open();!Sql->Eof;Sql->Next())
	   {
		   TKParamSem *par  = new TKParamSem(this);
		   par->Num         = Sql->FieldByName("id_parameter")->AsInteger;
		   par->Key         = Sql->FieldByName("key_name")->AsString;
		   par->Name        = Sql->FieldByName("name")->AsString;
		   par->DataType    = Sql->FieldByName("id_parameter_type")->AsInteger;;
		   par->Type        = Sql->FieldByName("id_input")->AsInteger;;
		   par->JournalType = Sql->FieldByName("col_type")->AsInteger;;
		   par->FieldValue  = Sql->FieldByName("name_field_value")->AsString;
		   if( par->Type == pstPK && par->JournalType == psjtManual )
			   par->FieldObject = "id_object";
		   else
			   par->FieldObject = Sql->FieldByName("name_field_object")->AsString;
		   par->Format      = Sql->FieldByName("format")->AsString;
		   par->Round       = Sql->FieldByName("round_val")->AsInteger;
		   if( id_param_pk == par->Num )ParamPK = par;
		   Params->Add(par);
	   }
	   Sql->Close();
       ((TKGlobalSem *)Global)->Journals->Add(this);
//   }
//   catch(...){}
}


/**
* ����� ����������� ���� ������� �� �����
* @param key - ���-���� ���������
*/
UnicodeString __fastcall TKJournalSem::KeyNameToFieldName( UnicodeString key )
{
   TKParamSem *par = Params->FindByKey( key );
   if( par == NULL )return "";
   else
	  if( par->JournalType == psjtManual )return par->FieldValue;
	  else return par->FieldObject;
}

/**
*  ������� ����� ������ �� ���������� ������
*/
void __fastcall TKJournalSem::QueryClear()
{
   for( int i=0; i<QueryParams->Count; i++)
   {
	   TKValueSem *val = (TKValueSem *)QueryParams->Objects[i];
	   delete val;
   }
   QueryParams->Clear();
}


/**
*  ������� ����� ������ �� ���������� ������
*/
int __fastcall TKJournalSem::QueryInsert()
{
	TDateTime dt    = dt.CurrentDateTime();

//������� ����� ������ � ��� �� �������
	numLine     = ((TKGlobalSem *)Global)->AutoNum(GEN_LINES);
	orderLine   = ((TKGlobalSem *)Global)->AutoNum(GEN_LINE_NUMBER);
//��� ����� ������ ����� ID_OBJECT
	numObject   = ((TKGlobalSem *)Global)->AutoNum(GEN_OBJECTS);
	Sql->Close();
	Sql->SQL->Clear();
	Sql->SQL->Add("INSERT INTO "+TableName+"(");
//������ [1] ���� ����� ����
	Sql->SQL->Add("ID_LINE,DT_LINE,ORDER_LINE,ID_OBJECT,STATUS_LINE,ID_USER,LAST_EDITING");
	for( int i=0; i<QueryParams->Count; i++)
		Sql->SQL->Add(","+QueryParams->Strings[i]);
	Sql->SQL->Add(") VALUES(");
//������ [3] ���� ����� ��������
	Sql->SQL->Add(":a0,:a1,:a2,:a3,:a4,:a5,:a6");
	for( int i=0; i<QueryParams->Count; i++)
		Sql->SQL->Add(",:a"+IntToStr(i+7));
	Sql->SQL->Add(")");
	Sql->Params->Items[0]->AsInteger  = numLine;
	Sql->Params->Items[1]->AsDateTime = dt;
	Sql->Params->Items[2]->AsInteger  = orderLine;
	Sql->Params->Items[3]->AsInteger  = numObject;
	Sql->Params->Items[4]->AsInteger  = 0;
	Sql->Params->Items[5]->AsInteger  = 0;
	Sql->Params->Items[6]->AsString   =
		dt.FormatString("dd.mm.yyyy hh:nn") + " " + getenv("USERNAME") + "@" + getenv("USERDOMAIN");
	for( int i=0; i<QueryParams->Count; i++)
		AddParamAnyType(i+7, (TKValueSem *)QueryParams->Objects[i] );
	UnicodeString ss=Sql->SQL->Text;
	Sql->ExecSQL();
	return numObject;
}

/**
*  ������� ����� ������ �� ��������� ������
*/
void __fastcall TKJournalSem::QueryUpdate(int obj)
{

	TDateTime dt    = dt.CurrentDateTime();

	Sql->Close();
	Sql->SQL->Clear();
	Sql->SQL->Add("UPDATE "+TableName+"");

//������ [1] ���� ����� ����
	Sql->SQL->Add("SET LAST_EDITING=:a0");
	for( int i=0; i<QueryParams->Count; i++)
		Sql->SQL->Add(","+QueryParams->Strings[i]+" = :a"+IntToStr(i+1));
	Sql->SQL->Add("WHERE id_object = :a"+IntToStr(QueryParams->Count+1));
	Sql->Params->Items[0]->AsString   =
		dt.FormatString("dd.mm.yyyy hh:nn") + " " + getenv("USERNAME") + "@" + getenv("USERDOMAIN");
	for( int i=0; i<QueryParams->Count; i++)
		AddParamAnyType(i+1, (TKValueSem *)QueryParams->Objects[i] );
	Sql->Params->Items[QueryParams->Count+1]->AsInteger   = obj;
	Sql->ExecSQL();

}

/**
*  �������� �������� � ����������� �� ����
*/
void __fastcall TKJournalSem::AddParamAnyType(int n,TKValueSem *val)
{
	switch(val->Type)
	{
		case psdtInteger:
		   Sql->Params->Items[n]->AsInteger = val->AsInteger;
		   break;
		case psdtFloat:
		   Sql->Params->Items[n]->AsFloat = val->AsFloat;
		   break;
		case psdtDateTime:
		   Sql->Params->Items[n]->AsDateTime = val->AsDateTime;
		   break;
		case psdtString:
		   Sql->Params->Items[n]->AsString = val->AsString;
		   break;
		default:
		   Sql->Params->Items[n]->AsString = "";
	}
}


/**
*  ��������� �������� � ����� ��������� � ������
*
* @param - key_name ���������
*/
void __fastcall TKJournalSem::AddParamQuery( UnicodeString key , int val)
{
	UnicodeString f = KeyNameToFieldName(key);
	if( f!= "" )
	{
		TKValueSem *v   = new TKValueSem;
		v->AsInteger    = val;
		v->Type         = psdtInteger;
		QueryParams->AddObject(f,v);
	}
}

/**
*  ��������� �������� � ������������ ��������� � ������
*
* @param - key_name ���������
*/
void __fastcall TKJournalSem::AddParamQuery( UnicodeString key , double val)
{
	UnicodeString f = KeyNameToFieldName(key);
	if( f!= "" )
	{
		TKValueSem *v   = new TKValueSem;
		v->AsFloat      = val;
		v->Type         = psdtFloat;
		QueryParams->AddObject(f,v);
	}
}

/**
*  ��������� �������� � ��������� ��������� � ������
*
* @param - key_name ���������
*/
void __fastcall TKJournalSem::AddParamQuery( UnicodeString key , TDateTime val)
{
	UnicodeString f = KeyNameToFieldName(key);
	if( f!= "" )
	{
		TKValueSem *v   = new TKValueSem;
		v->AsDateTime   = val;
		v->Type         = psdtDateTime;
		QueryParams->AddObject(f,v);
	}
}

/**
*  ��������� �������� � ������������ ��������� � ������
*
* @param - key_name ���������
*/
void __fastcall TKJournalSem::AddParamQuery( UnicodeString key , UnicodeString val)
{
	UnicodeString f = KeyNameToFieldName(key);
	if( f!= "" )
	{
		TKValueSem *v   = new TKValueSem;
		v->AsString     = val;
		v->Type         = psdtString;
		QueryParams->AddObject(f,v);
	}
}

/*******************************************************************************
* ����� ��������� ����������
*******************************************************************************/
//�����������
__fastcall TKPerfStatistic::TKPerfStatistic()
{
	List = new TList();
}

//����������
__fastcall TKPerfStatistic::~TKPerfStatistic()
{
	for( int i=0; i<List->Count; i++){
	   TKtatisricItem *item = (TKtatisricItem *)List->Items[i];
	   delete item;
	}
	delete List;
}

//����� � �������� ����� ������ �� �����������
TKtatisricItem * __fastcall TKPerfStatistic::FindOrCreate( UnicodeString name)
{
	TKtatisricItem *item = NULL;
	for( int i=0; i<List->Count; i++){
	   item = (TKtatisricItem *)List->Items[i];
	   if( item->Name == name )return item;
	}
	item            = new TKtatisricItem;
	TDateTime d;
	item->Name      = name;
	item->TimeFirst = item->TimeBegin.CurrentDateTime();
	item->TimeBegin = item->TimeBegin.CurrentDateTime();
	item->TimeEnd   = item->TimeBegin.CurrentDateTime();
	item->LoopCount = 0;
	item->MinTime   = 0;
	item->MaxTime   = 0;
	item->AvgTime   = 0;
	item->SumTime   = 0;
	item->ExeTime   = 0;
	item->Flag      = false;
	List->Add(item);
	return item;
}
void __fastcall TKPerfStatistic::Begin( UnicodeString name )
{
   TKtatisricItem *item = FindOrCreate( name );
   item->TimeBegin = item->TimeBegin.CurrentDateTime();
   item->Flag      = true;
}

void __fastcall TKPerfStatistic::Commit( UnicodeString name )
{
   TKtatisricItem *item = FindOrCreate( name );
   item->TimeEnd    = item->TimeEnd.CurrentDateTime();
   int t1 = MilliSecondsBetween( item->TimeEnd, item->TimeBegin );
   item->SumTime   += t1;
   if( item->LoopCount == 0 ){
	  item->TimeFirst = item->TimeBegin;
	  item->MinTime   = t1;
	  item->MaxTime   = t1;
   }
   else {
	  if( t1 < item->MinTime )item->MinTime = t1;
	  if( t1 > item->MaxTime )item->MaxTime = t1;
   }
   item->LoopCount++;
   item->AvgTime    = item->SumTime/item->LoopCount;
   item->ExeTime    = MilliSecondsBetween( item->TimeEnd, item->TimeFirst );
   item->Flag       = false;

}


void __fastcall TKPerfStatistic::Rollback( UnicodeString name )
{
   TKtatisricItem *item = FindOrCreate( name );
   item->Flag       = false;
}

void __fastcall TKPerfStatistic::Print( UnicodeString name )
{
	TStringList *sl = new TStringList();
	TDateTime dt;
	sl->Add("���������� "+ParamStr(0)+" �� "+dt.CurrentDateTime().DateTimeString());
	sl->Add("��������\t������\t���,��\t����,��\t�����,��\t���,��\t���,��");
	for( int i=0; i<List->Count; i++){
	   TKtatisricItem *item = (TKtatisricItem *)List->Items[i];
	   sl->Add(item->Name+"\t"+
		  IntToStr(item->LoopCount)+"\t"+
		  IntToStr(item->MinTime)+"\t"+
		  IntToStr(item->MaxTime)+"\t"+
		  IntToStr(item->AvgTime)+"\t"+
		  IntToStr(item->SumTime)+"\t"+
		  IntToStr(item->ExeTime));
	}
	sl->SaveToFile(name);
	delete sl;
}

