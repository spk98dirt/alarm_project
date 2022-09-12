#ifndef AsdClass3H
#define AsdClass3H

#include <Vcl.Imaging.jpeg.hpp>

#include <stdio.h>
#include <math.h>

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>

#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.BubbleCh.hpp>
#include <VCLTee.GanttCh.hpp>

#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "Uni.hpp"
#include <Data.DB.hpp>

#include "fs_iinterpreter.hpp"
#include "fs_icpp.hpp"

#include "LogClass.h"
#include "SemClass.h"
#include "Funcs.h"
#include "IntegralClass.h"

#define MAX_PARAM         256
#define MAX_CONTROLLER    1024
#define MAX_TRAFFIC_LIGHT 1024
#define MAX_ALARM         256
#define MAX_ALARM_PARAM   8




enum TKProblemPP
  {
  ppNone           = 0, //Нет проблем
  ppDayOn          = 1, //Горение вне графика
  ppNightOff       = 2  //Негорение по графику
  };

enum TKControllerStatus                             // Состояние одного пункта питания
  {
   csNone           = -1,                            // Еще не был инициирован
   csDisable        = 0,                             // Отключен совсем
   csPowerOff       = 1,                             // Нормальные, питание выключено
   csPowerOn        = 2,                             // Нормальные, включен
   csPowerNight     = 3,                             // Нормальный, включен на ночную фазу
   csNoModem        = 4,                             // Включен на автономном управлении или на каскад
   csLinkNone       = 99,                            // Связь не производилась
   csLinkError      = 100,                           // Одиночная ошибка связи
   csAlarm          = 200                            // Тревога
  };

enum TKParamStatus                             // Состояние одного параметра
  {
   psNone           = -1,                            // Еще не был инициирован
   psOk             = 1,                             // Нормальные, питание выключено
   psError          = 0                              // Тревога
  };

enum TKModeRT
{
	mrtUnknown      = 0,
	mrtDay          = 1,
	mrtNight        = 2,
	mrtEvening      = 3,
	mrtMorning      = 4
};

enum TKSpravStatObjectControlMode
{
	stocmUnknown   = -1, //Непонятный статус
	stocmSelect    = 1,  //Выделение цветом
	stocmSemantic  = 2,  //Изменение семантики
	stocmBoth      = 3   //Оба способа
};


struct TKSpravStatObject
{
   int StatObject;
   UnicodeString StatName;
   TColor StatColor1;
   TColor StatColor2;

   TKSpravStatObjectControlMode PanControlMode;
   UnicodeString                PanStyleSelect;
   int                          PanSemanticNum;
   UnicodeString                PanSemanticValue;
   int                          PanSemanticPKCode;
   int                          PanObjectCode;

};

struct TKParam
{
   int           Num;
   UnicodeString ShortName;
   UnicodeString AsoduName;
   UnicodeString Name;
   double        Value;
   TKParamStatus Status;
   TDateTime     RealTimeDT;
   TDateTime     ChangeDT;
   void          *Controller;
   bool          AlarmExceptionFlag;
   bool          AlarmFlag;
   UnicodeString AlarmText;
   UnicodeString AlarmAlgorithm;
   bool          AlarmEnableFlag;
   bool          AlarmCheckFlag;
   bool          AlarmNowFlag;
   int           IdObject;
   int           AlarmIdObject;
   int           ConstCount;
   int           TimeWithoutCheck;
   TDateTime     ConstDT;
   TDateTime     AlarmDT;
   unsigned short AsoduStatus;
};






/*
struct TKSetOnOffDT
{
	TDateTime  DT;
	TModeRT    Mode;
	TDateTime  DT_Beg;
	TDateTime  DT_End;
	TDateTime  DT_On;
	TDateTime  DT_Off;
	TDateTime  DT_NightOn;
	TDateTime  DT_NightOff;
};
*/



/*
* Дискретные параметры Горизонт
*/

#define	PARAM_SemA   0x0001  //Неисправность семистора фазы A     1
#define	PARAM_SemB   0x0002  //Неисправность семистора фазы B     2
#define	PARAM_SemC   0x0004  //Неисправность семистора фазы C     3
#define	PARAM_PerA   0x0010  //Перегрузка (>200A) фазы A          16
#define	PARAM_PerB   0x0020  //Перегрузка (>200A) фазы B          32
#define	PARAM_PerC   0x0040  //Перегрузка (>200A) фазы C          64
#define	PARAM_KZA    0x0100  //Короткое замыкание (>600A) фазы A  256
#define	PARAM_KZB    0x0200  //Короткое замыкание (>600A) фазы B  512
#define	PARAM_KZC    0x0400  //Короткое замыкание (>600A) фазы C  1024


#define	PARAM_StatA  0x0001 //Фаза A включена
#define	PARAM_StatB  0x0002 //Фаза B включена
#define	PARAM_StatC  0x0004 //Фаза C включена
#define	PARAM_Switch 0x0010 //Тумблер управления 0 - вкл, ручное  16
#define	PARAM_Contrl 0x0020 //Режим управления  0 - ручное        32
#define	PARAM_Door   0x0040 //Дверь             0 - закрыто       64
#define	PARAM_Guard  0x0080 //Охрана            0 - под охраной   128


#define	PARAM_BKS01  0x0001 //Вход БКС 01
#define	PARAM_BKS02  0x0002 //Вход БКС 02
#define	PARAM_BKS03  0x0004 //Вход БКС 03
#define	PARAM_BKS04  0x0008 //Вход БКС 04
#define	PARAM_BKS05  0x0010 //Вход БКС 05
#define	PARAM_BKS06  0x0020 //Вход БКС 06
#define	PARAM_BKS07  0x0040 //Вход БКС 07
#define	PARAM_BKS08  0x0080 //Вход БКС 08
#define	PARAM_BKS09  0x0100 //Вход БКС 09
#define	PARAM_BKS10  0x0200 //Вход БКС 10
#define	PARAM_BKS11  0x0400 //Вход БКС 11
#define	PARAM_BKS12  0x0800 //Вход БКС 12
#define	PARAM_BKS13  0x1000 //Вход БКС 13
#define	PARAM_BKS14  0x2000 //Вход БКС 14
#define	PARAM_BKS15  0x4000 //Вход БКС 15
#define	PARAM_BKS16  0x8000 //Вход БКС 16
#define	PARAM_BKS_FLAG 0x10000 //Взводится, когда активный хоть один БКС

#define	FLG_EXISTS    0x01 //наличие параметра 0-не
#define	FLG_ALARM     0x02 //авария параметра  0-нет
#define	FLG_FIXED     0x04 //фиксирована авария 0-не фикс
#define	FLG_INVALID   0x08 //значение недействительно 0-действит
#define	FLG_SNRLOST   0x10 //потеря связи с датчиком 0- связь норм
#define	FLG_LOOKALARM 0x20 //слежение за аварией 0-не следить


class TKArhiveValueAnalogParam: public TObject
{
   private:
   public:
	  double         Value;
	  unsigned short Status;
	  __fastcall TKArhiveValueAnalogParam();
	  void __fastcall Set( double val, unsigned short stat );
	  void __fastcall CopyTo(TKArhiveValueAnalogParam *val);



};

struct TKAlarmSprav
{
// Справочные парамеры по аварии
	int IdObject;
	UnicodeString Type;
	UnicodeString Script;
	UnicodeString Name;
	UnicodeString ShortName;
	int AutoCheck;
};

struct TKAlarm
{
	TKAlarmSprav *AlarmSprav; //Указатель на справочник тревог
	TKParam *Param;           //Если значение NULL, то новый тип тревог
//	bool isExist;             //Флаг активности тревоги
	bool isEvent;             // Флаг возникновения события
	bool isAlarm;             // Флаг возникновения тревоги
	TDateTime tmEvent;        // Время первого возникновения события
	TDateTime tmAlarm;        // Время первого возникновения тревоги
	int countEvent;           // Количество событий
//	bool isCheck;             //Флаг квитирования тревоги
//	TDateTime tmOn;           //Время возникновения
//	TDateTime tmCheck;        //Время квитирования
//	TDateTime tmOff;          //Время снятия

//	int countPoll;            //Количество опросов с момента возникновения
	UnicodeString Name;
	UnicodeString Type;
};


class TKArchiveValuesParam : public TObject
{
   private:
   public:
	  TKArhiveValueAnalogParam *Ia,*Ib,*Ic;  //Текущие значения токов по каждой фазе
	  TKArhiveValueAnalogParam *Ua,*Ub,*Uc;  //Текущие значения напряжений по каждой фазе
	  bool Link;    //Состояние связи
	  int CrashAll;
	  int DiscretAll;
	  int BKSAll;
	  bool BKSFlagRT; //Прилетели ли параметры БКС
//	  int Control;
//	  int Manual;
	  int DelayOn,DelayOff,DelayNOn,DelayNOff; //Смещения и ночные смещения в мин
	  int Phasa,PhasaN;
	  TKControllerStatus stat,stat_prev;                 //Состояние ПП
	  TKModeRT mode;                           //Текущий режим
	  void *Controller;
	  TObject *Controllers;
	  TKProblemPP Problem;                     //Проблема
	  TDateTime DT,DT_prev;
	  UnicodeString PP; //Используется только в Декодере
	  int Arm;          //Используется только в Декодере
	  int Epran;        //Режим управления горизонт

	  UnicodeString Dump; //Дамп всех параметров горизонт
	  bool DumpFlag;      //Флаг записи всех параметров в дамп
	  __fastcall TKArchiveValuesParam(TObject *cnts);
	  __fastcall ~TKArchiveValuesParam();
	  void __fastcall Clear();
	  bool __fastcall GetParamCrash( int mask );
	  bool __fastcall GetParamDiscret( int mask );
	  bool __fastcall GetParamBKS( int mask );
	  void __fastcall SetParamCrash( int val );
	  UnicodeString __fastcall PhaseCrash( UnicodeString phase );

	  void __fastcall SetParamDiscret( int val );
	  void __fastcall SetParamBKS( int val );
	  void __fastcall CopyTo(TKArchiveValuesParam * val);
	  UnicodeString __fastcall CodeToString(UnicodeString pp,int arm);
	  void __fastcall DecodeFromString(UnicodeString s);
	  void __fastcall DecodeFromString();
	  void __fastcall DecodeFromString(UnicodeString s,bool bd_flag);
	  void __fastcall DecodeFromString(bool bd_flag);
	  void __fastcall GetLastValue();
	  void __fastcall InitValue();
      void __fastcall SetValues(  TKParam *param );
      bool __fastcall CheckBKS(double val);
      int __fastcall  CheckAlarms();
	  bool __fastcall CheckAlarmScript(TKAlarm *alarm);
	  void __fastcall Save( TUniQuery *Query, UnicodeString tab, TDateTime d1 );


	  bool __fastcall IsEnablePhase( UnicodeString phase );
	  bool __fastcall IsEnablePhase( UnicodeString phase, int nphase );
	  UnicodeString __fastcall EnablePhase();
	  UnicodeString __fastcall EnablePhase(int nphase);
	  void __fastcall GetLastValue(TDateTime dt);
	  UnicodeString UdpString;
};


class TKArchiveValuesParamItem: public TCollectionItem
{
private:
	TKArchiveValuesParam *FValue;
public:
	__fastcall TKArchiveValuesParamItem(TCollection *Collection);
	__property TKArchiveValuesParam *Value = {read = FValue, write = FValue};
};

class TKArchiveValuesParamList : public TCollection
{
private:
	TKArchiveValuesParam * __fastcall GetItem(int Index);
	void __fastcall SetItem(int Index,TKArchiveValuesParam * AValue);
public:
	__fastcall TKArchiveValuesParamList();
//	__fastcall ~TKArchiveValuesParamList();
	void __fastcall Clear();
	void __fastcall Add(TKArchiveValuesParam * AValue);
	void __fastcall Delete(int Index);
	__property TKArchiveValuesParam *Items[int Index] = {read = GetItem, write = SetItem};
};


/*
enum TKAlarmType
{
	atNone         = 0,  //Непонят что
	atInfo         = 1,  //Информация
	atAlarm        = 2   //Тревога
};
*/
/**
*
*/



struct TKController
{
   int Arm;
   int Arm1;
   int AsoduIdObject;
   int Object;
   int NetAddr;
   int AsoduActive;
// Параметры из XML и справочника ПП
   int PosX,PosY;  //Координаты ПП в автоформе
// Уставки по токам по каждой фазе
   double IA_min,IA_max,IB_min,IB_max,IC_min,IC_max;
//Уставки напряжений по каждой фазе
   double UA_min,UA_max,UB_min,UB_max,UC_min,UC_max;
   TList *Alarms;

// Параметры из справочника контроллеров
//   UnicodeString AsoduArm;
   UnicodeString AsoduName;
   UnicodeString AsoduAddress;
   UnicodeString AsoduType;

   int AsoduKey;
   bool Enable;
//   int AsoduRepChildCount;
//   UnicodeString AsoduRepiter;
   UnicodeString AsoduTypeModem;
   UnicodeString AsoduTypeController;

   UnicodeString PP;
// Параметры для Уфы ------
   UnicodeString NamePP;
   UnicodeString NameTP;
//-------------------------
   int ppIdObject;
   UnicodeString ppAsoduName;    // тип управления
   UnicodeString ppBalans;      // на балансе
   UnicodeString ppStatus;      // статус
   UnicodeString ppStatusAF;    // Статус в Автоформе
   UnicodeString ppRayon;       // район

   UnicodeString ppAdmRayon;    // административный район
   UnicodeString ppServiceType; // тип договора
   UnicodeString ppServiceName; // кто обслуживает

   UnicodeString ppTextDisp;    // Примечание диспетчера

   UnicodeString ppAddress;     // адрес
   UnicodeString SwitchType;    // тип коммутации
   TObject *MapObject;
   TObject *Component;
   TKParam *Params[MAX_PARAM];
   TKControllerStatus Status;
   TDateTime UpdateDT;
   TDateTime CheckDT;
   TDateTime RealTimeDT;
   TDateTime EventDT;
   int       PollCount;
   bool      IsNewValue;
   bool      IsBKS;            // Наличие БКС (по справочнику ПП)
   UnicodeString BKS;            // Наличие БКС (по справочнику ПП)
   int FiderCount;            // Количество отходящих фидеров (по справочнику ПП)

   TKModeRT Mode;                           //Текущий режим
   TKProblemPP Problem;                     //Проблема

   UnicodeString Graphonoff;
   UnicodeString CodGraphOnOff;

   TKController *Cascade[MAX_CONTROLLER];
   int CountCascade;
   UnicodeString ParentCascade;

   TKArchiveValuesParam *ArchiveValuesParam;

   bool      IsUse;
   bool      IsViewChange;

// Для системы Идель
   TUniQuery *IBQuery;
   int  IntegralNcxema; //Номер схемы
   TKIntegralValue *IntegralValue;


//   int       Count1,Count2,Count3,CountAll;

};

class TKControllers: public TObject
{
private:
    void __fastcall Init();

public:
    TKGlobalSem *Global;
    UnicodeString  TableArchive;

    UnicodeString __fastcall AliasParamName( UnicodeString name );
    bool AlarmCheck;
    bool AlarmException;

   TList *Alarms;
   TKLog *Log;
   TKController *Controllers[MAX_CONTROLLER];
   __fastcall TKControllers();
   __fastcall TKControllers(TKGlobalSem *glob);
// Инициализация списка
   void __fastcall MakeTreeLayers( TAdvStringGrid *asg );

   void __fastcall InitControllersFromXml(UnicodeString file,TKGlobalSem *glob,int arm);
   void __fastcall InitControllersAndParams(TUniQuery *Sql1, int arm);
   void __fastcall InitControllers(TUniQuery *Sql1);
   void __fastcall InitControllers(TUniQuery *Sql1,UnicodeString order_by);

   void __fastcall InitProperties( TKGlobalSem *glob );

   void __fastcall CheckException(TUniQuery *Sql1);
   void __fastcall CheckAlarm(TUniQuery *Sql1);

   void __fastcall LinkToPP(TKGlobalSem *glob);
   void __fastcall CreateNewPP(TKGlobalSem *glob);
   void __fastcall SaveControllers(TKGlobalSem *glob);
   void __fastcall LinkToAlarm(TUniQuery *Sql1);

   void __fastcall SetPropertyForm(TForm *form, UnicodeString section, UnicodeString suffix);

   void __fastcall SortByPP(bool dir);
   void __fastcall SortByXY(bool dir);

   TKController * __fastcall FindByPP( UnicodeString pp );
   TKController * __fastcall FindByIdPP( int id_pp );
   TKParam * __fastcall FindByPPAndParam( TKController *cnt, UnicodeString par );

   TList *ListStatPP;
   void __fastcall InitListStatPP();
   TKSpravStatObject * __fastcall GetStatPP( int stat );


   bool __fastcall GetShedule( TUniQuery *Sql1,TDateTime dt, TDateTime *t_on, TDateTime *t_off );
   bool __fastcall GetShedule( TUniQuery *Sql1,TDateTime dt, TDateTime *t_off1, TDateTime *t_on2, TDateTime *t_off2 );
   UnicodeString __fastcall GetDispetcher( TUniQuery *Sql, TDateTime dt );
   int __fastcall  HowManyPhases( TKController *cnt, TKModeRT mode );
   int __fastcall  HowManyPhases( UnicodeString cod, TKModeRT mode );
   void __fastcall SetCascades();
   UnicodeString __fastcall ReadProperty(UnicodeString section, UnicodeString name, UnicodeString value);
   TDateTime __fastcall ReadPropertyDateTime(UnicodeString section, UnicodeString name, TDateTime value);
   int __fastcall ReadPropertyInt(UnicodeString section, UnicodeString name, int value);
   TKModeRT __fastcall CurrentModeRT(TDateTime d);
   TKModeRT __fastcall CurrentModeRT(TDateTime d,int d_on,int d_off,int d_on1,int d_off1);

   void __fastcall InitValues();

//   void __fastcall SetOnOff(TUniQuery *Sql1,TDateTime d, TKSetOnOffDT *set);

//   void __fastcall SortByCount();
   int Arm;
   UnicodeString ArmName;
   int Count;
   TDateTime TimeEveningToNight;
   TDateTime TimeNightToMorning;
   TDateTime t_off_prev,t_on_now,t_off_now;
   TKModeRT ModeRT;
// Флаг для отслеживания изменения по конфигурации (при старте)
   bool IsConfigChange;
// Флаг ждля отслеживания изменения по тревогам
   bool IsAlarmChange;
//  TKUdpControl *UdpControl;
   bool IsDump; //Флаг записи всех параметров PollService в поле arh_dump

   void __fastcall InitIntegral(TUniConnection *conn, UnicodeString suffix);


};
/*
class TKChart: public TChart
{
private:
//   TUniQuery *Sql;
   TChart *Chart;
public:
   __fastcall TKChart( TChart *chart );
   TKFloatSeries * __fastcall QuerySeries(TUniQuery *sql, UnicodeString dt_field,UnicodeString val_field);
   void __fastcall Clear();

};
*/

/**
* Структура одной записи в числовом ряду
*/
struct TKFloatSeriesItem
{
   double    Value;
   double    Z;
   bool      Status;
   TDateTime DT;
};

/**
* Класс работы с числовым рядом
*/
class TKFloatSeries: public TList
{
   private:
   public:
	  __fastcall TKFloatSeries();
	  __fastcall ~TKFloatSeries();

	  void __fastcall ClearValues();
	  void __fastcall Statistic();
	  void __fastcall Add( TKFloatSeriesItem *Item );
	  void __fastcall Add( double val, TDateTime dt );
	  void __fastcall MakeFromList( TStringList *sl );

	  void __fastcall Filter( TDateTime dt_min, TDateTime dt_max, bool flag);
	  void __fastcall Filter( TDateTime dt_min, TDateTime dt_max);
	  void __fastcall Filter( double min, double max);
	  void __fastcall FilterZ( double zmin, double zmax);
	  void __fastcall InverceFilter();
	  void __fastcall ClearFilter();


	 double Dispersion; //Дисперсия
	 double Deviation;  //Средне-квадратичное отклонение
	 double Min;        //Минимальное значение
	 double Max;        //Максимальное значение
	 double Avg;        //Среднее значение
	 bool StatisticFlag;
	 int CountCorrect;

	 TDateTime TimeOn,TimeOff;
};

 // видимость графиков

#define	Graf_IA   0x0001  //График тока фазы А
#define	Graf_IB   0x0002  //График тока фазы B
#define	Graf_IC   0x0004  //График тока фазы C
#define	Graf_UA   0x0010  //График напряжения фазы A
#define	Graf_UB   0x0020  //График напряжения фазы B
#define	Graf_UC   0x0040  //График напряжения фазы C
#define	Graf_AV   0x0100  //График аварий
#define	Graf_SV   0x0200  //График связи
#define	Graf_GR   0x0400  //График границ тока и напряжения
#define	Graf_TVO  0x0008  //Точки вкл-откл
#define	Graf_TZN  0x0080  //Точки значений
#define	Graf_OnOff  0x0800  //Точки горение/негорение
#define	Graf_AA  0x1000  //График тревог фазы А
#define	Graf_AB  0x2000  //График тревог фазы В
#define	Graf_AC  0x4000  //График тревог фазы С


class TKGrafIU: public TObject
{
   private:
	TAdvStringGrid *AdvStringGrid;
	TChart *Chart;
	TAreaSeries *Series4;
	TLineSeries *Series1;
	TLineSeries *Series2;
	TLineSeries *Series3;
	TLineSeries *Series17;
	TLineSeries *Series18;
	TLineSeries *Series21;
	TLineSeries *Series20;
	TLineSeries *Series22;
	TLineSeries *Series23;
	TLineSeries *Series24;
	TLineSeries *Series25;
	TLineSeries *Series26;
	TLineSeries *Series27;
	TLineSeries *Series28;
	TLineSeries *Series29;
	TLineSeries *Series30;
	TLineSeries *Series31;

	TLineSeries *Series36;
	TLineSeries *Series37;
	TLineSeries *Series38;
	TLineSeries *Series39;
	TLineSeries *Series40;
	TLineSeries *Series41;
   /*	TAreaSeries *Series36;
	TAreaSeries *Series37;
	TAreaSeries *Series38;
	TAreaSeries *Series39;
	TAreaSeries *Series40;
	TAreaSeries *Series41;  */

	TLineSeries *Series7;
	//TGanttSeries *Series7;
	TLineSeries *Series6;
	TAreaSeries *Series5;
	TBubbleSeries *Series9;
	TBubbleSeries *Series10;
	TBubbleSeries *Series11;
	TBubbleSeries *Series12;
	TBubbleSeries *Series13;
	TBubbleSeries *Series15;
	TBubbleSeries *Series14;
	TBubbleSeries *Series16;
   	TAreaSeries *Series8;
	TLineSeries *Series19;
	TBarSeries *Series32;

	TGanttSeries *Series33;
	TGanttSeries *Series34;
	TGanttSeries *Series35;
	void __fastcall GrafPaintClear();
	void __fastcall GrafPaintToki();
	void __fastcall GrafPaintSutki();
	void __fastcall GrafPaintTochki();
	void __fastcall GrafPaintNedel();
	TColor clIa,clIb,clIc,clSv,clPhase44,clAl,clDay,clNight,clEvening,clUa,clUb,clUc,clGor;
	TKGlobalSem *Global1;
	TDateTime Shift;
	UnicodeString punkt,night1,night2,morning;
	TDateTime DateOt,DateDo;
	TUniQuery *UniQuery1;
	TUniQuery *UniQuery2;
	TUniQuery *UniQuery3;
	TKControllers *Controllers;
	TKModeRT mod;

	int WAl;
	bool sutki;
	bool normbd;
	bool kza,kzb,kzc,pera,perb,perc,sima,simb,simc,door,guard,swith,contr;
	UnicodeString CodOnOff;
	void __fastcall ChartClickSeries(TCustomChart *Sender, TChartSeries *Series, int ValueIndex,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	double ia_max_d, ia_min_d, ia_nom_d, ua_nom_d;   // фаза а
	double ia_max_n, ia_min_n, ia_nom_n, ua_nom_n;
	double ia_max_u, ia_min_u, ia_nom_u, ua_nom_u;
	double ia_max_v, ia_min_v, ia_nom_v, ua_nom_v;

	double ib_max_d, ib_min_d, ib_nom_d, ub_nom_d;   // фаза b
	double ib_max_n, ib_min_n, ib_nom_n, ub_nom_n;
	double ib_max_u, ib_min_u, ib_nom_u, ub_nom_u;
	double ib_max_v, ib_min_v, ib_nom_v, ub_nom_v;

	double ic_max_d, ic_min_d, ic_nom_d, uc_nom_d;   // фаза c
	double ic_max_n, ic_min_n, ic_nom_n, uc_nom_n;
	double ic_max_u, ic_min_u, ic_nom_u, uc_nom_u;
	double ic_max_v, ic_min_v, ic_nom_v, uc_nom_v;

	TKJournalSem *Journal;

   public:
	__fastcall TKGrafIU(TAdvStringGrid *AdvStringGrid1, TChart *Chart1,TKGlobalSem *Global);
	void __fastcall GrafPaint(UnicodeString pp, UnicodeString Date1, UnicodeString Date2,bool sutki);
	void __fastcall GrafPaint(UnicodeString pp, UnicodeString Date1, UnicodeString Date2);
	void __fastcall GrafPaintNorm(bool norm);
	void __fastcall GrafCalcNorm(double delta, double procent);
	void __fastcall GrafSaveNorm(TDateTime DateSave);
	void __fastcall GrafPaintAlarm(UnicodeString pp, UnicodeString Date1, UnicodeString Date2);
	void __fastcall GrafPaintParam(UnicodeString pp, UnicodeString Date1, UnicodeString Date2, int param);
	void __fastcall GrafVisible(int DiscretGrafVisible);
	void __fastcall GrafToExcel();
	int DiscretGrafVisible;
	TStringList *Alarm;
};




#endif