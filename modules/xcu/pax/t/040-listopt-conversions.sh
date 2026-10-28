# pax -o listopt: the conversions of the list format
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.040
# With -v and -o listopt in list mode: %M gives the ls mode string (%.1M the
# type character), %F the path, %L "name -> target" for a symbolic link and
# the path otherwise, %D with no device is a blank and %(size)D equals
# %(size)u, %T writes the modification time as "%b %e %H:%M %Y" and
# %(mtime=%Y-%m-%d)T with the given format, \t is a tab, a newline is added after each entry and several listopt
# options are joined; TZ decides the zone of the times.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.040
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'umask 022; cd "$pc_top" && rm -rf "$pc_wd"' EXIT
umask 022
printf '12345\n' >f1 || pc_unresolved "setup: cannot write f1"
chmod 640 f1 && touch -d 2003-01-02T15:04:05Z f1 || pc_unresolved "setup: chmod or touch failed"
ln -s f1 s || pc_unresolved "setup: ln -s failed"
mkdir d && chmod 755 d || pc_unresolved "setup: mkdir failed"
pax -w -x ustar f1 s d >a 2>/dev/null </dev/null || pc_unresolved "setup: pax -w failed"
# lo <member> <listopt format>: the output for one member
lo() { m=$1; shift; TZ=UTC0 pax -v -o "listopt=$1" "$m" <a 2>err; }
# expect <member> <format> <expected> <message>
expect() { got=$(lo "$1" "$2"); [ "$got" = "$3" ] || pc_fail "$4: '$got', expected '$3'"; }
pc_test
got=$(lo f1 '%M' | cut -c1-10)
[ "$got" = '-rw-r-----' ] || pc_fail "%M for f1 gives '$got', expected -rw-r-----"
got=$(lo d '%M' | cut -c1-10)
[ "$got" = 'drwxr-xr-x' ] || pc_fail "%M for d gives '$got', expected drwxr-xr-x"
expect f1 '%.1M' - "%.1M for a regular file"
expect s '%.1M' l "%.1M for a symbolic link"
expect d '%.1M' d "%.1M for a directory"
expect f1 '%F' f1 "%F"
expect s '%L' 's -> f1' "%L for a symbolic link"
expect f1 '%L' f1 "%L for a regular file"
expect f1 '[%D]' '[ ]' "%D for a file that is not a device"
expect f1 '%(size)D' 6 "%(size)D"
expect f1 '%T' 'Jan  2 15:04 2003' "%T"
got=$(TZ=XYZ-3 pax -v -o 'listopt=%T' f1 <a 2>err)
[ "$got" = 'Jan  2 18:04 2003' ] || pc_fail "with TZ=XYZ-3 the time is '$got', expected Jan  2 18:04 2003"
expect f1 '%(mtime=%Y-%m-%d)T' 2003-01-02 "%(mtime=%Y-%m-%d)T"
expect f1 '%F\t%(size)d' "$(printf 'f1\t6')" "a tab in the format"
n=$(lo f1 '%F' | wc -l | tr -d ' ')
[ "$n" -eq 1 ] || pc_fail "the output for one member has $n lines, expected 1 (a newline is added after the entry)"
got=$(TZ=UTC0 pax -v -o 'listopt=%F' -o 'listopt=:%(size)d' f1 <a 2>err)
[ "$got" = 'f1:6' ] || pc_fail "two listopt options give '$got', expected f1:6"
pc_pass
