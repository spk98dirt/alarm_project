#include "AsdClass3.h"


//extern TForm *Form1;
//extern TMemo *Console;
//#define Log(message) if(Console->Enabled)Console->Lines->Add(message)
//extern TProgressBar *Bar;

/**
* Инициализация для конструкторов
*/

void __fastcall TKControllers::Init()
{
	  Alarms = new TList();
	  for( int i=0; i<MAX_CONTROLLER; i++)
		 Controllers[i] = NULL;
	  Log = NULL;
	  AlarmCheck     = true;
	  AlarmException = false;
	  Count          = 0;
	  IsConfigChange = false;
	  IsAlarmChange  = false;
	  IsDump         = false;
	  ListStatPP     = new TList();

}

/**
* Конструктор класса TKControllers
*
*@param arm - номер АРМа АСУ горизонт
*@param Sql - запрс к БД
*/
__fastcall TKControllers::TKControllers()
{
	  Init();
	  Global         = NULL;
}




/**
* Конструктор класса TKControllers
*
*@param arm - номер АРМа АСУ горизонт
*@param Sql - запрс к БД
*/
__fastcall TKControllers::TKControllers(TKGlobalSem *glob)
{
	  Init();
	  InitProperties(glob);
}

/*
* Инициализация свойств из INI-шника или J_PARAM_ASD
*/
void __fastcall TKControllers::InitProperties(TKGlobalSem *glob)
{
   Global = glob;
   TimeEveningToNight = StrToDateTimeDef( ReadProperty("ASD","TimeEveningToNightLight",""),TDateTime(0,0,0,0));
   TimeNightToMorning = StrToDateTimeDef( ReadProperty("ASD","TimeNightToMorningLight",""),TDateTime(5,0,0,0));
//   TableArchive       = ReadProperty("ASD","TableArchive", "PollserviceArchive");
   TableArchive       = glob->IniFile->ReadString("RtMonitor","TableArchive", "PollserviceArchive");

}

/**
* Инициализация списка статусов ПП
*/
void __fastcall TKControllers::InitListStatPP(){
   Global->Query->SQL->Clear();
   Global->Query->SQL->Add("SELECT UNIQUE sr.*,tp.* FROM J_SPRAV_STAT_RT sr, J_SPRAV_TYPE_OBJ tp");
   Global->Query->SQL->Add("WHERE tp.v_gis_type_obj_name='ПП' AND tp.id_object=sr.o_gis_type_obj_id ORDER BY v_asd_stat_rt_obj_id");
   for(Global->Query->Open(); !Global->Query->Eof; Global->Query->Next() ){
	  TKSpravStatObject *socm = new TKSpravStatObject;
	  socm->StatObject        = Global->Query->FieldByName("v_asd_stat_rt_obj_id")->AsInteger;
	  socm->StatName          = Global->Query->FieldByName("v_asd_stat_rt_obj_name")->AsString;
	  UnicodeString s         = Global->Query->FieldByName("v_asd_stat_rt_obj_color1")->AsAnsiString;
	  if( s != "" )socm->StatColor1   = RGBStringToColor(s);
	  else socm->StatColor1           = clBlack;
	  s                               = Global->Query->FieldByName("v_asd_stat_rt_obj_color2")->AsAnsiString;
	  if( s != "" )socm->StatColor2   = RGBStringToColor(s);
	  else socm->StatColor2           = clBlack;

//	  socm->StatColor1        = RGBStringToColor(Global->Query->FieldByName("v_asd_stat_rt_obj_color1")->AsAnsiString);
//	  socm->StatColor2        = RGBStringToColor(Global->Query->FieldByName("v_asd_stat_rt_obj_color2")->AsAnsiString);
	  socm->PanStyleSelect    = Global->Query->FieldByName("v_asd_stat_rt_obj_mode")->AsString;
	  s                       = Global->Query->FieldByName("v_asd_stat_rt_obj_control_mode")->AsString;
	  if ( s == "Семантика" )socm->PanControlMode = stocmSemantic;
	  else if ( s == "Выделение" )socm->PanControlMode = stocmSelect;
	  else if ( s == "Выделение+Семантика" )socm->PanControlMode = stocmBoth;
	  else socm->PanControlMode = stocmUnknown;
	  socm->PanSemanticNum    = Global->Query->FieldByName("v_gis_type_obj_sem_stat")->AsInteger;
	  socm->PanSemanticValue  = Global->Query->FieldByName("v_asd_stat_rt_obj_semantic")->AsString;
	  socm->PanSemanticPKCode = Global->Query->FieldByName("v_gis_type_obj_sem_pkcode")->AsInteger;
	  socm->PanObjectCode     = Global->Query->FieldByName("v_gis_type_obj_code")->AsInteger;
	  ListStatPP->Add(socm);
   }
}

 TKSpravStatObject * __fastcall TKControllers::GetStatPP( int stat ){
   TKSpravStatObject *socm = NULL;
   for( int i=0; i<ListStatPP->Count; i++){
	  socm = (TKSpravStatObject *)ListStatPP->Items[i];
	  if(socm->StatObject == stat )return socm;
   }
   return NULL;
}



void __fastcall TKControllers::SetPropertyForm(TForm *form, UnicodeString section, UnicodeString suffix){
// Ширина формы
   bool as = form->AutoSize;
   form->AutoSize = false;
   UnicodeString s = suffix+"Width";
   int x= Global->IniFile->ReadInteger(section,s,-1);
   if( x>=0 )form->Width = x;
   s = suffix+"FormWidth";
   x= Global->IniFile->ReadInteger(section,s,-1);
   if( x>=0 )form->Width = x;
// Высота формы
   s = suffix+"Height";
   x= Global->IniFile->ReadInteger(section,s,-1);
   if( x>=0 )form->Height = x;
   s = suffix+"FormHeight";
   x= Global->IniFile->ReadInteger(section,s,-1);
   if( x>=0 )form->Height = x;
// Центр формы относительно экрана
   s = suffix+"CenterY";
   x= Global->IniFile->ReadInteger(section,s,-1);
   if( x>=0 )form->Top = (Screen->Height - form->Height) * x/100;
   s = suffix+"CenterX";
   x= Global->IniFile->ReadInteger(section,s,-1);
   if( x>=0 )form->Left   = (Screen->Width - form->Width) * x/100;
// Координата Y
   s = suffix+"Top";
   x= Global->IniFile->ReadInteger(section,s,-1);
   if( x>=0 )form->Top   = x;
// Координата X
   s = suffix+"Left";
   x= Global->IniFile->ReadInteger(section,s,-1);
   if( x>=0 )form->Left  = x;
// Название формы
   s = suffix+"Name";
   s = Global->IniFile->ReadString(section,s,"");
   if( s != "" )form->Caption  = s;
// Поверх остальных форм
   s = suffix+"StayOnTop";
   x= Global->IniFile->ReadInteger(section,s,-1);
   if( x == 1 )form->FormStyle  = fsStayOnTop;
// Название шрифта
   s = suffix+"FontName";
   s = Global->IniFile->ReadString(section,s,"");
   if( s != "" )form->Font->Name  = s;
// Размер шрифта
   s = suffix+"FontSize";
   x= Global->IniFile->ReadInteger(section,s,-1);
   if( x >= 0 )form->Font->Size  = x;
   form->AutoSize = as;

}



/**
* Загрузить параметры сперва из журнала J_PARAM_ASD, затем из INI-шника, затем присвить "по умолчанию"
*
* @param section - секция инишника или раздела J_PARAM_ASD
* @param name - название параметра
* @param value - значение по-умолчанмю
* @return - значение параметра
*/
UnicodeString __fastcall TKControllers::ReadProperty(UnicodeString section, UnicodeString name, UnicodeString value)
{
   UnicodeString ret;
   if( Global == NULL )return value;
   if( Global->CheckOfExistsTable("j_param_asd") ){
	  Global->Query->SQL->Clear();
	  Global->Query->SQL->Add("SELECT * FROM j_param_asd WHERE v_razdelasd=:a1 AND v_paramasd=:a2");
	  Global->Query->Params->Items[0]->AsString = section;
	  Global->Query->Params->Items[1]->AsString = name;
	  Global->Query->Open();
	  if( !Global->Query->Eof )
	  {
		 ret = Global->Query->FieldByName("v_valueasd")->AsString;
		 Global->Query->Close();
		 return ret;
	  }
   }
   ret = Global->IniFile->ReadString(section,name,value);
   return ret;

}

int __fastcall TKControllers::ReadPropertyInt(UnicodeString section, UnicodeString name, int value){
   return StrToIntDef(ReadProperty(section,name,""),value);
}

TDateTime __fastcall TKControllers::ReadPropertyDateTime(UnicodeString section, UnicodeString name, TDateTime value){
   return StrToDateTimeDef(ReadProperty(section,name,""),value);
}



void __fastcall TKControllers::InitControllersFromXml(UnicodeString file,TKGlobalSem *glob,int arm)
{
// Создаем XML документ
	try
	{
	   if( Global == NULL )Global = glob;
	   TUniQuery *Sql1 = glob->Query;
	   TKController *cnt;
	   TKParam *param;
	   _di_IXMLDocument Config = NewXMLDocument();
	   TDateTime d1 = d1.CurrentDateTime();
	   Config->SetActive(true);
	   Config->Options << doNodeAutoIndent;
	   Config->NodeIndentStr = "   ";
// Загружаем документ из файла
	   if( file == "" )return;
	   if( !FileExists(file) )return;
	   if( ArmName == "" )ArmName = "Горизонт";

// Пропустить три незначащих символа (КС от Горизонта)
	   TFileStream *fstream = new TFileStream( file, fmOpenRead );
	   fstream->Seek(3,soFromBeginning );
	   Config->LoadFromStream(fstream, xetUTF_8);
	   delete fstream;
	   _di_IXMLNode node1 = Config->DocumentElement;
	   UnicodeString ss = node1->GetNodeName();
	   if( ss == "Project" )node1 = node1->ChildNodes->Nodes[WideString("Controllers")];

// Перебираем контроллеры
// Перебераем ноды с контроллерами
	   if(Log!=NULL)Log->StartBar(0,node1->ChildNodes->Count);

	   for( int i=0; i< node1->ChildNodes->Count; i++)
	   {
		   _di_IXMLNode node2 = node1->ChildNodes->Nodes[i];
		   int net = XMLNodeIntAttribute( node2->ChildNodes->Nodes[WideString("Link")],"NetAddr",-1);
		   if( net < 0 && net >= MAX_CONTROLLER )continue;
	//Проверяем, что нода с контроллером
		   UnicodeString s = node2->GetNodeName();
		   if( node2->GetNodeName() != WideString( "Controller" ) )continue;
//Определяем статус контроллера. Если активный, то создаем объект
		   if( XMLNodeAttribute( node2, "Active", "0") != "1" )continue;
		   if( i >= MAX_CONTROLLER )break;
		   Count++;
		   cnt              = new TKController;
		   cnt->ArchiveValuesParam = new TKArchiveValuesParam((TObject *)this);

		   for( int j=0; j<MAX_PARAM; j++ )cnt->Params[j] = NULL;
		   cnt->Alarms      = new TList();
		   cnt->IntegralValue = NULL;
		   cnt->ppStatusAF   = "";
		   cnt->MapObject   = NULL;
		   cnt->Component   = NULL;
		   cnt->PP          = "";
		   cnt->ParentCascade = "";
		   cnt->CountCascade = 0;
		   cnt->Status      = TKControllerStatus::csNone;
		   cnt->UpdateDT    = 0;
		   cnt->CheckDT     = 0;
		   cnt->EventDT     = 0;
		   cnt->IsNewValue  = true;
		   cnt->IsViewChange = false;
		   cnt->Arm         = arm;
           cnt->IsBKS       = false;
		   cnt->Object      = XMLNodeIntAttribute( node2, "NObject", -1 );
		   cnt->PosX        = XMLNodeIntAttribute( node2, "PosX", 0 );
		   if( cnt->PosX >= 750 && cnt->PosX < 850 )cnt->PosX        = RoundTo(cnt->PosX*15,3)/15;
		   else cnt->PosX        = RoundTo(cnt->PosX,2);

//		   int tx           = cnt->PosX%100;
//		   if( tx > 40 && tx < 50 )cnt->PosX += 10;
//		   cnt->PosX        = RoundTo(cnt->PosX,2);
		   cnt->PosY        = XMLNodeIntAttribute( node2, "PosY", 0 );
		   cnt->PosY        = RoundTo(cnt->PosY,1);
		   cnt->NetAddr     = net;
		   cnt->AsoduName   = XMLNodeAttribute( node2, "Name", "" );
		   cnt->AsoduActive = XMLNodeIntAttribute( node2, "Active", 1 );
// Тип модема
		   UnicodeString s1 = XMLNodeAttribute( node2->ChildNodes->Nodes[WideString("Link")],"ChannelDriver","");
		   if( s1.Pos("airudp") == 1 )cnt->AsoduTypeModem = "GSM";
		   else if( s1.Pos("airport") == 1 )cnt->AsoduTypeModem = "Радио";
		   else cnt->AsoduTypeModem = "Неизвестный";
// Уставки токов и напряжений
		   cnt->IA_min = 0; cnt->IB_min = 0; cnt->IC_min = 0;
		   cnt->IA_max = 0; cnt->IB_max = 0; cnt->IC_max = 0;
		   cnt->UA_min = 0; cnt->UB_min = 0; cnt->UC_min = 0;
		   cnt->UA_max = 0; cnt->UB_max = 0; cnt->UC_max = 0;
// Репитер или нет
/*
		 aic->isRepiter = false;
		 aic->AsoduRepiterChild = 0;
		 if( NodeAttribute( node2->ChildNodes->Nodes[WideString("Link")],"UseRepiter","0") == "0" )aic->useRepiter = false;
		 else aic->useRepiter = true;
		 aic->AsoduRepAddr   = NodeAttribute( node2->ChildNodes->Nodes[WideString("Link")],"RepAddr","");
*/
		 cnt->AsoduAddress  = node2->ChildNodes->Nodes[WideString("Address")]->Text;
		 cnt->AsoduType  = XMLNodeAttribute( node2->ChildNodes->Nodes[WideString("Type")],"TypeName","");
// Формируем суррогатный ключ контроллера
		 cnt->AsoduKey   = arm*10000+cnt->Object;
// Формируем параметр связи
		 param                 = new TKParam;
		 param->Num            = 0;
		 param->Name           = "Состояние связи";
		 param->ShortName      = "Link";
		 param->AsoduName      = "Link";
		 param->Status         = psNone;
		 param->AsoduStatus    = 0;
		 param->ConstDT        = 0;
		 param->ConstCount     = 0;
		 param->AlarmExceptionFlag = false;
		 param->AlarmFlag = false;
		 param->AlarmText      = "";
		 param->AlarmAlgorithm = "";
		 param->AlarmEnableFlag    = false;
		 param->AlarmNowFlag       = false;
		 param->AlarmCheckFlag     = false;
		 param->ChangeDT       = d1;
		 param->Controller     = (void *)cnt;
		 cnt->Params[0]        = param;


// А теперь считываем параметры
// Перебираем список всех параметров контроллера
		 _di_IXMLNode node3 = node2->ChildNodes->Nodes[WideString("Signals")];
		 for( int j=0; j<node3->ChildNodes->Count; j++ )
		 {
				_di_IXMLNode node4 = node3->ChildNodes->Nodes[j];
//Проверяем, что нода с сигналом
				if( node4->GetNodeName() != WideString( "Signal" ) )continue;
//Определяем статус сигнала. Если активный, то создаем объект
				if( XMLNodeAttribute( node4->ChildNodes->Nodes[WideString("State")], "IsExists", "0") != "1" )continue;
				int n = XMLNodeIntAttribute( node4, "NSignal", -1 );
				if( n<0 || n >= MAX_PARAM )continue;
				param                 = new TKParam;
				param->Num            = n;
				param->Name           = XMLNodeAttribute( node4, "Name", "");
				param->AsoduName      = XMLNodeAttribute( node4, "ShortName", "" );
				param->ShortName      = AliasParamName( param->AsoduName );
//Формируем уставки ПП по основным парамерам
				if( param->ShortName == "Ia" )
				{
					cnt->IA_min = XMLNodeIntAttribute( node4->ChildNodes->Nodes[WideString("Range")], "LoLimit", 0);
					cnt->IA_max = XMLNodeIntAttribute( node4->ChildNodes->Nodes[WideString("Range")], "HiLimit", 0);
				}
				else if( param->ShortName == "Ib" )
				{
					cnt->IB_min = XMLNodeIntAttribute( node4->ChildNodes->Nodes[WideString("Range")], "LoLimit", 0);
					cnt->IB_max = XMLNodeIntAttribute( node4->ChildNodes->Nodes[WideString("Range")], "HiLimit", 0);
				}
				else if( param->ShortName == "Ic" )
				{
					cnt->IC_min = XMLNodeIntAttribute( node4->ChildNodes->Nodes[WideString("Range")], "LoLimit", 0);
					cnt->IC_max = XMLNodeIntAttribute( node4->ChildNodes->Nodes[WideString("Range")], "HiLimit", 0);
				}
				else if( param->ShortName == "Ua" )
				{
					cnt->UA_min = XMLNodeIntAttribute( node4->ChildNodes->Nodes[WideString("Range")], "LoLimit", 0);
					cnt->UA_max = XMLNodeIntAttribute( node4->ChildNodes->Nodes[WideString("Range")], "HiLimit", 0);
				}
				else if( param->ShortName == "Ub" )
				{
					cnt->UB_min = XMLNodeIntAttribute( node4->ChildNodes->Nodes[WideString("Range")], "LoLimit", 0);
					cnt->UB_max = XMLNodeIntAttribute( node4->ChildNodes->Nodes[WideString("Range")], "HiLimit", 0);
				}
				else if( param->ShortName == "Uc" )
				{
					cnt->UC_min = XMLNodeIntAttribute( node4->ChildNodes->Nodes[WideString("Range")], "LoLimit", 0);
					cnt->UC_max = XMLNodeIntAttribute( node4->ChildNodes->Nodes[WideString("Range")], "HiLimit", 0);
				}

				param->Status         = psNone;
		        param->AsoduStatus    = 0;
				param->ConstDT     = 0;
				param->ConstCount  = 0;
				param->AlarmExceptionFlag = false;
				param->AlarmFlag = false;
				param->AlarmText      = "";
				param->AlarmAlgorithm = "";
				param->AlarmEnableFlag    = false;
				param->AlarmNowFlag       = false;
				param->AlarmCheckFlag     = false;
				param->ChangeDT       = d1;
				param->Controller     = (void *)cnt;
				cnt->Params[n]        = param;
	   }


		 Controllers[net] = cnt;
		 if(Log!=NULL)Log->SetBar(i);
		 Application->ProcessMessages();
	   }
	   if(Log!=NULL)Log->StopBar();
	   if(Log!=NULL)Log->Message("Считано "+IntToStr(Count)+" пунктов контроллеров");
	   LinkToPP(glob);
	   if( AlarmException )CheckException(Sql1);
	   LinkToAlarm(Sql1);
   }
	catch(Exception &e)
	{
		if(Log!=NULL)Log->Error("Ошибка при чтении XML "+e.Message);
	}

}

/**
* Связывание со справочником ПП
*
*@param Sql1 - квери к базе данных
*/
void __fastcall TKControllers::LinkToPP(TKGlobalSem *glob)
{

   try
   {
	  TUniQuery *Sql1 = glob->Query;
// Открываем журнал ПП через АПИ
	  TKJournalSem *j1 = glob->Journals->FindByKey("j_srpav_PP");
      TDateTime dt = dt.CurrentDateTime();
// Находим ID-шник Горизонтовского типа ПП
	  int id_type = -1;
	  Sql1->Close();
	  Sql1->SQL->Clear();

	  Sql1->SQL->Add("SELECT * FROM j_sprav_type_control WHERE v_idtypecontrol=:a1");
	  Sql1->Params->Items[0]->AsString = ArmName;
	  Sql1->Open();
	  if( !Sql1->Eof )id_type = Sql1->FieldByName("id_object")->AsInteger;


// Открываем справочник ПП
	  Sql1->Close();
	  Sql1->SQL->Clear();

	  Sql1->SQL->Add("SELECT pp.id_object pp_id_obj,pp.*,g.*,tc.v_idtypecontrol FROM j_srpav_pp pp,j_ListGraph g,j_sprav_type_control tc");
	  Sql1->SQL->Add("WHERE pp.o_graphonoff = g.id_object (+) AND pp.o_idtypecontrol = tc.id_object(+)");
	  int Count       = 0;
	  int CountNew    = 0;
	  int CountChange = 0;
	  for( Sql1->Open(); !Sql1->Eof; Sql1->Next() )
	  {
		  UnicodeString pp_link     = Sql1->FieldByName("v_id_kanal_sv")->AsString;
		  UnicodeString pp          = Sql1->FieldByName("v_pp")->AsString;
		  UnicodeString pp_type     = Sql1->FieldByName("v_idtypecontrol")->AsString;
		  int           pp_obj      = Sql1->FieldByName("pp_id_obj")->AsInteger;
		  bool IsGorizont = false;
// Ищем в списке контролеров
		  for(int j=0; j<MAX_CONTROLLER; j++)
		  {
			  TKController *cnt = Controllers[j];
			  if( cnt == NULL )continue;
			  if( cnt->AsoduName == pp_link || cnt->AsoduName == pp )
			  {
				 IsGorizont         = true;
				 cnt->PP            = pp;
				 cnt->ppIdObject    = pp_obj;
				 cnt->ppStatus      = Sql1->FieldByName("v_status_pp")->AsString;
				 cnt->ppBalans      = Sql1->FieldByName("v_nenabalans")->AsString;
				 cnt->Graphonoff    = Sql1->FieldByName("v_graphonoff")->AsString;
				 cnt->CodGraphOnOff = Sql1->FieldByName("v_CodGraphOnOff")->AsString;
// Проверяем, не изменился ли контроллер
				 j1->QueryClear();
				 if( pp_link != cnt->AsoduName )j1->AddParamQuery("id_kanal_sv",pp_link);

				 if( pp_type!= ArmName )j1->AddParamQuery("IDTypeControl",id_type);
				 UnicodeString s = Sql1->FieldByName("v_bcs")->AsString;
				 if( s == "Да" )cnt->IsBKS = true;


				 int x = Sql1->FieldByName("v_coordy")->AsInteger;
				 if( x!= cnt->PosY )j1->AddParamQuery("CoordY",cnt->PosY);
				 x = Sql1->FieldByName("v_coordx")->AsInteger;
				 if( x!= cnt->PosX )j1->AddParamQuery("CoordX",cnt->PosX);

				 double m = Sql1->FieldByName("v_pp_ia_min")->AsFloat;
				 if( m!= cnt->IA_min )j1->AddParamQuery("PP_Ia_min",cnt->IA_min);
				 m = Sql1->FieldByName("v_pp_ia_max")->AsFloat;
				 if( m!= cnt->IA_max )j1->AddParamQuery("PP_Ia_max",cnt->IA_max);
				 m = Sql1->FieldByName("v_pp_ib_min")->AsFloat;
				 if( m!= cnt->IB_min )j1->AddParamQuery("PP_Ib_min",cnt->IB_min);
				 m = Sql1->FieldByName("v_pp_ib_max")->AsFloat;
				 if( m!= cnt->IB_max )j1->AddParamQuery("PP_Ib_max",cnt->IB_max);
				 m = Sql1->FieldByName("v_pp_ic_min")->AsFloat;
				 if( m!= cnt->IC_min )j1->AddParamQuery("PP_Ic_min",cnt->IC_min);
				 m = Sql1->FieldByName("v_pp_ic_max")->AsFloat;
				 if( m!= cnt->IC_max )j1->AddParamQuery("PP_Ic_max",cnt->IC_max);

				 m = Sql1->FieldByName("v_pp_ua_min")->AsFloat;
				 if( m!= cnt->UA_min )j1->AddParamQuery("PP_Ua_min",cnt->UA_min);
				 m = Sql1->FieldByName("v_pp_ua_max")->AsFloat;
				 if( m!= cnt->UA_max )j1->AddParamQuery("PP_Ua_max",cnt->UA_max);
				 m = Sql1->FieldByName("v_pp_ub_min")->AsFloat;
				 if( m!= cnt->UB_min )j1->AddParamQuery("PP_Ub_min",cnt->UB_min);
				 m = Sql1->FieldByName("v_pp_ub_max")->AsFloat;
				 if( m!= cnt->UB_max )j1->AddParamQuery("PP_Ub_max",cnt->UB_max);
				 m = Sql1->FieldByName("v_pp_uc_min")->AsFloat;
				 if( m!= cnt->UC_min )j1->AddParamQuery("PP_Uc_min",cnt->UC_min);
				 m = Sql1->FieldByName("v_pp_uc_max")->AsFloat;
				 if( m!= cnt->UC_max )j1->AddParamQuery("PP_Uc_max",cnt->UC_max);

				 if( j1->QueryParams->Count > 0 )
				 {
					 j1->AddParamQuery("primechanie_pp","Изменен RT_MONITORом "+dt.FormatString("dd.mm.yy hh:nn:ss"));
					 j1->QueryUpdate( cnt->ppIdObject );
					 if(Log!=NULL)Log->Message("Изменен "+cnt->PP+" в справочник");
					 IsConfigChange = true;
					 CountChange++;
				 }


				 Count++;
			  }//end if
		  }//end for j
// Если ПП есть в базе, а XML его нет
/*
		  if( IsGorizont == false && pp_type == ArmName){
			j1->QueryClear();
			j1->AddParamQuery("IDTypeControl",-1);
			j1->QueryUpdate( pp_obj );
			if(Log!=NULL)Log->Message("Шкаф "+pp+" отключен от сервиса");
		  }
*/
	  }//end for
	  if(Log!=NULL)Log->Message("Связано "+IntToStr(Count)+" со справочником ПП");
	  if(Log!=NULL)Log->Message("Изменено "+IntToStr(CountChange)+" ПП справочнике");
	}//end try
	catch(Exception &e)
	{
		if(Log!=NULL)Log->Error("Ошибка связывания XML со справочником ПП "+e.Message);
	}
}

/**
* Связывание со справочником ПП
*
*@param Sql1 - квери к базе данных
*/
void __fastcall TKControllers::CreateNewPP(TKGlobalSem *glob)
{

   try
   {
	  int CountNew=0;
	  TUniQuery *Sql1 = glob->Query;
// Открываем журнал ПП через АПИ
	  TKJournalSem *j1 = glob->Journals->FindByKey("j_srpav_PP");
	  TDateTime dt = dt.CurrentDateTime();
// Находим ID-шник Горизонтовского типа ПП
	  int id_type = -1;
	  Sql1->Close();
	  Sql1->SQL->Clear();

	  Sql1->SQL->Add("SELECT * FROM j_sprav_type_control WHERE v_idtypecontrol=:a1");
	  Sql1->Params->Items[0]->AsString = ArmName;
	  Sql1->Open();
	  if( !Sql1->Eof )id_type = Sql1->FieldByName("id_object")->AsInteger;


// Проверяем ПП которых не было в списке
	  for(int j=0; j<MAX_CONTROLLER; j++)
	  {
		 TKController *cnt = Controllers[j];
		 if( cnt == NULL )continue;
		 if( cnt->PP != "" )continue;
		 cnt->PP         = cnt->AsoduName;
		 cnt->ppIdObject = -1;
		 cnt->ppStatus   = "Модем";
		 j1->QueryClear();
		 j1->AddParamQuery("pp",cnt->PP);
		 j1->AddParamQuery("id_kanal_sv",cnt->PP);
		 j1->AddParamQuery("status_pp",cnt->ppStatus);
		 j1->AddParamQuery("adres_pp_stis",cnt->AsoduAddress);
		 j1->AddParamQuery("IDTypeControl",id_type);
		 j1->AddParamQuery("CoordY",cnt->PosY);
		 j1->AddParamQuery("CoordX",cnt->PosX);
		 j1->AddParamQuery("PP_Ia_min",cnt->IA_min);
		 j1->AddParamQuery("PP_Ia_max",cnt->IA_max);
		 j1->AddParamQuery("PP_Ib_min",cnt->IB_min);
		 j1->AddParamQuery("PP_Ib_max",cnt->IA_max);
		 j1->AddParamQuery("PP_Ic_min",cnt->IC_min);
		 j1->AddParamQuery("PP_Ic_max",cnt->IC_max);
		 j1->AddParamQuery("PP_Ua_min",cnt->UA_min);
		 j1->AddParamQuery("PP_Ua_max",cnt->UA_max);
		 j1->AddParamQuery("PP_Ub_min",cnt->UB_min);
		 j1->AddParamQuery("PP_Ub_max",cnt->UA_max);
		 j1->AddParamQuery("PP_Uc_min",cnt->UC_min);
		 j1->AddParamQuery("PP_Uc_max",cnt->UC_max);
		 j1->AddParamQuery("primechanie_pp","Создан RT_MONITORом "+dt.FormatString("dd.mm.yy hh:nn:ss"));
		 cnt->ppIdObject = j1->QueryInsert();
		 if(Log!=NULL)Log->Message("Добавлен "+cnt->PP+" в справочник");
		 CountNew++;
		 IsConfigChange = true;

	  }//end for
	  if(Log!=NULL)Log->Message("Связано "+IntToStr(Count)+" со справочником ПП");
//	  if(Log!=NULL)Log->Message("Изменено "+IntToStr(CountChange)+" ПП справочнике");
	  if(Log!=NULL)Log->Message("Добавлено "+IntToStr(CountNew)+" ПП в справочник");
	}//end try
	catch(Exception &e)
	{
		if(Log!=NULL)Log->Error("Ошибка связывания XML со справочником ПП "+e.Message);
	}
}

/**
* Первоначальное заполнение значение из POLLSERVICEARCHIVE
*/
void __fastcall TKControllers::InitValues()
{

    try
    {
	  int n=0;
	  TUniQuery *Sql1 = Global->Query;
	  Sql1->Close();
	  Sql1->SQL->Clear();

	  Sql1->SQL->Add("SELECT * FROM pollservicearchive_rt WHERE  arh_id IN (");
	  Sql1->SQL->Add("SELECT MAX(arh_id) FROM pollservicearchive_rt WHERE arh_ua>=0 GROUP BY arh_pp)");
	  for(Sql1->Open();!Sql1->Eof; Sql1->Next() ){
		 UnicodeString pp = Sql1->FieldByName("arh_pp")->AsString;
		 TKController *cnt = FindByPP(pp);
		 if( cnt == NULL )continue;
		 n++;
		 cnt->ArchiveValuesParam->stat       = Sql1->FieldByName("arh_stat")->AsInteger;
		 cnt->ArchiveValuesParam->Problem    = Sql1->FieldByName("arh_problem")->AsInteger;
		 cnt->ArchiveValuesParam->DT         = Sql1->FieldByName("arh_dt")->AsDateTime;
		 cnt->ArchiveValuesParam->Link       = Sql1->FieldByName("arh_link")->AsInteger;
		 cnt->ArchiveValuesParam->mode       = Sql1->FieldByName("arh_mode")->AsInteger;
		 cnt->ArchiveValuesParam->Ia->Value  = Sql1->FieldByName("arh_ia")->AsFloat;
		 cnt->ArchiveValuesParam->Ia->Status = FLG_EXISTS;
		 cnt->ArchiveValuesParam->Ib->Value  = Sql1->FieldByName("arh_ib")->AsFloat;
	     cnt->ArchiveValuesParam->Ib->Status = FLG_EXISTS;
		 cnt->ArchiveValuesParam->Ic->Value  = Sql1->FieldByName("arh_ic")->AsFloat;
		 cnt->ArchiveValuesParam->Ic->Status = FLG_EXISTS;
		 cnt->ArchiveValuesParam->Ua->Value  = Sql1->FieldByName("arh_ua")->AsFloat;
		 cnt->ArchiveValuesParam->Ua->Status = FLG_EXISTS;
		 cnt->ArchiveValuesParam->Ub->Value  = Sql1->FieldByName("arh_ub")->AsFloat;
		 cnt->ArchiveValuesParam->Ub->Status = FLG_EXISTS;
		 cnt->ArchiveValuesParam->Uc->Value  = Sql1->FieldByName("arh_uc")->AsFloat;
		 cnt->ArchiveValuesParam->Uc->Status = FLG_EXISTS;
		 cnt->ArchiveValuesParam->CrashAll   = Sql1->FieldByName("arh_failabc")->AsInteger;
		 cnt->ArchiveValuesParam->DiscretAll = Sql1->FieldByName("arh_discretall")->AsInteger;
		 cnt->ArchiveValuesParam->DelayOn    = Sql1->FieldByName("arh_delay_on")->AsInteger;
		 cnt->ArchiveValuesParam->DelayOff   = Sql1->FieldByName("arh_delay_off")->AsInteger;
		 cnt->ArchiveValuesParam->Epran      = Sql1->FieldByName("arh_epran")->AsInteger;
		 cnt->ArchiveValuesParam->BKSAll     = Sql1->FieldByName("arh_bks")->AsInteger;
		 cnt->ArchiveValuesParam->Phasa      = Sql1->FieldByName("arh_phasa")->AsInteger;
		 cnt->ArchiveValuesParam->PhasaN     = Sql1->FieldByName("arh_phasan")->AsInteger;
         cnt->IsViewChange                   = true;

	  }
	  Sql1->Close();
	  if(Log!=NULL)Log->Message("Добавлено "+IntToStr(n)+" значений ПП");
	}//end try
	catch(Exception &e)
	{
		if(Log!=NULL)Log->Error("Ошибка формирование значение ПП "+e.Message);
	}
}




/**
* Проверка справочника контроллеров
*/
void _fastcall TKControllers::SaveControllers(TKGlobalSem *glob)
{
	try
	{
// Маркируем все контроллеры в справочнике и списке, чтобы найти неиспользованные
//	   for(int i=0; i<MAX_CONTROLLER; i++)
//	   {
//			TKController *cnt = Controllers[i];
//			if( cnt != NULL )cnt->IsUse = false;
//	   }
	  TUniQuery *Sql = glob->Query;

	   Sql->Close();
	   Sql->SQL->Clear();
	   Sql->SQL->Add("UPDATE j_kontrollers SET status_line = :a1, v_asodu_import_comments = :a2 WHERE v_asodu_arm = :a3");
	   Sql->Params->Items[0]->AsInteger = -1;
	   Sql->Params->Items[1]->AsString  = "";
	   Sql->Params->Items[2]->AsInteger = Arm;
	   Sql->ExecSQL();
// Проверяем контроллеры из массива
	   TKJournalSem *j_cnt   = new TKJournalSem("j_kontrollers",glob);

	   for(int i=0; i<MAX_CONTROLLER; i++)
	   {
			TKController *cnt = Controllers[i];
			if( cnt == NULL )continue;
			Sql->Close();
			Sql->SQL->Clear();
			Sql->SQL->Add("SELECT * FROM j_kontrollers WHERE v_asodu_arm = :a1 AND v_asodu_controllerid = :a2");
			Sql->Params->Items[0]->AsInteger = Arm;
			Sql->Params->Items[1]->AsString  = cnt->Object;
			Sql->Open();
// Если такого контроллера нет
			if( Sql->Eof )
			{
				j_cnt->QueryClear();
				j_cnt->AddParamQuery("Asodu_Arm",cnt->Arm);
				j_cnt->AddParamQuery("Asodu_ControllerId",cnt->Object);
				j_cnt->AddParamQuery("Asodu_NetAddr",cnt->NetAddr);
				j_cnt->AddParamQuery("Asodu_Active",cnt->AsoduActive);
				j_cnt->AddParamQuery("Asodu_Name",cnt->AsoduName);
				j_cnt->AddParamQuery("Asodu_Address",cnt->AsoduAddress);
				j_cnt->AddParamQuery("Asodu_Type",cnt->AsoduType);
				j_cnt->AddParamQuery("Asodu_key",cnt->AsoduKey);
				j_cnt->AddParamQuery("Asodu_TypeModem",cnt->AsoduTypeModem);
				j_cnt->AddParamQuery("Asodu_TypeController",cnt->AsoduTypeController);
				j_cnt->AddParamQuery("pp",cnt->ppIdObject);
				j_cnt->AddParamQuery("Asodu_import_comments","Новый контроллер");
				if(Log!=NULL)Log->Debug("Добавляем контроллер в справочник "+IntToStr(cnt->AsoduKey)+" ПП: "+cnt->AsoduName);
				j_cnt->QueryInsert();
			}
// Проверка изменений контроллера
			else
			{
				UnicodeString s = "";
				j_cnt->QueryClear();
				int id = Sql->FieldByName("id_object")->AsInteger;
				if( cnt->AsoduKey != Sql->FieldByName("v_asodu_key")->AsInteger )
				{
					s = s+"Key,";
					j_cnt->AddParamQuery("Asodu_key",cnt->AsoduName);
				}
				if( cnt->NetAddr != StrToIntDef(Sql->FieldByName("v_asodu_netaddr")->AsString,-1) )
				{
					s = s+"NetAddr,";
					j_cnt->AddParamQuery("Asodu_NetAddr",cnt->NetAddr);
				}
				if( cnt->AsoduName != Sql->FieldByName("v_asodu_name")->AsString )
				{
					s = s+"Название,";
					j_cnt->AddParamQuery("Asodu_Name",cnt->AsoduName);
				}
				if( cnt->AsoduAddress != Sql->FieldByName("v_asodu_address")->AsString )
				{
					s = s+"Адрес,";
					j_cnt->AddParamQuery("Asodu_Address",cnt->AsoduAddress);
				}
				if( cnt->AsoduType != Sql->FieldByName("v_asodu_type")->AsString )
				{
					s = s+"Прошивка,";
					j_cnt->AddParamQuery("Asodu_Type",cnt->AsoduType);
				}
				if( cnt->AsoduTypeModem != Sql->FieldByName("v_asodu_typemodem")->AsString )
				{
					s = s+"Тип модема,";
					j_cnt->AddParamQuery("Asodu_TypeModem",cnt->AsoduTypeModem);
				}
				if( cnt->AsoduTypeController != Sql->FieldByName("v_asodu_typecontroller")->AsString )
				{
					s = s+"Тип контроллера,";
					j_cnt->AddParamQuery("Asodu_TypeController",cnt->AsoduTypeController);

				}
				if( cnt->ppIdObject != Sql->FieldByName("o_pp")->AsInteger )
				{
					s = s+"ПП,";
					j_cnt->AddParamQuery("pp",cnt->ppIdObject);
				}
				if( s != "" )
				{
					if(Log!=NULL)Log->Debug("Изменен контроллер "+IntToStr(cnt->AsoduKey)+" ПП: "+cnt->AsoduName);
					j_cnt->AddParamQuery("Asodu_import_comments","Изменены "+s);
				}
				TKValueSem *v   = new TKValueSem;
				v->AsInteger    = 0;
				v->Type         = psdtInteger;
				j_cnt->QueryParams->AddObject("status_line",v);
				Sql->Close();
				j_cnt->QueryUpdate(id);
			}
	   }
// Удаляем неиспользованные контроллеры
	   Sql->Close();
	   Sql->SQL->Clear();
	   Sql->SQL->Add("DELETE FROM j_kontrollers WHERE v_asodu_arm = :a1 AND status_line = :a2");
	   Sql->Params->Items[0]->AsInteger = Arm;
	   Sql->Params->Items[1]->AsInteger = -1;
	   Sql->ExecSQL();
// Формируем отчет по изменениям
	}
	catch(Exception &e)
	{
		if(Log!=NULL)Log->Error("Ошибка сохранения справочника контроллеров "+e.Message);

	}
}



/**
* Связывание со справочником ПП
*
*@param Sql1 - квери к базе данных
*/
void __fastcall TKControllers::LinkToAlarm(TUniQuery *Sql1)
{

// Запрос к базе
	  Sql1->Close();
	  Sql1->SQL->Clear();


	  Sql1->SQL->Add("SELECT sp.v_asoduparam_shortname,sa.* FROM j_sprav_param_asodu sp,alarm_sprav sa");
	  Sql1->SQL->Add("WHERE sp.id_object(+) = sa.o_asoduparam_shortname AND v_alarm_sprav_onoff = :a1");
	  Sql1->Params->Items[0]->AsString = "Да";
	  for( Sql1->Open(); !Sql1->Eof; Sql1->Next() )
	  {
		  UnicodeString sname = Sql1->FieldByName("v_asoduparam_shortname")->AsString;
		  TKAlarmSprav *alarm_spr   = new TKAlarmSprav;
		  alarm_spr->IdObject  = Sql1->FieldByName("id_object")->AsInteger;
		  alarm_spr->Type      = Sql1->FieldByName("v_alarm_type_event")->AsString;
		  alarm_spr->ShortName = Sql1->FieldByName("v_alarm_name")->AsString;
		  alarm_spr->Name      = Sql1->FieldByName("v_alarm_sprav_name")->AsString;
		  alarm_spr->Script    = Sql1->FieldByName("v_alarm_sprav_algoritm")->AsString;
		  alarm_spr->AutoCheck = StrToIntDef(Sql1->FieldByName("v_alarm_sprav_time_kvit")->AsString,0);


		  Alarms->Add((void *)alarm_spr);

// Ищем в списке контролеров
		  for(int i=0; i<MAX_CONTROLLER; i++)
		  {
			  TKController *cnt = Controllers[i];
			  if( cnt == NULL )continue;

			  TKAlarm *alarm    = new TKAlarm;
			  alarm->AlarmSprav = alarm_spr;
			  alarm->isEvent    = false;
			  alarm->isAlarm    = false;
			  alarm->tmEvent    = 0;
			  alarm->tmAlarm    = 0;
			  alarm->countEvent = 0;
			  alarm->Param       = NULL;
//			  if( sname == u"Iа" ){
//                 int a=1;
//			  }
			  alarm->Param      = FindByPPAndParam( cnt, sname );
//			  if( alarm->Param == NULL ){
 //                int a=1;
//			  }
			  alarm->Name       = alarm_spr->Name;
			  cnt->Alarms->Add(alarm);

			  if( alarm->Param == NULL )continue;
			  TKParam *param    = alarm->Param;

			  param->AlarmFlag = true;
			  param->AlarmText      = Sql1->FieldByName("v_alarm_sprav_name")->AsString;
			  param->AlarmAlgorithm = Sql1->FieldByName("v_alarm_sprav_algoritm")->AsString;
			  param->AlarmEnableFlag    = false;
			  param->AlarmNowFlag       = false;
			  param->AlarmCheckFlag     = false;
			  if( Sql1->FindField("v_alarm_sprav_time_kvit") != NULL )
				 param->TimeWithoutCheck   = StrToIntDef(Sql1->FieldByName("v_alarm_sprav_time_kvit")->AsString,0);
			  else param->TimeWithoutCheck = 0;
			  param->IdObject       = Sql1->FieldByName("o_asoduparam_shortname")->AsInteger;
			  param->AlarmIdObject  = Sql1->FieldByName("id_object")->AsInteger;
		  }

	  }

// Заполняем текущими тревогами
	  Sql1->Close();
	  Sql1->SQL->Clear();
	  Sql1->SQL->Add("SELECT t.*,s.v_alarm_type_event FROM j_trevogi t,alarm_sprav s");
	  Sql1->SQL->Add("WHERE  t.o_alarm_sprav_id = s.id_object");
	  Sql1->SQL->Add("AND s.v_alarm_sprav_onoff = 'Да' AND v_date_time_sniat_trev IS NULL");
	  for( Sql1->Open(); !Sql1->Eof; Sql1->Next() ){
		 TKController *cnt = FindByIdPP( Sql1->FieldByName("o_pp")->AsInteger );
		 if( cnt == NULL )continue;
 // Ищем тревогу
		 for( int i=0; i<cnt->Alarms->Count; i++ ){
			TKAlarm *alarm = (TKAlarm *)cnt->Alarms->Items[i];
			if( alarm == NULL )continue;
			if( alarm->AlarmSprav->IdObject == Sql1->FieldByName("o_alarm_sprav_id")->AsInteger ){
			   alarm->tmEvent  = Sql1->FieldByName("v_date_time_voz_trev")->AsDateTime;
			   alarm->Type     = Sql1->FieldByName("v_tip_trevogi")->AsString;
			   alarm->isEvent  = true;
			   alarm->isAlarm  = true;
			   continue;
			}
		 }
	  }
	  Sql1->Close();


}



/**
* Инициализация контроллеров с параметрами по номеру АРМа
*
*@param Sql1 - квери к базе данных
*@param arm - номер PollServica, если -1 - то все
*/
void __fastcall TKControllers::InitControllersAndParams(TUniQuery *Sql1, int arm)
{

// Определяем количества Контроллеров
	  Arm = arm;
	  TDateTime d1 = d1.CurrentDateTime();
	  Sql1->Close();
      Sql1->SQL->Clear();
      Sql1->SQL->Add("select count(*) from j_kontrollers");
      Sql1->SQL->Add(" where v_asodu_arm=:a1");
	  Sql1->Params->Items[0]->AsInteger = arm;
	  int n = 0;
      Sql1->Open();
	  if( !Sql1->Eof )  n = Sql1->Fields->Fields[0]->AsInteger;
	  if( Log!=NULL )Log->StartBar(0,n);
// Запрос к базе
	  Sql1->Close();
	  Sql1->SQL->Clear();
	  Sql1->SQL->Add("SELECT pp.*,k.*,p.*,sp.*,sa.*,sa.id_object aid");
	  Sql1->SQL->Add("FROM j_srpav_pp pp, j_kontrollers k, j_param_asodu p, j_sprav_param_asodu sp,alarm_sprav sa");
	  Sql1->SQL->Add("WHERE pp.id_object = k.o_pp");
	  Sql1->SQL->Add(" AND k.id_object = p.o_asodu_key");
	  Sql1->SQL->Add(" AND sp.id_object = p.o_asoduparam_shortname");
	  Sql1->SQL->Add(" AND sp.id_object = sa.o_asoduparam_shortname(+)");
	  Sql1->SQL->Add(" AND v_asodu_arm=:a1");
	  Sql1->SQL->Add(" ORDER BY pp.v_pp,p.v_asoduparam_n");
	  Sql1->Params->Items[0]->AsInteger = arm;
	  Sql1->Open();
	  UnicodeString pp = "xxx";
	  TKController *cnt;
	  TKParam *param;
	  int nc=0;
	  Count = 0;
	  for( int i=0; !Sql1->Eof; i++ )
	  {
		 UnicodeString pp1 = Sql1->FieldByName("v_pp")->AsString;
// Создаем новый контроллер
         if( pp != pp1 )
		 {
			int net          = StrToIntDef(Sql1->FieldByName("v_asodu_netaddr")->AsString,-1);
			if( net < 0 && net >= MAX_CONTROLLER )continue;
			cnt              = new TKController;
		    cnt->IntegralValue = NULL;
			cnt->ArchiveValuesParam = new TKArchiveValuesParam((TObject *)this);
			cnt->Alarms      = new TList();
		    cnt->ppStatusAF   = "";
			cnt->MapObject   = NULL;
			cnt->Component   = NULL;
			cnt->Status      = TKControllerStatus::csNone;
		    cnt->IsViewChange= false;
			cnt->UpdateDT    = 0;
			cnt->CheckDT     = 0;
			cnt->EventDT     = 0;
			cnt->IsNewValue  = true;
			for( int j=0; j<MAX_PARAM; j++ )cnt->Params[j] = NULL;
            cnt->PP          = pp1;
			cnt->ppIdObject  = Sql1->FieldByName("o_pp")->AsInteger;
			cnt->Object      = StrToIntDef(Sql1->FieldByName("v_asodu_controllerid")->AsString,n);
            cnt->NetAddr     = net;
            cnt->Arm         = StrToIntDef(Sql1->FieldByName("v_asodu_arm")->AsString,1);
            cnt->ppStatus    = Sql1->FieldByName("v_status_pp")->AsString;
            cnt->ppBalans    = Sql1->FieldByName("v_nenabalans")->AsString;
			Controllers[net] = cnt;
			nc++;
			Count++;
            pp = pp1;
			if(Log!=NULL)Log->SetBar(nc);
			Application->ProcessMessages();
         }
// Создаем параметр
        int n                = StrToIntDef(Sql1->FieldByName("v_asoduparam_n")->AsString,-1);
		if( n<0 || n >= MAX_PARAM )continue;
		param                 = new TKParam;
		param->Num            = n;
		param->Name           = Sql1->FieldByName("v_spravparam_name")->AsString;
		param->ShortName      = AliasParamName(Sql1->FieldByName("v_asoduparam_shortname")->AsString);
		param->Status         = psNone;
		param->ConstDT     = 0;
		param->ConstCount  = 0;
		param->AlarmExceptionFlag = false;
		UnicodeString s       = Sql1->FieldByName("v_Alarm_sprav_onoff")->AsString;
		if( s == "Да" )param->AlarmFlag = true;
		else param->AlarmFlag = false;
		param->AlarmText      = Sql1->FieldByName("v_alarm_sprav_name")->AsString;
		param->AlarmAlgorithm = Sql1->FieldByName("v_alarm_sprav_algoritm")->AsString;
		param->AlarmEnableFlag    = false;
		param->AlarmNowFlag       = false;
		param->AlarmCheckFlag     = false;
		if( Sql1->FindField("v_alarm_sprav_time_kvit") != NULL )
		   param->TimeWithoutCheck   = StrToIntDef(Sql1->FieldByName("v_alarm_sprav_time_kvit")->AsString,0);
		else param->TimeWithoutCheck = 0;
		param->IdObject       = Sql1->FieldByName("o_asoduparam_shortname")->AsInteger;
		param->AlarmIdObject  = Sql1->FieldByName("aid")->AsInteger;
		param->ChangeDT       = d1;
		param->Controller     = (void *)cnt;
		cnt->Params[n]        = param;
		Sql1->Next();
     }
	 Sql1->Close();
	 if( AlarmException )CheckException(Sql1);
	 CheckAlarm(Sql1);
	 if(Log!=NULL)Log->StopBar();

}

/**
* Проверить все тревоги
*
*@param Sql1 - класс запроса к базе
*/
void __fastcall TKControllers::CheckAlarm(TUniQuery *Sql1)
{
	try
	{
		TDateTime dt = dt.CurrentDateTime();
		Sql1->Close();
		Sql1->SQL->Clear();
		Sql1->SQL->Add("SELECT t.id_object tid,pp.v_pp,s.v_alarm_sprav_name,sp.v_asoduparam_shortname,t.*");
		Sql1->SQL->Add("FROM j_trevogi t,j_srpav_pp pp,alarm_sprav s,j_sprav_param_asodu sp");
		Sql1->SQL->Add("WHERE pp.id_object = t.o_pp");
		Sql1->SQL->Add("AND t.o_alarm_sprav_id = s.id_object");
		Sql1->SQL->Add("AND s.v_alarm_sprav_onoff = \'Да\'");
		Sql1->SQL->Add("AND sp.id_object = s.o_asoduparam_shortname");
		Sql1->SQL->Add("AND ( t.v_alarm_enable = \'Да\' OR t.v_alarm_enable = \'Нет\' AND t.v_alarm_check = \'Нет\' )");
		Sql1->SQL->Add("AND t.id_object NOT IN");
		Sql1->SQL->Add("( SELECT t1.id_object FROM j_trevogi t1, j_Alarm_Exception e1");
		Sql1->SQL->Add("  WHERE t1.o_pp = e1.o_pp AND t1.o_alarm_sprav_id = e1.o_alarm_sprav_id");
		Sql1->SQL->Add("  AND e1.v_alarmexception_dateend > :a1 )");
		Sql1->Params->Items[0]->AsDateTime = dt;
		for( Sql1->Open(); !Sql1->Eof; Sql1->Next() )
		{
		   UnicodeString pp  = Sql1->FieldByName("v_pp")->AsString;
		   UnicodeString nam = Sql1->FieldByName("v_asoduparam_shortname")->AsString;
		   TKController *cnt = FindByPP( pp );
		   if( cnt == NULL )continue;
		   TKParam *par      = FindByPPAndParam( cnt, nam );
		   if( par == NULL )continue;
		   UnicodeString s1 = Sql1->FieldByName("v_alarm_enable")->AsString;
		   UnicodeString s2 = Sql1->FieldByName("v_alarm_check")->AsString;
		   par->AlarmEnableFlag                = true;
		   if( s1 == "Да" )par->AlarmNowFlag   = true;
		   if( s2 == "Да" )par->AlarmCheckFlag = true;
		   if(Log!=NULL)Log->Debug("Найдена тревога "+pp+" "+nam);

		}
		Sql1->Close();
	}
	catch(Exception &e)
	{
		if(Log!=NULL)Log->Error("Ошибка проверки  тревог "+e.Message);
	}
}


/**
* Проверить параметры на наличие исключений из аварий
*
*@param Sql1 - класс запроса к базе
*/
void __fastcall TKControllers::CheckException(TUniQuery *Sql1)
{
	try
	{
		TDateTime dt = dt.CurrentDateTime();
		Sql1->Close();
		Sql1->SQL->Clear();
		Sql1->SQL->Add("SELECT UNIQUE pp.v_pp,sp.v_asoduparam_shortname");
		Sql1->SQL->Add("FROM j_trevogi t1, j_Alarm_Exception e1,j_srpav_pp pp,j_sprav_param_asodu sp");
		Sql1->SQL->Add("WHERE t1.o_pp = e1.o_pp AND t1.o_alarm_sprav_id = e1.o_alarm_sprav_id");
		Sql1->SQL->Add("AND pp.id_object = t1.o_pp AND sp.id_object = t1.o_asoduparam_shortname");
		Sql1->SQL->Add("AND e1.v_alarmexception_dateend > :a1");
		Sql1->Params->Items[0]->AsDateTime = dt;
		for( Sql1->Open(); !Sql1->Eof; Sql1->Next() )
		{
		   UnicodeString pp  = Sql1->FieldByName("v_pp")->AsString;
		   UnicodeString nam = Sql1->FieldByName("v_asoduparam_shortname")->AsString;
//		   if(Log=NULL)Log->Error("Исключение тревоги "+pp+" "+nam);
		   TKController *cnt = FindByPP( pp );
		   if( cnt == NULL )continue;
		   TKParam *par      = FindByPPAndParam( cnt, nam );
		   if( par == NULL )continue;
		   par->AlarmExceptionFlag = true;
		   if(Log!=NULL)Log->Debug("Исключение тревоги "+pp+" "+nam);
		   Application->ProcessMessages();
		}
		Sql1->Close();
	}
	catch(Exception &e)
	{
		if(Log!=NULL)Log->Error("Ошибка проверки исключения тревог "+e.Message);
	}
}

/**
* Инициализация контроллеров с параметрами
*
*@param Sql1 - квери к базе данных
*/
void __fastcall TKControllers::InitControllers(TUniQuery *Sql1)
{
    InitControllers(Sql1, "ORDER BY pp.v_pp");
}

/**
* Инициализация контроллеров с параметрами
*
*@param Sql1 - квери к базе данных
*/
void __fastcall TKControllers::InitControllers(TUniQuery *Sql1,UnicodeString order_by)
{
    // Определяем количества Контроллеров
    Sql1->Close();
    Sql1->SQL->Text = "select count(*) from j_kontrollers";

    int n = 0;
	Sql1->Open();
	if( !Sql1->Eof )  n = Sql1->Fields->Fields[0]->AsInteger;
	if(Log!=NULL)Log->StartBar(0,n);

    // Запрос к базе
	  Sql1->Close();
	  Sql1->SQL->Clear();
	  Sql1->SQL->Add("SELECT ar.v_admin_rayon,pp.*,k.*,g.v_graphonoff,g.v_CodGraphOnOff,r.val_1,tc.v_idtypecontrol");
	  Sql1->SQL->Add("FROM j_srpav_pp pp, j_kontrollers k,j_ListGraph g,journal_1 r,j_sprav_type_control tc,j_sprav_admin_rayonov ar");
	  Sql1->SQL->Add("WHERE pp.id_object = k.o_pp(+) AND pp.o_graphonoff = g.id_object (+)");
	  Sql1->SQL->Add("AND pp.o_rayon = r.id_object(+) AND pp.o_idtypecontrol = tc.ID_OBJECT(+)");
	  Sql1->SQL->Add("AND pp.o_admin_rayon = ar.id_object(+) ");
	  Sql1->SQL->Add(order_by);
	  Sql1->Open();
	  TKController *cnt;
	  TKParam *param;
	  //TKArchiveValuesParam *ValuesParam;
	  Count = 0;
//      int nc=0;
	  for( int i=0; !Sql1->Eof; i++ )
	  {
		 if( i >= MAX_CONTROLLER )
            break;

		 UnicodeString pp1   = Sql1->FieldByName("v_pp")->AsString;
		 int net          = StrToIntDef(Sql1->FieldByName("v_asodu_netaddr")->AsString,-1);
		 cnt              = new TKController;
		 cnt->IntegralValue = NULL;
		 cnt->IsViewChange = true;
		 cnt->ArchiveValuesParam = new TKArchiveValuesParam((TObject *)this);
		 cnt->Alarms      = new TList();
		 cnt->ppStatusAF   = "";
		 cnt->MapObject   = NULL;
		 cnt->Component   = NULL;
		 cnt->ParentCascade = "";
		 cnt->CountCascade = 0;
		 cnt->Status      = TKControllerStatus::csNone;
		 cnt->UpdateDT    = 0;
		 cnt->CheckDT     = 0;
		 cnt->EventDT     = 0;
		 cnt->IsNewValue  = true;
		 for( int j=0; j<MAX_PARAM; j++ )cnt->Params[j] = NULL;
		 cnt->PP          = pp1;
		 if( Sql1->FindField("v_name_pp") != NULL )
		   cnt->NameTP    = Sql1->FieldByName("v_name_pp")->AsString;
		 if( Sql1->FindField("v_name_pp_cascad") != NULL )
		   cnt->NamePP    = Sql1->FieldByName("v_name_pp_cascad")->AsString;
//         if(Log)Log->Error("ПП="+cnt->PP+",название="+cnt->NamePP+",ТП="+cnt->NameTP);
		 cnt->ppIdObject  = Sql1->FieldByName("o_pp")->AsInteger;
		 cnt->Object      = StrToIntDef(Sql1->FieldByName("v_asodu_controllerid")->AsString,n);
		 cnt->NetAddr     = net;
		 cnt->Arm         = StrToIntDef(Sql1->FieldByName("v_asodu_arm")->AsString,1);
		 cnt->ppStatus    = Sql1->FieldByName("v_status_pp")->AsString;
		 cnt->ppBalans    = Sql1->FieldByName("v_nenabalans")->AsString;
		 cnt->Graphonoff  = Sql1->FieldByName("v_graphonoff")->AsString;
		 cnt->CodGraphOnOff = Sql1->FieldByName("v_CodGraphOnOff")->AsString;
		 cnt->ppRayon     = Sql1->FieldByName("val_1")->AsString;
		 cnt->ppAdmRayon  = Sql1->FieldByName("v_admin_rayon")->AsString;
		 if( Sql1->FindField("v_type_dogovor_pp") != NULL )
			cnt->ppServiceType  = Sql1->FieldByName("v_type_dogovor_pp")->AsString;
		 if( Sql1->FindField("v_service_organization_pp") != NULL )
			cnt->ppServiceName  = Sql1->FieldByName("v_service_organization_pp")->AsString;
		 cnt->ppTextDisp     = Sql1->FieldByName("v_primechanie_pp_disp")->AsString;

		 cnt->AsoduActive    = Sql1->FieldByName("v_asodu_active")->AsInteger;
		 cnt->AsoduAddress   = Sql1->FieldByName("v_asodu_address")->AsString;
		 cnt->AsoduType      = Sql1->FieldByName("v_asodu_type")->AsString;
		 cnt->AsoduTypeModem = Sql1->FieldByName("v_asodu_typemodem")->AsString;

		 if( Sql1->FindField("v_Head_pp_cascad") != NULL )
		   cnt->ParentCascade   = Sql1->FieldByName("v_Head_pp_cascad")->AsString;
		 cnt->ppAddress   =  Sql1->FieldByName("v_adres_pp_stis")->AsString;
		 if (Global->CheckOfExistsColumn("j_srpav_pp","v_pp_type_commut"))
			cnt->SwitchType  =  Sql1->FieldByName("v_pp_type_commut")->AsString;
		 if (Global->CheckOfExistsColumn("j_srpav_pp","v_fider_count_out"))
			cnt->FiderCount =  Sql1->FieldByName("v_fider_count_out")->AsInteger;
		  if (Global->CheckOfExistsColumn("j_srpav_pp","v_bcs"))
			cnt->BKS =  Sql1->FieldByName("v_bcs")->AsString;
		 if( cnt->BKS == "Да" )cnt->IsBKS = true;
		 cnt->IA_min	  =  Sql1->FieldByName("v_pp_ia_min")->AsFloat;
		 cnt->IA_max	  =  Sql1->FieldByName("v_pp_ia_max")->AsFloat;
		 cnt->IB_min      =  Sql1->FieldByName("v_pp_ib_min")->AsFloat;
		 cnt->IB_max	  =  Sql1->FieldByName("v_pp_ib_max")->AsFloat;
		 cnt->IC_min	  =  Sql1->FieldByName("v_pp_ic_min")->AsFloat;
		 cnt->IC_max      =  Sql1->FieldByName("v_pp_ic_max")->AsFloat;
		 cnt->UA_min 	  =  Sql1->FieldByName("v_pp_ua_min")->AsFloat;
		 cnt->UA_max 	  =  Sql1->FieldByName("v_pp_ua_max")->AsFloat;
		 cnt->UB_min  	  =  Sql1->FieldByName("v_pp_ub_min")->AsFloat;
		 cnt->UB_max	  =  Sql1->FieldByName("v_pp_ub_max")->AsFloat;
		 cnt->UC_min	  =  Sql1->FieldByName("v_pp_uc_min")->AsFloat;
		 cnt->UC_max	  =  Sql1->FieldByName("v_pp_uc_max")->AsFloat;

		 cnt->ppAsoduName =	 Sql1->FieldByName("v_idtypecontrol")->AsString;
		 cnt->PosX        =  Sql1->FieldByName("v_coordx")->AsInteger;
		 cnt->PosY        =  Sql1->FieldByName("v_coordy")->AsInteger;

//		 cnt->ArchiveValuesParam = new TKArchiveValuesParam((TObject *)this);

		 Controllers[i] = cnt;
		 if(Log!=NULL)Log->SetBar(i);
		 Application->ProcessMessages();
		 Count++;
		 Sql1->Next();
	 }
	 Sql1->Close();
	 if(Log!=NULL)Log->StopBar();
	 if(Log!=NULL)Log->Message("Считано "+IntToStr(Count)+" пунктов питания");
}


/**
* Установить каскады у контроллеров
*/
void __fastcall TKControllers::SetCascades()
{
// Перебираем контроллеры
	for(int i=0; i<MAX_CONTROLLER; i++)
	{
		TKController *cnt = Controllers[i];
		if( cnt == NULL )continue;
// Если контроллер модемный
		if( cnt->ppStatus == "" || cnt->ppStatus == "Модем"|| cnt->ppStatus =="Модем Горизонт")
		{
			cnt->CountCascade = 0;
			for(int j=0; j<MAX_CONTROLLER; j++)
			{
				if( i==j )continue;
				TKController *cnt1 = Controllers[j];
				if( cnt1 == NULL )continue;
// Ищем каскады
				if( cnt1->ppStatus == "Каскад" && cnt1->ParentCascade == cnt->PP )
				   cnt->Cascade[cnt->CountCascade++] = cnt1;
			}
        }
	}
}

/**
* Формировать Alias параметра
*/
UnicodeString __fastcall TKControllers::AliasParamName( UnicodeString name )
{
   if( name == "НС_А"   )return "СимА";
   if( name == "НС_В"   )return "СимВ";
   if( name == "НС_С"   )return "СимС";
   if( name == "НС_a"   )return "СимА";
   if( name == "НС_b"   )return "СимВ";
   if( name == "НС_c"   )return "СимС";
   if( name == "Пер_a"  )return "ПерА";
   if( name == "Пер_b"  )return "ПерВ";
   if( name == "Пер_c"  )return "ПерС";
   if( name == "КЗ_a"   )return "КЗА";
   if( name == "КЗ_b"   )return "КЗВ";
   if( name == "КЗ_c"   )return "КЗС";

   if( name == "Сост_a" )return "СостА";
   if( name == "Сост_b" )return "СостВ";
   if( name == "Сост_c" )return "СостС";

   if( name == "I_a" )return "Ia";
   if( name == "I_b" )return "Ib";
   if( name == "I_c" )return "Ic";
   if( name == "U_a" )return "Ua";
   if( name == "U_b" )return "Ub";
   if( name == "U_c" )return "Uc";
   if( name == "220 А")return "220_a";
   if( name == "220 В")return "220_b";
   if( name == "220 С")return "220_c";
   return name;
}

/**
* Ищем параметр по ПП
*
*@param pp - ПП
*/
TKController * __fastcall TKControllers::FindByPP( UnicodeString pp )
{
   TKController *cnt;
   for( int i=0; i<MAX_CONTROLLER; i++)
   {
	  cnt = Controllers[i];

	  if( cnt == NULL )continue;
	  if( cnt->PP == pp )return cnt;

   }
   return NULL;
}

/**
* Ищем параметр по ПП
*
*@param id_pp - id_object ПП
*/
TKController * __fastcall TKControllers::FindByIdPP( int id_pp )
{
   TKController *cnt;
   for( int i=0; i<MAX_CONTROLLER; i++)
   {
	  cnt = Controllers[i];

	  if( cnt == NULL )continue;
	  if( cnt->ppIdObject == id_pp )return cnt;

   }
   return NULL;
}



/**
* Ищем параметр по ПП и короткому имени
*
*@param cnt - структура с контроллером
*@param sname - короткое имя параметра типа Ia
*/
TKParam * __fastcall TKControllers::FindByPPAndParam( TKController *cnt, UnicodeString sname )
{
//   TKController *cnt = FindByPP( pp );
   if( cnt == NULL )return NULL;
   for( int i=0; i<MAX_PARAM; i++)
   {
	  TKParam *par = cnt->Params[i];
	  if( par == NULL )continue;
	  if( par->ShortName == sname )return par;
   }
   return NULL;
}


/**
* Чтение Фамилии диспетчера
*
* @param dt - интересуемая дата
* @return Возвращает f\фамилию диспетчера или NULL
*
*/
UnicodeString __fastcall TKControllers::GetDispetcher( TUniQuery *Sql, TDateTime dt )
{
   try
   {
	  TUniQuery *Sql1 = NULL;
	  UnicodeString ret = "";
	  if( Global != NULL )Sql1 = Global->Query;
	  else Sql1 = Sql;
	  if( !Sql1->Connection->Connected )
	  {
		  if( Log!= NULL )Log->Error("Нет соединения с БД. Пытаемся восстановить");
		  Sql1->Connection->PerformConnect(true);
	  }
	  if( !Sql1->Connection->Connected )
	  {
		  if( Log!= NULL )Log->Error("Невозможно установить соединение с БД.");
		  return ret;
	  }
	  TDateTime d1 = 0;
	  if( dt <= d1 )return ret;
	  d1 = DateOf(dt);
	  TDateTime d2 = IncDay(d1,1);;

	  Sql1->Close();
	  Sql1->SQL->Clear();
	  Sql1->SQL->Add("SELECT v_fio_grafik FROM  j_grafik_raboti");
	  Sql1->SQL->Add("WHERE v_date_grafik >= :a0 AND v_date_grafik < :a1");
	  Sql1->SQL->Add("AND v_dolgnost_grafik = :a2 AND v_galochka = :a3");
	  Sql1->Params->Items[0]->AsDateTime = d1;
	  Sql1->Params->Items[1]->AsDateTime = d2;
	  Sql1->Params->Items[2]->AsString   = "Диспетчер";

	  Sql1->Params->Items[3]->AsString   = "1,2";
	  Sql1->Open();

	  if( !Sql1->Eof )	  {
		 ret  = Sql1->FieldByName("v_fio_grafik")->AsString;
	  }
	  Sql1->Close();
	  return ret;
   }
   catch(Exception &ex)
   {
	  if(Log!=NULL)Log->Error("Ошибка чтения фамилли диспетчера "+ex.Message);
   }
}




/**
* Чтение графика включения/выключения на день
*
* @param dt - интересуемая дата
* @param t_on - указатель, куда возвращается время включения
* @param t_off - указатель, куда возвращается время выключения
* @return Возвращает false если ничего не найдено
*
*/
bool __fastcall TKControllers::GetShedule( TUniQuery *Sql, TDateTime dt, TDateTime *t_on, TDateTime *t_off )
{
   try
   {
	  TUniQuery *Sql1 = NULL;
	  if( Global != NULL )Sql1 = Global->Query;
	  else Sql1 = Sql;
	  if( !Sql1->Connection->Connected )
	  {
		  if( Log!= NULL )Log->Error("Нет соединения с БД. Пытаемся восстановить");
		  Sql1->Connection->PerformConnect(true);
	  }
	  if( !Sql1->Connection->Connected )
	  {
		  if( Log!= NULL )Log->Error("Невозможно установить соединение с БД.");
		  return false;
	  }
	  TDateTime d1 = 0;
	  if( dt <= d1 )return false;
	  UnicodeString tab_name ="j_graf_chas_goren";
	  bool flag = false;
	  d1 = DateOf(dt);
	  TDateTime d2 = IncDay(d1,1);;
// Определяем имена полей по названию параметров
	  UnicodeString f_date  = "v_calendar_date";
	  UnicodeString f_t_on  = "v_vkl_bez_uchet_perehod";
	  UnicodeString f_t_off = "v_vikl_bez_uchet_perehod";

	  Sql1->Close();
	  Sql1->SQL->Clear();
	  Sql1->SQL->Add("SELECT * FROM "+tab_name);
	  Sql1->SQL->Add("WHERE "+f_date+" >= :a0 AND "+f_date+" < :a1");
	  Sql1->Params->Items[0]->AsDateTime = d1;
	  Sql1->Params->Items[1]->AsDateTime = d2;
	  Sql1->Open();
	  if( !Sql1->Eof )
	  {
			*t_on  = d1 + 1 + Sql1->FieldByName(f_t_on)->AsDateTime -
				  int(Sql1->FieldByName(f_t_on)->AsDateTime);
			*t_off = d1 + 1 + Sql1->FieldByName(f_t_off)->AsDateTime -
				  int(Sql1->FieldByName(f_t_off)->AsDateTime);
			if( *t_on > *t_off )*t_on = *t_on - 1;
			flag = true;
	  }
	  Sql1->Close();
	  return flag;
   }
   catch(Exception &ex)
   {
	  if(Log!=NULL)Log->Error("Ошибка определения графика горения "+ex.Message);
   }
}


/**
* Чтение графика включения/выключения на день и 24 часовой смены
*
* @param dt - интересуемая дата
* @param t_on - указатель, куда возвращается время включения
* @param t_off - указатель, куда возвращается время выключения
* @return Возвращает false если ничего не найдено
*
*/
bool __fastcall TKControllers::GetShedule( TUniQuery *Sql,TDateTime d, TDateTime *t_off_prev,TDateTime *t_on_now, TDateTime *t_off_now )
{
   try
   {
		TUniQuery *Sql1 = NULL;
		if( Global != NULL )Sql1 = Global->Query;
		else Sql1 = Sql;
		TDateTime t_off1,t_on1,t_off2,t_on2;
// Находим вкл/выкл за заданную дату/время
		if (!GetShedule(Sql1,d,&t_on2,&t_off2))
		{
			if (Log != NULL) Log->Error("Не могу прочитать график суточных параметров");
			return false;
		}
// Проверяем, если свет еще горит, то дата предыдущая
		if( d < t_off2-1 )
		{
			d = d -1;
			if (!GetShedule(Sql1,d,&t_on2,&t_off2))
			{
				if (Log != NULL) Log->Error("Не могу прочитать график суточных параметров");
				return false;
			}
		}
// Находим график за предыдущую дату
		if (!GetShedule(Sql1,d-1,&t_on1,&t_off1))
		{
			if (Log != NULL) Log->Error("Не могу прочитать график суточных параметров за");
			return false;
		}
		UnicodeString s_on1  = t_on1.FormatString("dd.mm.yy hh:nn:ss");
		UnicodeString s_off1 = t_off1.FormatString("dd.mm.yy hh:nn:ss");
		UnicodeString s_on2  = t_on2.FormatString("dd.mm.yy hh:nn:ss");
		UnicodeString s_off2 = t_off2.FormatString("dd.mm.yy hh:nn:ss");
		*t_off_prev           = t_off1;
		*t_on_now             = t_on2;
		*t_off_now            = t_off2;
		//Log->Debug("Горен: "+s_on2+" - "+s_off2);
		//Log->Debug("Сутки: "+s_off1+" - "+s_off2);
   }
   catch(Exception &ex)
   {
	  if (Log != NULL) Log->Error("Ошибка определения графика горения "+ex.Message);
   }
   return true;
}



/**
* Определения количества фаз в заданном режиме
*/
int __fastcall TKControllers::HowManyPhases( TKController *cnt, TKModeRT mode )
{
   return HowManyPhases(cnt->CodGraphOnOff, mode);
}

/**
* Определения количества фаз в заданном режиме
*/
int __fastcall TKControllers::HowManyPhases( UnicodeString cod, TKModeRT mode )
{
   try
   {
	   int count = 0;
	   switch(mode)
	   {
		   case mrtDay :
			   if( cod.Length() > 0 )
				  count = StrToIntDef(cod.SubString(1,1),0);
			   break;
		   case mrtEvening :
			   if( cod.Length() > 1 )
				  count = StrToIntDef(cod.SubString(2,1),0);
			   break;
		   case mrtNight :
			   if( cod.Length() > 2 )
				  count = StrToIntDef(cod.SubString(2,1),0);
			   break;
		   case mrtMorning :
			   if( cod.Length() > 3 )
				  count = StrToIntDef(cod.SubString(3,1),0);
			   break;
	   }
	   return count;
   }
   catch(Exception &ex)
   {
	  Log->Error("Ошибка определения графика горения "+ex.Message);
   }
   return 0;

}


/**
* Проверка текущего режима (день/ночь/вечер)
*
*@param d - Текущая дата
*@return - состояние
*/
TKModeRT __fastcall TKControllers::CurrentModeRT(TDateTime d)
{
   return CurrentModeRT(d,0,0,0,0);
}

/**
* Проверка текущего режима (день/ночь/вечер)
*
*@param d - Текущая дата
*@return - состояние
*/
TKModeRT __fastcall TKControllers::CurrentModeRT(TDateTime d,int d_on,int d_off,int d_on1,int d_off1)
{
//  return mrtUnknown;
  try
  {
// Если наступили новые сутки
	//if( d > t_off_now )
	//{
		GetShedule(Global->Query,d,&t_off_prev,&t_on_now,&t_off_now);
// Здесь переключаем видеостену в дневной режим
	//}
// В диапазон не попал
	if( d < t_off_prev || d > IncMinute(t_off_now,d_off) )
	{
		ModeRT = mrtUnknown;
		return mrtUnknown;
	}
// Если еще не включилось, то день
	if( d < IncMinute(t_on_now,d_on) )
	{
		ModeRT = mrtDay;
		return mrtDay;
	}
// Если до полуночи то вечер
	TDateTime t1 = TimeEveningToNight+d_on1;
	TDateTime t2 = TimeNightToMorning+d_off1;
	TDateTime t  = DateOf(t_off_now) + t1;
	if( d < t )
	{
		ModeRT = mrtEvening;
		return mrtEvening;
	}
// Если до 5:40 то ночь
	t = DateOf(t_off_now) + t2;
	if( d < t )
	{
		ModeRT = mrtNight;
		return mrtNight;
	}
// С 5:40 до выключения то вечер (утро)
	ModeRT = mrtMorning;
	return mrtMorning;
  }
  catch(Exception &ex)
  {
//	   if(Log1!=NULL)Log1->Message("Ошибка CurrentModeRT "+ ex.Message);

  }
}


/**
* Заполнение дерева слоев
*
* @param dt - интересуемая дата
* @param t_on - указатель, куда возвращается время включения
* @param t_off - указатель, куда возвращается время выключения
* @return Возвращает false если ничего не найдено
*
*/
void __fastcall TKControllers::MakeTreeLayers( TAdvStringGrid *asg )
{
    try
    {
	    asg->FixedRows    = 0;
	    asg->FixedCols    = 0;
	    asg->ColCount     = 7;
	    asg->RowCount     = 1;
	    asg->ColWidths[0] = 20;
	    asg->ColWidths[3] = 0;
	    asg->ColWidths[4] = 20;
	    asg->ColWidths[5] = 0;
	    asg->ColWidths[6] = 0;
	    asg->ColWidths[2] = asg->ClientWidth - asg->ColWidths[0]  -
						  asg->ColWidths[3] - asg->ColWidths[4] - 10;

	    TUniQuery *Sql1 = Global->Query;
        
	    if( !Sql1->Connection->Connected )
	    {
		    if( Log!= NULL )Log->Error("Нет соединения с БД. Пытаемся восстановить");
		    Sql1->Connection->PerformConnect(true);
	    }
        
	    if( !Sql1->Connection->Connected )
	    {
		    if( Log!= NULL )Log->Error("Невозможно установить соединение с БД.");
		    return;
	    }

	    Sql1->Close();
	    Sql1->SQL->Clear();
	    Sql1->SQL->Add("SELECT tl.id_object id_object_tl,tl.*,lu.* FROM j_tree_user_layer tl,j_layer_panorama_user lu");
	    Sql1->SQL->Add("WHERE lu.o_userlayer  = tl.id_object(+) AND tl.v_onoff=:a1");
	    Sql1->SQL->Add("ORDER BY v_sortorder");
	    Sql1->Params->Items[0]->AsString = "Да";
	    int id_obj1 = -1;

        Sql1->Open();
        while(!Sql1->Eof)
        {
		    int id_obj =            Sql1->FieldByName("id_object_tl")->AsInteger;
		    UnicodeString l_group = Sql1->FieldByName("v_groupuserlayer")->AsString;
		    UnicodeString l_name  = Sql1->FieldByName("v_userlayer")->AsString;
		    UnicodeString l_def   = Sql1->FieldByName("v_defaultvalue")->AsString;
		    UnicodeString l_comm  = Sql1->FieldByName("v_commentlayer")->AsString;
		    UnicodeString l_code  = Sql1->FieldByName("v_codelayerpanorama")->AsString;
            // Если нода верхнего уровня
            
		    if( l_comm == "" )l_comm = l_name;
            
		    if( l_comm == "" )
            {
                Sql1->Next();
                continue;
            }
                
		    if( id_obj != id_obj1 )
            {
			    asg->Cells[1][asg->RowCount-1] = l_group;
			    asg->Cells[2][asg->RowCount-1] = l_name;
			    asg->Cells[3][asg->RowCount-1] = l_code;
			    asg->Cells[5][asg->RowCount-1] = "#";
                
			    if( l_def == "Да" )
                {
				    asg->AddCheckBox(4,asg->RowCount-1,true,false);
				    for( int i=0; i<5; i++ )
					    asg->CellProperties[i][asg->RowCount-1]->BrushColor = RGB(0xC0,0xC0,0xC0);
			    }
			    else 
                    asg->AddCheckBox(4,asg->RowCount-1,false,false);
			    asg->Cells[4][asg->RowCount-1] = l_def;
			    asg->RowCount++;
		    }
		    else 
            {
			    asg->Cells[3][asg->RowCount-2] = asg->Cells[3][asg->RowCount-2]+","+l_code;
		    }
		    id_obj1 = id_obj;
            Sql1->Next();
        }
        
	    Sql1->Close();
        
	    asg->RowCount--;
	    //asg->Group(1);
        
	    int n = -1;
	    for( int i=0; i<asg->RowCount; i++ )
        {
		    UnicodeString s0 = asg->Cells[4][i];
		    UnicodeString s1 = asg->Cells[1][i];
            
		    if( s0 == "" )
            {
			    n = i;
			    asg->CellProperties[1][i]->ReadOnly = true;
			    asg->CellProperties[2][i]->ReadOnly = true;
			    asg->AddCheckBox(3,i,false,false);
		    }
		    else 
            {
			    asg->Cells[5][i] = IntToStr(n);
			    for( int j=1; j<3; j++) asg->CellProperties[j][i]->ReadOnly = true;
		    }
	    }

    }
    catch(Exception &ex)
    {
	    if(Log!=NULL)Log->Error("Ошибка формирования списка слоев "+ex.Message);
        throw ex;
    }
}

/**
* Сортировка контроллеров по имени ПП
*/
void __fastcall TKControllers::SortByPP(bool dir){
   for(int i=0; i<MAX_CONTROLLER-1; i++){
	  TKController *cnt1 = Controllers[i];
	  if( cnt1 == NULL )continue;
	  for( int j=i+1; j<MAX_CONTROLLER; j++ ){
		 TKController *cnt1 = Controllers[i];
	     if( cnt1 == NULL )continue;
		 TKController *cnt2 = Controllers[j];
		 if( cnt2 == NULL )continue;
// Направления сортировки
		 if( ( dir && cnt1->PP.CompareIC( cnt2->PP) > 0 ) ||
			 ( !dir && cnt1->PP.CompareIC( cnt2->PP) < 0 ) ){
			 TKController *cnt3 = Controllers[i];
			 Controllers[i]     = Controllers[j];
			 Controllers[j]     = cnt3;
		 }
	  }
   }
}

/**
* Сортировка контроллеров по координатам
*/
void __fastcall TKControllers::SortByXY(bool dir){
   for(int i=0; i<MAX_CONTROLLER-1; i++){
	  TKController *cnt1 = Controllers[i];
	  if( cnt1 == NULL )continue;
	  for( int j=i+1; j<MAX_CONTROLLER; j++ ){
		 TKController *cnt1 = Controllers[i];
	     if( cnt1 == NULL )continue;
		 TKController *cnt2 = Controllers[j];
		 if( cnt2 == NULL )continue;
// Направления сортировки
		 if( ( dir && cnt1->PosX > cnt2->PosX) ||
			 ( dir && cnt1->PosX == cnt2->PosX && cnt1->PosY > cnt2->PosY) ||
			 ( !dir && cnt1->PosX < cnt2->PosX) ||
			 ( !dir && cnt1->PosX == cnt2->PosX && cnt1->PosY < cnt2->PosY) ){
			 TKController *cnt3 = Controllers[i];
			 Controllers[i]     = Controllers[j];
			 Controllers[j]     = cnt3;
		 }
	  }
   }

}


/**
* Функции работы с ситемой Идель (Интеграл)
*/
void __fastcall TKControllers::InitIntegral(TUniConnection *conn, UnicodeString suffix){
   try {
// Считываем дополнительные параметры с INI-шника
	  UnicodeString serv = ReadProperty("Integral","IBServer"+suffix,"");
	  UnicodeString user = ReadProperty("Integral","IBUser"+suffix,"sysdba");
	  UnicodeString pass = ReadProperty("Integral","IBPassword"+suffix,"masterkey");
	  UnicodeString base = ReadProperty("Integral","IBBase"+suffix,"");
	  UnicodeString name = ReadProperty("Integral","IBName"+suffix,"");
	  UnicodeString port = ReadProperty("Integral","IBPort"+suffix,"");
	  if( serv == "" || base == "" || name == "" )return;
// Подключаемся к серверу Interbase
	  conn->Server   = serv;
	  if( port != "" )conn->Port = StrToIntDef(port,3050);
	  conn->Username = user;
	  conn->Password = pass;
	  conn->Database = base;
	  conn->Connect();
	  if( !conn->Connected ){
		 if(Log!=NULL)Log->Error("Ошибка подключения к БД "+base);
	  }
	  if(Log!=NULL)Log->Message("Подключена БД "+base);
// Удаляем информацию о заданной системе из списка контроллеров
	  for( int i=0; i<MAX_CONTROLLER; i++ ){
		 TKController *cnt = (TKController *)Controllers[i];
		 if( cnt == NULL )continue;
		 if( cnt->AsoduName == name )cnt->AsoduName = "";
	  }
// Считываем справочную информацию о контроллерах из Interbase
	  TUniQuery *Query  = new TUniQuery(Application);
	  Query->Connection = conn;
	  Query->Close();
// Формируем справочник событий
	  TKIntergralSpravEventList *spr_event = new TKIntergralSpravEventList(Query);
	  spr_event->MakeFromBase();


	  TStringList *list = new TStringList();
	  for( int i=0; i<spr_event->Count; i++ ){
		 TKIntergralSpravEvent *event = spr_event->Items[i];
		 UnicodeString s = IntToStr(event->Id)+";"+IntToStr(event->Nsxema)+";"+event->Name+";";
		 for( int j=0; j< INTEGRAL_MASK_SIZE; j++ ){
			char buf[10];
//			sprintf(buf,("%02x",event->Mask[j]

			s=s+" "+IntToHex( event->Mask[j]&0xFF,2);
		 }
         list->Add(s);
	  }
	  list->SaveToFile("spr_event.txt");
	  delete list;




	  Query->SQL->Clear();
	  Query->SQL->Add("SELECT * FROM spr_pv WHERE spr_pv.enabled=:a1 ORDER BY naimpv");
	  Query->Params->Items[0]->AsInteger = 1;
	  for( Query->Open(); !Query->Eof; Query->Next() ){
		 UnicodeString pv_name = Query->FieldByName("NAIMPV")->AsString;
		 TKController *cnt = FindByPP( pv_name );
		 if( cnt == NULL ){
//			if(Log!=NULL)Log->Error("Не найден в АСД "+pv_name);
			continue;
		 }
		 cnt->Object       = Query->FieldByName("ID_PV")->AsInteger;
		 cnt->AsoduAddress = Query->FieldByName("ADRESS")->AsString;
		 cnt->AsoduName    = name;
		 cnt->IBQuery      = Query;
		 cnt->IntegralValue = new TKIntegralValue(cnt,this,spr_event);
		 cnt->IntegralValue->Id     = Query->FieldByName("ID_PV")->AsInteger;
		 cnt->IntegralValue->Name   = name;
		 cnt->IntegralValue->Nsxema = Query->FieldByName("NOCHEMS")->AsInteger;


		 if(Log!=NULL)Log->Message("ПП в справочнике связан с Идель "+pv_name);
	  }

   }

   catch(Exception &ex)
   {
	  if(Log!=NULL)Log->Error("Ошибка подключение к БД "+ex.Message);
   }


}


/*
void __fastcall TKControllers::MakeTreeLayers_1( TAdvStringGrid *asg )
{
   try
   {
	  asg->FixedRows    = 0;
	  asg->FixedCols    = 0;
	  asg->ColCount     = 8;
	  asg->RowCount     = 1;
	  asg->ColWidths[0] = 20;
//	  asg->ColWidths[1] = 0;
//	  asg->ColWidths[2] = 60;
	  asg->ColWidths[3] = 0;
	  asg->ColWidths[4] = 20;
	  asg->ColWidths[5] = 0;
//	  asg->ColWidths[7] = 0;
//	  asg->ColWidths[7] = 50;
//	  asg->ColWidths[8] = 50;
	  asg->ColWidths[2] = asg->ClientWidth - asg->ColWidths[0]  -
						  asg->ColWidths[3] - asg->ColWidths[4];


	  TUniQuery *Sql1 = NULL;
	  if( Global != NULL )Sql1 = Global->Query;
	  else return;
	  if( !Sql1->Connection->Connected )
	  {
		  if( Log!= NULL )Log->Error("Нет соединения с БД. Пытаемся восстановить");
		  Sql1->Connection->PerformConnect(true);
	  }
	  if( !Sql1->Connection->Connected )
	  {
		  if( Log!= NULL )Log->Error("Невозможно установить соединение с БД.");
		  return;
	  }



	  Sql1->Close();
	  Sql1->SQL->Clear();
	  Sql1->SQL->Add("SELECT tl.id_object id_object_tl,tl.*,lu.* FROM j_tree_user_layer tl,j_layer_panorama_user lu");
	  Sql1->SQL->Add("WHERE lu.o_userlayer  = tl.id_object(+) AND tl.v_onoff=:a1");
	  Sql1->SQL->Add("ORDER BY v_sortorder");
	  Sql1->Params->Items[0]->AsString = "Да";
	  int id_obj1 = -1;
	  int g_sort  = 0;
	  UnicodeString l_group1 = "xxx";
	  for( Sql1->Open(); !Sql1->Eof; Sql1->Next() ){
		  int id_obj =            Sql1->FieldByName("id_object_tl")->AsInteger;
		  UnicodeString l_group = Sql1->FieldByName("v_groupuserlayer")->AsString;
		  UnicodeString l_name  = Sql1->FieldByName("v_userlayer")->AsString;
//		  UnicodeString l_user  = Sql1->FieldByName("v_userlayer")->AsString;
		  UnicodeString l_def   = Sql1->FieldByName("v_defaultvalue")->AsString;
		  UnicodeString l_comm  = Sql1->FieldByName("v_commentlayer")->AsString;
		  UnicodeString l_code  = Sql1->FieldByName("v_codelayerpanorama")->AsString;
		  UnicodeString l_sort  = Sql1->FieldByName("v_sortorder")->AsString;

		  if( l_group != l_group1 )g_sort++;
		  l_group1 = l_group;
// Если нода верхнего уровня
		  if( l_comm == "" )l_comm = l_name;
		  if( l_comm == "" )continue;
		  if( id_obj != id_obj1 ){
			 asg->Cells[1][asg->RowCount-1] = IntToStr(g_sort)+". "+l_group;
			 asg->Cells[2][asg->RowCount-1] = l_name;
			 asg->Cells[3][asg->RowCount-1] = l_code;
			 asg->Cells[5][asg->RowCount-1] = "#";
			 if( l_def == "Да" ){
				asg->AddCheckBox(5,asg->RowCount-1,true,false);
				for( int i=0; i<5; i++ )
					asg->CellProperties[i][asg->RowCount-1]->BrushColor = RGB(0xC0,0xC0,0xC0);
			 }
			 else asg->AddCheckBox(5,asg->RowCount-1,false,false);
			 asg->Cells[4][asg->RowCount-1] = l_def;
			 asg->Cells[1][asg->RowCount-1] = l_sort;

			 asg->RowCount++;
		  }
		  else {
			 asg->Cells[4][asg->RowCount-2] = asg->Cells[3][asg->RowCount-2]+","+l_code;
		  }
		  id_obj1 = id_obj;
	  }
	  Sql1->Close();
	  asg->RowCount--;
//	  asg->Grouping->
	  asg->Group(1);
	  int n = -1;
	  for( int i=0; i<asg->RowCount; i++ ){
		 UnicodeString s0 = asg->Cells[5][i];
		 UnicodeString s1 = asg->Cells[1][i];
		 if( s0 == "" ){
			n = i;
//			asg->CellProperties[1][i]->CellSpanX = 2;
//			asg->Cells[1][i] = s1;
			asg->CellProperties[1][i]->ReadOnly = true;
			asg->CellProperties[3][i]->ReadOnly = true;
			asg->AddCheckBox(4,i,false,false);
		 }
		 else {
			asg->Cells[6][i] = IntToStr(n);
			for( int j=1; j<3; j++) asg->CellProperties[j][i]->ReadOnly = true;
		 }
	  }

   }
   catch(Exception &ex)
   {
	  if(Log!=NULL)Log->Error("Ошибка формирования списка слоев "+ex.Message);
   }
}
*/


/*************************************************************************************
* Работа с числовым рядом
*************************************************************************************/
/**
* Конструктор TKFloatSeriesItem
*/
/*
__fastcall TKFloatSeriesItem::TKFloatSeriesItem()
{
   Z      = 0;
   DT     = 0;
   Status = false;
}
*/

/**
* Конструктор TKFloatSeries
*/
__fastcall TKFloatSeries::TKFloatSeries(): TList()
{
   Min           = 0;
   Max           = 0;
   StatisticFlag = false;
}

/**
* Деструктор TKFloatSeries
*/
__fastcall TKFloatSeries::~TKFloatSeries()
{
   ClearValues();
}

/**
* Очистка значений
*/
void __fastcall TKFloatSeries::ClearValues()
{
   try
   {
      for( int i=0; i<Count; i++ )
      {
         TKFloatSeriesItem *Item = (TKFloatSeriesItem *)Items[i];
         delete Item;
      }
      TList::Clear();
   }
   catch(...)
   {
   }
}

/**
* Добавить одно значение в список
*
*@param Item - элемент который добавляется в список
*/
void __fastcall TKFloatSeries::Add( TKFloatSeriesItem *Item )
{
   TList::Add( (void *)Item );
}

/**
* Добавить одно значение в список
*
*@maram val - значение
*@param dt - дата и время хначения
*/
void __fastcall TKFloatSeries::Add( double val, TDateTime dt )
{
   TKFloatSeriesItem *Item = new TKFloatSeriesItem;
   Item->Value  = val;
   Item->Status = true;
   Item->Z      = 0;
   Item->DT     = dt;
   Add(Item);
}

/**
* Обработать статистику ряда
*/
void __fastcall TKFloatSeries::Statistic()
{
   try
   {
      bool isFirst = true;
      Min          = 0;
      Max          = 0;
      Avg          = 0;
      CountCorrect = 0;
      for( int i=0; i<Count; i++ )
      {
         TKFloatSeriesItem *Item = (TKFloatSeriesItem *)Items[i];
         if( !Item->Status )continue;
         if( isFirst )
         {
            Min     = Item->Value;
            Max     = Item->Value;
            isFirst = false;
         }
         else
         {
         if( Item->Value > Max )Max = Item->Value;
         if( Item->Value < Min )Min = Item->Value;
         }
         Avg += Item->Value;
         CountCorrect++;
      }
      if( CountCorrect!=0)Avg /= CountCorrect;
//Теперь считаем среднеквадратичное отклонение и дисперсию
      Dispersion = 0;
      Deviation  = 0;

      for( int i=0; i<Count; i++)
      {
         TKFloatSeriesItem *Item = (TKFloatSeriesItem *)Items[i];
         if( !Item->Status )continue;
         Dispersion += pow( (Item->Value - Avg ),2);
      }
      if( CountCorrect!=0)Dispersion /=  CountCorrect ;
      Deviation = sqrt( Dispersion );
      StatisticFlag = true;
   }
   catch(...)
   {
   }
}

/**
* Отфильтровать ряд по датам
*
*@param dt_min - минимальная дата время
*@param dt_max - максимальная дата время
*@param flag - если true - то добавлять щначения в список
*/
void __fastcall TKFloatSeries::Filter(TDateTime dt_min, TDateTime dt_max, bool flag)
{
   try
   {
      CountCorrect = 0;
      for( int i=0; i<Count; i++ )
      {
         TKFloatSeriesItem *Item = (TKFloatSeriesItem *)Items[i];
         if( Item->Status == false && flag == false )continue;
         if( Item->DT > dt_min && Item->DT < dt_max )Item->Status = true;
         else
         {
            if( flag == false )Item->Status = false;
         }
         if( Item->Status )CountCorrect++;
      }
   }
   catch(...)
   {
   }
}

/**
* Отфильтровать ряд по датам
*
*@param dt_min - минимальная дата время
*@param dt_max - максимальная дата время
*/
void __fastcall TKFloatSeries::Filter(TDateTime dt_min, TDateTime dt_max)
{
   Filter(dt_min,dt_max,false);
}


/**
* Отфильтровать ряд по доверительному интервалу абсолютных значений
*
*@param min - минимальное значение
*@param max - максимальное значение
*/
void __fastcall TKFloatSeries::Filter(double min, double max)
{
   try
   {
      CountCorrect = 0;
      for( int i=0; i<Count; i++ )
      {
         TKFloatSeriesItem *Item = (TKFloatSeriesItem *)Items[i];
         if( !Item->Status )continue;
         if( Item->Value > min && Item->Value < max )CountCorrect++;
         else Item->Status = false;
      }
   }
   catch(...)
   {
   }
}

/**
* Отфильтровать ряд на минимум максимум Z-оценки по доверительному интервалу Гаусс-распределения
*
*@param zmin - минимальное значение
*@param zmax - максимальное значение
*/
void __fastcall TKFloatSeries::FilterZ(double zmin, double zmax)
{
   try
   {
// Если не расчитывалась статистика, расчитать ее
      if( !StatisticFlag )Statistic();
      CountCorrect = 0;
      for( int i=0; i<Count; i++ )
      {
         TKFloatSeriesItem *Item = (TKFloatSeriesItem *)Items[i];
         if( !Item->Status )continue;
// Расчитываем Z-оценку для каждого значения и проверяем его на интервал
         Item->Z = 0;
         if( Deviation )Item->Z = (Item->Value - Avg)/Deviation;
// Проверяем значение Z-оценок на доверительный интервал         
         if( zmin!=0 && Item->Z <zmin )Item->Status = false;
         if( zmax!=0 && Item->Z >zmax )Item->Status = false;
         if( Item->Status )CountCorrect++;
      }
   }
   catch(...)
   {
   }
}



/**
* Инвертировать отфильтрованный ряд
*/
void __fastcall TKFloatSeries::InverceFilter()
{
   try
   {
      CountCorrect = 0;
      for( int i=0; i<Count; i++ )
      {
         TKFloatSeriesItem *Item = (TKFloatSeriesItem *)Items[i];
         if( Item->Status )Item->Status = false;
         else
         {
            Item->Status = true;
            CountCorrect++;
         }
      }
   }
   catch(...)
   {
   }
}


/**
* Очистить все фильтры ряда
*/
void __fastcall TKFloatSeries::ClearFilter()
{
   try
   {
      CountCorrect = 0;
      for( int i=0; i<Count; i++ )
	  {
         TKFloatSeriesItem *Item = (TKFloatSeriesItem *)Items[i];
         Item->Status = true;
		 CountCorrect++;
      }
   }
   catch(...)
   {
   }
}


/**
* Сформировать ряд из списка строк, возвращаемого по UDP протоколу
*/
void __fastcall TKFloatSeries::MakeFromList( TStringList *sl )
{
   try
   {
	  ClearValues();
	  CountCorrect = 0;
	  TDateTime dt;
	  double x;
	  for( int i=1; i<sl->Count; i++)
	  {
		 if( i%2 == 1 )
		 {
			dt  = StrToDateTimeDef(sl->Strings[i],(TDateTime)0);
		 }//end if
		 else
		 {
			x = StrToFloatDef(sl->Strings[i],0);
			Add(x,dt);
		 }
	  }
   }
   catch(...)
   {
   }

}


/**
* Класс TKArhiveValueAnalogParam
*/
__fastcall TKArhiveValueAnalogParam::TKArhiveValueAnalogParam() :TObject()
{
	Value  = -1;
	Status = 0;

}

void __fastcall TKArhiveValueAnalogParam::Set( double val, unsigned short stat )
{
	Value  = val;
	Status = stat;
}


void __fastcall TKArhiveValueAnalogParam::CopyTo(TKArhiveValueAnalogParam *val)
{
	val->Value  = Value;
	val->Status = Status;
}


/******************************************************************************
/**
* Класс TKArchiveValuesParam;
*/

/**
* Конструтктор
* @param cnts - разименованный объект TKControllers
*/
__fastcall TKArchiveValuesParam::TKArchiveValuesParam(TObject *cnts) :TObject()
{
	Ia = new TKArhiveValueAnalogParam();
	Ib = new TKArhiveValueAnalogParam();
	Ic = new TKArhiveValueAnalogParam();
	Ua = new TKArhiveValueAnalogParam();
	Ub = new TKArhiveValueAnalogParam();
	Uc = new TKArhiveValueAnalogParam();
	DumpFlag = false;
	stat     = csDisable;
	Clear();
	Controllers = cnts;
	Controller  = NULL;
	stat_prev   = TKControllerStatus::csNone;
	DT_prev     = (TDateTime)0;
}

/**
* Деструтктор
*/
__fastcall TKArchiveValuesParam::~TKArchiveValuesParam()
{
	delete Ia;
	delete Ib;
	delete Ic;
	delete Ua;
	delete Ub;
	delete Uc;
}


void __fastcall TKArchiveValuesParam::Clear()
{

   Ia->Set(-1,0);
   Ib->Set(-1,0);
   Ic->Set(-1,0);
   Ua->Set(-1,0);
   Ub->Set(-1,0);
   Uc->Set(-1,0);
   CrashAll   = 0;
   DiscretAll = PARAM_Switch |PARAM_Contrl;

//   DiscretAll = 0;
 //  Control   = -1;
 //  Manual    = -1;
   Link      = false;
   DelayOn   = 0;
   DelayOff  = 0;
   DelayNOn  = 0;
   DelayNOff = 0;
   Phasa     = 0;
   PhasaN    = 0;
   stat      = TKControllerStatus::csNone;
   mode      = mrtUnknown;
   Problem   = (TKProblemPP)0;
   DT        = 0;
   PP        = "";
   Arm       = 0;
   Epran     = 0;
   Dump      = "";
   BKSAll    = 0;
   BKSFlagRT = false;
}

/**
* Прочитать Ззачение аварийного параметра по маске
*/
bool __fastcall TKArchiveValuesParam::GetParamCrash( int mask )
{
	return( CrashAll & mask );
}

/**
* Вернуть строчку с ошибками фазы
*/
UnicodeString __fastcall TKArchiveValuesParam::PhaseCrash( UnicodeString phase ){
   UnicodeString s="";
   TStringList *sl = new TStringList();
   if( phase.UpperCase() == "A" ){
	  if( GetParamCrash(PARAM_SemA) )sl->Add("СемА");
	  if( GetParamCrash(PARAM_PerA) )sl->Add("ПерА");
	  if( GetParamCrash(PARAM_KZA) )sl->Add("КЗА");
   }
   if( phase.UpperCase() == "B" ){
	  if( GetParamCrash(PARAM_SemB) )sl->Add("СемB");
	  if( GetParamCrash(PARAM_PerB) )sl->Add("ПерB");
	  if( GetParamCrash(PARAM_KZB) )sl->Add("КЗB");
   }
   if( phase.UpperCase() == "C" ){
	  if( GetParamCrash(PARAM_SemC) )sl->Add("СемC");
	  if( GetParamCrash(PARAM_PerC) )sl->Add("ПерC");
	  if( GetParamCrash(PARAM_KZC) )sl->Add("КЗC");
   }
   if( sl->Count > 0 ){
	  sl->Delimiter = ',';
	  s = sl->DelimitedText;
   }
   delete sl;
   return s;
}



/**
* Прочитать значение дискретного параметра по маске
*/
bool __fastcall TKArchiveValuesParam::GetParamDiscret( int mask )
{
	return( DiscretAll & mask );
}

/**
* Прочитать значение параметра БКС по маске
*/
bool __fastcall TKArchiveValuesParam::GetParamBKS( int mask )
{
	return( BKSAll & mask );
}


/**
* Добавить значение аварийного парамета
*/
void __fastcall TKArchiveValuesParam::SetParamCrash( int val )
{
	CrashAll = CrashAll | val;
}

bool __fastcall TKArchiveValuesParam::IsEnablePhase( UnicodeString phase,int nphase )
{
	bool a=false,b=false,c=false;
	switch( nphase ){
	   case 1:
	   case 9:   a = true; break;
	   case 2:
	   case 10 : b = true; break;
	   case 3:
	   case 11 : c = true; break;
	   case 4:
	   case 12 : a = true; b=true; break;
	   case 5:
	   case 13 : b = true; c=true; break;
	   case 6:
	   case 14 : a = true; c=true; break;
	   case 7:
	   case 15 : a = true; b = true; c=true; break;
	}
	if( phase.UpperCase() == "A" )return a;
	if( phase.UpperCase() == "B" )return b;
	if( phase.UpperCase() == "C" )return c;
    return false;
}



bool __fastcall TKArchiveValuesParam::IsEnablePhase( UnicodeString phase )
{
	return IsEnablePhase( phase,Phasa );
}


UnicodeString __fastcall TKArchiveValuesParam::EnablePhase()
{

	UnicodeString s = "";
	if( IsEnablePhase("A") )s=s+"A";
	if( IsEnablePhase("B") )s=s+"B";
	if( IsEnablePhase("C") )s=s+"C";
	return s;
}

UnicodeString __fastcall TKArchiveValuesParam::EnablePhase(int nphase)
{

	UnicodeString s = "";
	if( IsEnablePhase("A",nphase) )s=s+"A";
	if( IsEnablePhase("B",nphase) )s=s+"B";
	if( IsEnablePhase("C",nphase) )s=s+"C";
	return s;
}



/**
* Добавить значение дискретного параметра
*/
void __fastcall TKArchiveValuesParam::SetParamDiscret( int val )
{
	DiscretAll = DiscretAll | val;
}

/**
* Добавить значение параметра БКС
*/
void __fastcall TKArchiveValuesParam::SetParamBKS( int val )
{
	BKSAll = BKSAll | val;
}


/**
* Устанавливаем значение структуры архива
*/
void __fastcall TKArchiveValuesParam::SetValues(  TKParam *param )
{
// Пишем параметр в дамп, есди флаг включен
   if( DumpFlag ){
	  if( Dump == "" )Dump = param->ShortName+"="+FloatToStr(param->Value);
	  else Dump = Dump+","+param->ShortName+"="+FloatToStr(param->Value);
   }


// Установка токов
   if( param->ShortName      == "Ia" )Ia->Set(param->Value,param->AsoduStatus);
   else if( param->ShortName == "Ib" )Ib->Set(param->Value,param->AsoduStatus);
   else if( param->ShortName == "Ic" )Ic->Set(param->Value,param->AsoduStatus);
// Установка напряжений
   else if( param->ShortName == "Ua" )Ua->Set(param->Value,param->AsoduStatus);
   else if( param->ShortName == "Ub" )Ub->Set(param->Value,param->AsoduStatus);
   else if( param->ShortName == "Uc" )Uc->Set(param->Value,param->AsoduStatus);
// Установка параметров влияющих на аварии
   else if( param->ShortName == "СимА" && param->Value == 1 && !param->AlarmExceptionFlag )SetParamCrash(PARAM_SemA);
   else if( param->ShortName == "СимВ" && param->Value == 1 && !param->AlarmExceptionFlag )SetParamCrash(PARAM_SemB);
   else if( param->ShortName == "СимС" && param->Value == 1 && !param->AlarmExceptionFlag )SetParamCrash(PARAM_SemC);
   else if( param->ShortName == "ПерА" && param->Value == 1 && !param->AlarmExceptionFlag )SetParamCrash(PARAM_PerA);
   else if( param->ShortName == "ПерВ" && param->Value == 1 && !param->AlarmExceptionFlag )SetParamCrash(PARAM_PerB);
   else if( param->ShortName == "ПерС" && param->Value == 1 && !param->AlarmExceptionFlag )SetParamCrash(PARAM_PerC);
   else if( param->ShortName == "КЗА"  && param->Value == 1 && !param->AlarmExceptionFlag )SetParamCrash(PARAM_KZA);
   else if( param->ShortName == "КЗВ"  && param->Value == 1 && !param->AlarmExceptionFlag )SetParamCrash(PARAM_KZB);
   else if( param->ShortName == "КЗС"  && param->Value == 1 && !param->AlarmExceptionFlag )SetParamCrash(PARAM_KZC);
//Установка дтскретных параметров
   else if( param->ShortName == "220_a" && param->Value == 1 && !param->AlarmExceptionFlag ){
	  SetParamDiscret(PARAM_StatA);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(DiscretAll));
   }
   else if( param->ShortName == "220_b" && param->Value == 1 && !param->AlarmExceptionFlag ){
	  SetParamDiscret(PARAM_StatB);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(DiscretAll));
   }
   else if( param->ShortName == "220_c" && param->Value == 1 && !param->AlarmExceptionFlag ){
	  SetParamDiscret(PARAM_StatC);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(DiscretAll));
   }
   else if( param->ShortName == "Тумбл" && param->Value == 1 && !param->AlarmExceptionFlag ){
	  SetParamDiscret(PARAM_Switch);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(DiscretAll));
   }
   else if( param->ShortName == "Упр"   && param->Value == 1 && !param->AlarmExceptionFlag ){
	  SetParamDiscret(PARAM_Contrl);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(DiscretAll));
   }
   else if( param->ShortName == "Охрана"&& param->Value == 1 && !param->AlarmExceptionFlag ){
	  SetParamDiscret(PARAM_Guard);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(DiscretAll));
   }
   else if( param->ShortName == "Дверь" && param->Value == 1 && !param->AlarmExceptionFlag ){
	  SetParamDiscret(PARAM_Door);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(DiscretAll));
   }
// Установка прочих параметров
//   else if( param->ShortName == "Тумбл" )Manual     = param->Value;
//   else if( param->ShortName == "Упр"   )Control    = param->Value;
   else if( param->ShortName == "СмВкл" )DelayOn    = param->Value;
   else if( param->ShortName == "СмОтк" )DelayOff   = param->Value;
   else if( param->ShortName == "СмВклН" )DelayNOn  = param->Value;
   else if( param->ShortName == "СмОткН" )DelayNOff = param->Value;
   else if( param->ShortName == "Вечер_Ф" )Phasa    = param->Value;
   else if( param->ShortName == "Ночь_Ф" )PhasaN    = param->Value;
   else if( param->ShortName == "ЭПРАН" )Epran      = param->Value;
   else if( param->ShortName == "IN_01" ){
	  SetParamBKS(PARAM_BKS_FLAG);
	  if( CheckBKS( param->Value ) )SetParamBKS(PARAM_BKS01);
   }
   else if( param->ShortName == "IN_02" ){
	  SetParamBKS(PARAM_BKS_FLAG);
	  if( CheckBKS( param->Value ) )SetParamBKS(PARAM_BKS02);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(BKSAll));
   }
   else if( param->ShortName == "IN_03" ){
	  SetParamBKS(PARAM_BKS_FLAG);
	  if( CheckBKS( param->Value ) )SetParamBKS(PARAM_BKS03);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(BKSAll));
   }
   else if( param->ShortName == "IN_04" ){
	  SetParamBKS(PARAM_BKS_FLAG);
	  if( CheckBKS( param->Value ) )SetParamBKS(PARAM_BKS04);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(BKSAll));
   }
   else if( param->ShortName == "IN_05" ){
	  SetParamBKS(PARAM_BKS_FLAG);
	  if( CheckBKS( param->Value ) )SetParamBKS(PARAM_BKS05);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(BKSAll));
   }
   else if( param->ShortName == "IN_06" ){
	  SetParamBKS(PARAM_BKS_FLAG);
	  if( CheckBKS( param->Value ) )SetParamBKS(PARAM_BKS06);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(BKSAll));
   }
   else if( param->ShortName == "IN_07" ){
	  SetParamBKS(PARAM_BKS_FLAG);
	  if( CheckBKS( param->Value ) )SetParamBKS(PARAM_BKS07);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(BKSAll));
   }
   else if( param->ShortName == "IN_08" ){
	  SetParamBKS(PARAM_BKS_FLAG);
	  if( CheckBKS( param->Value ) )SetParamBKS(PARAM_BKS08);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(BKSAll));
   }
   else if( param->ShortName == "IN_09" ){
	  SetParamBKS(PARAM_BKS_FLAG);
	  if( CheckBKS( param->Value ) )SetParamBKS(PARAM_BKS09);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(BKSAll));
   }
   else if( param->ShortName == "IN_10" ){
	  SetParamBKS(PARAM_BKS_FLAG);
	  if( CheckBKS( param->Value ) )SetParamBKS(PARAM_BKS10);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(BKSAll));
   }
   else if( param->ShortName == "IN_11" ){
	  SetParamBKS(PARAM_BKS_FLAG);
	  if( CheckBKS( param->Value ) )SetParamBKS(PARAM_BKS11);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(BKSAll));
   }
   else if( param->ShortName == "IN_12" ){
	  SetParamBKS(PARAM_BKS_FLAG);
	  if( CheckBKS( param->Value ) )SetParamBKS(PARAM_BKS12);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(BKSAll));
   }
   else if( param->ShortName == "IN_13" ){
	  SetParamBKS(PARAM_BKS_FLAG);
	  if( CheckBKS( param->Value ) )SetParamBKS(PARAM_BKS13);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(BKSAll));
   }
   else if( param->ShortName == "IN_14" ){
	  SetParamBKS(PARAM_BKS_FLAG);
	  if( CheckBKS( param->Value ) )SetParamBKS(PARAM_BKS14);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(BKSAll));
   }
   else if( param->ShortName == "IN_15" ){
	  SetParamBKS(PARAM_BKS_FLAG);
	  if( CheckBKS( param->Value ) )SetParamBKS(PARAM_BKS15);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(BKSAll));
   }
   else if( param->ShortName == "IN_16" ){
	  SetParamBKS(PARAM_BKS_FLAG);
	  if( CheckBKS( param->Value ) )SetParamBKS(PARAM_BKS16);
//	  Log(PP+" "+param->ShortName+" "+FloatToStr(param->Value)+" "+IntToStr(BKSAll));
   }

}

/**
* Проверка значения параметра БКС
*/
bool __fastcall TKArchiveValuesParam::CheckBKS(double val)
{
   int val_int = (int)val;
   switch( val_int ){
	  case 0:  //Выключено
	  case 2:  //Выключено авария
		 return false;
	  case 1:  //Включено
	  case 3:  //Включено авария
		 return true;
   }
   return false;
}


int __fastcall TKArchiveValuesParam::CheckAlarms() {
   int alarm_count = 0;
   TKController *cnt = (TKController *)Controller;
   if( cnt == NULL )return 0;
   for( int i=0; i<cnt->Alarms->Count; i++ ){
	  TKAlarm *alarm = (TKAlarm *)cnt->Alarms->Items[i];
	  if( alarm == NULL )continue;
	  if( CheckAlarmScript(alarm) )alarm_count++;
   }
   return alarm_count;
}


/**
* Проверка параметра на тревогу
*
*@param param - параметра
*@return - нпличие тревоги в параметре
*/
bool __fastcall TKArchiveValuesParam::CheckAlarmScript(TKAlarm *alarm){
   TKController  *cnt  = (TKController *)Controller;
   if( cnt == NULL )return false;
   if( cnt->PP == "" )return false;
// Создаем объект скрипта
   TfsScript *fsScript = new TfsScript(Application);
   fsScript->SyntaxType = "C++Script";
   fsScript->AddRTTI();
   fsScript->AddVariable("Link","boolean",Link);
// Импортируем переменные с токами и напряжениями из TKArchiveValuesParam
   fsScript->AddVariable("Ua","double",Ua->Value);
   fsScript->AddVariable("Ub","double",Ub->Value);
   fsScript->AddVariable("Uc","double",Uc->Value);
   fsScript->AddVariable("Ia","double",Ia->Value);
   fsScript->AddVariable("Ib","double",Ib->Value);
   fsScript->AddVariable("Ic","double",Ic->Value);
// Импортируем лимиты по токам и напряжениям
   fsScript->AddVariable("Ua_min","double",cnt->UA_min);
   fsScript->AddVariable("Ua_max","double",cnt->UA_max);
   fsScript->AddVariable("Ub_min","double",cnt->UB_min);
   fsScript->AddVariable("Ub_max","double",cnt->UB_max);
   fsScript->AddVariable("Uc_min","double",cnt->UC_min);
   fsScript->AddVariable("Uc_max","double",cnt->UC_max);
   fsScript->AddVariable("Ia_min","double",cnt->IA_min);
   fsScript->AddVariable("Ia_max","double",cnt->IA_max);
   fsScript->AddVariable("Ib_min","double",cnt->IB_min);
   fsScript->AddVariable("Ib_max","double",cnt->IB_max);
   fsScript->AddVariable("Ic_min","double",cnt->IC_min);
   fsScript->AddVariable("Ic_max","double",cnt->IC_max);
// Импортируем параметры с апаратными ошибками БК
   fsScript->AddVariable("SemA","boolean",GetParamCrash(PARAM_SemA));
   fsScript->AddVariable("SemB","boolean",GetParamCrash(PARAM_SemB));
   fsScript->AddVariable("SemC","boolean",GetParamCrash(PARAM_SemC));
   fsScript->AddVariable("PerA","boolean",GetParamCrash(PARAM_PerA));
   fsScript->AddVariable("PerB","boolean",GetParamCrash(PARAM_PerB));
   fsScript->AddVariable("PerC","boolean",GetParamCrash(PARAM_PerC));
   fsScript->AddVariable("KZA","boolean",GetParamCrash(PARAM_KZA));
   fsScript->AddVariable("KZB","boolean",GetParamCrash(PARAM_KZB));
   fsScript->AddVariable("KZC","boolean",GetParamCrash(PARAM_KZC));
// Импортируем разные дискретные параметры
   fsScript->AddVariable("StatA","boolean",GetParamDiscret(PARAM_StatA));
   fsScript->AddVariable("StatB","boolean",GetParamDiscret(PARAM_StatB));
   fsScript->AddVariable("StatC","boolean",GetParamDiscret(PARAM_StatC));
   fsScript->AddVariable("Switch","boolean",GetParamDiscret(PARAM_Switch));
   fsScript->AddVariable("Contrl","boolean",GetParamDiscret(PARAM_Contrl));
   fsScript->AddVariable("Door","boolean",GetParamDiscret(PARAM_Door));
   fsScript->AddVariable("Guard","boolean",GetParamDiscret(PARAM_Guard));
// Импортируем параметры БКС-12
   fsScript->AddVariable("BKS01","boolean",GetParamBKS(PARAM_BKS01));
   fsScript->AddVariable("BKS02","boolean",GetParamBKS(PARAM_BKS02));
   fsScript->AddVariable("BKS03","boolean",GetParamBKS(PARAM_BKS03));
   fsScript->AddVariable("BKS04","boolean",GetParamBKS(PARAM_BKS04));
   fsScript->AddVariable("BKS05","boolean",GetParamBKS(PARAM_BKS05));
   fsScript->AddVariable("BKS06","boolean",GetParamBKS(PARAM_BKS06));
   fsScript->AddVariable("BKS07","boolean",GetParamBKS(PARAM_BKS07));
   fsScript->AddVariable("BKS08","boolean",GetParamBKS(PARAM_BKS08));
   fsScript->AddVariable("BKS09","boolean",GetParamBKS(PARAM_BKS09));
   fsScript->AddVariable("BKS10","boolean",GetParamBKS(PARAM_BKS10));
   fsScript->AddVariable("BKS11","boolean",GetParamBKS(PARAM_BKS11));
   fsScript->AddVariable("BKS12","boolean",GetParamBKS(PARAM_BKS12));
   fsScript->AddVariable("BKS13","boolean",GetParamBKS(PARAM_BKS13));
   fsScript->AddVariable("BKS14","boolean",GetParamBKS(PARAM_BKS14));
   fsScript->AddVariable("BKS15","boolean",GetParamBKS(PARAM_BKS15));
   fsScript->AddVariable("BKS16","boolean",GetParamBKS(PARAM_BKS16));
   fsScript->AddVariable("BKS","boolean",BKSFlagRT);
// Служебные переменные

   fsScript->AddVariable("Mode","int",((TKControllers *)Controllers)->ModeRT);
   fsScript->AddVariable("PhaseA","boolean",IsEnablePhase("A"));
   fsScript->AddVariable("PhaseB","boolean",IsEnablePhase("B"));
   fsScript->AddVariable("PhaseC","boolean",IsEnablePhase("C"));

   fsScript->AddVariable("Alarm","boolean",false); //Возвращаемое значение
   fsScript->AddVariable("AlarmType","String",alarm->AlarmSprav->Type); //Возвращаемое значение
   fsScript->AddVariable("Event","boolean",false); //Возвращаемое значение
//	   fsScript->AddVariable("Alarm","boolean",alarm->isAlarm); //Возвращаемое значение
//	   fsScript->AddVariable("Event","boolean",alarm->isEvent); //Возвращаемое значение
   fsScript->AddVariable("Count","int",alarm->countEvent);
//	   fsScript->AddVariable("tmAlarm","TDateTime",alarm->tmAlarm);
   if( alarm->countEvent )
	  fsScript->AddVariable("TimeOut","int",MinutesBetween(cnt->RealTimeDT,alarm->tmEvent));
   else
	  fsScript->AddVariable("TimeOut","int",0);

   fsScript->AddVariable("Name","String",alarm->Name); //Возвращаемое значение

// Импортируем переменные с аппаратными ошибками

// fsScript1.Parent := fsGlobalUnit;
   fsScript->Lines->Text = "{\r\n"+alarm->AlarmSprav->Script+"}\r\n";
   double ua_xx = (double)fsScript->Variables["Uc"];

   if ( !fsScript->Run() )
	  ShowMessage("Ошибка скрипта тревоги "+fsScript->ErrorMsg);

// Обработка после выполнения скрипта
   bool          event1 = (bool)fsScript->Variables["Event"];
   bool          alarm1 = (bool)fsScript->Variables["Alarm"];
   Variant       name1  = fsScript->Variables["Name"];
   Variant       type1  = fsScript->Variables["AlarmType"];
   alarm->Name          = name1;
   alarm->Type          = type1;
// Регистрируем первое событие
   if( !alarm->isEvent && event1 ){
	  alarm->countEvent = 0;
	  alarm->tmEvent    = cnt->RealTimeDT;
   }
   if( !alarm->isAlarm && alarm1 ){
	  alarm->tmAlarm    = cnt->RealTimeDT;
   }
   if( event1 ){
	  alarm->countEvent++;
   }

   alarm->isEvent       = event1;
   alarm->isAlarm       = alarm1;
   delete fsScript;
   if( alarm->Type == "Тревога" || alarm->Type == "Авария" )return alarm1;
   else return false;

}


void __fastcall TKArchiveValuesParam::Save( TUniQuery *Query, UnicodeString tab, TDateTime d1 ){
   if( tab == "" )return;
//   TDateTime d1    = d1.CurrentDateTime();
   TKController  *cnt  = (TKController *)Controller;
   TKLog *log = ((TKControllers *)Controllers)->Log;
   try {
	  int num = 0;

	  Query->Close();
	  Query->SQL->Clear();
	  Query->SQL->Add("INSERT INTO "+tab+"(");
	  Query->SQL->Add("arh_Id, arh_PP,arh_NetAddr, arh_DT");
	  Query->SQL->Add(", arh_Ua, arh_Ub, arh_Uc");
	  Query->SQL->Add(", arh_Ia, arh_Ib, arh_Ic");
	  Query->SQL->Add(", arh_Link, arh_FailABC, arh_DiscretAll");
	  Query->SQL->Add(", arh_delay_on, arh_delay_off, arh_stat, arh_mode");
	  Query->SQL->Add(", arh_delayn_on, arh_delayn_off, arh_phasa, arh_phasan");
	  Query->SQL->Add(", arh_num_contr, arh_name_contr, arh_arm, arh_sysDT");
	  Query->SQL->Add(", arh_problem, arh_epran, arh_prev_dt, arh_prev_stat,arh_dump,arh_bks)");
	  Query->SQL->Add(" VALUES(Seq_PollServiceArchive.nextval, :a02, :a03, :a04");
	  Query->SQL->Add(", :a05, :a06, :a07");
	  Query->SQL->Add(", :a08, :a09, :a10");
	  Query->SQL->Add(", :a11, :a12, :a13");
	  Query->SQL->Add(", :a14, :a15, :a16, :a17");
	  Query->SQL->Add(", :a18, :a19, :a20, :a21");
	  Query->SQL->Add(", :a22, :a23, :a24, sysdate");
	  Query->SQL->Add(", :a26, :a27, :a28, :a29, :a30, :a31 )");
	  Query->Params->Items[num++]->AsString   = cnt->PP;
	  Query->Params->Items[num++]->AsInteger  = cnt->NetAddr;
	  Query->Params->Items[num++]->AsDateTime = DT;
	  Query->Params->Items[num++]->AsFloat    = Ua->Value;
	  Query->Params->Items[num++]->AsFloat    = Ub->Value;
	  Query->Params->Items[num++]->AsFloat    = Uc->Value;
	  Query->Params->Items[num++]->AsFloat    = Ia->Value;
	  Query->Params->Items[num++]->AsFloat    = Ib->Value;
	  Query->Params->Items[num++]->AsFloat    = Ic->Value;
	  Query->Params->Items[num++]->AsFloat    = Link;
	  Query->Params->Items[num++]->AsInteger  = CrashAll;
	  Query->Params->Items[num++]->AsInteger  = DiscretAll;
	  Query->Params->Items[num++]->AsInteger  = DelayOn;
	  Query->Params->Items[num++]->AsInteger  = DelayOff;
//	   Query->Params->Items[num++]->AsInteger = Control;
	  Query->Params->Items[num++]->AsInteger  = (int)stat;
	  Query->Params->Items[num++]->AsInteger  = (int)mode;
	  Query->Params->Items[num++]->AsInteger  = DelayNOn;
	  Query->Params->Items[num++]->AsInteger  = DelayNOff;
	  Query->Params->Items[num++]->AsInteger  = Phasa;
	  Query->Params->Items[num++]->AsInteger  = PhasaN;
	  Query->Params->Items[num++]->AsInteger  = cnt->Object;
	  Query->Params->Items[num++]->AsString   = cnt->AsoduName;
	  Query->Params->Items[num++]->AsInteger  = cnt->Arm;
	  Query->Params->Items[num++]->AsInteger  = Problem;
	  Query->Params->Items[num++]->AsInteger  = Epran;
	  Query->Params->Items[num++]->AsDateTime = DT_prev;
	  Query->Params->Items[num++]->AsInteger  = (int)stat_prev;
	  Query->Params->Items[num++]->AsString   = Dump;
	  Query->Params->Items[num++]->AsInteger  = BKSAll;

	  Query->ExecSQL();
   }
   catch (Exception &ex){
	  if( log!=NULL )log->Error("Ошибка сохранения в Oracle: "+ ex.Message);
	}



}





void __fastcall TKArchiveValuesParam::CopyTo(TKArchiveValuesParam *ArhValues)
{
	Ia->CopyTo(ArhValues->Ia);
	Ib->CopyTo(ArhValues->Ib);
	Ic->CopyTo(ArhValues->Ic);
	Ua->CopyTo(ArhValues->Ua);
	Ub->CopyTo(ArhValues->Ub);
	Uc->CopyTo(ArhValues->Uc);
	ArhValues->Link       = Link;
	ArhValues->CrashAll   = CrashAll;
	ArhValues->DiscretAll = DiscretAll;
//	ArhValues->Manual     = val->Manual;
	ArhValues->DelayOn    = DelayOn;
	ArhValues->DelayOff   = DelayOff;
	ArhValues->DelayNOn   = DelayNOn;
	ArhValues->DelayNOff  = DelayNOff;
	ArhValues->Phasa      = Phasa;
	ArhValues->PhasaN     = PhasaN;
	ArhValues->stat       = stat;
	ArhValues->mode       = mode;
	ArhValues->Problem    = Problem;
	ArhValues->Epran      = Epran;
	ArhValues->Dump       = Dump;
	ArhValues->DumpFlag   = DumpFlag;
	ArhValues->Controller = Controller;
	ArhValues->Controllers= Controllers;
	ArhValues->BKSAll     = BKSAll;
	ArhValues->BKSFlagRT  = BKSFlagRT;
	ArhValues->UdpString  = UdpString;
	ArhValues->stat_prev  = stat_prev;
	ArhValues->DT_prev    = DT_prev;
}

/**
* Кодируем значение парамера в строку для посылки
*/
UnicodeString __fastcall TKArchiveValuesParam::CodeToString(UnicodeString pp,int arm)
{

//	   Udp->SendAll("STAT_PP;"+cnt->PP+";"+IntToStr( (int)stat1 )+";"+IntToStr(Controllers->Arm)+";"+IntToStr((int)ArhValues->Problem));
//Формируем пакет, совместимый со старой версией АСД   0,1
   UnicodeString str = "STAT_PP;\""+pp;
//Добавляем статус    2
   str=str+"\";"+IntToStr((int)stat);
//Добавляем внешний параметр "Номер АРМа"  3
   str=str+";"+IntToStr(arm);
//Добавляем проблему   4
   str=str+";"+IntToStr((int)Problem);
// Даобавляем новые параметры в пакет
// Добавляем дату    5
   str=str+";\""+DT.FormatString("dd.mm.yy hh:nn:ss");
//Добавляем режим   6
   str=str+"\";"+IntToStr((int)mode);
//Добавляем параметр связи  7
   str=str+";"+IntToStr((int)Link);
//Добавляем токи со статусами   8,9,10,11,12,13
   str=str+";"+FloatToStr(Ia->Value)+";"+IntToStr(Ia->Status);
   str=str+";"+FloatToStr(Ib->Value)+";"+IntToStr(Ib->Status);
   str=str+";"+FloatToStr(Ic->Value)+";"+IntToStr(Ic->Status);
//Добавляем напряжения со статусами 14,15,16,17,18,19
   str=str+";"+FloatToStr(Ua->Value)+";"+IntToStr(Ua->Status);
   str=str+";"+FloatToStr(Ub->Value)+";"+IntToStr(Ub->Status);
   str=str+";"+FloatToStr(Uc->Value)+";"+IntToStr(Uc->Status);
//Добавляем парамеры с авариями и дискертными сигналами 20,21
   str=str+";"+IntToStr(CrashAll);
   str=str+";"+IntToStr(DiscretAll);
//Добавляем задержки включения/отключения 22,23
   str=str+";"+IntToStr(DelayOn);
   str=str+";"+IntToStr(DelayOff);
   str=str+";"+IntToStr(Epran);
// Добавляем параметр со всеми БКС12
   str=str+";"+IntToStr(BKSAll);
// Добавляем параметры с вечерней и ночной фазой
   str=str+";"+IntToStr(Phasa);
   str=str+";"+IntToStr(PhasaN);
   UdpString = str;
   return UdpString;
}

/**
* Декодируем значение из строки
*/

void __fastcall TKArchiveValuesParam::DecodeFromString()
{
   DecodeFromString(true);
}

/**
* Декодируем значение из строки
*/

void __fastcall TKArchiveValuesParam::DecodeFromString(bool db_flag)
{
   DecodeFromString(UdpString,db_flag);
}

/**
* Декодируем значение из строки
*/

void __fastcall TKArchiveValuesParam::DecodeFromString(UnicodeString s)
{
   DecodeFromString(s,true);
}



/**
* Декодируем значение из строки
*/

void __fastcall TKArchiveValuesParam::DecodeFromString(UnicodeString s,bool db_flag)
{
	Clear();
	TStringList *list   = new TStringList();
	list->Delimiter     = ';';
	list->DelimitedText = s;
	if( list->Count <= 0 )return;
	int n=0;
	if( list->Strings[n++] != L"STAT_PP" )return;
	if( n < list->Count )PP         = list->Strings[n++];
	if( n < list->Count )stat       = (TKControllerStatus)StrToIntDef(list->Strings[n++],0);
	if( n < list->Count )Arm        = StrToIntDef(list->Strings[n++],0);
	if( n < list->Count )Problem    = (TKProblemPP)StrToIntDef(list->Strings[n++],0);
	if( n < list->Count )DT         = StrToDateTimeDef(list->Strings[n++],0);
	if( n < list->Count )mode       = (TKModeRT)StrToIntDef(list->Strings[n++],0);
	if( n < list->Count )Link       = (bool)StrToIntDef(list->Strings[n++],0);
	if( Link ){ // Если со связью все в порядке, то продолжаем декадировать
//Декодируем токи со статусами
	   if( n < list->Count )Ia->Value  = StrToFloatDef(list->Strings[n++],-1);
	   if( n < list->Count )Ia->Status = StrToIntDef(list->Strings[n++],0);
	   if( n < list->Count )Ib->Value  = StrToFloatDef(list->Strings[n++],-1);
	   if( n < list->Count )Ib->Status = StrToIntDef(list->Strings[n++],0);
	   if( n < list->Count )Ic->Value  = StrToFloatDef(list->Strings[n++],-1);
	   if( n < list->Count )Ic->Status = StrToIntDef(list->Strings[n++],0);
//Декодируем напряжения
	   if( n < list->Count )Ua->Value  = StrToFloatDef(list->Strings[n++],-1);
	   if( n < list->Count )Ua->Status = StrToIntDef(list->Strings[n++],0);
	   if( n < list->Count )Ub->Value  = StrToFloatDef(list->Strings[n++],-1);
	   if( n < list->Count )Ub->Status = StrToIntDef(list->Strings[n++],0);
	   if( n < list->Count )Uc->Value  = StrToFloatDef(list->Strings[n++],-1);
	   if( n < list->Count )Uc->Status = StrToIntDef(list->Strings[n++],0);
//Декодируем дискреные значения
	   if( n < list->Count )CrashAll   = StrToIntDef(list->Strings[n++],0);
	   if( n < list->Count )DiscretAll = StrToIntDef(list->Strings[n++],0);
//Декодируем смещение
	   if( n < list->Count )DelayOn    = StrToIntDef(list->Strings[n++],0);
	   if( n < list->Count )DelayOff   = StrToIntDef(list->Strings[n++],0);
	   if( n < list->Count )Epran      = StrToIntDef(list->Strings[n++],0);
//Декодируем параметр БКС
	   if( n < list->Count ){
		  BKSAll     = StrToIntDef(list->Strings[n++],0);
		  if( GetParamBKS(0x10000) )BKSFlagRT = true;
	   }
	   if( n < list->Count )Phasa      = StrToIntDef(list->Strings[n++],-1);
	   if( n < list->Count )PhasaN     = StrToIntDef(list->Strings[n++],-1);
	}
	else { //Если нет, берем последнее значение из архива, когда была связь
	   if(db_flag)GetLastValue();
	}
	delete list;


}

/**
* Читаем последнее корректное значение из базы
*/
void __fastcall TKArchiveValuesParam::GetLastValue()
{
   TUniQuery *Query      = ((TKControllers *)Controllers)->Global->Query;
   UnicodeString TabName = ((TKControllers *)Controllers)->TableArchive;
   Query->Close();
   Query->SQL->Clear();
   Query->SQL->Add("SELECT * FROM "+TabName+" WHERE arh_id = ( SELECT MAX(arh_id) FROM pollservicearchive WHERE arh_pp=:a1 AND arh_stat!=:a2)");
   Query->Params->Items[0]->AsString  = PP;
   Query->Params->Items[1]->AsInteger = 100;
   Query->Open();
   if( !Query->Eof )
   {
	   Problem    = Query->FieldByName("arh_problem")->AsInteger;
	   DT         = Query->FieldByName("arh_dt")->AsDateTime;
//	   Link       = Query->FieldByName("arh_link")->AsInteger;
	   mode       = Query->FieldByName("arh_mode")->AsInteger;
	   Ia->Value  = Query->FieldByName("arh_ia")->AsFloat;
	   Ia->Status = FLG_EXISTS;
	   Ib->Value  = Query->FieldByName("arh_ib")->AsFloat;
	   Ib->Status = FLG_EXISTS;
	   Ic->Value  = Query->FieldByName("arh_ic")->AsFloat;
	   Ic->Status = FLG_EXISTS;
	   Ua->Value  = Query->FieldByName("arh_ua")->AsFloat;
	   Ua->Status = FLG_EXISTS;
	   Ub->Value  = Query->FieldByName("arh_ub")->AsFloat;
	   Ub->Status = FLG_EXISTS;
	   Uc->Value  = Query->FieldByName("arh_uc")->AsFloat;
	   Uc->Status = FLG_EXISTS;
	   CrashAll   = Query->FieldByName("arh_failabc")->AsInteger;
	   DiscretAll = Query->FieldByName("arh_discretall")->AsInteger;
	   DelayOn    = Query->FieldByName("arh_delay_on")->AsInteger;
	   DelayOff   = Query->FieldByName("arh_delay_off")->AsInteger;
	   Epran      = Query->FieldByName("arh_epran")->AsInteger;
	   BKSAll     = Query->FieldByName("arh_bks")->AsInteger;
	   Phasa      = Query->FieldByName("arh_phasa")->AsInteger;
	   PhasaN     = Query->FieldByName("arh_phasan")->AsInteger;

   }
   Query->Close();
}

/**
* Читаем последнее корректное значение из базы
*/
void __fastcall TKArchiveValuesParam::InitValue()
{
   TUniQuery *Query      = ((TKControllers *)Controllers)->Global->Query;
   UnicodeString TabName = ((TKControllers *)Controllers)->TableArchive;
   Query->Close();
   Query->SQL->Clear();
   Query->SQL->Add("SELECT * FROM "+TabName+" WHERE arh_id = ( SELECT MAX(arh_id) FROM pollservicearchive WHERE arh_pp=:a1 AND arh_stat!=:a2)");
   Query->Params->Items[0]->AsString  = PP;
   Query->Params->Items[1]->AsInteger = 100;
   Query->Open();
   if( !Query->Eof )
   {
	   Problem    = Query->FieldByName("arh_problem")->AsInteger;
	   DT         = Query->FieldByName("arh_dt")->AsDateTime;
	   TDateTime dt = dt.CurrentDate();
	   IncMinute(dt,-30);
	   if (DT < dt)
	   {
			Link       = 0;
			stat       = 0;
	   }
	   else
	   {
			Link       = Query->FieldByName("arh_link")->AsInteger;
			stat       = Query->FieldByName("arh_stat")->AsInteger;
	   }
	   mode       = Query->FieldByName("arh_mode")->AsInteger;
	   Ia->Value  = Query->FieldByName("arh_ia")->AsFloat;
	   Ia->Status = FLG_EXISTS;
	   Ib->Value  = Query->FieldByName("arh_ib")->AsFloat;
	   Ib->Status = FLG_EXISTS;
	   Ic->Value  = Query->FieldByName("arh_ic")->AsFloat;
	   Ic->Status = FLG_EXISTS;
	   Ua->Value  = Query->FieldByName("arh_ua")->AsFloat;
	   Ua->Status = FLG_EXISTS;
	   Ub->Value  = Query->FieldByName("arh_ub")->AsFloat;
	   Ub->Status = FLG_EXISTS;
	   Uc->Value  = Query->FieldByName("arh_uc")->AsFloat;
	   Uc->Status = FLG_EXISTS;
	   CrashAll   = Query->FieldByName("arh_failabc")->AsInteger;
	   DiscretAll = Query->FieldByName("arh_discretall")->AsInteger;
	   DelayOn    = Query->FieldByName("arh_delay_on")->AsInteger;
	   DelayOff   = Query->FieldByName("arh_delay_off")->AsInteger;
	   Epran      = Query->FieldByName("arh_epran")->AsInteger;
	   BKSAll     = Query->FieldByName("arh_bks")->AsInteger;
   }
   Query->Close();
}


void __fastcall TKArchiveValuesParam::GetLastValue(TDateTime dt)
{
   TUniQuery *Query      = ((TKControllers *)Controllers)->Global->Query;
   UnicodeString TabName = ((TKControllers *)Controllers)->TableArchive;
   Query->Close();
   Query->SQL->Clear();
   Query->SQL->Add("SELECT * FROM "+TabName+" WHERE arh_id = ( SELECT MAX(arh_id) FROM pollservicearchive WHERE arh_pp=:a1 AND arh_stat!=:a2 AND arh_dt <=:a3)");
   Query->Params->Items[0]->AsString   = PP;
   Query->Params->Items[1]->AsInteger  = 100;
   Query->Params->Items[2]->AsDateTime = IncMinute(dt,1);
   //Query->Params->Items[2]->AsDateTime = dt;
   Query->Open();
   if( !Query->Eof ){

	   Problem    = Query->FieldByName("arh_problem")->AsInteger;
	   DT         = Query->FieldByName("arh_dt")->AsDateTime;
	   Link       = Query->FieldByName("arh_link")->AsInteger;
	   mode       = Query->FieldByName("arh_mode")->AsInteger;
	   Ia->Value  = Query->FieldByName("arh_ia")->AsFloat;
	   Ia->Status = FLG_EXISTS;
	   Ib->Value  = Query->FieldByName("arh_ib")->AsFloat;
	   Ib->Status = FLG_EXISTS;
	   Ic->Value  = Query->FieldByName("arh_ic")->AsFloat;
	   Ic->Status = FLG_EXISTS;
	   Ua->Value  = Query->FieldByName("arh_ua")->AsFloat;
	   Ua->Status = FLG_EXISTS;
	   Ub->Value  = Query->FieldByName("arh_ub")->AsFloat;
	   Ub->Status = FLG_EXISTS;
	   Uc->Value  = Query->FieldByName("arh_uc")->AsFloat;
	   Uc->Status = FLG_EXISTS;
	   CrashAll   = Query->FieldByName("arh_failabc")->AsInteger;
	   DiscretAll = Query->FieldByName("arh_discretall")->AsInteger;
	   DelayOn    = Query->FieldByName("arh_delay_on")->AsInteger;
	   DelayOff   = Query->FieldByName("arh_delay_off")->AsInteger;
	   Epran      = Query->FieldByName("arh_epran")->AsInteger;
	   BKSAll     = Query->FieldByName("arh_bks")->AsInteger;
   }
   Query->Close();
}

/**
* Конструктор TKArchiveValuesParamItem
*/
__fastcall TKArchiveValuesParamItem::TKArchiveValuesParamItem(TCollection *Collection)
   : TCollectionItem(Collection), FValue(0) { }

/**
* Конструктор TKArchiveValuesParamList
*/
__fastcall TKArchiveValuesParamList::TKArchiveValuesParamList()
	: TCollection(__classid(TKArchiveValuesParamItem)) { }

/**
* Деструктор TKArchiveValuesParamItem
*/
//__fastcall TKArchiveValuesParamItem::~TKArchiveValuesParamItem()  {
//   Clear();
//}

/**
* Добавить элемент в список
*/
void __fastcall TKArchiveValuesParamList::Add(TKArchiveValuesParam *AValue)
{
	TKArchiveValuesParamItem *Item = (TKArchiveValuesParamItem *) TCollection::Add();
	Item->Value = AValue;
}

/**
* Удалить элемент из списка
*/
void __fastcall TKArchiveValuesParamList::Delete(int Index)
{
	if( Index < 0 && Index >= TCollection::Count )return;
	delete Items[Index];
	TCollection::Delete(Index);
}

/**
* Очистить весь список с удаленим элеиентов
*/
void __fastcall TKArchiveValuesParamList::Clear(){
	while( Count>0 )Delete(0);
}



/**
* Получить элемент из списка
*/
TKArchiveValuesParam * __fastcall TKArchiveValuesParamList::GetItem(int Index)
{
	TKArchiveValuesParamItem *Item = (TKArchiveValuesParamItem *) TCollection::GetItem(Index);
	return Item->Value;
}

/*
* Сохранить элемент в список
*/
void __fastcall TKArchiveValuesParamList::SetItem(int Index, TKArchiveValuesParam * AValue)
{
	TKArchiveValuesParamItem *Item = (TKArchiveValuesParamItem *) TCollection::GetItem(Index);
	Item->Value = AValue;
}





/**
* Конструктор класса TKGrafIU
*
*@param Chart1 - для графиков
*@param Sql - запрс к БД
*/
__fastcall TKGrafIU::TKGrafIU(TAdvStringGrid *AdvStringGrid1, TChart *Chart1,TKGlobalSem *Global)
:TObject()
{

Global1 = Global;
Controllers = new TKControllers(Global1);

AdvStringGrid = AdvStringGrid1;
Chart = Chart1;
Chart->ClearChart();
Chart->OnClickSeries = &ChartClickSeries;
Shift = Global1->IniFile->ReadDateTime("Graf","Shift",0);

normbd = false;

Series1  = new TLineSeries(Chart);
Series2  = new TLineSeries(Chart);
Series3  = new TLineSeries(Chart);
Series6  = new TLineSeries(Chart);
Series7  = new TLineSeries(Chart);
//Series7  = new TGanttSeries(Chart);
Series17 = new TLineSeries(Chart);
Series18 = new TLineSeries(Chart);
Series19 = new TLineSeries(Chart);
Series20 = new TLineSeries(Chart);

Series36 = new TLineSeries(Chart);
Series37 = new TLineSeries(Chart);
Series38 = new TLineSeries(Chart);
Series39 = new TLineSeries(Chart);
Series40 = new TLineSeries(Chart);
Series41 = new TLineSeries(Chart);

Series21 = new TLineSeries(Chart);
Series22 = new TLineSeries(Chart);
Series23 = new TLineSeries(Chart);
Series24 = new TLineSeries(Chart);
Series25 = new TLineSeries(Chart);
Series26 = new TLineSeries(Chart);
Series27 = new TLineSeries(Chart);
Series28 = new TLineSeries(Chart);
Series29 = new TLineSeries(Chart);
Series30 = new TLineSeries(Chart);
Series31 = new TLineSeries(Chart);
/*
Series36 = new TAreaSeries(Chart);
Series38 = new TAreaSeries(Chart);
Series40 = new TAreaSeries(Chart);

Series37 = new TAreaSeries(Chart);
Series39 = new TAreaSeries(Chart);
Series41 = new TAreaSeries(Chart);  */

Series4  = new TAreaSeries(Chart);
Series5  = new TAreaSeries(Chart);
Series8  = new TAreaSeries(Chart);
Series9  = new TBubbleSeries(Chart);
Series10 = new TBubbleSeries(Chart);
Series11 = new TBubbleSeries(Chart);
Series12 = new TBubbleSeries(Chart);
Series13 = new TBubbleSeries(Chart);
Series14 = new TBubbleSeries(Chart);
Series15 = new TBubbleSeries(Chart);
Series16 = new TBubbleSeries(Chart);
Series32 = new TBarSeries(Chart);
Series33  = new TGanttSeries(Chart);
Series34  = new TGanttSeries(Chart);
Series35  = new TGanttSeries(Chart);

Chart->AddSeries(Series4);
Chart->AddSeries(Series32);
Chart->AddSeries(Series33);
Chart->AddSeries(Series34);
Chart->AddSeries(Series35);
Chart->AddSeries(Series1);
Chart->AddSeries(Series2);
Chart->AddSeries(Series3);
Chart->AddSeries(Series17);
Chart->AddSeries(Series18);
Chart->AddSeries(Series19);
Chart->AddSeries(Series21);
Chart->AddSeries(Series20);

Chart->AddSeries(Series37);
Chart->AddSeries(Series39);
Chart->AddSeries(Series41);
Chart->AddSeries(Series36);
Chart->AddSeries(Series38);
Chart->AddSeries(Series40);

Chart->AddSeries(Series22);
Chart->AddSeries(Series23);
Chart->AddSeries(Series24);
Chart->AddSeries(Series25);
Chart->AddSeries(Series26);
Chart->AddSeries(Series27);
Chart->AddSeries(Series28);
Chart->AddSeries(Series29);
Chart->AddSeries(Series30);
Chart->AddSeries(Series31);
Chart->AddSeries(Series7);
Chart->AddSeries(Series6);
Chart->AddSeries(Series5);
Chart->AddSeries(Series9);
Chart->AddSeries(Series10);
Chart->AddSeries(Series11);
Chart->AddSeries(Series12);
Chart->AddSeries(Series13);
Chart->AddSeries(Series15);
Chart->AddSeries(Series14);
Chart->AddSeries(Series16);
Chart->AddSeries(Series8);

Series17->VertAxis = aRightAxis;
Series18->VertAxis = aRightAxis;
Series19->VertAxis = aRightAxis;
Series23->VertAxis = aRightAxis;
Series24->VertAxis = aRightAxis;
Series25->VertAxis = aRightAxis;
Series29->VertAxis = aRightAxis;
Series30->VertAxis = aRightAxis;
Series31->VertAxis = aRightAxis;

Series33->VertAxis = aLeftAxis;
Series34->VertAxis = aLeftAxis;
Series35->VertAxis = aLeftAxis;

Series32->Marks->Visible = false;

Series4->XValues->DateTime = true;

Chart->LeftAxis->Title->Caption = "Ток, А";
Chart->LeftAxis->Title->Angle = 90;
Chart->LeftAxis->Title->Visible = true;
Chart->LeftAxis->Automatic = false;
Chart->LeftAxis->Increment = 3;
Chart->LeftAxis->Minimum = 0;
Chart->LeftAxis->Maximum = 150;

Chart->BottomAxis->ExactDateTime = true;
Chart->Axes->Behind = false;

Chart->AxisVisible = true;

Chart->RightAxis->Title->Caption = "Напряжение, В";
Chart->RightAxis->Title->Angle = 90;
Chart->RightAxis->Title->Visible = true;
Chart->RightAxis->Automatic = false;
Chart->RightAxis->Maximum = 250;
Chart->RightAxis->Minimum = 180;
Chart->RightAxis->Increment = 10;

Chart->View3D = false;
Chart->Legend->Visible = false;
Chart->LeftAxis->LabelStyle = talValue;

Series1->Stairs = true;
Series2->Stairs = true;
Series3->Stairs = true;
Series6->Stairs = true;
Series7->Stairs = true;
Series17->Smoothed = true;
Series18->Smoothed = true;
Series19->Smoothed = true;

Series36->Stairs = true;
Series37->Stairs = true;
Series38->Stairs = true;
Series39->Stairs = true;
Series40->Stairs = true;
Series41->Stairs = true;

Series36->Marks->Visible = false;
Series37->Marks->Visible = false;
Series38->Marks->Visible = false;
Series39->Marks->Visible = false;
Series40->Marks->Visible = false;
Series41->Marks->Visible = false;

AdvStringGrid->ColCount     = 22;
AdvStringGrid->RowCount     = 1;
  //AdvStringGrid->FixedRows = 0;
AdvStringGrid->ColWidths[0] = 120;
AdvStringGrid->ColWidths[1] = 35;
AdvStringGrid->ColWidths[2] = 35;
AdvStringGrid->ColWidths[3] = 35;
AdvStringGrid->ColWidths[4] = 35;
AdvStringGrid->ColWidths[5] = 35;
AdvStringGrid->ColWidths[6] = 35;
AdvStringGrid->ColWidths[7] = 50;
AdvStringGrid->ColWidths[8] = 55;

AdvStringGrid->ColWidths[10] = 0;
AdvStringGrid->ColWidths[11] = 0;
AdvStringGrid->ColWidths[12] = 0;
AdvStringGrid->ColWidths[13] = 0;
AdvStringGrid->ColWidths[14] = 0;
AdvStringGrid->ColWidths[15] = 0;
AdvStringGrid->ColWidths[16] = 0;
AdvStringGrid->ColWidths[17] = 0;
AdvStringGrid->ColWidths[18] = 0;
AdvStringGrid->ColWidths[19] = 0;
AdvStringGrid->ColWidths[20] = 0;
AdvStringGrid->ColWidths[21] = 0;

UniQuery1 = new TUniQuery(Global1->Query);
UniQuery2 = new TUniQuery(Global1->Query);
UniQuery3 = new TUniQuery(Global1->Query);
UniQuery1->Connection = Global1->Query->Connection;
UniQuery2->Connection = Global1->Query->Connection;
UniQuery3->Connection = Global1->Query->Connection;
DiscretGrafVisible = Graf_IA  | Graf_IB | Graf_IC | Graf_UA | Graf_UB | Graf_UC | Graf_AV | Graf_SV | Graf_GR;
kza = false;
kzb = false;
kzc = false;
pera = false;
perb = false;
perc = false;
sima = false;
simb = false;
simc = false;
door = false;
guard = false;
swith = false;
contr = false;
UniQuery1->Close();
UniQuery1->SQL->Clear();
UniQuery1->SQL->Add("SELECT v_alarm_sprav_param_name as name ");
UniQuery1->SQL->Add("FROM alarm_sprav");
UniQuery1->SQL->Add("WHERE alarm_sprav.V_ALARM_SPRAV_ONOFF='Да'");
for(UniQuery1->Open(); !UniQuery1->Eof; UniQuery1->Next())
{
	   if (UniQuery1->FieldByName("name")->AsString == "Тумбл")  swith = true;
	   if (UniQuery1->FieldByName("name")->AsString == "Охрана") guard = true;
	   if (UniQuery1->FieldByName("name")->AsString == "СимА")   sima = true;
	   if (UniQuery1->FieldByName("name")->AsString == "СимВ")   simb = true;
	   if (UniQuery1->FieldByName("name")->AsString == "СимС")   simc = true;
	   if (UniQuery1->FieldByName("name")->AsString == "ПерА")   pera = true;
	   if (UniQuery1->FieldByName("name")->AsString == "ПерВ")   perb = true;
	   if (UniQuery1->FieldByName("name")->AsString == "ПерС")   perc = true;
	   if (UniQuery1->FieldByName("name")->AsString == "КЗА")    kza = true;
	   if (UniQuery1->FieldByName("name")->AsString == "КЗВ")    kzb = true;
	   if (UniQuery1->FieldByName("name")->AsString == "КЗС")    kzc = true;
}

Alarm = new TStringList();
Alarm->Clear();
Alarm->Delimiter     = ';';
}

//---------------------------------------------------------------------------

void __fastcall TKGrafIU::ChartClickSeries(TCustomChart *Sender, TChartSeries *Series,
		  int ValueIndex, TMouseButton Button, TShiftState Shift, int X,
		  int Y)
{
 try
 {
	AdvStringGrid->ScrollInView(0,ValueIndex);
	TGridRect r;
	r.Left = AdvStringGrid->FixedCols;
	r.Top = ValueIndex+1;
	r.Right = AdvStringGrid->ColCount;
	r.Bottom = ValueIndex+1;
	AdvStringGrid->Selection = r;
	Chart->CancelMouse = false;
 }
 catch (...)
 {

 }
}

//---------------------------------------------------------------------------

void __fastcall TKGrafIU::GrafPaint(UnicodeString pp, UnicodeString Date1, UnicodeString Date2,bool sutki)
{
try
{

	punkt = pp;
	TryStrToDateTime(Date1,DateOt);
	TryStrToDateTime(Date2,DateDo);
	DateDo = DateDo + 1;
	GrafPaintClear();

	if (! sutki )
	{
	GrafPaintToki();
	TryStrToDateTime(Date1,DateOt);
	TryStrToDateTime(Date2,DateDo);
	DateDo = DateDo + 1;
	GrafPaintSutki();
	TryStrToDateTime(Date1,DateOt);
	TryStrToDateTime(Date2,DateDo);
	DateDo = DateDo + 1;
	GrafPaintTochki();
	}
	else
	{
	GrafPaintNedel();
    TryStrToDateTime(Date1,DateOt);
	TryStrToDateTime(Date2,DateDo);
	DateDo = DateDo + 1;
	GrafPaintSutki();
	TryStrToDateTime(Date1,DateOt);
	TryStrToDateTime(Date2,DateDo);
	DateDo = DateDo + 1;
	GrafPaintTochki();
	}
   if ( AdvStringGrid->RowCount > 1 ) AdvStringGrid->FixedRows = 1;
   Global1->Query->Close();
   Chart->Axes->Left->Automatic = false;
   Screen->Cursor = crDefault;
   }
catch(...)
{
	Screen->Cursor = crDefault;
}

}

//---------------------------------------------------------------------------

void __fastcall TKGrafIU::GrafPaint(UnicodeString pp, UnicodeString Date1, UnicodeString Date2)
{
try
{
	punkt = pp;
	TryStrToDateTime(Date1,DateOt);
	TryStrToDateTime(Date2,DateDo);
	DateDo = DateDo + 1;
	GrafPaintClear();

	GrafPaintToki();
	TryStrToDateTime(Date1,DateOt);
	TryStrToDateTime(Date2,DateDo);
	DateDo = DateDo + 1;
	GrafPaintSutki();
	TryStrToDateTime(Date1,DateOt);
	TryStrToDateTime(Date2,DateDo);
	DateDo = DateDo + 1;
	GrafPaintTochki();
   if ( AdvStringGrid->RowCount > 1 ) AdvStringGrid->FixedRows = 1;
   Global1->Query->Close();
   Chart->Axes->Left->Automatic = false;
   Screen->Cursor = crDefault;
   }
catch(...)
{
	Screen->Cursor = crDefault;
}

}
//---------------------------------------------------------------------------
void __fastcall TKGrafIU::GrafPaintClear()
{
    Series1->Clear();            // Ia
	Series2->Clear();            // Ib
	Series3->Clear();            // Ic
	Series4->Clear();            // утро/вечер горение
	Series5->Clear();            // ночь
	Series6->Clear();            // связь
	Series7->Clear();            // аварии
	Series8->Clear();            // фон (пустой)
	Series9->Clear();            // откл ia
	Series10->Clear();           // откл ib
	Series11->Clear();           // откл ic
	Series12->Clear();           // вкл  ia
	Series13->Clear();           // вкл  ib
	Series14->Clear();           // вкл  ic
	Series15->Clear();           // откл эл.переметр
	Series16->Clear();           // вкл  эл.переметр
	Series17->Clear();           // Ua
	Series18->Clear();           // Ub
	Series19->Clear();           // Uc
	Series20->Clear();           // границы тока а
	Series21->Clear();           // границы тока b
	Series22->Clear();           // границы тока c
	Series23->Clear();           // границы напряжения а
	Series24->Clear();           // границы напряжения b
	Series25->Clear();           // границы напряжения c
	Series26->Clear();           // границы тока а
	Series27->Clear();           // границы тока b
	Series28->Clear();           // границы тока c
	Series29->Clear();           // границы напряжения а
	Series30->Clear();           // границы напряжения b
	Series31->Clear();           // границы напряжения c
	Series32->Clear();			 // горение/негорение
	Series36->Clear();           // границы тока а
	Series37->Clear();           // границы тока b
	Series38->Clear();           // границы тока c
	Series39->Clear();           // границы тока а
	Series40->Clear();           // границы тока b
	Series41->Clear();           // границы тока c
	AdvStringGrid->ClearAll();

	AdvStringGrid->ColCount     = 22;
	AdvStringGrid->RowCount     = 1;
	AdvStringGrid->Cells[0][0]  = "Время";
	AdvStringGrid->Cells[1][0]  = "Ia,A";
	AdvStringGrid->Cells[2][0]  = "Ib,A";
	AdvStringGrid->Cells[3][0]  = "Ic,A";
	AdvStringGrid->Cells[4][0]  = "Ua,B";
	AdvStringGrid->Cells[5][0]  = "Ub,B";
	AdvStringGrid->Cells[6][0]  = "Uc,B";
	AdvStringGrid->Cells[7][0]  = "Связь";
	AdvStringGrid->Cells[8][0]  = "Тревоги";

	AdvStringGrid->Cells[9][0]  = "Часть суток";
	AdvStringGrid->Cells[10][0]  = "Ia_min_н";
	AdvStringGrid->Cells[11][0]  = "Ia_max_н";
	AdvStringGrid->Cells[12][0]  = "Ib_min_н";
	AdvStringGrid->Cells[13][0]  = "Ib_max_н";
	AdvStringGrid->Cells[14][0]  = "Ic_min_н";
	AdvStringGrid->Cells[15][0]  = "Ic_max_н";

	AdvStringGrid->Cells[16][0]  = "Ia_min_н";
	AdvStringGrid->Cells[17][0]  = "Ia_max_н";
	AdvStringGrid->Cells[18][0]  = "Ib_min_н";
	AdvStringGrid->Cells[19][0]  = "Ib_max_н";
	AdvStringGrid->Cells[20][0]  = "Ic_min_н";
	AdvStringGrid->Cells[21][0]  = "Ic_max_н";

	clDay       = (TColor)RGBStringToColor(Controllers->ReadProperty("Graf","ChartDay","255,255,255"));
	clEvening	= (TColor)RGBStringToColor(Controllers->ReadProperty("Graf","ChartEvening","0xff,0xff,0x99"));
	clNight     = (TColor)RGBStringToColor(Controllers->ReadProperty("Graf","ChartNight","0xcc,0xcc,0x99"));
	clIa     	= (TColor)RGBStringToColor(Controllers->ReadProperty("Graf","ChartIA","0,0,215"));
	clIb     	= (TColor)RGBStringToColor(Controllers->ReadProperty("Graf","ChartIB","0,215,0"));
	clIc      	= (TColor)RGBStringToColor(Controllers->ReadProperty("Graf","ChartIC","0,215,215"));
	clSv     	= (TColor)RGBStringToColor(Controllers->ReadProperty("Graf","ChartSvjaz","0,0,0"));
	clAl     	= (TColor)RGBStringToColor(Controllers->ReadProperty("Graf","ChartAlarm","215,0,0"));
	clUa        = (TColor)RGBStringToColor(Controllers->ReadProperty("Graf","ChartUA","30,100,30"));
	clUb       	= (TColor)RGBStringToColor(Controllers->ReadProperty("Graf","ChartUB","30,170,130"));
	clUc	    = (TColor)RGBStringToColor(Controllers->ReadProperty("Graf","ChartUC","100,166,30"));
	clGor	    = (TColor)RGBStringToColor(Controllers->ReadProperty("Graf","ChartGoren","150,20,20"));
	// ia
	Series1->Color          = clIa;
	Series1->LinePen->Style = psSolid;
	Series1->LinePen->Width = 3;
	// границы ia
	Series20->Color          = clIa;
	Series20->LinePen->Style = psDash;
	Series20->LinePen->Width = 2;
	Series26->Color          = clIa;
	Series26->LinePen->Style = psDash;
	Series26->LinePen->Width = 2;
	Series36->Color          = clIa;
	Series36->LinePen->Style = psDash;
	Series36->LinePen->Width = 2;
	Series37->Color          = clIa;
	Series37->LinePen->Style = psDash;
	Series37->LinePen->Width = 2;
	// ib
	Series2->Color          = clIb;
	Series2->LinePen->Style = psSolid;
	Series2->LinePen->Width = 3;

	Series21->Color          = clIb;
	Series21->LinePen->Style = psDash;
	Series21->LinePen->Width = 2;
	Series27->Color          = clIb;
	Series27->LinePen->Style = psDash;
	Series27->LinePen->Width = 2;
	Series38->Color          = clIb;
	Series38->LinePen->Style = psDash;
	Series38->LinePen->Width = 2;
	Series39->Color          = clIb;
	Series39->LinePen->Style = psDash;
	Series39->LinePen->Width = 2;
	// ic
	Series3->Color          = clIc;
	Series3->LinePen->Style = psSolid;
	Series3->LinePen->Width = 3;

	Series22->Color          = clIc;
	Series22->LinePen->Style = psDash;
	Series22->LinePen->Width = 2;
	Series28->Color          = clIc;
	Series28->LinePen->Style = psDash;
	Series28->LinePen->Width = 2;
	Series40->Color          = clIc;
	Series40->LinePen->Style = psDash;
	Series40->LinePen->Width = 2;
	Series41->Color          = clIc;
	Series41->LinePen->Style = psDash;
	Series41->LinePen->Width = 2;
	// ua
	Series17->Color          = clUa;
	Series17->LinePen->Style = psSolid;
	Series17->LinePen->Width = 2;

	Series23->Color          = clUa;
	Series23->LinePen->Style = psSolid;
	Series23->LinePen->Width = 1;
	Series29->Color          = clUa;
	Series29->LinePen->Style = psSolid;
	Series29->LinePen->Width = 1;
	// ub
	Series18->Color          = clUb;
	Series18->LinePen->Style = psSolid;
	Series18->LinePen->Width = 2;

	Series24->Color          = clUb;
	Series24->LinePen->Style = psSolid;
	Series24->LinePen->Width = 1;
	Series30->Color          = clUb;
	Series30->LinePen->Style = psSolid;
	Series30->LinePen->Width = 1;
	// uc
	Series19->Color          = clUc;
	Series19->LinePen->Style = psSolid;
	Series19->LinePen->Width = 2;

	Series25->Color          = clUc;
	Series25->LinePen->Style = psSolid;
	Series25->LinePen->Width = 1;
	Series31->Color          = clUc;
	Series31->LinePen->Style = psSolid;
	Series31->LinePen->Width = 1;

	Series32->Color      = clGor;
	Series32->Pen->Width = 3;

		Series4->Color          = clEvening;
	Series4->InvertedStairs = true;
	Series4->Stairs         = true;
	Series4->AreaLinesPen->Visible = false;

	Series5->Color          = clNight;
	Series5->InvertedStairs = true;
	Series5->Stairs         = true;
	Series5->AreaLinesPen->Visible = false;

	Series8->ColorEachLine  = true;
	Series8->ColorEachPoint = true;
	Series8->InvertedStairs = false;

	//связь
	Series6->Color          = clSv;
	Series6->LinePen->Style = psSolid;
	Series6->LinePen->Width = 1;

	// авария
	Series7->Color          = clAl;
	Series7->LinePen->Style = psDash;
	Series7->LinePen->Width = 3;

	Series33->Color          = clIa;
	Series33->Marks->Visible = true;
	Series33->Pointer->VertSize = 12;
	Series33->Marks->Transparent = true;

	Series34->Color          = clIb;
	Series34->Marks->Visible = true;
	Series34->Pointer->VertSize = 12;
	Series34->Marks->Transparent = true;

	Series35->Color          = clIc;
	Series35->Marks->Visible = true;
	Series35->Pointer->VertSize = 12;
	Series35->Marks->Transparent = true;

	Series9->Color          = clIa;
	Series10->Color         = clIb;
	Series11->Color         = clIc;

	AdvStringGrid->CellProperties[0][0]->FontColor = clBlack;
	AdvStringGrid->CellProperties[1][0]->FontColor = clIa;
	AdvStringGrid->CellProperties[2][0]->FontColor = clIb;
	AdvStringGrid->CellProperties[3][0]->FontColor = clIc;
	AdvStringGrid->CellProperties[4][0]->FontColor = clUa;
	AdvStringGrid->CellProperties[5][0]->FontColor = clUb;
	AdvStringGrid->CellProperties[6][0]->FontColor = clUc;
	AdvStringGrid->CellProperties[7][0]->FontColor = clSv;
	AdvStringGrid->CellProperties[8][0]->FontColor = clAl ;

	AdvStringGrid->CellProperties[9][0]->FontColor  = clBlack;
	AdvStringGrid->CellProperties[10][0]->FontColor = clBlack;
	AdvStringGrid->CellProperties[11][0]->FontColor = clBlack;
	AdvStringGrid->CellProperties[12][0]->FontColor = clBlack;
	AdvStringGrid->CellProperties[13][0]->FontColor = clBlack;
	AdvStringGrid->CellProperties[14][0]->FontColor = clBlack;
	AdvStringGrid->CellProperties[15][0]->FontColor = clBlack;

	AdvStringGrid->CellProperties[16][0]->FontColor = clBlack;
	AdvStringGrid->CellProperties[17][0]->FontColor = clBlack;
	AdvStringGrid->CellProperties[18][0]->FontColor = clBlack;
	AdvStringGrid->CellProperties[19][0]->FontColor = clBlack;
	AdvStringGrid->CellProperties[20][0]->FontColor = clBlack;
	AdvStringGrid->CellProperties[21][0]->FontColor = clBlack;

	TKControllers *cont = new TKControllers(Global1);
	cont->InitProperties(Global1);
	TDateTime dt;
	dt =  cont->TimeEveningToNight;
	night1  = dt.FormatString("hh:mm:ss");
	night2  = "24:00:00";
	dt =  cont->TimeNightToMorning;
	morning = dt.FormatString("hh:mm:ss");

	/*
	night1  = Controllers->ReadProperty("ASD","TimeEveningToNightLight","00:00:00");
	night2  = "24:00:00";
	morning = Controllers->ReadProperty("ASD","TimeNightToMorningLight","06:00:00");
	*/

	//	double dni = DateDo - DateOt;      // если период больше 2х дней,
	/*
	if ( sutki )                   // то график по суткам
		Chart->BottomAxis->Increment=1;
	else                               // иначе по часам
		Chart->BottomAxis->Increment =1/24;
	*/
  Chart->UndoZoom();
}

void __fastcall TKGrafIU::GrafPaintParam(UnicodeString pp, UnicodeString Date1, UnicodeString Date2, int param)
{
// Проверяем соединение к ораклу и если нет восстанавливаем
   if( !Global1->Connection->Connected) Global1->Connection->PerformConnect(true);
	  TDateTime t_on, t_off;
	  t_on      = 0;
	  t_off     = 0;
	  Series7->Clear();
// Формируем SQL запрос к базе
/*int valparam;
if (param == 0) valparam = 1;
if (param == 1) valparam = 2;
if (param == 2) valparam = 3;
if (param == 3) valparam = 256;
if (param == 4) valparam = 512;
if (param == 5) valparam = 1024;
if (param == 6) valparam = 16;
if (param == 7) valparam = 32;
if (param == 8) valparam = 64;
if (param == 9) valparam = 128;
if (param == 10) valparam = 16;
if (param == 11) valparam = 64;

   UniQuery1->Close();
   UniQuery1->SQL->Clear();
   UniQuery1->SQL->Add("SELECT * FROM " + Controllers->TableArchive);
   if (param < 9)
		UniQuery1->SQL->Add("WHERE arh_pp=:a1 AND arh_dt >= :a2 AND arh_dt <= :a3 AND BITAND(ARH_FAILABC,:a4) != 0");
   else
		UniQuery1->SQL->Add("WHERE arh_pp=:a1 AND arh_dt >= :a2 AND arh_dt <= :a3 AND BITAND(ARH_DISCRETALL,:a4) != 0");
   UniQuery1->SQL->Add("ORDER BY arh_dt");
   UniQuery1->Params->Items[0]->AsString   = punkt;
   UniQuery1->Params->Items[1]->AsDateTime = DateOt;
   UniQuery1->Params->Items[2]->AsDateTime = DateDo;
   UniQuery1->Params->Items[3]->AsInteger  = valparam;
   */
kza = false;
kzb = false;
kzc = false;
pera = false;
perb = false;
perc = false;
sima = false;
simb = false;
simc = false;
door = false;
guard = false;
swith = false;
contr = false;
if (param == 0) sima = true;
if (param == 1) simb = true;
if (param == 2) simc = true;
if (param == 3) kza = true;
if (param == 4) kzb = true;
if (param == 5) kzc = true;
if (param == 6) pera = true;
if (param == 7) perb = true;
if (param == 8) perc = true;
if (param == 9) guard = true;
if (param == 10) swith = true;
if (param == 11) door = true;
   UniQuery1->Close();
   UniQuery1->SQL->Clear();
   UniQuery1->SQL->Add("SELECT * FROM " + Controllers->TableArchive);
   UniQuery1->SQL->Add("WHERE arh_pp=:a1 AND arh_dt >= :a2 AND arh_dt <= :a3");
   UniQuery1->SQL->Add("ORDER BY arh_dt");
   UniQuery1->Params->Items[0]->AsString = punkt;
   UniQuery1->Params->Items[1]->AsDateTime = DateOt;
   UniQuery1->Params->Items[2]->AsDateTime = DateDo;
// построение графика  аварии
   double dtAl = 0;
   int i=0;
   TDateTime dt;
   int max;
   for( i=0,UniQuery1->Open(); !UniQuery1->Eof;UniQuery1->Next())
   {
   if (dt != UniQuery1->FieldByName("arh_dt")->AsDateTime)
	   {
	   dt = UniQuery1->FieldByName("arh_dt")->AsDateTime;
	   double ia    = UniQuery1->FieldByName("arh_ia")->AsInteger;// >AsFloat;
	   double ib    = UniQuery1->FieldByName("arh_ib")->AsInteger;// >AsFloat;
	   double ic    = UniQuery1->FieldByName("arh_ic")->AsInteger;// >AsFloat;
	   double ua    = UniQuery1->FieldByName("arh_ua")->AsInteger;// >AsFloat;
	   double ub    = UniQuery1->FieldByName("arh_ub")->AsInteger;// >AsFloat;
	   double uc    = UniQuery1->FieldByName("arh_uc")->AsInteger;// >AsFloat;
	   double val   = UniQuery1->FieldByName("arh_link")->AsFloat;
	   int abc   = UniQuery1->FieldByName("arh_failabc")->AsInteger;
	   if ((val == 1) && ((ia<0) || (ib<0) || (ic<0) || (ua<0) || (ub<0) || (uc<0)))
	   {
		 continue;
	   }
	   if ((val == 0) && ((ia>0) || (ib>0) || (ic>0) || (ua>0) || (ub>0) || (uc>0)))
	   {
		 continue;
	   }
		i++;
	   AdvStringGrid->Cells[8][i] = "";
	   if ((abc & PARAM_SemA) && (sima))   AdvStringGrid->Cells[8][i] = AdvStringGrid->Cells[8][i] + "СимА,";
	   if ((abc & PARAM_SemB) && (simb))   AdvStringGrid->Cells[8][i] = AdvStringGrid->Cells[8][i] + "СимB,";
	   if ((abc & PARAM_SemC) && (simc))   AdvStringGrid->Cells[8][i] = AdvStringGrid->Cells[8][i] + "СимC,";
	   if ((abc & PARAM_PerA) && (pera))   AdvStringGrid->Cells[8][i] = AdvStringGrid->Cells[8][i] + "ПерA,";
	   if ((abc & PARAM_PerB) && (perb))   AdvStringGrid->Cells[8][i] = AdvStringGrid->Cells[8][i] + "ПерB,";
	   if ((abc & PARAM_PerC) && (perc))   AdvStringGrid->Cells[8][i] = AdvStringGrid->Cells[8][i] + "ПерC,";
	   if ((abc & PARAM_KZA)  && (kza))	   AdvStringGrid->Cells[8][i] = AdvStringGrid->Cells[8][i] + "КЗА,";
	   if ((abc & PARAM_KZB)  && (kzb))	   AdvStringGrid->Cells[8][i] = AdvStringGrid->Cells[8][i] + "КЗВ,";
	   if ((abc & PARAM_KZC)  && (kzc))	   AdvStringGrid->Cells[8][i] = AdvStringGrid->Cells[8][i] + "КЗC,";

	   if (Global1->CheckOfExistsColumn(Controllers->TableArchive,"ARH_DISCRETALL"))
	   {
		abc   = UniQuery1->FieldByName("ARH_DISCRETALL")->AsInteger;
		if (!(abc & PARAM_Switch) && (swith))  AdvStringGrid->Cells[8][i] = AdvStringGrid->Cells[8][i] + "Тумб.Откл,";
		//if (!(abc & PARAM_Contrl) && (contr))  AdvStringGrid->Cells[8][i] = AdvStringGrid->Cells[8][i] + "Руч.реж,";
		if ((abc & PARAM_Door)    && (door))   AdvStringGrid->Cells[8][i] = AdvStringGrid->Cells[8][i] + "Дв,";
		if ((abc & PARAM_Guard)   && (guard))  AdvStringGrid->Cells[8][i] = AdvStringGrid->Cells[8][i] + "Охр,";
	   }
	   AdvStringGrid->CellProperties[8][i]->FontColor = clAl;

		   // авария
	   if (AdvStringGrid->Cells[8][i] != "" )
	   {
			 Series7->AddXY(dt,Chart->LeftAxis->Maximum * 0.6,"",clAl );
	   }
	   else
	   {
			Series7->AddXY(dt,-1,"",clAl );
	   }
	  }
}
}

void __fastcall TKGrafIU::GrafPaintAlarm(UnicodeString pp, UnicodeString Date1, UnicodeString Date2)
{
try
{
	Series33->Clear();			 // тревоги фазы А
	Series34->Clear();			 // тревоги фазы В
	Series35->Clear();			 // тревоги фазы С
	punkt = pp;
	TryStrToDateTime(Date1,DateOt);
	TryStrToDateTime(Date2,DateDo);
	DateDo = DateDo + 1;
if (Alarm->Count > 0)
{
   UniQuery1->Close();
   UniQuery1->SQL->Clear();
   UniQuery1->SQL->Add("SELECT * FROM " + Controllers->TableArchive);
   UniQuery1->SQL->Add("WHERE arh_pp=:a1 AND arh_dt >= :a2 AND arh_dt <= :a3");
   UniQuery1->SQL->Add("ORDER BY arh_dt DESC");
   UniQuery1->Params->Items[0]->AsString = punkt;
   UniQuery1->Params->Items[1]->AsDateTime = DateOt;
   UniQuery1->Params->Items[2]->AsDateTime = DateDo;
   UniQuery1->Open();
   TDateTime data = UniQuery1->FieldByName("arh_dt")->AsDateTime;
   UniQuery1->Last();
   TDateTime data1 = UniQuery1->FieldByName("arh_dt")->AsDateTime;


   UniQuery1->Close();
   UniQuery1->SQL->Clear();
   UniQuery1->SQL->Add("SELECT s.v_pp,t.V_DATE_TIME_VOZ_TREV, t.V_DATE_TIME_SNIAT_TREV,V_ALARM_NAME  ,V_ALARM_SPRAV_PHASE");
   UniQuery1->SQL->Add("FROM j_trevogi t,j_srpav_pp s, alarm_sprav al");
   UniQuery1->SQL->Add("WHERE t.O_ALARM_SPRAV_ID=al.id_object AND s.id_object=o_pp AND s.v_pp=:a0 AND");
   UniQuery1->SQL->Add("((t.V_DATE_TIME_VOZ_TREV>=:a1 AND t.V_DATE_TIME_SNIAT_TREV<=:a2) OR ");
   UniQuery1->SQL->Add("(t.V_DATE_TIME_VOZ_TREV<=:a1 AND t.V_DATE_TIME_SNIAT_TREV>=:a2) OR");
   UniQuery1->SQL->Add("(t.V_DATE_TIME_VOZ_TREV>=:a1 AND t.V_DATE_TIME_VOZ_TREV<=:a2 AND t.V_DATE_TIME_SNIAT_TREV>=:a2) OR");
   UniQuery1->SQL->Add("(t.V_DATE_TIME_VOZ_TREV<=:a1 AND t.V_DATE_TIME_SNIAT_TREV>=:a1 AND t.V_DATE_TIME_SNIAT_TREV<=:a2) OR");
   UniQuery1->SQL->Add("(t.V_DATE_TIME_VOZ_TREV<=:a2 AND t.V_DATE_TIME_SNIAT_TREV is NULL))");
   UniQuery1->SQL->Add("AND (V_ALARM_GROUP_EVENT=:a3 ");
   for (int i=1; i < Alarm->Count;i++)
   {
	 UniQuery1->SQL->Add("OR V_ALARM_GROUP_EVENT=:a"+IntToStr(i+3));
   }
   UniQuery1->SQL->Add(")");
   UniQuery1->Params->Items[0]->AsString = punkt;
   UniQuery1->Params->Items[1]->AsDateTime = DateOt;
   UniQuery1->Params->Items[2]->AsDateTime = DateDo;
   for (int i=0; i < Alarm->Count;i++)
   {
	 UniQuery1->Params->Items[i+3]->AsString = Alarm->Strings[i];
   }
   //ShowMessage(UniQuery1->SQL->Text);
   //UniQuery1->Params->Items[4]->AsString = "C";
   for(UniQuery1->Open();!UniQuery1->Eof;UniQuery1->Next())
   {
		TDateTime voz = UniQuery1->FieldByName("V_DATE_TIME_VOZ_TREV")->AsDateTime;
		TDateTime sni = UniQuery1->FieldByName("V_DATE_TIME_SNIAT_TREV")->AsDateTime+0.00001;
		 if (sni > DateDo) sni = data;//DateDo;
		 if (sni < DateOt) sni = data;//DateDo;
		 if (voz < DateOt) voz =  data1;//DateDo;
		if (UniQuery1->FieldByName("V_ALARM_SPRAV_PHASE")->AsString == "A")
			Series33->AddGanttColor(voz,sni,15,UniQuery1->FieldByName("V_ALARM_NAME")->AsString,clIa);
		if (UniQuery1->FieldByName("V_ALARM_SPRAV_PHASE")->AsString == "B")
			Series34->AddGanttColor(voz,sni,9,UniQuery1->FieldByName("V_ALARM_NAME")->AsString,clIb);
		if (UniQuery1->FieldByName("V_ALARM_SPRAV_PHASE")->AsString == "C")
			Series35->AddGanttColor(voz,sni,3,UniQuery1->FieldByName("V_ALARM_NAME")->AsString,clIc);
   }
   /*
   UniQuery1->Close();
   UniQuery1->SQL->Clear();
   UniQuery1->SQL->Add("SELECT s.v_pp,t.V_DATE_TIME_VOZ_TREV, t.V_DATE_TIME_SNIAT_TREV,V_ALARM_NAME  ,V_ALARM_SPRAV_PHASE");
   UniQuery1->SQL->Add("FROM j_trevogi t,j_srpav_pp s, alarm_sprav al");
   UniQuery1->SQL->Add("WHERE t.O_ALARM_SPRAV_ID=al.id_object AND s.id_object=o_pp AND");
   UniQuery1->SQL->Add("s.v_pp=:a0 AND t.V_DATE_TIME_VOZ_TREV>=:a1 AND t.V_DATE_TIME_SNIAT_TREV<=:a2 ");
   UniQuery1->SQL->Add("AND ( V_ALARM_GROUP_EVENT=:a3 ");// AND V_ALARM_SPRAV_PHASE=:a4 ");
   for (int i=1; i < Alarm->Count;i++)
   {
	 UniQuery1->SQL->Add("OR V_ALARM_GROUP_EVENT=:a"+IntToStr(i+3));
   }
   UniQuery1->SQL->Add(")");
   UniQuery1->Params->Items[0]->AsString = punkt;
   UniQuery1->Params->Items[1]->AsDateTime = DateOt;
   UniQuery1->Params->Items[2]->AsDateTime = DateDo;
   for (int i=0; i < Alarm->Count;i++)
   {
	 UniQuery1->Params->Items[i+3]->AsString = Alarm->Strings[i];
   }
   //ShowMessage(UniQuery1->SQL->Text);
   //UniQuery1->Params->Items[4]->AsString = "C";
   for(UniQuery1->Open();!UniQuery1->Eof;UniQuery1->Next())
   {
		TDateTime voz = UniQuery1->FieldByName("V_DATE_TIME_VOZ_TREV")->AsDateTime;
		TDateTime sni = UniQuery1->FieldByName("V_DATE_TIME_SNIAT_TREV")->AsDateTime+0.00001;
		 if (sni > DateDo) sni = data;//DateDo;
		 if (voz < DateOt) voz =  data1;//DateDo;
		if (UniQuery1->FieldByName("V_ALARM_SPRAV_PHASE")->AsString == "A")
			Series33->AddGanttColor(voz,sni,15,UniQuery1->FieldByName("V_ALARM_NAME")->AsString,clIa);
		if (UniQuery1->FieldByName("V_ALARM_SPRAV_PHASE")->AsString == "B")
			Series34->AddGanttColor(voz,sni,9,UniQuery1->FieldByName("V_ALARM_NAME")->AsString,clIb);
		if (UniQuery1->FieldByName("V_ALARM_SPRAV_PHASE")->AsString == "C")
			Series35->AddGanttColor(voz,sni,3,UniQuery1->FieldByName("V_ALARM_NAME")->AsString,clIc);
   }

   UniQuery1->Close();
   UniQuery1->SQL->Clear();
   UniQuery1->SQL->Add("SELECT s.v_pp,t.V_DATE_TIME_VOZ_TREV, t.V_DATE_TIME_SNIAT_TREV,V_ALARM_NAME  ,V_ALARM_SPRAV_PHASE");
   UniQuery1->SQL->Add("FROM j_trevogi t,j_srpav_pp s, alarm_sprav al");
   UniQuery1->SQL->Add("WHERE t.O_ALARM_SPRAV_ID=al.id_object AND s.id_object=o_pp AND");
   UniQuery1->SQL->Add("s.v_pp=:a0 AND t.V_DATE_TIME_VOZ_TREV>=:a1 AND t.V_DATE_TIME_VOZ_TREV<=:a2 AND t.V_DATE_TIME_SNIAT_TREV>=:a3");
   UniQuery1->SQL->Add("AND ( V_ALARM_GROUP_EVENT=:a4 ");// AND V_ALARM_SPRAV_PHASE=:a4 ");
   for (int i=1; i < Alarm->Count;i++)
   {
	 UniQuery1->SQL->Add("OR V_ALARM_GROUP_EVENT=:a"+IntToStr(i+4));
   }
   UniQuery1->SQL->Add(")");
   UniQuery1->Params->Items[0]->AsString = punkt;
   UniQuery1->Params->Items[1]->AsDateTime = DateOt;
   UniQuery1->Params->Items[2]->AsDateTime = DateDo;
   UniQuery1->Params->Items[3]->AsDateTime = DateDo;
   for (int i=0; i < Alarm->Count;i++)
   {
	 UniQuery1->Params->Items[i+4]->AsString = Alarm->Strings[i];
   }
   //ShowMessage(UniQuery1->SQL->Text);
   //UniQuery1->Params->Items[4]->AsString = "C";
   for(UniQuery1->Open();!UniQuery1->Eof;UniQuery1->Next())
   {
		TDateTime voz = UniQuery1->FieldByName("V_DATE_TIME_VOZ_TREV")->AsDateTime;
		TDateTime sni = UniQuery1->FieldByName("V_DATE_TIME_SNIAT_TREV")->AsDateTime+0.00001;
		 if (sni > DateDo) sni = data;//DateDo;
		 if (voz < DateOt) voz =  data1;//DateDo;
		if (UniQuery1->FieldByName("V_ALARM_SPRAV_PHASE")->AsString == "A")
			Series33->AddGanttColor(voz,sni,15,UniQuery1->FieldByName("V_ALARM_NAME")->AsString,clIa);
		if (UniQuery1->FieldByName("V_ALARM_SPRAV_PHASE")->AsString == "B")
			Series34->AddGanttColor(voz,sni,9,UniQuery1->FieldByName("V_ALARM_NAME")->AsString,clIb);
		if (UniQuery1->FieldByName("V_ALARM_SPRAV_PHASE")->AsString == "C")
			Series35->AddGanttColor(voz,sni,3,UniQuery1->FieldByName("V_ALARM_NAME")->AsString,clIc);
   }

   UniQuery1->Close();
   UniQuery1->SQL->Clear();
   UniQuery1->SQL->Add("SELECT s.v_pp,t.V_DATE_TIME_VOZ_TREV, t.V_DATE_TIME_SNIAT_TREV,V_ALARM_NAME  ,V_ALARM_SPRAV_PHASE");
   UniQuery1->SQL->Add("FROM j_trevogi t,j_srpav_pp s, alarm_sprav al");
   UniQuery1->SQL->Add("WHERE t.O_ALARM_SPRAV_ID=al.id_object AND s.id_object=o_pp AND");
   UniQuery1->SQL->Add("s.v_pp=:a0 AND t.V_DATE_TIME_VOZ_TREV<=:a1 AND t.V_DATE_TIME_SNIAT_TREV>=:a2");
   UniQuery1->SQL->Add("AND ( V_ALARM_GROUP_EVENT=:a3 ");// AND V_ALARM_SPRAV_PHASE=:a4 ");
   for (int i=1; i < Alarm->Count;i++)
   {
	 UniQuery1->SQL->Add("OR V_ALARM_GROUP_EVENT=:a"+IntToStr(i+3));
   }
   UniQuery1->SQL->Add(")");
   UniQuery1->Params->Items[0]->AsString = punkt;
   UniQuery1->Params->Items[1]->AsDateTime = DateOt;
   UniQuery1->Params->Items[2]->AsDateTime = DateDo;
   for (int i=0; i < Alarm->Count;i++)
   {
	 UniQuery1->Params->Items[i+3]->AsString = Alarm->Strings[i];
   }
   //ShowMessage(UniQuery1->SQL->Text);
   //UniQuery1->Params->Items[4]->AsString = "C";
   for(UniQuery1->Open();!UniQuery1->Eof;UniQuery1->Next())
   {
		TDateTime voz = UniQuery1->FieldByName("V_DATE_TIME_VOZ_TREV")->AsDateTime;
		TDateTime sni = UniQuery1->FieldByName("V_DATE_TIME_SNIAT_TREV")->AsDateTime+0.00001;
		 if (sni > DateDo) sni = data;//DateDo;
		 if (voz < DateOt) voz =  data1;//DateDo;
		if (UniQuery1->FieldByName("V_ALARM_SPRAV_PHASE")->AsString == "A")
			Series33->AddGanttColor(voz,sni,15,UniQuery1->FieldByName("V_ALARM_NAME")->AsString,clIa);
		if (UniQuery1->FieldByName("V_ALARM_SPRAV_PHASE")->AsString == "B")
			Series34->AddGanttColor(voz,sni,9,UniQuery1->FieldByName("V_ALARM_NAME")->AsString,clIb);
		if (UniQuery1->FieldByName("V_ALARM_SPRAV_PHASE")->AsString == "C")
			Series35->AddGanttColor(voz,sni,3,UniQuery1->FieldByName("V_ALARM_NAME")->AsString,clIc);
   }

   UniQuery1->Close();
   UniQuery1->SQL->Clear();
   UniQuery1->SQL->Add("SELECT s.v_pp,t.V_DATE_TIME_VOZ_TREV, t.V_DATE_TIME_SNIAT_TREV,V_ALARM_NAME  ,V_ALARM_SPRAV_PHASE");
   UniQuery1->SQL->Add("FROM j_trevogi t,j_srpav_pp s, alarm_sprav al");
   UniQuery1->SQL->Add("WHERE t.O_ALARM_SPRAV_ID=al.id_object AND s.id_object=o_pp AND");
   UniQuery1->SQL->Add("s.v_pp=:a0 AND t.V_DATE_TIME_VOZ_TREV<=:a1 AND t.V_DATE_TIME_SNIAT_TREV>=:a2 AND t.V_DATE_TIME_SNIAT_TREV<=:a3");
   UniQuery1->SQL->Add("AND ( V_ALARM_GROUP_EVENT=:a4 ");// AND V_ALARM_SPRAV_PHASE=:a4 ");
   for (int i=1; i < Alarm->Count;i++)
   {
	 UniQuery1->SQL->Add("OR V_ALARM_GROUP_EVENT=:a"+IntToStr(i+4));
   }
   UniQuery1->SQL->Add(")");
   UniQuery1->Params->Items[0]->AsString = punkt;
   UniQuery1->Params->Items[1]->AsDateTime = DateOt;
   UniQuery1->Params->Items[2]->AsDateTime = DateOt;
   UniQuery1->Params->Items[3]->AsDateTime = DateDo;
   for (int i=0; i < Alarm->Count;i++)
   {
	 UniQuery1->Params->Items[i+4]->AsString = Alarm->Strings[i];
   }
   //ShowMessage(UniQuery1->SQL->Text);
   //UniQuery1->Params->Items[4]->AsString = "C";
   for(UniQuery1->Open();!UniQuery1->Eof;UniQuery1->Next())
   {
		TDateTime voz = UniQuery1->FieldByName("V_DATE_TIME_VOZ_TREV")->AsDateTime;
		TDateTime sni = UniQuery1->FieldByName("V_DATE_TIME_SNIAT_TREV")->AsDateTime+0.00001;
		 if (sni > DateDo) sni = data;//DateDo;
		 if (voz < DateOt) voz =  data1;//DateDo;
		if (UniQuery1->FieldByName("V_ALARM_SPRAV_PHASE")->AsString == "A")
			Series33->AddGanttColor(voz,sni,15,UniQuery1->FieldByName("V_ALARM_NAME")->AsString,clIa);
		if (UniQuery1->FieldByName("V_ALARM_SPRAV_PHASE")->AsString == "B")
			Series34->AddGanttColor(voz,sni,9,UniQuery1->FieldByName("V_ALARM_NAME")->AsString,clIb);
		if (UniQuery1->FieldByName("V_ALARM_SPRAV_PHASE")->AsString == "C")
			Series35->AddGanttColor(voz,sni,3,UniQuery1->FieldByName("V_ALARM_NAME")->AsString,clIc);
   }   */

}
}
catch(...)
{

}
}


//---------------------------------------------------------------------------

void __fastcall TKGrafIU::GrafPaintToki()
{
try
{
// Проверяем соединение к ораклу и если нет восстанавливаем
   if( !Global1->Connection->Connected) Global1->Connection->PerformConnect(true);
	  TDateTime t_on, t_off;
	  t_on      = 0;
	  t_off     = 0;

   UnicodeString Ia_min,Ia_max,Ib_min,Ib_max,Ic_min,Ic_max,modd;

// Формируем SQL запрос к базе
   UniQuery1->Close();
   UniQuery1->SQL->Clear();
   UniQuery1->SQL->Add("SELECT * FROM " + Controllers->TableArchive);
   UniQuery1->SQL->Add("WHERE arh_pp=:a1 AND arh_dt >= :a2 AND arh_dt <= :a3");
   UniQuery1->SQL->Add("ORDER BY arh_dt");
   UniQuery1->Params->Items[0]->AsString = punkt;
   UniQuery1->Params->Items[1]->AsDateTime = DateOt;
   UniQuery1->Params->Items[2]->AsDateTime = DateDo;
   double dni = DateDo - DateOt;     // если период больше 2х дней,

   double imax = 10;
   int i =0;
   UnicodeString dat_on = "";
   UnicodeString dat_off ="";
   UniQuery2->Connection = UniQuery1->Connection;
   TDateTime dt,dt1,dt3,data1,data2;
   UnicodeString dat, dat1;
   UniQuery1->Open();
   data1 = UniQuery1->FieldByName("arh_dt")->AsDateTime;
   dat = UniQuery1->FieldByName("arh_dt")->AsDateTime.TimeString();
   UniQuery1->Last();
   data2 = UniQuery1->FieldByName("arh_dt")->AsDateTime;
   UniQuery1->Close();

   TryStrToDateTime(DateOt.DateString() + " 00:00:00",DateOt);
   TryStrToDateTime(DateDo.DateString() + " 00:00:00",DateDo);

   UniQuery2->Close();
   UniQuery2->SQL->Clear();
   UniQuery2->SQL->Add("SELECT v_calendar_date AS c_date,");
   UniQuery2->SQL->Add("v_vkl_bez_uchet_perehod AS t_on,v_vikl_bez_uchet_perehod AS t_off,");
   UniQuery2->SQL->Add("v_vkl_uchet_perehod AS t_on_s,v_vikl_uchet_perehod AS t_off_s");
   UniQuery2->SQL->Add(" FROM j_graf_chas_goren WHERE v_calendar_date >= :a0 AND v_calendar_date < :a1");
   UniQuery2->Params->Items[0]->AsDateTime = DateOt;
   UniQuery2->Params->Items[1]->AsDateTime = (DateOt+1);
   UniQuery2->Open();
   int j = UniQuery2->RecordCount;

   if( !UniQuery2->Eof )
   {
	t_on  = DateOt + 1 + UniQuery2->FieldByName("t_on")->AsDateTime - int(UniQuery2->FieldByName("t_on")->AsDateTime) - Shift;
	t_off = DateOt + 1 + UniQuery2->FieldByName("t_off")->AsDateTime - int(UniQuery2->FieldByName("t_off")->AsDateTime) + Shift;
	if( t_on > t_off )t_on = t_on - 1;
   }
   dat_on = t_on.TimeString();
   if (t_on.FormatString("hh") == "00")//dat_on == "0:00:00")
   {
       dat_on = "23:59:59";
   }
   dat_off = t_off.TimeString();
   if (dat_on.Length() < 8) dat_on = "0" + dat_on;
   if (dat_off.Length() < 8) dat_off = "0" + dat_off;

   double ia1 = 0;
   double ib1 = 0;
   double ic1 = 0;
   double ua1 = 0;
   double ub1 = 0;
   double uc1 = 0;
// заполнение таблицы значений, графика токов и суточной заливки
   for( i=0,UniQuery1->Open(); !UniQuery1->Eof; UniQuery1->Next())
   {
   if (dt != UniQuery1->FieldByName("arh_dt")->AsDateTime)
	{
	   dt = UniQuery1->FieldByName("arh_dt")->AsDateTime;

		mod = mrtUnknown;
		mod = Controllers->CurrentModeRT(dt);
		/*
		mrtDay          = 1,
		mrtNight        = 2,
		mrtEvening      = 3,
		mrtMorning      = 4
		*/
		UniQuery2->Close();
		UniQuery2->SQL->Clear();
		UniQuery2->SQL->Add(" select n.* from J_NORM_PARAM_PP n, j_srpav_pp pp");
		UniQuery2->SQL->Add(" where pp.id_object = n.o_pp AND V_MODE_NORM_PARAM_PP = :a0 AND v_pp = :a1");
		UniQuery2->SQL->Add(" AND v_date_norm_param_pp<=:a2");
		UniQuery2->SQL->Add(" Order by v_date_norm_param_pp desc");
		if (mod == mrtDay)
		{
			UniQuery2->Params->Items[0]->AsString = "День";
			modd = "День";
		} else
			if (mod == mrtNight)
			{
				UniQuery2->Params->Items[0]->AsString = "Ночь";
				modd = "Ночь";
			}  else
				if ((mod == mrtEvening) || (mod == mrtMorning))
				{
				UniQuery2->Params->Items[0]->AsString = "Вечер";
				modd = "Вечер";
				}  else
				{
					UniQuery2->Params->Items[0]->AsString   = "Утро";
					modd = "Утро";
					 // иначе нет данных
				}
		UniQuery2->Params->Items[1]->AsString   = punkt;
		UniQuery2->Params->Items[2]->AsDateTime = dt;
		UniQuery2->Open();
		if( !UniQuery2->Eof )
		{
			Ia_min = UniQuery2->FieldByName("V_IA_MIN_NORM_PARAM_PP")->AsString;
			Ia_max = UniQuery2->FieldByName("V_IA_MAX_NORM_PARAM_PP")->AsString;
			Ib_min = UniQuery2->FieldByName("V_IB_MIN_NORM_PARAM_PP")->AsString;
			Ib_max = UniQuery2->FieldByName("V_IB_MAX_NORM_PARAM_PP")->AsString;
			Ic_min = UniQuery2->FieldByName("V_IC_MIN_NORM_PARAM_PP")->AsString;
			Ic_max = UniQuery2->FieldByName("V_IC_MAX_NORM_PARAM_PP")->AsString;
		}
		else
		{
			Ia_min = 0;
			Ia_max = 0;
			Ib_min = 0;
			Ib_max = 0;
			Ic_min = 0;
			Ic_max = 0;
		}
		UniQuery2->Close();

	   double ia    = UniQuery1->FieldByName("arh_ia")->AsFloat; //>AsInteger;// >AsFloat;
	   double ib    = UniQuery1->FieldByName("arh_ib")->AsFloat; //>AsInteger;// >AsFloat;
	   double ic    = UniQuery1->FieldByName("arh_ic")->AsFloat; //>AsInteger;// >AsFloat;
	   double ua    = UniQuery1->FieldByName("arh_ua")->AsInteger;// >AsFloat;
	   double ub    = UniQuery1->FieldByName("arh_ub")->AsInteger;// >AsFloat;
	   double uc    = UniQuery1->FieldByName("arh_uc")->AsInteger;// >AsFloat;
	   double m     = UniQuery1->FieldByName("arh_mode")->AsInteger;// >AsFloat;
	   double val   = UniQuery1->FieldByName("arh_link")->AsFloat;

	   if( imax < ia )imax = ia;
	   if( imax < ib )imax = ib;
	   if( imax < ic )imax = ic;
	   /*
	   if( imax < ua )imax = ua;
	   if( imax < ub )imax = ub;
	   if( imax < uc )imax = uc;
	   */
	   if ((val == 1) && ((ia<0) || (ib<0) || (ic<0) || (ua<0) || (ub<0) || (uc<0)))
	   {
		 continue;
	   }
       if ((val == 0) && ((ia>0) || (ib>0) || (ic>0) || (ua>0) || (ub>0) || (uc>0)))
	   {
		 continue;
	   }
	   i++;
	   if ((val == 0) && ((ia<0) || (ib<0) || (ic<0) || (ua<0) || (ub<0) || (uc<0)))
	   {
		  AdvStringGrid->RowCount = i+1;
		  AdvStringGrid->Cells[0][i] = dt.FormatString("dd.mm.yy    hh:nn");
		  AdvStringGrid->Cells[1][i] = "Н/Д";
		  AdvStringGrid->Cells[2][i] = "Н/Д";
		  AdvStringGrid->Cells[3][i] = "Н/Д";
		  AdvStringGrid->Cells[4][i] = "Н/Д";
		  AdvStringGrid->Cells[5][i] = "Н/Д";
		  AdvStringGrid->Cells[6][i] = "Н/Д";

		  AdvStringGrid->Cells[9][i] = modd;
		  AdvStringGrid->Cells[10][i] = Ia_min;
		  AdvStringGrid->Cells[11][i] = Ia_max;
		  AdvStringGrid->Cells[12][i] = Ib_min;
		  AdvStringGrid->Cells[13][i] = Ib_max;
		  AdvStringGrid->Cells[14][i] = Ic_min;
		  AdvStringGrid->Cells[15][i] = Ic_max;

	   }
	   else   if ((ia>=0) && (ib>=0) && (ic>=0) && (ua>=0) && (ub>=0) && (uc>=0))
	   {
		  AdvStringGrid->RowCount = i+1;
		  AdvStringGrid->Cells[0][i] = dt.FormatString("dd.mm.yy    hh:nn");
		  AdvStringGrid->Cells[1][i] = FloatToStr((int)ia);
		  AdvStringGrid->Cells[2][i] = FloatToStr((int)ib);
		  AdvStringGrid->Cells[3][i] = FloatToStr((int)ic);
		  AdvStringGrid->Cells[4][i] = FloatToStr(ua);
		  AdvStringGrid->Cells[5][i] = FloatToStr(ub);
		  AdvStringGrid->Cells[6][i] = FloatToStr(uc);

		  AdvStringGrid->Cells[9][i] = modd;
		  AdvStringGrid->Cells[10][i] = Ia_min;
		  AdvStringGrid->Cells[11][i] = Ia_max;
		  AdvStringGrid->Cells[12][i] = Ib_min;
		  AdvStringGrid->Cells[13][i] = Ib_max;
		  AdvStringGrid->Cells[14][i] = Ic_min;
		  AdvStringGrid->Cells[15][i] = Ic_max;


		 /* if (modd == "День")
		  {
			nd++;
			ia_d = ia_d + (int)ia;
			ib_d = ib_d + (int)ib;
			ic_d = ic_d + (int)ic;
		  }
		  if (modd == "Ночь")
		  {
			nn++;
			ia_n = ia_n + (int)ia;
			ib_n = ib_n + (int)ib;
			ic_n = ic_n + (int)ic;
		  }
		  if (modd == "Утро")
		  {
			nu++;
			ia_u = ia_u + (int)ia;
			ib_u = ib_u + (int)ib;
			ic_u = ic_u + (int)ic;
		  }
		  if (modd == "Вечер")
		  {
			nv++;
			ia_v = ia_v + (int)ia;
			ib_v = ib_v + (int)ib;
			ic_v = ic_v + (int)ic;
		  }    */
	   }

	   AdvStringGrid->CellProperties[1][i]->FontColor = clIa;
	   AdvStringGrid->CellProperties[2][i]->FontColor = clIb;
	   AdvStringGrid->CellProperties[3][i]->FontColor = clIc;
	   AdvStringGrid->CellProperties[4][i]->FontColor = clUa;
	   AdvStringGrid->CellProperties[5][i]->FontColor = clUb;
	   AdvStringGrid->CellProperties[6][i]->FontColor = clUc;
// ставим точку на графике токов
		if (ia>=0)
			Series1->AddXY(dt,ia,"",clIa);
		else
			Series1->AddXY(dt,ia1,"",Chart->Color);

		if (ib>=0)
			Series2->AddXY(dt,ib,"",clIb);
		else
			Series2->AddXY(dt,ib1,"",Chart->Color);

		if (ic>=0)
			Series3->AddXY(dt,ic,"",clIc);
		 else
			Series3->AddXY(dt,ic1,"",Chart->Color);

		if (ua>=0)
			Series17->AddXY(dt,ua,"",clUa);
		else
			Series17->AddXY(dt,ua1,"",Chart->Color);

		if (ub>=0)
			Series18->AddXY(dt,ub,"",clUb);
		else
			Series18->AddXY(dt,ub1,"",Chart->Color);

		if (uc>=0)
			Series19->AddXY(dt,uc,"",clUc);
		else
			Series19->AddXY(dt,uc1,"",Chart->Color);

	   dat1 = dat;
	   dat = dt.TimeString();
	   if (dat.Length() < 8) dat = "0" + dat;
UniQuery2->Connection = Global1->Query->Connection;
UniQuery2->Close();
UniQuery2->SQL->Clear();
UniQuery2->SQL->Add("SELECT g.v_CodGraphOnOff");
UniQuery2->SQL->Add("FROM j_srpav_pp pp, j_ListGraph g");
UniQuery2->SQL->Add("WHERE pp.o_graphonoff = g.id_object (+)");
UniQuery2->SQL->Add("AND  pp.v_pp=:a1");
UniQuery2->Params->Items[0]->AsString = punkt;
UniQuery2->Open();
CodOnOff = UniQuery2->FieldByName("v_CodGraphOnOff")->AsString;
UniQuery2->Close();
UniQuery2->SQL->Clear();
if (CodOnOff == "0333")
	   {
	   if (modd == "Ночь")
		{
		  for( int j=0; j<AdvStringGrid->ColCount; j++)
			 AdvStringGrid->CellProperties[j][i]->BrushColor = clEvening;         // темно желтый   ночь
			 Series4->AddXY(dt,200,"",clEvening);
		}
		else  if (modd == "Утро")
		  {
			 for( int j=0; j<AdvStringGrid->ColCount; j++)
				AdvStringGrid->CellProperties[j][i]->BrushColor = clEvening;    // желтый утро
				Series4->AddXY(dt,200,"",clEvening);
		  }
		  else
		  if (modd == "Вечер")//if ((dat > dat_on) && (dat < night2))
		  {
			 for( int j=0; j<AdvStringGrid->ColCount; j++)
				AdvStringGrid->CellProperties[j][i]->BrushColor = clEvening;    // желтый  вечер
				Series4->AddXY(dt,200,"",clEvening);
		  }
		  else if (modd == "День")
		  {
			 for( int j=0; j<AdvStringGrid->ColCount; j++)
				AdvStringGrid->CellProperties[j][i]->BrushColor = clDay;        // белый  день
				Series4->AddXY(dt,0,"", clDay);
		  }
	   /*
		 if ((dat >= night1) && (dat <= morning))
		{
		  for( int j=0; j<AdvStringGrid->ColCount; j++)
			 AdvStringGrid->CellProperties[j][i]->BrushColor = clEvening;         // темно желтый   ночь
		}
		else
		{
		  if ((dat > morning) && (dat < dat_off))
		  {
			 for( int j=0; j<AdvStringGrid->ColCount; j++)
				AdvStringGrid->CellProperties[j][i]->BrushColor = clEvening;    // желтый утро
		  }
		  else
		  if (((dat > dat_on) && (dat < night2)) || ((dat < night1) && (night2 > t_on) && (dat > "00:00:00")))//if ((dat > dat_on) && (dat < night2))
		  {
			 for( int j=0; j<AdvStringGrid->ColCount; j++)
				AdvStringGrid->CellProperties[j][i]->BrushColor = clEvening;    // желтый  вечер
		  }
		  else
		  {
			 for( int j=0; j<AdvStringGrid->ColCount; j++)
				AdvStringGrid->CellProperties[j][i]->BrushColor = clDay;        // белый  день
		  }
		 } */
		}
	   else
	   {
		  if (modd == "Ночь")
		{
		  for( int j=0; j<AdvStringGrid->ColCount; j++)
			 AdvStringGrid->CellProperties[j][i]->BrushColor = clNight;         // темно желтый   ночь
			 Series4->AddXY(dt,200,"",clNight);
		}
		else  if (modd == "Утро")
		  {
			 for( int j=0; j<AdvStringGrid->ColCount; j++)
				AdvStringGrid->CellProperties[j][i]->BrushColor = clEvening;    // желтый утро
				Series4->AddXY(dt,200,"",clEvening);
		  }
		  else
		  if (modd == "Вечер")//if ((dat > dat_on) && (dat < night2))
		  {
			 for( int j=0; j<AdvStringGrid->ColCount; j++)
				AdvStringGrid->CellProperties[j][i]->BrushColor = clEvening;    // желтый  вечер
				Series4->AddXY(dt,200,"",clEvening);
		  }
		  else if (modd == "День")
		  {
			 for( int j=0; j<AdvStringGrid->ColCount; j++)
				AdvStringGrid->CellProperties[j][i]->BrushColor = clDay;        // белый  день
				Series4->AddXY(dt,0,"", clDay);
		  }
	   /*
	   if ((dat >= night1) && (dat <= morning))
		{
		  for( int j=0; j<AdvStringGrid->ColCount; j++)
			 AdvStringGrid->CellProperties[j][i]->BrushColor = clNight;         // темно желтый   ночь
		}
		else
		{
		  if ((dat > morning) && (dat < dat_off))
		  {
			 for( int j=0; j<AdvStringGrid->ColCount; j++)
				AdvStringGrid->CellProperties[j][i]->BrushColor = clEvening;    // желтый утро
		  }
		  else
		   if (((dat > dat_on) && (dat < night2)) || ((dat < night1) && (night2 > t_on) && (dat > "00:00:00")))//if ((dat > dat_on) && (dat < night2))
		  {
			 for( int j=0; j<AdvStringGrid->ColCount; j++)
				AdvStringGrid->CellProperties[j][i]->BrushColor = clEvening;    // желтый  вечер
		  }
		  else
		  {
			 for( int j=0; j<AdvStringGrid->ColCount; j++)
				AdvStringGrid->CellProperties[j][i]->BrushColor = clDay;        // белый  день
		  }
		}  */
		}
	   Series5->AddXY(dt,0,"",Chart->Color);
	   if (dt > DateOt+1)
	   {
		UniQuery2->Close();
		UniQuery2->SQL->Clear();
		UniQuery2->SQL->Add("SELECT v_calendar_date AS c_date,");
		UniQuery2->SQL->Add("v_vkl_bez_uchet_perehod AS t_on,v_vikl_bez_uchet_perehod AS t_off,");
		UniQuery2->SQL->Add("v_vkl_uchet_perehod AS t_on_s,v_vikl_uchet_perehod AS t_off_s");
		UniQuery2->SQL->Add(" FROM j_graf_chas_goren WHERE v_calendar_date >= :a0 AND v_calendar_date < :a1");
		UniQuery2->Params->Items[0]->AsDateTime = DateOt;
		UniQuery2->Params->Items[1]->AsDateTime = (DateOt+1);
		UniQuery2->Open();
		int j = UniQuery2->RecordCount;
		if( !UniQuery2->Eof )
		{
			t_on  = DateOt + 1 + UniQuery2->FieldByName("t_on")->AsDateTime - int(UniQuery2->FieldByName("t_on")->AsDateTime) - Shift;
			t_off = DateOt + 1 + UniQuery2->FieldByName("t_off")->AsDateTime - int(UniQuery2->FieldByName("t_off")->AsDateTime) + Shift;
		  	if( t_on > t_off )t_on = t_on - 1;
		}
		dat_on = t_on.TimeString();
		if (t_on.FormatString("hh") == "00")//dat_on == "0:00:00")
		{
			dat_on = "23:59:59";
   		}
		dat_off = t_off.TimeString();
		if (dat_on.Length() < 8) dat_on = "0" + dat_on;
		if (dat_off.Length() < 8) dat_off = "0" + dat_off;
		DateOt = DateOt + 1;
	   }
	   ia1 = ia;
	   ib1 = ib;
	   ic1 = ic;
	   ua1 = ua;
	   ub1 = ub;
	   uc1 = uc;
	}
   }
   UniQuery1->Close();
 /*
   double del; // дельта

   // min и max по всем фазам за день
   if (nd != 0)
   {
	  del = ia_d/nd*0.05;
	  if (del < 2)
	  {
		ia_max_d = ia_d/nd + 2;
		ia_min_d = ia_d/nd - 2;
		if (ia_min_d < 0) ia_min_d = 0;
	  }
	  else
	  {
		ia_max_d = ia_d/nd + del;
		ia_min_d = ia_d/nd - del;
		if (ia_min_d < 0) ia_min_d = 0;
	  }

	  del = ib_d/nd*0.05;
	  if (del < 2)
	  {
		ib_max_d = ib_d/nd + 2;
		ib_min_d = ib_d/nd - 2;
		if (ib_min_d < 0) ib_min_d = 0;
	  }
	  else
	  {
		ib_max_d = ib_d/nd + del;
		ib_min_d = ib_d/nd - del;
		if (ib_min_d < 0) ib_min_d = 0;
	  }

	  del = ic_d/nd*0.05;
	  if (del < 2)
	  {
		ic_max_d = ic_d/nd + 2;
		ic_min_d = ic_d/nd - 2;
		if (ic_min_d < 0) ic_min_d = 0;
	  }
	  else
	  {
		ic_max_d = ic_d/nd + del;
		ic_min_d = ic_d/nd - del;
		if (ic_min_d < 0) ic_min_d = 0;
	  }
   }
   else
   {
	  ia_max_d = 0;
	  ia_min_d = 0;
	  ib_max_d = 0;
	  ib_min_d = 0;
	  ic_max_d = 0;
	  ic_min_d = 0;
   }

   // min и max по всем фазам за ночь
   if (nn != 0)
   {
	  del = ia_n/nn*0.05;
	  if (del < 2)
	  {
		ia_max_n = ia_n/nn + 2;
		ia_min_n = ia_n/nn - 2;
		if (ia_min_n < 0) ia_min_n = 0;
	  }
	  else
	  {
		ia_max_n = ia_n/nn + del;
		ia_min_n = ia_n/nn - del;
		if (ia_min_n < 0) ia_min_n = 0;
	  }

	  del = ib_n/nn*0.05;
	  if (del < 2)
	  {
		ib_max_n = ib_n/nn + 2;
		ib_min_n = ib_n/nn - 2;
		if (ib_min_n < 0) ib_min_n = 0;
	  }
	  else
	  {
		ib_max_n = ib_n/nn + del;
		ib_min_n = ib_n/nn - del;
		if (ib_min_n < 0) ib_min_n = 0;
	  }

	  del = ic_n/nn*0.05;
	  if (del < 2)
	  {
		ic_max_n = ic_n/nn + 2;
		ic_min_n = ic_n/nn - 2;
		if (ic_min_n < 0) ic_min_n = 0;
	  }
	  else
	  {
		ic_max_n = ic_n/nn + del;
		ic_min_n = ic_n/nn - del;
		if (ic_min_n < 0) ic_min_n = 0;
	  }
   }
   else
   {
	  ia_max_n = 0;
	  ia_min_n = 0;
	  ib_max_n = 0;
	  ib_min_n = 0;
	  ic_max_n = 0;
	  ic_min_n = 0;
   }

   // min и max по всем фазам за вечер
   if (nv != 0)
   {
	  del = ia_v/nv*0.05;
	  if (del < 2)
	  {
		ia_max_v = ia_v/nv + 2;
		ia_min_v = ia_v/nv - 2;
		if (ia_min_v < 0) ia_min_v = 0;
	  }
	  else
	  {
		ia_max_v = ia_v/nv + del;
		ia_min_v = ia_v/nv - del;
		if (ia_min_v < 0) ia_min_v = 0;
	  }

	  del = ib_v/nv*0.05;
	  if (del < 2)
	  {
		ib_max_v = ib_v/nv + 2;
		ib_min_v = ib_v/nv - 2;
		if (ib_min_v < 0) ib_min_v = 0;
	  }
	  else
	  {
		ib_max_v = ib_v/nv + del;
		ib_min_v = ib_v/nv - del;
		if (ib_min_v < 0) ib_min_v = 0;
	  }

	  del = ic_v/nv*0.05;
	  if (del < 2)
	  {
		ic_max_v = ic_v/nv + 2;
		ic_min_v = ic_v/nv - 2;
		if (ic_min_v < 0) ic_min_v = 0;
	  }
	  else
	  {
		ic_max_v = ic_v/nv + del;
		ic_min_v = ic_v/nv - del;
		if (ic_min_v < 0) ic_min_v = 0;
	  }
   }
   else
   {
	  ia_max_v = 0;
	  ia_min_v = 0;
	  ib_max_v = 0;
	  ib_min_v = 0;
	  ic_max_v = 0;
	  ic_min_v = 0;
   }

   // min и max по всем фазам за утро
   if (nu != 0)
   {
	  del = ia_u/nu*0.05;
	  if (del < 2)
	  {
		ia_max_u = ia_u/nu + 2;
		ia_min_u = ia_u/nu - 2;
		if (ia_min_u < 0) ia_min_u = 0;
	  }
	  else
	  {
		ia_max_u = ia_u/nu + del;
		ia_min_u = ia_u/nu - del;
		if (ia_min_u < 0) ia_min_u = 0;
	  }

	  del = ib_u/nu*0.05;
	  if (del < 2)
	  {
		ib_max_u = ib_u/nu + 2;
		ib_min_u = ib_u/nu - 2;
		if (ib_min_u < 0) ib_min_u = 0;
	  }
	  else
	  {
		ib_max_u = ib_u/nu + del;
		ib_min_u = ib_u/nu - del;
		if (ib_min_u < 0) ib_min_u = 0;
	  }

	  del = ic_u/nu*0.05;
	  if (del < 2)
	  {
		ic_max_u = ic_u/nu + 2;
		ic_min_u = ic_u/nu - 2;
		if (ic_min_u < 0) ic_min_u = 0;
	  }
	  else
	  {
		ic_max_u = ic_u/nu + del;
		ic_min_u = ic_u/nu - del;
		if (ic_min_u < 0) ic_min_u = 0;
	  }
   }
   else
   {
	  ia_max_u = 0;
	  ia_min_u = 0;
	  ib_max_u = 0;
	  ib_min_u = 0;
	  ic_max_u = 0;
	  ic_min_u = 0;
   }
   for (i = 1; i < AdvStringGrid->RowCount; i++)
   {
		if (AdvStringGrid->Cells[15][i] == "День")
		{
		AdvStringGrid->Cells[16][i]  = FloatToStr(ia_min_d);
		AdvStringGrid->Cells[17][i]  = FloatToStr(ia_max_d);
		AdvStringGrid->Cells[18][i]  = FloatToStr(ib_min_d);
		AdvStringGrid->Cells[19][i]  = FloatToStr(ib_max_d);
		AdvStringGrid->Cells[20][i]  = FloatToStr(ic_min_d);
		AdvStringGrid->Cells[21][i]  = FloatToStr(ic_max_d);
		}
		if (AdvStringGrid->Cells[15][i] == "Ночь")
		{
		AdvStringGrid->Cells[16][i]  = FloatToStr(ia_min_n);
		AdvStringGrid->Cells[17][i]  = FloatToStr(ia_max_n);
		AdvStringGrid->Cells[18][i]  = FloatToStr(ib_min_n);
		AdvStringGrid->Cells[19][i]  = FloatToStr(ib_max_n);
		AdvStringGrid->Cells[20][i]  = FloatToStr(ic_min_n);
		AdvStringGrid->Cells[21][i]  = FloatToStr(ic_max_n);
		}
		if (AdvStringGrid->Cells[15][i] == "Вечер")
		{
		AdvStringGrid->Cells[16][i]  = FloatToStr(ia_min_v);
		AdvStringGrid->Cells[17][i]  = FloatToStr(ia_max_v);
		AdvStringGrid->Cells[18][i]  = FloatToStr(ib_min_v);
		AdvStringGrid->Cells[19][i]  = FloatToStr(ib_max_v);
		AdvStringGrid->Cells[20][i]  = FloatToStr(ic_min_v);
		AdvStringGrid->Cells[21][i]  = FloatToStr(ic_max_v);
		}
		if (AdvStringGrid->Cells[15][i] == "Утро")
		{
		AdvStringGrid->Cells[16][i]  = FloatToStr(ia_min_u);
		AdvStringGrid->Cells[17][i]  = FloatToStr(ia_max_u);
		AdvStringGrid->Cells[18][i]  = FloatToStr(ib_min_u);
		AdvStringGrid->Cells[19][i]  = FloatToStr(ib_max_u);
		AdvStringGrid->Cells[20][i]  = FloatToStr(ic_min_u);
		AdvStringGrid->Cells[21][i]  = FloatToStr(ic_max_u);

		}
   }
   */

   Chart->Axes->Left->Maximum = imax;
   int max = 0;
   //double ia1;
  // double ib1;
  // double ic1;
   //TKController *cont = FormAuto->Controllers->FindByPP(punkt);
  // TKArchiveValuesParam *param = cont->ArchiveValuesParam;
   UniQuery3->Close();
   UniQuery3->SQL->Clear();
   UniQuery3->SQL->Add("SELECT v_pp,v_pp_ia_max,v_pp_ib_max,v_pp_ic_max,v_pp_ua_max,v_pp_ub_max,v_pp_uc_max,");
   UniQuery3->SQL->Add("v_pp_ia_min,v_pp_ib_min,v_pp_ic_min,v_pp_ua_min,v_pp_ub_min,v_pp_uc_min");
   UniQuery3->SQL->Add("FROM j_srpav_pp ");
   UniQuery3->SQL->Add("WHERE v_pp=:a1");
   UniQuery3->Params->Items[0]->AsString = punkt;
   UniQuery3->Open();

   if (UniQuery3->FieldByName("v_pp_ia_max")->AsFloat>imax) imax = UniQuery3->FieldByName("v_pp_ia_max")->AsFloat;
   if (UniQuery3->FieldByName("v_pp_ib_max")->AsFloat>imax) imax = UniQuery3->FieldByName("v_pp_ib_max")->AsFloat;
   if (UniQuery3->FieldByName("v_pp_ic_max")->AsFloat>imax) imax = UniQuery3->FieldByName("v_pp_ic_max")->AsFloat;

   double dtAl = 0;
   for( i=0,UniQuery1->Open(); !UniQuery1->Eof;UniQuery1->Next())
   {
   if (dt != UniQuery1->FieldByName("arh_dt")->AsDateTime)
	   {
	   dt = UniQuery1->FieldByName("arh_dt")->AsDateTime;
	   double ia    = UniQuery1->FieldByName("arh_ia")->AsInteger;// >AsFloat;
	   double ib    = UniQuery1->FieldByName("arh_ib")->AsInteger;// >AsFloat;
	   double ic    = UniQuery1->FieldByName("arh_ic")->AsInteger;// >AsFloat;
	   double ua    = UniQuery1->FieldByName("arh_ua")->AsInteger;// >AsFloat;
	   double ub    = UniQuery1->FieldByName("arh_ub")->AsInteger;// >AsFloat;
	   double uc    = UniQuery1->FieldByName("arh_uc")->AsInteger;// >AsFloat;
	   double val   = UniQuery1->FieldByName("arh_link")->AsFloat;
	   if ((val == 1) && ((ia<0) || (ib<0) || (ic<0) || (ua<0) || (ub<0) || (uc<0)))
	   {
		 continue;
	   }
	   if ((val == 0) && ((ia>0) || (ib>0) || (ic>0) || (ua>0) || (ub>0) || (uc>0)))
	   {
		 continue;
	   }
		i++;
		//kza,kzb,kzc,pera,perb,perc,sima,simb,simc,door,guard,swith,contr;
	   AdvStringGrid->Cells[7][i] = FloatToStr(val);
	   AdvStringGrid->CellProperties[7][i]->FontColor = clSv;

// связь
	   if (val == 1 )  Series6->AddXY(dt,imax*0.5,"",clSv);
	   else    Series6->AddXY(dt,0,"",clSv);


	   Series8->AddNullXY(dt,imax*1.1,"");;
	  //		Series8->AddNullXY(dt,imax*1.1,dt.FormatString("hh:mm"));

// границы
	   Series20->AddXY(dt,UniQuery3->FieldByName("v_pp_ia_max")->AsFloat,"",clIa);
	   Series21->AddXY(dt,UniQuery3->FieldByName("v_pp_ib_max")->AsFloat,"",clIb);
	   Series22->AddXY(dt,UniQuery3->FieldByName("v_pp_ic_max")->AsFloat,"",clIc);
	   Series23->AddXY(dt,UniQuery3->FieldByName("v_pp_ua_max")->AsFloat,"",clUa);
	   Series24->AddXY(dt,UniQuery3->FieldByName("v_pp_ub_max")->AsFloat,"",clUb);
	   Series25->AddXY(dt,UniQuery3->FieldByName("v_pp_uc_max")->AsFloat,"",clUc);

	   Series26->AddXY(dt,UniQuery3->FieldByName("v_pp_ia_min")->AsFloat,"",clIa);
	   Series27->AddXY(dt,UniQuery3->FieldByName("v_pp_ib_min")->AsFloat,"",clIb);
	   Series28->AddXY(dt,UniQuery3->FieldByName("v_pp_ic_min")->AsFloat,"",clIc);
	   Series29->AddXY(dt,UniQuery3->FieldByName("v_pp_ua_min")->AsFloat,"",clUa);
	   Series30->AddXY(dt,UniQuery3->FieldByName("v_pp_ub_min")->AsFloat,"",clUb);
	   Series31->AddXY(dt,UniQuery3->FieldByName("v_pp_uc_min")->AsFloat,"",clUc);

// горение/негорение
	   if (Global1->CheckOfExistsColumn(Controllers->TableArchive,"arh_problem"))
	   {
		if (UniQuery1->FieldByName("arh_problem")->AsInteger != 0 )
		{
			Series32->AddXY(dt,imax*0.4,"",clGor);
		}
	   }

   }
   }

	Chart->LeftAxis->Maximum = imax*1.1;
// ширина столбца аварии
/*
   AdvStringGrid->ColWidths[8]  =  0;
   if ((max*10 < 60)  && (max != 0))
   {
	   AdvStringGrid->ColWidths[8]  =  60;
   }
   else
	   AdvStringGrid->ColWidths[8]  =  max*10;
*/
   WAl = AdvStringGrid->ColWidths[8];
//   WAl = 0;
AdvStringGrid->Width = AdvStringGrid->ColWidths[0] + AdvStringGrid->ColWidths[1] + AdvStringGrid->ColWidths[2] +
						   AdvStringGrid->ColWidths[3] + AdvStringGrid->ColWidths[4] + AdvStringGrid->ColWidths[5] +
						   AdvStringGrid->ColWidths[6] + AdvStringGrid->ColWidths[7] + AdvStringGrid->ColWidths[8] + 30;

 }
catch(...)
{
	Screen->Cursor = crDefault;
}

}

//---------------------------------------------------------------------------

void __fastcall TKGrafIU::GrafPaintSutki()
{
try
{
 // Проверяем соединение к ораклу и если нет восстанавливаем
/*   if( !Global1->Connection->Connected) Global1->Connection->PerformConnect(true);
	  TDateTime t_on, t_off;
	  t_on      = 0;
	  t_off     = 0;

// Формируем SQL запрос к базе
   Global1->Query->Close();
   Global1->Query->SQL->Clear();
   Global1->Query->SQL->Add("SELECT arh_dt FROM " + Controllers->TableArchive);
   Global1->Query->SQL->Add("WHERE arh_pp=:a1 AND arh_dt >= :a2 AND arh_dt < :a3");
   Global1->Query->SQL->Add("ORDER BY arh_dt");
   Global1->Query->Params->Items[0]->AsString = punkt;
   Global1->Query->Params->Items[1]->AsDateTime = DateOt;
   Global1->Query->Params->Items[2]->AsDateTime = DateDo;
   double imax = Chart->Axes->Left->Maximum;
   int i =0;
   UnicodeString dat_on = "";
   UnicodeString dat_off ="";
   UniQuery1->Connection = Global1->Query->Connection;
   TDateTime dt,dt1,dt3,data1,data2;
   UnicodeString dat, dat1;
   Global1->Query->Open();
   data1 = Global1->Query->FieldByName("arh_dt")->AsDateTime;
   dat = Global1->Query->FieldByName("arh_dt")->AsDateTime.TimeString();
   Global1->Query->Last();
   data2 = Global1->Query->FieldByName("arh_dt")->AsDateTime;
   double h = 0.00007;
   Global1->Query->Close();

   TryStrToDateTime(DateOt.DateString() + " 00:00:00",DateOt);
   TryStrToDateTime(DateDo.DateString() + " 00:00:00",DateDo);

   UniQuery1->Close();
   UniQuery1->SQL->Clear();
   UniQuery1->SQL->Add("SELECT v_calendar_date AS c_date,");
   UniQuery1->SQL->Add("v_vkl_bez_uchet_perehod AS t_on,v_vikl_bez_uchet_perehod AS t_off,");
   UniQuery1->SQL->Add("v_vkl_uchet_perehod AS t_on_s,v_vikl_uchet_perehod AS t_off_s");
   UniQuery1->SQL->Add(" FROM j_graf_chas_goren WHERE v_calendar_date >= :a0 AND v_calendar_date < :a1");
   UniQuery1->Params->Items[0]->AsDateTime = DateOt;
   UniQuery1->Params->Items[1]->AsDateTime = (DateOt+1);
   UniQuery1->Open();
   int j = UniQuery1->RecordCount;
   if( !UniQuery1->Eof )
   {
	t_on  = DateOt + 1 + UniQuery1->FieldByName("t_on")->AsDateTime - int(UniQuery1->FieldByName("t_on")->AsDateTime) - Shift;
	t_off = DateOt + 1 + UniQuery1->FieldByName("t_off")->AsDateTime - int(UniQuery1->FieldByName("t_off")->AsDateTime) + Shift;
	if ( t_on > t_off ) t_on = t_on - 1;
   }
   dat_on = t_on.TimeString();
   if (t_on.FormatString("hh") == "00")//dat_on == "0:00:00")
   {
       dat_on = "23:59:59";
   }
   dat_off = t_off.TimeString();
   if (dat_on.Length() < 8) dat_on = "0" + dat_on;
   if (dat_off.Length() < 8) dat_off = "0" + dat_off;
   dt = data1;
// заполнение таблицы значений, графика токов и суточной заливки
if (CodOnOff == "0333")
{
   while ( dt <= data2)
   {
		dat1 = dat;
	   dat = dt.TimeString();
	   if (dat.Length() < 8) dat = "0" + dat;
	   if ((dat >= night1) && (dat <= morning))
		{
			 // желтый   ночь
			 Series4->AddXY(dt,200,"",clEvening);
		}
		else
		{
		  if ((dat > morning) && (dat < dat_off))
		  {
				// желтый утро
				Series4->AddXY(dt,200,"",clEvening);
		  }
		  else
		  if ((dat > dat_on) && (dat < night2))
		  {
				// желтый  вечер
				Series4->AddXY(dt,200,"",clEvening);
		  }
		  else
		  if ((dat>"00:00:00") && (dat<night1 ))
		  {
			   // желтый  вечер
				Series4->AddXY(dt,200,"",clEvening);
		  }
		  else
		  {
				// белый  день
				Series4->AddXY(dt,0,"", clDay);
		  }
		}
		if ((dat1 < dat_off) && (dat >= dat_off) && (dat_off > morning) && (t_off <= data2))
		{
			Series4->AddXY(t_off,200,"",clEvening);
		}
		if ((dat1 < dat_on) && (dat >= dat_on) && (dat_on > night1) && (t_on >= data1))
		{
			Series4->AddXY(t_on,200,"",clEvening);
		}
		if ((dat1 < morning) && (dat > morning))
		{
			TryStrToDateTime((dt.FormatString("dd.mm.yyyy")+" "+morning),dt3);
			if (( dt3> data1)  && ( dt3 < data2))
			{
				Series4->AddXY(dt3,200,"",clEvening);
			}
		}
		if ((dat1 > night1) && (dat < morning))
		{
			TryStrToDateTime((dt.FormatString("dd.mm.yyyy")+" "+night1),dt3);
			if (( dt3> data1)  && ( dt3 < data2))
			{
				Series4->AddXY(dt3,200,"",clEvening);
			}
		}
	   Series5->AddXY(dt,0,"",Chart->Color);
		(dt > (DateOt+1))
	   {
		UniQuery1->Close();
		UniQuery1->SQL->Clear();
		UniQuery1->SQL->Add("SELECT v_calendar_date AS c_date,");
		UniQuery1->SQL->Add("v_vkl_bez_uchet_perehod AS t_on,v_vikl_bez_uchet_perehod AS t_off,");
		UniQuery1->SQL->Add("v_vkl_uchet_perehod AS t_on_s,v_vikl_uchet_perehod AS t_off_s");
		UniQuery1->SQL->Add(" FROM j_graf_chas_goren WHERE v_calendar_date >= :a0 AND v_calendar_date < :a1");
		UniQuery1->Params->Items[0]->AsDateTime = DateOt;
		UniQuery1->Params->Items[1]->AsDateTime = (DateOt+1);
		UniQuery1->Open();
		int j = UniQuery1->RecordCount;
		if( !UniQuery1->Eof )
		{
			t_on  = DateOt + 1 + UniQuery1->FieldByName("t_on")->AsDateTime - int(UniQuery1->FieldByName("t_on")->AsDateTime) - Shift;
			t_off = DateOt + 1 + UniQuery1->FieldByName("t_off")->AsDateTime - int(UniQuery1->FieldByName("t_off")->AsDateTime) + Shift;
			if( t_on > t_off )t_on = t_on - 1;
		}
		dat_on = t_on.TimeString();
		/*
		if (t_on.FormatString("hh") == "00")//dat_on == "0:00:00")
		{
			dat_on = "23:59:59";
		}
		* /
		dat_off = t_off.TimeString();
		if (dat_on.Length() < 8) dat_on = "0" + dat_on;
		if (dat_off.Length() < 8) dat_off = "0" + dat_off;
		DateOt = DateOt + 1;
	   }  *
	dt = dt + h;
   }
}
else
{
   while ( dt <= data2)
   {

	   dat1 = dat;
	   dat = dt.TimeString();
	   if (dat.Length() < 8) dat = "0" + dat;
	   if ((dat >= night1) && (dat <= morning))
		{
			 // темно желтый   ночь
			 Series4->AddXY(dt,200,"",clNight);
		}
		else
		{
		  if ((dat > morning) && (dat < dat_off))
		  {
				// желтый утро
				Series4->AddXY(dt,200,"",clEvening);
		  }
		  else
		   if ((dat > dat_on) && (dat < night2)) //|| ((dat < night1) && (night2 > t_on) && (dat > "00:00:00")))//if ((dat > dat_on) && (dat < night2))
		  {
				// желтый  вечер
				Series4->AddXY(dt,200,"",clEvening);
		  }
		  else
		  if ((dat>"00:00:00") && (dat<night1 ))
		  {
			   // желтый  вечер
				Series4->AddXY(dt,200,"",clEvening);
		  }
		  else
		  {
				// белый  день
				Series4->AddXY(dt,0,"", clDay);
		  }
		}
		if ((dat1 < dat_off) && (dat >= dat_off) && (dat_off > morning) && (t_off <= data2))
		{
			Series4->AddXY(t_off,200,"",clEvening);
		}
		if ((dat1 < dat_on) && (dat >= dat_on) && (dat_on > night1) && (t_on >= data1))
		{
			Series4->AddXY(t_on,200,"",clEvening);
		}
		if ((dat1 < morning) && (dat > morning))
		{
			TryStrToDateTime((dt.FormatString("dd.mm.yyyy")+" "+morning),dt3);
			if (( dt3> data1)  && ( dt3 < data2))
			{
				Series4->AddXY(dt3,200,"",clNight);
			}
		}
		if ((dat1 > night1) && (dat < morning))
		{
			TryStrToDateTime((dt.FormatString("dd.mm.yyyy")+" "+night1),dt3);
			if (( dt3> data1)  && ( dt3 < data2))
			{
				Series4->AddXY(dt3,200,"",clEvening);
			}
		}
	   Series5->AddXY(dt,0,"",Chart->Color);
	   if (dt > (DateOt+1))
	   {
		UniQuery1->Close();
		UniQuery1->SQL->Clear();
		UniQuery1->SQL->Add("SELECT v_calendar_date AS c_date,");
		UniQuery1->SQL->Add("v_vkl_bez_uchet_perehod AS t_on,v_vikl_bez_uchet_perehod AS t_off,");
		UniQuery1->SQL->Add("v_vkl_uchet_perehod AS t_on_s,v_vikl_uchet_perehod AS t_off_s");
		UniQuery1->SQL->Add(" FROM j_graf_chas_goren WHERE v_calendar_date >= :a0 AND v_calendar_date < :a1");
		UniQuery1->Params->Items[0]->AsDateTime = DateOt;
		UniQuery1->Params->Items[1]->AsDateTime = (DateOt+1);
		UniQuery1->Open();
		int j = UniQuery1->RecordCount;
		if( !UniQuery1->Eof )
		{
			t_on  = DateOt + 1 + UniQuery1->FieldByName("t_on")->AsDateTime - int(UniQuery1->FieldByName("t_on")->AsDateTime) - Shift;
			t_off = DateOt + 1 + UniQuery1->FieldByName("t_off")->AsDateTime - int(UniQuery1->FieldByName("t_off")->AsDateTime) + Shift;
			if( t_on > t_off )t_on = t_on - 1;
		}
		dat_on = t_on.TimeString();
		if (t_on.FormatString("hh") == "00")//dat_on == "0:00:00")
		{
			dat_on = "23:59:59";
		}
		dat_off = t_off.TimeString();
		if (dat_on.Length() < 8) dat_on = "0" + dat_on;
		if (dat_off.Length() < 8) dat_off = "0" + dat_off;
		DateOt = DateOt + 1;
	   }
	dt = dt + h;
   }
} */
}
catch(...)
{
	Screen->Cursor = crDefault;
}

}

//---------------------------------------------------------------------------

void __fastcall TKGrafIU::GrafPaintTochki()
{
try
{
	if( !Global1->Connection->Connected) Global1->Connection->PerformConnect(true);
	  TDateTime t_on, t_off;
	  t_on      = 0;
	  t_off     = 0;
// Формируем SQL запрос к базе
   Global1->Query->Close();
   Global1->Query->SQL->Clear();
   Global1->Query->SQL->Add("SELECT * FROM " + Controllers->TableArchive);
   Global1->Query->SQL->Add("WHERE arh_pp=:a1 AND arh_dt >= :a2 AND arh_dt < :a3");
   Global1->Query->SQL->Add("ORDER BY arh_dt");
   Global1->Query->Params->Items[0]->AsString = punkt;
   Global1->Query->Params->Items[1]->AsDateTime = DateOt;
   Global1->Query->Params->Items[2]->AsDateTime = DateDo;
   int i =0;
   UnicodeString dat_on = "";
   UnicodeString dat_off ="";
   TDateTime dt,dt1;
   UnicodeString dat, dat1;

   TDateTime data1,data2;
   Global1->Query->Open();
   dt1 = Global1->Query->FieldByName("arh_dt")->AsDateTime;
   data1 = Global1->Query->FieldByName("arh_dt")->AsDateTime;
   Global1->Query->Last();
   data2 = Global1->Query->FieldByName("arh_dt")->AsDateTime;
   Global1->Query->Close();

   UniQuery1->Connection = Global1->Query->Connection;

   int max = 0;
   double ia1;
   double ib1;
   double ic1;
   double imax = Chart->Axes->Left->Maximum;

   UniQuery2->Connection = Global1->Query->Connection;
   UniQuery3->Connection = Global1->Query->Connection;
   UniQuery2->Close();
   UniQuery2->SQL->Clear();
   UniQuery2->SQL->Add("SELECT o_pp, v_pp_on_off_phase, v_pp_on_off_time, v_pp_on_or_off, v_pp_on_off_mode");
   UniQuery2->SQL->Add("FROM j_pp_on_off, j_srpav_pp");
   UniQuery2->SQL->Add("WHERE o_pp = j_srpav_pp.id_object AND  j_srpav_pp.v_pp=:a1");
   UniQuery2->SQL->Add("AND v_pp_on_off_time >= :a2 AND v_pp_on_off_time < :a3 order by v_pp_on_off_time");
   UniQuery2->Params->Items[0]->AsString = punkt;
   UniQuery2->Params->Items[1]->AsDateTime = data1;//DateTimePicker1->DateTime;
   UniQuery2->Params->Items[2]->AsDateTime = data2;//DateTimePicker2->DateTime;
   UniQuery2->Open();

   UniQuery3->Close();
   UniQuery3->SQL->Clear();
   UniQuery3->SQL->Add("SELECT DISTINCT v_pp,o_pp,");
   UniQuery3->SQL->Add("v_date_time_fakt_otkl,");
   UniQuery3->SQL->Add("v_date_time_fakt_vkl");
   UniQuery3->SQL->Add("FROM j_el_param_pp,");
   UniQuery3->SQL->Add("j_srpav_pp," + Controllers->TableArchive);
   UniQuery3->SQL->Add("WHERE o_pp = j_srpav_pp.id_object");
   UniQuery3->SQL->Add("AND  j_srpav_pp.v_pp=:a1");
   UniQuery3->SQL->Add("AND arh_pp = j_srpav_pp.v_pp");
   UniQuery3->SQL->Add("AND v_date_time_fakt_otkl>=:a2");
   UniQuery3->SQL->Add("AND v_date_time_fakt_vkl<= :a3");
   UniQuery3->SQL->Add("order by  v_date_time_fakt_otkl,");
   UniQuery3->SQL->Add("v_date_time_fakt_vkl");
   UniQuery3->Params->Items[0]->AsString = punkt;
   UniQuery3->Params->Items[1]->AsDateTime = data1;//DateTimePicker1->DateTime;
   UniQuery3->Params->Items[2]->AsDateTime = data2;//DateTimePicker2->DateTime;
   UniQuery3->Open();
   int tck = 0;
// построение графика точек вкл/откл.
   for( i=1,Global1->Query->Open(); !Global1->Query->Eof;i++, Global1->Query->Next())
   {
   if (dt != Global1->Query->FieldByName("arh_dt")->AsDateTime)
	   {
	   dt = Global1->Query->FieldByName("arh_dt")->AsDateTime;
	   double ia = Global1->Query->FieldByName("arh_ia")->AsFloat;
	   double ib = Global1->Query->FieldByName("arh_ib")->AsFloat;
	   double ic = Global1->Query->FieldByName("arh_ic")->AsFloat;
	   while ((dt == UniQuery2->FieldByName("v_pp_on_off_time")->AsDateTime) && (! UniQuery2->Eof))
	   {
		    if (UniQuery2->FieldByName("v_pp_on_off_phase")->AsString == "A")
		   {
				if (UniQuery2->FieldByName("v_pp_on_or_off")->AsString == "откл")
				{
					Series9->AddBubble(UniQuery2->FieldByName("v_pp_on_off_time")->AsDateTime,ia,imax/100,"",clIa);
				}
				else
				{
					Series12->AddBubble(UniQuery2->FieldByName("v_pp_on_off_time")->AsDateTime,ia,imax/100,"",clIa);
				}
		   }
		   if (UniQuery2->FieldByName("v_pp_on_off_phase")->AsString == "B")
		   {
				if (UniQuery2->FieldByName("v_pp_on_or_off")->AsString == "откл")
				{
					Series10->AddBubble(UniQuery2->FieldByName("v_pp_on_off_time")->AsDateTime,ib,imax/100,"",clIb);
				}
				else
				{
					Series13->AddBubble(UniQuery2->FieldByName("v_pp_on_off_time")->AsDateTime,ib,imax/100,"",clIb);
				}
		   }
		   if (UniQuery2->FieldByName("v_pp_on_off_phase")->AsString == "C")
		   {
				if (UniQuery2->FieldByName("v_pp_on_or_off")->AsString == "откл")
				{
					Series11->AddBubble(UniQuery2->FieldByName("v_pp_on_off_time")->AsDateTime,ic,imax/100,"",clIc);
				}
				else
				{
					Series14->AddBubble(UniQuery2->FieldByName("v_pp_on_off_time")->AsDateTime,ic,imax/100,"",clIc);
				}
		   }
			UniQuery2->Next();
	   }
		while (! UniQuery3->Eof)
	   {
		   Series15->AddBubble(UniQuery3->FieldByName("v_date_time_fakt_otkl")->AsDateTime,imax,imax/100,"",clIc);
		   Series16->AddBubble(UniQuery3->FieldByName("v_date_time_fakt_vkl")->AsDateTime,imax,imax/100,"",clIc);
		  UniQuery3->Next();
       }
	   dt1 = Global1->Query->FieldByName("arh_dt")->AsDateTime;
	   ia1 = Global1->Query->FieldByName("arh_ia")->AsFloat;
	   ib1 = Global1->Query->FieldByName("arh_ib")->AsFloat;
	   ic1 = Global1->Query->FieldByName("arh_ic")->AsFloat;
	   }
	   else
	   {
		   i--;
	   }
   }
   }
catch(...)
{
}
}

//---------------------------------------------------------------------------

//DiscretGrafVisible = Graf_IA  | Graf_IB | Graf_IC | Graf_UA | Graf_UB | Graf_UC | Graf_AV | Graf_SV | Graf_GR
void __fastcall TKGrafIU::GrafVisible(int DiscretGrafVisible)
{
try
{
if (DiscretGrafVisible & Graf_IA )
{
   Series1->Visible = true;
   AdvStringGrid->ColWidths[1] = 35;
}
else
{
   Series1->Visible = false;
   AdvStringGrid->ColWidths[1] = 0;
}
if (DiscretGrafVisible & Graf_IB )
{
   Series2->Visible = true;
   AdvStringGrid->ColWidths[2] = 35;
}
else
{
   Series2->Visible = false;
   AdvStringGrid->ColWidths[2] = 0;
}
if (DiscretGrafVisible & Graf_IC )
{
   Series3->Visible = true;
   AdvStringGrid->ColWidths[3] = 35;
}
else
{
   Series3->Visible = false;
   AdvStringGrid->ColWidths[3] = 0;
}
if (DiscretGrafVisible & Graf_UA )
{
   Series17->Visible = true;
   AdvStringGrid->ColWidths[4] = 38;
}
else
{
   Series17->Visible = false;
   AdvStringGrid->ColWidths[4] = 0;
}
if (DiscretGrafVisible & Graf_UB )
{
   Series18->Visible = true;
   AdvStringGrid->ColWidths[5] = 38;
}
else
{
   Series18->Visible = false;
   AdvStringGrid->ColWidths[5] = 0;
}
if (DiscretGrafVisible & Graf_UC )
{
   Series19->Visible = true;
   AdvStringGrid->ColWidths[6] = 38;
}
else
{
   Series19->Visible = false;
   AdvStringGrid->ColWidths[6] = 0;
}

if (DiscretGrafVisible & Graf_AV )
{
   Series7->Visible = true;
   AdvStringGrid->ColWidths[8] = WAl;
   AdvStringGrid->ColWidths[8] = 55;
}
else
{
   Series7->Visible = false;
   AdvStringGrid->ColWidths[8] = 0;
}
if (DiscretGrafVisible & Graf_SV )
{
   Series6->Visible = true;
   AdvStringGrid->ColWidths[7] = 50;
}
else
{
   Series6->Visible = false;
   AdvStringGrid->ColWidths[7] = 0;
}
if (DiscretGrafVisible & Graf_GR )
  {
	 if (Series1->Visible)
	 {
		if (normbd)
		{
			Series36->Visible = true;
			Series37->Visible = true;
		}
		else
		{
			Series20->Visible = true;
			Series26->Visible = true;
		}
	 }
	 else
	 {
		Series20->Visible = false;
		Series26->Visible = false;
		Series36->Visible = false;
		Series37->Visible = false;
	 }
	 if (Series2->Visible)
	 {
		if (normbd)
		{
			Series38->Visible = true;
			Series39->Visible = true;
		}
		else
		{
			Series21->Visible = true;
			Series27->Visible = true;
		}
	 }
	 else
	 {
		Series21->Visible = false;
		Series27->Visible = false;
		Series38->Visible = false;
		Series39->Visible = false;
	 }
	 if (Series3->Visible)
	 {
		if (normbd)
		{
			Series40->Visible = true;
			Series41->Visible = true;
		}
		else
		{
			Series22->Visible = true;
			Series28->Visible = true;
		}

	 }
	 else
	 {
		Series22->Visible = false;
		Series28->Visible = false;
		Series40->Visible = false;
		Series41->Visible = false;
	 }
	 if (Series17->Visible)
	 {
		Series23->Visible = true;
		Series29->Visible = true;
	 }
	 else
	 {
		Series23->Visible = false;
		Series29->Visible = false;
	 }
	 if (Series18->Visible)
	 {
		Series24->Visible = true;
		Series30->Visible = true;
	 }
	 else
	 {
		Series24->Visible = false;
		Series30->Visible = false;
	 }
	 if (Series19->Visible)
	 {
		Series25->Visible = true;
		Series31->Visible = true;
	 }
	 else
	 {
		Series25->Visible = false;
		Series31->Visible = false;
	 }
  }
else
  {
	 Series20->Visible = false;
	 Series21->Visible = false;
	 Series22->Visible = false;
	 Series23->Visible = false;
	 Series24->Visible = false;
	 Series25->Visible = false;
	 Series26->Visible = false;
	 Series27->Visible = false;
	 Series28->Visible = false;
	 Series29->Visible = false;
	 Series30->Visible = false;
	 Series31->Visible = false;
	 Series36->Visible = false;
	 Series37->Visible = false;
	 Series38->Visible = false;
	 Series39->Visible = false;
	 Series40->Visible = false;
	 Series41->Visible = false;
  }
if (DiscretGrafVisible & Graf_TVO )
{

   if (Series1->Visible)
	 {
		Series9->Visible = true;
		Series12->Visible = true;
	 }
	 else
	 {
		Series9->Visible = false;
		Series12->Visible = false;
	 }
	 if (Series2->Visible)
	 {
		Series10->Visible = true;
		Series13->Visible = true;
	 }
	 else
	 {
		Series10->Visible = false;
		Series13->Visible = false;
	 }
	 if (Series3->Visible)
	 {
		Series11->Visible = true;
		Series14->Visible = true;
	 }
	 else
	 {
		Series11->Visible = false;
		Series14->Visible = false;
	 }
   /*
   Series9->Visible  = true;
   Series10->Visible = true;
   Series11->Visible = true;
   Series12->Visible = true;
   Series13->Visible = true;
   Series14->Visible = true;
   */
   // точки значений
   Series15->Visible = true;
   Series16->Visible = true;
}
else
{
   Series9->Visible  = false;
   Series10->Visible = false;
   Series11->Visible = false;
   Series12->Visible = false;
   Series13->Visible = false;
   Series14->Visible = false;
   // точки значений
   Series15->Visible = false;
   Series16->Visible = false;
}
if (DiscretGrafVisible & Graf_TZN )
{
   if (Series1->Visible)
	 {
		Series1->Pointer->Visible = true;
		Series1->Pointer->Size = 2;
	 }
	 else
	 {
		Series1->Pointer->Visible = false;
	 }
	 if (Series2->Visible)
	 {
		Series2->Pointer->Visible = true;
		Series2->Pointer->Size = 2;
	 }
	 else
	 {
		Series2->Pointer->Visible = false;
	 }
	 if (Series3->Visible)
	 {
		Series3->Pointer->Visible = true;
		Series3->Pointer->Size = 2;
	 }
	 else
	 {
		Series3->Pointer->Visible = false;
	 }

	 /*if (Series17->Visible)
	 {
		Series17->Pointer->Visible = true;
		Series17->Pointer->Size = 2;
	 }
	 else
	 {
		Series17->Pointer->Visible = false;
	 }
	 if (Series18->Visible)
	 {
		Series18->Pointer->Visible = true;
		Series18->Pointer->Size = 2;
	 }
	 else
	 {
		Series18->Pointer->Visible = false;
	 }
	 if (Series19->Visible)
	 {
		Series19->Pointer->Visible = true;
		Series19->Pointer->Size = 2;
	 }
	 else
	 {
		Series19->Pointer->Visible = false;
	 } */
}
else
{

   Series1->Pointer->Visible = false;
   Series2->Pointer->Visible = false;
   Series3->Pointer->Visible = false;
  /* Series17->Pointer->Visible = false;
   Series18->Pointer->Visible = false;
   Series19->Pointer->Visible = false;    */
}
if (DiscretGrafVisible & Graf_OnOff )
{
   Series32->Visible = true;
}
else
{
   Series32->Visible = false;
}
if (DiscretGrafVisible & Graf_AA )
{
 Series33->Visible = true;
}
else
{
   Series33->Visible = false;
}
if (DiscretGrafVisible & Graf_AB )
{
 Series34->Visible = true;
}
else
{
   Series34->Visible = false;
}
if (DiscretGrafVisible & Graf_AC )
{
 Series35->Visible = true;
}
else
{
   Series35->Visible = false;
}
}
catch(...)
{

}
}

//---------------------------------------------------------------------------

void __fastcall TKGrafIU::GrafToExcel()
{
try
{
Variant App,Books,Book,Sheets,Sheet,Cell;
	App=CreateOleObject("Excel.Application");
	App.OlePropertySet("Visible",true);
	Books=App.OlePropertyGet("Workbooks");// - набор рабочих книг.
	App.OlePropertySet("SheetsInNewWorkbook",1);// - задаем количество листов в книге
	Books.OleProcedure("Add");// добавляем созданную книгу.
	Book=Books.OlePropertyGet("Item",1).OlePropertyGet("Worksheets");// делаем текущей 1 книгу
	Sheet=Book.OlePropertyGet("Item",1);// - устанавливаем 1 лист в качестве текущего.
   //	Sheet.OlePropertySet("Name",L"ПП "+WideString(pp));// - тут можно изменить имя созданного листа.

	Cell=Sheet.OlePropertyGet("Cells").OlePropertyGet("Item",1,1);	// - выбираем ячейку,  например 1,1 - будет выбрана ячейка A1.
	Cell.OlePropertyGet("Font").OlePropertySet("Bold", true);      	// устанавливаем жирность шрифта
	Cell.OlePropertyGet("Font").OlePropertySet("Size", 14);
	Cell.OlePropertyGet("Font").OlePropertySet("Name", WideString("Times New Roman"));
	Cell.OlePropertyGet("Columns").OlePropertyGet("Item",1).OlePropertySet("ColumnWidth",18);  // увеличиваем ширину столбца
	Cell.OlePropertySet("Value",WideString("График токов ПП "+punkt));	// - записываем в выбранную ячейку данные
	Cell=Sheet.OlePropertyGet("Range",Sheet.OlePropertyGet("Cells",1,2),Sheet.OlePropertyGet("Cells",1,4));  // выделяем диапазон ячеек
	Cell.OlePropertyGet("Columns").OlePropertyGet("Item",1).OlePropertySet("ColumnWidth",9);  // увеличиваем ширину столбца
	Cell=Sheet.OlePropertyGet("Range",Sheet.OlePropertyGet("Cells",1,1),Sheet.OlePropertyGet("Cells",1,6));  // выделяем диапазон ячеек
	Cell.OleProcedure("Merge");    					// объединение ячеек
	Cell.OlePropertySet("HorizontalAlignment", 3);  // устанавливаем выравнивание по центру
	Cell.OlePropertySet("VerticalAlignment", 2);    // устанавливаем выравнивание по центру

	/*d1 = DateOt;
	d2 = DateDo;      */

	Cell=Sheet.OlePropertyGet("Cells").OlePropertyGet("Item",2,1);// - выбираем ячейку
	Cell.OlePropertyGet("Font").OlePropertySet("Bold", true);
	Cell.OlePropertyGet("Font").OlePropertySet("Size", 14);
	Cell.OlePropertyGet("Font").OlePropertySet("Name", WideString("Times New Roman"));
	Cell.OlePropertySet("Value",WideString("с "+DateOt.FormatString("dd.mm.yyyy  hh:nn")+" по "+DateDo.FormatString("dd.mm.yyyy  hh:nn")));// - записываем в выбранную ячейку данные
	Cell=Sheet.OlePropertyGet("Range",Sheet.OlePropertyGet("Cells",2,1),Sheet.OlePropertyGet("Cells",2,6));
	Cell.OleProcedure("Merge");
	Cell.OlePropertySet("HorizontalAlignment", 3);
	Cell.OlePropertySet("VerticalAlignment", 2);
	int k=0;
    for (int j = 0; j < AdvStringGrid->ColCount; j++)
	{
		if (AdvStringGrid->ColWidths[j] > 0)
		{
		   k++;
		   for (int i = 0; i < AdvStringGrid->RowCount; i++)
		   {
			Cell=Sheet.OlePropertyGet("Cells").OlePropertyGet("Item",i+4,k);// - выбираем ячейку
			Cell.OlePropertySet("Value",WideString(AdvStringGrid->Cells[j][i]));// - записываем в выбранную ячейку данные из ячейки StringGrid указывая для нее столбец и строку.
		  }
		}
   	}
	Cell=Sheet.OlePropertyGet("Range",Sheet.OlePropertyGet("Cells",4,1),Sheet.OlePropertyGet("Cells",AdvStringGrid->RowCount+3,k));
	Cell.OlePropertyGet("Font").OlePropertySet("Size", 12);
	Cell.OlePropertyGet("Font").OlePropertySet("Name", WideString("Times New Roman"));
	Cell.OlePropertyGet("Borders").OlePropertyGet("Item", 1).OlePropertySet("LineStyle", 1); // рисуем тонкую границу
	Cell.OlePropertyGet("Borders").OlePropertyGet("Item", 2).OlePropertySet("LineStyle", 1);
	Cell.OlePropertyGet("Borders").OlePropertyGet("Item", 3).OlePropertySet("LineStyle", 1);
	Cell.OlePropertyGet("Borders").OlePropertyGet("Item", 4).OlePropertySet("LineStyle", 1);

	Cell=Sheet.OlePropertyGet("Range",Sheet.OlePropertyGet("Cells",4,1),Sheet.OlePropertyGet("Cells",4,k));
	Cell.OlePropertyGet("Borders").OlePropertyGet("Item", 1).OlePropertySet("LineStyle", 7);  // рисуем жирную границу
	Cell.OlePropertyGet("Borders").OlePropertyGet("Item", 2).OlePropertySet("LineStyle", 7);
	Cell.OlePropertyGet("Borders").OlePropertyGet("Item", 3).OlePropertySet("LineStyle", 7);
	Cell.OlePropertyGet("Borders").OlePropertyGet("Item", 4).OlePropertySet("LineStyle", 7);

	UnicodeString nam;
	TDateTime df;
	nam = "\\ПП "+WideString(punkt)+" "+FormatDateTime("dd-mm-yy hh_nn_s",df.CurrentDateTime());
	Sheet.OleProcedure("SaveAs",WideString(Controllers->ReadProperty("Options","TempDir",GetCurrentDir())+nam));// - здесь переменная SaveFile типа AnsiString, которая содержит путь куда сохранить документ. Указывать расширение файла необязательно, тогда будет по-умолчанию (для 2003 это *.xls, 2007 - *.xlsx).
 // 	App.OleProcedure("Quit");
 }
catch(...)
{

}
}
//---------------------------------------------------------------------------

void __fastcall TKGrafIU::GrafPaintNedel()
{
try
{
	if( !Global1->Connection->Connected) Global1->Connection->PerformConnect(true);
	  TDateTime t_on, t_off;
	  t_on      = 0;
	  t_off     = 0;
   UniQuery3->Connection = Global1->Query->Connection;
   UniQuery3->Close();
   UniQuery3->SQL->Clear();
   UniQuery3->SQL->Add("SELECT DISTINCT v_pp,j_el_param_pp.*");
   UniQuery3->SQL->Add("FROM j_el_param_pp,");
   UniQuery3->SQL->Add("j_srpav_pp");
   UniQuery3->SQL->Add("WHERE o_pp = j_srpav_pp.id_object");
   UniQuery3->SQL->Add("AND  j_srpav_pp.v_pp=:a1");
   UniQuery3->SQL->Add("AND v_date_el_param>=:a2");
   UniQuery3->SQL->Add("AND v_date_el_param<=:a3");
   UniQuery3->SQL->Add("order by  v_date_el_param");

   UniQuery3->Params->Items[0]->AsString = punkt;
   UniQuery3->Params->Items[1]->AsDateTime = DateOt;//DateTimePicker1->DateTime;
   UniQuery3->Params->Items[2]->AsDateTime = DateDo;//DateTimePicker2->DateTime;
   UniQuery3->Open();
   int i =0;
   UnicodeString dat_on = "";
   UnicodeString dat_off ="";
   TDateTime dt,dt1;
   UnicodeString dat, dat1;

   TDateTime data1,data2;

   dt1 = UniQuery3->FieldByName("v_date_el_param")->AsDateTime;
   data1 = UniQuery3->FieldByName("v_date_el_param")->AsDateTime;
   UniQuery3->Last();
   data2 = UniQuery3->FieldByName("v_date_el_param")->AsDateTime;
   UniQuery3->Close();

   double ia1;
   double ib1;
   double ic1;

   int tck = 0;
// построение графика точек вкл/откл.
   for( i=1,UniQuery3->Open(); !UniQuery3->Eof;i++, UniQuery3->Next())
   {
   if (dt != UniQuery3->FieldByName("v_date_el_param")->AsDateTime)
	   {
	   dt = UniQuery3->FieldByName("v_date_el_param")->AsDateTime;
	   double ia = UniQuery3->FieldByName("v_ia")->AsFloat;
	   double ib = UniQuery3->FieldByName("v_ib")->AsFloat;
	   double ic = UniQuery3->FieldByName("v_ic")->AsFloat;
	   double ua = UniQuery3->FieldByName("v_ua")->AsFloat;
	   double ub = UniQuery3->FieldByName("v_ub")->AsFloat;
	   double uc = UniQuery3->FieldByName("v_uc")->AsFloat;
	   AdvStringGrid->RowCount = i+1;
	   AdvStringGrid->Cells[0][i] = dt.FormatString("dd.mm.yy");
	   AdvStringGrid->Cells[1][i] = FloatToStr(ia);
	   AdvStringGrid->Cells[2][i] = FloatToStr(ib);
	   AdvStringGrid->Cells[3][i] = FloatToStr(ic);
	   AdvStringGrid->Cells[4][i] = FloatToStr(ua);
	   AdvStringGrid->Cells[5][i] = FloatToStr(ub);
	   AdvStringGrid->Cells[6][i] = FloatToStr(uc);

	   AdvStringGrid->CellProperties[1][i]->FontColor = clIa;
	   AdvStringGrid->CellProperties[2][i]->FontColor = clIb;
	   AdvStringGrid->CellProperties[3][i]->FontColor = clIc;
	   AdvStringGrid->CellProperties[4][i]->FontColor = clUa;
	   AdvStringGrid->CellProperties[5][i]->FontColor = clUb;
	   AdvStringGrid->CellProperties[6][i]->FontColor = clUc;
	   //Series15->AddBubble(UniQuery3->FieldByName("v_date_time_fakt_otkl")->AsDateTime,imax,imax/100,"",clIc);
	   //Series16->AddBubble(UniQuery3->FieldByName("v_date_time_fakt_vkl")->AsDateTime,imax,imax/100,"",clIc);
	   Series1->AddXY(dt,ia,"",clIa);
	   Series2->AddXY(dt,ib,"",clIb);
	   Series3->AddXY(dt,ic,"",clIc);
	   Series17->AddXY(dt,ua,"",clUa);
	   Series18->AddXY(dt,ub,"",clUb);
	   Series19->AddXY(dt,uc,"",clUc);
	   }
	   else
	   {
		   i--;
	   }
   }
   }
catch(...)
{
}
}
//---------------------------------------------------------------------------
void __fastcall TKGrafIU::GrafPaintNorm(bool norm)
{
normbd = norm;
if (norm)
{
	Series20->Visible = false;           // границы тока а
	Series21->Visible = false;           // границы тока b
	Series22->Visible = false;           // границы тока c
	Series26->Visible = false;           // границы тока а
	Series27->Visible = false;           // границы тока b
	Series28->Visible = false;           // границы тока c

	if (Series1->Visible)
	 {
		Series36->Visible = true;
		Series37->Visible = true;
	 }
	 else
	 {

		Series36->Visible = false;
		Series37->Visible = false;
	 }
	 if (Series2->Visible)
	 {
		Series38->Visible = true;
		Series39->Visible = true;
	 }
	 else
	 {
		Series38->Visible = false;
		Series39->Visible = false;
	 }
	 if (Series3->Visible)
	 {
		Series40->Visible = true;
		Series41->Visible = true;
	 }
	 else
	 {
		Series40->Visible = false;
		Series41->Visible = false;
	 }

	Series36->Clear();           // границы тока а
	Series37->Clear();           // границы тока b
	Series38->Clear();           // границы тока c
	Series39->Clear();           // границы тока а
	Series40->Clear();           // границы тока b
	Series41->Clear();           // границы тока c

	AdvStringGrid->ColWidths[10] = 35;
	AdvStringGrid->ColWidths[11] = 35;
	AdvStringGrid->ColWidths[12] = 35;
	AdvStringGrid->ColWidths[13] = 35;
	AdvStringGrid->ColWidths[14] = 35;
	AdvStringGrid->ColWidths[15] = 35;

	AdvStringGrid->ColWidths[16] = 0;
	AdvStringGrid->ColWidths[17] = 0;
	AdvStringGrid->ColWidths[18] = 0;
	AdvStringGrid->ColWidths[19] = 0;
	AdvStringGrid->ColWidths[20] = 0;
	AdvStringGrid->ColWidths[21] = 0;

	TDateTime dt;
	for (int i = 1; i < AdvStringGrid->RowCount; i++)
	{
		TryStrToDateTime(AdvStringGrid->Cells[0][i],dt);
		Series36->AddXY(dt,StrToFloat(AdvStringGrid->Cells[10][i]),"",AdvStringGrid->CellProperties[10][i]->BrushColor);
		Series37->AddXY(dt,StrToFloat(AdvStringGrid->Cells[11][i]),"",clIa);
		Series38->AddXY(dt,StrToFloat(AdvStringGrid->Cells[12][i]),"",AdvStringGrid->CellProperties[12][i]->BrushColor);
		Series39->AddXY(dt,StrToFloat(AdvStringGrid->Cells[13][i]),"",clIb);
		Series40->AddXY(dt,StrToFloat(AdvStringGrid->Cells[14][i]),"",AdvStringGrid->CellProperties[14][i]->BrushColor);
		Series41->AddXY(dt,StrToFloat(AdvStringGrid->Cells[15][i]),"",clIc);
	}
}
else
{
	if (Series1->Visible)
	 {
		Series20->Visible = true;
		Series26->Visible = true;
	 }
	 else
	 {
		Series20->Visible = false;
		Series26->Visible = false;
	 }
	 if (Series2->Visible)
	 {
		Series21->Visible = true;
		Series27->Visible = true;
	 }
	 else
	 {
		Series21->Visible = false;
		Series27->Visible = false;

	 }
	 if (Series3->Visible)
	 {
		Series22->Visible = true;
		Series28->Visible = true;
	 }
	 else
	 {
		Series22->Visible = false;
		Series28->Visible = false;
	 }

	Series36->Visible = false;
	Series37->Visible = false;
	Series38->Visible = false;
	Series39->Visible = false;
	Series40->Visible = false;
	Series41->Visible = false;

	AdvStringGrid->ColWidths[10] = 0;
	AdvStringGrid->ColWidths[11] = 0;
	AdvStringGrid->ColWidths[12] = 0;
	AdvStringGrid->ColWidths[13] = 0;
	AdvStringGrid->ColWidths[14] = 0;
	AdvStringGrid->ColWidths[15] = 0;
}

}

//---------------------------------------------------------------------------
void __fastcall TKGrafIU::GrafCalcNorm(double delta, double procent)
{
	int nd = 0; // количество "День"
	int nn = 0; // количество "Ночь"
	int nv = 0; // количество "Вечер"
	int nu = 0; // количество "Утро"

	int ia_d = 0;   // сумма тока фазы а за день
	int ia_n = 0;   // сумма тока фазы а за ночь
	int ia_v = 0;   // сумма тока фазы а за вечер
	int ia_u = 0;   // сумма тока фазы а за утро

	int ib_d = 0;   // сумма тока фазы b за день
	int ib_n = 0;   // сумма тока фазы b за ночь
	int ib_v = 0;   // сумма тока фазы b за вечер
	int ib_u = 0;   // сумма тока фазы b за утро

	int ic_d = 0;   // сумма тока фазы c за день
	int ic_n = 0;   // сумма тока фазы c за ночь
	int ic_v = 0;   // сумма тока фазы c за вечер
	int ic_u = 0;   // сумма тока фазы c за утро

	int ua_d = 0;   // сумма напряжения фазы а за день
	int ua_n = 0;   // сумма напряжения фазы а за ночь
	int ua_v = 0;   // сумма напряжения фазы а за вечер
	int ua_u = 0;   // сумма напряжения фазы а за утро

	int ub_d = 0;   // сумма напряжения фазы b за день
	int ub_n = 0;   // сумма напряжения фазы b за ночь
	int ub_v = 0;   // сумма напряжения фазы b за вечер
	int ub_u = 0;   // сумма напряжения фазы b за утро

	int uc_d = 0;   // сумма напряжения фазы c за день
	int uc_n = 0;   // сумма напряжения фазы c за ночь
	int uc_v = 0;   // сумма напряжения фазы c за вечер
	int uc_u = 0;   // сумма напряжения фазы c за утро

	TDateTime dt;
	UnicodeString modd;

	for (int i = 1; i < AdvStringGrid->RowCount; i++)
	{
	   TryStrToDateTime(AdvStringGrid->Cells[0][i],dt);
	   modd = AdvStringGrid->Cells[9][i];
		  if (modd == "День")
		  {
			nd++;
			ia_d = ia_d + StrToInt(AdvStringGrid->Cells[1][i]);
			ib_d = ib_d + StrToInt(AdvStringGrid->Cells[2][i]);
			ic_d = ic_d + StrToInt(AdvStringGrid->Cells[3][i]);
			ua_d = ua_d + StrToInt(AdvStringGrid->Cells[4][i]);
			ub_d = ub_d + StrToInt(AdvStringGrid->Cells[5][i]);
			uc_d = uc_d + StrToInt(AdvStringGrid->Cells[6][i]);
		  }
		  if (modd == "Ночь")
		  {
			nn++;
			ia_n = ia_n + StrToInt(AdvStringGrid->Cells[1][i]);
			ib_n = ib_n + StrToInt(AdvStringGrid->Cells[2][i]);
			ic_n = ic_n + StrToInt(AdvStringGrid->Cells[3][i]);
			ua_n = ua_n + StrToInt(AdvStringGrid->Cells[4][i]);
			ub_n = ub_n + StrToInt(AdvStringGrid->Cells[5][i]);
			uc_n = uc_n + StrToInt(AdvStringGrid->Cells[6][i]);
		  }
		  if (modd == "Утро")
		  {
			nu++;
			ia_u = ia_u + StrToInt(AdvStringGrid->Cells[1][i]);
			ib_u = ib_u + StrToInt(AdvStringGrid->Cells[2][i]);
			ic_u = ic_u + StrToInt(AdvStringGrid->Cells[3][i]);
			ua_u = ua_u + StrToInt(AdvStringGrid->Cells[4][i]);
			ub_u = ub_u + StrToInt(AdvStringGrid->Cells[5][i]);
			uc_u = uc_u + StrToInt(AdvStringGrid->Cells[6][i]);
		  }
		  if (modd == "Вечер")
		  {
			nv++;
			ia_v = ia_v + StrToInt(AdvStringGrid->Cells[1][i]);
			ib_v = ib_v + StrToInt(AdvStringGrid->Cells[2][i]);
			ic_v = ic_v + StrToInt(AdvStringGrid->Cells[3][i]);
			ua_v = ua_v + StrToInt(AdvStringGrid->Cells[4][i]);
			ub_v = ub_v + StrToInt(AdvStringGrid->Cells[5][i]);
			uc_v = uc_v + StrToInt(AdvStringGrid->Cells[6][i]);
		  }
	}
	double del; // дельта

   // min и max по всем фазам за день
   if (nd != 0)
   {
	  del = ia_d/nd*procent;
	  ia_nom_d = ia_d/nd;
	  ua_nom_d = ua_d/nd;
	  if (del < delta)
	  {
		ia_max_d = ia_d/nd + delta;
		ia_min_d = ia_d/nd - delta;
		if (ia_min_d < 0) ia_min_d = 0;
	  }
	  else
	  {
		ia_max_d = ia_d/nd + del;
		ia_min_d = ia_d/nd - del;
		if (ia_min_d < 0) ia_min_d = 0;
	  }

	  del = ib_d/nd*procent;
	  ib_nom_d = ib_d/nd;
	  ub_nom_d = ub_d/nd;
	  if (del < delta)
	  {
		ib_max_d = ib_d/nd + delta;
		ib_min_d = ib_d/nd - delta;
		if (ib_min_d < 0) ib_min_d = 0;
	  }
	  else
	  {
		ib_max_d = ib_d/nd + del;
		ib_min_d = ib_d/nd - del;
		if (ib_min_d < 0) ib_min_d = 0;
	  }

	  del = ic_d/nd*procent;
	  ic_nom_d = ic_d/nd;
	  uc_nom_d = uc_d/nd;
	  if (del < delta)
	  {
		ic_max_d = ic_d/nd + delta;
		ic_min_d = ic_d/nd - delta;
		if (ic_min_d < 0) ic_min_d = 0;
	  }
	  else
	  {
		ic_max_d = ic_d/nd + del;
		ic_min_d = ic_d/nd - del;
		if (ic_min_d < 0) ic_min_d = 0;
	  }
   }
   else
   {
	  ia_max_d = 0;
	  ia_min_d = 0;
	  ib_max_d = 0;
	  ib_min_d = 0;
	  ic_max_d = 0;
	  ic_min_d = 0;
	  ia_nom_d = 0;
	  ib_nom_d = 0;
	  ic_nom_d = 0;
	  ua_nom_d = 0;
	  ub_nom_d = 0;
	  uc_nom_d = 0;
   }

   // min и max по всем фазам за ночь
   if (nn != 0)
   {
	  del = ia_n/nn*procent;
	  ia_nom_n = ia_n/nn;
	  ua_nom_n = ua_n/nn;
	  if (del < delta)
	  {
		ia_max_n = ia_n/nn + delta;
		ia_min_n = ia_n/nn - delta;
		if (ia_min_n < 0) ia_min_n = 0;
	  }
	  else
	  {
		ia_max_n = ia_n/nn + del;
		ia_min_n = ia_n/nn - del;
		if (ia_min_n < 0) ia_min_n = 0;
	  }

	  del = ib_n/nn*procent;
	  ib_nom_n = ib_n/nn;
	  ub_nom_n = ub_n/nn;
	  if (del < delta)
	  {
		ib_max_n = ib_n/nn + delta;
		ib_min_n = ib_n/nn - delta;
		if (ib_min_n < 0) ib_min_n = 0;
	  }
	  else
	  {
		ib_max_n = ib_n/nn + del;
		ib_min_n = ib_n/nn - del;
		if (ib_min_n < 0) ib_min_n = 0;
	  }

	  del = ic_n/nn*procent;
	  ic_nom_n = ic_n/nn;
	  uc_nom_n = uc_n/nn;
	  if (del < delta)
	  {
		ic_max_n = ic_n/nn + delta;
		ic_min_n = ic_n/nn - delta;
		if (ic_min_n < 0) ic_min_n = 0;
	  }
	  else
	  {
		ic_max_n = ic_n/nn + del;
		ic_min_n = ic_n/nn - del;
		if (ic_min_n < 0) ic_min_n = 0;
	  }
   }
   else
   {
	  ia_max_n = 0;
	  ia_min_n = 0;
	  ib_max_n = 0;
	  ib_min_n = 0;
	  ic_max_n = 0;
	  ic_min_n = 0;
	  ia_nom_n = 0;
	  ib_nom_n = 0;
	  ic_nom_n = 0;
	  ua_nom_n = 0;
	  ub_nom_n = 0;
	  uc_nom_n = 0;
   }

   // min и max по всем фазам за вечер
   if (nv != 0)
   {
	  del = ia_v/nv*procent;
	  ia_nom_v = ia_v/nv;
	  ua_nom_v = ua_v/nv;
	  if (del < delta)
	  {
		ia_max_v = ia_v/nv + delta;
		ia_min_v = ia_v/nv - delta;
		if (ia_min_v < 0) ia_min_v = 0;
	  }
	  else
	  {
		ia_max_v = ia_v/nv + del;
		ia_min_v = ia_v/nv - del;
		if (ia_min_v < 0) ia_min_v = 0;
	  }

	  del = ib_v/nv*procent;
	  ib_nom_v = ib_v/nv;
	  ub_nom_v = ub_v/nv;
	  if (del < delta)
	  {
		ib_max_v = ib_v/nv + delta;
		ib_min_v = ib_v/nv - delta;
		if (ib_min_v < 0) ib_min_v = 0;
	  }
	  else
	  {
		ib_max_v = ib_v/nv + del;
		ib_min_v = ib_v/nv - del;
		if (ib_min_v < 0) ib_min_v = 0;
	  }

	  del = ic_v/nv*procent;
	  ic_nom_v = ic_v/nv;
	  uc_nom_v = uc_v/nv;
	  if (del < delta)
	  {
		ic_max_v = ic_v/nv + delta;
		ic_min_v = ic_v/nv - delta;
		if (ic_min_v < 0) ic_min_v = 0;
	  }
	  else
	  {
		ic_max_v = ic_v/nv + del;
		ic_min_v = ic_v/nv - del;
		if (ic_min_v < 0) ic_min_v = 0;
	  }
   }
   else
   {
	  ia_max_v = 0;
	  ia_min_v = 0;
	  ib_max_v = 0;
	  ib_min_v = 0;
	  ic_max_v = 0;
	  ic_min_v = 0;
	  ia_nom_v = 0;
	  ib_nom_v = 0;
	  ic_nom_v = 0;
	  ua_nom_v = 0;
	  ub_nom_v = 0;
	  uc_nom_v = 0;
   }

   // min и max по всем фазам за утро
   if (nu != 0)
   {
	  del = ia_u/nu*procent;
	  ia_nom_u = ia_u/nu;
	  ua_nom_u = ua_u/nu;
	  if (del < delta)
	  {
		ia_max_u = ia_u/nu + delta;
		ia_min_u = ia_u/nu - delta;
		if (ia_min_u < 0) ia_min_u = 0;
	  }
	  else
	  {
		ia_max_u = ia_u/nu + del;
		ia_min_u = ia_u/nu - del;
		if (ia_min_u < 0) ia_min_u = 0;
	  }

	  del = ib_u/nu*procent;
	  ib_nom_u = ib_u/nu;
	  ub_nom_u = ub_u/nu;
	  if (del < delta)
	  {
		ib_max_u = ib_u/nu + delta;
		ib_min_u = ib_u/nu - delta;
		if (ib_min_u < 0) ib_min_u = 0;
	  }
	  else
	  {
		ib_max_u = ib_u/nu + del;
		ib_min_u = ib_u/nu - del;
		if (ib_min_u < 0) ib_min_u = 0;
	  }

	  del = ic_u/nu*procent;
	  ic_nom_u = ic_u/nu;
	  uc_nom_u = uc_u/nu;
	  if (del < delta)
	  {
		ic_max_u = ic_u/nu + delta;
		ic_min_u = ic_u/nu - delta;
		if (ic_min_u < 0) ic_min_u = 0;
	  }
	  else
	  {
		ic_max_u = ic_u/nu + del;
		ic_min_u = ic_u/nu - del;
		if (ic_min_u < 0) ic_min_u = 0;
	  }
   }
   else
   {
	  ia_max_u = 0;
	  ia_min_u = 0;
	  ib_max_u = 0;
	  ib_min_u = 0;
	  ic_max_u = 0;
	  ic_min_u = 0;
	  ia_nom_u = 0;
	  ib_nom_u = 0;
	  ic_nom_u = 0;
	  ua_nom_u = 0;
	  ub_nom_u = 0;
	  uc_nom_u = 0;
   }

   Series36->Clear();           // границы тока а
   Series37->Clear();           // границы тока b
   Series38->Clear();           // границы тока c
   Series39->Clear();           // границы тока а
   Series40->Clear();           // границы тока b
   Series41->Clear();

   for (int i = 1; i < AdvStringGrid->RowCount; i++)
   {
		if (AdvStringGrid->Cells[9][i] == "День")
		{
		AdvStringGrid->Cells[16][i]  = FloatToStr(ia_min_d);
		AdvStringGrid->Cells[17][i]  = FloatToStr(ia_max_d);
		AdvStringGrid->Cells[18][i]  = FloatToStr(ib_min_d);
		AdvStringGrid->Cells[19][i]  = FloatToStr(ib_max_d);
		AdvStringGrid->Cells[20][i]  = FloatToStr(ic_min_d);
		AdvStringGrid->Cells[21][i]  = FloatToStr(ic_max_d);
		}
		if (AdvStringGrid->Cells[9][i] == "Ночь")
		{
		AdvStringGrid->Cells[16][i]  = FloatToStr(ia_min_n);
		AdvStringGrid->Cells[17][i]  = FloatToStr(ia_max_n);
		AdvStringGrid->Cells[18][i]  = FloatToStr(ib_min_n);
		AdvStringGrid->Cells[19][i]  = FloatToStr(ib_max_n);
		AdvStringGrid->Cells[20][i]  = FloatToStr(ic_min_n);
		AdvStringGrid->Cells[21][i]  = FloatToStr(ic_max_n);
		}
		if (AdvStringGrid->Cells[9][i] == "Вечер")
		{
		AdvStringGrid->Cells[16][i]  = FloatToStr(ia_min_v);
		AdvStringGrid->Cells[17][i]  = FloatToStr(ia_max_v);
		AdvStringGrid->Cells[18][i]  = FloatToStr(ib_min_v);
		AdvStringGrid->Cells[19][i]  = FloatToStr(ib_max_v);
		AdvStringGrid->Cells[20][i]  = FloatToStr(ic_min_v);
		AdvStringGrid->Cells[21][i]  = FloatToStr(ic_max_v);
		}
		if (AdvStringGrid->Cells[9][i] == "Утро")
		{
		AdvStringGrid->Cells[16][i]  = FloatToStr(ia_min_u);
		AdvStringGrid->Cells[17][i]  = FloatToStr(ia_max_u);
		AdvStringGrid->Cells[18][i]  = FloatToStr(ib_min_u);
		AdvStringGrid->Cells[19][i]  = FloatToStr(ib_max_u);
		AdvStringGrid->Cells[20][i]  = FloatToStr(ic_min_u);
		AdvStringGrid->Cells[21][i]  = FloatToStr(ic_max_u);
		}


		TryStrToDateTime(AdvStringGrid->Cells[0][i],dt);
		Series36->AddXY(dt,StrToFloat(AdvStringGrid->Cells[16][i]),"",clIa);
		Series37->AddXY(dt,StrToFloat(AdvStringGrid->Cells[17][i]),"",clIa);
		Series38->AddXY(dt,StrToFloat(AdvStringGrid->Cells[18][i]),"",clIb);
		Series39->AddXY(dt,StrToFloat(AdvStringGrid->Cells[19][i]),"",clIb);
		Series40->AddXY(dt,StrToFloat(AdvStringGrid->Cells[20][i]),"",clIc);
		Series41->AddXY(dt,StrToFloat(AdvStringGrid->Cells[21][i]),"",clIc);
   }

	AdvStringGrid->ColWidths[10] = 0;
	AdvStringGrid->ColWidths[11] = 0;
	AdvStringGrid->ColWidths[12] = 0;
	AdvStringGrid->ColWidths[13] = 0;
	AdvStringGrid->ColWidths[14] = 0;
	AdvStringGrid->ColWidths[15] = 0;

	AdvStringGrid->ColWidths[16] = 35;
	AdvStringGrid->ColWidths[17] = 35;
	AdvStringGrid->ColWidths[18] = 35;
	AdvStringGrid->ColWidths[19] = 35;
	AdvStringGrid->ColWidths[20] = 35;
	AdvStringGrid->ColWidths[21] = 35;

}

//---------------------------------------------------------------------------
void __fastcall TKGrafIU::GrafSaveNorm(TDateTime DateSave)
{
Journal = new  TKJournalSem("j_norm_param_pp",Global1) ;
	UniQuery1->Close();
	UniQuery1->SQL->Clear();
	UniQuery1->SQL->Add("SELECT id_object FROM j_srpav_pp where v_pp =:a1");
    UniQuery1->Params->Items[0]->AsString = punkt;
	UniQuery1->Open();
	UnicodeString o_pp = UniQuery1->FieldByName("id_object")->AsString;
	UniQuery1->Close();
Journal->QueryClear();
Journal->AddParamQuery("pp",o_pp);
Journal->AddParamQuery("Date_norm_param_pp",DateSave);
Journal->AddParamQuery("Mode_norm_param_pp","День");
Journal->AddParamQuery("Ia_nom_norm_param_pp",ia_nom_d);
Journal->AddParamQuery("Ua_nom_norm_param_pp",ua_nom_d);
Journal->AddParamQuery("Ia_max_norm_param_pp",ia_max_d);
Journal->AddParamQuery("Ia_min_norm_param_pp",ia_min_d);
Journal->AddParamQuery("Ib_nom_norm_param_pp",ib_nom_d);
Journal->AddParamQuery("Ub_nom_norm_param_pp",ub_nom_d);
Journal->AddParamQuery("Ib_max_norm_param_pp",ib_max_d);
Journal->AddParamQuery("Ib_min_norm_param_pp",ib_min_d);
Journal->AddParamQuery("Ic_nom_norm_param_pp",ic_nom_d);
Journal->AddParamQuery("Uc_nom_norm_param_pp",uc_nom_d);
Journal->AddParamQuery("Ic_max_norm_param_pp",ic_max_d);
Journal->AddParamQuery("Ic_min_norm_param_pp",ic_min_d);
Journal->QueryInsert();

Journal->QueryClear();
Journal->AddParamQuery("pp",o_pp);
Journal->AddParamQuery("Date_norm_param_pp",DateSave);
Journal->AddParamQuery("Mode_norm_param_pp","Ночь");
Journal->AddParamQuery("Ia_nom_norm_param_pp",ia_nom_n);
Journal->AddParamQuery("Ua_nom_norm_param_pp",ua_nom_n);
Journal->AddParamQuery("Ia_max_norm_param_pp",ia_max_n);
Journal->AddParamQuery("Ia_min_norm_param_pp",ia_min_n);
Journal->AddParamQuery("Ib_nom_norm_param_pp",ib_nom_n);
Journal->AddParamQuery("Ub_nom_norm_param_pp",ub_nom_n);
Journal->AddParamQuery("Ib_max_norm_param_pp",ib_max_n);
Journal->AddParamQuery("Ib_min_norm_param_pp",ib_min_n);
Journal->AddParamQuery("Ic_nom_norm_param_pp",ic_nom_n);
Journal->AddParamQuery("Uc_nom_norm_param_pp",uc_nom_n);
Journal->AddParamQuery("Ic_max_norm_param_pp",ic_max_n);
Journal->AddParamQuery("Ic_min_norm_param_pp",ic_min_n);
Journal->QueryInsert();

Journal->QueryClear();
Journal->AddParamQuery("pp",o_pp);
Journal->AddParamQuery("Date_norm_param_pp",DateSave);
Journal->AddParamQuery("Mode_norm_param_pp","Утро");
Journal->AddParamQuery("Ia_nom_norm_param_pp",ia_nom_u);
Journal->AddParamQuery("Ua_nom_norm_param_pp",ua_nom_u);
Journal->AddParamQuery("Ia_max_norm_param_pp",ia_max_u);
Journal->AddParamQuery("Ia_min_norm_param_pp",ia_min_u);
Journal->AddParamQuery("Ib_nom_norm_param_pp",ib_nom_u);
Journal->AddParamQuery("Ub_nom_norm_param_pp",ub_nom_u);
Journal->AddParamQuery("Ib_max_norm_param_pp",ib_max_u);
Journal->AddParamQuery("Ib_min_norm_param_pp",ib_min_u);
Journal->AddParamQuery("Ic_nom_norm_param_pp",ic_nom_u);
Journal->AddParamQuery("Uc_nom_norm_param_pp",uc_nom_u);
Journal->AddParamQuery("Ic_max_norm_param_pp",ic_max_u);
Journal->AddParamQuery("Ic_min_norm_param_pp",ic_min_u);
Journal->QueryInsert();

Journal->QueryClear();
Journal->AddParamQuery("pp",o_pp);
Journal->AddParamQuery("Date_norm_param_pp",DateSave);
Journal->AddParamQuery("Mode_norm_param_pp","Вечер");
Journal->AddParamQuery("Ia_nom_norm_param_pp",ia_nom_v);
Journal->AddParamQuery("Ua_nom_norm_param_pp",ua_nom_v);
Journal->AddParamQuery("Ia_max_norm_param_pp",ia_max_v);
Journal->AddParamQuery("Ia_min_norm_param_pp",ia_min_v);
Journal->AddParamQuery("Ib_nom_norm_param_pp",ib_nom_v);
Journal->AddParamQuery("Ub_nom_norm_param_pp",ub_nom_v);
Journal->AddParamQuery("Ib_max_norm_param_pp",ib_max_v);
Journal->AddParamQuery("Ib_min_norm_param_pp",ib_min_v);
Journal->AddParamQuery("Ic_nom_norm_param_pp",ic_nom_v);
Journal->AddParamQuery("Uc_nom_norm_param_pp",uc_nom_v);
Journal->AddParamQuery("Ic_max_norm_param_pp",ic_max_v);
Journal->AddParamQuery("Ic_min_norm_param_pp",ic_min_v);
Journal->QueryInsert();
Journal->QueryClear();

for (int i = 1; i < AdvStringGrid->RowCount; i++)
   {
		AdvStringGrid->Cells[10][i] = AdvStringGrid->Cells[16][i];
		AdvStringGrid->Cells[11][i] = AdvStringGrid->Cells[17][i];
		AdvStringGrid->Cells[12][i] = AdvStringGrid->Cells[18][i];
		AdvStringGrid->Cells[13][i] = AdvStringGrid->Cells[19][i];
		AdvStringGrid->Cells[14][i] = AdvStringGrid->Cells[20][i];
		AdvStringGrid->Cells[15][i] = AdvStringGrid->Cells[21][i];
   }
}

//---------------------------------------------------------------------------
