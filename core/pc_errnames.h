/*
 * pc_errnames.h - errno names for pc_errname().
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * The list is the set of error names in XSH 2.3 Error Numbers
 * (POSIX.1-2024). E2BIG is listed last so that duplicates such as
 * EAGAIN/EWOULDBLOCK resolve to the name listed first.
 */
#ifdef EACCES
	{ EACCES, "EACCES" },
#endif
#ifdef EADDRINUSE
	{ EADDRINUSE, "EADDRINUSE" },
#endif
#ifdef EADDRNOTAVAIL
	{ EADDRNOTAVAIL, "EADDRNOTAVAIL" },
#endif
#ifdef EAFNOSUPPORT
	{ EAFNOSUPPORT, "EAFNOSUPPORT" },
#endif
#ifdef EAGAIN
	{ EAGAIN, "EAGAIN" },
#endif
#ifdef EALREADY
	{ EALREADY, "EALREADY" },
#endif
#ifdef EBADF
	{ EBADF, "EBADF" },
#endif
#ifdef EBADMSG
	{ EBADMSG, "EBADMSG" },
#endif
#ifdef EBUSY
	{ EBUSY, "EBUSY" },
#endif
#ifdef ECANCELED
	{ ECANCELED, "ECANCELED" },
#endif
#ifdef ECHILD
	{ ECHILD, "ECHILD" },
#endif
#ifdef ECONNABORTED
	{ ECONNABORTED, "ECONNABORTED" },
#endif
#ifdef ECONNREFUSED
	{ ECONNREFUSED, "ECONNREFUSED" },
#endif
#ifdef ECONNRESET
	{ ECONNRESET, "ECONNRESET" },
#endif
#ifdef EDEADLK
	{ EDEADLK, "EDEADLK" },
#endif
#ifdef EDESTADDRREQ
	{ EDESTADDRREQ, "EDESTADDRREQ" },
#endif
#ifdef EDOM
	{ EDOM, "EDOM" },
#endif
#ifdef EDQUOT
	{ EDQUOT, "EDQUOT" },
#endif
#ifdef EEXIST
	{ EEXIST, "EEXIST" },
#endif
#ifdef EFAULT
	{ EFAULT, "EFAULT" },
#endif
#ifdef EFBIG
	{ EFBIG, "EFBIG" },
#endif
#ifdef EHOSTUNREACH
	{ EHOSTUNREACH, "EHOSTUNREACH" },
#endif
#ifdef EIDRM
	{ EIDRM, "EIDRM" },
#endif
#ifdef EILSEQ
	{ EILSEQ, "EILSEQ" },
#endif
#ifdef EINPROGRESS
	{ EINPROGRESS, "EINPROGRESS" },
#endif
#ifdef EINTR
	{ EINTR, "EINTR" },
#endif
#ifdef EINVAL
	{ EINVAL, "EINVAL" },
#endif
#ifdef EIO
	{ EIO, "EIO" },
#endif
#ifdef EISCONN
	{ EISCONN, "EISCONN" },
#endif
#ifdef EISDIR
	{ EISDIR, "EISDIR" },
#endif
#ifdef ELOOP
	{ ELOOP, "ELOOP" },
#endif
#ifdef EMFILE
	{ EMFILE, "EMFILE" },
#endif
#ifdef EMLINK
	{ EMLINK, "EMLINK" },
#endif
#ifdef EMSGSIZE
	{ EMSGSIZE, "EMSGSIZE" },
#endif
#ifdef EMULTIHOP
	{ EMULTIHOP, "EMULTIHOP" },
#endif
#ifdef ENAMETOOLONG
	{ ENAMETOOLONG, "ENAMETOOLONG" },
#endif
#ifdef ENETDOWN
	{ ENETDOWN, "ENETDOWN" },
#endif
#ifdef ENETRESET
	{ ENETRESET, "ENETRESET" },
#endif
#ifdef ENETUNREACH
	{ ENETUNREACH, "ENETUNREACH" },
#endif
#ifdef ENFILE
	{ ENFILE, "ENFILE" },
#endif
#ifdef ENOBUFS
	{ ENOBUFS, "ENOBUFS" },
#endif
#ifdef ENODEV
	{ ENODEV, "ENODEV" },
#endif
#ifdef ENOENT
	{ ENOENT, "ENOENT" },
#endif
#ifdef ENOEXEC
	{ ENOEXEC, "ENOEXEC" },
#endif
#ifdef ENOLCK
	{ ENOLCK, "ENOLCK" },
#endif
#ifdef ENOLINK
	{ ENOLINK, "ENOLINK" },
#endif
#ifdef ENOMEM
	{ ENOMEM, "ENOMEM" },
#endif
#ifdef ENOMSG
	{ ENOMSG, "ENOMSG" },
#endif
#ifdef ENOPROTOOPT
	{ ENOPROTOOPT, "ENOPROTOOPT" },
#endif
#ifdef ENOSPC
	{ ENOSPC, "ENOSPC" },
#endif
#ifdef ENOSYS
	{ ENOSYS, "ENOSYS" },
#endif
#ifdef ENOTCONN
	{ ENOTCONN, "ENOTCONN" },
#endif
#ifdef ENOTDIR
	{ ENOTDIR, "ENOTDIR" },
#endif
#ifdef ENOTEMPTY
	{ ENOTEMPTY, "ENOTEMPTY" },
#endif
#ifdef ENOTRECOVERABLE
	{ ENOTRECOVERABLE, "ENOTRECOVERABLE" },
#endif
#ifdef ENOTSOCK
	{ ENOTSOCK, "ENOTSOCK" },
#endif
#ifdef ENOTSUP
	{ ENOTSUP, "ENOTSUP" },
#endif
#ifdef ENOTTY
	{ ENOTTY, "ENOTTY" },
#endif
#ifdef ENXIO
	{ ENXIO, "ENXIO" },
#endif
#ifdef EOPNOTSUPP
	{ EOPNOTSUPP, "EOPNOTSUPP" },
#endif
#ifdef EOVERFLOW
	{ EOVERFLOW, "EOVERFLOW" },
#endif
#ifdef EOWNERDEAD
	{ EOWNERDEAD, "EOWNERDEAD" },
#endif
#ifdef EPERM
	{ EPERM, "EPERM" },
#endif
#ifdef EPIPE
	{ EPIPE, "EPIPE" },
#endif
#ifdef EPROTO
	{ EPROTO, "EPROTO" },
#endif
#ifdef EPROTONOSUPPORT
	{ EPROTONOSUPPORT, "EPROTONOSUPPORT" },
#endif
#ifdef EPROTOTYPE
	{ EPROTOTYPE, "EPROTOTYPE" },
#endif
#ifdef ERANGE
	{ ERANGE, "ERANGE" },
#endif
#ifdef EROFS
	{ EROFS, "EROFS" },
#endif
#ifdef ESOCKTNOSUPPORT
	{ ESOCKTNOSUPPORT, "ESOCKTNOSUPPORT" },
#endif
#ifdef ESPIPE
	{ ESPIPE, "ESPIPE" },
#endif
#ifdef ESRCH
	{ ESRCH, "ESRCH" },
#endif
#ifdef ESTALE
	{ ESTALE, "ESTALE" },
#endif
#ifdef ETIMEDOUT
	{ ETIMEDOUT, "ETIMEDOUT" },
#endif
#ifdef ETXTBSY
	{ ETXTBSY, "ETXTBSY" },
#endif
#ifdef EWOULDBLOCK
	{ EWOULDBLOCK, "EWOULDBLOCK" },
#endif
#ifdef EXDEV
	{ EXDEV, "EXDEV" },
#endif
#ifdef E2BIG
	{ E2BIG, "E2BIG" },
#endif
