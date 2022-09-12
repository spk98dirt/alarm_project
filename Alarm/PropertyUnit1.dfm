object Form2: TForm2
  Left = 716
  Top = 132
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = ' '#1050#1074#1080#1090#1080#1088#1086#1074#1072#1085#1080#1077' '#1090#1088#1077#1074#1086#1075#1080
  ClientHeight = 430
  ClientWidth = 446
  Color = clBtnFace
  Font.Charset = RUSSIAN_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'Arial'
  Font.Style = []
  FormStyle = fsStayOnTop
  Position = poDesktopCenter
  OnActivate = FormActivate
  OnClose = FormClose
  OnKeyPress = ListBox1KeyPress
  OnShow = FormShow
  TextHeight = 16
  object Label1: TLabel
    Left = 0
    Top = 284
    Width = 207
    Height = 16
    Caption = #1044#1086#1087#1086#1083#1085#1080#1090#1077#1083#1100#1085#1099#1081' '#1082#1086#1084#1084#1077#1085#1090#1072#1088#1080#1081
  end
  object Label2: TLabel
    Left = 0
    Top = 80
    Width = 124
    Height = 16
    Caption = #1058#1080#1087#1086#1074#1086#1077' '#1076#1077#1081#1089#1090#1074#1080#1077':'
  end
  object Label3: TLabel
    Left = 0
    Top = 8
    Width = 43
    Height = 16
    Caption = 'Label3'
  end
  object Label4: TLabel
    Left = 0
    Top = 24
    Width = 43
    Height = 16
    Caption = 'Label4'
  end
  object Label5: TLabel
    Left = 0
    Top = 48
    Width = 43
    Height = 16
    Caption = 'Label5'
  end
  object Memo1: TMemo
    Left = 0
    Top = 296
    Width = 441
    Height = 57
    Lines.Strings = (
      '')
    TabOrder = 0
    OnKeyPress = ListBox1KeyPress
  end
  object BitBtn1: TBitBtn
    Left = 232
    Top = 403
    Width = 95
    Height = 21
    Caption = #1050#1074#1080#1090#1080#1088#1086#1074#1072#1090#1100
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    OnClick = BitBtn1Click
  end
  object btnCancel: TBitBtn
    Left = 333
    Top = 402
    Width = 75
    Height = 21
    Caption = #1054#1090#1084#1077#1085#1080#1090#1100
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    OnClick = btnCancelClick
  end
  object CheckBox1: TCheckBox
    Left = 0
    Top = 373
    Width = 425
    Height = 24
    Caption = #1053#1077' '#1085#1072#1087#1086#1084#1080#1085#1072#1090#1100' '#1086' '#1090#1088#1077#1074#1086#1075#1077' '#1076#1086' '#1089#1083#1077#1076#1091#1102#1097#1077#1081' '#1089#1084#1077#1085#1099
    TabOrder = 3
    OnClick = CheckBox1Click
    OnKeyPress = ListBox1KeyPress
  end
  object AdvDateTimePicker1: TAdvDateTimePicker
    Left = 176
    Top = 349
    Width = 201
    Height = 25
    Date = 40823.000000000000000000
    Format = ''
    Time = 0.671481481484079300
    DoubleBuffered = True
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    Kind = dkDateTime
    ParentDoubleBuffered = False
    ParentFont = False
    TabOrder = 4
    Visible = False
    OnKeyPress = ListBox1KeyPress
    BorderStyle = bsSingle
    Ctl3D = True
    DateTime = 40823.671481481480000000
    Version = '1.3.6.5'
    LabelFont.Charset = DEFAULT_CHARSET
    LabelFont.Color = clWindowText
    LabelFont.Height = -11
    LabelFont.Name = 'Tahoma'
    LabelFont.Style = []
  end
  object ListBox1: TListBox
    Left = -3
    Top = 102
    Width = 441
    Height = 186
    AutoComplete = False
    TabOrder = 5
    OnKeyPress = ListBox1KeyPress
  end
  object btnRequest: TBitBtn
    Left = 0
    Top = 403
    Width = 95
    Height = 21
    Caption = #1047#1072#1103#1074#1082#1072
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 6
    OnClick = btnRequestClick
  end
end
