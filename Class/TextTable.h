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
#define  STRFLDSIZ      512                       // ��� ����� ���������� ����
#define  DELIMS    "sbift"                       // �������� ����� ������
#define  FLDDLM        '|'                       // ����������� �����
#define  OKCOUNT         3                       // �������� ��������
#define  MAXCHANGES     10                       // max % �� ������-�� ���������
#define  KSDLM         ' '                       // ���������� ����������� �����
#define  KSLEN           4                       // ����� ���� ����������� �����
//---------------------------------------------------------------------------
/// ����� ���� ���� � ��������� �����. �������� ����� ����������� ����� ��������� ����.
enum TFieldMark                                  // �������� ����� �����
  {
   fmStr   = 's',                                // ���������    : ftString
   fmBool  = 'b',                                // ����������   : ftBoolean
   fmInt   = 'i',                                // �����        : ftInteger
   fmFloat = 'f',                                // ������������ : ftFloat
   fmTime  = 't',                                 // ����\�����   : ftDataTime
   fmOnlyTime = 'o'                              // ����� ��� ����  ftTime 
  };
//---------------------------------------------------------------------------
struct TRecMark                                  // ��������
  {
   int                    Bookmark;              // ����� ��������
   TBookmarkFlag          BookmarkFlag;          // ����  ��������
  };
//---------------------------------------------------------------------------
struct TFieldParser                              // ������ ����
  {
   TFieldType             FieldType;             // ��� ����
   char                   FieldMark;             // ������ ����
   Word                   FieldSize;             // ������ ���� (���-�� ������)
   Word                   DataSize;              // ������ �����-� ������ ����

               __fastcall TFieldParser();        // �����������

   // �������������� �����_���� <-> ���_����
   static void __fastcall MarkToFieldType(char Mark, TFieldType & Type);
   static bool __fastcall FieldTypeToMark(TFieldType Type, char & Mark);

   // ������� ����������� ������ ���� �� ��� ���� ��� �������
   bool        __fastcall SetFieldType(TFieldType Type);
   bool        __fastcall SetFieldMark(char       Mark);

   // �������������� ����������_������������� <-> ���������_�������������
   bool        __fastcall BufferToString(char * Buf, AnsiString & Str);
   bool        __fastcall StringToBuffer(AnsiString & Str, char * Buf);
  };
//---------------------------------------------------------------------------
struct TRecordParser                             // ������ ������ �������
  {
   Word                   FieldCount ;           // ���������� �����
   AnsiString             FieldMarks ;           // ������ �������� �����
   Word                 * FieldOffs;             // ����� ���������� �����
   Word                   BufferSize ;           // ������ ������ ��� ������

               __fastcall TRecordParser();       // �����������

   // ������� �������� ������������ ������ ������
   static char __fastcall AccountCtrlSum(AnsiString Str, byte Len);
   static bool __fastcall ValidateString(AnsiString Str);

   AnsiString  __fastcall GetEmptyString();      // ������ � ������� ������

   // ������� ��������\�������\�������� ��������� ������ �������
   void        __fastcall ClearStructure ();    // �������
   void        __fastcall BuildStructure ();    // ���������� �� ������ ��������
   void        __fastcall CheckStructure ();    // �������� ����������-�� ������
   void        __fastcall ParseStructure (AnsiString Str);       // �������
   void        __fastcall ImportStructure(TFieldDefs * FldDefs); // ������
   void        __fastcall ExportStructure(TFieldDefs * FldDefs); // �������

   // �������������� ����������_������������� <-> ���������_�������������
   bool        __fastcall BufferToString(char * Buf, AnsiString & Str);
   bool        __fastcall StringToBuffer(AnsiString & Str, char * Buf);
  };
//---------------------------------------------------------------------------
class PACKAGE TTextTable : public TDataSet       // ������� ��� ��������� ����
{
private:
    TStringList            * FData;            // ������ �������
    TRecordParser            FRecStruct;       // ��������� ������
    int                      FCurRec;          // ����� ������� ������
    int                      FRecBufSize;      // ������ ������ ������+�������
    int                      FLastBookmark;    // ����� ��������� ��������
    AnsiString               FTableName;       // ��� ����� �������
    int                      FChangesCount;    // ���-�� �� ������-� ���������
    volatile long            IsLocked;
    CRITICAL_SECTION         cs_TextTable;

protected:
    // ������� ������ � ����������
    TBookmarkFlag __fastcall GetBookmarkFlag(char * Buffer);
    void          __fastcall SetBookmarkFlag(char * Buffer, TBookmarkFlag Value);
    void          __fastcall GetBookmarkData(char * Buffer, void * Data);
    void          __fastcall SetBookmarkData(char * Buffer, void * Data);

    // ������� ������ � ������� � ��������� �� ���
    char *        __fastcall AllocRecordBuffer1);
    void          __fastcall FreeRecordBuffer (char * & Buffer);
    WORD          __fastcall GetRecordSize    ();
    int           __fastcall GetRecordCount   ();
    int           __fastcall GetRecNo         ();
    void          __fastcall SetRecNo         (int Value);
    TGetResult    __fastcall GetRecord        (char * Buffer, TGetMode GetMode, bool DoCheck);

    // ���������� ��������� ������� ��� ��������� ����
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

    /// ��������� ����������� ����� ��� ���� ������ � �������� � � ������.
    // @param fieldRecord ����������� ������, ���������� �������� ���� ����� ��� ����������� �����.
    static AnsiString __fastcall insertCtrlSumToFiledRecordString(AnsiString strRecord);

    /// �������� ����� ���� ����.
    // @param fieldType ��� ����.
    static TFieldMark __fastcall getFieldMark(TFieldType fieldType);

    /// �������� ��������� ������������� �������� ����.
    // @param fieldType ��� ����.
    // @param recordBuf ������ � �������. ��������� ������ � ������ ������� �� fieldType.
    static AnsiString __fastcall getRecordStr(TFieldType fieldType, char *recordBuf);

    /// �������� ��������� ������������� �������� ����.
    // @param value ��������.
    static AnsiString __fastcall getRecordStr(TDateTime value);

    /// �������� ��������� ������������� �������� ����.
    // @param value ��������.
    static AnsiString __fastcall getRecordStr(double value);

    /// �������� ��������� ������������� �������� ����.
    // @param value ��������.
    static AnsiString __fastcall getRecordStr(int value);

    /// �������� ������ � ����.
    // @param fileName ��� �����.
    // @param recordStr ��������� ������������� ������.
    // @return true ���� �������, false ���� ������.
    static bool __fastcall writeRecordToFile(AnsiString fileName, AnsiString recordStr);

public:

      TNotifyEvent              OnNullStructure;               // ��� ���������

                    __fastcall  TTextTable(TComponent* Owner); // �����������
                    __fastcall  ~TTextTable();


      // ������� ������ � ���������� �������������� �����
      void  __fastcall SetFieldData(TField * Field, void * Buffer);
      bool  __fastcall GetFieldData(TField * Field, void * Buffer);

      // ������� ������ � ������ �������
      int           __fastcall LoadFromFile(AnsiString Name);  // �������� ��
      int           __fastcall SaveIntoFile(AnsiString Name);  // ������ �

      bool                     appendToFile(AnsiString fileName);

      // ����� ��� ����������� ������� TTable
      void          __fastcall CreateTable();                  // �������� ������
      void          __fastcall DeleteTable();                  // ��������
      void          __fastcall RenameTable(AnsiString NewName);// ��������������

      void          __fastcall Lock();
      void          __fastcall Unlock();
      int           __fastcall GetRecCount();

    /// �������� ������ � ����� �������.
    // @param fileName ���� �������.
    // @param time ���� 1: �����.
    // @param val ���� 2: ��������.
    // @return true ���� �������, false ���� �� ������� ������� ����.
    static bool appendValToTableFile(AnsiString fileName, TDateTime time, double val);

    /// �� ��, ��� � TTextTable::appendValToTableFile(AnsiString fileName, TDateTime time, double val)
    static bool appendValToTableFile(AnsiString fileName, TDateTime time, int val);

__published:

      __property AnsiString    TableName = {read = FTableName, write = FTableName};
      __property               Active;
  };
//---------------------------------------------------------------------------
#endif
