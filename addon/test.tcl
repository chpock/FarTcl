::Far::Message "test it\nonemore\nksjd" -allinone -title "My Title" -leftalign -warning
      asd
package require farlib
::Far::Message [list "test it" "onemore" "ksjd"] -title "My Title" -leftalign -warning -buttons {bla alb}
::Far::GetMsg 3 -pluginid B076F0B0-90AE-408c-AD09-491606F09435
::Far::GetMsg 3 -pluginid sdlfkjsldkjf
::Far::Menu [list a b "" &c d] -select 2 -checked {1 4} -separator 2
::Far::InputBox -srctext "тест тест" -title "Тест!!!!" -subtitle "Enter Value:" -destsize 15
set a [::Far::PluginControl get]
::Far::PluginControl getinfo [lindex $a 15]
set a [::Far::PluginControl findbyid bff142a4-f392-4e7d-84aa-a00298caf9ef]
::Far::PluginControl findbymodulename {C:\Program Files\Far Manager\Plugins\FarTcl\FarTcl.dll}
::Far::PluginControl getinfo $a
::Far::PluginControl load {C:\Program Files\Far Manager\Plugins\FarTcl\FarTcl.dll}
::Far::PluginControl unload $a
llength [set pl [::Far::PluginControl get]]
dict get [::Far::PluginControl getinfo [lindex $pl 15]]
set p [::Far::PluginControl findbyid -id bff142a4-f392-4e7d-84aa-a00298caf9e1]
::Far::FarColor 100 200
::Far::EditorControl addcolor 1 10 [::Far::FarColor 100 200 -underline] -line 3 -priority 10
::Far::EditorControl delcolor -line 3
::Far::EditorControl select stream -line 1 -height 2 -pos 10
::Far::EditorControl deleteblock
? ::Far::EditorControl deletechar
? ::Far::EditorControl deletestring
::Far::EditorControl expandtabs -line 2
::Far::EditorControl getbookmarks
::Far::EditorControl getfilename
::Far::EditorControl getinfo
::Far::EditorControl getstring -line 5
::Far::EditorControl inserttext "лдоывда офдыл длфоыв длаооо"
::Far::EditorControl realtotab 4 -line 1
::Far::EditorControl savefile -filename testname -eol crlf -codepage 1251
::Far::EditorControl setposition -line 10 -pos 10
::Far::EditorControl setstring "блабла блабла"
::Far::EditorControl settitle "тест титул"
::Far::EditorControl undoredo undo
::Far::EditorControl setparam -setworddiv "test"
::Far::EditorControl setparam -getworddiv
::Far::EditorControl setparam -setworddiv ""
::Far::EditorControl setparam -tabsize 2
::Far::EditorControl sessionbookmark get
::Far::EditorControl sessionbookmark add
::Far::EditorControl sessionbookmark delete
::Far::EditorControl sessionbookmark clear
::Far::EditorControl sessionbookmark prev
::Far::EditorControl quit
::Far::EditorControl getcolor -line 3 -item 1
while 1 {
  set info [::Far::EditorControl readinput]
  ::Far::EditorControl settitle $info
  ::Far::EditorControl redraw
  if { [dict get $info eventtype] eq "key" && [dict get $info keycode] == 27 } {
    ::Far::EditorControl settitle ""
    break
  }
  ::Far::EditorControl processinput -dump [dict get $info dump]
}
