# pax -p: set-id bits are not restored without e or o; later letters win
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.026
# Extracting a file archived with mode 6755 with -p p, or without -p, does
# not set the set-user-ID and set-group-ID bits and gives rwxr-xr-x under
# umask 022 (the ustar mode field holds 6755); in "-p em" the later m wins
# (the modification time is not preserved) and in "-p me" and "-p eme" it is
# preserved.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.026
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'umask 022; cd "$pc_top" && rm -rf "$pc_wd"' EXIT
umask 022
printf 'x\n' >f1 || pc_unresolved "setup: cannot write f1"
chmod 6755 f1 || pc_unresolved "setup: chmod 6755 failed"
m=$(ls -l f1 | cut -c1-10)
[ "$m" = '-rwsr-sr-x' ] || pc_unresolved "setup: the set-id bits could not be set (mode $m)"
pax -w f1 >a 2>/dev/null </dev/null || pc_unresolved "setup: pax -w failed"
pax -w -x ustar f1 >a.u 2>/dev/null </dev/null || pc_unresolved "setup: pax -w -x ustar failed"
rm -f f1
printf 'y\n' >g1 && touch -t 200101010000 g1 && touch -t 200201010000 ref || pc_unresolved "setup: touch failed"
pax -w g1 >b 2>/dev/null </dev/null || pc_unresolved "setup: pax -w failed"
rm -f g1
pc_test
mf=$(dd if=a.u bs=1 skip=100 count=8 2>/dev/null | tr -d '\000')
[ $(( 0$mf & 4095 )) -eq $((06755)) ] || pc_fail "the mode field of the ustar archive is '$mf', expected 6755 (set-user-ID, set-group-ID and rwxr-xr-x)"
pax -r -p p <a >o 2>err
m=$(ls -l f1 2>/dev/null | cut -c1-10)
[ "$m" = '-rwxr-xr-x' ] || pc_fail "-p p: the extracted file has mode '$m', expected -rwxr-xr-x (no set-id bits)"
rm -f f1
pax -r <a >o 2>err
m=$(ls -l f1 2>/dev/null | cut -c1-10)
case $m in
'') pc_fail "pax -r did not extract f1" ;;
???[sS]*|??????[sS]*) pc_fail "without -p the extracted file has mode '$m' with a set-id bit" ;;
esac
for opt in me eme; do
	rm -f g1
	pax -r -p $opt <b >o 2>/dev/null
	[ -f g1 ] || pc_fail "-p $opt: g1 was not extracted"
	[ -z "$(find g1 -newer ref -print)" ] || pc_fail "-p $opt: the modification time was not preserved"
done
rm -f g1
pax -r -p m -p e <b >o 2>/dev/null
[ -f g1 ] || pc_fail "-p m -p e: g1 was not extracted"
[ -z "$(find g1 -newer ref -print)" ] || pc_fail "-p m -p e: the modification time was not preserved"
rm -f g1
pax -r -p em <b >o 2>/dev/null
[ -f g1 ] || pc_fail "-p em: g1 was not extracted"
[ -n "$(find g1 -newer ref -print)" ] || pc_fail "-p em: the modification time was preserved although m came last"
pc_pass
