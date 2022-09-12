//---------------------------------------------------------------------------
#ifndef TextDataSetH
#define TextDataSetH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <Db.hpp>
//---------------------------------------------------------------------------
#define  STRFLDSIZ      512                       // Мах длина строкового поля
#define  DELIMS    "sbift"                       // Префиксы типов данных
#define  FLDDLM        '|'                       // Разделитель полей
#define  OKCOUNT         3                       // Критерий общности
#define  MAXCHANGES     10                       // max % не сохран-ых изменений
#define  KSDLM         ' '                       // Отделитель контрольной суммы
#define  KSLEN           4                       // Длина поля контрольной суммы
//---------------------------------------------------------------------------
/// Метка типа поля в текстовом файле. Ставится после разделителя перед значением поля.
enum TFieldMark                                  // Макркеры типов полей
  {
   fmStr   = 's',                                // Строковый    : ftString
   fmBool  = 'b',                                // Логический   : ftBoolean
   fmInt   = 'i',                                // Целый        : ftInteger
   fmFloat = 'f',                                // Вещественный : ftFloat
   fmTime  = 't',                                 // Дата\Время   : ftDataTime
   fmOnlyTime = 'o'                              // Время без даты  ftTime 
  };
//---------------------------------------------------------------------------
struct TRecMark                                  // Закладка
  {
   int                    Bookmark;              // Номер закладки
   TBookmarkFlag          BookmarkFlag;          // Флаг  закладки
  };
//---------------------------------------------------------------------------
struct TFieldParser                              // Парсер поля
  {
   TFieldType             FieldType;             // Тип поля
   char                   FieldMark;             // Маркер поля
   Word                   FieldSize;             // Размер поля (кол-во данных)
   Word                   DataSize;              // Размер физич-х данных поля

               __fastcall TFieldParser();        // Конструктор

   // Преобразования Метка_Поля <-> Тип_Поля
   static void __fastcall MarkToFieldType(char Mark, TFieldType & Type);
   static bool __fastcall FieldTypeToMark(TFieldType Type, char & Mark);

   // Задание структурных данных поля по его типу или маркеру
   bool        __fastcall SetFieldType(TFieldType Type);
   bool        __fastcall SetFieldMark(char       Mark);

   // Преобразования Внутреннее_Представление <-> Строковое_Представление
   bool        __fastcall BufferToString(char * Buf, AnsiString & Str);
   bool        __fastcall StringToBuffer(AnsiString & Str, char * Buf);
  };
//---------------------------------------------------------------------------
struct TRecordParser                             // Парсер записи таблицы
  {
   Word                   FieldCount ;           // Количество полей
   AnsiString             FieldMarks ;           // Строка описания полей
   Word                 * FieldOffs;             // Карта размещения полей
   Word                   BufferSize ;           // Размер буфера под запись

               __fastcall TRecordParser();       // Конструктор

   // Функции проверки корректности строки записи
   static char __fastcall AccountCtrlSum(AnsiString Str, byte Len);
   static bool __fastcall ValidateString(AnsiString Str);

   AnsiString  __fastcall GetEmptyString();      // Строка с пустыми полями

   // Функции создания\импорта\экспорта структуры записи таблицы
   void        __fastcall ClearStructure ();    // Очистка
   void        __fastcall BuildStructure ();    // Построение по строке маркеров
   void        __fastcall CheckStructure ();    // Проверка согласован-ти данных
   void        __fastcall ParseStructure (AnsiString Str);       // Парсинг
   void        __fastcall ImportStructure(TFieldDefs * FldDefs); // Импорт
   void        __fastcall ExportStructure(TFieldDefs * FldDefs); // Экспорт

   // Преобразования Внутреннее_Представление <-> Строковое_Представление
   bool        __fastcall BufferToString(char * Buf, AnsiString & Str);
   bool        __fastcall StringToBuffer(AnsiString & Str, char * Buf);
  };
//---------------------------------------------------------------------------
class PACKAGE TTextTable : public TDataSet       // Таблица аля текстовый файл
{
private:
    TStringList            * FData;            // Список записей
    TRecordParser            FRecStruct;       // Структура записи
    int                      FCurRec;          // Номер текущей записи
    int                      FRecBufSize;      // Размер буфера запись+закладк
    int                      FLastBookmark;    // Номер последней закладки
    AnsiString               FTableName;       // Имя файла таблицы
    int                      FChangesCount;    // Кол-во не сохран-х изменений
    volatile long            IsLocked;
    CRITICAL_SECTION         cs_TextTable;

protected:
    // Функции работы с закладками
    TBookmarkFlag __fastcall GetBookmarkFlag(char * Buffer);
    void          __fastcall SetBookmarkFlag(char * Buffer, TBookmarkFlag Value);
    void          __fastcall GetBookmarkData(char * Buffer, void * Data);
    void          __fastcall SetBookmarkData(char * Buffer, void * Data);

    // Функции работы с памятью и навигации по ней
    char *        __fastcall AllocRecordBuffer1);
    void          __fastcall FreeRecordBuffer (char * & Buffer);
    WORD          __fastcall GetRecordSize    ();
    int           __fastcall GetRecordCount   ();
    int           __fastcall GetRecNo         ();
    void          __fastcall SetRecNo         (int Value);
    TGetResult    __fastcall GetRecord        (char * Buffer, TGetMode GetMode, bool DoCheck);

    // Реализация сущьности таблицы аля текстовый файл
    void          __fastcall InternalOpen           ();
    void          __fastcall InternalClose          ();
    void          __fastcall InternalPost           ();
    void          __fastcall InternalInitFieldDefs  ();
    bool          __fastcall IsCursorOpen           ();
    void          __fastcall InternalAddRecord      (void * Buffer, bool Append);
    void          __fastcall InternalInitRecord     (char * Buffer);
    void          __fastcall InternalDelete         ();
    void          __fastcall InternalGotoBookmark   (void * Bookmark);
    void          __fastcall InternalSetToRecord    (char * Buffer);
    void          __fastcall InternalFirst          ();
    void          __fastcall InternalLast           ();
    void          __fastcall InternalHandleException();

    /// Посчитать контрольную сумму для всей записи и вставить её в запись.
    // @param fieldRecord Законченная запись, содержащая значение всех полей без контрольной суммы.
    static AnsiString __fastcall insertCtrlSumToFiledRecordString(AnsiString strRecord);

    /// Получить метку типа поля.
    // @param fieldType Тип поля.
    static TFieldMark __fastcall getFieldMark(TFieldType fieldType);

    /// Получить строковое представление значения ПОЛЯ.
    // @param fieldType Тип поля.
    // @param recordBuf Буфера с данными. Структура данных и размер зависят от fieldType.
    static AnsiString __fastcall getRecordStr(TFieldType fieldType, char *recordBuf);

    /// Получить строковое представление значения ПОЛЯ.
    // @param value Значение.
    static AnsiString __fastcall getRecordStr(TDateTime value);

    /// Получить строковое представление значения ПОЛЯ.
    // @param value Значение.
    static AnsiString __fastcall getRecordStr(double value);

    /// Получить строковое представление значения ПОЛЯ.
    // @param value Значение.
    static AnsiString __fastcall getRecordStr(int value);

    /// Записать запись в файл.
    // @param fileName Имя файла.
    // @param recordStr Строковое представление записи.
    // @return true если успешно, false если ошибка.
    static bool __fastcall writeRecordToFile(AnsiString fileName, AnsiString recordStr);

public:

      TNotifyEvent              OnNullStructure;               // Нет структуры

                    __fastcall  TTextTable(TComponent* Owner); // Конструктор
                    __fastcall  ~TTextTable();


      // Функции работы с внутренним представлением полей
      void  __fastcall SetFieldData(TField * Field, void * Buffer);
      bool  __fastcall GetFieldData(TField * Field, void * Buffer);

      // Функции работы с файлом таблицы
      int           __fastcall LoadFromFile(AnsiString Name);  // Загрузка из
      int           __fastcall SaveIntoFile(AnsiString Name);  // Запись в

      bool                     appendToFile(AnsiString fileName);

      // Косим под стандартную таблицу TTable
      void          __fastcall CreateTable();                  // Создание пустой
      void          __fastcall DeleteTable();                  // Удаление
      void          __fastcall RenameTable(AnsiString NewName);// Переименование

      void          __fastcall Lock();
      void          __fastcall Unlock();
      int           __fastcall GetRecCount();

    /// Добавить запись в конец таблицы.
    // @param fileName Файл таблицы.
    // @param time Поле 1: время.
    // @param val Поле 2: значение.
    // @return true если успешно, false если не удалось открыть файл.
    static bool appendValToTableFile(AnsiString fileName, TDateTime time, double val);

    /// То же, что и TTextTable::appendValToTableFile(AnsiString fileName, TDateTime time, double val)
    static bool appendValToTableFile(AnsiString fileName, TDateTime time, int val);

__published:

      __property AnsiString    TableName = {read = FTableName, write = FTableName};
      __property               Active;
  };
//---------------------------------------------------------------------------
#endif
