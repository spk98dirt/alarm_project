//---------------------------------------------------------------------------
#ifndef SemClassH
#define SemClassH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "Uni.hpp"
#include <Data.DB.hpp>
#include <inifiles.hpp>
#include <System.DateUtils.hpp>

//Генераторы по таблицам
#define GEN_PARAM_VALUE           "SEQ_PARAM_VALUE"
#define GEN_OBJECTS               "SEQ_OBJECTS"
#define GEN_LOG_VALUE             "SEQ_LOG_VALUE"
#define GEN_OTHER_TABLE           "SEQ_OTHER_TABLE"
#define GEN_LINE_NUMBER           "SEQ_LINE_NUMBER"
#define GEN_LINES                 "SEQ_LINES"
#define GEN_PARAM                 "SEQ_PARAM"
#define GEN_TREE_UNIT             "SEQ_TREE_UNIT"
#define GEN_FORMS                 "SEQ_FORMS"
#define GEN_MAGAZINE              "SEQ_MAGAZINE"
#define GEN_PARAM_JOURNAL         "SEQ_PARAM_IN_MAGAZINE"
#define GEN_PARAM_FORM            "SEQ_PARAM_IN_FORM"
#define GEN_BANNER                "SEQ_BANNER_IN_FORM"
#define GEN_SCRIPT                "SEQ_SCRIPT"
#define GEN_REPORT                "SEQ_REPORT"
#define GEN_FILTER                "SEQ_FILTER"
#define GEN_CONDITION             "SEQ_CONDITION"
#define GEN_GRAND_USER            "SEQ_GRAND_USER"
#define GEN_GRAND_GROUP           "SEQ_GRAND_GROUP"
#define GEN_TREE_SCRIPT           "SEQ_TREE_SCRIPT"
#define GEN_NORM_PARAM            "SEQ_NORM_PARAM"
#define GEN_AUTO_INCR             "SEQ_AUTONUMBER"


//#include <vector>

//using namespace std;
struct TKtatisricItem
{
	AnsiString Name;
	TDateTime  TimeFirst;
	TDateTime  TimeBegin;
	TDateTime  TimeEnd;
	int LoopCount;
	int MinTime;
	int MaxTime;
	int AvgTime;
	int SumTime;
	int ExeTime;
	bool Flag;
};


class TKPerfStatistic: public TObject
{
private:
   TList *List;
   TKtatisricItem * __fastcall FindOrCreate( UnicodeString name);

public:
   __fastcall TKPerfStatistic();
   __fastcall ~TKPerfStatistic();
   void __fastcall Begin( UnicodeString name );
   void __fastcall Commit( UnicodeString name );
   void __fastcall Rollback( UnicodeString name );
   void __fastcall Print( UnicodeString name );


};


enum TKParamSemDataType
{
	psdtUnknown  =  0, //Неизвестный тип
	psdtFloat    =  1, //Число с десятичной точкой (double)
	psdtString   =  2, //Строка
	psdtDateTime =  3, //TDateTime
	psdtInteger  =  4, //Целое число
	psdtBlob     =  5, //Блоб объект
	psdtBoolean  =  6  //Тип bool
};

enum TKQueryType
{
	qtInsert     =  1,
	qtDelete     =  2,
	qtUpdate     =  3
};


enum TKParamSemType
{
	pstNormal     = 1,            //Обычный параметр вводимый вручную
	pstImport     = 2,            //Импортируемый (не используется)
	pstCalculate  = 3,            //Расчитываемые
	pstSprav      = 4,            //Справочный
	pstConstant   = 5,            //Константа (не используется)
	pstNorm       = 6,            //Норматиынй
	pstEval       = 7,            //Вычисляемый (не понятно, чем отличается от расчитываемого)
	pstPK         = 9,            //Первичный ключ (определяющий сущность)
	pstSpravMulti = 14,           //Справочный с мультивыбором
	pstUser       = 17            // Login пользователя
};

enum TKParamSemJournalType
{
	psjtManual   = 31,     //Ручной ввод
	psjtLink     = 33      //Параметр связи с другим журналом FK (Другая сущность)
};


class TKParamSem: public TObject
{
private:
public:
// Журнал которому принадлежит параметр
	TObject               *Journal;
	int                   Num;
	UnicodeString         Key;
	UnicodeString         Name;
	TKParamSemDataType    DataType;
	TKParamSemJournalType JournalType;
	TKParamSemType        Type;
	UnicodeString         FieldValue;
	UnicodeString         FieldObject;
	UnicodeString         Format;
	int                   Round;
	__fastcall TKParamSem(TObject *journal);
};

class TKParamSemItem: public TCollectionItem
{
private:
	TKParamSem *FValue;
public:
	__fastcall TKParamSemItem(TCollection *Collection);
	__property TKParamSem *Value = {read = FValue, write = FValue};
};

class TKParamSemList : public TCollection
{
private:
	TKParamSem * __fastcall GetItem(int Index);
	void __fastcall SetItem(int Index,TKParamSem * AValue);
public:
	__fastcall TKParamSemList();
	void __fastcall Add(TKParamSem * AValue);
	void __fastcall Delete(int Index);
	__property TKParamSem *Items[int Index] = {read = GetItem, write = SetItem};
	TKParamSem * __fastcall FindByKey( UnicodeString key );
};


class TKValueSem: public TObject
{
private:
public:
	TKParamSemDataType Type;
	int           IdObject;
	Variant       AsVariant;
	UnicodeString AsString;
	double        AsFloat;
	int           AsInteger;
	TDateTime     AsDateTime;
	TDateTime     AsTime;
	TDateTime     AsDate;
	__fastcall TKValueSem();
	void __fastcall Set(int val);
	void __fastcall Set(double val);
	void __fastcall Set(TDateTime val);
	void __fastcall Set(UnicodeString val);
};

class TKJournalSem: public TObject
{
private:
	TObject      *Global;
// Список дополнительных журналов, соединенных с
public:
// Список параметров журналов
	TKParamSemList *Params;
	TKParamSem     *ParamPK;
	TKQueryType    QueryType;
	TStringList    *QueryParams;
	void __fastcall AddParamAnyType(int n,TKValueSem *val);
	int           QueryParamCount;
	int           Num;
	UnicodeString Key;
	UnicodeString Name;
	UnicodeString TableName;
	TUniQuery     *Sql;

	int numLine;   //Значение ключей послк Insert
	int orderLine; //Значение ключей послк Insert
	int numObject; //Значение ключей послк Insert

	__fastcall TKJournalSem( UnicodeString key, TObject *global );
// Добавляем журнал
	UnicodeString __fastcall KeyNameToFieldName( UnicodeString key );
	void __fastcall QueryClear();
	int __fastcall QueryInsert();
	void __fastcall QueryUpdate(int obj);
	bool __fastcall AddParamQuery( UnicodeString key );
	void __fastcall AddParamQuery( UnicodeString key, int val );
	void __fastcall AddParamQuery( UnicodeString key, double val );
	void __fastcall AddParamQuery( UnicodeString key, TDateTime val );
	void __fastcall AddParamQuery( UnicodeString key, UnicodeString val );
	void __fastcall ExecQuery();

//	void __fastcall QueryClose();
//	void __fastcall QueryStart();
//	void __fastcall QueryJoinJournal(TKJournalSem *j,UnicodeString key);
//	void __fastcall QuertAddCondition( UnicodeString str )
//	void __fastcall QueryJoin( UnicodeString key );
//	void __fastcall Query
};

class TKJournalSemItem: public TCollectionItem
{
private:
	TKJournalSem *FValue;
public:
	__fastcall TKJournalSemItem(TCollection *Collection);
	__property TKJournalSem *Value = {read = FValue, write = FValue};
};

class TKJournalSemList : public TCollection
{
private:
	TKJournalSem * __fastcall GetItem(int Index);
	void __fastcall SetItem(int Index,TKJournalSem * AValue);
public:
	__fastcall TKJournalSemList();
	void __fastcall Add(TKJournalSem * AValue);
	void __fastcall Delete(int Index);
	__property TKJournalSem *Items[int Index] = {read = GetItem, write = SetItem};
	TKJournalSem * __fastcall FindByKey( UnicodeString key );
};

class TKGlobalSem: public TObject
{
private:
	int count_digit;
	UnicodeString Mas[10];
	UnicodeString MasCh[10];
	UnicodeString MasD[10];
// Изврат с паролем из старого SEMа для обратной совместимости
	UnicodeString __fastcall DecodeStr(UnicodeString str);
	UnicodeString __fastcall ReverseDigit(UnicodeString str);
	UnicodeString __fastcall ReverseChar(UnicodeString str);
	UnicodeString __fastcall FullN(int a,int n);
	UnicodeString __fastcall ReadProperty(UnicodeString Selection, UnicodeString name, UnicodeString value);
//-------------------------------------------------------------

public:
    TKPerfStatistic *Statistic;
	TKJournalSemList *Journals;
	TUniConnection *Connection;
	TUniQuery      *Query;
	__fastcall TKGlobalSem( TUniConnection *Connection );
    int __fastcall AutoNum(  AnsiString seq );
	bool __fastcall CheckArg(UnicodeString str);
	UnicodeString __fastcall GetArg(UnicodeString str);
// Создание/удаление/проверка таблиц/последовательностей/индексов
	bool __fastcall CheckOfExistsTable( UnicodeString tab_name );
	bool __fastcall CheckOfExistsIndex( UnicodeString index_name );
	bool __fastcall CheckOfExistsColumn( UnicodeString tab_name,UnicodeString col_name );
	bool __fastcall CheckOfExistsSequence( UnicodeString seq_name );
	bool __fastcall CreateTable( UnicodeString tab_name, TStringList *fields );
	bool __fastcall CreateSequence( UnicodeString seq_name );
	bool __fastcall CreateIndex( UnicodeString tab_name,UnicodeString field_name, UnicodeString index_name,bool is_unique );
	bool __fastcall CreateIndex( UnicodeString tab_name,UnicodeString field_name,bool is_unique );
	bool __fastcall AddColumnToTable( UnicodeString tab_name,UnicodeString col_name,UnicodeString col_str);
	bool __fastcall CheckConnect();
	void __fastcall SetProperties(UnicodeString Selection, TForm *Form1);
	UnicodeString UserSem,PasswordSem;
	TMemIniFile *IniFile;
//	void __fastcall
};





#endif
