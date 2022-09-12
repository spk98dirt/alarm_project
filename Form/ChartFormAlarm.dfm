object FormTok: TFormTok
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'FormTok'
  ClientHeight = 562
  ClientWidth = 1028
  Color = clBtnFace
  Constraints.MinHeight = 600
  Constraints.MinWidth = 1000
  ParentFont = True
  OnCreate = FormActivate
  OnResize = FormResize
  OnShow = FormShow
  TextHeight = 25
  object Panel5: TPanel
    Left = 0
    Top = 0
    Width = 1028
    Height = 562
    Align = alClient
    TabOrder = 0
    object Label11: TLabel
      Left = 1
      Top = 36
      Width = 1026
      Height = 13
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ExplicitWidth = 3
    end
    object Panel6: TPanel
      Left = 1
      Top = 49
      Width = 1026
      Height = 512
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      object AdvSplitter2: TAdvSplitter
        Left = 768
        Top = 0
        Width = 6
        Height = 512
        Align = alRight
        AutoSnap = False
        Appearance.BorderColor = clNone
        Appearance.BorderColorHot = clNone
        Appearance.Color = clWhite
        Appearance.ColorTo = clSilver
        Appearance.ColorHot = clWhite
        Appearance.ColorHotTo = clGray
        GripStyle = sgDots
        OnClick = AdvSplitter2Click
        ExplicitLeft = 730
        ExplicitHeight = 522
      end
      object Panel2: TPanel
        Left = 0
        Top = 0
        Width = 768
        Height = 512
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        object ToolBar1: TToolBar
          Left = 0
          Top = 0
          Width = 768
          Height = 40
          ButtonHeight = 33
          ButtonWidth = 32
          Caption = 'ToolBar1'
          EdgeInner = esLowered
          EdgeOuter = esNone
          Images = ImageList1
          TabOrder = 0
          object Label2: TLabel
            Left = 0
            Top = 0
            Width = 5
            Height = 25
            Layout = tlCenter
          end
          object DateTimePicker1: TAdvDateTimePicker
            Left = 5
            Top = 0
            Width = 100
            Height = 33
            Hint = #1053#1072#1095#1072#1083#1100#1085#1072#1103' '#1076#1072#1090#1072
            Date = 41659.000000000000000000
            Format = ''
            Time = 0.662557870367891200
            DoubleBuffered = True
            Kind = dkDate
            ParentDoubleBuffered = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
            BorderStyle = bsSingle
            Ctl3D = True
            DateTime = 41659.662557870370000000
            Version = '1.3.6.5'
            LabelFont.Charset = DEFAULT_CHARSET
            LabelFont.Color = clWindowText
            LabelFont.Height = -18
            LabelFont.Name = 'Segoe UI'
            LabelFont.Style = []
          end
          object Label3: TLabel
            Left = 105
            Top = 0
            Width = 21
            Height = 25
            Alignment = taCenter
            Caption = #1087#1086
            Layout = tlCenter
          end
          object DateTimePicker2: TAdvDateTimePicker
            Left = 126
            Top = 0
            Width = 100
            Height = 33
            Hint = #1050#1086#1085#1077#1095#1085#1072#1103' '#1076#1072#1090#1072
            Date = 41659.000000000000000000
            Format = ''
            Time = 0.662557870367891200
            DoubleBuffered = True
            Kind = dkDate
            ParentDoubleBuffered = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 0
            BorderStyle = bsSingle
            Ctl3D = True
            DateTime = 41659.662557870370000000
            Version = '1.3.6.5'
            LabelFont.Charset = DEFAULT_CHARSET
            LabelFont.Color = clWindowText
            LabelFont.Height = -18
            LabelFont.Name = 'Segoe UI'
            LabelFont.Style = []
          end
          object ToolButton3: TToolButton
            Left = 226
            Top = 0
            Hint = #1055#1086#1089#1090#1088#1086#1080#1090#1100' '#1075#1088#1072#1092#1080#1082
            ImageIndex = 2
            ParentShowHint = False
            ShowHint = True
            OnClick = AdvOfficeCheckBox1Click
          end
          object ToolButton6: TToolButton
            Left = 258
            Top = 0
            Width = 39
            Caption = 'ToolButton6'
            ImageIndex = 4
            Style = tbsSeparator
          end
          object ToolButton4: TToolButton
            Left = 297
            Top = 0
            Width = 4
            Caption = 'ToolButton4'
            ImageIndex = 3
            Style = tbsSeparator
            Visible = False
          end
          object ToolButton5: TToolButton
            Left = 301
            Top = 0
            Hint = #1040#1074#1090#1086#1086#1073#1085#1086#1074#1083#1077#1085#1080#1077' '#1075#1088#1072#1092#1080#1082#1072
            Caption = 'ToolButton5'
            ImageIndex = 3
            ParentShowHint = False
            ShowHint = True
            Visible = False
            OnClick = ToolButton5Click
          end
          object Label4: TLabel
            Left = 333
            Top = 0
            Width = 5
            Height = 25
          end
          object ToolButton1: TToolButton
            Left = 338
            Top = 0
            Hint = #1069#1082#1089#1087#1086#1088#1090' '#1076#1072#1085#1085#1099#1093' '#1074' Excel'
            ImageIndex = 0
            ParentShowHint = False
            ShowHint = True
            OnClick = ToolButton1Click
          end
          object ToolButton2: TToolButton
            Left = 370
            Top = 0
            Hint = #1050#1086#1087#1080#1088#1086#1074#1072#1085#1080#1077' '#1075#1088#1072#1092#1080#1082#1072' '#1074' '#1073#1091#1092#1077#1088' '#1086#1073#1084#1077#1085#1072
            ImageIndex = 1
            ParentShowHint = False
            ShowHint = True
            OnClick = ToolButton2Click
          end
        end
        object Panel4: TPanel
          Left = 0
          Top = 40
          Width = 768
          Height = 144
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          object Label5: TLabel
            Left = 38
            Top = 4
            Width = 5
            Height = 25
            Caption = 'I'
          end
          object Label6: TLabel
            Left = 75
            Top = 4
            Width = 17
            Height = 25
            Caption = ' U'
          end
          object Label7: TLabel
            Left = 103
            Top = 4
            Width = 66
            Height = 25
            Caption = #1058#1088#1077#1074#1086#1075#1072
          end
          object CheckAL: TAdvOfficeCheckBox
            Left = 8
            Top = 111
            Width = 65
            Height = 20
            Hint = #1043#1088#1072#1092#1080#1082' '#1087#1072#1088#1072#1084#1077#1090#1088#1086#1074
            ShowHint = True
            TabOrder = 0
            OnClick = CheckIAClick
            Alignment = taLeftJustify
            Caption = ''
            ReturnIsTab = False
            Version = '1.8.1.2'
          end
          object CheckIA: TAdvOfficeCheckBox
            Left = 8
            Top = 20
            Width = 40
            Height = 20
            Hint = #1043#1088#1072#1092#1080#1082' '#1090#1086#1082#1072' '#1092#1072#1079#1099' A'
            Color = clHighlight
            Checked = True
            ParentColor = False
            ShowHint = True
            TabOrder = 1
            OnClick = CheckIAClick
            Alignment = taRightJustify
            Caption = 'A'
            ReturnIsTab = False
            State = cbChecked
            Version = '1.8.1.2'
          end
          object CheckIB: TAdvOfficeCheckBox
            Left = 8
            Top = 40
            Width = 40
            Height = 20
            Hint = #1043#1088#1072#1092#1080#1082' '#1090#1086#1082#1072' '#1092#1072#1079#1099' B'
            Checked = True
            ShowHint = True
            TabOrder = 2
            OnClick = CheckIAClick
            Alignment = taRightJustify
            Caption = 'B'
            ReturnIsTab = False
            State = cbChecked
            Version = '1.8.1.2'
          end
          object CheckIC: TAdvOfficeCheckBox
            Left = 8
            Top = 60
            Width = 40
            Height = 20
            Hint = #1043#1088#1072#1092#1080#1082' '#1090#1086#1082#1072' '#1092#1072#1079#1099' C'
            Checked = True
            ShowHint = True
            TabOrder = 3
            OnClick = CheckIAClick
            Alignment = taRightJustify
            Caption = 'C'
            ReturnIsTab = False
            State = cbChecked
            Version = '1.8.1.2'
          end
          object CheckSV: TAdvOfficeCheckBox
            Left = 8
            Top = 85
            Width = 80
            Height = 20
            Hint = #1043#1088#1072#1092#1080#1082' '#1089#1074#1103#1079#1080
            Checked = True
            ShowHint = True
            TabOrder = 4
            OnClick = CheckIAClick
            Alignment = taLeftJustify
            Caption = '  '#1057#1074#1103#1079#1100
            ReturnIsTab = False
            State = cbChecked
            Version = '1.8.1.2'
          end
          object CheckUa: TAdvOfficeCheckBox
            Left = 75
            Top = 20
            Width = 20
            Height = 20
            Hint = #1043#1088#1072#1092#1080#1082' '#1085#1072#1087#1088#1103#1078#1077#1085#1080#1103' '#1092#1072#1079#1099' A'
            ShowHint = True
            TabOrder = 5
            OnClick = CheckIAClick
            Alignment = taLeftJustify
            Caption = ''
            ReturnIsTab = False
            Version = '1.8.1.2'
          end
          object CheckUb: TAdvOfficeCheckBox
            Left = 75
            Top = 40
            Width = 20
            Height = 20
            Hint = #1043#1088#1072#1092#1080#1082' '#1085#1072#1087#1088#1103#1078#1077#1085#1080#1103' '#1092#1072#1079#1099' B'
            ShowHint = True
            TabOrder = 6
            OnClick = CheckIAClick
            Alignment = taLeftJustify
            Caption = ''
            ReturnIsTab = False
            Version = '1.8.1.2'
          end
          object CheckUc: TAdvOfficeCheckBox
            Left = 75
            Top = 60
            Width = 20
            Height = 20
            Hint = #1043#1088#1072#1092#1080#1082' '#1085#1072#1087#1088#1103#1078#1077#1085#1080#1103' '#1092#1072#1079#1099' C'
            ShowHint = True
            TabOrder = 7
            OnClick = CheckIAClick
            Alignment = taLeftJustify
            Caption = ''
            ReturnIsTab = False
            Version = '1.8.1.2'
          end
          object CheckGr: TAdvOfficeCheckBox
            Left = 430
            Top = 25
            Width = 217
            Height = 20
            Hint = #1043#1088#1072#1092#1080#1082' '#1075#1088#1072#1085#1080#1094' '#1074#1099#1073#1088#1072#1085#1085#1099#1093' '#1087#1072#1088#1072#1084#1077#1090#1088#1086#1074
            ShowHint = True
            TabOrder = 8
            OnClick = CheckIAClick
            Alignment = taLeftJustify
            Caption = #1043#1088#1072#1085#1080#1094#1099' '#1091' '#1074#1099#1073#1088#1072#1085#1085#1099#1093' '#1087#1072#1088#1072#1084#1077#1090#1088#1086#1074
            ReturnIsTab = False
            Version = '1.8.1.2'
          end
          object CheckTVO: TAdvOfficeCheckBox
            Left = 585
            Top = 84
            Width = 115
            Height = 20
            Hint = #1043#1088#1072#1092#1080#1082' '#1089#1087#1088#1072#1074#1086#1095#1085#1099#1093' '#1079#1085#1072#1095#1077#1085#1080#1081' '#1074#1082#1083'-'#1086#1090#1082#1083
            ShowHint = True
            TabOrder = 9
            Visible = False
            OnClick = CheckIAClick
            Alignment = taLeftJustify
            Caption = #1058#1086#1095#1082#1080' '#1074#1082#1083'-'#1086#1090#1082#1083
            ReturnIsTab = False
            Version = '1.8.1.2'
          end
          object CheckTZN: TAdvOfficeCheckBox
            Left = 430
            Top = 4
            Width = 115
            Height = 20
            Hint = #1043#1088#1072#1092#1080#1082' '#1092#1072#1082#1090#1080#1095#1077#1089#1082#1080#1093' '#1079#1085#1072#1095#1077#1085#1080#1081' '#1074#1082#1083'-'#1086#1090#1082#1083
            ShowHint = True
            TabOrder = 10
            OnClick = CheckIAClick
            Alignment = taLeftJustify
            Caption = #1058#1086#1095#1082#1080' '#1079#1085#1072#1095#1077#1085#1080#1081
            ReturnIsTab = False
            Version = '1.8.1.2'
          end
          object CheckBox: TAdvOfficeCheckBox
            Left = 430
            Top = 46
            Width = 204
            Height = 20
            Hint = #1055#1077#1088#1077#1082#1083#1102#1095#1077#1085#1080#1077' '#1055#1086#1076#1088#1086#1073#1085#1099#1081' '#1075#1088#1072#1092#1080#1082'/'#1059#1089#1088#1077#1076#1085#1077#1085#1085#1099#1081
            ShowHint = True
            TabOrder = 11
            OnClick = CheckBoxClick
            Alignment = taLeftJustify
            Caption = #1059#1089#1088#1077#1076#1085#1077#1085#1085#1099#1081' '#1075#1088#1072#1092#1080#1082
            ReturnIsTab = False
            Version = '1.8.1.2'
          end
          object ComboBox1: TComboBox
            Left = 28
            Top = 112
            Width = 199
            Height = 33
            Hint = #1043#1088#1072#1092#1080#1082' '#1087#1072#1088#1072#1084#1077#1090#1088#1086#1074
            DoubleBuffered = False
            ParentColor = True
            ParentDoubleBuffered = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 12
            Text = #1055#1072#1088#1072#1084#1077#1090#1088#1099
            OnClick = ComboBox1Click
            Items.Strings = (
              #1052#1077#1078#1092#1072#1079#1085#1086#1077' '#1079#1072#1084#1099#1082#1072#1085#1080#1077' '#1092'. A'
              #1052#1077#1078#1092#1072#1079#1085#1086#1077' '#1079#1072#1084#1099#1082#1072#1085#1080#1077' '#1092'. B'
              #1052#1077#1078#1092#1072#1079#1085#1086#1077' '#1079#1072#1084#1099#1082#1072#1085#1080#1077' '#1092'. C'
              #1050#1047' '#1092'. A'
              #1050#1047' '#1092'. B'
              #1050#1047' '#1092'. C'
              #1055#1077#1088#1077#1075#1088#1091#1079#1082#1072' '#1087#1086' '#1090#1086#1082#1091' '#1092'. A'
              #1055#1077#1088#1077#1075#1088#1091#1079#1082#1072' '#1087#1086' '#1090#1086#1082#1091' '#1092'. B'
              #1055#1077#1088#1077#1075#1088#1091#1079#1082#1072' '#1087#1086' '#1090#1086#1082#1091' '#1092'. C'
              #1054#1093#1088#1072#1085#1072
              #1058#1091#1084#1073#1083#1077#1088
              #1044#1074#1077#1088#1100)
          end
          object CheckAa: TAdvOfficeCheckBox
            Left = 120
            Top = 20
            Width = 20
            Height = 20
            Hint = #1058#1088#1077#1074#1086#1075#1080' '#1092#1072#1079#1099' '#1040
            ShowHint = True
            TabOrder = 13
            OnClick = CheckIAClick
            Alignment = taLeftJustify
            Caption = ''
            ReturnIsTab = False
            Version = '1.8.1.2'
          end
          object CheckAb: TAdvOfficeCheckBox
            Left = 120
            Top = 40
            Width = 20
            Height = 20
            Hint = #1058#1088#1077#1074#1086#1075#1080' '#1092#1072#1079#1099' '#1042
            ShowHint = True
            TabOrder = 14
            OnClick = CheckIAClick
            Alignment = taLeftJustify
            Caption = ''
            ReturnIsTab = False
            Version = '1.8.1.2'
          end
          object CheckAc: TAdvOfficeCheckBox
            Left = 120
            Top = 60
            Width = 20
            Height = 20
            Hint = #1058#1088#1077#1074#1086#1075#1080' '#1092#1072#1079#1099' '#1057
            ShowHint = True
            TabOrder = 15
            OnClick = CheckIAClick
            Alignment = taLeftJustify
            Caption = ''
            ReturnIsTab = False
            Version = '1.8.1.2'
          end
          object CheckListBox1: TCheckListBox
            Left = 183
            Top = 5
            Width = 241
            Height = 99
            Hint = #1057#1087#1080#1089#1086#1082' '#1090#1088#1077#1074#1086#1075
            ItemHeight = 25
            ParentShowHint = False
            ShowHint = True
            TabOrder = 16
            OnClick = CheckListBox1Click
          end
          object CheckListEdit1: TCheckListEdit
            Left = 430
            Top = 96
            Width = 154
            Height = 33
            Hint = #1058#1088#1077#1074#1086#1075#1080
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -18
            Font.Name = 'Segoe UI'
            Font.Style = []
            ParentFont = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 17
            Visible = False
            AutoDropWidthSize = False
            DropWidth = 121
            DropFont.Charset = DEFAULT_CHARSET
            DropFont.Color = clWindowText
            DropFont.Height = -12
            DropFont.Name = 'Tahoma'
            DropFont.Style = []
            LabelFont.Charset = DEFAULT_CHARSET
            LabelFont.Color = clWindowText
            LabelFont.Height = -90
            LabelFont.Name = 'Segoe UI'
            LabelFont.Style = []
            TextDelimiter = ','
            TextEndChar = ']'
            TextStartChar = '['
            Version = '1.4.4.5'
          end
          object ComboBox2: TComboBox
            Left = 28
            Top = 85
            Width = 141
            Height = 33
            Hint = #1043#1088#1072#1092#1080#1082' '#1089#1074#1103#1079#1080
            ParentColor = True
            ParentShowHint = False
            ShowHint = True
            TabOrder = 18
            Text = #1057#1074#1103#1079#1100
            Visible = False
          end
          object AdvOfficeCheckBox1: TAdvOfficeCheckBox
            Left = 430
            Top = 70
            Width = 204
            Height = 20
            Hint = #1043#1088#1072#1092#1080#1082' '#1079#1072' '#1076#1080#1089#1087#1077#1090#1095#1077#1088#1089#1082#1080#1077' '#1089#1091#1090#1082#1080
            ShowHint = True
            TabOrder = 19
            OnClick = AdvOfficeCheckBox1Click
            Alignment = taLeftJustify
            Caption = #1044#1080#1089#1087#1077#1090#1095#1077#1088#1089#1082#1080#1077' '#1089#1091#1090#1082#1080
            ReturnIsTab = False
            Version = '1.8.1.2'
          end
        end
        object Panel9: TPanel
          Left = 0
          Top = 184
          Width = 768
          Height = 328
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 2
          object Chart1: TChart
            Left = 0
            Top = 0
            Width = 768
            Height = 328
            Legend.Visible = False
            Title.Font.Color = clBlack
            Title.Font.Height = -19
            Title.Font.Style = [fsBold]
            Title.Text.Strings = (
              '')
            Title.Visible = False
            AxisBehind = False
            BottomAxis.DateTimeFormat = 'mm.dd.yy'
            BottomAxis.ExactDateTime = False
            BottomAxis.Increment = 0.041700000000000000
            DepthAxis.Automatic = False
            DepthAxis.AutomaticMaximum = False
            DepthAxis.AutomaticMinimum = False
            DepthAxis.EndPosition = 1.000000000000000000
            DepthTopAxis.Automatic = False
            DepthTopAxis.AutomaticMaximum = False
            DepthTopAxis.AutomaticMinimum = False
            DepthTopAxis.Title.Caption = 'ryjiyujjyjiuyjtyuku'
            LeftAxis.Automatic = False
            LeftAxis.AutomaticMaximum = False
            LeftAxis.AutomaticMinimum = False
            LeftAxis.ExactDateTime = False
            LeftAxis.Increment = 3.000000000000000000
            LeftAxis.LabelStyle = talValue
            LeftAxis.Maximum = 472.912234424798200000
            LeftAxis.MaximumOffset = 10
            LeftAxis.Title.Caption = #1058#1086#1082', '#1040
            LeftAxis.Title.Font.Height = -13
            LeftAxis.TitleSize = 4
            Panning.MouseWheel = pmwNone
            RightAxis.Automatic = False
            RightAxis.AutomaticMaximum = False
            RightAxis.AutomaticMinimum = False
            RightAxis.ExactDateTime = False
            RightAxis.Increment = 10.000000000000000000
            RightAxis.Maximum = 250.000000000000000000
            RightAxis.Minimum = 180.000000000000000000
            RightAxis.Title.Angle = 90
            RightAxis.Title.Caption = #1053#1072#1087#1088#1103#1078#1077#1085#1080#1077', '#1042
            RightAxis.Title.Font.Height = -13
            RightAxis.TitleSize = 4
            TopAxis.Automatic = False
            TopAxis.AutomaticMaximum = False
            TopAxis.AutomaticMinimum = False
            View3D = False
            View3DOptions.Orthogonal = False
            Zoom.MinimumPixels = 12
            Align = alClient
            BevelOuter = bvNone
            TabOrder = 0
            DefaultCanvas = 'TGDIPlusCanvas'
            PrintMargins = (
              15
              21
              15
              21)
            ColorPaletteIndex = 13
            object Series1: TGanttSeries
              HoverElement = [heCurrent]
              Active = False
              Marks.Emboss.Color = 8487297
              Marks.Shadow.Color = 8684676
              Marks.Shadow.Visible = False
              Marks.Visible = True
              ClickableLine = False
              Pointer.Brush.Gradient.EndColor = 6144242
              Pointer.Gradient.EndColor = 6144242
              Pointer.InflateMargins = True
              Pointer.Style = psRectangle
              XValues.Name = 'Start'
              XValues.Order = loAscending
              YValues.Name = 'Y'
              YValues.Order = loNone
              Callout.Style = psRightTriangle
              Callout.Arrow.Visible = False
              StartValues.Name = 'Start'
              StartValues.Order = loAscending
              EndValues.Name = 'End'
              EndValues.Order = loNone
              NextTask.Name = 'NextTask'
              NextTask.Order = loNone
            end
          end
        end
      end
      object Panel3: TPanel
        Left = 774
        Top = 0
        Width = 252
        Height = 512
        Align = alRight
        BevelOuter = bvNone
        Constraints.MaxHeight = 900
        Constraints.MinWidth = 250
        TabOrder = 1
        object AdvStringGrid1: TAdvStringGrid
          Left = 0
          Top = 0
          Width = 252
          Height = 512
          Align = alClient
          ColCount = 9
          DrawingStyle = gdsClassic
          FixedCols = 0
          RowCount = 1
          FixedRows = 0
          Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
          ParentShowHint = False
          ScrollBars = ssVertical
          ShowHint = True
          TabOrder = 0
          HintShowCells = True
          ActiveCellFont.Charset = DEFAULT_CHARSET
          ActiveCellFont.Color = clWindowText
          ActiveCellFont.Height = -11
          ActiveCellFont.Name = 'Tahoma'
          ActiveCellFont.Style = [fsBold]
          ControlLook.FixedGradientHoverFrom = clGray
          ControlLook.FixedGradientHoverTo = clWhite
          ControlLook.FixedGradientDownFrom = clGray
          ControlLook.FixedGradientDownTo = clSilver
          ControlLook.DropDownHeader.Font.Charset = DEFAULT_CHARSET
          ControlLook.DropDownHeader.Font.Color = clWindowText
          ControlLook.DropDownHeader.Font.Height = -11
          ControlLook.DropDownHeader.Font.Name = 'Tahoma'
          ControlLook.DropDownHeader.Font.Style = []
          ControlLook.DropDownHeader.Visible = True
          ControlLook.DropDownHeader.Buttons = <>
          ControlLook.DropDownFooter.Font.Charset = DEFAULT_CHARSET
          ControlLook.DropDownFooter.Font.Color = clWindowText
          ControlLook.DropDownFooter.Font.Height = -11
          ControlLook.DropDownFooter.Font.Name = 'Tahoma'
          ControlLook.DropDownFooter.Font.Style = []
          ControlLook.DropDownFooter.Visible = True
          ControlLook.DropDownFooter.Buttons = <>
          Filter = <>
          FilterDropDown.Font.Charset = DEFAULT_CHARSET
          FilterDropDown.Font.Color = clWindowText
          FilterDropDown.Font.Height = -11
          FilterDropDown.Font.Name = 'Tahoma'
          FilterDropDown.Font.Style = []
          FilterDropDownClear = '(All)'
          FilterEdit.TypeNames.Strings = (
            'Starts with'
            'Ends with'
            'Contains'
            'Not contains'
            'Equal'
            'Not equal'
            'Larger than'
            'Smaller than'
            'Clear')
          FixedColWidth = 20
          FixedRowHeight = 22
          FixedFont.Charset = DEFAULT_CHARSET
          FixedFont.Color = clWindowText
          FixedFont.Height = -16
          FixedFont.Name = 'Tahoma'
          FixedFont.Style = [fsBold]
          FloatFormat = '%.2f'
          HoverButtons.Buttons = <>
          HTMLSettings.ImageFolder = 'images'
          HTMLSettings.ImageBaseName = 'img'
          PrintSettings.DateFormat = 'dd/mm/yyyy'
          PrintSettings.Font.Charset = DEFAULT_CHARSET
          PrintSettings.Font.Color = clWindowText
          PrintSettings.Font.Height = -11
          PrintSettings.Font.Name = 'Tahoma'
          PrintSettings.Font.Style = []
          PrintSettings.FixedFont.Charset = DEFAULT_CHARSET
          PrintSettings.FixedFont.Color = clWindowText
          PrintSettings.FixedFont.Height = -11
          PrintSettings.FixedFont.Name = 'Tahoma'
          PrintSettings.FixedFont.Style = []
          PrintSettings.HeaderFont.Charset = DEFAULT_CHARSET
          PrintSettings.HeaderFont.Color = clWindowText
          PrintSettings.HeaderFont.Height = -11
          PrintSettings.HeaderFont.Name = 'Tahoma'
          PrintSettings.HeaderFont.Style = []
          PrintSettings.FooterFont.Charset = DEFAULT_CHARSET
          PrintSettings.FooterFont.Color = clWindowText
          PrintSettings.FooterFont.Height = -11
          PrintSettings.FooterFont.Name = 'Tahoma'
          PrintSettings.FooterFont.Style = []
          PrintSettings.PageNumSep = '/'
          ScrollBarAlways = saVert
          ScrollWidth = 26
          SearchFooter.FindNextCaption = 'Find &next'
          SearchFooter.FindPrevCaption = 'Find &previous'
          SearchFooter.Font.Charset = DEFAULT_CHARSET
          SearchFooter.Font.Color = clWindowText
          SearchFooter.Font.Height = -11
          SearchFooter.Font.Name = 'Tahoma'
          SearchFooter.Font.Style = []
          SearchFooter.HighLightCaption = 'Highlight'
          SearchFooter.HintClose = 'Close'
          SearchFooter.HintFindNext = 'Find next occurrence'
          SearchFooter.HintFindPrev = 'Find previous occurrence'
          SearchFooter.HintHighlight = 'Highlight occurrences'
          SearchFooter.MatchCaseCaption = 'Match case'
          SearchFooter.ResultFormat = '(%d of %d)'
          ShowDesignHelper = False
          Version = '8.6.14.2'
          ColWidths = (
            20
            20
            20
            20
            20
            64
            64
            64
            64)
          RowHeights = (
            22)
        end
      end
    end
    object Panel8: TPanel
      Left = 1
      Top = 1
      Width = 1026
      Height = 35
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 1
      object LabelPP: TLabel
        Left = 0
        Top = 13
        Width = 38
        Height = 13
        Align = alLeft
        Caption = #8470' '#1055#1042': '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object LabelAD: TLabel
        Left = 983
        Top = 13
        Width = 43
        Height = 13
        Align = alRight
        Alignment = taRightJustify
        Caption = #1040#1076#1088#1077#1089': '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label8: TLabel
        Left = 0
        Top = 0
        Width = 3
        Height = 13
        Align = alTop
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
    end
  end
  object UniQuery1: TUniQuery
    Left = 600
    Top = 144
  end
  object ImageList1: TImageList
    Height = 24
    Width = 24
    Left = 816
    Bitmap = {
      494C01010500A001040018001800FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000600000003000000001002000000000000048
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000404A8000404A8000404A8000404A8000404A8000404A8000404
      A8000404A8000404A80000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000404A8000404A8000404A8000404A8000404A8000404A8000404
      A8000404A8000404A80000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000D9C1B600C9A89900B07F6B00B1816C00B080
      6A00EEEEEE000404A8000404A8000404A8000404A8000404A8000404A8000404
      A8000404A8000404A80000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000B1806C0000000000CDAD9F008E482900873B1B00893F1F00863B1A00863A
      1900EEEEEE000404A8000404A8000404A8000404A8000404A8000404A8000404
      A8000404A8000404A80000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000B68A7700904C2E00924E31008E4729008C4424008D462800955336009655
      3800EEEEEE000404A8000404A8000404A8000404A8000404A8000404A8000404
      A8000404A8000404A80000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000CDAFA200833614008E492A008F4A2C00A46C5300B6887400CBAB9E00D2B5
      AA00EEEEEE000404A8000404A8000404A8000404A8000404A8000404A8000404
      A8000404A8000404A80000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000E9DCD6008C4526008439170080300E00AC79620000000000000000000000
      0000000000000404A8000404A8000404A8000404A8000404A8000404A8000404
      A8000404A8000404A80000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000CFB1A300C7A59600B98F7B00B1806A00DECAC300000000000000
      0000000000000404A8000404A8000404A8000404A8000404A8000404A8000404
      A8000404A8000404A80000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000404A8000404A8000404A8000404A8000404A8000404A8000404
      A8000404A8000404A80000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000D5BCB000EEEEEE00EEEEEE00EEEE
      EE00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000CAA99B00A871
      5900AB776000D2B8AC0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000E9DCD600CCAB9D00A6715800B386
      7000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D0B3A6008B43
      2300883E1E00B98E7A0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000AE7B
      6600873D1D0095533600E1CFC600000000000000000000000000000000000000
      000000000000CCAD9F0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000D9C1
      B700863D1D00894120009E634800EADCD6000000000000000000000000000000
      000000000000CEAFA20095523600904B2D00883E1F00AE7C6600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000B1806C008C44250084381600A0664D00CDAFA200E9DCD600000000000000
      000000000000DFCCC4009F6249008B442400894020009E624800E4D2CA000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000BF978500934F320081341200914E3100A9745C00AD7A6400AC78
      6200A8735C0095543700914C2E008D462700873D1C008E482A00E9DCD6000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000CCAC9F00965438008335130084381700863B1A00873D
      1D00883E1F008C432400873D1D0096543700BE968400B1826D00D7BFB3000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000DBC5BA00B88B7800955438008C452600873C
      1B00873C1B008C442500A2684F00D4BAAF000000000000000000D4BAAE000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000CFB1A300CAA9
      9B00C8A59600CFB1A400E9DCD600000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000072727200606060005D5D
      5D005D5D5D005D5D5D005D5D5D005D5D5D005D5D5D005D5D5D005D5D5D005D5D
      5D005D5D5D005E5E5E006E6E6E00D5D5D5000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000772B00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000072727200606060005D5D5D005D5D5D005D5D
      5D005D5D5D005D5D5D005D5D5D005D5D5D005D5D5D005D5D5D005D5D5D005E5E
      5E006E6E6E00D9D9D9000000000000000000000000005D5D5D00B8B8B800D5D5
      D500D5D5D500D5D5D500D5D5D500D5D5D500D5D5D500D5D5D500D5D5D500D5D5
      D500D5D5D500C0C0C0005C5C5C00D5D5D5000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000772B0000772B000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000005D5D5D00B8B8B800D1D1D100D1D1D100D1D1
      D100D1D1D100D1D1D100D1D1D100D1D1D100D1D1D100D1D1D100D1D1D100C0C0
      C0005C5C5C00CCCCCC0000000000000000000000000063636300FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC0066666600D5D5D500D5D5D500D5D5D500D5D5D500D5D5
      D500D5D5D500D5D5D500D5D5D500000000000000000000000000000000000000
      000000000000000000000000000000000000DCC7BD00BE958400B6897600B88B
      7800B5887400C7A3940000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000772B0000772B0000772B0000772B00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000063636300FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC0066666600C9C9C90000000000000000000000000064646400FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC0068686800585858005555550055555500555555005555
      5500555555005656560069696900000000000000000000000000000000000000
      0000B07F6A00000000000000000098573B00894020008A4121008A4121008A41
      21008A40210089402000904C2E009A5B4000DFCCC40000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000772B0000772B0000772B0000772B0000772B000000
      00000000000000000000000000000000000000000000000000000A8E4700007B
      2F0000722A0000731C00006611000064100000660E0000660F0000650D000065
      110000680E0004823700FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC0068686800C9C9C90000000000000000000000000065656500FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC0065656500B6B6B600FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC005F5F5F00000000000000000000000000000000000000
      0000B486720096563A0098583C008C442500904B2D00904B2D008E4829008B43
      23008B4323008D4627008F492A008B43240093503300BC928000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000A9A9A9000000000000000000BFBFBF00ABABAB0097979700979797009A9A
      9A00A9A9A900E4E4E40000772B0000772B0000772B0000772B0000772B000077
      2B0000000000000000000000000000000000000000000000000000742000FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00007D3400FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC0065656500C9C9C90000000000000000000000000067676700FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC0067676700D5D5D500FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC0067676700000000000000000000000000000000000000
      0000D0B3A600873D1D008E482900904C2E008F4A2B008B42230096553900AB77
      6100AB776100A0654B008C4526008B4324008F492B00863A1900A1664C000000
      0000000000000000000000000000000000000000000000000000000000000000
      000091919100AEAEAE00A1A1A100686868006464640066666600626262006161
      610064646400E4E4E40000772B0000772B0000772B0000772B0000772B000077
      2B0000772B0000772B0000000000000000000000000000000000006E1F00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00D9D7DA00D3DED800BFD3
      C700FCFCFC0000782C00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC0067676700CBCBCB0000000000000000000000000069696900FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC0068686800D5D5D500FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC0068686800000000000000000000000000000000000000
      0000DECAC1008A4222008F4A2B008D47280096553900D3B8AC00000000000000
      00000000000000000000E1CEC600B3836E00873D1D00904A2C008B422300C29C
      8B00000000000000000000000000000000000000000000000000000000000000
      0000919191006E6E6E00717171006F6F6F0068686800707070007A7A7A007B7B
      7B0073737300E4E4E40000772B0000772B0000772B0000772B0000772B000077
      2B0000772B0000772B0000772B0000000000000000000000000000762500FCFC
      FC008AA99000176F26005AA07B00498E5F00659D7C000079340000701A003584
      5800FCFCFC00007B3000FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC0068686800C9C9C9000000000000000000000000006A6A6A00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC006A6A6A00D5D5D500FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC006A6A6A00000000000000000000000000000000000000
      000000000000995A3E008D4728008E472900863A1900B5877300000000000000
      000000000000000000000000000000000000AF7E68008E492A008E492A008B43
      2400DECAC1000000000000000000000000000000000000000000000000000000
      0000AEAEAE005D5D5D00707070007171710096969600B2B2B200CBCBCB00CBCB
      CB00B9B9B900E4E4E40000772B0000772B0000772B0000772B0000772B000077
      2B0000772B0000772B000000000000000000000000000000000000742100FCFC
      FC00C4D3CC0000690300005E000000802800008E460000801C00005B0000B5C1
      BC00FCFCFC0000772A00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC006A6A6A00C9C9C9000000000000000000000000006A6A6A00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC006A6A6A00D5D5D500FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC006A6A6A00000000000000000000000000000000000000
      00000000000000000000DECAC100DECAC100C9A79800C39E8E00DECAC1000000
      00000000000000000000000000000000000000000000AB755E008B4324008A42
      2200AA745D000000000000000000000000000000000000000000000000000000
      0000CBCBCB0065656500636363005C5C5C00A1A1A10000000000000000000000
      0000000000000000000000772B0000772B0000772B0000772B0000772B000077
      2B0000772B000000000000000000000000000000000000000000006D1B00FCFC
      FC00FCFCFC00DBCEDD00308C510088BC9B001C8E48003D9B71007FA18000CBCC
      C300FCFCFC0000712600FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC006A6A6A00C9C9C9000000000000000000000000006C6C6C00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC006B6B6B00D5D5D500FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC006B6B6B00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C5A09000883F1E008F4A
      2C008F492A00DFCBC20000000000000000000000000000000000000000000000
      000000000000BBBBBB00C2C2C200919191009191910000000000000000000000
      0000000000000000000000772B0000772B0000772B0000772B0000772B000000
      0000000000000000000000000000000000000000000000000000006D1C00FCFC
      FC00FCFCFC00FCFCFC0083B79C000183290000771B0012864D00FCFCFC00FCFC
      FC00FCFCFC0000712600FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC006B6B6B00CACACA000000000000000000000000006E6E6E00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC006E6E6E00D5D5D500FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC006E6E6E00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000093513400873C
      1B00853A1900AD7B650000000000000000000000000000000000000000000000
      000000000000000000000000000000000000A9A9A90000000000000000000000
      0000000000000000000000772B0000772B0000772B0000772B00E4E4E400D9D9
      D90000000000000000000000000000000000000000000000000000752500FCFC
      FC00A6C5B000006B0C000071010027834C00028A48000071050000680C00E9D9
      E000FCFCFC0000782B00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC006E6E6E00CACACA0000000000000000000000000070707000FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC006F6F6F00D5D5D500FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC006F6F6F00000000000000000000000000C6A39300A269
      4F00A46A5100C09A890000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D7BEB300BE95
      84009E634800BC94820000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000772B0000772B0000772B00E4E4E400636363009797
      970000000000000000000000000000000000000000000000000000772A00FCFC
      FC006AA07F0000681E0023804F00FCFCFC00D4CBD1002D8653000B7A3800A2BF
      AE00FCFCFC0000772B00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC006F6F6F00CBCBCB0000000000000000000000000072727200FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC0071717100D5D5D500FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC0071717100000000000000000000000000CBAA9B008539
      1800883D1D009F63490000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000B1B1B1008181
      810093939300D4D4D40000000000000000000000000000000000000000000000
      0000000000000000000000772B0000000000E4E4E400BDBDBD0093939300B3B3
      B30000000000000000000000000000000000000000000000000000712200FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FFFEFF0000792D00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC0071717100CCCCCC0000000000000000000000000073737300FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC0076767600D5D5D500FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC007272720000000000000000000000000000000000A46A
      52008C4425008A402000D5BBAF00000000000000000000000000000000000000
      00000000000000000000DECAC100000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C0C0C0006161
      610066666600BFBFBF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000742800FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC0000772C00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC0076767600CCCCCC0000000000000000000000000073737300FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00B4B4B4007B7B
      7B00707070007373730077777700D5D5D500FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC007373730000000000000000000000000000000000C7A5
      9600883F1F008E48290094523500E4D4CC000000000000000000000000000000
      00000000000000000000BA8F7C008D462700934F3200934F32008D462700D6BD
      B200000000000000000000000000000000000000000000000000FAFAFA008F8F
      8F005F5F5F008181810000000000000000000000000000000000000000000000
      00000000000091919100D3D3D300D6D6D600D1D1D100E7E7E700000000000000
      0000000000000000000000000000000000000000000000000000029B4C000697
      47000B95480000903C0000842E0000842D0000832F0000832D0000822D000086
      350000842F00018D3E00FCFCFC00FCFCFC00B4B4B4007B7B7B00707070007373
      730077777700CDCDCD0000000000000000000000000076767600FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00848484008080
      8000F5F5F500AEAEAE006B6B6B00D5D5D500FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC0075757500000000000000000000000000000000000000
      0000955438008C4425008C45260094513400E0CDC50000000000000000000000
      0000000000000000000000000000AB7660008B4223008F492B00883E1F00B789
      750000000000000000000000000000000000000000000000000000000000C4C4
      C40060606000666666008D8D8D00000000000000000000000000000000000000
      000000000000BABABA00747474006F6F6F0065656500A7A7A700000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000076767600FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC008484840080808000E3E3E300E3E3
      E3006B6B6B00CBCBCB0000000000000000000000000078787800FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC0085858500F5F5
      F500FCFCFC00AAAAAA006B6B6B00D5D5D500FCFCFC00FCFCFC00FCFCFC00D5D5
      D500D5D5D500D5D5D5007C7C7C00000000000000000000000000000000000000
      0000DEC9C100955235008B4425008D4728008E482A00B4867200D7BEB400D7BE
      B400D7BEB400D2B6AA00A7715900955236008F4A2C00904B2D008B422300A973
      5B00000000000000000000000000000000000000000000000000000000000000
      00009696960067676700616161008D8D8D00C4C4C40000000000000000000000
      0000E4E4E400C1C1C1007B7B7B006C6C6C00666666008B8B8B00EAEAEA000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000078787800FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC0085858500E3E3E300FCFCFC00AAAA
      AA006B6B6B00000000000000000000000000000000007A7A7A00D3D3D300D5D5
      D500D5D5D500D5D5D500D5D5D500D5D5D500D5D5D500D5D5D5008F8F8F00A9A9
      A900AAAAAA0064646400D5D5D500FCFCFC00FCFCFC00FCFCFC00A9A9A9007575
      7500676767006C6C6C007A7A7A00000000000000000000000000000000000000
      00000000000000000000A46B5200883D1D008E482A008B432300894020008940
      200089402000894021008C4425008F4A2C00904A2C008A4122008D462600904C
      2E00000000000000000000000000000000000000000000000000000000000000
      0000E7E7E700A1A1A1007070700060606000777777008B8B8B008D8D8D008D8D
      8D008686860073737300717171006B6B6B006767670071717100E7E7E7000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000007A7A7A00FCFCFC00FCFCFC00FCFCFC00FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC008F8F8F00E3E3E300AAAAAA006464
      6400D5D5D5000000000000000000000000000000000066666600676767006868
      6800686868006868680068686800676767006B6B6B007C7C7C00676767006060
      600070707000D5D5D500FCFCFC00FCFCFC00FCFCFC00FCFCFC00888888008F8F
      8F00F5F5F500C1C1C1006D6D6D00000000000000000000000000000000000000
      0000000000000000000000000000B07F69008E4728008A4222008E482A008F4A
      2B00904B2D00904B2D008F492B008C4425008B442400C099880000000000A974
      5D00DECAC1000000000000000000000000000000000000000000000000000000
      00000000000000000000B3B3B300747474006262620065656500666666006868
      6800696969006C6C6C006363630086868600C1C1C10091919100C1C1C1000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000666666006767670068686800686868006868
      680068686800676767006B6B6B007C7C7C00676767006060600070707000D6D6
      D600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000076767600D6D6D600FCFC
      FC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC00FCFCFC008B8B8B00F5F5
      F500FCFCFC009C9C9C0077777700000000000000000000000000000000000000
      000000000000000000000000000000000000DDC8BF00A9735C00945235008F4A
      2B00883F1E00893F1F00904C2E009E614700D1B4A80000000000000000000000
      0000DECAC1000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000CECECE009D9D9D00777777006D6D6D006666
      6600676767007070700095959500DADADA000000000000000000A7A7A7000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000E3E3E300E3E3E300E3E3E300E3E3E300E3E3
      E300E3E3E300E3E3E300E3E3E300E3E3E300E3E3E300E3E3E300E3E3E3000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000007C7C7C00C3C3C300CDCD
      CD00CDCDCD00D1D1D100D0D0D000D0D0D000D0D0D000CFCFCF008C8C8C009E9E
      9E00949494006E6E6E00D5D5D500000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000DECA
      C100D1B4A800D1B4A800DECAC100000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000EAEAEA00CACACA00BCBC
      BC00C5C5C500C9C9C90000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000076767600646464006363
      63006363630062626200626262006262620062626200626262006B6B6B006666
      660087878700D5D5D50000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000060000000300000000100010000000000400200000000000000000000
      000000000000000000000000FFFFFF00FFFFFF000000000000000000FFFFFF00
      0000000000000000FFFFFF000000000000000000FFF803000000000000000000
      FFF803000000000000000000FE0003000000000000000000F400030000000000
      00000000F00003000000000000000000F00003000000000000000000F0780300
      0000000000000000F83803000000000000000000FFF803000000000000000000
      FFFF0F000000000000000000C3FF0F000000000000000000C3FFFF0000000000
      00000000E1FBFF000000000000000000E0F83F000000000000000000F0381F00
      0000000000000000F8001F000000000000000000FC001F000000000000000000
      FE00DF000000000000000000FFC1FF000000000000000000FFFFFF0000000000
      00000000FFFFFF000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF80
      00FFFFFFFFFFFDFFFE00038000FFFFFFFFFFFCFFFE0003800001FF03FFFFFC3F
      FE0003800001F6007FFFFC1FC00003800001F0003FF6000FC00003800001F000
      1FF00003C00003800001F03C0FF00001C00003800001F83F07F00003C0000380
      0001FC1F87F07C07C00003800001FFFF83F87C1FC00003800001FFFFC3FF7C0F
      C00003800001C3FFC3FFFC0FC00003800001C3FFFFC3FD0FC00003800001E1FD
      FFC3FFFFC00003800001E0FC0FC3F83FC00003800001F07E0FE1F83FFE000380
      0001F0000FF0701FFE0007800001FC000FF0001FFE0007800001FE0027FC001F
      FE000FFF8001FF0077FE00DFFE001FFF8001FFE1FFFF83FFFFFFFFFF8003FFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000
      000000000000}
  end
  object UniQuery2: TUniQuery
    Left = 704
    Top = 144
  end
  object UniQuery3: TUniQuery
    Left = 760
    Top = 144
  end
end
