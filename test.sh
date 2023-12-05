#!/bin/bash

. script/assert.sh

BIN='bin/ipcalc'
LOG='valgrind.log'

default(){
	valgrind --leak-check=full --log-file="$LOG" ./"$BIN" > /dev/null
	grep --count 'All heap blocks were freed -- no leaks are possible' "$LOG"
}

assert_raises "./$BIN" 0
assert_raises "./$BIN-c '1.1.1.1'" 0
assert_raises "./$BIN -c '999' -f ipv4" 0

assert default '1'

assert_end