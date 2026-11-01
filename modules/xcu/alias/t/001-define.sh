# alias name=string defines an alias that is then substituted.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.001
# MUTANT    alias.define_status1
# MUTANT    alias.define_noop
# MUTANT    alias.define_first_word
# MUTANT    alias.define_drop_last
# MUTANT    alias.define_wrong_name
# MUTANT    alias.define_value_after_last_eq
# MUTANT    alias.define_empty_ignored
# MUTANT    alias.define_once
#
# The alias is put to use by eval in the main shell (not in a command
# substitution, which would hide an alias that vanishes after one use), on
# a later line than the definition, and twice.
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.001
# use TEXT: run TEXT through the shell, so that its aliases are looked up
use() { eval "$@"; }
pc_test
alias pcal1='echo hello world'
rc=$?
[ "$rc" -eq 0 ] || pc_fail "alias definition exited $rc"
use 'pcal1 extra' >o1 2>/dev/null
got=$(cat o1)
[ "$got" = "hello world extra" ] || pc_fail "alias not substituted, got '$got'"
use pcal1 >o2 2>/dev/null
got=$(cat o2)
[ "$got" = "hello world" ] || pc_fail "alias not substituted a second time, got '$got'"
# the name ends at the first equals sign; later ones belong to the value
alias pcal1e='echo a=b'
rc=$?
[ "$rc" -eq 0 ] || pc_fail "alias with an equals sign in the value exited $rc"
use pcal1e >o3 2>/dev/null
got=$(cat o3)
[ "$got" = "a=b" ] || pc_fail "value with an equals sign gives '$got'"
# an empty string is a value too
alias pcal1z=
rc=$?
[ "$rc" -eq 0 ] || pc_fail "alias with an empty value exited $rc"
alias pcal1z >/dev/null 2>&1 || pc_fail "alias with an empty value does not exist"
pc_pass
