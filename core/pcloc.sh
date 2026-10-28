# pcloc.sh - helpers for tests of utilities under a locale other than the
# POSIX locale. Source after pc.sh, and only after the test has changed into
# a directory of its own: the files below are made in the working directory.
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Making a locale is optional (POSIX2_LOCALEDEF). pcloc_need reports
# NOTINUSE when the system does not claim it. A locale source that the
# system's localedef refuses is UNRESOLVED (only what the locale then does
# is judged). The names of the locales are pathnames, which XBD 7.1 says
# can be used as the value of a locale variable.
#
#     . "$PC_CORE/pc.sh"
#     . "$PC_CORE/pcloc.sh"
#     pc_begin XCU.sort.017
#     d=pcl.$$; mkdir "$d" && cd "$d" || pc_unresolved "no directory"
#     trap 'cd ..; rm -rf "$d"' EXIT
#     pcloc_make                  made in setup: sets PCLOC
#     pcl_probe() { ... }         the work under test, KIND is L or C
#     pcl_want()  { ... }         what pcl_probe must print for KIND
#     pc_test
#     pcloc_matrix "$PCLOC" LC_COLLATE
#     pc_pass
#
# The locale PCLOC (single-byte code set) differs from the POSIX locale in
# every category:
#   LC_COLLATE  b collates before a, then the element ch (the characters c
#               and h together), then every other character
#   LC_CTYPE    byte 0xe1 is a lowercase letter and 0xc1 an uppercase letter
#               (its toupper mapping), besides the portable characters
#   LC_NUMERIC  radix character <comma>, thousands separator <period>
#   LC_TIME     weekday and month names of the form ua, uaday, ma, mona, ...
#               ante and post for the meridiem; %c is "%Y.%m.%d %H:%M:%S"
#   LC_MESSAGES yesexpr ^([xX]|[[:upper:]]!), noexpr ^([nN]); the variant
#               locale PCLOCY (pcloc_make y) has the yesexpr ^([xX]|[[.ch.]])
# The locale PCLOCMB (pcloc_make_mb) has a two-byte code set (mb_cur_max 2):
# the bytes 0xe1 0xe2 are one lowercase letter; its other categories are
# those of the POSIX locale.

pcloc_nl='
'

pcloc_need() {
	pcloc_v=$(getconf POSIX2_LOCALEDEF 2>/dev/null) || pc_notinuse "getconf cannot report POSIX2_LOCALEDEF"
	case $pcloc_v in
	[1-9]*) ;;
	*) pc_notinuse "creating locales is not claimed (POSIX2_LOCALEDEF is '$pcloc_v')" ;;
	esac
	command -v localedef >/dev/null 2>&1 || pc_notinuse "no localedef utility"
}

pcloc_UP="A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"
pcloc_LO="a b c d e f g h i j k l m n o p q r s t u v w x y z"
pcloc_DG="zero one two three four five six seven eight nine"
pcloc_PUNCT="exclamation-mark quotation-mark number-sign dollar-sign percent-sign ampersand apostrophe left-parenthesis right-parenthesis asterisk plus-sign comma hyphen-minus period slash colon semicolon less-than-sign equals-sign greater-than-sign question-mark commercial-at left-square-bracket backslash right-square-bracket circumflex underscore grave-accent left-curly-bracket vertical-line right-curly-bracket tilde"

# pcloc__list NAME... : <NAME>;<NAME>;... in pcloc_s
pcloc__list() {
	pcloc_s=
	for pcloc_w
	do
		pcloc_s="${pcloc_s:+$pcloc_s;}<$pcloc_w>"
	done
}

# pcloc__sym CHAR : the symbolic name of a character of the portable set, in pcloc_sym
pcloc__sym() {
	case $1 in
	[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]) pcloc_sym=$1 ;;
	0) pcloc_sym=zero ;;
	1) pcloc_sym=one ;;
	2) pcloc_sym=two ;;
	3) pcloc_sym=three ;;
	4) pcloc_sym=four ;;
	5) pcloc_sym=five ;;
	6) pcloc_sym=six ;;
	7) pcloc_sym=seven ;;
	8) pcloc_sym=eight ;;
	9) pcloc_sym=nine ;;
	' ') pcloc_sym=space ;;
	'!') pcloc_sym=exclamation-mark ;;
	'"') pcloc_sym=quotation-mark ;;
	'#') pcloc_sym=number-sign ;;
	'$') pcloc_sym=dollar-sign ;;
	'%') pcloc_sym=percent-sign ;;
	'&') pcloc_sym=ampersand ;;
	"'") pcloc_sym=apostrophe ;;
	'(') pcloc_sym=left-parenthesis ;;
	')') pcloc_sym=right-parenthesis ;;
	'*') pcloc_sym=asterisk ;;
	'+') pcloc_sym=plus-sign ;;
	',') pcloc_sym=comma ;;
	'-') pcloc_sym=hyphen-minus ;;
	'.') pcloc_sym=period ;;
	'/') pcloc_sym=slash ;;
	':') pcloc_sym=colon ;;
	';') pcloc_sym=semicolon ;;
	'<') pcloc_sym=less-than-sign ;;
	'=') pcloc_sym=equals-sign ;;
	'>') pcloc_sym=greater-than-sign ;;
	'?') pcloc_sym=question-mark ;;
	'@') pcloc_sym=commercial-at ;;
	'[') pcloc_sym=left-square-bracket ;;
	'\') pcloc_sym=backslash ;;
	']') pcloc_sym=right-square-bracket ;;
	'^') pcloc_sym=circumflex ;;
	'_') pcloc_sym=underscore ;;
	'`') pcloc_sym=grave-accent ;;
	'{') pcloc_sym=left-curly-bracket ;;
	'|') pcloc_sym=vertical-line ;;
	'}') pcloc_sym=right-curly-bracket ;;
	'~') pcloc_sym=tilde ;;
	*) pc_unresolved "setup: pcloc has no symbolic name for a character it needs" ;;
	esac
}

# pcloc__str STRING : the string in the form a locale source takes (every
# character as its symbolic name, in double quotes), in pcloc_str
pcloc__str() {
	pcloc_str=\"
	pcloc_r=$1
	while [ -n "$pcloc_r" ]
	do
		pcloc_t=${pcloc_r#?}
		pcloc_c=${pcloc_r%"$pcloc_t"}
		pcloc__sym "$pcloc_c"
		pcloc_str="$pcloc_str<$pcloc_sym>"
		pcloc_r=$pcloc_t
	done
	pcloc_str="$pcloc_str\""
}

# pcloc__val CHAR : the numeric value of the character in the code set, in pcloc_n
pcloc__val() {
	pcloc_n=$(printf '%d' "'$1")
}

# pcloc__emit NAME : one charmap line for NAME with the value in pcloc_n, and the second name
pcloc__emit() {
	printf '<%s> \\x%02X\n' "$1" "$pcloc_n"
	case $pcloc_n in
	193 | 225 | 226 | 227) pc_notinuse "a portable character is encoded in the range this helper uses for its extra characters" ;;
	esac
	case $1 in
	alert) pcloc_al=BEL ;;
	backspace) pcloc_al=BS ;;
	tab) pcloc_al=HT ;;
	newline) pcloc_al=LF ;;
	vertical-tab) pcloc_al=VT ;;
	form-feed) pcloc_al=FF ;;
	carriage-return) pcloc_al=CR ;;
	hyphen-minus) pcloc_al=hyphen ;;
	period) pcloc_al=full-stop ;;
	slash) pcloc_al=solidus ;;
	backslash) pcloc_al=reverse-solidus ;;
	circumflex) pcloc_al=circumflex-accent ;;
	underscore) pcloc_al=low-line ;;
	left-curly-bracket) pcloc_al=left-brace ;;
	right-curly-bracket) pcloc_al=right-brace ;;
	*) pcloc_al= ;;
	esac
	if [ -n "$pcloc_al" ]; then
		printf '<%s> \\x%02X\n' "$pcloc_al" "$pcloc_n"
	fi
}

# pcloc__charmap FILE CODESET MBMAX EXTRA : a charmap of the portable
# character set (values found by asking printf, so no encoding is assumed)
# followed by the lines in EXTRA
pcloc__charmap() {
	pcloc_cmcs=$2
	pcloc_cmmax=$3
	pcloc_cmextra=$4
	{
		printf '<code_set_name> %s\n<mb_cur_max> %s\n<mb_cur_min> 1\nCHARMAP\n' "$pcloc_cmcs" "$pcloc_cmmax"
		printf '<NUL> \\x00\n'
		for pcloc_e in 'alert:\a' 'backspace:\b' 'tab:\t' 'vertical-tab:\v' 'form-feed:\f' 'carriage-return:\r'
		do
			pcloc_ch=$(printf '%b' "${pcloc_e#*:}")
			pcloc__val "$pcloc_ch"
			pcloc__emit "${pcloc_e%%:*}"
		done
		pcloc__val "$pcloc_nl"
		pcloc__emit newline
		pcloc__val ' '
		pcloc__emit space
		while read -r pcloc_nm pcloc_ch
		do
			pcloc__val "$pcloc_ch"
			pcloc__emit "$pcloc_nm"
		done <<'EOF'
exclamation-mark !
quotation-mark "
number-sign #
dollar-sign $
percent-sign %
ampersand &
apostrophe '
left-parenthesis (
right-parenthesis )
asterisk *
plus-sign +
comma ,
hyphen-minus -
period .
slash /
colon :
semicolon ;
less-than-sign <
equals-sign =
greater-than-sign >
question-mark ?
commercial-at @
left-square-bracket [
backslash \
right-square-bracket ]
circumflex ^
underscore _
grave-accent `
left-curly-bracket {
vertical-line |
right-curly-bracket }
tilde ~
EOF
		set -- $pcloc_DG
		for pcloc_ch in 0 1 2 3 4 5 6 7 8 9
		do
			pcloc__val "$pcloc_ch"
			pcloc__emit "$1"
			shift
		done
		for pcloc_ch in $pcloc_UP $pcloc_LO
		do
			pcloc__val "$pcloc_ch"
			pcloc__emit "$pcloc_ch"
		done
		printf '%s' "$pcloc_cmextra"
		printf 'END CHARMAP\n'
	} >"$1" || pc_unresolved "setup: cannot write the character map"
}

# pcloc__ctype KIND : the LC_CTYPE source; sb has the letters e1 and c1, mb the two-byte letter
pcloc__ctype() {
	printf 'LC_CTYPE\n'
	pcloc__list $pcloc_UP
	if [ "$1" = sb ]; then printf 'upper %s;<x-upp1>\n' "$pcloc_s"; else printf 'upper %s\n' "$pcloc_s"; fi
	pcloc__list $pcloc_LO
	if [ "$1" = sb ]; then printf 'lower %s;<x-low1>\n' "$pcloc_s"; else printf 'lower %s;<m-one>\n' "$pcloc_s"; fi
	pcloc__list $pcloc_DG
	printf 'digit %s\n' "$pcloc_s"
	printf 'space <tab>;<newline>;<vertical-tab>;<form-feed>;<carriage-return>;<space>\n'
	printf 'cntrl <NUL>;<alert>;<backspace>;<tab>;<newline>;<vertical-tab>;<form-feed>;<carriage-return>\n'
	pcloc__list $pcloc_PUNCT
	printf 'punct %s\n' "$pcloc_s"
	pcloc__list $pcloc_DG A B C D E F a b c d e f
	printf 'xdigit %s\n' "$pcloc_s"
	printf 'blank <space>;<tab>\n'
	if [ "$1" = sb ]; then
		set -- $pcloc_UP
		pcloc_p=
		for pcloc_l in $pcloc_LO
		do
			pcloc_p="${pcloc_p:+$pcloc_p;}(<$pcloc_l>,<$1>)"
			shift
		done
		printf 'toupper %s;(<x-low1>,<x-upp1>)\n' "$pcloc_p"
	fi
	printf 'END LC_CTYPE\n'
}

# pcloc__tl KEYWORD PREFIX SUFFIX LETTER... : a list of names for LC_TIME
pcloc__tl() {
	pcloc_k=$1
	pcloc_pre=$2
	pcloc_suf=$3
	shift 3
	pcloc_line=$pcloc_k
	pcloc_sep=' '
	for pcloc_l
	do
		pcloc__str "$pcloc_pre$pcloc_l$pcloc_suf"
		pcloc_line="$pcloc_line$pcloc_sep$pcloc_str"
		pcloc_sep=';'
	done
	printf '%s\n' "$pcloc_line"
}

# pcloc__kv KEYWORD STRING : KEYWORD "STRING"
pcloc__kv() {
	pcloc__str "$2"
	printf '%s %s\n' "$1" "$pcloc_str"
}

pcloc__time() {
	printf 'LC_TIME\n'
	pcloc__tl abday u '' a b c d e f g
	pcloc__tl day u day a b c d e f g
	pcloc__tl abmon m '' a b c d e f g h i j k l
	pcloc__tl ab_alt_mon n '' a b c d e f g h i j k l
	pcloc__tl mon mon '' a b c d e f g h i j k l
	pcloc__tl alt_mon non '' a b c d e f g h i j k l
	pcloc__kv d_t_fmt '%Y.%m.%d %H:%M:%S'
	pcloc__kv d_fmt '%d.%m.%y'
	pcloc__kv t_fmt '%H:%M:%S'
	pcloc__str ante
	pcloc_a=$pcloc_str
	pcloc__str post
	printf 'am_pm %s;%s\n' "$pcloc_a" "$pcloc_str"
	pcloc__kv t_fmt_ampm '%I:%M:%S %p'
	printf 'END LC_TIME\n'
}

# pcloc__rest VARIANT : the other categories; the variant y has a yesexpr that
# names the collating element ch, so it is a valid expression only where
# LC_COLLATE is the locale's own
pcloc__rest() {
	printf 'LC_MONETARY\ncopy "POSIX"\nEND LC_MONETARY\n'
	printf 'LC_NUMERIC\ndecimal_point "<comma>"\nthousands_sep "<period>"\ngrouping 3;3\nEND LC_NUMERIC\n'
	pcloc__time
	printf 'LC_MESSAGES\n'
	if [ "$1" = y ]; then
		pcloc__kv yesexpr '^([xX]|[[.ch.]])'
	else
		pcloc__kv yesexpr '^([xX]|[[:upper:]]!)'
	fi
	pcloc__kv noexpr '^([nN])'
	printf 'END LC_MESSAGES\n'
}

# pcloc__localedef MAP SRC LOC TAG : run localedef; a status of 2 or 3 is
# NOTINUSE for TAG mb and for status 3, UNRESOLVED otherwise
pcloc__localedef() {
	LC_ALL=C localedef -c -f "$PWD/$1" -i "$PWD/$2" "$3" >pcloc.out 2>pcloc.err </dev/null
	pcloc_rc=$?
	case $pcloc_rc in
	0 | 1) ;;
	3) pc_notinuse "localedef cannot create locales (status 3)" ;;
	2)
		if [ "$4" = mb ]; then pc_notinuse "localedef does not support the multibyte code set of the test (status 2)"; fi
		pc_unresolved "localedef refused the test locale: limits or code set (status 2)"
		;;
	*) pc_unresolved "localedef refused the test locale source $2 (status $pcloc_rc)" ;;
	esac
	[ -e "$3" ] || pc_unresolved "localedef reported success but nothing exists at $3"
	pcloc__verify "$3" "$4"
}

# pcloc__verify LOC TAG : what localedef made must be in effect when named as
# a locale, or a test that uses it would blame the utility under test for a
# locale that was never selected (localedef may issue warnings, status 1, and
# still leave a locale that does not hold what the source said, and a system
# may not accept the name of the locale). The locale utility is asked for the
# radix character of the single-byte locale (a comma, the POSIX locale has a
# period) and, for the multibyte one, for a charmap name that differs from the
# POSIX locale's. A system that leaves the values out (the page lets it) gives
# nothing to check, so the locale is trusted then.
pcloc__verify() {
	command -v locale >/dev/null 2>&1 || return 0
	case $2 in
	mb)
		pcloc_vg=$(LC_ALL=$1 locale charmap 2>/dev/null </dev/null)
		pcloc_vp=$(LC_ALL=POSIX locale charmap 2>/dev/null </dev/null)
		if [ -n "$pcloc_vg" ] && [ "$pcloc_vg" = "$pcloc_vp" ]; then
			pc_unresolved "setup: the multibyte locale made by localedef is not in effect (charmap '$pcloc_vg', as in the POSIX locale)"
		fi
		;;
	*)
		pcloc_vg=$(LC_ALL=$1 locale -k decimal_point 2>/dev/null </dev/null)
		pcloc_vg=${pcloc_vg#*=}
		pcloc_vg=${pcloc_vg#\"}
		pcloc_vg=${pcloc_vg%\"}
		if [ -n "$pcloc_vg" ] && [ "$pcloc_vg" != , ]; then
			pc_unresolved "setup: the locale made by localedef is not in effect (its radix character reads '$pcloc_vg', not a comma)"
		fi
		;;
	esac
}

# pcloc_make [y|t] : make the single-byte locale; PCLOC is its name. With the
# argument y the locale made is PCLOCY, the same except for its yesexpr; with
# t it is PCLOCT, the same except that its collating sequence is not total: b
# has the weight of a, so the two collate as equal.
pcloc_make() {
	pcloc_pe=pcloc
	pcloc_pv=PCLOC
	case ${1:-} in
	y) pcloc_pe=pcloc_y pcloc_pv=PCLOCY ;;
	t) pcloc_pe=pcloc_t pcloc_pv=PCLOCT ;;
	esac
	pcloc_need
	case $PWD in
	/*) ;;
	*) pc_unresolved "setup: the working directory has no absolute name" ;;
	esac
	[ -e pcloc.seq ] || printf '0\n' >pcloc.seq || pc_unresolved "setup: cannot write in the working directory"
	pcloc__charmap pcloc.map PCLOC 1 '<x-low1> \xe1
<x-upp1> \xc1
'
	{
		pcloc__ctype sb
		if [ "${1:-}" = t ]; then
			printf 'LC_COLLATE\norder_start forward\n<NUL>\n<a>\n<b> <a>\nUNDEFINED\norder_end\nEND LC_COLLATE\n'
		else
			printf 'LC_COLLATE\ncollating-element <ch> from "<c><h>"\norder_start forward\n<NUL>\n<b>\n<a>\n<ch>\nUNDEFINED\norder_end\nEND LC_COLLATE\n'
		fi
		pcloc__rest "${1:-}"
	} >"$pcloc_pe.src" || pc_unresolved "setup: cannot write the locale source"
	eval "$pcloc_pv=\$PWD/\$pcloc_pe.loc"
	pcloc__localedef pcloc.map "$pcloc_pe.src" "$PWD/$pcloc_pe.loc" sb
}

# pcloc_make_mb : make the multibyte locale; PCLOCMB is its name
pcloc_make_mb() {
	pcloc_need
	case $PWD in
	/*) ;;
	*) pc_unresolved "setup: the working directory has no absolute name" ;;
	esac
	[ -e pcloc.seq ] || printf '0\n' >pcloc.seq || pc_unresolved "setup: cannot write in the working directory"
	pcloc__charmap pcloc2.map PCLOCMB 2 '<m-one> \xe1\xe2
'
	{
		pcloc__ctype mb
		printf 'LC_COLLATE\ncopy "POSIX"\nEND LC_COLLATE\n'
		printf 'LC_MONETARY\ncopy "POSIX"\nEND LC_MONETARY\n'
		printf 'LC_NUMERIC\ncopy "POSIX"\nEND LC_NUMERIC\n'
		printf 'LC_TIME\ncopy "POSIX"\nEND LC_TIME\n'
		printf 'LC_MESSAGES\ncopy "POSIX"\nEND LC_MESSAGES\n'
	} >pcloc2.src || pc_unresolved "setup: cannot write the locale source"
	PCLOCMB=$PWD/pcloc2.loc
	pcloc__localedef pcloc2.map pcloc2.src "$PCLOCMB" mb
}

# pcloc_hex : the bytes read from standard input as lowercase hexadecimal
# digits (written without separators), so that bytes outside the portable set
# can be compared in a shell whatever its own locale
pcloc_hex() {
	LC_ALL=C od -An -v -tx1 | LC_ALL=C tr -d ' \n'
}

# pcloc_yes SCENARIO RESPONSE : run the utility named by SCENARIO (rm, mv, cp
# or find) on fresh files with the -i or -ok question answered by RESPONSE (a
# printf format, so it can hold octal escapes) and print Y if the action took
# place, N if not
pcloc_yes() {
	read -r pcloc_q <pcloc.seq || pcloc_q=0
	pcloc_q=$((pcloc_q + 1))
	printf '%s\n' "$pcloc_q" >pcloc.seq
	pcloc_dd=pcloc.y$pcloc_q
	mkdir "$pcloc_dd" || {
		printf 'SETUP-FAILED'
		return 0
	}
	case $1 in
	rm)
		: >"$pcloc_dd/f"
		printf "$2"'\n' | rm -i "$pcloc_dd/f" >/dev/null 2>&1
		if [ -e "$pcloc_dd/f" ]; then printf N; else printf Y; fi
		;;
	mv)
		printf 'a\n' >"$pcloc_dd/s"
		printf 'b\n' >"$pcloc_dd/t"
		printf "$2"'\n' | mv -i "$pcloc_dd/s" "$pcloc_dd/t" >/dev/null 2>&1
		if [ -e "$pcloc_dd/s" ]; then printf N; else printf Y; fi
		;;
	cp)
		printf 'a\n' >"$pcloc_dd/s"
		printf 'b\n' >"$pcloc_dd/t"
		printf "$2"'\n' | cp -i "$pcloc_dd/s" "$pcloc_dd/t" >/dev/null 2>&1
		read -r pcloc_z <"$pcloc_dd/t"
		if [ "$pcloc_z" = a ]; then printf Y; else printf N; fi
		;;
	find)
		: >"$pcloc_dd/f"
		printf "$2"'\n' | find "$pcloc_dd" -name f -ok rm '{}' \; >/dev/null 2>&1
		if [ -e "$pcloc_dd/f" ]; then printf N; else printf Y; fi
		;;
	esac
}

# pcloc__apply NAME TOKEN : set and export NAME for the token of a matrix row
pcloc__apply() {
	case $2 in
	-) ;;
	L) eval "$1=\$pcloc_loc; export $1" ;;
	C) eval "$1=C; export $1" ;;
	NULL) eval "$1=; export $1" ;;
	esac
}

# pcloc__run LANG LCALL VAR KIND : run pcl_probe in the environment of a row
pcloc__run() {
	(
		unset LANG LANGUAGE NLSPATH LC_ALL LC_COLLATE LC_CTYPE LC_MESSAGES LC_MONETARY LC_NUMERIC LC_TIME
		for pcloc_f in ${pcl_basec:-}
		do
			pcloc__apply "$pcloc_f" C
		done
		for pcloc_f in $pcloc_fixed
		do
			pcloc__apply "$pcloc_f" L
		done
		pcloc__apply LANG "$1"
		pcloc__apply LC_ALL "$2"
		pcloc__apply "$pcloc_var" "$3"
		pcl_probe "$4"
	) </dev/null
}

# pcloc_matrix LOCALE VAR [FIXEDVAR...] : run the test's pcl_probe in the
# environments that XBD 8.2 gives a meaning to and compare its output with
# what pcl_want prints for the same KIND. KIND is L when the locale named
# LOCALE must be in effect for VAR and C when the POSIX locale must be. The
# rows are:
#   VAR=L; LC_ALL=L with VAR=C; LC_ALL=C with VAR=L; LANG=L alone;
#   LANG=C with VAR=L; LANG=L with VAR=C; LC_ALL set but null with VAR=L.
# FIXEDVAR variables are set to LOCALE in every row. When pcl_lonly is not
# empty, the rows that expect the POSIX locale are left out (the probe uses
# characters whose behaviour in the POSIX locale is unspecified). The
# variables named in pcl_basec are set to C in every row (before the others),
# so that a category the probe does not care about cannot come from an
# implementation-defined default locale. When the test leaves pcl_basec
# unset, it is every category except LOCALE's VAR and the FIXEDVARs.
pcloc_matrix() {
	pcloc_loc=$1
	pcloc_var=$2
	shift 2
	pcloc_fixed=$*
	if [ -z "${pcl_basec+set}" ]; then
		# every category the test does not vary is the POSIX locale, so that no
		# category comes from an implementation-defined default locale (XBD 8.2),
		# whose code set need not be that of the locale made here (XBD 7.1)
		pcl_basec=
		for pcloc_f in LC_COLLATE LC_CTYPE LC_MESSAGES LC_MONETARY LC_NUMERIC LC_TIME
		do
			case " $pcloc_var $pcloc_fixed " in
			*" $pcloc_f "*) ;;
			*) pcl_basec="$pcl_basec $pcloc_f" ;;
			esac
		done
	fi
	while IFS='|' read -r pcloc_d pcloc_lang pcloc_lcall pcloc_vv pcloc_k
	do
		if [ "$pcloc_k" = C ] && [ -n "${pcl_lonly:-}" ]; then
			continue
		fi
		pcloc_got=$(pcloc__run "$pcloc_lang" "$pcloc_lcall" "$pcloc_vv" "$pcloc_k")
		pcloc_exp=$(pcl_want "$pcloc_k")
		[ "$pcloc_got" = "$pcloc_exp" ] || pc_fail "$pcloc_var: with $pcloc_d the result was [$pcloc_got], not [$pcloc_exp]"
	done <<EOF
$pcloc_var set to the locale|-|-|L|L
LC_ALL set to the locale and $pcloc_var to C|-|L|C|L
LC_ALL set to C and $pcloc_var to the locale|-|C|L|C
only LANG set to the locale|L|-|-|L
LANG set to C and $pcloc_var to the locale|C|-|L|L
LANG set to the locale and $pcloc_var to C|L|-|C|C
LC_ALL null and $pcloc_var set to the locale|-|NULL|L|L
EOF
}
