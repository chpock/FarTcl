
APICall(EditorControl) {

  InitReturnCode();

  InitParam(Int, EditorID, 1);
  InitParam(Int, Cmd,      2);

  switch (Cmd) {
      case ECTL_ADDCOLOR: {
          NewStruct(EditorColor, ec);
          AssignParam(Int,   ec.StringNumber, 3);
          AssignParam(Int,   ec.StartPos,     4);
          AssignParam(Int,   ec.EndPos,       5);
          AssignParam(Int,   ec.Priority,     6);
          AssignParam(UWide, ec.Flags,        7);
          AssignParam(UWide, ec.Color.Flags,   8);
          AssignParam(Int,   ec.Color.ForegroundColor, 9);
          AssignParam(Int,   ec.Color.BackgroundColor, 10);
          AssignParam(GUID,  ec.Owner,        11);
          ec.ColorItem = 0;

          intptr_t result = Info.EditorControl(EditorID, ECTL_ADDCOLOR, 0, &ec);

          FreeParam(GUID,  ec.Owner);
          FreeParam(Int,   ec.Color.BackgroundColor);
          FreeParam(Int,   ec.Color.ForegroundColor);
          FreeParam(UWide, ec.Color.Flags);
          FreeParam(UWide, ec.Flags);
          FreeParam(Int,   ec.Priority);
          FreeParam(Int,   ec.EndPos);
          FreeParam(Int,   ec.StartPos);
          FreeParam(Int,   ec.StringNumber);

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          ReturnResult(Int, result, OK);
          break;
      }
      case ECTL_GETCOLOR: {
          NewStruct(EditorColor, ec);
          AssignParam(Int, ec.StringNumber, 3);
          AssignParam(Int, ec.ColorItem,    4);

          InitResult(Dict);

          if (Info.EditorControl(EditorID, ECTL_GETCOLOR, 0, &ec)) {

              NewList(Color);
              LAppend(Color, UWide, ec.Color.Flags);
              LAppend(Color, Int,   ec.Color.ForegroundColor);
              LAppend(Color, Int,   ec.Color.BackgroundColor);

              DSet(Result, "startpos", Int,   ec.StartPos);
              DSet(Result, "endpos",   Int,   ec.EndPos);
              DSet(Result, "priority", Int,   ec.Priority);
              DSet(Result, "flags",    UWide, ec.Flags);
              DSet(Result, "owner",    GUID,  ec.Owner);
              DSet(Result, "color",    Obj,   Color);

          } else {
              SetReturnCode(ERROR);
          }

          FreeParam(Int, ec.StringNumber);
          FreeParam(Int, ec.ColorItem);

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          Return();
          break;
      }
      case ECTL_SETPARAM: {
          NewStruct(EditorSetParameter, esp);
          AssignParam(UWide, esp.Type, 3);

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
              case ESPT_TABSIZE: {
                  AssignParam(Int, esp.iParam, 4);
                  break;
              }
              case ESPT_SETWORDDIV: {
                  AssignParam(Str, esp.wszParam, 4);
                  break;
              }
              case ESPT_GETWORDDIV: {
                  size_t size = Info.EditorControl(EditorID, ECTL_SETPARAM, 0, &esp);
                  esp.wszParam = (wchar_t *)ckalloc(size);
                  esp.Size = size;

                  Info.EditorControl(EditorID, ECTL_SETPARAM, 0, &esp);

                  InitResult(Str, esp.wszParam);

                  ckfree(esp.wszParam);

                  FreeParam(UWide, ec.Type);

                  FreeParam(Int, EditorID);
                  FreeParam(Int, Cmd);

                  Return(OK);
                  break;
              }
          }

          intptr_t result = Info.EditorControl(EditorID, ECTL_SETPARAM, 0, &esp);

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
              case ESPT_TABSIZE: {
                  InitResult(Int, result);
                  FreeParam(Int, esp.iParam);
                  break;
              }
              case ESPT_SETWORDDIV: {
                  InitResult(Str, esp.wszParam);
                  FreeParam(Str, esp.wszParam);
                  break;
              }
          }

          FreeParam(UWide, ec.Type);

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          Return(OK);
          break;
      }
      case ECTL_READINPUT: {
          INPUT_RECORD rec;

          Info.EditorControl(EditorID, ECTL_READINPUT, 0, &rec);

          InitResult(Dict);

          switch (rec.EventType) {
              case FARMACRO_KEY_EVENT:
              case KEY_EVENT: {
                  DSet(Result, "eventtype",   Str,  L"key");
                  DSet(Result, "keydown",     Bool, rec.Event.KeyEvent.bKeyDown);
                  DSet(Result, "repeatcount", Int,  rec.Event.KeyEvent.wRepeatCount);
                  DSet(Result, "keycode",     Int,  rec.Event.KeyEvent.wVirtualKeyCode);
                  DSet(Result, "scancode",    Int,  rec.Event.KeyEvent.wVirtualScanCode);
                  DSet(Result, "keystate",    Int,  rec.Event.KeyEvent.dwControlKeyState);
                  DSet(Result, "uchar",       Obj,  Tcl_NewUnicodeObj((const Tcl_UniChar *)&rec.Event.KeyEvent.uChar, 1));
                  DSet(Result, "macro",       Bool, (rec.EventType = FARMACRO_KEY_EVENT));
                  break;
              }
              case MOUSE_EVENT: {
                  DSet(Result, "eventtype",       Str, L"mouse");
                  DSet(Result, "x",               Int, rec.Event.MouseEvent.dwMousePosition.X);
                  DSet(Result, "y",               Int, rec.Event.MouseEvent.dwMousePosition.Y);
                  DSet(Result, "buttonstate",     Int, rec.Event.MouseEvent.dwButtonState);
                  DSet(Result, "controlkeystate", Int, rec.Event.MouseEvent.dwControlKeyState);
                  DSet(Result, "flags",           Int, rec.Event.MouseEvent.dwEventFlags);
                  break;
              }
              case WINDOW_BUFFER_SIZE_EVENT: {
                  DSet(Result, "eventtype", Str, L"windowsize");
                  DSet(Result, "x",         Int, rec.Event.WindowBufferSizeEvent.dwSize.X);
                  DSet(Result, "y",         Int, rec.Event.WindowBufferSizeEvent.dwSize.Y);
                  break;
              }
              case MENU_EVENT: {
                  DSet(Result, "eventtype", Str, L"menu");
                  break;
              }
              case FOCUS_EVENT: {
                  DSet(Result, "eventtype", Str, L"focus");
                  break;
              }
              default: {
                  DSet(Result, "eventtype", Str, L"unknown");
                  break;
              }
          }

          DSet(Result, "dump", Obj, Tcl_NewByteArrayObj((const unsigned char *)&rec, sizeof(INPUT_RECORD)));

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          Return(OK);
          break;
      }
      case ECTL_PROCESSINPUT: {
          InitParam(Int, ProcessType, 3);

          switch (ProcessType) {
              case 0: {
                  INPUT_RECORD *rec;
                  rec = (INPUT_RECORD *)Tcl_GetByteArrayFromObj(objv[4], nullptr);

                  intptr_t result = Info.EditorControl(EditorID, ECTL_PROCESSINPUT, 0, rec);

                  SetResult(Int, result);

                  break;
              }
              default: {
                  SetResultError(L"Unknown process type");
                  break;
              }
          }

          FreeParam(Int, ProcessType);

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          Return();
          break;
      }
      case ECTL_SELECT: {
          NewStruct(EditorSelect, es);
          AssignParam(Int, es.BlockType, 3);
          AssignParam(Int, es.BlockStartLine, 4);
          AssignParam(Int, es.BlockStartPos, 5);
          AssignParam(Int, es.BlockWidth, 6);
          AssignParam(Int, es.BlockHeight, 7);

          intptr_t result = Info.EditorControl(EditorID, ECTL_SELECT, 0, &es);

          FreeParam(Int, es.BlockHeight);
          FreeParam(Int, es.BlockWidth);
          FreeParam(Int, es.BlockStartPos);
          FreeParam(Int, es.BlockStartLine);
          FreeParam(Int, es.BlockType);

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          ReturnResult(Int, result, OK);
          break;
      }
      case ECTL_DELCOLOR: {
          NewStruct(EditorDeleteColor, edc);
          AssignParam(GUID, edc.Owner, 3);
          AssignParam(Int,  edc.StringNumber, 4);
          AssignParam(Int,  edc.StartPos, 5);

          intptr_t result = Info.EditorControl(EditorID, ECTL_SELECT, 0, &edc);

          FreeParam(Int,  edc.StartPos);
          FreeParam(Int,  edc.StringNumber);
          FreeParam(GUID, edc.Owner);

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          ReturnResult(Int, result, OK);
          break;
      }
      case ECTL_DELETESESSIONBOOKMARK: {
          InitParam(Int, Index, 3)

          intptr_t result = Info.EditorControl(EditorID, ECTL_DELETESESSIONBOOKMARK, 0, (void *)Index);

          FreeParam(Int, Index);

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          ReturnResult(Int, result, OK);
          break;
      }
      case ECTL_EXPANDTABS: {
          InitParam(Int, Line, 3);

          intptr_t result = Info.EditorControl(EditorID, ECTL_EXPANDTABS, 0, &Line);

          FreeParam(Int, Line);

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          ReturnResult(Int, result, OK);
          break;
      }
      case ECTL_TABTOREAL:
      case ECTL_REALTOTAB: {
          NewStruct(EditorConvertPos, ecp);
          AssignParam(Int, ecp.StringNumber, 3);
          AssignParam(Int, ecp.SrcPos,       4);

          intptr_t result = Info.EditorControl(EditorID, static_cast<EDITOR_CONTROL_COMMANDS>(Cmd), 0, &ecp);

          if (result) {
              result = ecp.DestPos;
          }

          FreeParam(Int, ecp.SrcPos);
          FreeParam(Int, ecp.StringNumber);

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          ReturnResult(Int, result, OK);
          break;
      }
      case ECTL_GETSESSIONBOOKMARKS:
      case ECTL_GETBOOKMARKS: {
          intptr_t size = Info.EditorControl(EditorID, static_cast<EDITOR_CONTROL_COMMANDS>(Cmd), 0, nullptr);

          InitResult(List);

          if (size) {
              EditorBookmarks *bm = (EditorBookmarks *)ckalloc(size);
              if (bm) {
                  bm->StructSize = sizeof(EditorBookmarks);
                  bm->Size = size;
                  if (Info.EditorControl(EditorID, static_cast<EDITOR_CONTROL_COMMANDS>(Cmd), 0, bm)) {
                      for (size_t i = 0; i < bm->Count; i++) {
                          NewDict(Bookmark);
                          DSet(Bookmark, "line", Int, bm->Line[i]);
                          DSet(Bookmark, "cursor", Int, bm->Cursor[i]);
                          DSet(Bookmark, "screenline", Int, bm->ScreenLine[i]);
                          DSet(Bookmark, "leftpos", Int, bm->LeftPos[i]);
                          LAppend(Result, Obj, Bookmark);
                      }
                  }
              }
              ckfree(bm);
          }

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          Return(OK);
          break;
      }
      case ECTL_GETFILENAME: {
          intptr_t size = Info.EditorControl(EditorID, ECTL_GETFILENAME, 0, nullptr);

          if (size) {
              wchar_t *fn = (wchar_t *)ckalloc(size);
              if (fn) {
                  Info.EditorControl(EditorID, ECTL_GETFILENAME, size, fn);

                  SetResult(Str, fn);
              }
              ckfree(fn);
          }

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          Return(OK);
          break;
      }
      case ECTL_GETINFO: {
          NewStruct(EditorInfo, ei);
          Info.EditorControl(EditorID, ECTL_GETINFO, 0, &ei);

          InitResult(Dict);
          DSet(Result, "id",                   Int, ei.EditorID);
          DSet(Result, "sizex",                Int, ei.WindowSizeX);
          DSet(Result, "sizey",                Int, ei.WindowSizeY);
          DSet(Result, "totallines",           Int, ei.TotalLines);
          DSet(Result, "curline",              Int, ei.CurLine);
          DSet(Result, "curpos",               Int, ei.CurPos);
          DSet(Result, "curtabbos",            Int, ei.CurTabPos);
          DSet(Result, "topscreenline",        Int, ei.TopScreenLine);
          DSet(Result, "leftpos",              Int, ei.LeftPos);
          DSet(Result, "overtype",             Int, ei.Overtype);
          DSet(Result, "blocktype",            Int, ei.BlockType);
          DSet(Result, "blockstartline",       Int, ei.BlockStartLine);
          DSet(Result, "tabsize",              Int, ei.TabSize);
          DSet(Result, "bookmarkcount",        Int, ei.BookmarkCount);
          DSet(Result, "sessionbookmarkcount", Int, ei.SessionBookmarkCount);
          DSet(Result, "codepage",             Int, ei.CodePage);
          DSet(Result, "curstate",             Int, ei.CurState);
          DSet(Result, "options",              Int, ei.Options);

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          Return(OK);
          break;
      }
      case ECTL_GETSTRING: {
          NewStruct(EditorGetString, egs);
          AssignParam(Int, egs.StringNumber, 3);
          Info.EditorControl(EditorID, ECTL_GETSTRING, 0, &egs);

          InitResult(Dict);
          DSet(Result, "string",   Str, egs.StringText);
          DSet(Result, "selstart", Int, egs.SelStart);
          DSet(Result, "selend",   Int, egs.SelEnd);

          FreeParam(Int, egs.StringNumber);

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          Return(OK);
          break;
      }
      case ECTL_INSERTSTRING: {
          InitParam(Int, Indent, 3);

          intptr_t result = Info.EditorControl(EditorID, ECTL_INSERTSTRING, 0, &Indent);

          FreeParam(Int, Indent);

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          ReturnResult(Int, result, OK);
          break;
      }
      case ECTL_INSERTTEXT: {
          InitParam(Str, String, 3);

          intptr_t result = Info.EditorControl(EditorID, ECTL_INSERTTEXT, 0, String);

          FreeParam(Str, String)

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          ReturnResult(Int, result, OK);
          break;
      }
      case ECTL_SAVEFILE: {
          NewStruct(EditorSaveFile, esf);
          AssignParam(StrConst, esf.FileName, 3);
          AssignParam(StrConst, esf.FileEOL,  4);
          AssignParam(Int,      esf.CodePage, 5);

          intptr_t result = Info.EditorControl(EditorID, ECTL_SAVEFILE, 0, &esf);

          FreeParam(Int,      esf.CodePage);
          FreeParam(StrConst, esf.FileEOL);
          FreeParam(StrConst, esf.FileName);

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          ReturnResult(Int, result, OK);
          break;
      }
      case ECTL_SETPOSITION: {
          NewStruct(EditorSetPosition, esp);
          AssignParam(Int, esp.CurLine,       3);
          AssignParam(Int, esp.CurPos,        4);
          AssignParam(Int, esp.CurTabPos,     5);
          AssignParam(Int, esp.TopScreenLine, 6);
          AssignParam(Int, esp.LeftPos,       7);
          AssignParam(Int, esp.Overtype,      8);

          intptr_t result = Info.EditorControl(EditorID, ECTL_SETPOSITION, 0, &esp);

          FreeParam(Int, esp.Overtype);
          FreeParam(Int, esp.LeftPos);
          FreeParam(Int, esp.TopScreenLine);
          FreeParam(Int, esp.CurTabPos);
          FreeParam(Int, esp.Cur.Pos);
          FreeParam(Int, esp.CurLine);

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          ReturnResult(Int, result, OK);
          break;
      }
      case ECTL_SETSTRING: {
          NewStruct(EditorSetString, ess);
          AssignParam(Int,      ess.StringNumber, 3);
          AssignParam(StrConst, ess.StringText,   4);
          AssignParam(StrLen,   ess.StringLength, 4);
          AssignParam(StrConst, ess.StringEOL,    5);

          intptr_t result = Info.EditorControl(EditorID, ECTL_SETSTRING, 0, &ess);

          FreeParam(StrConst, ess.StringEOL);
          FreeParam(StrLen,   ess.StringLength);
          FreeParam(StrConst, ess.StringText);
          FreeParam(Int,      ess.StringNumber);

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          ReturnResult(Int, result, OK);
          break;
      }
      case ECTL_SETTITLE: {
          InitParam(Str, String, 3);

          intptr_t result = Info.EditorControl(EditorID, ECTL_SETTITLE, 0, String);

          FreeParam(Str, String);

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          ReturnResult(Int, result, OK);
          break;
      }
      case ECTL_UNDOREDO: {
          NewStruct(EditorUndoRedo, eur);
          AssignParam(UWide, eur.Command, 3);

          intptr_t result = Info.EditorControl(EditorID, ECTL_UNDOREDO, 0, &eur);

          FreeParam(UWide, eur.Command);

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          ReturnResult(Int, result, OK);
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
      case ECTL_QUIT: {

          intptr_t result = Info.EditorControl(EditorID, static_cast<EDITOR_CONTROL_COMMANDS>(Cmd), 0, nullptr);

          FreeParam(Int, EditorID);
          FreeParam(Int, Cmd);

          ReturnResult(Int, result, OK);
          break;
      }
  }

  ReturnResult(Str, L"EditorControl: Unknown API call", ERROR);

}
