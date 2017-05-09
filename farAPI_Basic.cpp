
APICall(Message) {
  InitParam(GUID,  PluginId,      1);
  InitParam(GUID,  Id,            2);
  InitParam(UWide, Flags,         3);
  InitParam(Str,   HelpTopic,     4);
  InitParam(Str,   Items,         5);
  InitParam(Int,   ButtonsNumber, 6);

  intptr_t result = Info.Message(PluginId, Id,  Flags, HelpTopic, (const wchar_t **)Items, 0,  ButtonsNumber);

  FreeParam(GUID,  PluginId);
  FreeParam(GUID,  Id);
  FreeParam(UWide, Flags);
  FreeParam(Str,   HelpTopic);
  FreeParam(Str,   Items);
  FreeParam(Int,   ButtonsNumber);

  ReturnResult(Int, result, OK);
};

APICall(GetMsg) {
  InitParam(GUID, PluginId, 1);
  InitParam(Int,  MsgId,    2);

  const wchar_t* Msg = Info.GetMsg(PluginId, MsgId);

  FreeParam(GUID, PluginId);
  FreeParam(Int,  MsgId);

  ReturnResult(Str, Msg, OK);
};

APICall(Menu) {
  InitParam(GUID,  PluginId,  1);
  InitParam(GUID,  Id,        2);
  InitParam(Int,   X,         3);
  InitParam(Int,   Y,         4);
  InitParam(Int,   MaxHeight, 5);
  InitParam(UWide, Flags,     6);
  InitParam(Str,   Title,     7);
  InitParam(Str,   Bottom,    8);
  InitParam(Str,   HelpTopic, 9);

  int ItemsNumber;
  Tcl_ListObjLength(interp, objv[10], &ItemsNumber);

  FarMenuItem *MenuItems = \
      (FarMenuItem *)ckalloc(ItemsNumber*sizeof(FarMenuItem));
  Tcl_DString *MenuItemsDS = \
      (Tcl_DString *)ckalloc(ItemsNumber*sizeof(Tcl_DString));

  // initialize menuitem space
  memset(MenuItems, 0, ItemsNumber*sizeof(FarMenuItem));
  // initialize dstring space
  for(int i = 0; i < ItemsNumber; ++i) \
      Tcl_DStringInit(&MenuItemsDS[i]);

  for(int i = 0; i < ItemsNumber; ++i) {

      Tcl_Obj* item;

      Tcl_ListObjIndex(interp, objv[10], i, &item);
      Tcl_WinUtfToTChar(Tcl_GetStringFromObj(item, nullptr), -1, &MenuItemsDS[i]);

      Tcl_ListObjIndex(interp, objv[11], i, &item);
      Tcl_GetUWideIntFromObj(interp, item, &MenuItems[i].Flags);

      if (Tcl_DStringLength(&MenuItemsDS[i])) {
          MenuItems[i].Text = (wchar_t *)Tcl_DStringValue(&MenuItemsDS[i]);
      } else {
          MenuItems[i].Text = nullptr;
      };

  };

  // TODO: BreakKeys, BreakCode
  struct FarKey *BreakKeys = nullptr;

  intptr_t BreakCode = -1;

  intptr_t result = Info.Menu(PluginId, Id, X, Y, MaxHeight, Flags,
                              Title, Bottom, HelpTopic, BreakKeys, &BreakCode,
                              MenuItems, ItemsNumber);

  // free allocated memory
  for(int i = 0; i < ItemsNumber; ++i) \
      Tcl_DStringFree(&MenuItemsDS[i]);
  ckfree(MenuItems);
  ckfree(MenuItemsDS);

  FreeParam(GUID,  PluginId);
  FreeParam(GUID,  Id);
  FreeParam(Int,   X);
  FreeParam(Int,   Y);
  FreeParam(Int,   MaxHeight);
  FreeParam(UWide, Flags);
  FreeParam(Str,   Title);
  FreeParam(Str,   Bottom);
  FreeParam(Str,   HelpTopic);

  InitResult(List);
  LAppend(Result, Int, result);
  LAppend(Result, Int, BreakCode);

  Return(OK);
};

APICall(InputBox) {
  InitParam(GUID,  PluginId,    1);
  InitParam(GUID,  Id,          2);
  InitParam(Str,   Title,       3);
  InitParam(Str,   SubTitle,    4);
  InitParam(Str,   HistoryName, 5);
  InitParam(Str,   SrcText,     6);
  InitParam(Int,   DestSize,    7);
  InitParam(Str,   HelpTopic,   8);
  InitParam(UWide, Flags,       9);

  // Increase DestSize by adding space for \0 line ending
  DestSize++;

  wchar_t *DestText = (wchar_t *)ckalloc(DestSize);

  intptr_t result = Info.InputBox(PluginId, Id, Title, SubTitle, HistoryName, SrcText,
                                  DestText, DestSize, HelpTopic, Flags);

  FreeParam(GUID,  PluginId);
  FreeParam(GUID,  Id);
  FreeParam(Str,   Title);
  FreeParam(Str,   SubTitle);
  FreeParam(Str,   HistoryName);
  FreeParam(Str,   SrcText);
  FreeParam(Int,   DestSize);
  FreeParam(Str,   HelpTopic);
  FreeParam(UWide, Flags);

  InitResult(List);
  LAppend(Result, Int, result);
  LAppend(Result, Str, DestText);

  ckfree(DestText);

  Return(OK);
};
