
int Far_PluginControl_GetPlugins(ClientData cd, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  size_t PluginsCount=Info.PluginsControl(INVALID_HANDLE_VALUE, PCTL_GETPLUGINS, 0, NULL);
  HANDLE *Plugins=(HANDLE*)ckalloc(PluginsCount*sizeof(HANDLE));
  Info.PluginsControl(INVALID_HANDLE_VALUE, PCTL_GETPLUGINS, PluginsCount, Plugins);

  InitResult_CToTcl();
  for (size_t i=0; i<PluginsCount; i++)  {
    AppendResult_CToTcl_Pointer(Plugins[i]);
  };

  ckfree(Plugins);
  SetResult_CToTcl();

  return TCL_OK;
};

int Far_PluginControl_GetPluginInfo(ClientData cd, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  int  return_code = TCL_OK;

  InitParam_TclToC_Pointer(1, hHandle, HANDLE);

  size_t size=Info.PluginsControl(hHandle, PCTL_GETPLUGININFORMATION, 0, NULL);
  if (size > 0) {
    FarGetPluginInformation *FGPInfo=(FarGetPluginInformation*)ckalloc(size);
    FGPInfo->StructSize=sizeof(FarGetPluginInformation);
    Info.PluginsControl(hHandle,PCTL_GETPLUGININFORMATION,size,FGPInfo);

    Tcl_Obj *PInfo = Tcl_NewDictObj();
    Tcl_DictObjPut(interp, PInfo, Tcl_NewStringObj("flags", -1), Tcl_NewWideIntObj(FGPInfo->PInfo->Flags));
    Tcl_DictObjPut(interp, PInfo, Tcl_NewStringObj("commandprefix", -1), Tcl_NewWStringObj(FGPInfo->PInfo->CommandPrefix));

    Tcl_Obj *DiskMenu = Tcl_NewListObj(0, NULL);
    for (size_t i=0; i<FGPInfo->PInfo->DiskMenu.Count; i++) {
      Tcl_ListObjAppendElement(interp, DiskMenu, Tcl_NewGuidObj(FGPInfo->PInfo->DiskMenu.Guids[i]));
      Tcl_ListObjAppendElement(interp, DiskMenu, Tcl_NewWStringObj(FGPInfo->PInfo->DiskMenu.Strings[i]));
    };
    Tcl_DictObjPut(interp, PInfo, Tcl_NewStringObj("diskmenu", -1), DiskMenu);

    Tcl_Obj *PluginMenu = Tcl_NewListObj(0, NULL);
    for (size_t i=0; i<FGPInfo->PInfo->PluginMenu.Count; i++) {
      Tcl_ListObjAppendElement(interp, PluginMenu, Tcl_NewGuidObj(FGPInfo->PInfo->PluginMenu.Guids[i]));
      Tcl_ListObjAppendElement(interp, PluginMenu, Tcl_NewWStringObj(FGPInfo->PInfo->PluginMenu.Strings[i]));
    };
    Tcl_DictObjPut(interp, PInfo, Tcl_NewStringObj("pluginmenu", -1), PluginMenu);

    Tcl_Obj *PluginConfig = Tcl_NewListObj(0, NULL);
    for (size_t i=0; i<FGPInfo->PInfo->PluginConfig.Count; i++) {
      Tcl_ListObjAppendElement(interp, PluginConfig, Tcl_NewGuidObj(FGPInfo->PInfo->PluginConfig.Guids[i]));
      Tcl_ListObjAppendElement(interp, PluginConfig, Tcl_NewWStringObj(FGPInfo->PInfo->PluginConfig.Strings[i]));
    };
    Tcl_DictObjPut(interp, PInfo, Tcl_NewStringObj("pluginconfig", -1), PluginConfig);

    Tcl_Obj *FGPInfoDict = Tcl_NewDictObj();
    Tcl_DictObjPut(interp, FGPInfoDict, Tcl_NewStringObj("plugininfo", -1), PInfo);

    Tcl_DictObjPut(interp, FGPInfoDict, Tcl_NewStringObj("modulename", -1), Tcl_NewWStringObj(FGPInfo->ModuleName));
    Tcl_DictObjPut(interp, FGPInfoDict, Tcl_NewStringObj("flags", -1), Tcl_NewWideIntObj(FGPInfo->Flags));
    Tcl_DictObjPut(interp, FGPInfoDict, Tcl_NewStringObj("id", -1), Tcl_NewGuidObj(FGPInfo->GInfo->Guid));
    Tcl_DictObjPut(interp, FGPInfoDict, Tcl_NewStringObj("title", -1), Tcl_NewWStringObj(FGPInfo->GInfo->Title));
    Tcl_DictObjPut(interp, FGPInfoDict, Tcl_NewStringObj("description", -1), Tcl_NewWStringObj(FGPInfo->GInfo->Description));
    Tcl_DictObjPut(interp, FGPInfoDict, Tcl_NewStringObj("author", -1), Tcl_NewWStringObj(FGPInfo->GInfo->Author));
    Tcl_DictObjPut(interp, FGPInfoDict, Tcl_NewStringObj("ver_major", -1), Tcl_NewIntObj(FGPInfo->GInfo->Version.Major));
    Tcl_DictObjPut(interp, FGPInfoDict, Tcl_NewStringObj("ver_minor", -1), Tcl_NewIntObj(FGPInfo->GInfo->Version.Minor));
    Tcl_DictObjPut(interp, FGPInfoDict, Tcl_NewStringObj("ver_revision", -1), Tcl_NewIntObj(FGPInfo->GInfo->Version.Revision));
    Tcl_DictObjPut(interp, FGPInfoDict, Tcl_NewStringObj("ver_build", -1), Tcl_NewIntObj(FGPInfo->GInfo->Version.Build));
    Tcl_DictObjPut(interp, FGPInfoDict, Tcl_NewStringObj("ver_stage", -1), Tcl_NewIntObj(FGPInfo->GInfo->Version.Stage));
    Tcl_DictObjPut(interp, FGPInfoDict, Tcl_NewStringObj("far_ver_major", -1), Tcl_NewIntObj(FGPInfo->GInfo->MinFarVersion.Major));
    Tcl_DictObjPut(interp, FGPInfoDict, Tcl_NewStringObj("far_ver_minor", -1), Tcl_NewIntObj(FGPInfo->GInfo->MinFarVersion.Minor));
    Tcl_DictObjPut(interp, FGPInfoDict, Tcl_NewStringObj("far_ver_revision", -1), Tcl_NewIntObj(FGPInfo->GInfo->MinFarVersion.Revision));
    Tcl_DictObjPut(interp, FGPInfoDict, Tcl_NewStringObj("far_ver_build", -1), Tcl_NewIntObj(FGPInfo->GInfo->MinFarVersion.Build));
    Tcl_DictObjPut(interp, FGPInfoDict, Tcl_NewStringObj("far_ver_stage", -1), Tcl_NewIntObj(FGPInfo->GInfo->MinFarVersion.Stage));
    Tcl_SetObjResult(interp, FGPInfoDict);

    ckfree(FGPInfo);
  }
  else {
    Tcl_AppendResult(interp, "unknown plugin handle", NULL);
    return_code = TCL_ERROR;
  };

  FreeParam_TclToC_Pointer(hHandle);

  return return_code;
};

int Far_PluginControl_UnloadPlugin(ClientData cd, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  InitParam_TclToC_Pointer(1, hHandle, HANDLE);

  intptr_t result = Info.PluginsControl(hHandle, PCTL_UNLOADPLUGIN, 0, NULL);

  FreeParam_TclToC_Pointer(hHandle);

  SetResult_CToTcl_Int(result);
  return TCL_OK;
};

int Far_PluginControl_FindPlugin(ClientData cd, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  int  return_code = TCL_OK;

  InitParam_TclToC_Int(1, FindType);

  HANDLE hPlugin = NULL;
  if ( FindType != PFM_GUID && FindType != PFM_MODULENAME ) {
    Tcl_AppendResult(interp, "unknown plugin find method", NULL);
    return_code = TCL_ERROR;
  }
  else {
    if ( FindType == PFM_GUID ) {
      InitParam_TclToC_GUID(2, PluginId);
      if ( PluginIdPtr == nullptr ) {
        hPlugin = nullptr;
        Tcl_AppendResult(interp, "invalid guid, ", NULL);
      }
      else {
        hPlugin = reinterpret_cast<HANDLE>(Info.PluginsControl(INVALID_HANDLE_VALUE, PCTL_FINDPLUGIN, PFM_GUID, PluginIdPtr));
      };
      FreeParam_TclToC_GUID(PluginId);
    } else if ( FindType == PFM_MODULENAME ) {
      InitParam_TclToC_String(2, ModuleName);
      hPlugin = reinterpret_cast<HANDLE>(Info.PluginsControl(INVALID_HANDLE_VALUE, PCTL_FINDPLUGIN, PFM_MODULENAME, ModuleName));
      FreeParam_TclToC_String(ModuleName);
    };
    if ( hPlugin == nullptr ) {
      Tcl_AppendResult(interp, "cant find requested plugin", NULL);
      return_code = TCL_ERROR;
    }
    else {
      SetResult_CToTcl_Pointer(hPlugin);
    };
  };

  FreeParam_TclToC_Int(FindType);

  return return_code;
};

int Far_PluginControl_LoadPlugin(ClientData cd, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
  InitParam_TclToC_String(1, ModuleName);
  InitParam_TclToC_Boolean(2, ForceLoad);

  HANDLE hPlugin = NULL;
  if ( ForceLoad ) {
    hPlugin = reinterpret_cast<HANDLE>(Info.PluginsControl(INVALID_HANDLE_VALUE, PCTL_FORCEDLOADPLUGIN, PLT_PATH, ModuleName));
  }
  else {
    hPlugin = reinterpret_cast<HANDLE>(Info.PluginsControl(INVALID_HANDLE_VALUE, PCTL_LOADPLUGIN, PLT_PATH, ModuleName));
  };

  FreeParam_TclToC_String(ModuleName);
  FreeParam_TclToC_Boolean(ForceLoad);

  if ( hPlugin == nullptr ) {
    Tcl_AppendResult(interp, "cant load plugin module", NULL);
    return TCL_ERROR;
  }
  else {
    SetResult_CToTcl_Pointer(hPlugin);
    return TCL_OK;
  };
};