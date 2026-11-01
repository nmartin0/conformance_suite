# ifelse, incr, decr, len, index, substr, translit, shift
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.004
# ifelse chooses by string equality, handles the 4,5 and 6-or-more argument
# forms; incr, decr, len, index, substr, translit and shift give the
# specified strings.
# MUTANT    m4.ifelse_prefix
# MUTANT    m4.ifelse_case
# MUTANT    m4.ifelse_trim
# MUTANT    m4.ifelse_numeric
# MUTANT    m4.ifelse_length
# MUTANT    m4.ifelse_nonequal_third
# MUTANT    m4.ifelse_5args_fifth
# MUTANT    m4.ifelse_4args_empty
# MUTANT    m4.ifelse_6_discard2
# MUTANT    m4.ifelse_6_stop
# MUTANT    m4.ifelse_6_last
# MUTANT    m4.ifelse_6_discard4
# MUTANT    m4.incr_2
# MUTANT    m4.incr_abs
# MUTANT    m4.incr_nocarry
# MUTANT    m4.incr_ident
# MUTANT    m4.decr_2
# MUTANT    m4.decr_clamp0
# MUTANT    m4.decr_ident
# MUTANT    m4.decr_is_incr
# MUTANT    m4.len_plus1
# MUTANT    m4.len_stops_at_space
# MUTANT    m4.len_empty_one
# MUTANT    m4.len_minus1
# MUTANT    m4.index_last
# MUTANT    m4.index_firstchar
# MUTANT    m4.index_prefix_only
# MUTANT    m4.index_1origin
# MUTANT    m4.index_missing_0
# MUTANT    m4.index_missing_len
# MUTANT    m4.substr_1origin
# MUTANT    m4.substr_beyond_err
# MUTANT    m4.substr_third_zero_all
# MUTANT    m4.substr_third_ignored
# MUTANT    m4.substr_len_is_end
# MUTANT    m4.substr_len_plus1
# MUTANT    m4.translit_first_only
# MUTANT    m4.translit_reverse
# MUTANT    m4.translit_pad_last
# MUTANT    m4.translit_nodelete
# MUTANT    m4.translit_noop
# MUTANT    m4.shift_unquoted
# MUTANT    m4.shift_space
# MUTANT    m4.shift_drop2
# MUTANT    m4.shift_keep_first
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in <<'EOM'
define(`m',M)dnl
ifelse(a,a,yes,no)|ifelse(a,b,yes,no)|ifelse(a,b,c)|ifelse(a,b,c,d,e,f,g)|incr(4)|decr(4)|len(hello)|index(hello,ll)|index(hello,z)|substr(hello,1,3)|substr(hello,3)|substr(hello,9)|translit(hello,el,ip)|translit(hello,el)|shift(a,b,c)
ifelse(a,b,c,d,e)|ifelse(a,b,c,d,d,x)|ifelse(a,b,c,a,a,y,z)|ifelse(`a ',a,yes,no)|ifelse(,,e)|ifelse(1,01,yes,no)|incr(9)|incr(-5)|decr(0)|decr(100)|len()|len(`ab cd')|index(hello,lo)|index(hello,h)|index(hello,o)|index(abab,b)|substr(hello,0,2)|substr(hello,1,0)|substr(hello,5)|translit(hello,lo,01)|translit(hello,lo,x)|shift(a)|shift(a,b)|shift(a,`m',`m')
EOM
printf 'yes|no||g|5|3|5|2|-1|ell|lo||hippo|ho|b,c\nd|x|y|no|e|no|10|-4|-1|99|0|5|3|0|4|1|he|||he001|hexx||b|m,m\n' >exp
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "output differs: $(cat out)"
pc_pass
