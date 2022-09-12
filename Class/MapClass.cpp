//---------------------------------------------------------------------------

#pragma hdrstop

#include "MapClass.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

/*****************************************************************************
* Класс TKMapObject, TKPMapObjectItem т TKMapObjectList
*****************************************************************************/
/**
* Конструктор TKMapObject
*/
__fastcall TKMapObject::TKMapObject(UnicodeString id,double lat,double lon,TMapObj *mo)
	  :TObject()
{
	MapObject = mo;
	Id        = id;
	Latitude  = lat;
	Longitude = lon;
	Name      = "";
	Stat      = "";
	Stat1     = -1;
	Stat2     = -1;
	Stat3     = -1;
	CreateDT  = CreateDT.CurrentDateTime();
	UpdateDT  = CreateDT;
	CheckDT   = CreateDT;
	Label1    = NULL;
	Label2    = NULL;
	Label3    = NULL;

}



/**
* Конструктор TKMapObjectItem
*/
__fastcall TKMapObjectItem::TKMapObjectItem(TCollection *Collection)
   : TCollectionItem(Collection), FValue(0) { }

/**
* Конструктор TKMapObjectList
*/
__fastcall TKMapObjectList::TKMapObjectList(TMapView *map,int pan_obj, int pan_sem)
	: TCollection(__classid(TKMapObjectItem))
{
	Log                   = NULL;
	MapView 			  = map;
//	Type                  = type;
	PanoramaObjCode       = pan_obj;
	PanoramaSemanticCode  = pan_sem;
	MapObj  = new TMapObj(map);
	MapObj->MapView       = map;
	MapFind = new TMapFind(map);
	MapFind->MapView      = map;
	MapFind->MapObj       = MapObj;
}

/**
* Добавить элемент в список
*/
void __fastcall TKMapObjectList::Add(TKMapObject *AValue)
{
	TKMapObjectItem *Item = (TKMapObjectItem *) TCollection::Add();
	Item->Value = AValue;
}

/**
* Удалить элемент из списка
*/
void __fastcall TKMapObjectList::Delete(int Index)
{
	if( Index < 0 && Index >= TCollection::Count )return;
	delete Items[Index];
	TCollection::Delete(Index);
}

/**
* Получить элемент из списка
*/
TKMapObject * __fastcall TKMapObjectList::GetItem(int Index)
{
	TKMapObjectItem *Item = (TKMapObjectItem *) TCollection::GetItem(Index);
	return Item->Value;
}

/*
* Сохранить элемент в список
*/
void __fastcall TKMapObjectList::SetItem(int Index, TKMapObject * AValue)
{
	TKMapObjectItem *Item = (TKMapObjectItem *) TCollection::GetItem(Index);
	Item->Value = AValue;
}

/**
* Создать все объекты по карте
*/
void __fastcall TKMapObjectList::CreateAllByMap(int sem,UnicodeString val )
{
	try
	{
// Задаем поиск объектов
		 MapFind->Active                                     = false;
		 MapFind->FindPoint                                  = false;
		 MapFind->MapSelect->Clear();
		 MapFind->MapSelect->Excode[-1][0]                   = false;
		 MapFind->MapSelect->Excode[PanoramaObjCode][OL_MARK]= true;

		 if( Log )Log->StartBar(0,2);
		 if( Log )Log->SetBar(1);
		 for(MapFind->Active = true; !MapFind->Eof; MapFind->Next())
		 {
// Находим PK содержащийся в семантике
			 UnicodeString s_id   = MapFind->MapObj->Semantic->SemanticValue[MapFind->MapObj->Semantic->FindByCode(PanoramaSemanticCode)];
// Находим кординаты и переводим их в WGS-84
			 TCompMapPoint *point = MapFind->MapObj->Metric->Points[0][1];
			 point->PlaceOut = PP_GEO;
			 double X,Y;
			 point->GetPoint(X,Y);
			 Y        = Y/M_PI*180;
			 X        = X/M_PI*180;
// Создаем объект панорамы
			 TMapObj *mo   = new TMapObj( MapView );
			 mo->MapView   = MapView;
			 MapView->ObjectByKey(0,MapView->MapLists->MapLists[1]->ListName,MapFind->MapObj->Key ,mo);
// Создаем объект TKMapObject
			 TKMapObject *tmo = new TKMapObject(s_id,Y,X,mo);
			 Add(tmo);
			 if( sem >= 0 && val!= "" )
			 {
				mo->Semantic->SemanticValue[mo->Semantic->FindByCode(sem)] = val;
				mo->Commit();

             }
			 if(Log)Log->Message("Найден объект "+tmo->Id+" "+
				 FloatToStr(tmo->Latitude)+" "+FloatToStr(tmo->Longitude));
			 Application->ProcessMessages();
		 }//end for
		 MapView->Refresh();
		 if(Log)Log->StopBar();
		 CreateAllLabelByMap();
	}
	catch(Exception &e)
	{
		if( Log!=NULL)Log->Error("Ошибка создания списка по карте "+e.Message);
    }
}

/**
* Создать метки у всех объектов
*/
void __fastcall TKMapObjectList::CreateAllLabelByMap()
{
	try
	{
// Перебиреам список объектов
		if( Log )Log->StartBar(0,Count);
		for( int i=0; i<Count; i++)
		{
			TKMapObject *tmo = Items[i];
			MapFind->Active                                     = false;
			MapFind->FindPoint                                  = false;
			MapFind->MapSelect->Clear();
		    MapFind->MapSelect->AddSemantic(PanoramaSemanticCode,tmo->Id,SC_EQUAL);
			int j=0;
			for(MapFind->Active = true; !MapFind->Eof; MapFind->Next())
			{
				if( MapFind->MapObj->ExCode == PanoramaObjCode )continue;
				if( j == 0 && tmo->Label1 == NULL )
				{
					tmo->Label1 = new TMapObj(MapView);
					tmo->Label1->MapView = MapView;
					MapView->ObjectByKey(0,MapView->MapLists->MapLists[1]->ListName,MapFind->MapObj->Key ,
					   tmo->Label1);
					if(Log)Log->Message("Объект "+tmo->Id+" найдена подпись 1");

				}
				if( j == 1 && tmo->Label2 == NULL )
				{
					tmo->Label2 = new TMapObj(MapView);
					tmo->Label2->MapView = MapView;
					MapView->ObjectByKey(0,MapView->MapLists->MapLists[1]->ListName,MapFind->MapObj->Key ,
					   tmo->Label2);
					if(Log)Log->Message("Объект "+tmo->Id+" найдена подпись 2");

				}
				if( j == 2 && tmo->Label3 == NULL )
				{
					tmo->Label3 = new TMapObj(MapView);
					tmo->Label3->MapView = MapView;
					MapView->ObjectByKey(0,MapView->MapLists->MapLists[1]->ListName,MapFind->MapObj->Key ,
					   tmo->Label3);
					if(Log)Log->Message("Объект "+tmo->Id+" найдена подпись 3");

				}
				j++;
			}
			if(Log)Log->SetBar(i);
		 }//end for
		 MapView->Refresh();
		 if(Log)Log->StopBar();
	}
	catch(Exception &e)
	{
		if( Log!=NULL)Log->Error("Ошибка создания списка меток "+e.Message);
    }
}



/**
* Удалить все объекты на карте и очистить список
*/
void __fastcall TKMapObjectList::DeleteAllByMap()
{
	try
	{
// Задаем поиск объектов
		 MapFind->Active                                     = false;
		 MapFind->FindPoint                                  = false;
		 MapFind->MapSelect->Clear();
//		 MapFind->MapSelect->Excode[-1][0]                   = false;
//		 MapFind->MapSelect->Excode[PanoramaObjCode][OL_MARK]= true;
		 MapFind->MapSelect->AddSemantic(PanoramaSemanticCode,"",SC_ANY);

		 if( Log )Log->StartBar(0,2);
		 if( Log )Log->SetBar(1);
		 for(MapFind->Active = true; !MapFind->Eof; MapFind->Next())
		 {
// Находим PK содержащийся в семантике
			 UnicodeString s_id   = MapFind->MapObj->Semantic->SemanticValue[MapFind->MapObj->Semantic->FindByCode(PanoramaSemanticCode)];
// Находим кординаты и переводим их в WGS-84
			 TCompMapPoint *point = MapFind->MapObj->Metric->Points[0][1];
			 point->PlaceOut = PP_GEO;
			 double X,Y;
			 point->GetPoint(X,Y);
			 MapFind->MapObj->Delete();
			 MapFind->MapObj->Commit();
			 if(Log)Log->Message("Удаляем объект "+s_id);
			 Application->ProcessMessages();
		 }//end for
		 MapView->Refresh();
		 Clear();
		 if(Log)Log->StopBar();
	}
	catch(Exception &e)
	{
		if( Log!=NULL)Log->Error("Ошибка удаления объекта с карты "+e.Message);
	}
}

/**
* Поиск в списке по идентификатору
*
*@param id - Значение первичного ключа объекта
*@return - Возвращается объект TKMapObject либо NULL если ничего не найдено
*/
TKMapObject * __fastcall TKMapObjectList::FindById(UnicodeString id)
{
	try
	{
		for( int i=0; i<Count; i++)
		{
			TKMapObject *tmo = Items[i];
			if( tmo ->Id == id )return tmo;
		}
		return NULL;
	}
	catch(Exception &e)
	{
		if( Log)Log->Error("Ошибка поиска объекта в списке "+e.Message);
	}

}



/**
* Создать объект с координатами и идентификатором на карте и в списке
* @param id - значение превичного ключа объекта
* @param X,Y - координаты P_GEO
* @return - вновь созданный объект TKMapObject
*/

TKMapObject * __fastcall TKMapObjectList::CreateMapObject(UnicodeString id,UnicodeString lat,UnicodeString lon)
{
	try
	{
		  double X,Y,X1,Y1;
		  Y        = StrToFloatDef(lon,0)*M_PI/180.0;
		  X        = StrToFloatDef(lat,0)*M_PI/180.0;
		  if( X == 0 || Y == 0 )return NULL;
		  TMapObj *mo = new TMapObj(MapView);
		  mo->MapView = MapView;
		  int key = mo->CreateObjectByExcode(0, KM_IDDOUBLE2,PanoramaObjCode,OL_MARK);
		  TCompMapPoint *point = new TCompMapPoint(MapView);
		  point->PlaceInp = PP_GEO;
		  point->PlaceOut = PP_PICTURE;
		  point->SetPoint(X,Y);
		  point->GetPoint(X1,Y1);
		  mo->Metric->Append(0,point);
		  mo->Semantic->AddWithValue(PanoramaSemanticCode,id);
		  mo->Commit();
		  TKMapObject *tmo = new TKMapObject(id,
				StrToFloatDef(lat,0),
				StrToFloatDef(lon,0),
				mo);
		  Add(tmo);
		  if(Log)Log->Message("Добавляем объект "+id+" "+
			 FloatToStr(X)+","+
			 FloatToStr(Y)+","+
			 FloatToStr(X1)+","+
			 FloatToStr(Y1));
		  return tmo;
	}
	catch(Exception &e)
	{
		if( Log)Log->Error("Ошибка создания объекта на карте "+e.Message);
	}

}

/**
* Создать объекты подписи к объекту
* @param tmo - Объект TKMapObject
* @param cod1,cod2,cod3 - Три кода объектов подписей для разных масштабов
* @param text - Текст подписи
*/
void __fastcall TKMapObjectList::CreateLabel3(TKMapObject *tmo,int cod1,int cod2, int cod3, UnicodeString text)
{
	try
	{

		 TCompMapPoint *point = tmo->MapObject->Metric->Points[0][1];
//		 TCompMapPoint *point1 = new TCompMapPoint(MapView);
//		 TCompMapPoint *point2 = new TCompMapPoint(MapView);
		 point->PlaceOut = PP_PLANE;
		 point->PlaceInp = PP_PLANE;
/*
		 point1->PlaceOut = PP_PLANE;
		 point1->PlaceInp = PP_PLANE;
		 point2->PlaceOut = PP_PLANE;
		 point2->PlaceInp = PP_PLANE;
*/
		 double X,Y;
		 point->GetPoint(X,Y);
//		 point1->SetPoint(X,Y+10);
 //		 point2->SetPoint(X,Y+50);
		 tmo->Name = text;
		 if( tmo->Label1 == NULL )
		 {
			 tmo->Label1 = new TMapObj(MapView);
			 tmo->Label1->MapView = MapView;
			 int key = tmo->Label1->CreateObjectByExcode(0, KM_IDDOUBLE2,cod1,OL_TEXT);
			 tmo->Label1->Text[0]    = text;
			 point->SetPoint(X+30,Y+22);
			 tmo->Label1->Metric->Append(0,point);
			 point->SetPoint(X+100,Y+70);
			 tmo->Label1->Metric->Append(0,point);
			 tmo->Label1->Semantic->AddWithValue(PanoramaSemanticCode,tmo->Id);
			 tmo->Label1->Commit();
		 }
		 if( tmo->Label2 == NULL )
		 {
			 tmo->Label2 = new TMapObj(MapView);
			 tmo->Label2->MapView = MapView;
			 int key = tmo->Label2->CreateObjectByExcode(0, KM_IDDOUBLE2,cod2,OL_TEXT);
			 tmo->Label2->Text[0]    = text;
			 point->SetPoint(X+30,Y+22);
			 tmo->Label2->Metric->Append(0,point);
			 point->SetPoint(X+100,Y+70);
			 tmo->Label2->Metric->Append(0,point);
			 tmo->Label2->Semantic->AddWithValue(PanoramaSemanticCode,tmo->Id);
			 tmo->Label2->Commit();
		 }
		 if( tmo->Label3 == NULL )
		 {
			 tmo->Label3 = new TMapObj(MapView);
			 tmo->Label3->MapView = MapView;
			 int key = tmo->Label3->CreateObjectByExcode(0, KM_IDDOUBLE2,cod3,OL_TEXT);
			 tmo->Label3->Text[0]    = text;
			 point->SetPoint(X+30,Y+22);
			 tmo->Label3->Metric->Append(0,point);
			 point->SetPoint(X+100,Y+70);
			 tmo->Label3->Metric->Append(0,point);
			 tmo->Label3->Semantic->AddWithValue(PanoramaSemanticCode,tmo->Id);
			 tmo->Label3->Commit();
		 }

		  if(Log)Log->Message("Добавляем метку "+text+" в объект "+tmo->Id);
	}
	catch(Exception &e)
	{
		if( Log)Log->Error("Ошибка создания подписи объекта "+e.Message);
	}

}

/**
* Перемещение объекта вместе с подписями по новым координаиам
* @param tmo - TKMapObject объект
* @param lat,lon - Широта и долгота в текстовм виде
*/
void __fastcall TKMapObjectList::MoveMapObject(TKMapObject *tmo,UnicodeString lat,UnicodeString lon)
{
	try
	{
// Находим старые координаты кординаты и переводим их в PP_GEO
		 TCompMapPoint *point = tmo->MapObject->Metric->Points[0][1];
		 point->PlaceOut = PP_GEO;
		 point->PlaceInp = PP_GEO;
		 double Xold,Yold,Xnew,Ynew,dX,dY,X,Y;
		 point->GetPoint(Xold,Yold);
//		 Yold        = Yold/M_PI*180;
//		 Xold        = Xold/M_PI*180;
// Находим новые координаты
		 Ynew        = StrToFloatDef(lon,0)*M_PI/180.0;
		 Xnew        = StrToFloatDef(lat,0)*M_PI/180.0;


		 if( Xnew == 0 || Ynew == 0 )return;
// Находим смещение
		 dX          = Xnew - Xold;
		 dY          = Ynew - Yold;
// Переносим объект
		 point->SetPoint(Xnew,Ynew);
		 tmo->MapObject->Metric->Points[0][1] = point;
		 tmo->MapObject->Commit();
// Переносим метки (если есть)

		 if( tmo->Label1 )
		 {
              int ex = tmo->Label1->ExCode;
			  point = tmo->Label1->Metric->Points[0][1];
			  point->PlaceOut = PP_GEO;
			  point->PlaceInp = PP_GEO;
			  point->GetPoint(X,Y);
			  point->SetPoint(X+dX,Y+dY);
			  tmo->Label1->Metric->Points[0][1] = point;
			  point = tmo->Label1->Metric->Points[0][2];
			  point->PlaceOut = PP_GEO;
			  point->PlaceInp = PP_GEO;
			  point->GetPoint(X,Y);
			  point->SetPoint(X+dX,Y+dY);
			  tmo->Label1->Metric->Points[0][2] = point;
			  tmo->Label1->Commit();
		 }
		 if( tmo->Label2 )
		 {
			  point = tmo->Label2->Metric->Points[0][1];
			  point->PlaceOut = PP_GEO;
			  point->PlaceInp = PP_GEO;
			  point->GetPoint(X,Y);
			  point->SetPoint(X+dX,Y+dY);
			  tmo->Label2->Metric->Points[0][1] = point;
			  point = tmo->Label2->Metric->Points[0][2];
			  point->PlaceOut = PP_GEO;
			  point->PlaceInp = PP_GEO;
			  point->GetPoint(X,Y);
			  point->SetPoint(X+dX,Y+dY);
			  tmo->Label2->Metric->Points[0][2] = point;
			  tmo->Label2->Commit();
		 }
		 if( tmo->Label3 )
		 {
			  point = tmo->Label3->Metric->Points[0][1];
			  point->PlaceOut = PP_GEO;
			  point->PlaceInp = PP_GEO;
			  point->GetPoint(X,Y);
			  point->SetPoint(X+dX,Y+dY);
			  tmo->Label3->Metric->Points[0][1] = point;
			  point = tmo->Label3->Metric->Points[0][2];
			  point->PlaceOut = PP_GEO;
			  point->PlaceInp = PP_GEO;
			  point->GetPoint(X,Y);
			  point->SetPoint(X+dX,Y+dY);
			  tmo->Label3->Metric->Points[0][2] = point;
			  tmo->Label3->Commit();
		 }

	}
	catch(Exception &e)
	{
		if( Log)Log->Error("Ошибка перемещения объекта "+e.Message);
	}

}


/*****************************************************************************
* Класс TKMapObjectStatus
*****************************************************************************/
/**
* Конструктор TKMapObjectStatus
*/

__fastcall TKMapObjectStatus::TKMapObjectStatus(TUniQuery *gQuery)
{
	ListPP = new TStringList();
	Query = gQuery;
	ListStatus = new TList();
/*
	Query->SQL->Clear();
	Query->SQL->Add("FROM J_SPRAV_STAT_RT rt, J_SPRAV_TYPE_OBJ tp");
	Query->SQL->Add(" WHERE  tp.id_object=rt.o_gis_type_obj_id");
	UnicodeSring s = "";
	for(Query->Open();  !Query->Eof; Query->Next() ){
	   TKMapObjectStatusType *most = new TKMapObjectStatusType;
	   most->SelectMode  = Query->FieldByName("v_asd_stat_rt_obj_mode")->AsAnsiString;
	   s                 = Query->FieldByName("v_asd_stat_rt_obj_color1")->AsAnsiString;
	   if( s == "" )most->Color1 = clRed;
	   else         most->Color1 = RGBStringToColor(s);
	   s                 = Query->FieldByName("v_asd_stat_rt_obj_color2")->AsAnsiString;
	   if( s == "" )most->Color2 = clRed;
	   else         most->Color2 = RGBStringToColor(s);
	   most->Status      = Query->FieldByName("v_asd_stat_rt_obj_id")->AsInteger;
	   s                 = Query->FieldByName("v_asd_stat_rt_obj_control_mode")->AsAnsiString;
	   most->ControlMode = mostNone;
	   if( s == "Семантика" )most->ControlMode = mostSemantic;
	   else if( s == "Выделение" )most->ControlMode = mostSelect;
	   else if( s == "Выделение+Семантика" )most->ControlMode = mostSelectAndSemantic;
	   most->SemValue    = Query->FieldByName("v_asd_stat_rt_obj_semantic")->AsAnsiString;

	}
*/


}

void __fastcall TKMapObjectStatus::Add(UnicodeString pp,UnicodeString  st)
{
	if (ListPP->IndexOf(pp) == -1 )
	{
		ListPP->Add(pp);
		ListPP->Add(st);
		add = true;
	}
	else
	{
		int num = ListPP->IndexOf(pp);
		if (st != ListPP->Strings[num+1]) add =  false;
    }
}

void __fastcall TKMapObjectStatus::Delete(UnicodeString pp)
{

}

/*****
* Новый метод, который не лезет в бызу
*/
void __fastcall TKMapObjectStatus::Status(TKController *cnt, TKSpravStatObject *sso) {
   if( sso == NULL )return;
   TMapObj *mo = (TMapObj *)cnt->MapObject;
   UnicodeString st =  IntToStr(sso->StatObject);
   mo->Style = OS_NORMAL;
   UnicodeString st1 = mo->Semantic->SemanticValue[mo->Semantic->FindByCode(sso->PanSemanticNum)];
// Проверка были ли изменения
   if( st == st1 )return;
   TObjectStyleSelect sel = SEL_COLOR;
   if( sso->PanStyleSelect == "COLOR")      sel = SEL_COLOR;
   else if( sso->PanStyleSelect == "BLINK1")sel = SEL_BLINK1;
   else if( sso->PanStyleSelect == "LINE")  sel = SEL_LINE;
   else if( sso->PanStyleSelect == "BLINK2")sel = SEL_BLINK2;
   switch( sso->PanControlMode ){
	  case  stocmSemantic :
		 mo->Semantic->SemanticValue[mo->Semantic->FindByCode(sso->PanSemanticNum)] = sso->PanSemanticValue;
		 mo->Commit();
		 break;
	  case 	stocmSelect   :
		 mo->Style         = OS_SELECT;
		 mo->StyleSelect   = sel;
		 mo->ColorImage    = sso->StatColor1;
		 mo->ColorImageUp  = sso->StatColor2;
		 mo->Commit();
	  case  stocmBoth     :
		 mo->Style         = OS_SELECT;
		 mo->StyleSelect   = sel;
		 mo->ColorImage    = sso->StatColor1;
		 mo->ColorImageUp  = sso->StatColor2;
		 mo->Semantic->SemanticValue[mo->Semantic->FindByCode(sso->PanSemanticNum)] = sso->PanSemanticValue;
		 mo->Commit();
   }
   Application->ProcessMessages();
}

/*****
* Старый метод, которые лезет в базу
*/
void __fastcall TKMapObjectStatus::Status(TKController *cnt,TKControllerStatus stat,int SPP)
{
	  TMapObj *mo = (TMapObj *)cnt->MapObject;
	  UnicodeString st =  IntToStr(stat);
	  Query->SQL->Clear();
	  Query->SQL->Add("SELECT v_asd_stat_rt_obj_id as id_obj, v_asd_stat_rt_obj_semantic as sem ");
	  Query->SQL->Add("FROM J_SPRAV_STAT_RT, J_SPRAV_TYPE_OBJ ");
	  Query->SQL->Add("WHERE J_SPRAV_TYPE_OBJ.v_gis_type_obj_name='ПП' ");
	  Query->SQL->Add("AND J_SPRAV_TYPE_OBJ.id_object=J_SPRAV_STAT_RT.o_gis_type_obj_id ");
	  Query->SQL->Add("AND J_SPRAV_STAT_RT.v_asd_stat_rt_obj_id="+st+" order by v_asd_stat_rt_obj_id ");
	  Query->Open();
	  UnicodeString st2 = Query->FieldByName("sem")->AsAnsiString;
	  mo->Style = OS_NORMAL;
 //	  Add(IntToStr(mo->Key),st);
	  UnicodeString st1 = mo->Semantic->SemanticValue[mo->Semantic->FindByCode(SPP)];
 //	  UnicodeString st1 = mo->Semantic->FindByCode(SPP);
  //	  if ( (! add) || (st!=st1) )
	  if (( st != st1 ) &&  (st2 != ""))
	  {
			  Query->SQL->Clear();
			  Query->SQL->Add("SELECT v_asd_stat_rt_obj_id as id_obj,v_asd_stat_rt_obj_name as nam,");
			  Query->SQL->Add("v_asd_stat_rt_obj_color1 as color1, v_asd_stat_rt_obj_color2 as color2, v_asd_stat_rt_obj_mode as styl,");
			  Query->SQL->Add("v_asd_stat_rt_obj_control_mode as stattype, v_asd_stat_rt_obj_semantic as sem ");
			  Query->SQL->Add("FROM J_SPRAV_STAT_RT, J_SPRAV_TYPE_OBJ ");
			  Query->SQL->Add("WHERE J_SPRAV_TYPE_OBJ.v_gis_type_obj_name='ПП' ");
			  Query->SQL->Add("AND J_SPRAV_TYPE_OBJ.id_object=J_SPRAV_STAT_RT.o_gis_type_obj_id ");
			  Query->SQL->Add("AND J_SPRAV_STAT_RT.v_asd_stat_rt_obj_id="+st+" order by v_asd_stat_rt_obj_id ");
			  Query->Open();
			  Color1 = RGBStringToColor(Query->FieldByName("color1")->AsAnsiString);
			  Color2 = RGBStringToColor(Query->FieldByName("color2")->AsAnsiString);
			  TObjectStyleSelect sel;
			  if (Query->FieldByName("styl")->AsAnsiString == "COLOR")
			  {
				  sel = SEL_COLOR;
			  }
			  if (Query->FieldByName("styl")->AsAnsiString == "BLINK1")
			  {
				  sel = SEL_BLINK1;
			  }
			  if (Query->FieldByName("styl")->AsAnsiString == "LINE")
			  {
				  sel = SEL_LINE;
			  }
			  if (Query->FieldByName("styl")->AsAnsiString == "BLINK2")
			  {
				  sel = SEL_BLINK2;
			  }
			  if (Query->FieldByName("stattype")->AsString == "Семантика")
			  {
				  mo->Semantic->SemanticValue[mo->Semantic->FindByCode(SPP)] = Query->FieldByName("sem")->AsAnsiString;
				  mo->Commit();

			  }
			  if (Query->FieldByName("stattype")->AsAnsiString == "Выделение")
			  {
				  mo->Style         = OS_SELECT;
				  mo->StyleSelect   = sel;
				  mo->ColorImage    = Color1;
				  mo->ColorImageUp  = Color2;
				  mo->Commit();
				  Application->ProcessMessages();
			  }
			  if (Query->FieldByName("stattype")->AsAnsiString== "Выделение+Семантика")
			  {
				  mo->Style         = OS_SELECT;
				  mo->StyleSelect   = sel;
				  mo->ColorImage    = Color1;
				  mo->ColorImageUp  = Color2;
				  mo->Semantic->SemanticValue[mo->Semantic->FindByCode(SPP)] = Query->FieldByName("sem")->AsAnsiString;
				  mo->Commit();
			  }
			  Application->ProcessMessages();
			  Query->Close();
			  Query->SQL->Clear();
	  }
}


 /*****************************************************************************
* Класс TKMapObjectSearch
*****************************************************************************/
/**
* Конструктор TKMapObjectSearch
*/

__fastcall TKMapObjectSearch::TKMapObjectSearch(TMapObj *tmo,TMapFind *tmf,TUniQuery *gQuery)
{
   tmo1          = tmo;
   tmf1          = tmf;
   MapView       = tmo->MapView;
   Query1        = gQuery;
   ListObjTypes  = new TList();
   ListObjSelect = new TList();
   Query1->SQL->Clear();
   Query1->SQL->Add("SELECT st.*,tl.v_groupuserlayer,tl.v_userlayer FROM J_SPRAV_TYPE_OBJ st,j_layer_panorama_user lu,j_tree_user_layer tl");
   Query1->SQL->Add("WHERE lu.id_object(+) = st.o_codeline AND lu.o_userlayer  = tl.id_object(+) AND tl.v_onoff='Да'");
   Query1->SQL->Add("AND v_gis_type_obj_code_parent IS NULL");
   Query1->SQL->Add("ORDER BY v_gis_type_obj_id");


//   Query1->SQL->Add("SELECT * FROM J_SPRAV_TYPE_OBJ ORDER BY v_gis_type_obj_id");
   for( Query1->Open(); !Query1->Eof; Query1->Next() ){
	  TKSpravMapObjectType *mop = new TKSpravMapObjectType;
	  mop->Id             = Query1->FieldByName("v_gis_type_obj_id")->AsInteger;
//	  mop->Name           = Query1->FieldByName("v_gis_type_obj_name")->AsString;
	  mop->Name           = Query1->FieldByName("v_userlayer")->AsString;
	  mop->Group          = Query1->FieldByName("v_groupuserlayer")->AsString;
	  mop->Type           = Query1->FieldByName("v_gis_type_obj_name")->AsString;
	  mop->CodePanorama   = StrToIntDef(Query1->FieldByName("v_gis_type_obj_code")->AsString,-1);
	  mop->CodeParent     = StrToIntDef(Query1->FieldByName("v_gis_type_obj_code_parent")->AsString,-1);
	  mop->CodeSemantic   = StrToIntDef(Query1->FieldByName("v_gis_type_obj_sem_stat")->AsString,-1);
	  mop->PKCodeSemantic = StrToIntDef(Query1->FieldByName("v_gis_type_obj_sem_pkcode")->AsString,-1);
	  ListObjTypes->Add( mop );
   }
}

/**
* Нахождение кода понарамы по названию в справочнике
*/
TKSpravMapObjectType * __fastcall TKMapObjectSearch::FindByName(UnicodeString name){
   for( int i=0; i< ListObjTypes->Count; i++ ){
	   TKSpravMapObjectType *mop = (TKSpravMapObjectType *)ListObjTypes->Items[i];
	   if( mop->Name == name )return mop;
   }
   return NULL;

}

/**
* Нахождение кода понарамы
*/
TKSpravMapObjectType * __fastcall TKMapObjectSearch::FindByCode(int code){
   for( int i=0; i< ListObjTypes->Count; i++ ){
	   TKSpravMapObjectType *mop = (TKSpravMapObjectType *)ListObjTypes->Items[i];
	   if( mop->CodePanorama == code )return mop;
   }
   return NULL;

}

/**
* Найти минимальное расстояние между точкой поиска и текущей точкой MapObj
*/
double __fastcall TKMapObjectSearch::GetDistance( double X, double Y , TMapObj *Obj){

   double d = -1;
   for( int i=0; i< Obj->Metric->PointCount[0]; i++ ){
	  Obj->Metric->Points[0][i+1]->PlaceInp = PP_PICTURE;
	  Obj->Metric->Points[0][i+1]->PlaceOut = PP_PLANE;
	  double a  = X - Obj->Metric->Points[0][i+1]->X;
	  double b  = Y - Obj->Metric->Points[0][i+1]->Y;
//      double a,b;
//	  Obj->Metric->Points[0][i+1]->GetPoint(a,b);
	  double d1 = sqrt( a*a + b*b );
	  if( i == 0 )d = d1;
	  else if( d1 < d && d1!=0 )d = d1;
   }
   return d;
}


/**
* Поиск пункта питания по сети
*/
void __fastcall TKMapObjectSearch::Search(TColor Color1,TObjectStyleSelect Type)
{

	tmo1->StyleSelect = Type;
	tmo1->Center();
//**	SelectClear();
//**	SelectAdd(tmo1,Color1,Type);

	tmo1->MapView->Refresh();
	tmo1->MapView->ViewSelect->ClearSampleAll();
	tmo1->MapView->Refresh();
	int ret = tmo1->MapView->ViewSelect->SelectSample(true,tmo1->ListName,tmo1->Key);
//	tmo1->Style  = OS_NORMAL;
	tmo1->MapView->Refresh();


	 tmo1->MapView->ViewSelect->SetSiteEnabledForSample();
	 tmo1->MapView->Refresh();
	 tmo1->MapView->SetViewSelected(true,Color1);
//	 tmo1->Style  = OS_NORMAL;
//	 tmf1->Active    = false;
	 tmo1->MapView->Refresh();
//	 tmo1->Center();
//	 tmo1->MapView->Refresh();
//	 return;

	TKSpravMapObjectType *mop_net = FindByName("Сети НО");
	TKSpravMapObjectType *mop_pp  = FindByName("ПП");
	if( mop_net == NULL || mop_pp == NULL ){
	   ShowMessage("В справочнике типов объектов панорамы нет сетей и ПП");
	   return;
	}
	UnicodeString id = "";
	double X,Y,X1,Y1,X2,Y2,XX,YY;
	int i = 1;
	TCompMapPoint *point = tmo1->Metric->Points[0][1];
	point->PlaceOut = PP_PICTURE;
	point->GetPoint(X,Y);
	// Запускаем поиск с радиусом  пиксела у найденного объекта
	tmf1->Active    = false;
	tmf1->MapSelect->ClearAll();
	tmf1->FindPoint = true;
	tmf1->X         = X;
	tmf1->Y         = Y;
	tmf1->Radius    = 100;
	tmf1->Active    = true;
	// Перебираем объекты в точке поиска
	UnicodeString code = "";
	double xy = X*Y;
	double xy1,A,B,C,T;
	for(;!tmf1->Eof;tmf1->Next())	{
	   if( tmf1->MapObj->ExCode == mop_net->CodePanorama ){ //Если Сеть
		  id   = tmf1->MapObj->Semantic->SemanticValue[tmf1->MapObj->Semantic->FindByCode(mop_net->PKCodeSemantic)];
		  point = tmo1->Metric->Points[0][1];
		  point->PlaceOut = PP_PICTURE;
		  point->GetPoint(X1,Y1);
		  point = tmo1->Metric->Points[0][2];
		  point->PlaceOut = PP_PICTURE;
		  point->GetPoint(X2,Y2);
		  i=3;
		  while (X2>0)	   {
			 A = Y2-Y1;
			 B = X1-X2;
			 C = Y1*(X2-X1)-X1*A;
			 T = sqrt(A*A+B*B);
			 xy1 = abs(A*X/T + B*Y/T + C/T);
			 if (xy > xy1)	   {
				xy = xy1;
				code = id;
			 }
			 X1 = X2;
			 Y1 = Y2;
			 point = tmo1->Metric->Points[0][i];
			 point->PlaceOut = PP_PICTURE;
			 point->GetPoint(X2,Y2);
			 i++;
		  }
	   }
	}
	if (code!="")	{
		tmf1->Active    = false;
		tmf1->MapSelect->ClearAll();
		tmf1->FindPoint = false;
		tmf1->MapSelect->ClearSemantic();
		tmf1->MapSelect->AddSemantic( mop_pp->PKCodeSemantic ,code,SC_EQUAL);
		tmf1->Active  = true;
		for(;!tmf1->Eof;tmf1->Next())	{
			if( tmf1->MapObj->ExCode == mop_pp->CodePanorama) { //Если ПП
//**				SelectAdd(tmf1->MapObj,Color1,Type);

			tmo1->Style = OS_SELECT;
			tmo1->MapView->ViewSelect->SelectSample(true,tmo1->ListName,tmo1->Key);
			tmo1->StyleSelect = Type;
			   break;
			}
		}
	 }
	 tmo1->MapView->ViewSelect->SetSiteEnabledForSample();
	 tmo1->MapView->SetViewSelected(true,Color1);
	 tmo1->Style  = OS_NORMAL;
	 tmf1->Active      = false;
	 MapView->Selecting = false;
	 MapView->Refresh();
}

/**
* Найти все объекты АСД в радиусе R метров от точки X,Y и записать в GRID
*/

int __fastcall TKMapObjectSearch::SearchAllObjects(double X,double Y,int R,TAdvStringGrid *Grid){
// Очистка таблицы
   for( int i=Grid->FixedCols; i<Grid->ColCount; i++ ){
	  for( int j=1; j<Grid->RowCount; j++ )Grid->Cells[i][j] = "";
	  Grid->RemoveCheckBox(0,i);
   }
// З
   Grid->FixedRows = 0;
   Grid->RowCount  = 1;
   Grid->ColCount  = 6;

   tmf1->Active    = false;
   tmf1->MapSelect->ClearAll();
   tmf1->FindPoint = true;
   TPPLACE place_inp = tmf1->PlaceInp;
   TPPLACE place_out = tmf1->PlaceOut;


   tmf1->PlaceInp = PP_PLANE;
   tmf1->PlaceOut = PP_PICTURE;
   tmf1->X        = X;
   tmf1->Y        = Y;
   tmf1->Radius   = R;
   Grid->ColWidths[1] = Grid->Canvas->TextWidth(Grid->Cells[1][0])+5;
	// Перебираем объекты в точке поиска
   for(	tmf1->Active = true; !tmf1->Eof; tmf1->Next() ){
	  TKSpravMapObjectType *mop = FindByCode( tmf1->MapObj->ExCode );
	  if( mop == NULL )continue;
	  if( mop->CodeParent > 0 )continue;
	  UnicodeString name = "";
	  if( mop->Group != "" )name = mop->Group+" - ";
	  if( mop->Type != "" && mop->Type != mop->Group )name = name + mop->Type+", ";
	  name = name + mop->Name;
	  UnicodeString s_name = "";
	  if( mop->PKCodeSemantic > 0 )
		 s_name   =  tmf1->MapObj->Semantic->SemanticValue[tmf1->MapObj->Semantic->FindByCode(mop->PKCodeSemantic)];
	  if( s_name != "" )name = name+" "+s_name;
	  int dist = (int)GetDistance(X,Y,tmf1->MapObj);

// Ищем что такой объект уже есть в таблице
	  int n = -1;
	  for( int i=0; i<Grid->RowCount; i++ )
		 if( name == Grid->Cells[1][i] ){
			n = i;
			break;
		 }
	  if( n == -1 ){
		 int w = Grid->Canvas->TextWidth(Grid->Cells[1][Grid->RowCount-1])+5;
		 if( Grid->ColWidths[1]<w)Grid->ColWidths[1] = w;


		 Grid->RowCount++;
		 Grid->Objects[0][Grid->RowCount-1] = tmf1->MapObj;
		 Grid->Cells[1][Grid->RowCount-1]   = name;
		 Grid->Cells[3][Grid->RowCount-1]   = IntToStr( dist );
		 Grid->Cells[4][Grid->RowCount-1]   = IntToStr(tmf1->MapObj->Key);
		 Grid->Cells[5][Grid->RowCount-1]   = IntToStr(tmf1->MapObj->LayerNumber);
		 Grid->AddCheckBox(0,Grid->RowCount-1,true,false);
		 Grid->CellProperties[0][Grid->RowCount-1]->ReadOnly = false;
		 Grid->CellProperties[1][Grid->RowCount-1]->ReadOnly = true;
		 Grid->CellProperties[2][Grid->RowCount-1]->ReadOnly = true;
		 Grid->CellProperties[3][Grid->RowCount-1]->ReadOnly = true;
		 Grid->CellProperties[4][Grid->RowCount-1]->ReadOnly = true;
		 Grid->CellProperties[5][Grid->RowCount-1]->ReadOnly = true;

	  }
	  else {
		 if( StrToIntDef( Grid->Cells[3][n],99999999 ) > dist ){
			Grid->Objects[0][Grid->RowCount-1] = tmf1->MapObj;
			Grid->Cells[3][Grid->RowCount-1]   = IntToStr( dist );
			Grid->Cells[4][Grid->RowCount-1]   = IntToStr(tmf1->MapObj->Key);
		    Grid->Cells[5][Grid->RowCount-1]   = IntToStr(tmf1->MapObj->LayerNumber);
		 }

	  }

   }
   if( Grid->RowCount > 1 )Grid->FixedRows = 1;
//   Grid->Group(1);

   tmf1->PlaceInp    = place_inp;
   tmf1->PlaceOut    = place_out;
   Grid->ClientWidth = Grid->ColWidths[0] + Grid->ColWidths[1] + Grid->ColWidths[3] + 10;
   return (Grid->RowCount-1);
}


/**
* Чистим все объекты выделения на карте
*/
void __fastcall TKMapObjectSearch::SelectClear(){
   for( int i=0; i< ListObjSelect->Count; i++ ){
	  TMapObj *mo = (TMapObj *)ListObjSelect->Items[i];
	  mo->Style   = OS_NORMAL;
	  mo->Commit();
	  delete mo;
   }
   ListObjSelect->Clear();
   MapView->Refresh();
}

/**
* Добавляем объект выделения на карту
*/
void __fastcall TKMapObjectSearch::SelectAdd( TMapObj *Obj, TColor Color, TObjectStyleSelect Type ) {
   TMapObj *mo       = new TMapObj( MapView );
   mo->MapView       = MapView;
   MapView->ObjectByKey(0,MapView->MapLists->MapLists[1]->ListName,Obj->Key ,mo);
   mo->Style         = OS_SELECT;
   mo->StyleSelect   = Type;
   mo->ColorImage    = clRed;
   mo->ColorImageUp  = clWhite;
   mo->Commit();
}

