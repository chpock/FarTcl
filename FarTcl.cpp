#include <plugin.hpp>
#include <tcl.h>
#include "FarTclMessages.hpp"
#include "version.hpp"
#include <initguid.h>
#include "guid.hpp"
#include <errno.h>

static Tcl_Interp *interp = NULL;
static intptr_t ActiveEditor = -1;
static struct PluginStartupInfo Info;
static struct FarStandardFunctions FSF;
char *MainGuidStr = NULL;

intptr_t MsgBox(const wchar_t *Mesg);
intptr_t MsgBox(const int Mesg);

#include "farAPI_Tcl.cpp"
#include "farAPI_Basic.cpp"
#include "farAPI_Editor.cpp"
#include "farAPI_PluginManager.cpp"

const wchar_t *GetMsg(int MsgId)
{
  return Info.GetMsg(&MainGuid, MsgId);
};

intptr_t MsgBox(const wchar_t *Mesg)
{
  wchar_t buf[500];
  FSF.snprintf(buf, 500, L"%s\n%s\x01", L"MessageBox - string", Mesg);
  return Info.Message(&MainGuid, nullptr,  FMSG_MB_OK|FMSG_LEFTALIGN|FMSG_ALLINONE, NULL, (const wchar_t **)buf, 0,  0);                           /* ButtonsNumber */
};
intptr_t MsgBox(const int Mesg)
{
  wchar_t value[50];
  FSF.itoa(Mesg, value, 10);
  const wchar_t *MsgItems[]= { L"MessageBox - integer",  value,  L"\x01" };
  return Info.Message(&MainGuid, nullptr,  FMSG_MB_OK|FMSG_LEFTALIGN, NULL, MsgItems, ARRAYSIZE(MsgItems),  0);                           /* ButtonsNumber */
};

intptr_t ErrorMessage(const wchar_t *Message)
{
  wchar_t buf[500];
  FSF.snprintf(buf, 500, L"%s\n%s", GetMsg(MErrorMessageTitle), Message);
  return Info.Message(&MainGuid, nullptr,  FMSG_MB_OK|FMSG_WARNING|FMSG_ALLINONE|FMSG_LEFTALIGN, NULL, (const wchar_t **)buf, 0,  0);
};
intptr_t ErrorMessage(const wchar_t *Message1, const wchar_t *Message2)
{
  wchar_t buf[500];
  FSF.snprintf(buf, 500, L"%s\n%s", Message1, Message2);
  return ErrorMessage(buf);
};

int TclStdOut(ClientData, CONST char * buf, int toWrite, int *errorCode)
{
  *errorCode = 0;
  Tcl_SetErrno(0);
  if (ActiveEditor != -1) {
    // TODO: В выходном буфере могут быть \0 символы, они прерывают вывод в буфер
    Info.EditorControl(ActiveEditor, ECTL_INSERTTEXT, 0, (wchar_t *)buf);
    Info.EditorControl(ActiveEditor, ECTL_REDRAW, 0, NULL);
  };
  return toWrite;
};

int TclStdErr(ClientData, CONST char * buf, int toWrite, int *errorCode)
{
  *errorCode = 0;
  Tcl_SetErrno(0);
  if (ActiveEditor != -1) {
    // TODO: В выходном буфере могут быть \0 символы, они прерывают вывод в буфер
    Info.EditorControl(ActiveEditor, ECTL_INSERTTEXT, 0, (wchar_t *)buf);
    Info.EditorControl(ActiveEditor, ECTL_REDRAW, 0, NULL);
  };
  return toWrite;
};

int TclCloseChannel(ClientData instanceData, Tcl_Interp *interp)
{
  return EINVAL;
};
int TclInputChannel(ClientData instanceData, char *buf, int bufSize, int *errorCodePtr)
{
  return EINVAL;
};
void TclWatchChannel(ClientData instanceData, int mask)
{
};
int TclGetHandleChannel(ClientData instanceData, int direction, ClientData *handlePtr)
{
  return TCL_ERROR;
};

Tcl_ChannelType stderrChannelType = {
    "farstderrtype",            /* Type name. */
    TCL_CHANNEL_VERSION_5,  /* v5 channel */
    TclCloseChannel,        /* Close proc. */
    TclInputChannel,        /* Input proc. */
    TclStdErr,              /* Output proc. */
    NULL,                    /* NULL Seek proc. */
    NULL,                    /* NULL Set option proc. */
    NULL,                    /* NULL Get option proc. */
    TclWatchChannel,        /* Set up the notifier to watch the channel. */
    TclGetHandleChannel,    /* Get an OS handle from channel. */
    NULL,                    /* NULL close2proc. */
    NULL,                    /* NULL Set blocking or non-blocking mode. */
    NULL,                    /* NULL flush proc. */
    NULL,                    /* NULL handler proc. */
    NULL,                    /* NULL Wide seek proc. */
    NULL,                    /* NULL Thread action proc. */
    NULL                    /* NULL Truncate proc. */
};

Tcl_ChannelType stdoutChannelType = {
    "farstdouttype",            /* Type name. */
    TCL_CHANNEL_VERSION_5,  /* v5 channel */
    TclCloseChannel,        /* Close proc. */
    TclInputChannel,        /* Input proc. */
    TclStdOut,              /* Output proc. */
    NULL,                    /* NULL Seek proc. */
    NULL,                    /* NULL Set option proc. */
    NULL,                    /* NULL Get option proc. */
    TclWatchChannel,        /* Set up the notifier to watch the channel. */
    TclGetHandleChannel,    /* Get an OS handle from channel. */
    NULL,                    /* NULL close2proc. */
    NULL,                    /* NULL Set blocking or non-blocking mode. */
    NULL,                    /* NULL flush proc. */
    NULL,                    /* NULL handler proc. */
    NULL,                    /* NULL Wide seek proc. */
    NULL,                    /* NULL Thread action proc. */
    NULL                    /* NULL Truncate proc. */
};

void ExecuteBlockLine()
{
  const wchar_t *crlf = L"\n";

  Tcl_DString dString;

  Tcl_DStringInit(&dString);

  EditorInfo ei={sizeof(EditorInfo)};
  Info.EditorControl(-1,ECTL_GETINFO,0,&ei);

  struct EditorGetString egs={sizeof(EditorGetString)};
  if (ei.BlockType != BTYPE_NONE) {
    intptr_t CurrentLine = ei.BlockStartLine;
    Tcl_DString line;
    while (1) {
      egs.StringNumber = CurrentLine++;
      if (!Info.EditorControl(-1,ECTL_GETSTRING,0,&egs))
        break;
      if (egs.SelStart == -1 || egs.SelStart==egs.SelEnd)
        break;
      if (egs.SelEnd != -1)
        egs.StringLength = egs.SelEnd;
      while (egs.SelStart>0) {
        egs.StringText++;
        egs.StringLength--;
        egs.SelStart--;
      };
      if (egs.StringLength > 0) {
        Tcl_DStringInit(&line);
        if (Tcl_DStringLength(&dString) > 0) {
          Tcl_WinTCharToUtf(crlf, sizeof(*crlf), &line);
          Tcl_DStringAppend(&dString, Tcl_DStringValue(&line), Tcl_DStringLength(&line));
        };
        Tcl_WinTCharToUtf(egs.StringText, egs.StringLength * sizeof(wchar_t), &line);
        Tcl_DStringAppend(&dString, Tcl_DStringValue(&line), Tcl_DStringLength(&line));
        Tcl_DStringFree(&line);
      };
    };
    struct EditorSelect es={sizeof(EditorSelect)};
    es.BlockType = BTYPE_NONE;
    Info.EditorControl(-1,ECTL_SELECT,0,&es);
  }
  else {
    egs.StringNumber=-1;
    Info.EditorControl(-1,ECTL_GETSTRING,0,&egs);
    Tcl_WinTCharToUtf(egs.StringText, -1, &dString);
  };

  // ставим курсор на последнюю строку
  struct EditorSetPosition esp={sizeof(EditorSetPosition),-1,-1,-1,-1,-1,-1};
  esp.CurLine = ei.TotalLines - 1;
  esp.CurPos = 0;
  Info.EditorControl(-1,ECTL_SETPOSITION,0,&esp);

  // получаем последнюю строку
  egs.StringNumber=-1;
  Info.EditorControl(-1,ECTL_GETSTRING,0,&egs);

  // если строка пустая - поставить курсор на последний символ и вставить строку
  if (egs.StringLength > 0) {
    esp.CurLine = -1;
    esp.CurPos = egs.StringLength;
    Info.EditorControl(-1,ECTL_SETPOSITION,0,&esp);
    Info.EditorControl(-1,ECTL_INSERTSTRING,0,0);
  };

  ActiveEditor = ei.EditorID;
  int resultcode = Tcl_EvalEx(interp, (char *)Tcl_DStringValue(&dString), Tcl_DStringLength(&dString), TCL_EVAL_GLOBAL);
  ActiveEditor = -1;

  wchar_t sOK[]       = L"[TCL_OK]", \
          sERROR[]    = L"[TCL_ERROR]", \
          sRETURN[]   = L"[TCL_RETURN]", \
          sBREAK[]    = L"[TCL_BREAK]", \
          sCONTINUE[] = L"[TCL_CONTINUE]", \
          sSPACE[]    = L" ";

  switch (resultcode) {
  case TCL_OK:
    Info.EditorControl(-1,ECTL_INSERTTEXT, 0, sOK);
    break;
  case TCL_ERROR:
    Info.EditorControl(-1,ECTL_INSERTTEXT, 0, sERROR);
    break;
  case TCL_RETURN:
    Info.EditorControl(-1,ECTL_INSERTTEXT, 0, sRETURN);
    break;
  case TCL_BREAK:
    Info.EditorControl(-1,ECTL_INSERTTEXT, 0, sBREAK);
    break;
  case TCL_CONTINUE:
    Info.EditorControl(-1,ECTL_INSERTTEXT, 0, sCONTINUE);
    break;
  default:
    wchar_t buff[60];
    FSF.sprintf(buff, L"[TCL_CUSTOM %d]", resultcode);
    Info.EditorControl(-1,ECTL_INSERTTEXT, 0, buff);
  };

  Tcl_WinUtfToTChar(Tcl_GetStringResult(interp), -1, &dString);

  if (Tcl_DStringLength(&dString) > 0) {
    Info.EditorControl(-1,ECTL_INSERTTEXT, 0, sSPACE);
    Info.EditorControl(-1,ECTL_INSERTTEXT, 0, Tcl_DStringValue(&dString));
  };
  Info.EditorControl(-1,ECTL_INSERTSTRING,0,0);

  Tcl_DStringFree(&dString);
};

void InitializeInterp()
{
  if (interp != NULL) {
    Tcl_DeleteInterp(interp);
    interp = NULL;
  };

  interp = Tcl_CreateInterp();

  Tcl_Eval(interp, "encoding system utf-8");
  Tcl_Eval(interp, "set tcl_library [file join [file dirname [info nameofexecutable]] Plugins FarTcl lib tcl]");

   Tcl_Channel stdoutChannel = Tcl_CreateChannel(&stdoutChannelType, "farstdout", (ClientData) TCL_STDOUT, TCL_WRITABLE);
  if (stdoutChannel)
  {
    Tcl_SetChannelOption(NULL, stdoutChannel,"-translation", "lf");
    Tcl_SetChannelOption(NULL, stdoutChannel,"-buffering", "none");
    Tcl_SetChannelOption(NULL, stdoutChannel,"-encoding", "unicode");
    Tcl_RegisterChannel(interp, stdoutChannel);
  };
  Tcl_SetStdChannel(stdoutChannel, TCL_STDOUT);

   Tcl_Channel stderrChannel = Tcl_CreateChannel(&stderrChannelType, "farstderr", (ClientData) TCL_STDERR, TCL_WRITABLE);
  if (stderrChannel)
  {
    Tcl_SetChannelOption(NULL, stderrChannel,"-translation", "lf");
    Tcl_SetChannelOption(NULL, stdoutChannel,"-buffering", "none");
    Tcl_SetChannelOption(NULL, stderrChannel,"-encoding", "unicode");
    Tcl_RegisterChannel(interp, stderrChannel);
  };
  Tcl_SetStdChannel(stderrChannel, TCL_STDERR);

  if (Tcl_Init(interp) == TCL_ERROR) {
    Tcl_DString buf;
    Tcl_WinUtfToTChar(Tcl_GetStringResult(interp), -1, &buf);
    ErrorMessage(GetMsg(MErrorTclInit), (const wchar_t *)Tcl_DStringValue(&buf));
    Tcl_DStringFree(&buf);
  };

  if (Tcl_Eval(interp, "package require farlib") == TCL_ERROR) {
    Tcl_DString buf;
    Tcl_WinUtfToTChar(Tcl_GetStringResult(interp), -1, &buf);
    ErrorMessage(GetMsg(MErrorFarlibInit), (const wchar_t *)Tcl_DStringValue(&buf));
    Tcl_DStringFree(&buf);
  }
  else {
    Tcl_CreateObjCommand(interp, "::Far::core::Message", Far_Message, NULL, NULL);
    Tcl_CreateObjCommand(interp, "::Far::core::GetMsg", Far_GetMsg, NULL, NULL);
    Tcl_CreateObjCommand(interp, "::Far::core::Menu", Far_Menu, NULL, NULL);
    Tcl_CreateObjCommand(interp, "::Far::core::InputBox", Far_InputBox, NULL, NULL);

    Tcl_CreateObjCommand(interp, "::Far::core::PluginControl_GetPlugins", Far_PluginControl_GetPlugins, NULL, NULL);
    Tcl_CreateObjCommand(interp, "::Far::core::PluginControl_GetPluginInfo", Far_PluginControl_GetPluginInfo, NULL, NULL);
    Tcl_CreateObjCommand(interp, "::Far::core::PluginControl_FindPlugin", Far_PluginControl_FindPlugin, NULL, NULL);
    Tcl_CreateObjCommand(interp, "::Far::core::PluginControl_UnloadPlugin", Far_PluginControl_UnloadPlugin, NULL, NULL);
    Tcl_CreateObjCommand(interp, "::Far::core::PluginControl_LoadPlugin", Far_PluginControl_LoadPlugin, NULL, NULL);

    Tcl_CreateObjCommand(interp, "::Far::core::EditorControl", Far_EditorControl, NULL, NULL);
  };

  wchar_t guidstr[37];
  FSF.sprintf(guidstr, L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", \
      MainGuid.Data1, MainGuid.Data2, MainGuid.Data3, \
      MainGuid.Data4[0], MainGuid.Data4[1], MainGuid.Data4[2], MainGuid.Data4[3], \
      MainGuid.Data4[4], MainGuid.Data4[5], MainGuid.Data4[6], MainGuid.Data4[7]);

  Tcl_DString buf;
  Tcl_WinTCharToUtf(guidstr, -1, &buf);
  Tcl_SetVar(interp, "::Far::MainGUID", Tcl_DStringValue(&buf), 0);
  Tcl_DStringFree(&buf);

};

__declspec(dllexport) void WINAPI GetGlobalInfoW(struct GlobalInfo *Info)
{
  Info->StructSize=sizeof(GlobalInfo);
  Info->MinFarVersion=FARMANAGERVERSION;
  Info->Version=VER_INTER;
  Info->Guid=MainGuid;
  Info->Title=PLUGIN_NAME;
  Info->Description=PLUGIN_DESC;
  Info->Author=PLUGIN_AUTHOR;
}

__declspec(dllexport) void WINAPI SetStartupInfoW(const struct PluginStartupInfo *Info)
{
  ::Info=*Info;
  FSF=*Info->FSF;
  ::Info.FSF=&FSF;

  Tcl_FindExecutable(::Info.ModuleName);
  InitializeInterp();
}

__declspec(dllexport) void WINAPI GetPluginInfoW(struct PluginInfo *Info)
{
  Info->StructSize=sizeof(*Info);
  Info->Flags=PF_EDITOR;
  static const wchar_t *PluginMenuStrings[1];
  PluginMenuStrings[0]=GetMsg(MFarTclMenu);
  Info->PluginMenu.Guids=&MenuGuid;
  Info->PluginMenu.Strings=PluginMenuStrings;
  Info->PluginMenu.Count=ARRAYSIZE(PluginMenuStrings);
}

__declspec(dllexport) HANDLE WINAPI OpenW(const struct OpenInfo *OInfo)
{
  if (OInfo->OpenFrom == OPEN_EDITOR) {
    size_t i;
    struct FarMenuItem MenuItems[2] = {}, *MenuItem;
    int Msgs[]={MMenuEditorExecute, MMenuEditorReinit};

    for (MenuItem=MenuItems,i=0; i < ARRAYSIZE(MenuItems); ++i, ++MenuItem)
    {
      MenuItem->Text = GetMsg(Msgs[i]);
    };

    MenuItems[0].Flags=MIF_SELECTED;
    int MenuCode=(int)Info.Menu(&MainGuid, nullptr,-1,-1,0,FMENU_AUTOHIGHLIGHT|FMENU_WRAPMODE,
                             GetMsg(MFarTclMenu),NULL,NULL,NULL,NULL,
                             MenuItems,ARRAYSIZE(MenuItems));

    switch (MenuCode)
    {
      // Exit menu
      case -1:
        break;
      // reinit interp
      case 1:
        InitializeInterp();
        break;
      // execute block/line
      case 0:
        ExecuteBlockLine();
        break;
    }
    return nullptr;
  };

  if (OInfo->OpenFrom == OPEN_PLUGINSMENU)
  {
    wchar_t TempName[MAX_PATH];
    FSF.MkTemp(TempName,MAX_PATH-4,NULL);
    wcscat_s(TempName, MAX_PATH, L".tcl");
    Info.Editor(TempName, NULL, -1, -1, -1, -1, EF_NONMODAL|EF_IMMEDIATERETURN|EF_DELETEONLYFILEONCLOSE|EF_CREATENEW|EF_DISABLEHISTORY|EF_DISABLESAVEPOS, 1, 1, CP_DEFAULT);
    if (interp != NULL) {
      Tcl_Obj *objPtr;
      Tcl_DString ds;
      objPtr = Tcl_NewObj();
      Tcl_ListObjAppendElement(interp, objPtr, Tcl_NewStringObj("cd", -1));
      FSF.GetCurrentDirectory(MAX_PATH, TempName);
      Tcl_WinTCharToUtf(TempName, -1, &ds);
      Tcl_ListObjAppendElement(interp, objPtr, Tcl_NewStringObj(Tcl_DStringValue(&ds), -1));
      Tcl_DStringFree(&ds);

      Tcl_IncrRefCount(objPtr);
      Tcl_EvalObjEx(interp, objPtr, TCL_EVAL_GLOBAL|TCL_EVAL_DIRECT);
      Tcl_DecrRefCount(objPtr);
    };

    return nullptr;
  };

  return nullptr;
};
