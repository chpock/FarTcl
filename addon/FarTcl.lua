local FarTcl_MainGUID="BFF142A4-F392-4E7D-84AA-A00298CAF9EF"

Macro {
  area="Editor"; key="CtrlEnter"; description="FarTcl: Execute selected block/line"; flags="EnableOutput";
  condition=function() return Plugin.Exist(FarTcl_MainGUID) end;
  action=function() Plugin.Menu(FarTcl_MainGUID) Keys("Enter") end;
}

