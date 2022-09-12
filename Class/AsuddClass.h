#ifndef AsuddClassH
#define AsuddClassH

#include <System.SysUtils.hpp>
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

/**
* ��������� ������
*/
struct TKAsuddPackage
{
// ��������� ������
	char   Signature[4];
	UInt8  pid;
	UInt8  RepliedTo;
//	UInt16 DataSize;
	UInt8  HeaderCS;

};

/*
class TKAsudd public TObject
{

};
*/


#endif
