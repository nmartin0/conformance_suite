# The alias output is suitable for reinput.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.003
# MUTANT    alias.show_unquoted
# MUTANT    alias.show_sq_unescaped
# MUTANT    alias.show_dq_unescaped
# MUTANT    alias.show_drop_backslash
# MUTANT    alias.show_trim_blank
# MUTANT    alias.show_newline_to_space
# MUTANT    alias.show_drop_last_char
# VARIANT   alias.quote_double
# VARIANT   alias.quote_backslash
#
# Three values: one that is a command (run before and after reinput), one
# full of characters special to the shell with blanks at both ends, and one
# with a newline and a tab. After the alias is removed, the line written is
# given back to alias; the value must be exactly as it was.
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.003
nl='
'
tab=$(printf '\t')
v1=$(cat <<'PCV'
echo 'a b' "c d" \$f '$g'
PCV
)
v2=$(cat <<'PCV'
 it's "q" \ back\\slash $HOME $(x) `y` ;|&<>*?[a]#~! x 
PCV
)
v3="one${nl}two${tab}x"
exp1='a b c d $f $g'

# value_of FILE: print "count|first word" of the text after the first equals
# sign of the line in FILE, read as shell words (a subshell, so that a
# malformed line cannot end the test).
value_of() {
	vl=$(cat "$1"; echo x)
	vl=${vl%x}
	vl=${vl%"$nl"}
	vr=${vl#*=}
	(eval "set -- $vr" 2>/dev/null; printf '%s|%s' "$#" "$1")
}

pc_test
k=0
for v in "$v1" "$v2" "$v3"; do
	k=$((k + 1))
	alias "pcal3$k=$v" || pc_fail "alias pcal3$k failed"
	alias "pcal3$k" >"out$k" 2>/dev/null || pc_fail "alias pcal3$k failed to show"
	pc_need_file "out$k" "the output"
	got=$(value_of "out$k")
	[ "$got" = "1|$v" ] || pc_fail "line for value $k does not read back as the value: '$got'"
	unalias "pcal3$k" 2>/dev/null
	if alias "pcal3$k" >/dev/null 2>&1; then
		pc_unresolved "unalias did not remove pcal3$k"
	fi
	line=$(cat "out$k")
	( eval "alias $line" ) >/dev/null 2>&1 || pc_fail "reinput of line $k was not accepted"
	eval "alias $line"
	alias "pcal3$k" >"again$k" 2>/dev/null || pc_fail "alias pcal3$k missing after reinput"
	got=$(value_of "again$k")
	[ "$got" = "1|$v" ] || pc_fail "after reinput value $k is '$got'"
done
got1=$(eval pcal31)
[ "$got1" = "$exp1" ] || pc_fail "reinput alias runs as '$got1', wanted '$exp1'"
pc_pass
