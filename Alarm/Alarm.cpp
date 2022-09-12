//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include "AppHelper.h"
#include "fLoginForm.h"
//---------------------------------------------------------------------------

USEFORM("Source\fLoginForm.cpp", LoginForm);
USEFORM("..\Form\ChartFormAlarm.cpp", FormTok);
USEFORM("PropertyUnit1.cpp", Form2);
USEFORM("AlarmUnit1.cpp", Form1);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
        apphelper = new TAppHelper();
        try
        {
            apphelper->Initialize();
		    Application->MainFormOnTaskBar = true;

            TLoginForm* fl = new TLoginForm(NULL, apphelper);
            try
            {
                if(fl->ShowModal() == mrCancel)
                    return 0;
            }
            __finally
            {
                delete fl;
            }


         Application->CreateForm(__classid(TForm1), &Form1);
         Application->CreateForm(__classid(TForm2), &Form2);
         Application->Run();
        }
        __finally
        {
            delete apphelper;
        }
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}

	}
	return 0;
}
//---------------------------------------------------------------------------
