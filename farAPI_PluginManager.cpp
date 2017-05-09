APICall(GetPlugins) {

    size_t PluginsCount = Info.PluginsControl(INVALID_HANDLE_VALUE, PCTL_GETPLUGINS, 0, NULL);

    HANDLE *Plugins = (HANDLE*)ckalloc(PluginsCount*sizeof(HANDLE));

    Info.PluginsControl(INVALID_HANDLE_VALUE, PCTL_GETPLUGINS, PluginsCount, Plugins);

    InitResult(List);

    for (size_t i = 0; i<PluginsCount; i++) {
        LAppend(Result, Ptr, Plugins[i]);
    }

    ckfree(Plugins);

    Return(OK);
}

APICall(GetPluginInfo) {

    InitReturnCode();

    InitParam(HANDLE, hHandle, 1);

    size_t size = Info.PluginsControl(hHandle, PCTL_GETPLUGININFORMATION, 0, nullptr);

    if (size) {

        // TODO: Check OOM
        FarGetPluginInformation *FGPInfo=(FarGetPluginInformation*)ckalloc(size);
        FGPInfo->StructSize=sizeof(FarGetPluginInformation);
        
        Info.PluginsControl(hHandle, PCTL_GETPLUGININFORMATION, size, FGPInfo);

        NewList(DiskMenu);
        for (size_t i = 0; i < FGPInfo->PInfo->DiskMenu.Count; i++) {
            LAppend(DiskMenu, GUID, FGPInfo->PInfo->DiskMenu.Guids[i]);
            LAppend(DiskMenu, Str,  FGPInfo->PInfo->DiskMenu.Strings[i]);
        }

        NewList(PluginMenu);
        for (size_t i = 0; i < FGPInfo->PInfo->PluginMenu.Count; i++) {
            LAppend(PluginMenu, GUID, FGPInfo->PInfo->PluginMenu.Guids[i]);
            LAppend(PluginMenu, Str,  FGPInfo->PInfo->PluginMenu.Strings[i]);
        }

        NewList(PluginConfig);
        for (size_t i = 0; i < FGPInfo->PInfo->PluginConfig.Count; i++) {
            LAppend(PluginConfig, GUID, FGPInfo->PInfo->PluginConfig.Guids[i]);
            LAppend(PluginConfig, Str,  FGPInfo->PInfo->PluginConfig.Strings[i]);
        }

        NewList(PluginVersion);
        LAppend(PluginVersion, Int, FGPInfo->GInfo->Version.Major);
        LAppend(PluginVersion, Int, FGPInfo->GInfo->Version.Minor);
        LAppend(PluginVersion, Int, FGPInfo->GInfo->Version.Revision);
        LAppend(PluginVersion, Int, FGPInfo->GInfo->Version.Build);
        LAppend(PluginVersion, Int, FGPInfo->GInfo->Version.Stage);

        NewList(MinFarVersion);
        LAppend(MinFarVersion, Int, FGPInfo->GInfo->MinFarVersion.Major);
        LAppend(MinFarVersion, Int, FGPInfo->GInfo->MinFarVersion.Minor);
        LAppend(MinFarVersion, Int, FGPInfo->GInfo->MinFarVersion.Revision);
        LAppend(MinFarVersion, Int, FGPInfo->GInfo->MinFarVersion.Build);
        LAppend(MinFarVersion, Int, FGPInfo->GInfo->MinFarVersion.Stage);

        NewDict(PInfo);
        DSet(PInfo, "flags",         UWide, FGPInfo->PInfo->Flags);
        DSet(PInfo, "commandprefix", Str,   FGPInfo->PInfo->CommandPrefix);
        DSet(PInfo, "diskmenu",      Obj,   DiskMenu);
        DSet(PInfo, "pluginmenu",    Obj,   PluginMenu); 
        DSet(PInfo, "pluginconfig",  Obj,   PluginConfig);

        InitResult(Dict);
        DSet(Result, "plugininfo",   Obj,   PInfo);
        DSet(Result, "modulename",   Str,   FGPInfo->ModuleName);
        DSet(Result, "flags",        UWide, FGPInfo->Flags);
        DSet(Result, "title",        Str,   FGPInfo->GInfo->Title);
        DSet(Result, "description",  Str,   FGPInfo->GInfo->Description);
        DSet(Result, "author",       Str,   FGPInfo->GInfo->Author);
        DSet(Result, "ver",          Obj,   PluginVersion);
        DSet(Result, "min_far_ver",  Obj,   MinFarVersion);
            
        ckfree(FGPInfo);

    } else {

        InitResult(Str, L"unknown plugin handle");
        SetReturnCode(ERROR);

    }

    FreeParam(HANDLE, hHandle);

    Return();
}

APICall(UnloadPlugin) {

    InitParam(HANDLE, hHandle, 1);

    intptr_t result = Info.PluginsControl(hHandle, PCTL_UNLOADPLUGIN, 0, nullptr);

    FreeParam(HANDLE, hHandle);

    ReturnResult(Int, result, OK);

}

APICall(FindPlugin) {

    InitParam(Int, FindType, 1);

    HANDLE hPlugin = nullptr;

    switch (FindType) {
        case PFM_GUID: {
         
            InitParam(GUID, PluginId, 2);

            hPlugin = reinterpret_cast<HANDLE>(Info.PluginsControl( \
                INVALID_HANDLE_VALUE, PCTL_FINDPLUGIN, PFM_GUID, PluginId));

            FreeParam(GUID, PluginId);

            break;
        }
        case PFM_MODULENAME: {
          
            InitParam(Str, ModuleName, 2);

            hPlugin = reinterpret_cast<HANDLE>(Info.PluginsControl( \
                INVALID_HANDLE_VALUE, PCTL_FINDPLUGIN, PFM_MODULENAME, ModuleName));

            FreeParam(Str, ModuleName);

            break;
        }
    }

    FreeParam(Int, FindType);

    if (hPlugin) {
        ReturnResult(HANDLE, hPlugin, OK);
    } else {
        ReturnResult(Str, L"can't find plugin", ERROR);
    }

}

APICall(LoadPlugin) {

  InitParam(Str,  ModuleName, 1);
  InitParam(Bool, ForceLoad,  2);

  HANDLE hPlugin = nullptr;

  if (ForceLoad) {
    hPlugin = reinterpret_cast<HANDLE>(Info.PluginsControl( \
        INVALID_HANDLE_VALUE, PCTL_FORCEDLOADPLUGIN, PLT_PATH, ModuleName));
  } else {
    hPlugin = reinterpret_cast<HANDLE>(Info.PluginsControl( \
        INVALID_HANDLE_VALUE, PCTL_LOADPLUGIN, PLT_PATH, ModuleName));
  }

  FreeParam(Str,  ModuleName);
  FreeParam(Bool, ForceLoad);

  if (hPlugin) {
      ReturnResult(HANDLE, hPlugin, OK);
  } else {
      ReturnResult(Str, L"can't load plugin module", ERROR);
  }

}