tcl 学习


//
package require struct::matrix
package require csv

//csv 操作
::struct::matrix xdata
set chan [open $OPTS(csv_input_filename)]
csv::read2matrix $chan xdata
close $chan
csv::joinlist [xdata get rect 0 0 end end]



puts stderr [xdata rows]


set curTime [clock seconds]
set curTimeExpiredStr [clock format $curTime -format %Y-%m-%d]
