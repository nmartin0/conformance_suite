# alias with a name writes one name=value line.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.002
# MUTANT    alias.show_no_newline
# MUTANT    alias.show_extra_blank
# MUTANT    alias.show_no_name
# MUTANT    alias.show_space_sep
# MUTANT    alias.show_unquoted
# MUTANT    alias.show_stderr
# MUTANT    alias.show_all
# MUTANT    alias.show_prefix
# MUTANT    alias.show_exit1
# MUTANT    alias.show_empty_value
# MUTANT    alias.show_drop_last_char
# VARIANT   alias.quote_double
# VARIANT   alias.quote_backslash
#
# The quoting form is the implementation's choice; the value is checked by
# reading the text after the equals sign as shell words.
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.002
nl='
'
pc_test
alias pcal2='echo a b'
alias pcal2b='echo other'
alias pcal2 >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "alias pcal2 exited $rc"
pc_need_file out "the output"
content=$(cat out; echo x)
content=${content%x}
line=${content%"$nl"}
[ "$content" != "$line" ] || pc_fail "output does not end with a newline"
case $line in
*"$nl"*) pc_fail "more than one line written" ;;
pcal2=*) ;;
*) pc_fail "line does not begin with pcal2=: $line" ;;
esac
rest=${line#pcal2=}
res=$(eval "set -- $rest" 2>/dev/null; printf '%s|%s' "$#" "$1")
[ "$res" = "1|echo a b" ] || pc_fail "the text after the equals sign is not the value as one word: '$rest'"
pc_pass
