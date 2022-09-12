object FormTok: TFormTok
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'FormTok'
  ClientHeight = 535
  ClientWidth = 927
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 17
    Width = 927
    Height = 3
    Cursor = crVSplit
    Align = alTop
    ExplicitTop = 49
    ExplicitWidth = 486
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 927
    Height = 17
    Align = alTop
    TabOrder = 0
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 516
    Width = 927
    Height = 19
    Panels = <>
  end
  object Panel2: TPanel
    Left = 0
    Top = 20
    Width = 927
    Height = 496
    Align = alClient
    Caption = 'Panel2'
    TabOrder = 2
    DesignSize = (
      927
      496)
    object Splitter2: TSplitter
      Left = 645
      Top = 1
      Height = 494
      Align = alRight
      ExplicitLeft = 640
      ExplicitTop = 304
      ExplicitHeight = 100
    end
    object Label1: TLabel
      Left = 8
      Top = 43
      Width = 151
      Height = 19
      Caption = #1058#1086#1082' '#1082#1072#1078#1076#1086#1081' '#1092#1072#1079#1099', '#1040
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 8
      Top = 6
      Width = 26
      Height = 23
      Caption = #1055#1055
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object SpeedButton1: TSpeedButton
      Left = 616
      Top = 6
      Width = 28
      Height = 28
      Hint = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100' '#1074' '#1073#1091#1092#1077#1088' '#1086#1073#1084#1077#1085#1072
      Anchors = [akTop, akRight]
      Flat = True
      Glyph.Data = {
        F6060000424DF606000000000000360000002800000018000000180000000100
        180000000000C0060000EB0A0000EB0A00000000000000000001FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFC5B198865B208656178656168656168656168656168656168656168656
        16865616865617865B20C4B399FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFF885A1EF1D096F7E1B0F6E0AEF6E0AEF6E0AEF6E0AE
        F6E0AEF6E0AEF6E0AEF6E0AEF7E1B0F1D096885A1EFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF835313F8E3B2FAEEC3F9ECC1F9
        ECC1F9ECC1F9ECC1F9ECC1F9ECC1F9ECC1F9ECC1FAEEC3F8E3B2835313FFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7D4F10F2D3
        A0F6DCAEF5DBACF5DBACF5DBACF5DBACF5DBACF5DBACF5DBACF5DBACF6DCAEF2
        D3A07D4F10FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFF774B0FF1C78FF6CE9AF4CD99F4CD99F4CD99F4CD99F4CD99F4CD99F4CD
        99F4CD99F6CE9AF1C78F774B0FC4A06E865616C4A06F865616FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFF72470FE9BB7FEDC189ECC088ECC188ECC188ECC188
        ECC188ECC188ECC188ECC188EDC289E9BC7F72470FDFBF94DFBF94DFBF94BF9B
        6AFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6D430CE2B373E6B97DE5B87CE5
        B77BE5B77AE4B67AE4B77AE5B77BE5B77BE5B87DE6BA7EE2B4756D430BE1C39A
        E1C39AE1C39A7D4F12FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF683F0ADCAC
        6BE2B880E2B77EE2B77EE2B77EE2B77EE2B77EE2B77EE2B77EE2B77EE3B880DD
        AD6E683F0AE4C8A2E4C8A2E4C8A2BF9F73FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFF633B08D7A867E0B983DFB781DFB781DFB781DFB781DFB781DFB781DFB7
        81DFB781E0B983D8A96A633B08E8CEAAE8CEAAE8CEAA74490EFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFF5E3705D6A76AE3BF90E1BD8DE1BD8DE1BD8DE1BD8D
        E1BD8DE1BD8DE1BD8DE1BD8DE3BF90D6A96C5E3705EBD3B2EBD3B2EBD3B2BEA0
        76FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF5B3A0DB68039C49251C2914FC2
        914FC2914FC2914FC2914FC2914FC2914FC2914FC49251B680395C380BEFD8BA
        EFD8BAEFD8BA6C430BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFB1A18E5736
        0B54300253300153300153300153300153300153300153300153300154300256
        350AA98F6EF2DEC3F2DEC3F2DEC3BCA17CFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBCA27EF6E3CBF6E3CBF6E3CBF6E3CBF6E3
        CBF6E3CBF6E3CBF6E3CBF6E3CBF6E3CBF6E3CBF6E3CB633C07FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF5F3906F9E9D3F9E9D3
        F9E9D3F9E9D3F9E9D3F9E9D3F9E9D3F9E9D3F9E9D3F9E9D3F9E9D3F9E9D3B99F
        7CFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFB9
        A07EFDEEDBFDEEDBFDEEDBFDEEDBFDEEDBFDEEDBFDEEDBFDEEDBFDEEDBFDEEDB
        FDEEDBFDEEDB5B3604FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFF573302B79E80573302B79F7F573302B29978573302B9A38557
        3302B29977573302B9A284573302AD9574FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
      ParentShowHint = False
      ShowHint = True
      OnClick = SpeedButton1Click
    end
    object SpeedButton2: TSpeedButton
      Left = 614
      Top = 34
      Width = 28
      Height = 28
      Hint = #1050#1086#1087#1080#1088#1086#1074#1072#1085#1080#1077' '#1075#1088#1072#1092#1080#1082#1072' '#1074' '#1073#1091#1092#1077#1088' '#1086#1073#1084#1077#1085#1072
      Anchors = [akTop, akRight]
      Flat = True
      Glyph.Data = {
        F6060000424DF606000000000000360000002800000018000000180000000100
        180000000000C0060000EB0A0000EB0A00000000000000000001FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA4A4CD2B2B
        942423962322962322962322962322962322962322962322962322962423962A
        2A969A9AC9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFF3232954D4FE8656AF5646AF3646AF3656AF5656AF4656AF4656AF4656A
        F4656AF4666BF65153EA282893FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFF292893595CF08897FF8D9CFF8C9CFF8C9CFF8C9CFF
        8C9CFF8C9CFF8C9CFF8D9CFF8998FF5C5FF21F1E91FFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2727905456EB838EFF8390FF82
        8FFF828FFF828FFF828FFF828FFF828FFF8390FF828FFF575CED1A1A8DFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF23238D5154
        E87986FF7884FF7884FF7884FF7884FF7884FF7884FF7884FF7884FF7985FF54
        58EA17178A2322962423962A2A969A9AC9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFF20208B4F53E5737CFD7279FB7179FB7179FB7179FB7179FB7179FB7179
        FB7279FB737CFD5053E61515875155D9666BF65153EA282893FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFF1F1E864B4EE16970F7676EF5676EF5676EF5676EF5
        676EF5676EF5676EF5676EF56970F74C4FE0131386717DE28998FF5C5FF21F1E
        91FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF121281484BDD5F65F15D63EF5D
        63EF5D63EF5D63EF5D63EF5D63EF5D63EF5D63EF5F65F1484BDD1314846973E2
        828FFF575CED1A1A8DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0B0B7E4C4E
        DA696DED6568EB6265EB5F63EA5E62EA5E62EA6064EA6266EB6569EB696DED4A
        4CD9101081606AE27985FF5458EA17178AFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFF07077A5253D87274EB7072E97072E97072E97072E97072E97072E97072
        E97072E97274EB4C4ED60C0C7E5B61DF737CFD5053E6151587FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFF0303775555D47F7FE97C7CE77C7CE77C7CE77C7CE7
        7C7CE77C7CE77C7CE77C7CE77F7FE94F50D1090A7C5258DA6970F74C4FE01616
        86FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000745D5DD49090EA8D8DE88D
        8DE88D8DE88D8DE88D8DE88D8DE88D8DE88D8DE89191EA5555D10505794A4FD5
        5F65F1484BDD1B1B84FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0808743A3A
        C36262D25F5FD05F5FD05F5FD05F5FD05F5FD05F5FD05F5FD05F5FD06262D334
        34BF0809775154D2696DED4A4CD9181781FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFF8B8BB808087200007000006F00006F00006F00006F00006F00006F0000
        6F00006F0000700404714244B37072E97274EB4C4ED614147EFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0303775555D47F7FE9
        7C7CE77C7CE77C7CE77C7CE77C7CE77C7CE77C7CE77C7CE77F7FE94F50D11212
        7DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00
        00745D5DD49090EA8D8DE88D8DE88D8DE88D8DE88D8DE88D8DE88D8DE88D8DE8
        9191EA5555D10E0E7AFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFF0808743A3AC36262D25F5FD05F5FD05F5FD05F5FD05F5FD05F
        5FD05F5FD05F5FD06262D33434BF161679FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8B8BB808087200007000006F00006F0000
        6F00006F00006F00006F00006F00006F0000700A0A729797C0FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
      ParentShowHint = False
      ShowHint = True
      OnClick = SpeedButton2Click
    end
    object Chart1: TChart
      Left = 1
      Top = 72
      Width = 644
      Height = 423
      Title.Font.Color = clBlack
      Title.Font.Height = -19
      Title.Font.Style = [fsBold]
      Title.Text.Strings = (
        '')
      Title.Visible = False
      LeftAxis.MaximumOffset = 10
      View3D = False
      Align = alCustom
      BevelOuter = bvNone
      TabOrder = 0
      Anchors = [akLeft, akTop, akRight, akBottom]
      ColorPaletteIndex = 13
      object Series4: TAreaSeries
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Callout.Length = 20
        Marks.Visible = False
        SeriesColor = 8454143
        ShowInLegend = False
        AreaChartBrush.Color = clWhite
        AreaLinesPen.Visible = False
        DrawArea = True
        InvertedStairs = True
        LinePen.Visible = False
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        Stairs = True
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
      object Series5: TAreaSeries
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        ShowInLegend = False
        AreaLinesPen.Visible = False
        DrawArea = True
        LinePen.Visible = False
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        Stairs = True
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
      object Series1: TLineSeries
        LegendTitle = #1060#1072#1079#1072' A'
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        SeriesColor = clBlue
        LinePen.Width = 2
        Pointer.Brush.Gradient.EndColor = 10708548
        Pointer.Gradient.EndColor = 10708548
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
      object Series2: TLineSeries
        LegendTitle = #1060#1072#1079#1072' B'
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        SeriesColor = clGreen
        LinePen.Width = 2
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
      object Series3: TLineSeries
        LegendTitle = #1060#1072#1079#1072' C'
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        SeriesColor = 64
        LinePen.Width = 2
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
    end
    object AdvStringGrid1: TAdvStringGrid
      Left = 648
      Top = 1
      Width = 278
      Height = 494
      Cursor = crDefault
      Align = alRight
      DrawingStyle = gdsClassic
      FixedCols = 0
      RowCount = 1
      FixedRows = 0
      ScrollBars = ssBoth
      TabOrder = 1
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
      FixedRowHeight = 22
      FixedFont.Charset = DEFAULT_CHARSET
      FixedFont.Color = clWindowText
      FixedFont.Height = -11
      FixedFont.Name = 'Tahoma'
      FixedFont.Style = [fsBold]
      FloatFormat = '%.2f'
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
      ShowDesignHelper = False
      Version = '6.0.4.4'
    end
  end
end
