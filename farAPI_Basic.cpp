
int Far_Message(ClientData cd, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  InitParam_TclToC_GUID(1, PluginId);
  InitParam_TclToC_GUID(2, Id);
  InitParam_TclToC_WideInt(3, Flags);
  InitParam_TclToC_String(4, HelpTopic);
  InitParam_TclToC_String(5, Items);
  InitParam_TclToC_Int(6, ButtonsNumber);

  intptr_t result = Info.Message(PluginIdPtr, IdPtr,  Flags, HelpTopic, (const wchar_t **)Items, 0,  ButtonsNumber);

  FreeParam_TclToC_GUID(PluginId);
  FreeParam_TclToC_GUID(Id);
  FreeParam_TclToC_WideInt(Flags);
  FreeParam_TclToC_String(HelpTopic);
  FreeParam_TclToC_String(Items);
  FreeParam_TclToC_Int(ButtonsNumber);

  SetResult_CToTcl_Int(result);
  return TCL_OK;
};

int Far_GetMsg(ClientData cd, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  InitParam_TclToC_GUID(1, PluginId);
  InitParam_TclToC_Int(2, MsgId);

  const wchar_t* Msg = Info.GetMsg(PluginIdPtr, MsgId);

  FreeParam_TclToC_GUID(PluginId);
  FreeParam_TclToC_Int(MsgId);

  SetResult_CToTcl_String(Msg);
  return TCL_OK;
};

int Far_Menu(ClientData cd, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  InitParam_TclToC_GUID(1, PluginId);
  InitParam_TclToC_GUID(2, Id);
  InitParam_TclToC_Int(3, X);
  InitParam_TclToC_Int(4, Y);
  InitParam_TclToC_Int(5, MaxHeight);
  InitParam_TclToC_WideInt(6, Flags);
  InitParam_TclToC_String(7, Title);
  InitParam_TclToC_String(8, Bottom);
  InitParam_TclToC_String(9, HelpTopic);

  int ItemsNumber;
  FarMenuItem  *MenuItems = NULL;
  Tcl_DString *MenuItemsDS = NULL;

  Tcl_ListObjLength(interp, objv[10], &ItemsNumber);

  MenuItems = (FarMenuItem *)ckalloc(ItemsNumber*sizeof(FarMenuItem));
  memset(MenuItems, 0, ItemsNumber*sizeof(FarMenuItem));
  MenuItemsDS = (Tcl_DString *)ckalloc(ItemsNumber*sizeof(Tcl_DString));
  for(int i = 0; i < ItemsNumber; ++i)
  {
    Tcl_Obj* item;
    Tcl_WideInt MenuItemFlags;

    Tcl_ListObjIndex(interp, objv[10], i, &item);
    Tcl_WinUtfToTChar(Tcl_GetStringFromObj(item, NULL), -1, &MenuItemsDS[i]);

    Tcl_ListObjIndex(interp, objv[11], i, &item);
    Tcl_GetWideIntFromObj(interp, item, &MenuItemFlags);

    if (Tcl_DStringLength(&MenuItemsDS[i])) {
      MenuItems[i].Text = (wchar_t *)Tcl_DStringValue(&MenuItemsDS[i]);
    }
    else {
      MenuItems[i].Text = NULL;
    };

    MenuItems[i].Flags = MenuItemFlags;
  };


  // TODO: BreakKeys, BreakCode
  struct FarKey *BreakKeys = NULL;
  intptr_t BreakCode = -1;

  int result = (int)Info.Menu(PluginIdPtr, IdPtr, X, Y, MaxHeight, Flags,
                             Title, Bottom, HelpTopic, BreakKeys, &BreakCode,
                             MenuItems, ItemsNumber);

  for(int i = 0; i < ItemsNumber; ++i) Tcl_DStringFree(&MenuItemsDS[i]);
  ckfree(MenuItems);
  ckfree(MenuItemsDS);

  FreeParam_TclToC_GUID(PluginId);
  FreeParam_TclToC_GUID(Id);
  FreeParam_TclToC_Int(X);
  FreeParam_TclToC_Int(Y);
  FreeParam_TclToC_Int(MaxHeight);
  FreeParam_TclToC_WideInt(Flags);
  FreeParam_TclToC_String(Title);
  FreeParam_TclToC_String(Bottom);
  FreeParam_TclToC_String(HelpTopic);

  InitResult_CToTcl();
  AppendResult_CToTcl_Int(result);
  AppendResult_CToTcl_Int(BreakCode);
  SetResult_CToTcl();
  return TCL_OK;
};

int Far_InputBox(ClientData cd, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  InitParam_TclToC_GUID(1, PluginId);
  InitParam_TclToC_GUID(2, Id);
  InitParam_TclToC_String(3, Title);
  InitParam_TclToC_String(4, SubTitle);
  InitParam_TclToC_String(5, HistoryName);
  InitParam_TclToC_String(6, SrcText);
  InitParam_TclToC_Int(7, DestSize);
  InitParam_TclToC_String(8, HelpTopic);
  InitParam_TclToC_WideInt(9, Flags);

  wchar_t *DestText = new wchar_t[++DestSize];

  intptr_t result = Info.InputBox(PluginIdPtr, IdPtr, Title, SubTitle, HistoryName, SrcText,
                                  DestText, DestSize, HelpTopic, Flags);

  FreeParam_TclToC_GUID(PluginId);
  FreeParam_TclToC_GUID(Id);
  FreeParam_TclToC_String(Title);
  FreeParam_TclToC_String(SubTitle);
  FreeParam_TclToC_String(HistoryName);
  FreeParam_TclToC_String(SrcText);
  FreeParam_TclToC_Int(DestSize);
  FreeParam_TclToC_String(HelpTopic);
  FreeParam_TclToC_WideInt(Flags);

  InitResult_CToTcl();
  AppendResult_CToTcl_Int(result);
  AppendResult_CToTcl_String(DestText);
  delete [] DestText;
  SetResult_CToTcl();
  return TCL_OK;
};
