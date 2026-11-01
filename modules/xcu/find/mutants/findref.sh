# Shared by the find faults and variants: a wrapper around the real
# utility that rewrites its arguments, the output or the status in exactly
# one way, chosen by name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_findwrap NAME: put a broken standalone find, first on PATH, that plants
# the fault or the permitted choice NAME. The wrapper keeps its helpers in
# pc-mutbin of the test's private directory.
pc_findwrap() {
	mkdir -p pc-mutbin
	pcf_here=$PWD/pc-mutbin
	printf '#!/bin/sh\nREAL="%s"\nPCF_DIR="%s"\nexport PCF_DIR\n. "%s"\npc_findmut %s "$@"\n' \
		"$(command -v find)" "$pcf_here" "${PC_MUTANT_FILE%/*}/../mutants/findref.sh" "$1" >pc-mutbin/find
	chmod +x pc-mutbin/find
	# helper that stands in for the -ok primary (prompt, reply, run)
	cp "${PC_MUTANT_FILE%/*}/../mutants/findok.sh" pc-mutbin/findok.sh
	# awk program that regroups an expression
	cp "${PC_MUTANT_FILE%/*}/../mutants/findexpr.awk" pc-mutbin/findexpr.awk
	cp "${PC_MUTANT_FILE%/*}/../mutants/findorder.awk" pc-mutbin/findorder.awk
	PATH=$pcf_here:$PATH
}

# pc_findmut NAME ARGS...: act as find, except for what NAME says.
pc_findmut() {
	M=$1
	shift
	nl='
'
	# ---- the environment faults act before anything else
	case $M in
	loc_*) pc_findloc ;;
	esac
	# ---- the expression regrouping faults (done by awk)
	case $M in
	prec_* | noshort_*)
		printf '%s\n' "$@" >pc-mutbin/tok.$$
		awk -v mode="$M" -f "$PCF_DIR/findexpr.awk" pc-mutbin/tok.$$ >pc-mutbin/tok2.$$ || exit 1
		set --
		while IFS= read -r l; do set -- "$@" "$l"; done <pc-mutbin/tok2.$$
		rm -f pc-mutbin/tok.$$ pc-mutbin/tok2.$$
		;;
	esac
	# ---- implicit -print faults look at the whole expression first
	pcf_pr= pcf_ex= pcf_or=
	for pcf_x in "$@"; do
		case $pcf_x in
		-print | -print0) pcf_pr=1 ;;
		-exec | -ok) pcf_ex=1 ;;
		-o) pcf_or=1 ;;
		esac
	done
	case $M in
	depth_prune_acts)
		pcf_d= pcf_p=
		for pcf_x in "$@"; do
			[ "$pcf_x" = -depth ] && pcf_d=1
			[ "$pcf_x" = -prune ] && pcf_p=1
		done
		[ -n "$pcf_d" ] && [ -n "$pcf_p" ] && M=depth_ignored ;;
	esac
	# ---- argument rewriting
	hasdepth=
	phase=0 inexec= execkind= prev= drop= skip= stopped= seenpath= firstopt=
	n=$#
	while [ "$n" -gt 0 ]; do
		a=$1
		shift
		n=$((n - 1))
		nx=
		[ "$n" -gt 0 ] && nx=$1
		b1= b2= b3= f1= f2= f3= f4= f5= f6= f7= f8=
		drop= skip=
		# where are we: options, path operands, expression
		case $phase in
		0) case $a in
			-H | -L) ;;
			-* | '!' | '(') phase=2 ;;
			*) phase=1 ;;
			esac ;;
		1) case $a in
			-* | '!' | '(') phase=2 ;;
			esac ;;
		esac
		if [ "$phase" = 1 ]; then
			[ -n "$stopped" ] && drop=1
			case $M in
			err_skipped) [ -e "$a" ] || [ -L "$a" ] || drop=1 ;;
			err_stops) [ -e "$a" ] || [ -L "$a" ] || stopped=1 ;;
			paths_first_only) [ -n "$seenpath" ] && drop=1 ;;
			paths_last_only)
				case $nx in
				'' | -* | '!' | '(') ;;
				*) drop=1 ;;
				esac ;;
			slash_stripped)
				while :; do
					case $a in
					?*/) a=${a%/} ;;
					*) break ;;
					esac
				done ;;
			slash_collapsed)
				case $a in
				*//) a=${a%/} ;;
				esac ;;
			esac
		fi
		[ "$phase" = 1 ] && [ -z "$drop" ] && seenpath=1
		if [ "$phase" = 0 ]; then
			case $M in
			opt_H_ignored) [ "$a" = -H ] && drop=1 ;;
			opt_L_ignored) [ "$a" = -L ] && drop=1 ;;
			opt_first_wins)
				# only the first of -H and -L counts
				case $a in
				-H | -L)
					if [ -n "$firstopt" ]; then drop=1; else firstopt=$a; fi ;;
				esac ;;
			newer_link_own) case $a in -H | -L) a=-P ;; esac ;;
			opt_H_as_L) [ "$a" = -H ] && a=-L ;;
			opt_L_as_H) [ "$a" = -L ] && a=-H ;;
			esac
		fi
		term=
		[ "$a" = -depth ] && hasdepth=1
		if [ "$phase" = 2 ]; then
			# ---- terminator of -exec / -ok
			if [ -n "$inexec" ]; then
				case $a in
				';') term=semi ;;
				+) [ "$prev" = '{}' ] && [ "$execkind" = exec ] && term=plus ;;
				esac
			fi
			case $M in
			depth_if_evaluated)
				case $prev in
				-a | -o | '(') [ "$a" = -depth ] && drop=1 ;;
				esac ;;
			newer_link_own) ;;
			newer_dangling_old)
				if [ "$prev" = -newer ] && [ -L "$a" ] && [ ! -e "$a" ]; then
					[ -f "$PCF_DIR/ref0" ] || TZ=UTC0 touch -t 197001020000 "$PCF_DIR/ref0"
					a=$PCF_DIR/ref0
				fi ;;
			depth_root_only) [ "$a" = -depth ] && { drop=1; hasdepth=1; } ;;
			# ---- 003 -name
			name_whole_path) [ "$a" = -name ] && a=-path ;;
			name_case_blind) [ "$a" = -name ] && a=-iname ;;
			name_prefix) [ "$prev" = -name ] && a="$a*" ;;
			name_slash_path)
				if [ "$a" = -name ]; then case $nx in */*) a=-path ;; esac; fi ;;
			name_no_glob)
				[ "$prev" = -name ] && a=$(printf '%s\n' "$a" | sed 's/[*?[]/\\&/g') ;;
			name_not_dirs)
				[ "$prev" = -name ] && { f1='!' f2=-type f3=d; } ;;
			# ---- 004 leading period
			period_star)
				if [ "$prev" = -name ]; then case $a in '*'*) f1='!' f2=-name f3='.*' ;; esac; fi ;;
			period_qmark)
				if [ "$prev" = -name ]; then case $a in '?'*) f1='!' f2=-name f3='.*' ;; esac; fi ;;
			# ---- 005 -iname
			iname_as_name) [ "$a" = -iname ] && a=-name ;;
			iname_lower)
				if [ "$a" = -iname ]; then a=-name; fi
				if [ "$prev" = -iname ]; then a=$(printf '%s\n' "$a" | tr '[:upper:]' '[:lower:]'); fi ;;
			iname_upper)
				if [ "$a" = -iname ]; then a=-name; fi
				if [ "$prev" = -iname ]; then a=$(printf '%s\n' "$a" | tr '[:lower:]' '[:upper:]'); fi ;;
			# ---- 006 -path
			path_as_name) [ "$a" = -path ] && a=-name ;;
			path_substring) [ "$prev" = -path ] && a="*$a*" ;;
			path_literal)
				[ "$prev" = -path ] && a=$(printf '%s\n' "$a" | sed 's/[*?[]/\\&/g') ;;
			path_star_stops)
				if [ "$prev" = -path ]; then
					pcf_s='*'
					pcf_t=$a
					while :; do
						case $pcf_t in
						*/*) pcf_s="$pcf_s/*" pcf_t=${pcf_t#*/} ;;
						*) break ;;
						esac
					done
					pcf_s="$pcf_s/*"
					f1='!' f2=-path f3=$pcf_s
				fi ;;
			# ---- 007 -type
			type_*_as_*)
				if [ "$a" = -type ]; then
					pcf_x=${M#type_}
					pcf_from=${pcf_x%%_as_*}
					pcf_to=${pcf_x##*_as_}
					if [ "$nx" = "$pcf_from" ]; then skip=1; f1=$pcf_to; fi
				fi ;;
			type_*_also_*)
				if [ "$a" = -type ]; then
					pcf_x=${M#type_}
					pcf_from=${pcf_x%%_also_*}
					pcf_to=${pcf_x##*_also_}
					if [ "$nx" = "$pcf_from" ]; then
						set -- "$@" '(' -type "$pcf_from" -o -type "$pcf_to" ')'
						shift
						n=$((n - 1))
						prev=$nx
						continue
					fi
				fi ;;
			# ---- 008 -perm
			perm_exact_atleast)
				if [ "$a" = -perm ]; then case $nx in -*) ;; *) skip=1; f1="-$nx" ;; esac; fi ;;
			perm_atleast_exact)
				if [ "$a" = -perm ]; then case $nx in -*) skip=1; f1=${nx#-} ;; esac; fi ;;
			perm_atleast_any)
				if [ "$a" = -perm ]; then case $nx in -*) skip=1; f1="/${nx#-}" ;; esac; fi ;;
			perm_symbolic_ignored)
				if [ "$a" = -perm ]; then
					case $nx in
					[0-7]* | -[0-7]*) ;;
					*) skip=1; f1=-000 ;;
					esac
				fi ;;
			perm_octal_ignored)
				if [ "$a" = -perm ]; then
					case $nx in
					[0-7]* | -[0-7]*) skip=1; f1=-000 ;;
					esac
				fi ;;
			# ---- 009 -links
			links_plus_ge)
				if [ "$prev" = -links ]; then case $a in +*) a=+$((${a#+} - 1)) ;; esac; fi ;;
			links_minus_le)
				if [ "$prev" = -links ]; then case $a in -*) a=-$((${a#-} + 1)) ;; esac; fi ;;
			links_signs_swapped)
				if [ "$prev" = -links ]; then
					case $a in +*) a=-${a#+} ;; -*) a=+${a#-} ;; esac
				fi ;;
			links_exact_ge)
				if [ "$prev" = -links ]; then case $a in [0-9]*) a=+$((a - 1)) ;; esac; fi ;;
			links_exact_le)
				if [ "$prev" = -links ]; then case $a in [0-9]*) a=-$((a + 1)) ;; esac; fi ;;
			links_sign_ignored)
				if [ "$prev" = -links ]; then case $a in [+-]*) a=${a#[+-]} ;; esac; fi ;;
			# ---- 010 -size
			size_floor)
				if [ "$a" = -size ]; then
					case $nx in
					*[!0-9]* | '') ;;
					*)
						# whole 512-byte blocks, rounded down
						pcf_lo=$((nx * 512))
						if [ "$pcf_lo" -eq 0 ]; then
							set -- "$@" -size -512c
						else
							set -- "$@" '(' -size +$((pcf_lo - 1))c -size -$((pcf_lo + 512))c ')'
						fi
						shift
						n=$((n - 1))
						prev=$nx
						continue ;;
					esac
				fi ;;
			size_c_blocks)
				if [ "$prev" = -size ]; then a=${a%c}; fi ;;
			size_k)
				if [ "$a" = -size ]; then
					case $nx in
					*[!0-9]* | '') ;;
					*)
						# units of 1024 bytes, rounded up
						if [ "$nx" -eq 0 ]; then
							set -- "$@" -size 0c
						else
							set -- "$@" '(' -size +$(((nx - 1) * 1024))c -size -$((nx * 1024 + 1))c ')'
						fi
						shift
						n=$((n - 1))
						prev=$nx
						continue ;;
					esac
				fi ;;
			size_c_off1)
				if [ "$prev" = -size ]; then
					case $a in
					[0-9]*c) a=$((${a%c} + 1))c ;;
					esac
				fi ;;
			size_plus_ge)
				if [ "$prev" = -size ]; then case $a in +*[0-9]) a=+$((${a#+} - 1)) ;; esac; fi ;;
			size_minus_le)
				if [ "$prev" = -size ]; then case $a in -*[0-9]) a=-$((${a#-} + 1)) ;; esac; fi ;;
			size_plus_floor)
				if [ "$prev" = -size ]; then
					case $a in
					+*[0-9]) a=+$(((${a#+} + 1) * 512 - 1))c ;;
					esac
				fi ;;
			# ---- 011 times
			time_plus_ge)
				case $prev in
				-mtime | -atime | -ctime) case $a in +*) a=+$((${a#+} - 1)) ;; esac ;;
				esac ;;
			time_minus_le)
				case $prev in
				-mtime | -atime | -ctime) case $a in -*) a=-$((${a#-} + 1)) ;; esac ;;
				esac ;;
			time_exact_minus1)
				case $prev in
				-mtime | -atime | -ctime) case $a in [0-9]*) a=$((a - 1)) ;; esac ;;
				esac ;;
			time_exact_plus1)
				case $prev in
				-mtime | -atime | -ctime) case $a in [0-9]*) a=$((a + 1)) ;; esac ;;
				esac ;;
			time_sign_ignored)
				case $prev in
				-mtime | -atime | -ctime) case $a in [+-]*) a=${a#[+-]} ;; esac ;;
				esac ;;
			time_atime_as_mtime) [ "$a" = -atime ] && a=-mtime ;;
			time_mtime_as_atime) [ "$a" = -mtime ] && a=-atime ;;
			time_ctime_as_mtime) [ "$a" = -ctime ] && a=-mtime ;;
			time_mtime_as_ctime) [ "$a" = -mtime ] && a=-ctime ;;
			time_atime_as_ctime) [ "$a" = -atime ] && a=-ctime ;;
			time_rounded_up)
				# ages rounded up to whole days: "n" is read as "n-1", "+n" as "+(n-1)" and "-n" as "-(n-1)"
				case $prev in
				-mtime | -atime | -ctime)
					case $a in
					+*) a=+$((${a#+} - 1)) ;;
					-*) a=-$((${a#-} - 1)) ;;
					[0-9]*) a=$((a - 1)) ;;
					esac ;;
				esac ;;
			# ---- 012 -user -group, 013 -nouser -nogroup
			user_name_nomatch)
				if [ "$prev" = -user ]; then case $a in *[!0-9]*) a=4242424 ;; esac; fi ;;
			user_number_nomatch)
				if [ "$prev" = -user ]; then case $a in *[!0-9]* | '') ;; *) a=4242424 ;; esac; fi ;;
			user_any)
				if [ "$a" = -user ]; then
					set -- "$@" '(' -user "$nx" -o '!' -user "$nx" ')'
					shift
					n=$((n - 1))
					prev=$nx
					continue
				fi ;;
			user_inverted) [ "$a" = -user ] && { b1='!'; } ;;
			group_name_nomatch)
				if [ "$prev" = -group ]; then case $a in *[!0-9]*) a=4242424 ;; esac; fi ;;
			group_number_nomatch)
				if [ "$prev" = -group ]; then case $a in *[!0-9]* | '') ;; *) a=4242424 ;; esac; fi ;;
			group_any)
				if [ "$a" = -group ]; then
					set -- "$@" '(' -group "$nx" -o '!' -group "$nx" ')'
					shift
					n=$((n - 1))
					prev=$nx
					continue
				fi ;;
			group_inverted) [ "$a" = -group ] && { b1='!'; } ;;
			nouser_as_nogroup) [ "$a" = -nouser ] && a=-nogroup ;;
			nogroup_as_nouser) [ "$a" = -nogroup ] && a=-nouser ;;
			nouser_always) [ "$a" = -nouser ] && { a=-links; f1=+0; } ;;
			nouser_never) [ "$a" = -nouser ] && { a=-links; f1=0; } ;;
			nogroup_always) [ "$a" = -nogroup ] && { a=-links; f1=+0; } ;;
			nogroup_never) [ "$a" = -nogroup ] && { a=-links; f1=0; } ;;
			# ---- 014 -exec ;
			exec_semi_true)
				case $a in
				-exec | -ok) b1='(' ;;
				esac
				[ "$term" = semi ] && { f1=-o f2=-links f3=+0 f4=')'; } ;;
			exec_semi_false)
				[ "$term" = semi ] && { f1=-links f2=0; } ;;
			exec_semi_batch) [ "$term" = semi ] && [ "$prev" = '{}' ] && a=+ ;;
			exec_arg_dot) [ "$inexec" = 1 ] && [ "$a" = '{}' ] && a=. ;;
			exec_twice | exec_skips | exec_status_ignored | exec_status_inverted | exec_status_nonzero)
				if [ "$a" = -exec ]; then
					case $M in
					exec_twice) pcf_w='"$@"; "$@"' ;;
					exec_skips) pcf_w='exit 0' ;;
					exec_status_ignored) pcf_w='"$@"; exit 0' ;;
					exec_status_inverted) pcf_w='"$@" && exit 1; exit 0' ;;
					exec_status_nonzero) pcf_w='"$@"; exit 1' ;;
					esac
					f1=sh f2=-c f3=$pcf_w f4=sh
				fi ;;
			# ---- 015 -exec +
			plus_false) [ "$term" = plus ] && { f1=-links f2=0; } ;;
			plus_fail_ignored | plus_skips | plus_cwd | plus_drops_last | plus_reversed | plus_single | plus_dup_last)
				if [ "$a" = -exec ]; then
					# count the words of the utility and its arguments before the {}
					pcf_k=0
					pcf_i=0
					pcf_seen=
					for pcf_w in "$@"; do
						pcf_i=$((pcf_i + 1))
						[ "$pcf_i" -gt "$n" ] && break
						[ "$pcf_w" = '{}' ] && { pcf_seen=1; break; }
						pcf_k=$((pcf_k + 1))
					done
					[ -z "$pcf_seen" ] && pcf_k=0
					case $M in
					plus_fail_ignored) pcf_w='k=$1; shift; "$@"; exit 0' ;;
					plus_skips) pcf_w='exit 0' ;;
					plus_cwd) pcf_w='k=$1; shift; for l; do :; done; d=${l%/*}; [ "$d" = "$l" ] && d=.; cd "$d" && exec "$@"' ;;
					plus_drops_last) pcf_w='k=$1; shift; printf "%s\n" "$@" | awk "{ a[NR] = \$0 } END { for (i = 1; i < NR; i++) print a[i] }" >"$PCF_DIR/args.$$"; set --; while IFS= read -r l; do set -- "$@" "$l"; done <"$PCF_DIR/args.$$"; rm -f "$PCF_DIR/args.$$"; exec "$@"' ;;
					plus_dup_last) pcf_w='k=$1; shift; for l; do :; done; exec "$@" "$l"' ;;
					plus_reversed) pcf_w='k=$1; shift; printf "%s\n" "$@" | awk -v k="$k" "{ a[NR] = \$0 } END { for (i = 1; i <= k; i++) print a[i]; for (i = NR; i > k; i--) print a[i] }" >"$PCF_DIR/args.$$"; set --; while IFS= read -r l; do set -- "$@" "$l"; done <"$PCF_DIR/args.$$"; rm -f "$PCF_DIR/args.$$"; exec "$@"' ;;
					plus_single) pcf_w='"$@" || : >"$PCF_DIR/fail"; exit 0'; pcf_k= ;;
					esac
					f1=sh f2=-c f3=$pcf_w f4=sh f5=$pcf_k
				fi
				if [ "$M" = plus_single ] && [ "$term" = plus ]; then
					a=';'
				fi ;;
			# ---- 016 -ok
			ok_*)
				if [ "$a" = -ok ]; then
					a=-exec
					f1=sh f2="$PCF_DIR/findok.sh" f3=${M#ok_}
				fi ;;
			# ---- 021 operators
			not_ignored) [ "$a" = '!' ] && drop=1 ;;
			paren_ignored) case $a in '(' | ')') drop=1 ;; esac ;;
			or_as_and) [ "$a" = -o ] && a=-a ;;
			and_as_or) [ "$a" = -a ] && a=-o ;;
			# ---- 022 implicit print
			# ---- 017 -print0
			# ---- 019 -depth 020 -prune
			depth_ignored) [ "$a" = -depth ] && drop=1 ;;
			prune_ignored) [ "$a" = -prune ] && { a=-links; f1=+0; } ;;
			prune_false) [ "$a" = -prune ] && { f1=-links f2=0; } ;;
			# ---- 018 -newer
			newer_ctime) [ "$a" = -newer ] && a=-cnewer ;;
			newer_atime) [ "$a" = -newer ] && a=-anewer ;;
			newer_not)
				if [ "$a" = -newer ]; then b1='!'; fi ;;
			newer_or_equal)
				if [ "$a" = -newer ]; then
					set -- "$@" '(' -newer "$nx" -o -name "${nx##*/}" ')'
					shift
					n=$((n - 1))
					prev=$nx
					continue
				fi ;;
			# ---- 024 -xdev 027 -mount
			xdev_false) [ "$a" = -xdev ] && { f1=-links f2=0; } ;;
			mount_false) [ "$a" = -mount ] && { f1=-links f2=0; } ;;
			xdev_prunes_all) [ "$a" = -xdev ] && { f1=-prune; } ;;
			mount_prunes_all) [ "$a" = -mount ] && { f1=-prune; } ;;
			esac
			case $a in
			-exec | -ok) inexec=1 execkind=exec; [ "$a" = -ok ] && execkind=ok ;;
			esac
		fi
		[ -n "$b1" ] && set -- "$@" "$b1"
		[ -n "$b2" ] && set -- "$@" "$b2"
		[ -n "$b3" ] && set -- "$@" "$b3"
		[ -z "$drop" ] && set -- "$@" "$a"
		[ -n "$f1" ] && set -- "$@" "$f1"
		[ -n "$f2" ] && set -- "$@" "$f2"
		[ -n "$f3" ] && set -- "$@" "$f3"
		[ -n "$f4" ] && set -- "$@" "$f4"
		[ -n "$f5" ] && set -- "$@" "$f5"
		[ -n "$f6" ] && set -- "$@" "$f6"
		[ -n "$f7" ] && set -- "$@" "$f7"
		[ -n "$f8" ] && set -- "$@" "$f8"
		if [ -n "$skip" ]; then
			shift
			n=$((n - 1))
			prev=$nx
		else
			prev=$a
		fi
		[ -n "$term" ] && inexec=
	done
	case $M in
	depth_always) set -- "$@" -depth ;;
	implicit_print_with_exec) [ -n "$pcf_ex" ] && [ -z "$pcf_pr" ] && set -- "$@" -print ;;
	implicit_print_ungrouped) [ -z "$pcf_ex$pcf_pr" ] && [ -n "$pcf_or" ] && set -- "$@" -print ;;
	esac
	pc_findrun "$@"
}

# pc_findloc: run with the locale categories find uses worked out by the
# rules of the internationalization variables, except for what M says.
pc_findloc() {
	pcf_all=${LC_ALL-}
	pcf_lang=${LANG-}
	pcf_allset=${LC_ALL+set}
	[ "$M" = loc_all_ignored ] && { pcf_all= pcf_allset=; }
	[ "$M" = loc_lang_ignored ] && pcf_lang=
	for pcf_c in LC_COLLATE LC_CTYPE LC_MESSAGES; do
		eval "pcf_v=\${$pcf_c-}"
		if [ -n "$pcf_all" ]; then
			pcf_e=$pcf_all
		elif [ "$M" = loc_all_null_wins ] && [ -n "$pcf_allset" ]; then
			pcf_e=
		elif [ -n "$pcf_v" ]; then
			pcf_e=$pcf_v
		elif [ -n "$pcf_lang" ]; then
			pcf_e=$pcf_lang
		else
			pcf_e=
		fi
		case "$M:$pcf_c" in
		loc_ignored:* | loc_collate_ignored:LC_COLLATE | loc_ctype_ignored:LC_CTYPE | loc_messages_ignored:LC_MESSAGES) pcf_e= ;;
		esac
		[ -z "$pcf_e" ] && pcf_e=POSIX
		eval "$pcf_c=\$pcf_e"
		export "$pcf_c"
	done
	unset LC_ALL LANG
}

# pc_findrun ARGS...: run the real find and shape what comes out.
pc_findrun() {
	out=$PCF_DIR/out.$$
	rm -f "$PCF_DIR/fail"
	case $M in
	err_no_diag | stderr_dropped) "$REAL" "$@" >"$out" 2>/dev/null ;;
	stderr_to_stdout) "$REAL" "$@" >"$out" 2>&1 ;;
	stdin_consumed) IFS= read -r pcf_x; "$REAL" "$@" >"$out" ;;
	*) "$REAL" "$@" >"$out" ;;
	esac
	rc=$?
	opnd=
	for pcf_x in "$@"; do
		case $pcf_x in
		-H | -L) ;;
		*) opnd=$pcf_x; break ;;
		esac
	done
	case $M in
	all_skip_root) sed 1d "$out" ;;
	all_skip_hidden) grep -v '/\.[^/]*$' "$out" ;;
	all_dup) sed p "$out" ;;
	all_no_descend)
		awk -v op="$opnd" 'BEGIN { b = gsub(/\//, "/", op); if (substr(op, length(op)) != "/") b++ }
			{ l = $0; c = gsub(/\//, "/", l); if (c <= b) print }' "$out" ;;
	all_skip_symlinks)
		while IFS= read -r l; do [ -L "$l" ] || printf '%s\n' "$l"; done <"$out" ;;
	all_skip_fifo)
		while IFS= read -r l; do [ -p "$l" ] || printf '%s\n' "$l"; done <"$out" ;;
	all_skip_empty_dir)
		while IFS= read -r l; do
			if [ -d "$l" ] && [ ! -L "$l" ] && [ -z "$(ls -A "$l")" ]; then :; else printf '%s\n' "$l"; fi
		done <"$out" ;;
	slash_doubled)
		awk -v op="$opnd" '{ if (length($0) > length(op) && substr($0, 1, length(op)) == op) print op "/" substr($0, length(op) + 1); else print }' "$out" ;;
	root_no_slash)
		awk -v op="$opnd" '{ if ($0 == op) { while (length($0) > 1 && substr($0, length($0)) == "/") $0 = substr($0, 1, length($0) - 1) } print }' "$out" ;;
	depth_root_only)
		# the operand comes last, the rest in the order without -depth
		if [ -n "$hasdepth" ]; then sed 1d "$out"; sed 1q "$out"; else cat "$out"; fi ;;
	print0_newline) tr '\000' '\n' <"$out" ;;
	print0_both) tr '\000' '\n' <"$out" | while IFS= read -r l; do printf '%s\n\000' "$l"; done ;;
	print0_double) tr '\000' '\n' <"$out" | while IFS= read -r l; do printf '%s\000\000' "$l"; done ;;
	print0_sep)
		tr '\000' '\n' <"$out" | { first=1; while IFS= read -r l; do [ -z "$first" ] && printf '\000'; printf '%s' "$l"; first=; done; } ;;
	print0_nothing) : ;;
	implicit_print_missing) [ -n "$pcf_ex$pcf_pr" ] && cat "$out" ;;
	dangling_dropped)
		while IFS= read -r l; do if [ -L "$l" ] && [ ! -e "$l" ]; then :; else printf '%s\n' "$l"; fi; done <"$out" ;;
	dangling_error)
		while IFS= read -r l; do
			if [ -L "$l" ] && [ ! -e "$l" ]; then echo "find: $l: dangling" >&2; rc=1; else printf '%s\n' "$l"; fi
		done <"$out" ;;
	order_reverse) awk -v depth="${hasdepth:+1}" -f "$PCF_DIR/findorder.awk" "$out" ;;
	*) cat "$out" ;;
	esac
	case $M in
	status_always0 | err_status0 | plus_fail_ignored_rc) rc=0 ;;
	status2) [ "$rc" -ne 0 ] && rc=2 ;;
	status255) [ "$rc" -ne 0 ] && rc=255 ;;
	esac
	if [ -f "$PCF_DIR/fail" ]; then
		rm -f "$PCF_DIR/fail"
		[ "$rc" -eq 0 ] && rc=1
	fi
	rm -f "$out"
	exit "$rc"
}
