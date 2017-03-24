set f [open version.hpp]; set a [read $f]; close $f
regexp {VER_MAJOR\s+(\d+).+VER_MINOR\s+(\d+).+VER_REVIS\s+(\d+)} $a -> ma mi re
puts [join "$ma $mi $re" .]