package require farlib::flags 0.0.1
package require procarg
package provide farlib 0.0.1

namespace eval ::Far {
  namespace export *

  procarg regtype guid -expression {[string match -nocase {[0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f]-[0-9a-f][0-9a-f][0-9a-f][0-9a-f]-[0-9a-f][0-9a-f][0-9a-f][0-9a-f]-[0-9a-f][0-9a-f][0-9a-f][0-9a-f]-[0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f]} $val]}
  procarg regtype pointer -expression {[string match -nocase {ptr[0-9a-f][0-9a-f]*} $val]}

  procarg register Message {
    {0          string  -allowempty false}
    {-pluginid  guid    -allowempty true}
    {-id        guid    -allowempty true}
    {-warning   switch}
    {-leftalign switch}
    {-allinone  switch}
    {-stdbutt   string  -restrict {"" ok okcancel abortretryignore yesno yesnocancel retrycancel}}
    {-helptopic string}
    {-title     string}
    {-buttons   list}
  }

  procarg register GetMsg {
    {0          int  -restrict { { 0 + } }}
    {-pluginid  guid -allowempty true}
  }

  procarg register Menu {
    {0                    list -restrict { 1 + }}
    {-pluginid            guid -allowempty true}
    {-id                  guid -allowempty true}
    {-x                   int  -default -1 -restrict { { -1 + } }}
    {-y                   int  -default -1 -restrict { { -1 + } }}
    {-maxheight           int  -default 0 -restrict { { 0 + } }}
    {-title               str}
    {-bottom              str}
    {-helptopic           str}
    {-autohl              switch}
    {-changeconsoletitle  switch}
    {-showamp             switch}
    {-reverseautohl       switch}
    {-wrap                switch}
    {-select              int  -default 0 -restrict { { 0 + } }}
    {-checked             list}
    {-separator           list}
    {-disabled            list}
    {-grayed              list}
    {-hidden              list}
  }

  procarg register InputBox {
    {-pluginid          guid -allowempty true}
    {-id                guid -allowempty true}
    {-title             string}
    {-subtitle          string}
    {-historyname       string}
    {-srctext           string}
    {-destsize          int  -default 1024 -restrict { { 1 + } }}
    {-helptopic         string}
    {-enableempty       switch}
    {-password          switch}
    {-expandenv         switch}
    {-nouselasthistory  switch}
    {-buttons           switch}
    {-noampersand       switch}
    {-editpath          switch}
    {-editpathexec      switch}
  }

  namespace eval PluginControl {
    namespace export *
    namespace ensemble create

    procarg register load {
      {0           string -allowempty false}
      {-forceload  switch}
    }

    procarg register findbyid {
      {0 guid}
    }

    procarg register findbymodulename {
      {0 string -allowempty false}
    }

    procarg register unload {
      {0 pointer}
    }

    procarg register getinfo {
      {0 pointer}
    }

  }

  namespace eval EditorControl {
    namespace export *
    namespace ensemble create

    procarg register addcolor {
      {0               int}
      {1               int}
      {2               list -restrict { 3 3 }}
      {-id             int  -default -1 -restrict { {-1 +} }}
      {-line           int  -default -1 -restrict { {-1 +} }}
      {-priority       int  -default 0 -restrict { {0 +} }}
      {-owner          guid -allowempty true}
      {-tabmarkfirst   switch}
      {-tabmarkcurrent switch}
    }

    procarg register delcolor {
      {-id    int  -default -1 -restrict { {-1 +} }}
      {-owner guid -allowempty true}
      {-line  int  -default -1 -restrict { {-1 +} }}
      {-pos   int  -default -1 -restrict { {-1 +} }}
    }

    procarg register getcolor {
      {-id   int -default -1 -restrict { {-1 +} }}
      {-line int -default -1 -restrict { {-1 +} }}
      {-item int -default 0  -restrict { {0 +} }}
    }

    procarg register select {
      {0       string -restrict {none stream column}}
      {-id     int -default -1 -restrict { {-1 +} }}
      {-line   int -default -1 -restrict { {-1 +} }}
      {-pos    int -default 0  -restrict { {0 +} }}
      {-width  int -default 0}
      {-height int -default 1 -restrict { {1 +} }}
    }

    procarg register redraw {
      {-id int -default -1 -restrict { {-1 +} }}
    }

    procarg register deleteblock {
      {-id int -default -1 -restrict { {-1 +} }}
    }

    procarg register deletechar {
      {-id int -default -1 -restrict { {-1 +} }}
    }

    procarg register deletestring {
      {-id int -default -1 -restrict { {-1 +} }}
    }

    procarg register expandtabs {
      {-id   int -default -1 -restrict { {-1 +} }}
      {-line int -default -1 -restrict { {-1 +} }}
    }

    procarg register getbookmarks {
      {-id int -default -1 -restrict { {-1 +} }}
    }

    procarg register getfilename {
      {-id int -default -1 -restrict { {-1 +} }}
    }

    procarg register getinfo {
      {-id int -default -1 -restrict { {-1 +} }}
    }

    procarg register getstring {
      {-id   int -default -1 -restrict { {-1 +} }}
      {-line int -default -1 -restrict { {-1 +} }}
    }

    procarg register insertstring {
      {-id     int     -default -1 -restrict { {-1 +} }}
      {-indent boolean -default true}
    }

    procarg register inserttext {
      {0   string -allowempty false}
      {-id int    -default -1 -restrict { {-1 +} }}
    }

    procarg register quit {
      {-id int -default -1 -restrict { {-1 +} }}
    }

    procarg register realtotab {
      {-spos int -default 0  -restrict { {-1 +} }}
      {-id   int -default -1 -restrict { {-1 +} }}
      {-line int -default -1 -restrict { {-1 +} }}
    }

    procarg register tabtoreal {
      {-spos int -default 0  -restrict { {-1 +} }}
      {-id   int -default -1 -restrict { {-1 +} }}
      {-line int -default -1 -restrict { {-1 +} }}
    }

    procarg register savefile {
      {-id       int    -default -1 -restrict { {-1 +} }}
      {-filename string}
      {-eol      string -restrict {"" crlf lf}}
      {-codepage int    -restrict { {0 +} }}
    }

    procarg register setposition {
      {-id            int -default -1 -restrict { {-1 +} }}
      {-line          int -default -1 -restrict { {-1 +} }}
      {-pos           int -default -1 -restrict { {-1 +} }}
      {-tabpos        int -default -1 -restrict { {-1 +} }}
      {-topscreenline int -default -1 -restrict { {-1 +} }}
      {-leftpos       int -default -1 -restrict { {-1 +} }}
      {-overtype      int -default -1 -restrict { {-1 +} }}
    }

    procarg register setstring {
      {0         string -allowempty false}
      {-id       int    -default -1 -restrict { {-1 +} }}
      {-line     int    -default -1 -restrict { {-1 +} }}
      {-eol      string -restrict {"" crlf lf}}
      {-encoding string -default "default"}
    }

    procarg register settitle {
      {0   string}
      {-id int -default -1 -restrict { {-1 +} }}
    }

    procarg register undoredo {
      {0   string -restrict {begin end undo redo}}
      {-id int    -default -1 -restrict { {-1 +} }}
    }

    procarg register readinput {
      {-id int -default -1 -restrict { {-1 +} }}
    }

    procarg register processinput {
      {-id   int    -default -1 -restrict { {-1 +} }}
      {-dump string -nodefault -allowempty false}
    }

    procarg register setparam {
      {-id               int     -default -1 -restrict { {-1 +} }}
      {-autoindent       boolean -nodefault}
      {-charcodebase     string  -nodefault -restrict {oct hex dec}}
      {-codepage         int     -nodefault -restrict { {0 +} }}
      {-cursorbeyoneol   boolean -nodefault}
      {-expandtabs       string  -nodefault -restrict {no all new}}
      {-lockmode         boolean -nodefault}
      {-savefileposition boolean -nodefault}
      {-showwhitespace   string  -nodefault -restrict {no space spaceline}}
      {-setbom           boolean -nodefault}
      {-setworddiv       string  -nodefault}
      {-getworddiv       switch  -nodefault}
      {-tabsize          int     -nodefault -restrict { {1 512} }}
    }

    namespace eval sessionbookmark {
      namespace export *
      namespace ensemble create

      procarg register add {
        {-id int -default -1 -restrict { {-1 +} }}
      }
      procarg register clear {
        {-id int -default -1 -restrict { {-1 +} }}
      }
      procarg register get {
        {-id int -default -1 -restrict { {-1 +} }}
      }
      procarg register delete {
        {-id    int -default -1 -restrict { {-1 +} }}
        {-index int -default -1 -restrict { {-1 +} }}
      }
      procarg register next {
        {-id int -default -1 -restrict { {-1 +} }}
      }
      procarg register prev {
        {-id int -default -1 -restrict { {-1 +} }}
      }
    }
  }

  procarg register FarColor {
    {0          int -restrict { { 0 + } }}
    {1          int -restrict { { 0 + } }}
    {-fg4bit    switch}
    {-bg4bit    switch}
    {-bold      switch}
    {-italic    switch}
    {-underline switch}
  }

  variable codepage2encoding [list 65001 utf-8 \
    855 cp855 866 cp866 \
    20866 koi8-r \
    28592 iso8859-2 28595 iso8859-5 28597 iso8859-7 28598 iso8859-8 \
    1250 cp1250 1251 cp1251 1252 cp1252 1253 cp1253 1255 cp1255 \
  ]
  variable encoding2codepage [lreverse $codepage2encoding]

  variable VirtualKeyCodes [list {} lbutton rbutton cancel mbutton xbutton1 xbutton2 {} \
                                 back tab {} {} clear return {} {} \
                                 shift control menu pause capital kana {} junja \
                                 final hanja {} escape {} nonconvert accept modechange \
                                 space prior next end home left up right \
                                 down select print execute snapshot insert delete help \
                                 0 1 2 3 4 5 6 7 \
                                 8 9 {} {} {} {} {} {} \
                                 {} a b c d e f g \
                                 h i j k l m n o \
                                 p q r s t u v w \
                                 x y z lwin rwin apps {} sleep \
                                 numpad0 numpad1 numpad2 numpad3 numpad4 numpad5 numpad6 numpad7 \
                                 numpad8 numpad9 multiply add separator subtract decimal divide \
                                 f1 f2 f3 f4 f5 f6 f7 f8 \
                                 f9 f10 f11 f12 f13 f14 f15 f16 \
                                 f17 f18 f19 f20 f21 f22 f23 f24 \
                                 {} {} {} {} {} {} {} {} \
                                 numlock scroll oem_nec_equal oem_fj_masshou oem_fj_touroku oem_fj_loya oem_fj_roya {} \
                                 {} {} {} {} {} {} {} {} \
                                 lshift rshift lcontrol rcontrol lmenu rmenu browser_back browser_forward \
                                 browser_refresh browser_stop browser_search browser_favorites browser_home volume_mute volume_down volume_up \
                                 media_next_track media_prev_track media_stop media_play_pause launch_mail launch_media_select launch_app1 launch_app2 \
                                 {} {} oem_1 oem_plus oem_comma oem_minus oem_period oem_2 \
                                 oem_3 {} {} {} {} {} {} {} \
                                 {} {} {} {} {} {} {} {} \
                                 {} {} {} {} {} {} {} {} \
                                 {} {} {} oem_4 oem_5 oem_6 oem_7 oem_8 \
                                 {} {} oem_102 {} {} processkey {} packet \
                                 {} oem_reset oem_jump oem_pa1 oem_pa2 oem_pa3 oem_wsctrl {} \
                                 {} {} {} {} {} {} attn crsel \
                                 exsel ereof play zoom noname pa1 oem_clear {}]

}

proc ::Far::Encoding2Codepage { encoding } {
    variable encoding2codepage
    if { [catch { dict get $encoding2codepage $encoding } result] } {
        return -code error "Error: encoding '$encoding' is not supported"
    }
    return $result
}

proc ::Far::Codepage2Encoding { codepage } {
    variable codepage2encoding
    if { [catch { dict get $codepage2encoding $codepage } result] } {
        return -code error "Error: codepage '$codepage' is not supported"
    }
    return $result
}

proc ::Far::Message { msg args } {
  procarg parse

  if { $opts(-pluginid) eq "" } {
    set opts(-pluginid) $::Far::MainGUID
  }

  if { $opts(-allinone) } {
    set msg "$opts(-title)\n$msg"
  } {
    set opts(-allinone) true
    set msg "$opts(-title)\n[join $msg \n]"
  }

  if { $opts(-stdbutt) eq "" } {
    set butcount [llength $opts(-buttons)]
    if { !$butcount } {
      set opts(-stdbutt) ok
    } {
      append msg \n[join $opts(-buttons) \n]
    }
  } {
    set butcount 0
  }

  set flags 0
  foreach {k v} [list -warning FMSG_WARNING -leftalign FMSG_LEFTALIGN \
                      -allinone FMSG_ALLINONE] {
    if { $opts($k) } { set flags [expr { $flags | [set ::Far::$v] }] }
  }

  foreach {k v} [list ok FMSG_MB_OK okcancel FMSG_MB_OKCANCEL \
                      abortretryignore FMSG_MB_ABORTRETRYIGNORE \
                      yesno FMSG_MB_YESNO yesnocancel FMSG_MB_YESNOCANCEL \
                      retrycancel FMSG_MB_RETRYCANCEL] {
    if { $opts(-stdbutt) eq $k } { set flags [expr { $flags | [set ::Far::$v] }]; break }
  }

  return [::Far::core::Message $opts(-pluginid) $opts(-id) $flags $opts(-helptopic) $msg $butcount]
}

proc ::Far::GetMsg { msgid args } {
  procarg parse

  if { $opts(-pluginid) eq "" } {
    set opts(-pluginid) $::Far::MainGUID
  }

  return [::Far::core::GetMsg $opts(-pluginid) $msgid]
}

proc ::Far::Menu { menuitems args } {
  procarg parse

  if { $opts(-pluginid) eq "" } {
    set opts(-pluginid) $::Far::MainGUID
  }

  set flags 0
  foreach {k v} [list -autohl FMENU_AUTOHIGHLIGHT -changeconsoletitle FMENU_CHANGECONSOLETITLE \
                      -showamp FMENU_SHOWAMPERSAND -reverseautohl FMENU_REVERSEAUTOHIGHLIGHT \
                      -wrap FMENU_WRAPMODE] {
    if { $opts($k) } { set flags [expr { $flags | [set ::Far::$v] }] }
  }

  array set menuitem_param [list]
  for { set i 0 } { $i < [llength $menuitems] } { incr i } {
    set menuitem_param($i,flags) 0
    if { $i == $opts(-select) } {
      set menuitem_param($i,flags) [expr { $menuitem_param($i,flags) | [set ::Far::MIF_SELECTED]}]
    }
    foreach {k v} [list -checked MIF_CHECKED -separator MIF_SEPARATOR -disabled MIF_DISABLE \
                        -grayed MIF_GRAYED -hidden MIF_HIDDEN] {
      if { [lsearch -exact $opts($k) $i] != -1 } {
        set menuitem_param($i,flags) [expr { $menuitem_param($i,flags) | [set ::Far::$v]}]
      }
    }
  }

  for { set i 0 } { $i < [llength $menuitems] } { incr i } {
    lappend menuitems_flags $menuitem_param($i,flags)
  }

  return [::Far::core::Menu $opts(-pluginid) $opts(-id) $opts(-x) $opts(-y) \
                             $opts(-maxheight) $flags $opts(-title) $opts(-bottom) $opts(-helptopic) \
                             $menuitems $menuitems_flags]
}

proc ::Far::InputBox { args } {
  procarg parse

  if { $opts(-pluginid) eq "" } {
    set opts(-pluginid) $::Far::MainGUID
  }

  set flags 0
  foreach {k v} [list -enableempty FIB_ENABLEEMPTY -password FIB_PASSWORD -expandenv FIB_EXPANDENV \
                      -nouselasthistory FIB_NOUSELASTHISTORY -buttons FIB_BUTTONS -noampersand FIB_NOAMPERSAND \
                      -editpath FIB_EDITPATH -editpathexec FIB_EDITPATHEXEC] {
    if { $opts($k) } { set flags [expr { $flags | [set ::Far::$v] }] }
  }

  return [::Far::core::InputBox $opts(-pluginid) $opts(-id) $opts(-title) $opts(-subtitle) $opts(-historyname) \
                                $opts(-srctext) $opts(-destsize) $opts(-helptopic) $flags]
}


proc ::Far::PluginControl::load { modulename args } {
  procarg parse

  return [::Far::core::LoadPlugin $modulename $opts(-forceload)]
}

proc ::Far::PluginControl::findbyid { id } {
  procarg parse
  return [::Far::core::FindPlugin $::Far::PFM_GUID $id]
}

proc ::Far::PluginControl::findbymodulename { modulename } {
  procarg parse
  return [::Far::core::FindPlugin $::Far::PFM_MODULENAME $modulename]
}

proc ::Far::PluginControl::unload { handle } {
  procarg parse
  return [::Far::core::UnloadPlugin $handle]
}

proc ::Far::PluginControl::getinfo { handle } {
  procarg parse
  return [::Far::core::GetPluginInfo $handle]
}

proc ::Far::PluginControl::get { } {
  return [::Far::core::GetPlugins]
}

proc ::Far::EditorControl::addcolor { spos epos color args } {
  procarg parse

  if { $opts(-owner) eq "" } { set opts(-owner) $::Far::MainGUID }

  set flags 0
  foreach {k v} [list -tabmarkfirst ECF_TABMARKFIRST -tabmarkcurrent ECF_TABMARKCURRENT] {
    if { $opts($k) } { set flags [expr { $flags | [set ::Far::$v] }] }
  }

  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_ADDCOLOR $opts(-line) $spos $epos \
    $opts(-priority) $flags [lindex $color 0] [lindex $color 1] [lindex $color 2] $opts(-owner)]
}

proc ::Far::EditorControl::getcolor { args } {
  procarg parse

  set info [::Far::core::EditorControl $opts(-id) $::Far::ECTL_GETCOLOR $opts(-line) $opts(-item)]

  return $info
}

proc ::Far::EditorControl::select { blocktype args } {
  procarg parse

  switch -- $blocktype {
    stream  { set blocktype $::Far::BTYPE_STREAM }
    column  { set blocktype $::Far::BTYPE_COLUMN }
    default { set blocktype $::Far::BTYPE_NONE   }
  }

  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_SELECT $blocktype $opts(-line) \
    $opts(-pos) $opts(-width) $opts(-height)]
}

proc ::Far::EditorControl::delcolor { args } {
  procarg parse

  if { $opts(-owner) eq "" } { set opts(-owner) $::Far::MainGUID }

  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_DELCOLOR $opts(-owner) $opts(-line) $opts(-pos)]
}

proc ::Far::EditorControl::redraw { args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_REDRAW]
}

proc ::Far::EditorControl::deleteblock { args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_DELETEBLOCK]
}

proc ::Far::EditorControl::deletechar { args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_DELETECHAR]
}

proc ::Far::EditorControl::deletestring { args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_DELETECHAR]
}

proc ::Far::EditorControl::expandtabs { args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_EXPANDTABS $opts(-line)]
}

proc ::Far::EditorControl::getbookmarks { args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_GETBOOKMARKS]
}

proc ::Far::EditorControl::getfilename { args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_GETFILENAME]
}

proc ::Far::EditorControl::getstring { args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_GETSTRING $opts(-line)]
}

proc ::Far::EditorControl::getinfo { args } {
  procarg parse
  set info [::Far::core::EditorControl $opts(-id) $::Far::ECTL_GETINFO]

  set curstate [dict get $info curstate]
  dict set info curstate [dict create]
  foreach {k v} [list ECSTATE_MODIFIED modified ECSTATE_SAVED saved ECSTATE_LOCKED locked] {
    dict set info curstate $v [expr { !!($curstate & [set ::Far::$k]) }]
  }

  switch -- [dict get $info blocktype] {
    $::Far::BTYPE_STREAM  { dict set info blocktype "stream" }
    $::Far::BTYPE_COLUMN  { dict set info blocktype "column" }
    default  { dict set info blocktype "none" }
  }

  set options [dict get $info options]
  dict set info options [dict create]
  foreach {k v} [list EOPT_EXPANDALLTABS expandalltabs EOPT_EXPANDONLYNEWTABS expandonlynewtabs \
                      EOPT_PERSISTENTBLOCKS persistentblocks EOPT_DELREMOVESBLOCKS delremoveblocks \
                      EOPT_AUTOINDENT autoindent EOPT_AUTODETECTCODEPAGE autodetectcodepage \
                      EOPT_CURSORBEYONDEOL cursorbeyondeol EOPT_SAVEFILEPOSITION savefileposition \
                      EOPT_SHOWWHITESPACE showwhitespace EOPT_BOM bom EOPT_SHOWLINEBREAK showlinebreak] {
    dict set info options $v [expr { !!($options & [set ::Far::$k]) }]
  }

  return $info
}

proc ::Far::EditorControl::insertstring { args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_INSERTSTRING $opts(-indent)]
}

proc ::Far::EditorControl::inserttext { string args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_INSERTTEXT $string]
}

proc ::Far::EditorControl::quit { args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_QUIT]
}

proc ::Far::EditorControl::realtotab { args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_REALTOTAB $opts(-line) $opts(-spos)]
}

proc ::Far::EditorControl::tabtoreal { args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_TABTOREAL $opts(-line) $opts(-spos)]
}

proc ::Far::EditorControl::savefile { args } {
  procarg parse
  switch -- $opts(-eol) {
    crlf    { set opts(-eol) "\r\n" }
    lf      { set opts(-eol) "\n"   }
    default { set opts(-eol) ""     }
  }
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_SAVEFILE $opts(-filename) $opts(-eol) $opts(-codepage)]
}

proc ::Far::EditorControl::setstring { string args } {
  procarg parse
  switch -- $opts(-eol) {
    crlf    { set opts(-eol) "\r\n" }
    lf      { set opts(-eol) "\n"   }
    default { set opts(-eol) ""     }
  }
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_SETSTRING $opts(-line) $string $opts(-eol)]
}

proc ::Far::EditorControl::setposition { args } {
  procarg parse

  if { $opts(-pos) != -1 } { set opts(-tabpos) -1 }

  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_SETPOSITION $opts(-line) $opts(-pos) \
    $opts(-tabpos) $opts(-topscreenline) $opts(-leftpos) $opts(-overtype)]
}

proc ::Far::EditorControl::settitle { title args } {
  procarg parse

  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_SETTITLE $title]
}

proc ::Far::EditorControl::readinput { args } {
  procarg parse

  set info [::Far::core::EditorControl $opts(-id) $::Far::ECTL_READINPUT]

  switch -- [dict get $info eventtype] {
    "key" {
      dict set info key [lindex $::Far::VirtualKeyCodes [dict get $info keycode]]
    }
  }

  return $info
}

proc ::Far::EditorControl::processinput { args } {
  procarg parse

  if { [info exists opts(-dump)] } {
    return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_PROCESSINPUT 0 $opts(-dump)]
  }
}

proc ::Far::EditorControl::undoredo { command args } {
  procarg parse

  switch -- $command {
    "begin" { set command $::Far::EUR_BEGIN }
    "end"   { set command $::Far::EUR_END   }
    "undo"  { set command $::Far::EUR_UNDO  }
    "redo"  { set command $::Far::EUR_REDO  }
  }

  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_UNDOREDO $command]
}

proc ::Far::EditorControl::setparam { args } {
  procarg parse

  set result 0
  foreach {k v} [array get opts] {
    switch -- $k {
      "-autoindent" {
        set result [::Far::core::EditorControl $opts(-id) $::Far::ECTL_SETPARAM $::Far::ESPT_AUTOINDENT $v]
      }
      "-charcodebase" {
        switch -- $v {
          "oct" { set v 0 }
          "hex" { set v 2 }
          "dec" { set v 1 }
        }
        set result [::Far::core::EditorControl $opts(-id) $::Far::ECTL_SETPARAM $::Far::ESPT_CHARCODEBASE $v]
      }
      "-codepage" {
        set result [::Far::core::EditorControl $opts(-id) $::Far::ECTL_SETPARAM $::Far::ESPT_CODEPAGE $v]
      }
      "-expandtabs" {
        switch -- $v {
          "no"   { set v $::Far::EXPAND_NOTABS }
          "all" { set v $::Far::EXPAND_ALLTABS }
          "new" { set v $::Far::EXPAND_NEWTABS }
        }
        set result [::Far::core::EditorControl $opts(-id) $::Far::ECTL_SETPARAM $::Far::ESPT_EXPANDTABS $v]
      }
      "-cursorbeyoneol" {
        set result [::Far::core::EditorControl $opts(-id) $::Far::ECTL_SETPARAM $::Far::ESPT_CURSORBEYONDEOL $v]
      }
      "-lockmode" {
        set result [::Far::core::EditorControl $opts(-id) $::Far::ECTL_SETPARAM $::Far::ESPT_LOCKMODE $v]
      }
      "-savefileposition" {
        set result [::Far::core::EditorControl $opts(-id) $::Far::ECTL_SETPARAM $::Far::ESPT_SAVEFILEPOSITION $v]
      }
      "-showwhitespace" {
        switch -- $v {
          "no"   { set v 0 }
          "space" { set v 1 }
          "spaceline" { set v 2 }
        }
        set result [::Far::core::EditorControl $opts(-id) $::Far::ECTL_SETPARAM $::Far::ESPT_SHOWWHITESPACE $v]
      }
      "-setbom" {
        set result [::Far::core::EditorControl $opts(-id) $::Far::ECTL_SETPARAM $::Far::ESPT_SETBOM $v]
      }
      "-setworddiv" {
        if { [string length $v] > 255 } { set v [string range $v 0 254] }
        set result [::Far::core::EditorControl $opts(-id) $::Far::ECTL_SETPARAM $::Far::ESPT_SETWORDDIV $v]
      }
      "-tabsize" {
        set result [::Far::core::EditorControl $opts(-id) $::Far::ECTL_SETPARAM $::Far::ESPT_TABSIZE $v]
      }
    }
  }

  if { [info exists opts(-getworddiv)] } {
    return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_SETPARAM $::Far::ESPT_GETWORDDIV]
  }

  return $result
}

proc ::Far::EditorControl::sessionbookmark::add { args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_ADDSESSIONBOOKMARK]
}

proc ::Far::EditorControl::sessionbookmark::clear { args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_CLEARSESSIONBOOKMARKS]
}

proc ::Far::EditorControl::sessionbookmark::delete { args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_DELETESESSIONBOOKMARK $opts(-index)]
}

proc ::Far::EditorControl::sessionbookmark::get { args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_GETSESSIONBOOKMARKS]
}

proc ::Far::EditorControl::sessionbookmark::next { args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_NEXTSESSIONBOOKMARK]
}

proc ::Far::EditorControl::sessionbookmark::prev { args } {
  procarg parse
  return [::Far::core::EditorControl $opts(-id) $::Far::ECTL_PREVSESSIONBOOKMARK]
}

proc ::Far::FarColor { fg bg args } {
  procarg parse

  set flags 0
  foreach {k v} [list -fg4bit FCF_FG_4BIT -bg4bit FCF_BG_4BIT -bold FCF_FG_BOLD -italic FCF_FG_ITALIC \
                      -underline FCF_FG_UNDERLINE] {
    if { $opts($k) } { set flags [expr { $flags | [set ::Far::$v] }] }
  }
  return [list $flags $fg $bg]
}

