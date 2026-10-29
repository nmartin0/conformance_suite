# tc.sh - helpers for the toolchain tests of the conventions suite (POSIX sh).
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Source after pc.sh:
#
#     . "$PC_CORE/pc.sh"
#     . "${PC_CORE%/core}/conv/lib/tc.sh"
#     tc_begin CONV.toolchain.001
#     tc_need CC AS
#     ...
#
# A test works in its own directory below the working directory the driver
# gives it and removes that directory when it ends. The tools are found with
# "command -v" (c17 then cc for the compiler; as, ld, ar); the environment
# variables PC_CONV_CC, PC_CONV_AS, PC_CONV_LD and PC_CONV_AR name a tool
# explicitly, which is how conv/neg/run.sh substitutes tools that are
# deliberately wrong. Compile and link options beyond the POSIX compiler
# utility (-shared, -fPIC, -Wl,...) are probed once per run (answers kept next
# to the driver's result files) and the tests
# that depend on them report NOTINUSE when the compiler refuses them.

tc_lib=${PC_CORE%/core}/conv/lib
tc_root=$PWD

tc_find() { # NAME cmd... : $PC_CONV_NAME, else the first command found
	tc_n=$1
	shift
	eval "tc_o=\${PC_CONV_$tc_n:-}"
	if [ -n "$tc_o" ]; then
		[ -x "$tc_o" ] && printf '%s\n' "$tc_o"
		return 0
	fi
	for tc_c in "$@"; do
		tc_p=$(command -v "$tc_c" 2>/dev/null) || continue
		case $tc_p in /*) printf '%s\n' "$tc_p"; return 0 ;; esac
	done
	return 0
}

tc_cleanup() {
	cd "$tc_root" 2>/dev/null
	rm -rf "$tc_dir"
}

tc_begin() { # ID
	pc_begin "$1"
	tc_dir=$tc_root/tc.$$
	mkdir "$tc_dir" || pc_unresolved "cannot create a private directory"
	trap tc_cleanup EXIT
	trap 'exit 1' HUP INT TERM
	cd "$tc_dir" || pc_unresolved "cannot enter the private directory"
	CC=$(tc_find CC c17 cc)
	AS=$(tc_find AS as)
	LD=$(tc_find LD ld)
	AR=$(tc_find AR ar)
	export CC AS LD AR
}

tc_need() { # CC AS LD AR ... : NOTINUSE when a tool is absent
	for tc_x in "$@"; do
		eval "tc_v=\${$tc_x:-}"
		[ -n "$tc_v" ] || pc_notinuse "no $tc_x on this system (found with command -v)"
	done
}

# Show the first lines of the last captured output.
tc_out() {
	head -n 4 "$tc_dir/last.out" 2>/dev/null | tr '\n' ' ' | cut -c 1-300
}

# tc_do cmd... : run quietly, capture output, return the status.
tc_do() {
	"$@" >"$tc_dir/last.out" 2>&1
}

# tc_must "what" cmd... : like tc_do, but a failure is a problem of the setup.
tc_must() {
	tc_w=$1
	shift
	tc_do "$@" || pc_fail "$tc_w did not work: $(tc_out)"
}

# ---- probes (cached for the run in the driver's working directory) ----

tc_probe() { # NAME : result in $tc_pr
	# the driver's per-run directory (where the result file lives) keeps the
	# answers for the other tests of the run; without it they are recomputed
	tc_pd=${PC_RESULT%/*}
	[ -d "$tc_pd" ] && [ -w "$tc_pd" ] || tc_pd=$tc_dir
	tc_pf=$tc_pd/tc-probe.$1
	if [ -f "$tc_pf" ] && read -r tc_pr <"$tc_pf"; then
		return 0
	fi
	mkdir "$tc_dir/probe-$1" || pc_unresolved "cannot create a probe directory"
	tc_pr=$(cd "$tc_dir/probe-$1" && "tc_probe_$1")
	[ -n "$tc_pr" ] || tc_pr=no
	printf '%s\n' "$tc_pr" >"$tc_pf.$$" && mv "$tc_pf.$$" "$tc_pf"
}

# can the compiler make a shared object from position-independent code?
tc_probe_pic() {
	printf 'int tcp(void){return 1;}\n' >p.c
	"$CC" -fPIC -shared -o p.so p.c >/dev/null 2>&1 && echo yes
}

# libraries needed to call dlopen: "-" for none, or "-ldl"
tc_probe_dl() {
	cat >p.c <<'EOT'
#include <dlfcn.h>
int main(void) { void *h = dlopen("./nonexistent.so", RTLD_NOW); return h != 0 && dlsym(h, "x") != 0 && dlclose(h) != 0; }
EOT
	if "$CC" -o p p.c >/dev/null 2>&1; then echo -; elif "$CC" -o p p.c -ldl >/dev/null 2>&1; then echo -ldl; fi
}

# -Wl,-rpath,... accepted and the result runs
tc_probe_rpath() {
	printf 'int main(void){return 0;}\n' >p.c
	"$CC" -o p p.c -Wl,-rpath,/tc-probe-absent >/dev/null 2>&1 && ./p && echo yes
}

# -Wl,-soname,... accepted when making a shared object
tc_probe_soname() {
	printf 'int tcp(void){return 1;}\n' >p.c
	"$CC" -fPIC -shared -o p.so p.c -Wl,-soname,libtcp.so.1 >/dev/null 2>&1 && echo yes
}

# the linker links a trivial object into a relocatable object
tc_probe_ld() {
	printf 'int tcp(void){return 1;}\n' >p.c
	"$CC" -c -o p.o p.c >/dev/null 2>&1 && "$LD" -r -o q.o p.o >/dev/null 2>&1 && echo yes
}

# tc_need_probe NAME "what" : NOTINUSE unless the probe said yes
tc_need_probe() {
	tc_probe "$1"
	[ "$tc_pr" = yes ] || pc_notinuse "$2 is refused by the compiler or linker of this system"
}

tc_need_dl() { # sets TC_DL to the extra library flags (possibly empty)
	tc_probe dl
	case $tc_pr in
	-) TC_DL= ;;
	-ldl) TC_DL=-ldl ;;
	*) pc_notinuse "dlopen cannot be linked with this compiler" ;;
	esac
}

tc_need_ld() {
	tc_need LD CC AR
	tc_need_probe ld "linking a trivial object with the linker command"
}

# ---- object file reading ----

tc_dump() { # FILE > FILE.dump (the reader is built once per test)
	if [ ! -x "$tc_dir/elfdump" ]; then
		tc_do "$CC" -o "$tc_dir/elfdump" "$tc_lib/elfdump.c" || pc_fail "the object reader did not compile: $(tc_out)"
	fi
	"$tc_dir/elfdump" "$1" >"$1.dump" 2>/dev/null || pc_fail "$1 is not a readable object file"
}

# tc_sym FILE NAME : the .symtab record of a symbol ("" when absent)
tc_sym() {
	awk -v n="$2" '$1 == "SYM" && $2 == ".symtab" && $4 == n { print; exit }' "$1.dump"
}

tc_symdyn() { # FILE NAME : the .dynsym record
	awk -v n="$2" '$1 == "SYM" && $2 == ".dynsym" && $4 == n { print; exit }' "$1.dump"
}

tc_key() { # LINE KEY : value of KEY=value in a record
	printf '%s\n' "$1" | tr ' ' '\n' | sed -n "s/^$2=//p"
}

tc_sec() { # FILE NAME : the SEC record of a section
	awk -v n="$2" '$1 == "SEC" && $3 == n { print; exit }' "$1.dump"
}

tc_type() { # FILE : REL, EXEC or DYN
	sed -n '1s/.* type=\([A-Za-z0-9]*\) .*/\1/p' "$1.dump"
}

tc_dyn() { # FILE TAG : strings of the dynamic entries with that tag, one per line
	awk -v t="$2" '$1 == "DYN" && $2 == t { print $4 }' "$1.dump"
}

# ---- building ----

tc_cobj() { # NAME.c : compile NAME.c to NAME.o (extra options after)
	tc_cn=$1
	shift
	tc_must "compiling $tc_cn" "$CC" -c -o "${tc_cn%.c}.o" "$tc_cn" "$@"
}

tc_aobj() { # NAME.s : assemble NAME.s to NAME.o
	tc_must "assembling $1" "$AS" -o "${1%.s}.o" "$1"
}

# tc_chk_sec FILE NAME TYPE REQUIRED ABSENT : fail unless the section exists with
# the type, has every attribute letter in REQUIRED and none of those in ABSENT.
tc_chk_sec() {
	tc_l=$(tc_sec "$1" "$2")
	[ -n "$tc_l" ] || pc_fail "no section named $2"
	set -- "$1" "$2" "$3" "$4" "$5" $tc_l
	[ "$9" = "$3" ] || pc_fail "section $2 has type $9, not $3"
	for tc_c in $(printf '%s' "$4" | sed 's/./& /g'); do
		case ${10} in *"$tc_c"*) ;; *) pc_fail "section $2 lacks attribute $tc_c (has ${10})" ;; esac
	done
	for tc_c in $(printf '%s' "$5" | sed 's/./& /g'); do
		case ${10} in *"$tc_c"*) pc_fail "section $2 has attribute $tc_c (has ${10})" ;; esac
	done
}

# The size of a pointer in bytes (probe "ptr"), in $tc_pr.
tc_probe_ptr() {
	printf '#include <stdio.h>\nint main(void) { printf("%%d\\n", (int)sizeof(void *)); return 0; }\n' >p.c
	"$CC" -o p p.c >/dev/null 2>&1 && ./p
}
