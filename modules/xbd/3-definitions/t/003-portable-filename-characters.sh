# Every portable filename character can be used in a file name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.3-definitions.003
. "$PC_CORE/pc.sh"
pc_begin XBD.3-definitions.003
LC_ALL=POSIX
export LC_ALL
pc_test
tryname() {
	n=$1
	printf '%s\n' "$n" >"./$n" || pc_fail "cannot create a file named '$n'"
	[ -f "./$n" ] || pc_fail "'$n' is not a regular file after it was created"
	line=
	read -r line <"./$n" || pc_fail "cannot read back the file named '$n'"
	[ "$line" = "$n" ] || pc_fail "the file named '$n' holds '$line'"
	listed=$(ls -d -- "./$n" 2>/dev/null </dev/null)
	[ "$listed" = "./$n" ] || pc_fail "ls -d did not list the file named '$n' ('$listed')"
	rm -- "./$n" 2>/dev/null </dev/null || pc_fail "cannot remove the file named '$n'"
	[ ! -e "./$n" ] || pc_fail "the file named '$n' exists after rm"
}
for n in A B C D E F G H I J K L M N O P Q R S T U V W X Y Z \
	a b c d e f g h i j k l m n o p q r s t u v w x y z \
	0 1 2 3 4 5 6 7 8 9 _ - \
	.a ..a a. a..b a.b a_b a-b _a -a x_ x- Zz9.A_a-0.b_c1; do
	tryname "$n"
done
pc_pass
