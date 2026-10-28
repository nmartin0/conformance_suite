# A file that does not exist and is created by a utility or a shell redirection is a regular file of length zero owned by the effective user, with group the effective group or the directory's group, and mode 0666 less the umask; a created directory is empty with mode 0777 less the umask, a created FIFO has mode 0666 less the umask, and the file times are current
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.1-relationship.003
. "$PC_CORE/pc.sh"
pc_begin XCU.1.1-relationship.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
umask 027
uid=$(id -u) || pc_unresolved "id -u"
egid=$(id -g) || pc_unresolved "id -g"
set -- $(ls -ldn .) || pc_unresolved "ls -ldn ."
dgid=$4
: >ref || pc_unresolved "cannot create the reference file"
pc_test
sleep 2
touch t1
tee t2 </dev/null >/dev/null
: >t3
dd if=/dev/null of=t4 2>/dev/null
sort -o t5 /dev/null
sed -n 'w t6' /dev/null
awk 'BEGIN { printf "" > "t7" }'
mkdir d1
mkfifo p1
look() {
	# look file type mode
	f=$1
	[ -e "$f" ] || pc_fail "$f was not created"
	case $2 in
	f) [ -f "$f" ] || pc_fail "$f is not a regular file" ;;
	d) [ -d "$f" ] || pc_fail "$f is not a directory" ;;
	p) [ -p "$f" ] || pc_fail "$f is not a FIFO" ;;
	esac
	m=$(ls -ldn "$f" | cut -c 1-10)
	[ "$m" = "$3" ] || pc_fail "$f has mode $m, wanted $3"
	set -- $(ls -ldn "$f")
	[ "$3" = "$uid" ] || pc_fail "$f is owned by $3, not by $uid"
	{ [ "$4" = "$egid" ] || [ "$4" = "$dgid" ]; } || pc_fail "$f has group $4, wanted $egid or $dgid"
	[ "$(find "$f" -newer ref -print)" = "$f" ] || pc_fail "$f has an old modification time"
	[ "$(find "$f" -mtime -1 -atime -1 -ctime -1 -print)" = "$f" ] || pc_fail "$f has an old time"
}
for f in t1 t2 t3 t4 t5 t6 t7; do
	look $f f -rw-r-----
	[ ! -s "$f" ] || pc_fail "$f is not empty"
done
look d1 d drwxr-x---
[ -z "$(ls -A d1)" ] || pc_fail "the new directory is not empty"
look p1 p prw-r-----
pc_pass
