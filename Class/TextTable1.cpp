//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include <ctype.h>                               // ������ � ���������
#include <stdlib.h>                              // ������ � �������
#include <sysutils.hpp>                          // ������ � �����\��������       "
#include "TextTable.h"                           // ����������� ��������
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
#pragma option -w-sig  // ��������� : Conversion may lose significant digits.

// ============================== TFieldParser ==============================

__fastcall TFieldParser::TFieldParser() { SetFieldType(ftString); }
//---------------------------------------------------------------------------
void __fastcall TFieldParser::MarkToFieldType(char Mark, TFieldType & Type)
  {
   char         LowCaseMark;                     // ������ � ������ ��������

   LowCaseMark = (char)tolower( (int)Mark );     // ��������� � ������ �������
   switch ( LowCaseMark )                        // ������ ���� � ��� ����
     {
      case fmBool  : Type = ftBoolean ; break;   // ����������
      case fmInt   : Type = ftInteger ; break;   // �����
      case fmFloat : Type = ftFloat   ; break;   // ������������
      case fmTime  : Type = ftDateTime; break;   // ����\�����
      default      : Type = ftString  ;          // �� ��������� - ���������
     }
  }
//---------------------------------------------------------------------------
bool __fastcall TFieldParser::FieldTypeToMark(TFieldType Type, char & Mark)
  {
   bool         KnownType = true;                // ���� ������������ ����

   switch ( Type )                               // ��� �� ��� ����
     {
      case ftString   : Mark = fmStr  ; break;   // ���������
      case ftBoolean  : Mark = fmBool ; break;   // ����������
      case ftInteger  : Mark = fmInt  ; break;   // �����
      case ftFloat    : Mark = fmFloat; break;   // ������������
      case ftDateTime : Mark = fmTime ; break;   // ����\�����
      default         : KnownType = false;       // ������� ������ ���� (����)
     }
   return KnownType;                             // �������� � ��������� ����
  }
//---------------------------------------------------------------------------
bool __fastcall TFieldParser::SetFieldType(TFieldType Type)
  {
   FieldType = Type;                             // ������������� ���
   switch ( Type )                               // �� ���� ����� �������
     {
      case ftString   : FieldSize = STRFLDSIZ; DataSize = STRFLDSIZ+1; break; //
      case ftBoolean  : FieldSize =         0; DataSize =           2; break; //
      case ftInteger  : FieldSize =         0; DataSize =           4; break; //
      case ftFloat    :                                                       //
      case ftDateTime : FieldSize =         0; DataSize =           8; break; //
     }
   return FieldTypeToMark(Type,FieldMark);       // ������ + ����������� ����
  }
//---------------------------------------------------------------------------
bool __fastcall TFieldParser::SetFieldMark(char Mark)
  {
   TFieldType   Type;                            // ��� ����

   MarkToFieldType(Mark,Type);                   // ������ ���� � ��� ����
   return  SetFieldType(Type);                   // ����� ������ �� ����
  }
//---------------------------------------------------------------------------
bool __fastcall TFieldParser::BufferToString(char * Buf, AnsiString & Str)
  {
   bool         Success = true;                  // ����� ��������������
   WORD       * bData;                           // ���������� ������
   long       * iData;                           // ����� ������
   double     * dData;                           // ������������ � ����\�����
   TDateTime    DateTime;                        // ���� ��������-�� ����\�����
   TTimeStamp   TimeStamp;                       // ��.  ��������-�� ����\�����

   if ( Buf == NULL ) return false;              // ��� ������ - ������ ��������
   try
     {
      Str = "";                                  // ������� ������
      switch ( FieldType )                       // �������������� ���� ����
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
         default         :    Success = false;   // ����������� ���
        };
     }
   catch ( ... ) { Success = false; }            // ������ ��������������

   return Success;                               // ������� ������� ���� ���
  }
//---------------------------------------------------------------------------
bool __fastcall TFieldParser::StringToBuffer(AnsiString & Str, char * Buf)
  {
   bool         Success = true;                  // ����� ��������������
   WORD       * bData;                           // ���������� ������
   long       * iData;                           // ����� ������
   double     * dData;                           // ������������ � ����\�����
   int          DecPos;                          // ��������� ���������� �����
   char         WrongDlm;                        // ��������. �����. �����������
   TDateTime    DateTime;                        // ���� ��������-�� ����\�����
   TTimeStamp   TimeStamp;                       // ��.  ��������-�� ����\�����

   if ( Buf == NULL   ) return false;            // ��� ������ - ������ ��������
   if ( Str.IsEmpty() ) return false;            // ������ ���������������

   try
     {
      if (FieldType!=ftString) Str = Str.Trim(); // ��������� �������
      switch ( FieldType )                       // �������������� ���� ����
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
         default         :    Success = false;   // ����������� ��� ����
        };
     }
   catch ( ... ) { Success = false; }            // ������ ��������������

   return Success;                               // ������� ������� ��� ���
  }

// ============================= TRecordParser ==============================

__fastcall TRecordParser::TRecordParser() { ClearStructure(); }
//---------------------------------------------------------------------------
char __fastcall TRecordParser::AccountCtrlSum(AnsiString Str, byte Len)
  {
   byte         CtrlLen;                         // ���������� �����
   byte         CtrlSum;                         // ����������� �����

   // �������� ���������� ��������� ����� (������ ���� �� ������� ����� ������)
   CtrlLen = Len; if ( CtrlLen > Str.Length() ) CtrlLen  = Str.Length();

   // ������� ����������� �����
   CtrlSum = 0; for (int i=1; i <= CtrlLen; i++) CtrlSum += (byte)Str[i];
   CtrlSum = 0xFF - CtrlSum + 1;

   return (char)CtrlSum;                         // ������� ����������� �����
  }
//---------------------------------------------------------------------------
bool __fastcall TRecordParser::ValidateString(AnsiString Str)
  {
   bool         IsValid = false;                 // ������ ����������
   byte         CtrlSum;                         // ����������� ����� (������)
   int          StrLen;                          // ����� ������
   int          SumVal;                          // ����������� ����� (�� Str)
   AnsiString   SumStr;                          // ������ ����������� �����

   StrLen  = Str.Length();                       // �������� ����� ������
   if ( (StrLen > KSLEN) && (KSLEN > 1) )        // ������ ��������� ���-�� � ��
     {
      SumStr = Str.SubString(StrLen-KSLEN+1,KSLEN); // �������� ������ ��
      if ( SumStr[1] == KSDLM )                     // ��������� ����������
        {
         SumStr.Delete(1,1);                     // ��������� ����������
         SumStr = SumStr.Trim();                 // ��������� � ��� �������
         SumVal = SumStr.ToIntDef(-1);           // ������������� � �����
         if ( SumVal >= 0 )                      // ������ ������ ��
           {
            CtrlSum = AccountCtrlSum(Str,StrLen-KSLEN); // ��������� ����� �����
            IsValid = ( SumVal == int(CtrlSum) );       // ��������� ����� �����
           }
        }
     }

   return IsValid;                               // ������� ������ ����������
  }
//---------------------------------------------------------------------------
AnsiString  __fastcall TRecordParser::GetEmptyString()
  {
   AnsiString   Str = "";                        // ������ � ������� ������
   byte         SumVal;                          // ����������� �����
   AnsiString   SumStr;                          // ������ ����������� �����
   int          BlankCount;                      // ���-�� ��������

   for (int i=1; i <= FieldMarks.Length(); i++)  // �� ���� �������� �����
     {
      Str += FLDDLM;                             // �������� �����������
      Str += FieldMarks[i];                      // �������� ������
     }
   SumVal = AccountCtrlSum(Str,Str.Length());    // ��������� ����������� �����
   SumStr = IntToStr(int(SumVal));               // ����������� ����� � ������
   BlankCount = KSLEN - SumStr.Length() - 1;     // �� �����-�� �������� �������
   if (BlankCount>0) SumStr = AnsiString::StringOfChar(' ',BlankCount) + SumStr;
   Str += AnsiString(KSDLM) + SumStr;            // �������� ����������� �����

   return Str;                                   // ������� ������ ������ �����
  }
//---------------------------------------------------------------------------
void __fastcall TRecordParser::ClearStructure()
  {
   FieldCount =  0;                              // ����� ���
   FieldMarks = "";                              // �������� ���
   BufferSize =  0;                              // ����� ������� �����
   if ( FieldOffs != NULL ) { delete [] FieldOffs; FieldOffs = NULL; } // �� �����
  }
//---------------------------------------------------------------------------
void __fastcall TRecordParser::BuildStructure()
  {
   TFieldParser Field;                           // ������ ����

   if ( FieldOffs != NULL ) { delete [] FieldOffs; FieldOffs = NULL; }
   FieldMarks = FieldMarks.Trim();               // �������� ������ �������
   FieldCount = FieldMarks.Length();             // ����� �������� - ����� �����
   FieldOffs  = new Word[FieldCount];            // ����� ��� ����� �����
   for (int i=0; i < FieldCount; i++)            // �� ���� �����
     {
      Field.SetFieldMark(FieldMarks[i+1]);       // ������ ���� �� �������
      FieldMarks[i+1] = Field.FieldMark;         // ��� ���������� �� ����� ����
      FieldOffs [i  ] = BufferSize;              // ��������� �������� ����
      BufferSize   += Field.DataSize + 1;        // ��������� ������� ������
     }
  }
//---------------------------------------------------------------------------
void __fastcall TRecordParser::CheckStructure()
  {
   if ( ( FieldCount != FieldMarks.Length() )         || // �����������������
        ( FieldMarks.Length() && (FieldOffs == NULL) ) ) // ���������� ������
      BuildStructure();                          // ����� ���������� ���������
  }
//---------------------------------------------------------------------------
void __fastcall TRecordParser::ParseStructure(AnsiString Str)
  {
   AnsiString   TmpStr;                          // ��������������� ������
   int          DlmPos;                          // ��������� ����������� �����
   int          StrLen;                          // ����� ������

   ClearStructure();                             // ������� ���������

   TmpStr = Str.LowerCase();                     // ������ � ������ �������
   TmpStr = TmpStr.Trim();                       // �������� �������
   do
     {
      DlmPos = TmpStr.Pos(FLDDLM);               // ��������� ����������� �����
      StrLen = TmpStr.Length();                  // ������� ����� ������
      if ( DlmPos )                              // ������ ����������� ����
        {
         if ( StrLen  ) TmpStr.Delete(1,DlmPos); // ������� �� �� ����
         // �� ������������ ���� ����� �� (���-��+��) - ���������� ������ ����
         if ( (DlmPos + KSLEN) < StrLen ) FieldMarks += TmpStr[1];
        }
     }
   while ( DlmPos && StrLen );                   // ���� ������� �����������

   BuildStructure();                             // ������ ��������� �� ��������
  }
//---------------------------------------------------------------------------
void __fastcall TRecordParser::ImportStructure(TFieldDefs * FldDefs)
  {
   TFieldParser Field;                           // ������ ����
   TFieldType   FldType;                         // ��� ����
   bool         FldUnkn = false;                 // ���� ������������ ���� ����

   ClearStructure();                             // �������� ���������
   if ( FldDefs == NULL ) return;                // ������ �������������

   for (int i=0; i < FldDefs->Count; i++ )       // �� ���� ��������� �����
     {
      FldType = FldDefs->Items[i]->DataType;     // �������� ��� ����
      if ( Field.SetFieldType(FldType) )         // ����� ������ �� ����
        FieldMarks += Field.FieldMark;           // ��������� ������
      else FldUnkn  = true;                      // ������� ������ ���� (����)
     }

   BuildStructure();                             // ������ ��������� �� ��������
   if ( FldUnkn ) ExportStructure(FldDefs);      // ��� ��������������� � FldDef
  }
//---------------------------------------------------------------------------
void __fastcall TRecordParser::ExportStructure(TFieldDefs * FldDefs)
  {
   TFieldParser Field;                           // ������ ����
   AnsiString   FldName;                         // �������� ����

   if ( FldDefs == NULL ) return;                // ������ ��������������

   if ( FldDefs->Count ) FldDefs->Clear();       // �������� �������� �����
   for (int i=0; i < FieldCount; i++)            // �� ���� �����
     {
      Field.SetFieldMark(FieldMarks[i+1]);       // ���������� ������ �� �������
      FldName = "Field_" + IntToStr(i);          // ��� ����
      FldDefs->Add(FldName,Field.FieldType,Field.FieldSize,False); // ��������
     }
  }
//---------------------------------------------------------------------------
bool __fastcall TRecordParser::BufferToString(char * Buf, AnsiString & Str)
  {
   TFieldParser Field;                           // ������ ����
   AnsiString   FldStr;                          // ��������� ������������� ����
   int          BufPos;                          // ��������� ���� � ������
   byte         SumVal;                          // ����������� �����
   AnsiString   SumStr;                          // ������ ����������� �����
   int          BlankCount;                      // ���-�� ��������

   Str = "";                                     // �������� �������� ������
   if ( Buf == NULL ) return false;              // ��� ������ - ��� ������
   CheckStructure();                             // ��������� ���������

   for (int i=0; i < FieldCount; i++)            // �� ���� �����
     {
      Field.SetFieldMark(FieldMarks[i+1]);       // ��������� ������ �� �������
      Str += FLDDLM;                             // �������� �����������
      Str += Field.FieldMark;                    // �������� ����������� � ���
      BufPos = FieldOffs[i];                     // ��������� ���� � ������
      if ( Buf[BufPos] )                         // ���� ������
        {
         // ������������� � ������ ����� ������ ���������� � ������� ����
         if ( Field.BufferToString(&Buf[BufPos+1],FldStr) ) Str += FldStr;
        }
     }

   SumVal = AccountCtrlSum(Str,Str.Length());    // ��������� ����������� �����
   SumStr = IntToStr(int(SumVal));               // ����������� ����� � ������
   BlankCount = KSLEN - SumStr.Length() - 1;     // �� �����-�� �������� �������
   if (BlankCount>0) SumStr = AnsiString::StringOfChar(' ',BlankCount) + SumStr;
   Str += AnsiString(KSDLM) + SumStr;            // �������� ����������� �����

   return true;                                  // �� ������ �������
  }
//---------------------------------------------------------------------------
bool __fastcall TRecordParser::StringToBuffer(AnsiString & Str, char * Buf)
  {
   TFieldParser Field;                           // ������ ����
   AnsiString   TmpStr;                          // ��������������� ������
   AnsiString   FldStr;                          // ��������� ������������� ����
   int          DlmPos  = 0;                     // ��������� ����������� ����
   int          FldNum  = 0;                     // ����� �������� ����
   int          BufPos  = 0;                     // ��������� ���� � ������
   bool         Success = true;                  // ����������� ��������

   // ��������� ������������ ������ � ������� ������
   if ( !ValidateString(Str) || ( Buf == NULL ) ) return false;

   setmem(Buf, BufferSize, 0);                   // ��������� ����� ������
   TmpStr = Str;                                 // �������� ������
   DlmPos = TmpStr.Pos(FLDDLM);                  // ���� ������ ������� ����

   // ���� ���� ����, ��� ��������� �� ���������� � �� ������� �������������
   while ( Success && DlmPos  && ( FldNum < FieldCount ) )
     {
      TmpStr.Delete(1,DlmPos);                   // �� ����.���� � �����������
      DlmPos = TmpStr.Pos(FLDDLM);               // ���� ������ ���������� ����

      if ( TmpStr.Length() > KSLEN )             // ��� + ���-�� + ��
        {
         Field.SetFieldMark(TmpStr[1]);          // �� ������������ ��� ���
         // ���� ������������� ��������� ������������ ���� �������. ������
         if ( DlmPos ) FldStr = TmpStr.SubString(2,DlmPos-2);
         else          FldStr = TmpStr.SubString(2,TmpStr.Length()-1-KSLEN);
         // ���� ��� ���� ��������� � ���, ��� �� ��������� �� �����������
         if ( Field.FieldMark == FieldMarks[FldNum+1] )
            Buf[BufPos] = (char)Field.StringToBuffer(FldStr,&Buf[BufPos+1]);
         else Success = false;                   // ������������ ����
        }
      else Success = false;                      // ���� �� ������

      BufPos += Field.DataSize + 1;              // ������� �� ������
      FldNum++;                                  // ��������� ���-�� �����
     }

   return Success;                               // ������� ���������
  }

// =============================== TTextTable ===============================

static inline void ValidCtrCheck(TTextTable *) { new TTextTable(NULL); }
//---------------------------------------------------------------------------
__fastcall TTextTable::TTextTable(TComponent* Owner) : TDataSet(Owner)
   {
    FRecStruct.ClearStructure();                 // �������� ��������� �������
   }
//---------------------------------------------------------------------------
TBookmarkFlag __fastcall TTextTable::GetBookmarkFlag(char * Buffer)
  {
   TRecMark   * RecInfo;                         // ��������� �� ���� � ��������

   RecInfo = (TRecMark*)(Buffer + FRecStruct.BufferSize);
   return RecInfo->BookmarkFlag;                 // ������� ���� ��������
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::SetBookmarkFlag(char * Buffer, TBookmarkFlag Value)
  {
   TRecMark   * RecInfo;                         // ��������� �� ���� � ��������

   RecInfo = (TRecMark*)(Buffer + FRecStruct.BufferSize);
   RecInfo->BookmarkFlag = Value;                // ���������� ���� ��������
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::GetBookmarkData(char * Buffer, void * Data)
  {
   TRecMark   * RecInfo;                         // ��������� �� ���� � ��������

   RecInfo = (TRecMark*)(Buffer + FRecStruct.BufferSize);
   if ( Data != NULL ) *(int*)Data = RecInfo->Bookmark; // �������� � ��������
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::SetBookmarkData(char * Buffer, void *Data)
  {
   TRecMark   * RecInfo;                         // ��������� �� ���� � ��������

   RecInfo = (TRecMark*)(Buffer + FRecStruct.BufferSize);
   RecInfo->Bookmark = *((int*)(Data));          // ���������� ����� ��������
  }
//---------------------------------------------------------------------------
char * __fastcall TTextTable::AllocRecordBuffer1()
  {
   return (char*)malloc(FRecBufSize);            // �������� ������ ��� ������
  }
//---------------------------------------------------------------------------
void  __fastcall TTextTable::FreeRecordBuffer(char * & Buffer)
  {
   if ( Buffer != NULL ) free((void*)Buffer);    // ���������� ������ ��-��� ������
  }
//---------------------------------------------------------------------------
WORD __fastcall TTextTable::GetRecordSize()
  {
   return (WORD) FRecStruct.BufferSize;          // ������� ��� ������ ������
  }
//---------------------------------------------------------------------------
int __fastcall TTextTable::GetRecordCount()
  {
   return FData->Count;                          // ������� ����� �������
  }
//---------------------------------------------------------------------------
int __fastcall TTextTable::GetRecNo()
  {
   UpdateCursorPos();                            // �������� ��������� �������
   // FCurRec=-1 ��� ���� BOF. ����� ������ ��������� ����� ������� ������ (� 1)
   if ( (FCurRec == -1) && (RecordCount > 0) ) return 1;
   else                                        return (FCurRec+1);
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::SetRecNo(int Value)
  {
   TResyncMode  ResyncMode;                      // ����� ������� ������

   // ��� ��������� ��������� �������� ��������� � ������ �������
   if ( (Value < 0) || (Value >= FData->Count) ) Value = 0;
   FCurRec = Value - 1;                          // FCurRec ���������� � ����
   ResyncMode.Clear();                           // �������� ����� �������
   Resync(ResyncMode);                           // ��������� � ������� ������
  }
//---------------------------------------------------------------------------
TGetResult __fastcall TTextTable::GetRecord(char * Buffer, TGetMode GetMode, bool DoCheck)
  {
   TGetResult   RetVal;                          // ��������� ��������� ������
   TRecMark   * RecInfo;                         // ��������� �� ���� � ��������

   if ( FData->Count < 1 ) RetVal = grEOF;       // �������� ����� �����
   else
     {
      RetVal = grOK;                             // ���� �� � �����
      switch ( GetMode )                         // ����� ������ ��������
        {
        case gmNext   : {                        // ���������
                          if ( FCurRec >= RecordCount-1 ) RetVal = grEOF;
                          else                            FCurRec++;
                        } break;
        case gmPrior  : {                        // ����������
                          if ( FCurRec <= 0 )             RetVal = grBOF;
                          else                            FCurRec--;
                        } break;
        case gmCurrent: {                        // �������
                          if ( (FCurRec < 0) || (FCurRec >= RecordCount) )
                                                          RetVal = grError;
                        } break;
        };

      if ( RetVal == grOK )                      // ���� ���� �� � �����
        {
         // �������� � ����� ������ ������ � ���������� � ��������
         FRecStruct.StringToBuffer(FData->Strings[FCurRec],Buffer);
         RecInfo = (TRecMark*)(Buffer + FRecStruct.BufferSize);
         RecInfo->BookmarkFlag = bfCurrent;
         RecInfo->Bookmark     = (int)(FData->Objects[FCurRec]);
        }
      else if ( (RetVal == grError) && DoCheck ) DatabaseError("No Records");
     }

   return RetVal;                                // ������� ����� ��� ��� ������
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::SetFieldData(TField * Field, void * Buffer)
  {
   int          FldPos;                          // �������� ���� � ������
   char       * FldBuf;                          // ����� ����

   if ( (Field == NULL) || (Buffer == NULL) ) return;       // ������� ������
   FldPos = FRecStruct.FieldOffs[Field->FieldNo-1];         // �������� ��������
   FldBuf = (char*)(ActiveBuffer() + FldPos);               // �������� ����� ����
   FldBuf[0] = 0x01;                                        // ���� �����������
   memcpy(&FldBuf[1],Buffer,Field->DataSize);               // �� ������ � ����

   DataEvent(deFieldChange, long(Field));        // �������� �� ��������� ����
  }
//---------------------------------------------------------------------------
bool __fastcall TTextTable::GetFieldData(TField * Field, void * Buffer)
  {
   int          FldPos;                          // �������� ���� � ������
   char       * FldBuf;                          // ����� ����

   //
   if ( (Field == NULL) || (Buffer == NULL) ) return false; //
   FldPos = FRecStruct.FieldOffs[Field->FieldNo-1];         // �������� ��������
   FldBuf = (char*)(ActiveBuffer() + FldPos);               // �������� ����� ����
   memcpy(Buffer,&FldBuf[1],Field->DataSize);               // �� ���� � ������

   return (  FldBuf[0] != 0x00 );                // ������� ���� ��������� ����
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalOpen()
  {
   AnsiString   EmptyStr;                        // ������ ������ �����
   int          EmptyIdx;                        // ������ ������ ������ �����

   if (FData == NULL) FData = new TStringList(); // ������� ����� ������ �����

   if ( FileExists(TableName) )                  // ���������� ���� �������
     {
      LoadFromFile(TableName);                   // ��������� ������ �� �����
      InternalInitFieldDefs();                   // ���������� �������� �����
     }
   else
      FRecStruct.ImportStructure(FieldDefs);     // ��������� �� �������� �����

   // ��� ��������� - �������. ���� ��������� - ������ ��� ������ ������
   if ( !FieldDefs->Count && (OnNullStructure != NULL) ) OnNullStructure(this);
   else if ( FData->Count )                      // ������ ������ ����
     {
      EmptyStr = FRecStruct.GetEmptyString();    // ������ ������ �����
      do
        {
         EmptyIdx = FData->IndexOf(EmptyStr);        // ���� ������ ������ �����
         if (EmptyIdx >= 0) FData->Delete(EmptyIdx); // ���� ���� �� ������� �
        }
      while ( EmptyIdx >= 0 );                   // ���� ���� ������ ������
     }

   FChangesCount = 0;                            // ��������� ���
   FLastBookmark = FData->Count;                 // ����� ��������� ��������
   FCurRec       = -1;                           // �� ������ �����
   FRecBufSize   = FRecStruct.BufferSize+sizeof(TRecMark); // ���� + ��������
   BookmarkSize  = sizeof(int);                  // ������ ��������

   if ( DefaultFields ) CreateFields();          // ������� ����
   BindFields(true);                             // ��������� ���� � ������
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalClose()
  {
   // ��� ������ ����������� ��� ������� ������������� �� TComponent ����������
   // ��� �������� (������� ������ TableName = NULL) � ������ ����� Close
   if ((FData!=NULL) && (!TableName.IsEmpty()))  // ���� ������ � ��� �����
     {
      if (!FData->Count && FRecStruct.FieldCount)// ��� ������, �� ���� ���������
        {
         FData->Add(FRecStruct.GetEmptyString());// ������� ������ ������ �����
         FChangesCount = 1;                      // Ÿ ���� ����� ���������
        }
      if (FChangesCount) SaveIntoFile(TableName);// ��������� � �����
      FData->Clear();                            // �������� ������
      delete FData  ; FData = NULL;              // ������� ���� ���������
     }

   FRecStruct.ClearStructure();                  // ������ ���������
   BindFields(false);                            // ���������� ���� �� ������
   if ( DefaultFields ) DestroyFields();         // ������� ����

   FLastBookmark =  0;                           // �������� ������� ��������
   FCurRec       = -1;                           // �� ������ �����
   FChangesCount =  0;                           // ��������� ���
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalPost()
  {
   AnsiString   PostStr;                         // ��������� ������
   int          ChangePercent = 0;               // ������� ���������

   // ����������� �������� ����� � ������
   FRecStruct.BufferToString(ActiveBuffer(),PostStr);

   if ( FCurRec == -1 ) FCurRec = 0;             // ������ � ������ �������
   if ( State == dsEdit )                        // ����� ��������������?
      FData->Strings[FCurRec] = PostStr;         // �������� ����� � ����� ������
   else                                          // ����� ��������� ����� ������
     {                                           // � ������� ��������� �������
      FLastBookmark++;
      FData->InsertObject(FCurRec, PostStr, (TObject*)(FLastBookmark));
     }
   FChangesCount++;                              // ��������� ���-�� ���������

   // ���� ������� ��������� ������� ��������� MAXCHANGES, �� �������� ��
   if      (  RecordCount  ) ChangePercent = int(100.0*FChangesCount/RecordCount);
   else if ( FChangesCount ) ChangePercent = MAXCHANGES + 1;
   if ( (ChangePercent >= MAXCHANGES) && !TableName.IsEmpty() )
     {
      SaveIntoFile(TableName);                   // ��������� ���������
      FChangesCount  = 0;                        // ��������� ���
     }
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalInitFieldDefs()
  {
   int          RecNum  =  0;                     // ����� ������
   int          AccCnt  =  0;                     // �����������\�������
   AnsiString   FldDefs = "";                     // ������ �������� �����
   bool         VldStr;                           // ���� ���������� ������

   // ���� �� ����� OKCOUNT ������� � ���������� ���������� �����
   FldDefs = "";
   while ( (RecNum < FData->Count) && (AccCnt < OKCOUNT) )
     {
      VldStr = FRecStruct.ValidateString(FData->Strings[RecNum]);// ���������
      if ( VldStr )                                              // ����������
        {
         FRecStruct.ParseStructure( FData->Strings[RecNum] );    // ������ �����
         if   ( FldDefs == FRecStruct.FieldMarks ) AccCnt++ ;    // �������
         else { FldDefs  = FRecStruct.FieldMarks ; AccCnt = 1; } // �� �������
        }
      RecNum++;                                  // � ��������� ������
     }

   FRecStruct.ExportStructure(FieldDefs);        // ����� �������� �����
  }
//---------------------------------------------------------------------------
bool __fastcall TTextTable::IsCursorOpen() { return ( FData != NULL ); }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalAddRecord(void * Buffer, bool Append)
  {
   FChangesCount++;                              // ��������� ���-�� ���������
   FLastBookmark++;                              // ��������� � ������ ��������
   if ( Append ) InternalLast();                 // �������� ������ � �����
   else if ( FCurRec == -1 ) FCurRec = 0;        // �������� ������ � ������
   FData->InsertObject(FCurRec, (char*)(Buffer), (TObject*)(FLastBookmark));
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalInitRecord(char * Buffer)
  {
   setmem(Buffer, RecordSize, 0);                // �������� ������ ��� ������
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalDelete()
  {
   FData->Delete(FCurRec);                       // ������� ������ �� ���������
   if ( FCurRec >= FData->Count ) FCurRec--;     // ��������� � ������� ������

   if ( !FData->Count )                          // ��� ������
     {
      FData->Add(FRecStruct.GetEmptyString());   // �������� ������ ������ �����
      FChangesCount = 1;                         // ��������� ���
     }
   else FChangesCount++;                         // ��������� ���-�� ���������
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalGotoBookmark(void * Bookmark)
  {
   int          MarkVal;                         // �������� ��������
   int          Index;                           // ������ ������

   MarkVal = *((int*)Bookmark);                       // ����� �� ����� ������
   Index = FData->IndexOfObject((TObject*)(MarkVal)); // ����� ������ �� ��������
   // ������������� ��� ������, � ��������� ������ ������� ��������� �� ������
   if ( Index != -1 ) FCurRec = Index; else DatabaseError("Bookmark not found");
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalSetToRecord(char * Buffer)
  {
   TRecMark   * RecInfo;                         // ��������� �� ���� � ��������

   RecInfo = (TRecMark*)(Buffer + FRecStruct.BufferSize);
   InternalGotoBookmark(&RecInfo->Bookmark);     // ������ �� ��� ��������
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalFirst()
  {
   FCurRec = -1;                                 // �� ������ ������
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalLast()
  {
   FCurRec = FData->Count;                       // �� ��������� ������
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::InternalHandleException()
  {
   Application->HandleException(this);           // ��������� ����������
  }
//---------------------------------------------------------------------------
int __fastcall TTextTable::LoadFromFile(AnsiString Name)
  {
   AnsiString   LoadName;                        // ���� ������ ���������
   int          LoadCount = 0;                   // ����� ����������� �������
   bool         VldStr;                          // ���������� ������

   //
   if ( Name.IsEmpty() ) LoadName = TableName; else LoadName = Name;
   if ( (FData!=NULL) && !LoadName.IsEmpty() )   // ���� ������ � ��� �����
     {
      FData->LoadFromFile(LoadName);             // ��������� ������ �� �����
      for (int i=0; i<FData->Count; i++)         // �� ���� ��������� �������
        {
         // ���� ������ �������, �� ������� �� ����� ��������, ����� ������
         VldStr = FRecStruct.ValidateString(FData->Strings[i]);
         if (VldStr) FData->Objects[i] = (TObject*)(i); else FData->Delete (i);
        }
      LoadCount = FData->Count;                  // ����� �������
     }

   return LoadCount;                             // ����� ����������� �������
  }
//---------------------------------------------------------------------------
int __fastcall TTextTable::SaveIntoFile(AnsiString Name)
  {
   AnsiString   SaveName;                        // ���� ���� ���������
   int          SaveCount = 0;                   // ����� ���������� �������

   // ���� �� ������� ���� ���������, ��������� ���� ������ ���������
   if ( Name.IsEmpty() ) SaveName = TableName; else SaveName = Name;
   if ( (FData!=NULL) && !SaveName.IsEmpty() )   // ���� ������ � ��� �����
     {
      FData->SaveToFile(SaveName);               // ���������
      SaveCount = FData->Count;                  // ����� �������
     }

   return SaveCount;                             // ����� ���������� �������
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::CreateTable()
  {
   // ������� ������ ���� �������
   if ( Active ) { DatabaseError("Close table before create it."); return; }
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::DeleteTable()
  {
   // ������� ������ ���� �������
   if ( Active ) { DatabaseError("Close table before delete it."); return; }

   // ���� ���� ������� ����������, �� ������ ���
   if ( FileExists(FTableName) ) DeleteFile(FTableName);
  }
//---------------------------------------------------------------------------
void __fastcall TTextTable::RenameTable(AnsiString NewName)
  {
   // ������� ������ ���� �������
   if ( Active ) { DatabaseError("Close table before rename it."); return; }

   // ���� ���� ������� ����������, �� ����������� ���
   if ( FileExists(FTableName) )
     { if ( RenameFile(FTableName,NewName) ) FTableName = NewName; }
  }
//---------------------------------------------------------------------------
namespace Textdataset                            // ����������� ����������
  {
   void __fastcall PACKAGE Register()
     {
      TComponentClass classes[1] = {__classid(TTextTable)};
      RegisterComponents("Data Access", classes, 0);
     }
  }
//---------------------------------------------------------------------------
