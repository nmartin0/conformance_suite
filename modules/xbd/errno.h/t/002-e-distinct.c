/*
 * The E* values are pairwise distinct apart from the two permitted pairs.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.errno.h.002
 */
#include <errno.h>
#include <string.h>
#include "pc.h"

int main(void)
{
	static const struct { const char *name; int val; } e[] = {
		{ "E2BIG", E2BIG },
		{ "EACCES", EACCES },
		{ "EADDRINUSE", EADDRINUSE },
		{ "EADDRNOTAVAIL", EADDRNOTAVAIL },
		{ "EAFNOSUPPORT", EAFNOSUPPORT },
		{ "EAGAIN", EAGAIN },
		{ "EALREADY", EALREADY },
		{ "EBADF", EBADF },
		{ "EBADMSG", EBADMSG },
		{ "EBUSY", EBUSY },
		{ "ECANCELED", ECANCELED },
		{ "ECHILD", ECHILD },
		{ "ECONNABORTED", ECONNABORTED },
		{ "ECONNREFUSED", ECONNREFUSED },
		{ "ECONNRESET", ECONNRESET },
		{ "EDEADLK", EDEADLK },
		{ "EDESTADDRREQ", EDESTADDRREQ },
		{ "EDOM", EDOM },
		{ "EDQUOT", EDQUOT },
		{ "EEXIST", EEXIST },
		{ "EFAULT", EFAULT },
		{ "EFBIG", EFBIG },
		{ "EHOSTUNREACH", EHOSTUNREACH },
		{ "EIDRM", EIDRM },
		{ "EILSEQ", EILSEQ },
		{ "EINPROGRESS", EINPROGRESS },
		{ "EINTR", EINTR },
		{ "EINVAL", EINVAL },
		{ "EIO", EIO },
		{ "EISCONN", EISCONN },
		{ "EISDIR", EISDIR },
		{ "ELOOP", ELOOP },
		{ "EMFILE", EMFILE },
		{ "EMLINK", EMLINK },
		{ "EMSGSIZE", EMSGSIZE },
		{ "EMULTIHOP", EMULTIHOP },
		{ "ENAMETOOLONG", ENAMETOOLONG },
		{ "ENETDOWN", ENETDOWN },
		{ "ENETRESET", ENETRESET },
		{ "ENETUNREACH", ENETUNREACH },
		{ "ENFILE", ENFILE },
		{ "ENOBUFS", ENOBUFS },
		{ "ENODEV", ENODEV },
		{ "ENOENT", ENOENT },
		{ "ENOEXEC", ENOEXEC },
		{ "ENOLCK", ENOLCK },
		{ "ENOLINK", ENOLINK },
		{ "ENOMEM", ENOMEM },
		{ "ENOMSG", ENOMSG },
		{ "ENOPROTOOPT", ENOPROTOOPT },
		{ "ENOSPC", ENOSPC },
		{ "ENOSYS", ENOSYS },
		{ "ENOTCONN", ENOTCONN },
		{ "ENOTDIR", ENOTDIR },
		{ "ENOTEMPTY", ENOTEMPTY },
		{ "ENOTRECOVERABLE", ENOTRECOVERABLE },
		{ "ENOTSOCK", ENOTSOCK },
		{ "ENOTSUP", ENOTSUP },
		{ "ENOTTY", ENOTTY },
		{ "ENXIO", ENXIO },
		{ "EOPNOTSUPP", EOPNOTSUPP },
		{ "EOVERFLOW", EOVERFLOW },
		{ "EOWNERDEAD", EOWNERDEAD },
		{ "EPERM", EPERM },
		{ "EPIPE", EPIPE },
		{ "EPROTO", EPROTO },
		{ "EPROTONOSUPPORT", EPROTONOSUPPORT },
		{ "EPROTOTYPE", EPROTOTYPE },
		{ "ERANGE", ERANGE },
		{ "EROFS", EROFS },
		{ "ESOCKTNOSUPPORT", ESOCKTNOSUPPORT },
		{ "ESPIPE", ESPIPE },
		{ "ESRCH", ESRCH },
		{ "ESTALE", ESTALE },
		{ "ETIMEDOUT", ETIMEDOUT },
		{ "ETXTBSY", ETXTBSY },
		{ "EWOULDBLOCK", EWOULDBLOCK },
		{ "EXDEV", EXDEV },
	};
	size_t i, j, n = sizeof e / sizeof e[0];

	pc_begin("XBD.errno.h.002");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			const char *a = e[i].name, *b = e[j].name;
			int allowed = (!strcmp(a, "EAGAIN") && !strcmp(b, "EWOULDBLOCK")) ||
			    (!strcmp(a, "EWOULDBLOCK") && !strcmp(b, "EAGAIN")) ||
			    (!strcmp(a, "ENOTSUP") && !strcmp(b, "EOPNOTSUPP")) ||
			    (!strcmp(a, "EOPNOTSUPP") && !strcmp(b, "ENOTSUP"));
			pc_check(allowed || e[i].val != e[j].val, "%s and %s have the same value (%d)", a, b, e[i].val);
		}
	pc_pass();
}
