

int Far_EditorControl(ClientData cd, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  intptr_t result = 0;

  InitParam_TclToC_Int(1, EditorID);
  InitParam_TclToC_Int(2, Cmd);

  switch (Cmd)
  {
//    case ECTL_SETKEYBAR:
    case ECTL_ADDCOLOR: {
      EditorColor ec={sizeof(EditorColor)};
      AssignParam_TclToC_Int(3, ec.StringNumber);
      ec.ColorItem = 0;
      AssignParam_TclToC_Int(4, ec.StartPos);
      AssignParam_TclToC_Int(5, ec.EndPos);
      AssignParam_TclToC_Int(6, ec.Priority);
      AssignParam_TclToC_WideInt(7, ec.Flags);
      AssignParam_TclToC_WideInt(8, ec.Color.Flags);
      AssignParam_TclToC_Int(9, ec.Color.ForegroundColor);
      AssignParam_TclToC_Int(10, ec.Color.BackgroundColor);
      AssignParam_TclToC_GUID(11, ec.Owner);
      result = Info.EditorControl(EditorID, ECTL_ADDCOLOR, NULL, &ec);
      break;
    }
    case ECTL_GETCOLOR: {
      EditorColor ec={sizeof(EditorColor)};
      AssignParam_TclToC_Int(3, ec.StringNumber);
      AssignParam_TclToC_Int(4, ec.ColorItem);

      InitResultDict_CToTcl();
      if (Info.EditorControl(EditorID, ECTL_GETCOLOR, NULL, &ec)) {
        AppendResultDict_CToTcl_Int("startpos", ec.StartPos);
        AppendResultDict_CToTcl_Int("endpos", ec.EndPos);
        AppendResultDict_CToTcl_Int("priority", ec.Priority);
        AppendResultDict_CToTcl_WideInt("flags", ec.Priority);
        AppendResultDict_CToTcl_GUID("owner", ec.Owner);
        Tcl_Obj *color = Tcl_NewListObj(0, NULL);
        Tcl_ListObjAppendElement(interp, color, Tcl_NewWideIntObj(ec.Color.Flags));
        Tcl_ListObjAppendElement(interp, color, Tcl_NewWideIntObj(ec.Color.ForegroundColor));
        Tcl_ListObjAppendElement(interp, color, Tcl_NewWideIntObj(ec.Color.BackgroundColor));
        AppendResultDict_CToTcl_Object("color", color);
      };
      SetResultDict_CToTcl();

      FreeParam_TclToC_Int(EditorID);
      return TCL_OK;
      break;
    }
    case ECTL_SETPARAM: {
      EditorSetParameter esp={sizeof(EditorSetParameter)};
      InitParam_TclToC_Int(3, Param);
      esp.Type = static_cast<EDITOR_SETPARAMETER_TYPES>(Param);
      Tcl_DString ds;
      Tcl_DStringInit(&ds);

      switch (esp.Type) {
        case ESPT_AUTOINDENT:
        case ESPT_CHARCODEBASE:
        case ESPT_CODEPAGE:
        case ESPT_CURSORBEYONDEOL:
        case ESPT_EXPANDTABS:
        case ESPT_LOCKMODE:
        case ESPT_SAVEFILEPOSITION:
        case ESPT_SHOWWHITESPACE:
        case ESPT_SETBOM:
        case ESPT_TABSIZE:
          AssignParam_TclToC_Int(4, esp.iParam);
          break;
        case ESPT_SETWORDDIV:
          esp.wszParam = NULL;
          Tcl_WinUtfToTChar(Tcl_GetStringFromObj(objv[4], NULL), -1, &ds);
          if (Tcl_DStringLength(&ds) != 0) esp.wszParam = (wchar_t *)Tcl_DStringValue(&ds);
          break;
        case ESPT_GETWORDDIV: {
          esp.wszParam = NULL;
          size_t size = Info.EditorControl(EditorID, ECTL_SETPARAM, NULL, &esp);
          esp.wszParam = new wchar_t[size];
          esp.Size = size;

          Info.EditorControl(EditorID, ECTL_SETPARAM, NULL, &esp);

          SetResult_CToTcl_String(esp.wszParam);

          Tcl_DStringFree(&ds);
          FreeParam_TclToC_Int(Param);
          FreeParam_TclToC_Int(EditorID);
          delete [] esp.wszParam;
          return TCL_OK;
          break;
        };
      };

      result = Info.EditorControl(EditorID, ECTL_SETPARAM, NULL, &esp);

      Tcl_DStringFree(&ds);
      FreeParam_TclToC_Int(Param);
      break;
    }
    case ECTL_READINPUT: {
      INPUT_RECORD rec;

      Info.EditorControl(EditorID, ECTL_READINPUT, NULL, &rec);

      InitResultDict_CToTcl();
      switch (rec.EventType) {
        case FARMACRO_KEY_EVENT:
        case KEY_EVENT: {
          AppendResultDict_CToTcl_StringConst("eventtype", "key");
          AppendResultDict_CToTcl_Boolean("keydown", rec.Event.KeyEvent.bKeyDown);
          AppendResultDict_CToTcl_Int("repeatcount", rec.Event.KeyEvent.wRepeatCount);
          AppendResultDict_CToTcl_Int("keycode", rec.Event.KeyEvent.wVirtualKeyCode);
          AppendResultDict_CToTcl_Int("scancode", rec.Event.KeyEvent.wVirtualScanCode);
          AppendResultDict_CToTcl_Int("keystate", rec.Event.KeyEvent.dwControlKeyState);
          AppendResultDict_CToTcl_Unicode("uchar", (const Tcl_UniChar *)&rec.Event.KeyEvent.uChar, 1);
          AppendResultDict_CToTcl_Object("macro", (rec.EventType = FARMACRO_KEY_EVENT)?Tcl_NewBooleanObj(1):Tcl_NewBooleanObj(0));
          break;
        }
        case MOUSE_EVENT: {
          AppendResultDict_CToTcl_StringConst("eventtype", "mouse");
          AppendResultDict_CToTcl_Int("x", rec.Event.MouseEvent.dwMousePosition.X);
          AppendResultDict_CToTcl_Int("y", rec.Event.MouseEvent.dwMousePosition.Y);
          AppendResultDict_CToTcl_Int("buttonstate", rec.Event.MouseEvent.dwButtonState);
          AppendResultDict_CToTcl_Int("controlkeystate", rec.Event.MouseEvent.dwControlKeyState);
          AppendResultDict_CToTcl_Int("flags", rec.Event.MouseEvent.dwEventFlags);
          break;
        }
        case WINDOW_BUFFER_SIZE_EVENT: {
          AppendResultDict_CToTcl_StringConst("eventtype", "windowsize");
          AppendResultDict_CToTcl_Int("x", rec.Event.WindowBufferSizeEvent.dwSize.X);
          AppendResultDict_CToTcl_Int("y", rec.Event.WindowBufferSizeEvent.dwSize.Y);
          break;
        }
        case MENU_EVENT: {
          AppendResultDict_CToTcl_StringConst("eventtype", "menu");
        }
        case FOCUS_EVENT: {
          AppendResultDict_CToTcl_StringConst("eventtype", "focus");
        }
        default: {
          AppendResultDict_CToTcl_StringConst("eventtype", "unknown");
        }
      };
      AppendResultDict_CToTcl_CustomType("dump", &rec, INPUT_RECORD);
      SetResultDict_CToTcl();

      FreeParam_TclToC_Int(EditorID);
      return TCL_OK;
      break;
    }
     case ECTL_PROCESSINPUT: {
       InitParam_TclToC_Int(3, ProcessType);

       switch (ProcessType) {
         case 0: {
           InitParam_TclToC_CustomType(4, rec, INPUT_RECORD);
          result = Info.EditorControl(EditorID, ECTL_PROCESSINPUT, NULL, rec);
           FreeParam_TclToC_CustomType(rec);
         };
       }

       FreeParam_TclToC_Int(ProcessType);
       break;
     }
    case ECTL_SELECT: {
      EditorSelect es={sizeof(EditorSelect)};
      AssignParam_TclToC_Int(3, es.BlockType);
      AssignParam_TclToC_Int(4, es.BlockStartLine);
      AssignParam_TclToC_Int(5, es.BlockStartPos);
      AssignParam_TclToC_Int(6, es.BlockWidth);
      AssignParam_TclToC_Int(7, es.BlockHeight);
      result = Info.EditorControl(EditorID, ECTL_SELECT, NULL, &es);
      break;
    }
    case ECTL_DELCOLOR: {
      EditorDeleteColor edc={sizeof(EditorDeleteColor)};
      AssignParam_TclToC_GUID(3, edc.Owner);
      AssignParam_TclToC_Int(4, edc.StringNumber);
      AssignParam_TclToC_Int(5, edc.StartPos);
      result = Info.EditorControl(EditorID, ECTL_DELCOLOR, NULL, &edc);
      break;
    }
    case ECTL_DELETESESSIONBOOKMARK: {
      InitParam_TclToC_Int(3, Index);
      result = Info.EditorControl(EditorID, ECTL_DELETESESSIONBOOKMARK, NULL, (void *)Index);
      FreeParam_TclToC_Int(Index);
      break;
    }
    case ECTL_EXPANDTABS:  {
      InitParam_TclToC_Int(3, Line);
      result = Info.EditorControl(EditorID, ECTL_EXPANDTABS, NULL, &Line);
      FreeParam_TclToC_Int(Line);
      break;
    }
    case ECTL_TABTOREAL:
    case ECTL_REALTOTAB: {
      EditorConvertPos ecp={sizeof(EditorConvertPos)};
      AssignParam_TclToC_Int(3, ecp.StringNumber);
      AssignParam_TclToC_Int(4, ecp.SrcPos);
      result = Info.EditorControl(EditorID, static_cast<EDITOR_CONTROL_COMMANDS>(Cmd), NULL, &ecp);
      if (result) {
        result = ecp.DestPos;
      };
      break;
    }
    case ECTL_GETSESSIONBOOKMARKS:
    case ECTL_GETBOOKMARKS: {
      InitResult_CToTcl();
      intptr_t size = Info.EditorControl(EditorID, static_cast<EDITOR_CONTROL_COMMANDS>(Cmd), 0, NULL);
      if (size) {
        EditorBookmarks *bm = (EditorBookmarks *)ckalloc(size);
        if (bm) {
          bm->StructSize = sizeof(EditorBookmarks);
          bm->Size = size;
          if (Info.EditorControl(EditorID, static_cast<EDITOR_CONTROL_COMMANDS>(Cmd), 0, bm)) {
            for (size_t i = 0; i < bm->Count; i++) {
              Tcl_Obj *el = Tcl_NewListObj(0, NULL);
              Tcl_ListObjAppendElement(interp, el, Tcl_NewIntObj(bm->Line[i]));
              Tcl_ListObjAppendElement(interp, el, Tcl_NewIntObj(bm->Cursor[i]));
              Tcl_ListObjAppendElement(interp, el, Tcl_NewIntObj(bm->ScreenLine[i]));
              Tcl_ListObjAppendElement(interp, el, Tcl_NewIntObj(bm->LeftPos[i]));
              AppendResult_CToTcl_Object(el);
            };
          };
        };
        ckfree(bm);
      };
      FreeParam_TclToC_Int(EditorID);
      FreeParam_TclToC_Int(Cmd);
      SetResult_CToTcl();
      return TCL_OK;
      break;
    }
    case ECTL_GETFILENAME: {
      intptr_t size = Info.EditorControl(EditorID, ECTL_GETFILENAME, 0, NULL);
      if (size) {
        wchar_t *fn = new wchar_t[size];
        if (fn) {
          Info.EditorControl(EditorID, ECTL_GETFILENAME, size, fn);
          SetResult_CToTcl_String(fn);
          delete [] fn;
        };
      };
      FreeParam_TclToC_Int(EditorID);
      FreeParam_TclToC_Int(Cmd);
      return TCL_OK;
      break;
    }
    case ECTL_GETINFO: {
      EditorInfo ei={sizeof(EditorInfo)};
      Info.EditorControl(EditorID, ECTL_GETINFO, 0, &ei);

      InitResultDict_CToTcl();
      AppendResultDict_CToTcl_Int("id", ei.EditorID);
      AppendResultDict_CToTcl_Int("sizex", ei.WindowSizeX);
      AppendResultDict_CToTcl_Int("sizey", ei.WindowSizeY);
      AppendResultDict_CToTcl_Int("totallines", ei.TotalLines);
      AppendResultDict_CToTcl_Int("curline", ei.CurLine);
      AppendResultDict_CToTcl_Int("curpos", ei.CurPos);
      AppendResultDict_CToTcl_Int("curtabpos", ei.CurTabPos);
      AppendResultDict_CToTcl_Int("topscreenline", ei.TopScreenLine);
      AppendResultDict_CToTcl_Int("leftpos", ei.LeftPos);
      AppendResultDict_CToTcl_Int("overtype", ei.Overtype);
      AppendResultDict_CToTcl_Int("blocktype", ei.BlockType);
      AppendResultDict_CToTcl_Int("blockstartline", ei.BlockStartLine);
      AppendResultDict_CToTcl_Int("tabsize", ei.TabSize);
      AppendResultDict_CToTcl_Int("bookmarkcount", ei.BookmarkCount);
      AppendResultDict_CToTcl_Int("sessionbookmarkcount", ei.SessionBookmarkCount);
      AppendResultDict_CToTcl_Int("codepage", ei.CodePage);
      AppendResultDict_CToTcl_Int("curstate", ei.CurState);
      AppendResultDict_CToTcl_Int("options", ei.Options);
      SetResultDict_CToTcl();

      FreeParam_TclToC_Int(EditorID);
      FreeParam_TclToC_Int(Cmd);
      return TCL_OK;
      break;
    }
    case ECTL_GETSTRING: {
      EditorGetString egs={sizeof(EditorGetString)};
      AssignParam_TclToC_Int(3, egs.StringNumber);
      Info.EditorControl(EditorID, ECTL_GETSTRING, 0, &egs);
      InitResult_CToTcl();

      Tcl_DString ds;
      Tcl_WinTCharToUtf(egs.StringText, egs.StringLength * sizeof(wchar_t), &ds);
      AppendResult_CToTcl_Object(Tcl_NewStringObj(Tcl_DStringValue(&ds), Tcl_DStringLength(&ds)));
      Tcl_DStringFree(&ds);

      AppendResult_CToTcl_Int(egs.SelStart);
      AppendResult_CToTcl_Int(egs.SelEnd);
      SetResult_CToTcl();
      FreeParam_TclToC_Int(EditorID);
      FreeParam_TclToC_Int(Cmd);
      return TCL_OK;
      break;
    }
    case ECTL_INSERTSTRING: {
      InitParam_TclToC_Int(3, Indent);
      result = Info.EditorControl(EditorID, ECTL_INSERTSTRING, NULL, &Indent);
      FreeParam_TclToC_Int(Indent);
      break;
    }
    case ECTL_INSERTTEXT: {
      InitParam_TclToC_String(3, String);
      result = Info.EditorControl(EditorID, ECTL_INSERTTEXT, NULL, String);
      FreeParam_TclToC_String(String);
      break;
    }
    case ECTL_SAVEFILE: {
      EditorSaveFile esf={sizeof(EditorSaveFile)};
      esf.FileName = NULL;
      esf.FileEOL = NULL;
      Tcl_DString FileNameDS;
      Tcl_DString FileEOLDS;
      Tcl_WinUtfToTChar(Tcl_GetStringFromObj(objv[3], NULL), -1, &FileNameDS);
      if (Tcl_DStringLength(&FileNameDS) != 0) esf.FileName = (wchar_t *)Tcl_DStringValue(&FileNameDS);
      Tcl_WinUtfToTChar(Tcl_GetStringFromObj(objv[4], NULL), -1, &FileEOLDS);
      if (Tcl_DStringLength(&FileEOLDS) != 0) esf.FileEOL = (wchar_t *)Tcl_DStringValue(&FileEOLDS);
      AssignParam_TclToC_Int(5, esf.CodePage);

      result = Info.EditorControl(EditorID, ECTL_SAVEFILE, NULL, &esf);

      Tcl_DStringFree(&FileNameDS);
      Tcl_DStringFree(&FileEOLDS);
      break;
    }
    case ECTL_SETPOSITION: {
      EditorSetPosition esp={sizeof(EditorSetPosition)};
      AssignParam_TclToC_Int(3, esp.CurLine);
      AssignParam_TclToC_Int(4, esp.CurPos);
      AssignParam_TclToC_Int(5, esp.CurTabPos);
      AssignParam_TclToC_Int(6, esp.TopScreenLine);
      AssignParam_TclToC_Int(7, esp.LeftPos);
      AssignParam_TclToC_Int(8, esp.Overtype);

      result = Info.EditorControl(EditorID, ECTL_SETPOSITION, NULL, &esp);
      break;
    }
    case ECTL_SETSTRING: {
      EditorSetString ess={sizeof(EditorSetString)};
      ess.StringEOL = NULL;
      Tcl_DString ds;
      Tcl_DString eol;
      AssignParam_TclToC_Int(3, ess.StringNumber);
      Tcl_WinUtfToTChar(Tcl_GetStringFromObj(objv[4], NULL), -1, &ds);
      ess.StringText = (wchar_t *)Tcl_DStringValue(&ds);
      ess.StringLength = Tcl_DStringLength(&ds)/sizeof(wchar_t);
      Tcl_WinUtfToTChar(Tcl_GetStringFromObj(objv[5], NULL), -1, &eol);
      if (Tcl_DStringLength(&eol)) ess.StringEOL = (wchar_t *)Tcl_DStringValue(&eol);

      result = Info.EditorControl(EditorID, ECTL_SETSTRING, NULL, &ess);

      Tcl_DStringFree(&ds);
      Tcl_DStringFree(&eol);
      break;
    }
    case ECTL_SETTITLE: {
      InitParam_TclToC_String(3, String);
      result = Info.EditorControl(EditorID, ECTL_SETTITLE, NULL, String);
      FreeParam_TclToC_String(String);
      break;
    }
    case ECTL_UNDOREDO: {
      EditorUndoRedo eur={sizeof(EditorUndoRedo)};
      InitParam_TclToC_Int(3, Command);
      eur.Command = static_cast<EDITOR_UNDOREDO_COMMANDS>(Command);
      result = Info.EditorControl(EditorID, ECTL_UNDOREDO, NULL, &eur);
      FreeParam_TclToC_Int(Command);
      break;
    }
    case ECTL_REDRAW:
    case ECTL_ADDSESSIONBOOKMARK:
    case ECTL_CLEARSESSIONBOOKMARKS:
    case ECTL_DELETEBLOCK:
    case ECTL_DELETECHAR:
    case ECTL_DELETESTRING:
    case ECTL_NEXTSESSIONBOOKMARK:
    case ECTL_PREVSESSIONBOOKMARK:
    case ECTL_QUIT:
      result = Info.EditorControl(EditorID, static_cast<EDITOR_CONTROL_COMMANDS>(Cmd), NULL, NULL);
      break;
  };

  FreeParam_TclToC_Int(EditorID);
  FreeParam_TclToC_Int(Cmd);
  SetResult_CToTcl_Int(result);
  return TCL_OK;
};

