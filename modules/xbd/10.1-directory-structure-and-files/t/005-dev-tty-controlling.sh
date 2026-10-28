# /dev/tty is the controlling terminal of the process: output written to it
# goes to the terminal even when standard output and error are redirected,
# and input read from it is what is typed there.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.10.1-directory-structure-and-files.005
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XBD.10.1-directory-structure-and-files.005
pc_pty_need
cat >inner.sh <<'IN'
exec >out.txt 2>err.txt </dev/null
printf 'MARKER-OUT-7431\n' >/dev/tty
echo "write $?" >rc.txt
IFS= read -r line </dev/tty
echo "read $?" >>rc.txt
printf '%s\n' "$line" >typed.txt
IN
printf 'typed-text-5527\n' >input.txt
pc_test
pc_pty_run -i input.txt inner.sh
grep 'MARKER-OUT-7431' pty.out >/dev/null 2>&1 || pc_fail "text written to /dev/tty did not reach the terminal"
grep 'MARKER-OUT-7431' out.txt err.txt >/dev/null 2>&1 && pc_fail "text written to /dev/tty went to the redirected output"
grep '^write 0$' rc.txt >/dev/null 2>&1 || pc_fail "writing to /dev/tty failed"
grep '^read 0$' rc.txt >/dev/null 2>&1 || pc_fail "reading from /dev/tty failed"
[ "$(cat typed.txt 2>/dev/null)" = "typed-text-5527" ] || pc_fail "reading /dev/tty gave '$(cat typed.txt 2>/dev/null)', not the text typed at the terminal"
pc_pass
