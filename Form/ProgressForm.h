//---------------------------------------------------------------------------

#ifndef ProgressFormH
#define ProgressFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormProgress1 : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TProgressBar *ProgressBar1;
private:	// User declarations
public:		// User declarations
	__fastcall TFormProgress1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormProgress1 *FormProgress1;
//---------------------------------------------------------------------------
#endif
