//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include <ctype.h>                               // Работа с символами
#include <stdlib.h>                              // Работа с памятью
#include <sysutils.hpp>                          // Работа с датой\временем       "
#include "TextTable.h"                           // Собственный головник
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
#pragma option -w-sig  // Отключает : Conversion may lose significant digits.

// ============================== TFieldParser ==============================

__fastcall TFieldParser::TFieldParser() { SetFieldType(ftString); }
//---------------------------------------------------------------------------
void __fastcall TFieldParser::MarkToFieldType(char Mark, TFieldType & Type)
  {
   char         LowCaseMark;                     // Маркер в нижнем регистре

   LowCaseMark = (char)tolower( (int)Mark );     // Переводим в нижний регистр
   switch ( LowCaseMark )                        // Маркер поля в тип поля
     {
      case fmBool  : Type = ftBoolean ; break;   // Логический
      case fmInt   : Type = ftInteger ; break;   // Целый
      case fmFloat : Type = ftFloat   ; break;   // Вещественный
      case fmTime  : Type = ftDateTime; break;   // Дата\Время
      default      : Type = ftString  ;          // Всё остальное - строковый
     }
  }
//---------------------------------------------------------------------------
bool __fastcall TFieldParser::FieldTypeToMark(TFieldType Type, char & Mark)
  {
   bool         KnownType = true;                // Флаг неизвестного типа

   switch ( Type )                               // Что за тип поля
     {
      case ftString   : Mark = fmStr  ; break;   // Строковый
      case ftBoolean  : Mark = fmBool ; break;   // Логический
      case ftInteger  : Mark = fmInt  ; break;   // Целый
      case ftFloat    : Mark = fmFloat; break;   // Вещественный
      case ftDateTime : Mark = fmTime ; break;   // Дата\Время
      default         : KnownType = false;       // Незнаем такого типа (пока)
     }
   return KnownType;                             // Извещаем о поддержке типа
  }
//---------------------------------------------------------------------------
bool __fastcall TFieldParser::SetFieldType(TFieldType Type)
  {
   FieldType = Type;                             // Устанавливаем тип
   switch ( Type )                               // По типу задаём размеры
     {
      case ftString   : FieldSize = STRFLDSIZ; DataSize = STRFLDSIZ+1; break; //
      case ftBoolean  : FieldSize =         0; DataSize =           2; break; //
      case ftInteger  : FieldSize =         0; DataSize =           4; break; //
      case ftFloat    :                                                       //
      case ftDateTime : FieldSize =         0; DataSize =           8; break; //
     }
   return FieldTypeToMark(Type,FieldMark);       // Маркер + известность типа
  }
//---------------------------------------------------------------------------
bool __fastcall TFieldParser::SetFieldMark(char Mark)
  {
   TFieldType   Type;                            // Тип поля

   MarkToFieldType(Mark,Type);                   // Маркер поля в тип поля
   return  SetFieldType(Type);                   // Задаём данные по типу
  }
//---------------------------------------------------------------------------
bool __fastcall TFieldParser::BufferToString(char * Buf, AnsiString & Str)
  {
   bool         Success = true;                  // Успех преобразования
   WORD       * bData;                           // Логические данные
   long       * iData;                           // Целые данные
   double     * dData;                           // Вещественные и Дата\Время
   TDateTime    DateTime;                        // Одно представ-ие Даты\Время
   TTimeStamp   TimeStamp;                       // Др.  представ-ие Даты\Время

   if ( Buf == NULL ) return false;              // Нет буфера - нечего преобраз
   try
     {
      Str = "";                                  // Очищаем строку
      switch ( FieldType )                       // Соответственно типу поля
        {
         case ftBoolean  : {  bData = (WORD*)(Buf);
                              if ( *bData ) Str = "True "; else Str = "False";
                           }; break;
		 case ftInteger  : {  iData = (  long*)(Buf); Str = IntToStr  ((int)*iData);
						   }; break;
         case ftFloat    : {  dData = (double*)(Buf); Str = FloatToStr(*dData);
                           }; break;
         case ftDateTime : {  dData = (double*)(Buf);
                              TimeStamp.Date = int(*dData/86400000.0);
                              TimeStamp.Time = int(*dData - TimeStamp.Date*86400000.0);
                              DateTime = TimeStampToDateTime(TimeStamp);
                              DateSeparator =  '/'; TimeSeparator =  ':';
                              ShortDateFormat = "dd/mm/yyyy";
                              LongTimeFormat  = "hh:mm:ss";
                              Str = DateTimeToStr(DateTime);
                           }; break;
         case ftString   : {  Str = AnsiString(Buf);
                           }; break;
         default         :    Success = false;   // Неизвестный тип
        };
     }
   catch ( ... ) { Success = false; }            // Ошибка преобразования

   return Success;                               // Вернуть успешно либо нет
  }
//---------------------------------------------------------------------------
bool __fastcall TFieldParser::StringToBuffer(AnsiString & Str, char * Buf)
  {
   bool         Success = true;                  // Успех преобразования
   WORD       * bData;                           // Логические данные
   long       * iData;                           // Целые данные
   double     * dData;                           // Вещественные и Дата\Время
   int          DecPos;                          // Положение десятичной точки
   char         WrongDlm;                        // Неправил. десят. разделитель
   TDateTime    DateTime;                        // Одно представ-ие Даты\Время
   TTimeStamp   TimeStamp;                       // Др.  представ-ие Даты\Время

   if ( Buf == NULL   ) return false;            // Нет буфера - некуда преобраз
   if ( Str.IsEmpty() ) return false;            // Нечего преобразовывать

   try
     {
      if (FieldType!=ftString) Str = Str.Trim(); // Отбросить пробелы
      switch ( FieldType )                       // Соответственно типу поля
        {
         case ftBoolean  : {  bData = (WORD*)Buf; Str = Str.LowerCase();
                             if ( Str.Pos('t') ) *bData = 1; else *bData = 0;
                           }; break;
         case ftInteger  : {  iData  = (long*)Buf; *iData = StrToInt(Str);
                           }; break;
         case ftFloat    : {  dData  = (double*)Buf;
                              if (DecimalSeparator == ',') WrongDlm = '.';
                              else                         WrongDlm = ',';
                              DecPos = Str.Pos(WrongDlm);
                              if ( DecPos ) Str[DecPos] = DecimalSeparator;
                              *dData = StrToFloat(Str);
                           }; break;
         case ftDateTime : {  dData = (double*)Buf;
                              DateSeparator =  '/'; TimeSeparator =  ':';
                              ShortDateFormat = "dd/mm/yyyy";
                              LongTimeFormat  = "hh:mm:ss";
                              DateTime = StrToDateTime(Str);
                              TimeStamp = DateTimeToTimeStamp(DateTime);
                              *dData = double(TimeStamp.Date)*86400000.0+double(TimeStamp.Time);
                           }; break;
         case ftString   : {  if (Str.Length()>STRFLDSIZ) Str.SetLength(STRFLDSIZ);
                              StrLCopy(Buf,Str.c_str(),Str.Length());
                           }; break;
         default         :    Success = false;   // Неизвестный тип поля
        };
     }
   catch ( ... ) { Success = false; }            // Ошибка преобразования

   return Success;                               // Вернуть успешно или нет
  }

// ============================= TRecordParser ==============================

__fastcall TRecordParser::TRecordParser() { ClearStructure(); }
//---------------------------------------------------------------------------
char __fastcall TRecordParser::AccountCtrlSum(AnsiString Str, byte Len)
  {
   byte         CtrlLen;                         // Правильная длина
   byte         CtrlSum;                         // Контрольная сумма

   // Праверка валидности указанной длины (должна быть не длиньше самой строки)
   CtrlLen = Len; if ( CtrlLen > Str.Length() ) CtrlLen  = Str.Length();

   // Считаем контрольную сумму
   CtrlSum = 0; for (int i=1; i <= CtrlLen; i++) CtrlSum += (byte)Str[i];
   CtrlSum = 0xFF - CtrlSum + 1;

   return (char)CtrlSum;                         // Вернуть контрольную сумму
  }
//---------------------------------------------------------------------------
bool __fastcall TRecordParser::ValidateString(AnsiString Str)
  {
   bool         IsValid = false;                 // Статус валидности
   byte         CtrlSum;                         // Контрольная сумма (Расчёт)
   int          StrLen;                          // Длина строки
   int          SumVal;                          // Контрольная сумма (из Str)
   AnsiString   SumStr;                          // Строка контрольной суммы

   StrLen  = Str.Length();                       // Получить длину строки
   if ( (StrLen > KSLEN) && (KSLEN > 1) )        // Должна содержать что-то и КС
     {
      SumStr = Str.SubString(StrLen-KSLEN+1,KSLEN); // Выделить строку КС
      if ( SumStr[1] == KSDLM )                     // Проверить отделитель
        {
         SumStr.Delete(1,1);                     // Отбросить отделитель
         SumStr = SumStr.Trim();                 // Отбросить в ней пробелы
         SumVal = SumStr.ToIntDef(-1);           // Преобразовать в число
         if ( SumVal >= 0 )                      // Верный формат КС
           {
            CtrlSum = AccountCtrlSum(Str,StrLen-KSLEN); // Расчитать контр сумму
            IsValid = ( SumVal == int(CtrlSum) );       // Проверить контр сумму
           }
        }
     }

   return IsValid;                               // Вернуть статус валидности
  }
//---------------------------------------------------------------------------
AnsiString  __fastcall TRecordParser::GetEmptyString()
  {
   AnsiString   Str = "";                        // Строка с пустыми полями
   byte         SumVal;                          // Контрольная сумма
   AnsiString   SumStr;                          // Строка контрольной суммы
   int          BlankCount;                      // Кол-во пробелов

   for (int i=1; i <= FieldMarks.Length(); i++)  // По всем маркерам полей
     {
      Str += FLDDLM;                             // Добавить разделитель
      Str += FieldMarks[i];                      // Добавить маркер
     }
   SumVal = AccountCtrlSum(Str,Str.Length());    // Посчитать контрольную сумму
   SumStr = IntToStr(int(SumVal));               // Контрольную сумму в строку
   BlankCount = KSLEN - SumStr.Length() - 1;     // По необх-ти добавить пробелы
   if (BlankCount>0) SumStr = AnsiString::StringOfChar(' ',BlankCount) + SumStr;
   Str += AnsiString(KSDLM) + SumStr;            // Добавить контрольную сумму

   return Str;                                   // Вернуть строку пустых полей
  }
//---------------------------------------------------------------------------
void __fastcall TRecordParser::ClearStructure()
  {
   FieldCount =  0;                              // Полей нет
   FieldMarks = "";                              // Маркеров нет
   BufferSize =  0;                              // Буфеп нулевой длины
   if ( FieldOffs != NULL ) { delete [] FieldOffs; FieldOffs = NULL; } // Ёк карту
  }
//---------------------------------------------------------------------------
void __fastcall TRecordParser::BuildStructure()
  {
   TFieldParser Field;                           // Парсер поля

   if ( FieldOffs != NULL ) { delete [] FieldOffs; FieldOffs = NULL; }
   FieldMarks = FieldMarks.Trim();               // Оставить только маркеры
   FieldCount = FieldMarks.Length();             // Сколь маркеров - столь полей
   FieldOffs  = new Word[FieldCount];            // Место под карту полей
   for (int i=0; i < FieldCount; i++)            // По всем полям
     {
      Field.SetFieldMark(FieldMarks[i+1]);       // Данные типа по маркеру
      FieldMarks[i+1] = Field.FieldMark;         // Как восприняли на самом деле
      FieldOffs [i  ] = BufferSize;              // Сохранить смещение поля
      BufferSize   += Field.DataSize + 1;        // Инкримент размера буфера
     }
  }
//---------------------------------------------------------------------------
void __fastcall TRecordParser::CheckStructure()
  {
   if ( ( FieldCount != FieldMarks.Length() )         || // Несогласованности
        ( FieldMarks.Length() && (FieldOffs == NULL) ) ) // Отсутствие данных
      BuildStructure();                          // Тогда перестроим структуру
  }
//---------------------------------------------------------------------------
void __fastcall TRecordParser::ParseStructure(AnsiString Str)
  {
   AnsiString   TmpStr;                          // Вспомогательная строка
   int          DlmPos;                          // Положение разделителя полей
   int          StrLen;                          // Длина строки

   ClearStructure();                             // Очистим структуру

   TmpStr = Str.LowerCase();                     // Запись в нижний регистр
   TmpStr = TmpStr.Trim();                       // Отбросим пробелы
   do
     {
      DlmPos = TmpStr.Pos(FLDDLM);               // Положение разделителя полей
      StrLen = TmpStr.Length();                  // Текущая длина строки
      if ( DlmPos )                              // Найден разделитель поля
        {
         if ( StrLen  ) TmpStr.Delete(1,DlmPos); // Удаляем всё до поля
         // За разделителем есть место на (что-то+КС) - запоминаем маркер поля
         if ( (DlmPos + KSLEN) < StrLen ) FieldMarks += TmpStr[1];
        }
     }
   while ( DlmPos && StrLen );                   // Пока находим разделители

   BuildStructure();                             // Строим структуру по маркерам
  }
//---------------------------------------------------------------------------
void __fastcall TRecordParser::ImportStructure(TFieldDefs * FldDefs)
  {
   TFieldParser Field;                           // Парсер поля
   TFieldType   FldType;                         // Тип поля
   bool         FldUnkn = false;                 // Флаг неизвестного типа поля

   ClearStructure();                             // Очистить структуру
   if ( FldDefs == NULL ) return;                // Нечего импортировать

   for (int i=0; i < FldDefs->Count; i++ )       // По всем описаниям полей
     {
      FldType = FldDefs->Items[i]->DataType;     // Получить тип поля
      if ( Field.SetFieldType(FldType) )         // Задаём данные по типу
        FieldMarks += Field.FieldMark;           // Добавляем маркер
      else FldUnkn  = true;                      // Незнаем такого типа (пока)
     }

   BuildStructure();                             // Строим структуру по маркерам
   if ( FldUnkn ) ExportStructure(FldDefs);      // Для согласованности с FldDef
  }
//---------------------------------------------------------------------------
void __fastcall TRecordParser::ExportStructure(TFieldDefs * FldDefs)
  {
   TFieldParser Field;                           // Парсер поля
   AnsiString   FldName;                         // Название поля

   if ( FldDefs == NULL ) return;                // Некуда экспортировать

   if ( FldDefs->Count ) FldDefs->Clear();       // Очистить описание полей
   for (int i=0; i < FieldCount; i++)            // По всем полям
     {
      Field.SetFieldMark(FieldMarks[i+1]);       // Определить данные по маркеру
      FldName = "Field_" + IntToStr(i);          // Имя поля
      FldDefs->Add(FldName,Field.FieldType,Field.FieldSize,False); // Добавить
     }
  }
//---------------------------------------------------------------------------
bool __fastcall TRecordParser::BufferToString(char * Buf, AnsiString & Str)
  {
   TFieldParser Field;                           // Парсер поля
   AnsiString   FldStr;                          // Строковое представление поля
   int          BufPos;                          // Положение поля в буфере
   byte         SumVal;                          // Контрольная сумма
   AnsiString   SumStr;                          // Строка контрольной суммы
   int          BlankCount;                      // Кол-во пробелов

   Str = "";                                     // Очистить выходную строку
   if ( Buf == NULL ) return false;              // Нет буфера - нет строки
   CheckStructure();                             // Проверить структуру

   for (int i=0; i < FieldCount; i++)            // По всем полям
     {
      Field.SetFieldMark(FieldMarks[i+1]);       // Опрделить данные по маркеру
      Str += FLDDLM;                             // Добавить разделитель
      Str += Field.FieldMark;                    // Добавить разделитель и тип
      BufPos = FieldOffs[i];                     // Положение поля в буфере
      if ( Buf[BufPos] )                         // Поле задано
        {
         // Преобразовать в строку часть буфера отосящуюся к данному полю
         if ( Field.BufferToString(&Buf[BufPos+1],FldStr) ) Str += FldStr;
        }
     }

   SumVal = AccountCtrlSum(Str,Str.Length());    // Посчитать контрольную сумму
   SumStr = IntToStr(int(SumVal));               // Контрольную сумму в строку
   BlankCount = KSLEN - SumStr.Length() - 1;     // По необх-ти добавить пробелы
   if (BlankCount>0) SumStr = AnsiString::StringOfChar(' ',BlankCount) + SumStr;
   Str += AnsiString(KSDLM) + SumStr;            // Добавить контрольную сумму

   return true;                                  // Всё прошло успешно
  }
//---------------------------------------------------------------------------
bool __fastcall TRecordParser::StringToBuffer(AnsiString & Str, char * Buf)
  {
   TFieldParser Field;                           // Парсер поля
   AnsiString   TmpStr;                          // Вспомогательная строка
   AnsiString   FldStr;                          // Строковое представление поля
   int          DlmPos  = 0;                     // Положение разделителя поля
   int          FldNum  = 0;                     // Номер текущего поля
   int          BufPos  = 0;                     // Положение поля в буфере
   bool         Success = true;                  // Возращаемое значение

   // Проверяем правильность строки и наличие буфера
   if ( !ValidateString(Str) || ( Buf == NULL ) ) return false;

   setmem(Buf, BufferSize, 0);                   // Заполнить буфер нулями
   TmpStr = Str;                                 // Копируем строку
   DlmPos = TmpStr.Pos(FLDDLM);                  // Ищем начало первого поля

   // Пока есть поля, они совпадают со структурой и всё успешно преобразуется
   while ( Success && DlmPos  && ( FldNum < FieldCount ) )
     {
      TmpStr.Delete(1,DlmPos);                   // Ёк пред.поле и разделитель
      DlmPos = TmpStr.Pos(FLDDLM);               // Ищем начало следующего поля

      if ( TmpStr.Length() > KSLEN )             // Тип + что-то + КС
        {
         Field.SetFieldMark(TmpStr[1]);          // За разделителем идёт тип
         // Поле заканчивается следующим разделителем либо контрол. суммой
         if ( DlmPos ) FldStr = TmpStr.SubString(2,DlmPos-2);
         else          FldStr = TmpStr.SubString(2,TmpStr.Length()-1-KSLEN);
         // Если тип поля совпадает с тем, что по структуре то преобразуем
         if ( Field.FieldMark == FieldMarks[FldNum+1] )
            Buf[BufPos] = (char)Field.StringToBuffer(FldStr,&Buf[BufPos+1]);
         else Success = false;                   // Несовпадение типа
        }
      else Success = false;                      // Поле не полное

      BufPos += Field.DataSize + 1;              // Сдвижка по буферу
      FldNum++;                                  // Инкримент кол-ва полей
     }

   return Success;                               // Вернуть результат
  }

// =============================== TTextTable ===============================

static inline void ValidCtrCheck(TTextTable *) { new TTextTable(NULL); }
//---------------------------------------------------------------------------
__fastcall TTextTable::TTextTable(TComponent* Owner) : TDataSet(Owner)
   {
    FRecStruct.ClearStructure();                 // Очистить структуру таблицы
   }
//---------------------------------------------------------------------------
TBookmarkFlag __fastcall TTextTable::GetBookmarkFlag(char * Buffer)
  {
   TRecMark   * RecInfo;                         // Указатель на инфу о закладке

   RecInfo = (TRecMark*)(Buffer + FRecStruct.BufferSize);
   return RecInfo->BookmarkFlag;                 // Вернуть флаг закладки
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::SetBookmarkFlag(char * Buffer, TBookmarkFlag Value)
  {
   TRecMark   * RecInfo;                         // Указатель на инфу о закладке

   RecInfo = (TRecMark*)(Buffer + FRecStruct.BufferSize);
   RecInfo->BookmarkFlag = Value;                // Установить флаг закладки
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::GetBookmarkData(char * Buffer, void * Data)
  {
   TRecMark   * RecInfo;                         // Указатель на инфу о закладке

   RecInfo = (TRecMark*)(Buffer + FRecStruct.BufferSize);
   if ( Data != NULL ) *(int*)Data = RecInfo->Bookmark; // Получить № закладки
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::SetBookmarkData(char * Buffer, void *Data)
  {
   TRecMark   * RecInfo;                         // Указатель на инфу о закладке

   RecInfo = (TRecMark*)(Buffer + FRecStruct.BufferSize);
   RecInfo->Bookmark = *((int*)(Data));          // Установить номер закладки
  }
//---------------------------------------------------------------------------
char * __fastcall TTextTable::AllocRecordBuffer1()
  {
   return (char*)malloc(FRecBufSize);            // Выделить память под запись
  }
//---------------------------------------------------------------------------
void  __fastcall TTextTable::FreeRecordBuffer(char * & Buffer)
  {
   if ( Buffer != NULL ) free((void*)Buffer);    // Освободить память из-под записи
  }
//---------------------------------------------------------------------------
WORD __fastcall TTextTable::GetRecordSize()
  {
   return (WORD) FRecStruct.BufferSize;          // Вернуть мах размер записи
  }
//---------------------------------------------------------------------------
int __fastcall TTextTable::GetRecordCount()
  {
   return FData->Count;                          // Вернуть число записей
  }
//---------------------------------------------------------------------------
int __fastcall TTextTable::GetRecNo()
  {
   UpdateCursorPos();                            // Обновить положение курсора
   // FCurRec=-1 это флаг BOF. Иначе просто возращаем номер текущей записи (с 1)
   if ( (FCurRec == -1) && (RecordCount > 0) ) return 1;
   else                                        return (FCurRec+1);
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::SetRecNo(int Value)
  {
   TResyncMode  ResyncMode;                      // Опции вычитки записи

   // При нарушении диапазона помещаем указатель в начало таблицы
   if ( (Value < 0) || (Value >= FData->Count) ) Value = 0;
   FCurRec = Value - 1;                          // FCurRec нумеруется с нуля
   ResyncMode.Clear();                           // Очистить опции вычитки
   Resync(ResyncMode);                           // Установка № текущей записи
  }
//---------------------------------------------------------------------------
TGetResult __fastcall TTextTable::GetRecord(char * Buffer, TGetMode GetMode, bool DoCheck)
  {
   TGetResult   RetVal;                          // Результат получения записи
   TRecMark   * RecInfo;                         // Указатель на инфу о закладке

   if ( FData->Count < 1 ) RetVal = grEOF;       // Достигли конца файла
   else
     {
      RetVal = grOK;                             // Пока всё в норме
      switch ( GetMode )                         // Какую запись получить
        {
        case gmNext   : {                        // Следующую
                          if ( FCurRec >= RecordCount-1 ) RetVal = grEOF;
                          else                            FCurRec++;
                        } break;
        case gmPrior  : {                        // Предыдущую
                          if ( FCurRec <= 0 )             RetVal = grBOF;
                          else                            FCurRec--;
                        } break;
        case gmCurrent: {                        // Текущую
                          if ( (FCurRec < 0) || (FCurRec >= RecordCount) )
                                                          RetVal = grError;
                        } break;
        };

      if ( RetVal == grOK )                      // Если пока всё в норме
        {
         // Копируем в буфер строку записи и информацию о закладке
         FRecStruct.StringToBuffer(FData->Strings[FCurRec],Buffer);
         RecInfo = (TRecMark*)(Buffer + FRecStruct.BufferSize);
         RecInfo->BookmarkFlag = bfCurrent;
         RecInfo->Bookmark     = (int)(FData->Objects[FCurRec]);
        }
      else if ( (RetVal == grError) && DoCheck ) DatabaseError("No Records");
     }

   return RetVal;                                // Вернуть успех или код ошибки
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::SetFieldData(TField * Field, void * Buffer)
  {
   int          FldPos;                          // Смещение поля в буфере
   char       * FldBuf;                          // Буфер поля

   if ( (Field == NULL) || (Buffer == NULL) ) return;       // Наличие данных
   FldPos = FRecStruct.FieldOffs[Field->FieldNo-1];         // Получить смещение
   FldBuf = (char*)(ActiveBuffer() + FldPos);               // Получить буфер поля
   FldBuf[0] = 0x01;                                        // Поле установлено
   memcpy(&FldBuf[1],Buffer,Field->DataSize);               // Из записи в поле

   DataEvent(deFieldChange, long(Field));        // Сообщить об изменении поля
  }
//---------------------------------------------------------------------------
bool __fastcall TTextTable::GetFieldData(TField * Field, void * Buffer)
  {
   int          FldPos;                          // Смещение поля в буфере
   char       * FldBuf;                          // Буфер поля

   //
   if ( (Field == NULL) || (Buffer == NULL) ) return false; //
   FldPos = FRecStruct.FieldOffs[Field->FieldNo-1];         // Получить смещение
   FldBuf = (char*)(ActiveBuffer() + FldPos);               // Получить буфер поля
   memcpy(Buffer,&FldBuf[1],Field->DataSize);               // Из поля в запись

   return (  FldBuf[0] != 0x00 );                // Вернуть флаг установки поля
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalOpen()
  {
   AnsiString   EmptyStr;                        // Строка пустых полей
   int          EmptyIdx;                        // Индекс строки пустых полей

   if (FData == NULL) FData = new TStringList(); // Создать новый список строк

   if ( FileExists(TableName) )                  // Существует файл таблицы
     {
      LoadFromFile(TableName);                   // Загружаем данные из файла
      InternalInitFieldDefs();                   // Определить описание полей
     }
   else
      FRecStruct.ImportStructure(FieldDefs);     // Структура из описания полей

   // Нет структуры - сообщим. Есть структура - удалим все пустые строки
   if ( !FieldDefs->Count && (OnNullStructure != NULL) ) OnNullStructure(this);
   else if ( FData->Count )                      // Строки вообще есть
     {
      EmptyStr = FRecStruct.GetEmptyString();    // Строка пустых полей
      do
        {
         EmptyIdx = FData->IndexOf(EmptyStr);        // Ищем строку пустых полей
         if (EmptyIdx >= 0) FData->Delete(EmptyIdx); // если есть то удаляем её
        }
      while ( EmptyIdx >= 0 );                   // Пока есть пустые строки
     }

   FChangesCount = 0;                            // Изменений нет
   FLastBookmark = FData->Count;                 // Номер последней закладки
   FCurRec       = -1;                           // На начало файла
   FRecBufSize   = FRecStruct.BufferSize+sizeof(TRecMark); // Инфа + закладка
   BookmarkSize  = sizeof(int);                  // Размер закладки

   if ( DefaultFields ) CreateFields();          // Создать поля
   BindFields(true);                             // Привязать поля к данным
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalClose()
  {
   // При вызове деструктора все объекты наследованные от TComponent помечаются
   // как удалённые (поэтому видимо TableName = NULL) и только потом Close
   if ((FData!=NULL) && (!TableName.IsEmpty()))  // Есть данные и имя файла
     {
      if (!FData->Count && FRecStruct.FieldCount)// Нет данных, но есть структура
        {
         FData->Add(FRecStruct.GetEmptyString());// Добавим строку пустых полей
         FChangesCount = 1;                      // Её надо будет запомнить
        }
      if (FChangesCount) SaveIntoFile(TableName);// Сохранить в файле
      FData->Clear();                            // Очистить данные
      delete FData  ; FData = NULL;              // Удалить само хранилище
     }

   FRecStruct.ClearStructure();                  // Чистим структуру
   BindFields(false);                            // Осоединяем поля от данных
   if ( DefaultFields ) DestroyFields();         // Удаляем поля

   FLastBookmark =  0;                           // Обнулить счётчик закладок
   FCurRec       = -1;                           // На начало файла
   FChangesCount =  0;                           // Изменений нет
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalPost()
  {
   AnsiString   PostStr;                         // Изменённая строка
   int          ChangePercent = 0;               // Процент изменений

   // Преобразуем активный буфер в строку
   FRecStruct.BufferToString(ActiveBuffer(),PostStr);

   if ( FCurRec == -1 ) FCurRec = 0;             // Данные в начале таблицы
   if ( State == dsEdit )                        // Режим редактирования?
      FData->Strings[FCurRec] = PostStr;         // Активный буфер в текущ запись
   else                                          // Иначе добавляем новую запись
     {                                           // в текущем положении курсора
      FLastBookmark++;
      FData->InsertObject(FCurRec, PostStr, (TObject*)(FLastBookmark));
     }
   FChangesCount++;                              // Инкримент кол-ва изменений

   // Если процент изменённых записей превышает MAXCHANGES, то сохраним их
   if      (  RecordCount  ) ChangePercent = int(100.0*FChangesCount/RecordCount);
   else if ( FChangesCount ) ChangePercent = MAXCHANGES + 1;
   if ( (ChangePercent >= MAXCHANGES) && !TableName.IsEmpty() )
     {
      SaveIntoFile(TableName);                   // Сохраняем изменения
      FChangesCount  = 0;                        // Изменений нет
     }
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalInitFieldDefs()
  {
   int          RecNum  =  0;                     // Номер записи
   int          AccCnt  =  0;                     // Аккамулятор\счётчик
   AnsiString   FldDefs = "";                     // Строка описания полей
   bool         VldStr;                           // Флаг валидности строки

   // Пока не найдём OKCOUNT записей с одинаковой структурой полей
   FldDefs = "";
   while ( (RecNum < FData->Count) && (AccCnt < OKCOUNT) )
     {
      VldStr = FRecStruct.ValidateString(FData->Strings[RecNum]);// Проверяем
      if ( VldStr )                                              // валидность
        {
         FRecStruct.ParseStructure( FData->Strings[RecNum] );    // Парсим струк
         if   ( FldDefs == FRecStruct.FieldMarks ) AccCnt++ ;    // Совпала
         else { FldDefs  = FRecStruct.FieldMarks ; AccCnt = 1; } // Не совпала
        }
      RecNum++;                                  // К следующей записи
     }

   FRecStruct.ExportStructure(FieldDefs);        // Задаём описание полей
  }
//---------------------------------------------------------------------------
bool __fastcall TTextTable::IsCursorOpen() { return ( FData != NULL ); }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalAddRecord(void * Buffer, bool Append)
  {
   FChangesCount++;                              // Инкримент кол-ва изменений
   FLastBookmark++;                              // Инкримент № послед закладки
   if ( Append ) InternalLast();                 // Добавить данные в конец
   else if ( FCurRec == -1 ) FCurRec = 0;        // Добавить данные в начало
   FData->InsertObject(FCurRec, (char*)(Buffer), (TObject*)(FLastBookmark));
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalInitRecord(char * Buffer)
  {
   setmem(Buffer, RecordSize, 0);                // Выделить память под запись
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalDelete()
  {
   FData->Delete(FCurRec);                       // Удалить запись из хранилища
   if ( FCurRec >= FData->Count ) FCurRec--;     // Декримент № текущей записи

   if ( !FData->Count )                          // Нет данных
     {
      FData->Add(FRecStruct.GetEmptyString());   // Добавить строку пустых полей
      FChangesCount = 1;                         // Изменений нет
     }
   else FChangesCount++;                         // Инкримент кол-ва изменений
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalGotoBookmark(void * Bookmark)
  {
   int          MarkVal;                         // Значение закладки
   int          Index;                           // Индекс записи

   MarkVal = *((int*)Bookmark);                       // Целое по этому адресу
   Index = FData->IndexOfObject((TObject*)(MarkVal)); // Найти запись по зпкладке
   // Позиционируем эту запись, в противном случае выводим сообщение об ошибке
   if ( Index != -1 ) FCurRec = Index; else DatabaseError("Bookmark not found");
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalSetToRecord(char * Buffer)
  {
   TRecMark   * RecInfo;                         // Указатель на инфу о закладке

   RecInfo = (TRecMark*)(Buffer + FRecStruct.BufferSize);
   InternalGotoBookmark(&RecInfo->Bookmark);     // Шлёпать на эту закладку
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalFirst()
  {
   FCurRec = -1;                                 // На первую запись
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalLast()
  {
   FCurRec = FData->Count;                       // На последнюю запись
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalHandleException()
  {
   Application->HandleException(this);           // Обработка исключения
  }
//---------------------------------------------------------------------------
int __fastcall TTextTable::LoadFromFile(AnsiString Name)
  {
   AnsiString   LoadName;                        // Файл откуда загружать
   int          LoadCount = 0;                   // Число загруженных записей
   bool         VldStr;                          // Валидность строки

   //
   if ( Name.IsEmpty() ) LoadName = TableName; else LoadName = Name;
   if ( (FData!=NULL) && !LoadName.IsEmpty() )   // Есть данные и имя файла
     {
      FData->LoadFromFile(LoadName);             // Загрузить данные из файла
      for (int i=0; i<FData->Count; i++)         // По всем считанным записям
        {
         // Если строка валидна, то зададим ей номер закладки, иначе удалим
         VldStr = FRecStruct.ValidateString(FData->Strings[i]);
         if (VldStr) FData->Objects[i] = (TObject*)(i); else FData->Delete (i);
        }
      LoadCount = FData->Count;                  // Число записей
     }

   return LoadCount;                             // Число загруженных записей
  }
//---------------------------------------------------------------------------
int __fastcall TTextTable::SaveIntoFile(AnsiString Name)
  {
   AnsiString   SaveName;                        // Файл куда сохранять
   int          SaveCount = 0;                   // Число сохранённых записей

   // Если не указано куда сохранять, сохраняем туда откуда открывали
   if ( Name.IsEmpty() ) SaveName = TableName; else SaveName = Name;
   if ( (FData!=NULL) && !SaveName.IsEmpty() )   // Есть данные и имя файла
     {
      FData->SaveToFile(SaveName);               // Сохраняем
      SaveCount = FData->Count;                  // Число записей
     }

   return SaveCount;                             // Число сохранённых записей
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::CreateTable()
  {
   // Таблица должна быть закрыта
   if ( Active ) { DatabaseError("Close table before create it."); return; }
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::DeleteTable()
  {
   // Таблица должна быть закрыта
   if ( Active ) { DatabaseError("Close table before delete it."); return; }

   // Если файл таблицы существует, то удалим его
   if ( FileExists(FTableName) ) DeleteFile(FTableName);
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::RenameTable(AnsiString NewName)
  {
   // Таблица должна быть закрыта
   if ( Active ) { DatabaseError("Close table before rename it."); return; }

   // Если файл таблицы существует, то переименуем его
   if ( FileExists(FTableName) )
     { if ( RenameFile(FTableName,NewName) ) FTableName = NewName; }
  }
//---------------------------------------------------------------------------
namespace Textdataset                            // Регистрация компонента
  {
   void __fastcall PACKAGE Register()
     {
      TComponentClass classes[1] = {__classid(TTextTable)};
      RegisterComponents("Data Access", classes, 0);
     }
  }
//---------------------------------------------------------------------------
