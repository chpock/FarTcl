set fd [open [lindex $argv 0] r]; set buf [read $fd]; close $fd
regsub -all -- {/\*.*?\*/} $buf {} buf
regsub -all -line {\s*#.*} $buf {} buf
regsub -all -line {\s*//.*} $buf {} buf
puts "namespace eval ::Far {"
foreach {-> match} [regexp -all -inline {static const [\w\d]+\s+(.+?);} $buf] {
  foreach {-> key value} [regexp -all -inline {\s*([\w\d]+)\s*=\s*([\w\d]+)\s*,?} $match] {
    if { [string range $value end-2 end] eq "ULL" } { set value [string range $value 0 end-3] }
    if { [string range $value end-1 end] eq "LL" } { set value [string range $value 0 end-2] }
    puts "  variable $key $value"
  }
}
puts "}\npackage provide farlib::flags 0.0.1"
