//---------------------------------------------------------------------------

#ifndef MapClassH
#define MapClassH
//---------------------------------------------------------------------------
#endif

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "mapcmp.hpp"
#include "mapviewa.hpp"
#include "mapwina.hpp"
#include "mapcompa.hpp"
#include "mapfind.hpp"
#include "mapobj.hpp"
#include "mapobja.hpp"
#include "Uni.hpp"


#include <Vcl.ComCtrls.hpp>
#include "AdvGrid.hpp"
#include "LogClass.h"
#include "AsdClass3.h"
#include "Funcs.h"


// Флаг каким образом выделять объект
enum TKMapObjectStatusControl
{
   mostNone               = 0,    //
   mostSelect             = 1,    // выделение
   mostSemantic           = 2,    // семантика
   mostSelectAndSemantic  = 3     // выделение+семантика
};


enum TKMapObjectType
{
   motTrafficLight  = 100,
   motAutoTracker   = 200
};

struct TKMapObjectStatusType {
   UnicodeString SelectMode;
   TColor Color1;
   TColor Color2;
//   UnicodeString RtName;
   int    Status;
//   UnicodeString SemName;
   TKMapObjectStatusControl ControlMode;
   UnicodeString SemValue;
//   int    SemStatCode;
//   int    SemPKCode;
//   int    ObjCode;
   UnicodeString ObjType;

};


class TKMapObject: public TObject
{
private:
	TMapView *MapView;
public:
	UnicodeString         Id;
	UnicodeString         Name;
	UnicodeString         Stat;
	int                   Stat1,Stat2,Stat3;
	TMapObj               *MapObject,*Label1, *Label2, *Label3;
	double                Latitude;
	double                Longitude;
	TDateTime             CreateDT;
	TDateTime             UpdateDT;
	TDateTime             CheckDT;
	__fastcall TKMapObject(UnicodeString id,double lat,double lon,TMapObj *mo);
};



class TKMapObjectItem: public TCollectionItem
{
private:
	TKMapObject *FValue;
public:
	__fastcall TKMapObjectItem(TCollection *Collection);
	__property TKMapObject *Value = {read = FValue, write = FValue};
};

class TKMapObjectList : public TCollection
{
private:
	TKMapObjectType Type;
	TKMapObject * __fastcall GetItem(int Index);
	TMapView *MapView;
	TMapObj  *MapObj;
	TMapFind *MapFind;
	int PanoramaObjCode;
	int PanoramaSemanticCode;
	void __fastcall SetItem(int Index,TKMapObject * AValue);
public:
	TKLog *Log;
	__fastcall TKMapObjectList(TMapView *map,int pan_obj, int pan_sem);
	void __fastcall Add(TKMapObject * AValue);
	void __fastcall Delete(int Index);
	__property TKMapObject *Items[int Index] = {read = GetItem, write = SetItem};
	void __fastcall CreateAllByMap(int sem,UnicodeString val);
	void __fastcall CreateAllLabelByMap();
	void __fastcall DeleteAllByMap();
	TKMapObject * __fastcall FindById(UnicodeString id);
	TKMapObject * __fastcall CreateMapObject(UnicodeString id,UnicodeString lat,UnicodeString lon);
	void __fastcall CreateLabel3(TKMapObject *tmo,int sem1,int sem2, int sem3, UnicodeString text);
	void __fastcall MoveMapObject(TKMapObject *tmo,UnicodeString lat,UnicodeString lon);

};




class TKMapObjectStatus: public TObject
{
private:
	TStringList * ListPP;
	TList       *ListStatus;
	TUniQuery *Query;
	void __fastcall Add(UnicodeString pp,UnicodeString  st);
	bool add;
	void __fastcall Delete(UnicodeString pp);
public:
 //	TMapObjectStatusType SatusType;
	UnicodeString Id;
	TColor Color1;
	TColor Color2;
   //	__fastcall TKMapObjectStatus(UnicodeString id,Tcolor col1,TColor col2,TMapObj *mo);
	__fastcall TKMapObjectStatus(TUniQuery *gQuery);
	void __fastcall Status(TKController *cnt,TKControllerStatus stat,int SPP);
    void __fastcall Status(TKController *cnt, TKSpravStatObject *sso);

};


struct TKSpravMapObjectType {
   int Id;
   UnicodeString Name;
   UnicodeString Group;
   UnicodeString Type;
   int CodePanorama;
   int CodeParent;
   int CodeSemantic;
   int PKCodeSemantic;
};


class TKMapObjectSearch: public TObject
{
private:
	 TMapObj *tmo1;
	 TMapFind *tmf1;
	 TMapView *MapView;
	 TUniQuery *Query1;
	 TList *ListObjTypes;
	 TList *ListObjSelect;
public:
	__fastcall TKMapObjectSearch(TMapObj *tmo,TMapFind *tmf,TUniQuery *gQuery);
	void __fastcall Search(TColor Color1,TObjectStyleSelect Type);
	TKSpravMapObjectType * __fastcall FindByName(UnicodeString name);
	TKSpravMapObjectType * __fastcall FindByCode(int code);
	int __fastcall SearchAllObjects(double X,double Y,int R,TAdvStringGrid *Grid);
	double __fastcall GetDistance( double X, double Y , TMapObj *Obj);
	void __fastcall SelectClear();
	void __fastcall SelectAdd( TMapObj *Obj, TColor Color, TObjectStyleSelect Type );

};



 /*

class TKMapObjectItem: public TCollectionItem
{
private:
	TKMapObject *FValue;
public:
	__fastcall TKMapObjectItem(TCollection *Collection);
	__property TKMapObject *Value = {read = FValue, write = FValue};
};

class TKMapObjectList : public TCollection
{
private:
	TKMapObjectType Type;
	TKMapObject * __fastcall GetItem(int Index);
	TMapView *MapView;
	TMapObj  *MapObj;
	TMapFind *MapFind;
	int PanoramaObjCode;
	int PanoramaSemanticCode;
	void __fastcall SetItem(int Index,TKMapObject * AValue);
public:
	TKLog *Log;
	__fastcall TKMapObjectList(TMapView *map,int pan_obj, int pan_sem);
	void __fastcall Add(TKMapObject * AValue);
	void __fastcall Delete(int Index);
	__property TKMapObject *Items[int Index] = {read = GetItem, write = SetItem};
	void __fastcall CreateAllByMap(int sem,UnicodeString val);
	void __fastcall CreateAllLabelByMap();
	void __fastcall DeleteAllByMap();
	TKMapObject * __fastcall FindById(UnicodeString id);
	TKMapObject * __fastcall CreateMapObject(UnicodeString id,UnicodeString lat,UnicodeString lon);
	void __fastcall CreateLabel3(TKMapObject *tmo,int sem1,int sem2, int sem3, UnicodeString text);
	void __fastcall MoveMapObject(TKMapObject *tmo,UnicodeString lat,UnicodeString lon);

};
*/
