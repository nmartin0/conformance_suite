# FS of one blank splits on runs of blanks, any other single character splits at each occurrence, longer values are regular expressions, and a change to FS applies from the next record.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.004
# MUTANT    awk.fs_first_char
# MUTANT    awk.fs_single_is_ere
# MUTANT    awk.fs_regex_literal
# MUTANT    awk.fs_blank_literal
# MUTANT    awk.fs_change_immediate
# MUTANT    awk.fs_prog_ignored
# MUTANT    awk.default_fs_keeps_edges
# MUTANT    awk.fs_default_blank_only
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.004
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
got=$(printf 'a,b,,c\n' | awk -F, '{ print NF }' 2>/dev/null)
[ "$got" = "4" ] || pc_fail "-F, did not give four fields for a,b,,c"
got=$(printf ',a\n' | awk -F, '{ print NF, "[" $1 "]" }' 2>/dev/null)
[ "$got" = "2 []" ] || pc_fail "a leading separator did not give an empty first field"
got=$(printf 'a b\tc\n' | awk '{ print NF }' 2>/dev/null)
[ "$got" = "3" ] || pc_fail "the default FS did not split at tabs and blanks"
got=$(printf 'a b\tc\n' | awk -F'\t' '{ print $2 }' 2>/dev/null)
[ "$got" = "c" ] || pc_fail "-F with a tab escape failed"
got=$(printf 'a|b\n' | awk -F'|' '{ print $2 }' 2>/dev/null)
[ "$got" = "b" ] || pc_fail "FS of | was not taken as a literal character"
got=$(printf 'a.b\n' | awk -F. '{ print $2 }' 2>/dev/null)
[ "$got" = "b" ] || pc_fail "FS of . was not taken as a literal character"
got=$(printf 'a1b22c333d\n' | awk -F'[0-9]+' '{ print NF, $4 }' 2>/dev/null)
[ "$got" = "4 d" ] || pc_fail "a regular expression FS gave the wrong fields"
got=$(printf 'a:b\n' | awk 'BEGIN { FS = ":" } { print $2 }' 2>/dev/null)
[ "$got" = "b" ] || pc_fail "FS set in BEGIN was not used"
got=$(printf 'a:b c\nd:e f\n' | awk '{ FS = ":"; print $1 }' 2>/dev/null)
[ "$got" = "a:b${NL}d" ] || pc_fail "a change to FS affected the record already read"
got=$(printf ' a  b \n' | awk '{ print NF, "[" $1 "]" }' 2>/dev/null)
[ "$got" = "2 [a]" ] || pc_fail "leading and trailing blanks produced fields with FS of a blank"
got=$(printf 'a b\n' | awk -F' ' '{ print $2 }' 2>/dev/null)
[ "$got" = "b" ] || pc_fail "-F ' ' did not behave as the default separator"
got=$(printf ' a  b\n' | awk -F' ' '{ print NF, $2 }' 2>/dev/null)
[ "$got" = "2 b" ] || pc_fail "-F ' ' did not skip leading blanks and split at runs of blanks"
pc_pass
