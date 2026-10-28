# The operand - means standard input (where the utility says so) or a file named -; never anything else.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.12.2-utility-syntax-guidelines.010
#
# A file named - exists. For cat, cut and sort the OPERANDS say that - is
# standard input; for head, tail, grep and sed they say nothing, so it is
# implementation-defined whether - is standard input or the file named -,
# and either is accepted.
. "$PC_CORE/pc.sh"
pc_begin XBD.12.2-utility-syntax-guidelines.010
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'FILE-1\nFILE-2\n' >./- || pc_unresolved "cannot create a file named -"
printf 'STDIN-1\nSTDIN-2\n' >in || pc_unresolved "cannot create in"
pc_test
chk() { # <what> <got> <expected>
	[ "$2" = "$3" ] || pc_fail "$1 gave '$2', expected '$3'"
}
either() { # <what> <got> <stdin form> <file form>
	[ "$2" = "$3" ] || [ "$2" = "$4" ] || pc_fail "$1 gave '$2', expected the standard input ('$3') or the file named - ('$4')"
}
chk "cat -" "$(cat - <in 2>/dev/null)" "STDIN-1${NL}STDIN-2"
chk "sort -" "$(sort - <in 2>/dev/null)" "STDIN-1${NL}STDIN-2"
chk "cut -f 1 -" "$(cut -f 1 - <in 2>/dev/null)" "STDIN-1${NL}STDIN-2"
either "head -n 1 -" "$(head -n 1 - <in 2>/dev/null)" "STDIN-1" "FILE-1"
either "tail -n 1 -" "$(tail -n 1 - <in 2>/dev/null)" "STDIN-2" "FILE-2"
either "grep -e 1 -" "$(grep -e 1 - <in 2>/dev/null)" "STDIN-1" "FILE-1"
either "sed -n 1p -" "$(sed -n 1p - <in 2>/dev/null)" "STDIN-1" "FILE-1"
pc_pass
