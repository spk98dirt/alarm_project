#ifndef IntegralClassH
#define IntegralClassH

#include <stdio.h>
#include <math.h>

#include <Classes.hpp>
#include <Controls.hpp>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>

#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "Uni.hpp"
#include <Data.DB.hpp>

/*
#include "AsdClass3.h"
#include "UdpClass.h"
#include "LogClass.h"
#include "Funcs.h"
*/


#define INTEGRAL_MASK_SIZE 32

class TKIntergralSpravEvent: public TObject
{
private:
public:
   int Id;                         //Идентификатор
   int Nsxema;                     //Номер схемы
   UnicodeString Name;             //Событие
   char Mask[INTEGRAL_MASK_SIZE];  //Маска
   __fastcall TKIntergralSpravEvent();

};

class TKIntergralSpravEventItem: public TCollectionItem
{
private:
	TKIntergralSpravEvent *FValue;
public:
	__fastcall TKIntergralSpravEventItem(TCollection *Collection);
	__property TKIntergralSpravEvent *Value = {read = FValue, write = FValue};
};

class TKIntergralSpravEventList : public TCollection
{
private:
	TKIntergralSpravEvent * __fastcall GetItem(int Index);
	void __fastcall SetItem(int Index,TKIntergralSpravEvent * AValue);
public:
	TUniQuery *Query;
	__fastcall TKIntergralSpravEventList(TUniQuery *Query);
	void __fastcall Add(TKIntergralSpravEvent * AValue);
	void __fastcall Delete(int Index);
	__property TKIntergralSpravEvent *Items[int Index] = {read = GetItem, write = SetItem};
	void __fastcall MakeFromBase( );


	void __fastcall FindByMask( TKIntergralSpravEventList *stats, UnicodeString mask, int nsxema);
    void __fastcall HexToChar1(UnicodeString mask,char *mask1);
};


class TKIntegralValue: TObject {
   protected:
   public:
	  int Id;
	  int Nsxema;
	  UnicodeString Name;
	  AnsiString Stat;
//	  char Stat[INTEGRAL_MASK_SIZE];  //32 байт состояний

	  __fastcall TKIntegralValue(void *cnt,TObject *controllers,TKIntergralSpravEventList *spr_event);


	  double *Ia,*Ib,*Ic;  //Текущие значения токов по каждой фазе
	  bool Link;           //Состояние связи
	  TDateTime DT;
	  void *Controller;
	  TObject *Controllers;
	  TKIntergralSpravEventList *SpravEvent; //Справочник состояний
	  TKIntergralSpravEventList *Stats;     //Список состояний

  	  void __fastcall GetLastValue();



};



#endif

