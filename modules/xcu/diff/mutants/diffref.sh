# Shared by the diff faults and variants: a wrapper over the real utility
# that plants one fault (or one permitted choice), chosen by name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_diffref NAME ARGS...: behaves as diff should, except for what NAME says.
# The real utility runs with its output kept in files; the fault then alters
# the output, the status, the options or the operands.
pc_diffref() (
	M=$1
	shift
	T=.pcdw.$$
	trap 'rm -f "$T".*' EXIT
	TAB=$(printf '\t')
	mode=def n= fb= fr= cform=
	OPTIND=1
	while getopts bcC:efruU: o; do
		case $o in
		b) fb=1 ;;
		r) fr=1 ;;
		c) mode=c n=3 cform=c ;;
		C) mode=c n=$OPTARG cform=C ;;
		u) mode=u n=3 cform=u ;;
		U) mode=u n=$OPTARG cform=U ;;
		e) mode=e ;;
		f) mode=f ;;
		esac
	done
	shift $((OPTIND - 1))
	isdirs=
	if [ $# -eq 2 ] && [ -d "$1" ] && [ -d "$2" ]; then isdirs=1; fi
	# ---- changes to the options and operands, before the real run ----
	case $M in
	swap_operands) [ $# -eq 2 ] && set -- "$2" "$1" ;;
	c_ctx_short | u_ctx_short) [ "$n" -gt 0 ] && n=$((n - 1)) ;;
	c_ctx_long | u_ctx_long) n=$((n + 1)) ;;
	c_default_two) [ "$cform" = c ] && n=2 ;;
	u_default_two) [ "$cform" = u ] && n=2 ;;
	b_ignored) fb= ;;
	d_no_recurse) fr= ;;
	d_always_r) fr=1 ;;
	b_trail_kept | b_runs_unequal | b_tab_ne_space)
		case $M in
		b_trail_kept) S='s/[ 	][ 	]*/ /g' ;;
		b_runs_unequal) S='s/[ 	]*$//' ;;
		b_tab_ne_space) S='s/[ 	]*$//;s/  */ /g' ;;
		esac
		sed "$S" "$1" >"$T.1"
		sed "$S" "$2" >"$T.2"
		fb=
		set -- "$T.1" "$T.2"
		;;
	b_blank_eq_none | b_lead_ignored)
		case $M in
		b_blank_eq_none) S='s/[ 	]//g' ;;
		b_lead_ignored) S='s/^[ 	]*//;s/[ 	]*$//;s/[ 	][ 	]*/ /g' ;;
		esac
		sed "$S" "$1" >"$T.1"
		sed "$S" "$2" >"$T.2"
		if cmp -s "$T.1" "$T.2"; then exit 0; fi
		;;
	s_dash_f1_broken) [ "$1" = - ] && set -- ./- "$2" ;;
	s_dash_f2_broken) [ "$2" = - ] && set -- "$1" ./- ;;
	s_dash_empty)
		: >"$T.1"
		[ "$1" = - ] && set -- "$T.1" "$2"
		[ "$2" = - ] && set -- "$1" "$T.1"
		;;
	s_stdin_head)
		sed 100q >"$T.1"
		[ "$1" = - ] && set -- "$T.1" "$2"
		[ "$2" = - ] && set -- "$1" "$T.1"
		;;
	s_stdin_always)
		if [ "$1" != - ] && [ "$2" != - ]; then set -- - "$2"; fi
		;;
	empty_err)
		for a in "$@"; do
			if [ -f "$a" ] && [ ! -s "$a" ]; then
				echo "diff: $a: empty file" >&2
				exit 2
			fi
		done
		;;
	d_file_dir_ignored)
		if [ $# -eq 2 ] && { [ -d "$1" ] || [ -d "$2" ]; } && ! { [ -d "$1" ] && [ -d "$2" ]; }; then
			exit 0
		fi
		;;
	c_tz_ignored) TZ=UTC0; export TZ ;;
	esac
	RO=
	[ -n "$fb" ] && RO="$RO -b"
	[ -n "$fr" ] && RO="$RO -r"
	case $mode in
	c) RO="$RO -C $n" ;;
	u) RO="$RO -U $n" ;;
	e) RO="$RO -e" ;;
	f) RO="$RO -f" ;;
	esac
	# ---- the real run ----
	command diff $RO "$@" >"$T.o" 2>"$T.e"
	rc=$?
	# ---- changes to the output and the status ----
	case $M in
	status_diff0) [ "$rc" -eq 1 ] && rc=0 ;;
	status_diff2) [ "$rc" -eq 1 ] && rc=2 ;;
	status_err1) [ "$rc" -ge 2 ] && rc=1 ;;
	status_err0) [ "$rc" -ge 2 ] && rc=0 ;;
	status3) [ "$rc" -ge 2 ] && rc=3 ;;
	status255) [ "$rc" -ge 2 ] && rc=255 ;;
	same_rc1) [ "$rc" -eq 0 ] && rc=1 ;;
	err_silent) : >"$T.e" ;;
	err_stdout) cat "$T.e" >>"$T.o"; : >"$T.e" ;;
	same_blank) [ "$rc" -eq 0 ] && echo >>"$T.o" ;;
	same_header)
		if [ "$rc" -eq 0 ]; then
			case $mode in
			c) printf '*** %s\n--- %s\n' "$1" "$2" >"$T.o" ;;
			u) printf -- '--- %s\n+++ %s\n' "$1" "$2" >"$T.o" ;;
			esac
		fi
		;;
	no_final_nl)
		printf '%s' "$(cat "$T.o")" >"$T.p"
		mv "$T.p" "$T.o"
		;;
	swap_operands | c_ctx_short | u_ctx_short | c_ctx_long | u_ctx_long | c_default_two | u_default_two | b_* | s_* | empty_err | d_file_dir_ignored | c_tz_ignored | d_no_recurse | d_always_r) ;;
	*) pc_diffpost "$@" ;;
	esac
	[ -f "$T.p" ] && mv "$T.p" "$T.o"
	cat "$T.o"
	cat "$T.e" >&2
	exit $rc
)

# pc_diffpost OPERANDS...: turn $T.o into $T.p for the fault or variant M.
pc_diffpost() {
	H='[0-9][0-9,]*[acd][0-9][0-9,]*'
	case $M in
	# ---- default output format ----
	def_marks_swapped)
		awk '/^< /{print ">" substr($0,2);next} /^> /{print "<" substr($0,2);next} {print}' "$T.o" >"$T.p" ;;
	def_no_sep) awk '$0!="---"' "$T.o" >"$T.p" ;;
	def_no_old) awk '!/^< /' "$T.o" >"$T.p" ;;
	def_no_new) awk '!/^> /' "$T.o" >"$T.p" ;;
	def_no_prefix) awk '/^[<>] /{print substr($0,3);next} {print}' "$T.o" >"$T.p" ;;
	def_no_space) awk '/^[<>] /{print substr($0,1,1) substr($0,3);next} {print}' "$T.o" >"$T.p" ;;
	def_dash_range) awk '/^[0-9]/{gsub(",","-")} {print}' "$T.o" >"$T.p" ;;
	def_single_dup)
		awk '/^[0-9][0-9,]*c[0-9][0-9,]*$/{
			i=index($0,"c"); a=substr($0,1,i-1); b=substr($0,i+1)
			if (index(a,",")==0) a=a "," a
			if (index(b,",")==0) b=b "," b
			print a "c" b; next} {print}' "$T.o" >"$T.p" ;;
	def_add_off)
		awk '/^[0-9][0-9,]*a[0-9][0-9,]*$/{i=index($0,"a"); print (substr($0,1,i-1)+1) substr($0,i); next} {print}' "$T.o" >"$T.p" ;;
	def_del_off)
		awk '/^[0-9][0-9,]*d[0-9][0-9,]*$/{i=index($0,"d"); print substr($0,1,i) (substr($0,i+1)+1); next} {print}' "$T.o" >"$T.p" ;;
	def_range1_end_off)
		awk '/^[0-9]+,[0-9]+[acd]/{i=match($0,/[acd]/); a=substr($0,1,i-1); split(a,r,","); print r[1] "," (r[2]+1) substr($0,i); next} {print}' "$T.o" >"$T.p" ;;
	def_range2_end_off)
		awk '/^[0-9][0-9,]*[acd][0-9]+,[0-9]+$/{i=match($0,/[acd]/); b=substr($0,i+1); split(b,r,","); print substr($0,1,i) r[1] "," (r[2]+1); next} {print}' "$T.o" >"$T.p" ;;
	def_c_as_a)
		awk '/^[0-9][0-9,]*c[0-9][0-9,]*$/{sub(/c/,"a")} {print}' "$T.o" >"$T.p" ;;
	def_order_rev)
		awk '/^[0-9][0-9,]*[acd][0-9][0-9,]*$/{g++} {b[g]=b[g] $0 "\n"} END{for(i=g;i>=1;i--) printf "%s", b[i]}' "$T.o" >"$T.p" ;;
	# ---- context format ----
	c_hdr_no_time) awk 'NR<=2{print $1 " " $2;next} {print}' "$T.o" >"$T.p" ;;
	c_hdr_swapped)
		awk 'NR==1{sub(/^\*\*\*/,"---")} NR==2{sub(/^---/,"***")} {print}' "$T.o" >"$T.p" ;;
	c_hdr_names_same)
		awk 'NR==1{nm=$2} NR==2{sub(/^--- [^ ]* /,"--- " nm " ")} {print}' "$T.o" >"$T.p" ;;
	c_time_swap)
		awk '{l[NR]=$0}
			END{
			split(l[1],a," "); split(l[2],b," ")
			p1="*** " a[2] " "; p2="--- " b[2] " "
			t1=substr(l[1],length(p1)+1); t2=substr(l[2],length(p2)+1)
			print p1 t2; print p2 t1
			for(k=3;k<=NR;k++) print l[k]}' "$T.o" >"$T.p" ;;
	c_time_now)
		d=$(date '+%a %b %e %T %Y')
		awk -v d="$d" 'NR<=2{print $1 " " $2 " " d;next} {print}' "$T.o" >"$T.p" ;;
	c_time_date_pad) sed '1,2s/\([A-Za-z][a-z][a-z]\)  \([0-9]\) /\1 0\2 /' "$T.o" >"$T.p" ;;
	c_sep_short) awk '$0=="***************"{print "**************";next} {print}' "$T.o" >"$T.p" ;;
	c_range1_stars) sed 's/^\(\*\*\* [0-9,]*\) \*\*\*\*$/\1 ***/' "$T.o" >"$T.p" ;;
	c_range2_dashes) sed 's/^\(--- [0-9,]*\) ----$/\1 ---/' "$T.o" >"$T.p" ;;
	c_range1_end_off)
		awk '/^\*\*\* [0-9]+,[0-9]+ \*\*\*\*$/{split($2,r,","); print "*** " r[1] "," (r[2]+1) " ****";next} {print}' "$T.o" >"$T.p" ;;
	c_range2_end_off)
		awk '/^--- [0-9]+,[0-9]+ ----$/{split($2,r,","); print "--- " r[1] "," (r[2]+1) " ----";next} {print}' "$T.o" >"$T.p" ;;
	c_bang_minus) awk 'NR>2 && /^! /{print "- " substr($0,3);next} {print}' "$T.o" >"$T.p" ;;
	c_bang_missing) awk 'NR>2 && /^! /{print "  " substr($0,3);next} {print}' "$T.o" >"$T.p" ;;
	c_mark_nospace) awk 'NR>2 && /^! /{print "!" substr($0,3);next} {print}' "$T.o" >"$T.p" ;;
	c_ctx_one_space) awk 'NR>2 && /^  /{print substr($0,2);next} {print}' "$T.o" >"$T.p" ;;
	c_hunks_merged) awk '$0=="***************"{s++; if(s>1) next} {print}' "$T.o" >"$T.p" ;;
	c_no_second)
		awk 'NR<=2{print;next} /^\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*$/{skip=0} /^--- [0-9]/{skip=1} !skip{print}' "$T.o" >"$T.p" ;;
	c_no_first)
		awk 'NR<=2{print;next} /^\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*$/{print;skip=0;next} /^\*\*\* [0-9]/{skip=1} /^--- [0-9]/{skip=0} !skip{print}' "$T.o" >"$T.p" ;;
	c_no_header | u_no_header) awk 'NR>2' "$T.o" >"$T.p" ;;
	# ---- unified format ----
	u_hdr_notab) sed "1,2s/$TAB/ /" "$T.o" >"$T.p" ;;
	u_hdr_nozone) sed '1,2s/ [-+][0-9][0-9][0-9][0-9]$//' "$T.o" >"$T.p" ;;
	u_hdr_zone_colon) sed '1,2s/ \([-+][0-9][0-9]\)\([0-9][0-9]\)$/ \1:\2/' "$T.o" >"$T.p" ;;
	u_hdr_date_slash) sed '1,2s/\([0-9][0-9][0-9][0-9]\)-\([0-9][0-9]\)-\([0-9][0-9]\)/\1\/\2\/\3/' "$T.o" >"$T.p" ;;
	u_hdr_time_missing) sed '1,2s/ [0-9][0-9]:[0-9][0-9]:[0-9][0-9]//' "$T.o" >"$T.p" ;;
	u_hdr_stars) sed '1s/^--- /*** /' "$T.o" >"$T.p" ;;
	u_hdr_swapped_names)
		awk -F'\t' '{l[NR]=$0} END{
			n1=substr(l[1],5); sub(/\t.*/,"",n1); n2=substr(l[2],5); sub(/\t.*/,"",n2)
			r1=substr(l[1],5+length(n1)); r2=substr(l[2],5+length(n2))
			print "--- " n2 r1; print "+++ " n1 r2
			for(k=3;k<=NR;k++) print l[k]}' "$T.o" >"$T.p" ;;
	u_stdin_name) sed "1,2s/^\\(---\\|+++\\) -$TAB/\\1 stdin$TAB/" "$T.o" >"$T.p" ;;
	u_range_end_not_count)
		awk '/^@@ /{
			for(k=2;k<=3;k++){ s=substr($k,2); if(index(s,",")){split(s,r,","); $k=substr($k,1,1) r[1] "," (r[1]+r[2]-1)} }
			print;next} {print}' "$T.o" >"$T.p" ;;
	u_start_off)
		awk '/^@@ /{ s=substr($2,2); split(s,r,","); $2="-" (r[1]+1) (index(s,",")?"," r[2]:""); print;next} {print}' "$T.o" >"$T.p" ;;
	u_ctx_nospace) awk 'NR>2 && /^ /{print substr($0,2);next} {print}' "$T.o" >"$T.p" ;;
	u_marks_swapped)
		awk 'NR>2 && /^-/{print "+" substr($0,2);next} NR>2 && /^\+/{print "-" substr($0,2);next} {print}' "$T.o" >"$T.p" ;;
	u_add_before_del)
		awk 'function flush(  i){for(i=1;i<=na;i++)print a[i];for(i=1;i<=nd;i++)print d[i];nd=na=0}
			NR<=2||/^@@/{flush();print;next}
			/^-/{ if(na>0) flush(); d[++nd]=$0; next}
			/^\+/{a[++na]=$0;next}
			{flush();print}
			END{flush()}' "$T.o" >"$T.p" ;;
	u_no_join)
		awk -v n="$n" '
		function emit(from,to,  i,s1,s2,c1,c2,t) {
			s1=h1; s2=h2; c1=0; c2=0
			for(i=1;i<from;i++){ t=substr(b[i],1,1); if(t!="+") s1++; if(t!="-") s2++ }
			for(i=from;i<=to;i++){ t=substr(b[i],1,1); if(t!="+") c1++; if(t!="-") c2++ }
			printf "@@ -%d,%d +%d,%d @@\n", (c1?s1:s1-1), c1, (c2?s2:s2-1), c2
			for(i=from;i<=to;i++) print b[i]
		}
		function flush(  i,j,k,rs,re,L,m) {
			if(nb==0) return
			rs=0
			for(i=1;i<=nb;i++){
				if(substr(b[i],1,1)==" "){ if(!rs) rs=i } else {
					if(rs && seen){ re=i-1; break }
					rs=0; seen=1 }
			}
			if(rs && seen && re) {
				L=re-rs+1; m=(n<L)?n:L
				emit(1,rs-1+m); emit(re-m+1,nb)
			} else emit(1,nb)
			nb=0; seen=0; re=0; rs=0
		}
		NR<=2{print;next}
		/^@@ /{ flush(); s=substr($2,2); split(s,r,","); h1=r[1]; s=substr($3,2); split(s,r,","); h2=r[1]; next }
		{ b[++nb]=$0 }
		END{ flush() }' "$T.o" >"$T.p" ;;
	# ---- ed script ----
	e_order_fwd)
		awk '/^[0-9][0-9,]*[acd]$/{g++} {b[g]=b[g] $0 "\n"} END{for(i=g;i>=1;i--) printf "%s", b[i]}' "$T.o" >"$T.p" ;;
	e_no_dot) awk '$0!="."' "$T.o" >"$T.p" ;;
	e_c_as_a) awk '/^[0-9][0-9,]*c$/{sub(/c$/,"a")} {print}' "$T.o" >"$T.p" ;;
	e_d_as_c) awk '/^[0-9][0-9,]*d$/{sub(/d$/,"c")} {print}' "$T.o" >"$T.p" ;;
	e_space_range) awk '/^[0-9]+,[0-9]+[acd]$/{sub(/,/," ")} {print}' "$T.o" >"$T.p" ;;
	e_cmd_first)
		awk '/^[0-9][0-9,]*[acd]$/{l=substr($0,length($0)); print l substr($0,1,length($0)-1);next} {print}' "$T.o" >"$T.p" ;;
	e_num_off)
		awk '/^[0-9][0-9,]*[acd]$/{l=substr($0,length($0)); a=substr($0,1,length($0)-1); split(a,r,","); if(r[2]!="") print (r[1]+1) "," (r[2]+1) l; else print (r[1]+1) l; next} {print}' "$T.o" >"$T.p" ;;
	e_text_prefixed)
		awk '/^[0-9][0-9,]*[acd]$/{print;next} $0=="."{print;next} {print "> " $0}' "$T.o" >"$T.p" ;;
	e_text_dropped)
		awk '/^[0-9][0-9,]*[acd]$/{print;next} $0=="."{print;next} {next}' "$T.o" >"$T.p" ;;
	e_insert)
		max=$(wc -l <"$1")
		awk -v max="$max" '/^[0-9]+a$/{ k=substr($0,1,length($0)-1)+1; if(k<=max){print k "i";next} } {print}' "$T.o" >"$T.p" ;;
	# ---- forward script ----
	f_order_rev)
		awk '/^[acd][0-9]/{g++} {b[g]=b[g] $0 "\n"} END{for(i=g;i>=1;i--) printf "%s", b[i]}' "$T.o" >"$T.p" ;;
	f_no_dot) awk '$0!="."' "$T.o" >"$T.p" ;;
	f_c_as_a) awk '/^c[0-9]/{sub(/^c/,"a")} {print}' "$T.o" >"$T.p" ;;
	f_comma_range) awk '/^[acd][0-9]+ [0-9]+$/{sub(/ /,",")} {print}' "$T.o" >"$T.p" ;;
	f_cmd_last)
		awk '/^[acd][0-9]/{print substr($0,2) substr($0,1,1);next} {print}' "$T.o" >"$T.p" ;;
	f_num_off)
		awk '/^[acd][0-9]/{l=substr($0,1,1); split(substr($0,2),r," "); if(r[2]!="") print l (r[1]+1) " " (r[2]+1); else print l (r[1]+1); next} {print}' "$T.o" >"$T.p" ;;
	f_text_dropped)
		awk '/^[acd][0-9]/{print;next} $0=="."{print;next} {next}' "$T.o" >"$T.p" ;;
	# ---- directories ----
	d_no_only) awk '!/^Only in /' "$T.o" >"$T.p" ;;
	d_only_dir_wrong)
		awk -v a="$1" -v b="$2" '/^Only in /{
			r=substr($0,9)
			if(index(r,a)==1 && (substr(r,length(a)+1,1)==":"||substr(r,length(a)+1,1)=="/")) {print "Only in " b substr(r,length(a)+1);next}
			if(index(r,b)==1 && (substr(r,length(b)+1,1)==":"||substr(r,length(b)+1,1)=="/")) {print "Only in " a substr(r,length(b)+1);next}
			} {print}' "$T.o" >"$T.p" ;;
	d_only_slash) sed 's|^Only in \(.*\): |Only in \1/|' "$T.o" >"$T.p" ;;
	d_same_reported)
		cp "$T.o" "$T.p"
		if [ -d "$1" ] && [ -d "$2" ]; then
			for f in "$1"/*; do
				b=${f##*/}
				if [ -f "$f" ] && [ -f "$2/$b" ] && cmp -s "$f" "$2/$b"; then
					echo "diff $1/$b $2/$b" >>"$T.p"
					rc=1
				fi
			done
		fi ;;
	d_hdr_noopts) sed 's/^diff -[a-z]* /diff /' "$T.o" >"$T.p" ;;
	d_hdr_word) sed 's/^diff /Files /' "$T.o" >"$T.p" ;;
	d_hdr_opts_fixed) sed 's/^diff -[a-z]* /diff -x /' "$T.o" >"$T.p" ;;
	d_hdr_one_name) awk '/^diff /{n=NF; s=$1; for(i=2;i<n;i++) s=s " " $i; print s;next} {print}' "$T.o" >"$T.p" ;;
	d_body_dropped) awk '/^(Only in|diff |Common)/' "$T.o" >"$T.p" ;;
	o_abs) sed "s|^Only in |Only in $PWD/|" "$T.o" >"$T.p" ;;
	o_base) sed 's|^Only in .*/\([^/:]*\): |Only in \1: |' "$T.o" >"$T.p" ;;
	o_sub_missing) awk '!/^Only in [^:]*\/[^:]*: /' "$T.o" >"$T.p" ;;
	# ---- variants on directories ----
	d_common_subdirs)
		: >"$T.p"
		if [ -d "$1" ] && [ -d "$2" ]; then
			for f in "$1"/*; do
				b=${f##*/}
				[ -d "$f" ] && [ -d "$2/$b" ] && echo "Common subdirectories: $1/$b and $2/$b" >>"$T.p"
			done
		fi
		cat "$T.o" >>"$T.p" ;;
	d_order_rev)
		awk '/^(Only in |diff |Common subdirectories)/{g++} {b[g]=b[g] $0 "\n"} END{for(i=g;i>=1;i--) printf "%s", b[i]}' "$T.o" >"$T.p" ;;
	d_hdr_names) sed 's|^\(diff.*\) [^ ]*/\([^ /]*\) [^ ]*/\([^ /]*\)$|\1 \2 \3|' "$T.o" >"$T.p" ;;
	# ---- permitted choices in the unified format ----
	u_frac_none) sed '1,2s/\.[0-9][0-9]* / /' "$T.o" >"$T.p" ;;
	u_frac_one) sed '1,2s/\(:[0-9][0-9]\)\(\.[0-9]*\)\{0,1\} /\1.0 /' "$T.o" >"$T.p" ;;
	u_empty_blank) awk 'NR>2 && $0==" "{print "";next} {print}' "$T.o" >"$T.p" ;;
	u_empty_space) awk 'NR>2 && $0==""{print " ";next} {print}' "$T.o" >"$T.p" ;;
	# ---- permitted choice in the context format: wider blanks ----
	c_blanks)
		awk 'NR<=2{ p=substr($0,1,3); r=substr($0,5); sp=index(r," "); print p "  " substr(r,1,sp-1) "   " substr(r,sp+1); next }
			/^\*\*\* [0-9,]+ \*\*\*\*$/{print "***  " $2 "  ****";next}
			/^--- [0-9,]+ ----$/{print "---  " $2 "  ----";next}
			{print}' "$T.o" >"$T.p" ;;
	*) echo "pc_diffref: unknown fault $M" >&2; cp "$T.o" "$T.p" ;;
	esac
}
