# A missing file leaves the entry alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.crontab.005
. "$PC_CORE/pc.sh"
pc_begin XCU.crontab.005
LC_ALL=C; export LC_ALL
d=pccron.$$
had=no
mkdir "$d" || pc_fail "cannot make $d"
if crontab -l >"$d/bak" 2>/dev/null </dev/null; then had=yes; fi
# touched: set just before the first change of the user's crontab entry.
touched=no
# Put the saved entry back (or remove the one made here when there was none).
# The saved copy is kept, and its place reported, if it cannot be put back.
cleanup() {
	if [ "$touched" = yes ]; then
		if [ "$had" = yes ]; then
			n=0
			until crontab "$d/bak" >/dev/null 2>&1 </dev/null; do
				n=$((n + 1))
				if [ "$n" -ge 3 ]; then
					echo "posixcheck: could not restore the crontab entry; the saved copy is $PWD/$d/bak" >&2
					return
				fi
			done
		else
			crontab -r >/dev/null 2>&1 </dev/null
		fi
	fi
	rm -rf "$d"
}
trap cleanup EXIT
# a signal ends the test through the EXIT trap, so the entry is put back
trap 'exit 1' HUP INT TERM
# entries FILE: e = the normalised non-blank, non-comment lines of FILE
entries() {
	e=
	set -f
	while IFS= read -r el; do
		case $el in
		*[!' 	']*) ;;
		*) continue ;;
		esac
		et=${el#"${el%%[!' 	']*}"}
		case $et in '#'*) continue ;; esac
		set -- $el
		e="$e$*;"
	done <"$1"
	set +f
}
printf '# first entry\n0 0 30 2 * true pccrontab_a\n' >"$d/a"
printf '\n# second entry\n5 1 30 2 * true pccrontab_b\n' >"$d/b"
touched=yes
crontab "$d/a" >"$d/o" 2>"$d/e" </dev/null || pc_unresolved "setup: crontab could not install an entry (the user may not be allowed to use it)"
pc_test
crontab "$d/nosuchfile" >"$d/o" 2>"$d/e" </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "crontab of a missing file exited with status 0"
crontab -l >"$d/l" 2>"$d/e" </dev/null || pc_fail "crontab -l failed"
entries "$d/a"
want=$e
entries "$d/l"
[ "$e" = "$want" ] || pc_fail "the installed entries changed to '$e' from '$want'"
pc_pass
