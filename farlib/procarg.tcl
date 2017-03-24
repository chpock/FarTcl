
package provide procarg 0.0.1

namespace eval procarg {
  variable box
  variable paramtypes {string boolean integer double switch list}
  variable regtypes
  array set regtypes [list]

  namespace export *
  namespace ensemble create
  set box [dict create ::procarg::registerkey [list \
      -default [list string "" "" ignore false] \
      -nodefault [list switch false "" ignore false] \
      -restrict [list string "" "" ignore false] \
      -allowempty [list boolean ignore "" ignore false] \
      __cache [list -default "" -restrict "" -allowempty ignore -nodefault false]]]
}

proc procarg::regtype { type args } {
  variable regtypes
  variable paramtypes

  parse

  if { $opts(-expression) ne "" && [catch {set val 0; if $opts(-expression) {}} msg] } {
    return -code error "error while eval expression for custom argument type \"$type\": $msg"
  }
  if { [lsearch -exact $paramtypes $type] < 0 } { lappend paramtypes $type }
  if { $opts(-errormsg) eq "" } { set opts(-errormsg) "is not correct ${type}." }
  set regtypes($type) [list $opts(-expression) $opts(-errormsg)]
}

proc procarg::registerkey { func key type args } {
  variable box
  variable paramtypes

  parse

  if { [set idx [lsearch -glob $paramtypes ${type}*]] < 0 } {
    return -code error "${func}: unknown type $type while registering arguments.\nAllowed types: [string trim $paramtypes]"
  }
  set type [lindex $paramtypes $idx]
  if { $type eq "switch" } {
    set opts(-default) false
  }
  # check restrict rules
  if { $opts(-restrict) ne "" && [lindex $opts(-restrict) 0] ne "script" } {
    switch -- $type {
      integer -
      double {
        foreach el $opts(-restrict) {
          if { [llength $el] == 1 } {
            if { ![string is $type -strict $el] } {
              return -code error "${func}: error in declaration restrict of \"$key\", \"$el\" is not ${type}."
            }
          } elseif { [llength $el] == 2 } {
            if { ([set l [lindex $el 0]] ne "-" && ![string is $type -strict $l]) \
                  || ([set h [lindex $el 1]] ne "+" && ![string is $type -strict $h]) } {
              return -code error "${func}: error in declaration restrict of \"$key\" range \"$el\"."
            }
          } else {
            return -code error "${func}: error in declaration restrict of \"$key\" list size more then 2 \"$el\"."
          }
        }
      }
      list {
        lassign $opts(-restrict) l h
          if { $l eq "" || $l eq "-" } {
            set l 0
          } elseif { ![string is integer -strict $l] } {
            return -code error "${func}: error in declaration restrict of \"$key\", \"$l\" lower list size is not integer."
          }
          if { $h eq "" || $h eq "+" } {
            set h "+"
          } elseif { ![string is integer -strict $h] } {
            return -code error "${func}: error in declaration restrict of \"$key\", \"$l\" upper list size is not integer."
          }
          set opts(-restrict) [list $l $h]
      }
    }
  }
  # check default value (ignore proc arguments)
  if { ![string match {[0-9]} $key] && [catch { checkvalue $key $opts(-default) $type $opts(-restrict) true } msg] } {
    return -code error "${func}: error in declaration of default arguments.\n$msg"
  }
  dict set box $func $key [list $type $opts(-default) $opts(-restrict) $opts(-allowempty) $opts(-nodefault)]
}

proc procarg::register { func params } {
  variable box
  variable paramtypes

  if { [string range $func 0 1] ne "::" } {
    if { [set ns [uplevel 1 [list namespace current]]] eq "::" } {
      set func ::$func
    } {
      set func ${ns}::$func
    }
  }

  foreach param $params {
    if { [llength $param] < 2 } {
      return -code error "${func}: error in declaration arguments \"argument type ?options?\": <${param}>"
    }
    if { [catch {registerkey {*}[concat [list $func] $param]} msg] } {
      return -code error $msg
    }
  }

  set cache [list]
  dict for { key val } [dict get $box $func] {
    if { [string match {[0-9]} $key] || $key eq "__cache" } continue
    if { [lindex $val 4] } continue
    lappend cache $key [lindex $val 1]
  }
  dict set box $func __cache $cache

  return ""
}

proc procarg::parse { } {
  variable box

  upvar args a opts o

  if { [string range [set func [lindex [info level -1] 0]] 0 1] ne "::" } {
    if { [set ns [uplevel 1 [list namespace current]]] eq "::" } {
      set func ::$func
    } {
      set func ${ns}::$func
    }
  }

  if { ![dict exists $box $func] } { return -code error "arguments for $func not registered yet."  }

  array set o [dict get $box $func __cache]

  if { [info exists a] } {
    for { set idx 0 } { $idx < [llength $a] } { incr idx } {
      set key [lindex $a $idx]
      if { ![dict exists $box $func $key] } { return -code error "${func}: unknown option $key, must be one of: [dict keys [dict get $box $func] -*]" }
      lassign [dict get $box $func $key] type default restrict allowempty
      if { $type eq "switch" } { set val true } { set val [lindex $a [incr idx]] }
      if { [catch {checkvalue $key $val $type $restrict $allowempty} msg] } {
        return -code error "${func}: error while parse arguments\n$msg"
      }
      set o($key) $val
    }
  }

  foreach idx [dict keys [dict get $box $func] {[0-9]}] {
    if { [set key [lindex [info args $func] $idx]] eq "" } {
      return -code error "${func}: not found defined proc argument #${idx}."
    }
    set val [uplevel 1 [list set $key]]
    lassign [dict get $box $func $idx] type default restrict allowempty
    if { [catch {checkvalue $key $val $type $restrict $allowempty} msg] } {
      return -code error "${func}: error while parse arguments\n$msg"
    }
  }

  return ""
}

proc procarg::checkvalue { key val type restrict allowempty } {
  variable regtypes
  # check for empty
  if { $allowempty ne "ignore" } {
    if { $allowempty && $val eq "" } { return $val }
    if { !$allowempty && $val eq "" } {
      return -code error "$key not allowed to be empty."
    }
  }
  # simple type check
  switch -- $type {
    boolean -
    integer -
    double {
      if { ![string is $type -strict $val] } {
        return -code error "$key \"$val\" is not $type value."
      }
    }
    list {
      if { [catch {llength $val} msg] } {
        return -code error "$key \"$val\" bad list - $msg"
      }
    }
  }
  # simple custom type check
  if { [info exists regtypes($type)] && [lindex $regtypes($type) 0] ne "" } {
    if { [catch {if [lindex $regtypes($type) 0] { set _ 0 } { set _ 1 } } msg] } {
      return -code error "$key \"$val\" type check expression failed: $msg"
    } elseif { $msg } {
      return -code error "$key \"$val\" [lindex $regtypes($type) 1]"
    }
  }
  # check for restrictions
  if { $restrict eq "" } { return $val }
  if { [lindex $restrict 0] eq "script" } {
    if { [set expression [lindex $restrict 1]] ne "" } {
      if { [catch {if $expression { set _ 0 } { set _ 1 } } msg] } {
        return -code error "$key \"$val\" restrict expression failed: $msg"
      } elseif { $msg } {
        if { [set errstr [lindex $restrict 2]] eq "" } {
          set errstr "is not allowed."
        }
        return -code error "$key \"$val\" $errstr"
      }
    }
  } {
    switch -- $type {
      string {
        if { [lsearch -exact $restrict $val] < 0 } {
          return -code error "$key \"$val\" is not in allowed values: [join [split $restrict] {, }]"
        }
      }
      double -
      integer {
        foreach el $restrict {
          if { [llength $el] == 1 } {
            if { $val == $el } { return $val }
          } elseif { [llength $el] == 2 } {
            if { [set l [lindex $el 0]] eq "-" } {
              if { $val <= [lindex $el 1] } { return $val }
            } elseif { [set h [lindex $el 1]] eq "+" } {
              if { $val >= $l } { return $val }
            } elseif { $val >= $l && $val <= $h } {
              return $val
            }
          }
        }
        return -code error "$key \"$val\" is not covered by allowed ranges: [join $restrict {, }]"
      }
      list {
        lassign $restrict l h
        if { [llength $val] < $l || ($h ne "+" && [llength $val] > $h) } {
          return -code error "$key \"$val\" is not covered by allowed range of list size: ${l}..${h}"
        }
      }
    }
  }

  return $val
}

procarg::register ::procarg::regtype {
  {-expression string}
  {-errormsg string}
}