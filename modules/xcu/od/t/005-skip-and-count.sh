# od -j skips bytes (decimal, octal or hexadecimal) and fails if the input is shorter; -N limits the bytes formatted and is no error if fewer remain.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.005
# MUTANT    od.j_ignored
# MUTANT    od.j_plus1
# MUTANT    od.j_minus1
# MUTANT    od.j_oct_dec
# MUTANT    od.j_hex_0
# MUTANT    od.j_suffix_ign
# MUTANT    od.j_k_1000
# MUTANT    od.j_b_1024
# MUTANT    od.j_m_1000000
# MUTANT    od.j_hexb_mult
# MUTANT    od.j_trunc32
# MUTANT    od.j_per_file
# MUTANT    od.j_short_status0
# MUTANT    od.j_short_silent
# MUTANT    od.j_short_stdout
# MUTANT    od.j_equal_err
# MUTANT    od.j_limit_plus1
# MUTANT    od.N_ignored
# MUTANT    od.N_plus1
# MUTANT    od.N_minus1
# MUTANT    od.N_oct_dec
# MUTANT    od.N_hex_0
# MUTANT    od.N_short_err
# MUTANT    od.N_per_file
# MUTANT    od.N_before_j
# VARIANT   od.status2
# VARIANT   od.status255
# VARIANT   od.gap3
# VARIANT   od.hex_upper
# VARIANT   od.an_empty
. "$PC_CORE/pc.sh"
pc_begin XCU.od.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcdefghij' >f1 || pc_fail "cannot create input"
printf 'abc' >fa || pc_fail "cannot create input"
printf 'def' >fb || pc_fail "cannot create input"
printf 'abcdefghijklmnopqrstuvwxyz0123456789' >f36 || pc_fail "cannot create input"
# 512 a, 512 b, then c up to one mebibyte, then d: tells the multipliers apart
awk 'BEGIN { for (i = 0; i < 512; i++) printf "a"; for (i = 0; i < 512; i++) printf "b"; for (i = 1024; i < 1048576; i++) printf "c"; for (i = 0; i < 8; i++) printf "d" }' >fbig || pc_fail "cannot create input"
pc_test
got=$(od -A n -v -t x1 -j 2 -N 3 f1 2>/dev/null </dev/null)
set -- $got
[ "$*" = "63 64 65" ] || pc_fail "-j 2 -N 3 is wrong"
got=$(od -A n -v -t x1 -j 010 f1 2>/dev/null </dev/null)
set -- $got
[ "$*" = "69 6a" ] || [ "$*" = "69 6A" ] || pc_fail "-j 010 did not skip eight bytes"
got=$(od -A n -v -t x1 -j 0x4 -N 0x2 f1 2>/dev/null </dev/null)
set -- $got
[ "$*" = "65 66" ] || pc_fail "-j 0x4 -N 0x2 is wrong"
got=$(od -A n -v -t x1 -N 100 f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
case $got in
*rc=0) ;;
*) pc_fail "-N larger than the input was an error" ;;
esac
set -- ${got%rc=0}
[ $# -eq 10 ] || pc_fail "-N larger than the input did not format all ten bytes"
od -A n -v -t x1 -j 100 f1 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "-j beyond the end of the input gave status 0"
[ -s err ] || pc_fail "-j beyond the end of the input wrote no diagnostic to standard error"
# the input is ten bytes: 11 is too many, 10 is not
od -A n -v -t x1 -j 11 f1 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "-j 11 on ten bytes gave status 0"
[ -s err ] || pc_fail "-j 11 on ten bytes wrote no diagnostic"
od -A n -v -t x1 -j 10 f1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-j 10 on ten bytes gave status $rc"
od -A n -v -t x1 -j 4294967296 f1 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "-j 4294967296 on ten bytes gave status 0"
od -A n -v -t x1 -j 4294967297 f1 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "-j 4294967297 on ten bytes gave status 0"
# -j and -N count over the concatenated input files
got=$(od -A n -v -t x1 -j 4 fa fb 2>/dev/null </dev/null)
set -- $got
[ "$*" = "65 66" ] || pc_fail "-j 4 over two files is wrong"
got=$(od -A n -v -t x1 -N 4 fa fb 2>/dev/null </dev/null)
set -- $got
[ "$*" = "61 62 63 64" ] || pc_fail "-N 4 over two files is wrong"
got=$(od -A n -v -t x1 -j 1 -N 4 fa fb 2>/dev/null </dev/null)
set -- $got
[ "$*" = "62 63 64 65" ] || pc_fail "-j 1 -N 4 over two files is wrong"
# prefixes: 0X for hex, and the multipliers b, k, m of -j
got=$(od -A n -v -t x1 -j 0X4 -N 2 f1 2>/dev/null </dev/null)
set -- $got
[ "$*" = "65 66" ] || pc_fail "-j 0X4 -N 0x2 is wrong"
got=$(od -A n -v -t x1 -N 0X2 f1 2>/dev/null </dev/null)
set -- $got
[ "$*" = "61 62" ] || pc_fail "-N 0X2 is wrong"
got=$(od -A n -v -t x1 -N 010 f1 2>/dev/null </dev/null)
set -- $got
[ $# -eq 8 ] || pc_fail "-N 010 did not format eight bytes"
got=$(od -A n -v -t x1 -j 0x1b -N 1 f36 2>/dev/null </dev/null)
set -- $got
[ "$*" = "31" ] || pc_fail "-j 0x1b did not skip 27 bytes (the b is a hexadecimal digit)"
got=$(od -A n -v -t x1 -j 1b -N 1 fbig 2>/dev/null </dev/null)
set -- $got
[ "$*" = "62" ] || pc_fail "-j 1b did not skip 512 bytes"
got=$(od -A n -v -t x1 -j 1k -N 1 fbig 2>/dev/null </dev/null)
set -- $got
[ "$*" = "63" ] || pc_fail "-j 1k did not skip 1024 bytes"
got=$(od -A n -v -t x1 -j 2b -N 1 fbig 2>/dev/null </dev/null)
set -- $got
[ "$*" = "63" ] || pc_fail "-j 2b did not skip 1024 bytes"
got=$(od -A n -v -t x1 -j 1m -N 1 fbig 2>/dev/null </dev/null)
set -- $got
[ "$*" = "64" ] || pc_fail "-j 1m did not skip 1048576 bytes"
pc_pass
