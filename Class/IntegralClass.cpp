#include "IntegralClass.h"
#include "AsdClass3.h"
#include "UdpClass.h"
#include "LogClass.h"
#include "Funcs.h"


/*****************************************************************************
* Класс TKIntergralSpravEvent, TKIntergralSpravEventItem т TKIntergralSpravEventList
*****************************************************************************/
/**
* Конструктор TKIntergralSpravEvent
*/
__fastcall TKIntergralSpravEvent::TKIntergralSpravEvent():TObject(){
   Id   = -1;
   Name = "";
   Nsxema = -1;
   for( int i=0; i<INTEGRAL_MASK_SIZE; i++)Mask[i]=(char)0x00;
}

/**
* Конструктор TKIntergralSpravEventItem
*/
__fastcall TKIntergralSpravEventItem::TKIntergralSpravEventItem(TCollection *Collection)
   : TCollectionItem(Collection), FValue(0) { }

/**
* Конструктор TKIntergralSpravEventList
*/
__fastcall TKIntergralSpravEventList::TKIntergralSpravEventList(TUniQuery *query)
	: TCollection(__classid(TKIntergralSpravEventItem)) {
	Query = query;
}

/**
* Добавить элемент в список
*/
void __fastcall TKIntergralSpravEventList::Add(TKIntergralSpravEvent *AValue)
{
	TKIntergralSpravEventItem *Item = (TKIntergralSpravEventItem *) TCollection::Add();
	Item->Value = AValue;
}

/**
* Удалить элемент из списка
*/
void __fastcall TKIntergralSpravEventList::Delete(int Index)
{
	if( Index < 0 && Index >= TCollection::Count )return;
	delete Items[Index];
	TCollection::Delete(Index);
}

/**
* Получить элемент из списка
*/
TKIntergralSpravEvent * __fastcall TKIntergralSpravEventList::GetItem(int Index)
{
	TKIntergralSpravEventItem *Item = (TKIntergralSpravEventItem *) TCollection::GetItem(Index);
	return Item->Value;
}

/*
* Сохранить элемент в список
*/
void __fastcall TKIntergralSpravEventList::SetItem(int Index, TKIntergralSpravEvent * AValue)
{
	TKIntergralSpravEventItem *Item = (TKIntergralSpravEventItem *) TCollection::GetItem(Index);
	Item->Value = AValue;
}

/**
* Сформировать список событий из справочника
* @param Query - для запросов к базе
*/
void __fastcall TKIntergralSpravEventList::MakeFromBase(  ){
//   TMemStream

   Query->Close();
   Query->SQL->Clear();
   Query->SQL->Add("SELECT * FROM spr_events ORDER BY nochems,sortclass");
   for( Query->Open(); !Query->Eof; Query->Next() ){
	  TKIntergralSpravEvent *event = new TKIntergralSpravEvent();
	  event->Id   = Query->FieldByName("id_event")->AsInteger;
	  event->Name = Query->FieldByName("naim_event")->AsString;
	  event->Nsxema = Query->FieldByName("nochems")->AsInteger;
	  TBlob *blob  = Query->GetBlob("vmask");
	  blob->Read(0,32,event->Mask);

	  Add(event);
   }



}



/**
* Сформировать список событий по маске
* @param mask - маска
* @param sxema - номер схемы
*/
void __fastcall TKIntergralSpravEventList::FindByMask( TKIntergralSpravEventList *stats, UnicodeString mask, int nsxema)
{
   stats->Clear();
   for( int i=0; i<Count; i++ ){
	  TKIntergralSpravEvent *event = Items[i];
	  if( nsxema == event->Nsxema ){
		 int n = 0;
		 char mask1[INTEGRAL_MASK_SIZE];
		 HexToChar1(mask,mask1);

		 for( int j=0; j<INTEGRAL_MASK_SIZE/2; j++ ){
			char x1 = event->Mask[j];
			char x2 = mask1[j];

			char x = event->Mask[j] & mask1[j];
			if(  x == event->Mask[j] )n++;
		 }
		 if( n == INTEGRAL_MASK_SIZE/2 ){
			stats->Add( event );
		 }


	  }
   }
}

void __fastcall TKIntergralSpravEventList::HexToChar1(UnicodeString mask,char *mask1){
   for( int i=0; i<INTEGRAL_MASK_SIZE/2 && i<mask.Length()/2; i++){
	  mask1[i] = 0;
	  switch( mask[i*2+1] ){
		 case '0' : mask1[i] = 0x00; break;
		 case '1' : mask1[i] = 0x10; break;
		 case '2' : mask1[i] = 0x20; break;
		 case '3' : mask1[i] = 0x30; break;
		 case '4' : mask1[i] = 0x40; break;
		 case '5' : mask1[i] = 0x50; break;
		 case '6' : mask1[i] = 0x60; break;
		 case '7' : mask1[i] = 0x70; break;
		 case '8' : mask1[i] = 0x80; break;
		 case '9' : mask1[i] = 0x90; break;
		 case 'a' :
		 case 'A' : mask1[i] = 0xa0; break;
		 case 'b' :
		 case 'B' : mask1[i] = 0xb0; break;
		 case 'c' :
		 case 'C' : mask1[i] = 0xc0; break;
		 case 'd' :
		 case 'D' : mask1[i] = 0xd0; break;
		 case 'e' :
		 case 'E' : mask1[i] = 0xe0; break;
		 case 'f' :
		 case 'F' : mask1[i] = 0xf0; break;
	  }
	  switch( mask[i*2+2] ){
		 case '0' : mask1[i] += 0x00; break;
		 case '1' : mask1[i] += 0x01; break;
		 case '2' : mask1[i] += 0x02; break;
		 case '3' : mask1[i] += 0x03; break;
		 case '4' : mask1[i] += 0x04; break;
		 case '5' : mask1[i] += 0x05; break;
		 case '6' : mask1[i] += 0x06; break;
		 case '7' : mask1[i] += 0x07; break;
		 case '8' : mask1[i] += 0x08; break;
		 case '9' : mask1[i] += 0x09; break;
		 case 'a' :
		 case 'A' : mask1[i] += 0x0a; break;
		 case 'b' :
		 case 'B' : mask1[i] += 0x0b; break;
		 case 'c' :
		 case 'C' : mask1[i] += 0x0c; break;
		 case 'd' :
		 case 'D' : mask1[i] += 0x0d; break;
		 case 'e' :
		 case 'E' : mask1[i] += 0x0e; break;
		 case 'f' :
		 case 'F' : mask1[i] += 0x0f; break;
	  }
   }

}



 /******************************************************************************/
 /* Класс TKIntegralValue                                                      */
 /******************************************************************************/
__fastcall TKIntegralValue::TKIntegralValue(void *cnt,TObject *controllers,TKIntergralSpravEventList *spr_event){
   Controller  = cnt;
   Controllers = controllers;
   SpravEvent  = spr_event;
   Stats       = new TKIntergralSpravEventList( spr_event->Query );
   Id = ((TKController *)cnt)->Object;
   Name = ((TKController *)cnt)->AsoduName;
}

/**
* Считываем последнее значение
*/
void __fastcall TKIntegralValue::GetLastValue(){
   TUniQuery *Query = SpravEvent->Query;
   Query->Close();
   Query->SQL->Clear();
   Query->SQL->Add("SELECT * FROM event_log WHERE id_log = ( SELECT max(id_log) FROM event_log WHERE nsign1 IS NOT NULL AND id_pv=:a0 )");
   Query->Params->Items[0]->AsInteger = Id;
   Query->Open();
   if( !Query->Eof ){

	  DT   = DateOf(Query->FieldByName("data_event")->AsDateTime) + TimeOf(Query->FieldByName("time_event")->AsDateTime);
	  Stat = Query->FieldByName("nsign1")->AsString;
	  if( Stat.Length() >= INTEGRAL_MASK_SIZE )SpravEvent->FindByMask( Stats, Stat, Nsxema );

   }
}

//;

