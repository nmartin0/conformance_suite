# Lists the pathnames find wrote with the entries of every directory in
# the opposite (descending) order, parents still before their entries (after,
# with depth set): a hierarchy walked in another valid order.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
function sortdesc(arr, cnt,   i, j, x) {
	for (i = 2; i <= cnt; i++) {
		x = arr[i]
		for (j = i - 1; j >= 1 && arr[j] < x; j--) arr[j + 1] = arr[j]
		arr[j + 1] = x
	}
}
function emit(node,   i, c, tmp) {
	if (!depth) print node
	c = nch[node]
	for (i = 1; i <= c; i++) tmp[i] = kid[node, i]
	sortdesc(tmp, c)
	for (i = 1; i <= c; i++) emit(tmp[i])
	if (depth) print node
}
{
	line[NR] = $0
	seen[$0] = 1
}
END {
	for (i = 1; i <= NR; i++) {
		p = line[i]
		k = match(p, /\/[^\/]*$/)
		par = ""
		if (k > 1) {
			q = substr(p, 1, k - 1)
			if (q in seen) par = q
			else if ((q "/") in seen) par = q "/"
		}
		if (par == "") roots[++nr] = p
		else { nch[par]++; kid[par, nch[par]] = p }
	}
	for (i = 1; i <= nr; i++) emit(roots[i])
}
