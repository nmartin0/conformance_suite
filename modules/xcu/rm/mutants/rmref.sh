# Shared by the rm faults and variants: a small reimplementation of rm in
# which one thing can be changed. With no fault name (M=ref) it follows the
# text, so a fault differs from it in exactly one requirement.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_rm M ARGS...   M names the fault or variant; ARGS are rm's arguments.
#
# The removal of a single non-directory goes to the real utility, so that the
# diagnostic for a failure comes from the system: PC_RMBIN names it when the
# reimplementation runs as a program of its own (a function uses
# "command rm"). Directories are emptied by walking them with cd, so that no
# path name longer than a limit is ever needed; the fault pathbased walks by
# whole path names instead.
#
# Return codes of pc_file: 0 done, 1 error, 3 stop at once (status 1),
# 4 stop at once (status 0).

pc_real() {
	if [ -n "${PC_RMBIN-}" ]; then "$PC_RMBIN" "$@"; else command rm "$@"; fi
}

# pc_ask DISP : the question about DISP; returns 0 when the reply is affirmative
pc_ask() {
	case $M in
	i_no_prompt) return 0 ;;
	i_prompt_no_name) printf 'rm: remove the file? ' >&2 ;;
	i_prompt_stdout) printf 'rm: remove %s? ' "$1" ;;
	prompt_nl) printf 'Remove this entry: %s\nAnswer yes or no\n' "$1" >&2 ;;
	prompt_quoted) printf "rm: remove \"%s\" (y/n) " "$1" >&2 ;;
	prompt_abs) printf 'rm: remove %s/%s? ' "$PC_CWD" "$1" >&2 ;;
	*) printf 'rm: remove %s? ' "$1" >&2 ;;
	esac
	pc_ans=
	read -r pc_ans || pc_ans=
	case $M in i_yes_upper_only) case $pc_ans in Y*) return 0 ;; esac; return 1 ;; esac
	case $pc_ans in [yY]*) return 0 ;; esac
	return 1
}

# pc_msg DISP DEPTH : the -v message
pc_msg() {
	if [ "$M" = v_always ]; then printf 'removed %s\n' "$1"; return 0; fi
	[ -n "$v" ] || return 0
	case $M in
	v_none) return 0 ;;
	v_first_only) [ -n "${PC_FIRST-}" ] || return 0 ;;
	v_top_only) [ "$2" -eq 0 ] || return 0 ;;
	v_stderr) printf 'removed %s\n' "$1" >&2; return 0 ;;
	v_no_name) printf 'removed\n'; return 0 ;;
	v_quoted) printf "removed '%s'\n" "$1"; return 0 ;;
	v_two_lines) printf 'removal done\nname: %s\n' "$1"; return 0 ;;
	esac
	printf 'removed %s\n' "$1"
}

# pc_diag TEXT : a diagnostic
pc_diag() {
	printf 'rm: %s\n' "$1" >&2
}

# pc_file REL DISP DEPTH : one directory entry, as if it were an operand
pc_file() (
	rel=$1 disp=$2 depth=$3
	if [ ! -e "$rel" ] && [ ! -L "$rel" ]; then
		if [ -n "$f" ]; then
			case $M in
			f_diag) pc_diag "$disp: no such file" ;;
			f_status1) return 1 ;;
			f_aborts) return 4 ;;
			esac
			return 0
		fi
		case $M in
		v_names_failed) [ -z "$v" ] || printf 'removed %s\n' "$disp" ;;
		esac
		case $M in
		nonexist_silent) ;;
		nonexist_diag_stdout) printf 'rm: %s: no such file\n' "$disp" ;;
		*) pc_diag "$disp: no such file" ;;
		esac
		case $M in
		nonexist_status0) return 0 ;;
		nonexist_aborts) return 3 ;;
		esac
		return 1
	fi
	isdir=
	if [ -d "$rel" ]; then isdir=1; fi
	if [ -n "$isdir" ] && [ -L "$rel" ] && [ "$M" != follow_links ]; then isdir=; fi
	if [ -n "$isdir" ]; then
		if [ -z "$r" ] && [ -z "$d" ]; then
			case $M in
			dir_removed_empty) rmdir -- "$rel" 2>/dev/null && return 0 ;;
			dir_recursive) pc_real -rf -- "$rel"; return 0 ;;
			dir_no_diag) return 1 ;;
			dir_diag_stdout) printf 'rm: %s: is a directory\n' "$disp"; return 1 ;;
			dir_status0) pc_diag "$disp: is a directory"; return 0 ;;
			dir_aborts) pc_diag "$disp: is a directory"; return 3 ;;
			esac
			pc_diag "$disp: is a directory"
			return 1
		fi
		if [ -z "$r" ]; then
			# -d alone: only an empty directory
			case $M in
			d_empty_refused) pc_diag "$disp: is a directory"; return 1 ;;
			d_nonempty_removed) pc_real -rf -- "$rel"; pc_msg "$disp" "$depth"; return 0 ;;
			esac
			if [ -n "$i" ]; then pc_ask "$disp" || return 0; fi
			if ! rmdir -- "$rel" 2>/dev/null; then
				case $M in
				d_nonempty_status0) pc_diag "$disp: directory not empty"; return 0 ;;
				d_nonempty_no_diag) return 1 ;;
				esac
				pc_diag "$disp: directory not empty"
				return 1
			fi
			pc_msg "$disp" "$depth"
			if [ "$M" = d_status1 ]; then return 1; fi
			return 0
		fi
		# -r or -R
		if [ -n "$i" ]; then pc_ask "$disp" || return 0; fi
		pc_tree "$rel" "$disp" "$depth" || {
			pc_diag "$disp: cannot remove everything in it"
			return 1
		}
		if [ "$M" = R_leaves_top ] && [ "$depth" -eq 0 ]; then return 0; fi
		if [ -n "$i" ]; then pc_ask "$disp" || return 0; fi
		if ! rmdir -- "$rel" 2>/dev/null; then
			if [ "$M" = follow_links ] && [ -L "$rel" ]; then
				pc_real -- "$rel" || return 1
				pc_msg "$disp" "$depth"
				return 0
			fi
			pc_diag "$disp: cannot remove"
			return 1
		fi
		pc_msg "$disp" "$depth"
		if [ "$M" = R_status1 ] && [ "$depth" -eq 0 ]; then return 1; fi
		return 0
	fi
	# not a directory (or a link to one)
	if [ -n "$i" ]; then
		if ! pc_ask "$disp"; then
			case $M in
			i_neg_removes) ;;
			i_neg_status1) return 1 ;;
			*) return 0 ;;
			esac
		fi
		if [ "$M" = i_yes_ignored ]; then return 0; fi
	fi
	if [ "$M" = d_file_refused ] && [ -n "$d" ] && [ -z "$r" ]; then
		pc_diag "$disp: not a directory"
		return 1
	fi
	if [ "$M" = link_target_removed ] && [ -L "$rel" ] && [ -f "$rel" ]; then
		pc_real -- "$(readlink -- "$rel")" 2>/dev/null
	fi
	if ! pc_msgtxt=$(pc_real -- "$rel" 2>&1); then
		case $M in
		perm_silent) return 1 ;;
		perm_status0) printf '%s\n' "$pc_msgtxt" >&2; return 0 ;;
		perm_aborts) printf '%s\n' "$pc_msgtxt" >&2; return 3 ;;
		perm_diag_stdout) printf '%s\n' "$pc_msgtxt"; return 1 ;;
		perm_f_silent) if [ -n "$f" ]; then return 1; fi ;;
		perm_f_status0) if [ -n "$f" ]; then printf '%s\n' "$pc_msgtxt" >&2; return 0; fi ;;
		esac
		printf '%s\n' "$pc_msgtxt" >&2
		return 1
	fi
	pc_msg "$disp" "$depth"
	if [ "$M" = status1_ok ]; then return 1; fi
	return 0
)

# pc_tree REL DISP DEPTH : remove what the directory holds
pc_tree() (
	rel=$1 disp=$2 depth=$3
	st=0
	nd=$((depth + 1))
	if [ "$M" = depth_limit ] && [ "$nd" -gt 40 ]; then return 1; fi
	if [ "$M" = pathbased ]; then
		set --
		for e in "$disp"/* "$disp"/.[!.]* "$disp"/..?*; do
			[ -e "$e" ] || [ -L "$e" ] || continue
			set -- "$e" "$@"
		done
		for e; do
			pc_file "$e" "$e" "$nd" || st=1
		done
		return $st
	fi
	cd -P -- "$rel" || return 1
	set --
	for e in * .[!.]* ..?*; do
		[ -e "$e" ] || [ -L "$e" ] || continue
		case $M in
		R_no_dotfiles) case $e in .*) continue ;; esac ;;
		R_no_empty_dirs) if [ -d "$e" ] && [ ! -L "$e" ]; then
				pc_empty=1
				for pc_x in "$e"/* "$e"/.[!.]* "$e"/..?*; do
					if [ -e "$pc_x" ] || [ -L "$pc_x" ]; then pc_empty=; fi
				done
				[ -z "$pc_empty" ] || continue
			fi ;;
		R_shallow) if [ -d "$e" ] && [ ! -L "$e" ]; then continue; fi ;;
		esac
		set -- "$e" "$@"
	done
	if [ "$M" = reverse_order ]; then
		for e; do
			pc_file "$e" "$disp/$e" "$nd" || st=1
		done
	else
		# the list was built backwards: turn it round
		pc_n=$#
		pc_k=0
		while [ "$pc_k" -lt "$pc_n" ]; do
			pc_k=$((pc_k + 1))
			eval "pc_e=\${$((pc_n - pc_k + 1))}"
			pc_file "$pc_e" "$disp/$pc_e" "$nd" || st=1
		done
	fi
	return $st
)

pc_rm() {
	M=$1
	shift
	if [ "$M" = noop ]; then return 0; fi
	PC_CWD=$(pwd)
	f= i= r= d= v= rl=
	while [ $# -gt 0 ]; do
		case $1 in
		--) shift; break ;;
		-?*)
			pc_o=${1#-}
			while [ -n "$pc_o" ]; do
				pc_c=${pc_o%"${pc_o#?}"}
				pc_o=${pc_o#?}
				case $pc_c in
				f)
					f=1
					if [ "$M" != f_after_i_ignored ]; then i=; fi
					;;
				i)
					if [ "$M" = i_after_f_ignored ] && [ -n "$f" ]; then :; else i=1; f=; fi
					;;
				r | R) r=1 rl=$pc_c ;;
				d) d=1 ;;
				v) v=1 ;;
				*) printf 'rm: unknown option -%s\n' "$pc_c" >&2; return 2 ;;
				esac
			done
			shift
			;;
		*) break ;;
		esac
	done
	if [ "$M" = r_ignored ] && [ "$rl" = r ]; then r=; fi
	if [ "$M" = R_ignored ] && [ "$rl" = R ]; then r=; fi
	if [ "$M" = f_after_i_ignored ] && [ -n "$f" ] && [ -n "$i" ]; then f=; fi
	export M f i r d v rl PC_CWD
	if [ $# -eq 0 ]; then
		if [ -z "$f" ]; then
			pc_diag "usage: rm [-diRrv] file..."
			return 1
		fi
		case $M in
		f_noargs_status1) return 1 ;;
		f_noargs_diag) pc_diag "usage: rm [-diRrv] file..."; return 0 ;;
		esac
		return 0
	fi
	st=0
	pc_first=1
	for a; do
		PC_FIRST=
		if [ -n "$pc_first" ]; then PC_FIRST=1; fi
		export PC_FIRST
		pc_first=
		pc_base=${a%"${a##*[!/]}"}
		pc_last=${pc_base##*/}
		pc_isdot=
		case $pc_last in . | ..) pc_isdot=1 ;; esac
		[ -n "$pc_base" ] || pc_isdot=1
		case $M in
		dot_trailing_unchecked) case $a in . | ..) ;; *) pc_isdot= ;; esac ;;
		dotdot_unchecked) [ "$pc_last" != .. ] || pc_isdot= ;;
		dot_unchecked) [ "$pc_last" != . ] || pc_isdot= ;;
		esac
		if [ -n "$pc_isdot" ]; then
			case $M in
			dot_removed)
				if [ "$a" = . ]; then pc_real -rf -- ./* ./.[!.]* 2>/dev/null; return 0; fi ;;
			dotdot_removed)
				if [ "$a" = .. ]; then pc_real -rf -- ../* ../.[!.]* 2>/dev/null; return 0; fi ;;
			dot_no_diag) st=1; continue ;;
			dot_status0) pc_diag "$a: refused"; continue ;;
			dot_diag_stdout) printf 'rm: %s: refused\n' "$a"; st=1; continue ;;
			esac
			pc_diag "$a: refused"
			st=1
			continue
		fi
		pc_file "$a" "$a" 0
		pc_rc=$?
		if [ "$M" = only_first ]; then return $pc_rc; fi
		case $pc_rc in
		0) ;;
		3) return 1 ;;
		4) return 0 ;;
		*) st=1 ;;
		esac
	done
	if [ "$M" = extra_removed ]; then pc_real -f -- ? 2>/dev/null; fi
	case $M in
	status_last_only) return $pc_rc ;;
	ref_status2) if [ "$st" -ne 0 ]; then return 2; fi ;;
	ref_status255) if [ "$st" -ne 0 ]; then return 255; fi ;;
	esac
	return $st
}
