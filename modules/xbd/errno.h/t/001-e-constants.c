/*
 * The E* macros are positive int constant expressions usable in #if.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.errno.h.001
 */
#include <errno.h>
#include "pc.h"

#if !defined(E2BIG)
#error "E2BIG is not defined"
#elif (E2BIG) <= 0
#error "E2BIG is not positive in #if"
#endif
#if !defined(EACCES)
#error "EACCES is not defined"
#elif (EACCES) <= 0
#error "EACCES is not positive in #if"
#endif
#if !defined(EADDRINUSE)
#error "EADDRINUSE is not defined"
#elif (EADDRINUSE) <= 0
#error "EADDRINUSE is not positive in #if"
#endif
#if !defined(EADDRNOTAVAIL)
#error "EADDRNOTAVAIL is not defined"
#elif (EADDRNOTAVAIL) <= 0
#error "EADDRNOTAVAIL is not positive in #if"
#endif
#if !defined(EAFNOSUPPORT)
#error "EAFNOSUPPORT is not defined"
#elif (EAFNOSUPPORT) <= 0
#error "EAFNOSUPPORT is not positive in #if"
#endif
#if !defined(EAGAIN)
#error "EAGAIN is not defined"
#elif (EAGAIN) <= 0
#error "EAGAIN is not positive in #if"
#endif
#if !defined(EALREADY)
#error "EALREADY is not defined"
#elif (EALREADY) <= 0
#error "EALREADY is not positive in #if"
#endif
#if !defined(EBADF)
#error "EBADF is not defined"
#elif (EBADF) <= 0
#error "EBADF is not positive in #if"
#endif
#if !defined(EBADMSG)
#error "EBADMSG is not defined"
#elif (EBADMSG) <= 0
#error "EBADMSG is not positive in #if"
#endif
#if !defined(EBUSY)
#error "EBUSY is not defined"
#elif (EBUSY) <= 0
#error "EBUSY is not positive in #if"
#endif
#if !defined(ECANCELED)
#error "ECANCELED is not defined"
#elif (ECANCELED) <= 0
#error "ECANCELED is not positive in #if"
#endif
#if !defined(ECHILD)
#error "ECHILD is not defined"
#elif (ECHILD) <= 0
#error "ECHILD is not positive in #if"
#endif
#if !defined(ECONNABORTED)
#error "ECONNABORTED is not defined"
#elif (ECONNABORTED) <= 0
#error "ECONNABORTED is not positive in #if"
#endif
#if !defined(ECONNREFUSED)
#error "ECONNREFUSED is not defined"
#elif (ECONNREFUSED) <= 0
#error "ECONNREFUSED is not positive in #if"
#endif
#if !defined(ECONNRESET)
#error "ECONNRESET is not defined"
#elif (ECONNRESET) <= 0
#error "ECONNRESET is not positive in #if"
#endif
#if !defined(EDEADLK)
#error "EDEADLK is not defined"
#elif (EDEADLK) <= 0
#error "EDEADLK is not positive in #if"
#endif
#if !defined(EDESTADDRREQ)
#error "EDESTADDRREQ is not defined"
#elif (EDESTADDRREQ) <= 0
#error "EDESTADDRREQ is not positive in #if"
#endif
#if !defined(EDOM)
#error "EDOM is not defined"
#elif (EDOM) <= 0
#error "EDOM is not positive in #if"
#endif
#if !defined(EDQUOT)
#error "EDQUOT is not defined"
#elif (EDQUOT) <= 0
#error "EDQUOT is not positive in #if"
#endif
#if !defined(EEXIST)
#error "EEXIST is not defined"
#elif (EEXIST) <= 0
#error "EEXIST is not positive in #if"
#endif
#if !defined(EFAULT)
#error "EFAULT is not defined"
#elif (EFAULT) <= 0
#error "EFAULT is not positive in #if"
#endif
#if !defined(EFBIG)
#error "EFBIG is not defined"
#elif (EFBIG) <= 0
#error "EFBIG is not positive in #if"
#endif
#if !defined(EHOSTUNREACH)
#error "EHOSTUNREACH is not defined"
#elif (EHOSTUNREACH) <= 0
#error "EHOSTUNREACH is not positive in #if"
#endif
#if !defined(EIDRM)
#error "EIDRM is not defined"
#elif (EIDRM) <= 0
#error "EIDRM is not positive in #if"
#endif
#if !defined(EILSEQ)
#error "EILSEQ is not defined"
#elif (EILSEQ) <= 0
#error "EILSEQ is not positive in #if"
#endif
#if !defined(EINPROGRESS)
#error "EINPROGRESS is not defined"
#elif (EINPROGRESS) <= 0
#error "EINPROGRESS is not positive in #if"
#endif
#if !defined(EINTR)
#error "EINTR is not defined"
#elif (EINTR) <= 0
#error "EINTR is not positive in #if"
#endif
#if !defined(EINVAL)
#error "EINVAL is not defined"
#elif (EINVAL) <= 0
#error "EINVAL is not positive in #if"
#endif
#if !defined(EIO)
#error "EIO is not defined"
#elif (EIO) <= 0
#error "EIO is not positive in #if"
#endif
#if !defined(EISCONN)
#error "EISCONN is not defined"
#elif (EISCONN) <= 0
#error "EISCONN is not positive in #if"
#endif
#if !defined(EISDIR)
#error "EISDIR is not defined"
#elif (EISDIR) <= 0
#error "EISDIR is not positive in #if"
#endif
#if !defined(ELOOP)
#error "ELOOP is not defined"
#elif (ELOOP) <= 0
#error "ELOOP is not positive in #if"
#endif
#if !defined(EMFILE)
#error "EMFILE is not defined"
#elif (EMFILE) <= 0
#error "EMFILE is not positive in #if"
#endif
#if !defined(EMLINK)
#error "EMLINK is not defined"
#elif (EMLINK) <= 0
#error "EMLINK is not positive in #if"
#endif
#if !defined(EMSGSIZE)
#error "EMSGSIZE is not defined"
#elif (EMSGSIZE) <= 0
#error "EMSGSIZE is not positive in #if"
#endif
#if !defined(EMULTIHOP)
#error "EMULTIHOP is not defined"
#elif (EMULTIHOP) <= 0
#error "EMULTIHOP is not positive in #if"
#endif
#if !defined(ENAMETOOLONG)
#error "ENAMETOOLONG is not defined"
#elif (ENAMETOOLONG) <= 0
#error "ENAMETOOLONG is not positive in #if"
#endif
#if !defined(ENETDOWN)
#error "ENETDOWN is not defined"
#elif (ENETDOWN) <= 0
#error "ENETDOWN is not positive in #if"
#endif
#if !defined(ENETRESET)
#error "ENETRESET is not defined"
#elif (ENETRESET) <= 0
#error "ENETRESET is not positive in #if"
#endif
#if !defined(ENETUNREACH)
#error "ENETUNREACH is not defined"
#elif (ENETUNREACH) <= 0
#error "ENETUNREACH is not positive in #if"
#endif
#if !defined(ENFILE)
#error "ENFILE is not defined"
#elif (ENFILE) <= 0
#error "ENFILE is not positive in #if"
#endif
#if !defined(ENOBUFS)
#error "ENOBUFS is not defined"
#elif (ENOBUFS) <= 0
#error "ENOBUFS is not positive in #if"
#endif
#if !defined(ENODEV)
#error "ENODEV is not defined"
#elif (ENODEV) <= 0
#error "ENODEV is not positive in #if"
#endif
#if !defined(ENOENT)
#error "ENOENT is not defined"
#elif (ENOENT) <= 0
#error "ENOENT is not positive in #if"
#endif
#if !defined(ENOEXEC)
#error "ENOEXEC is not defined"
#elif (ENOEXEC) <= 0
#error "ENOEXEC is not positive in #if"
#endif
#if !defined(ENOLCK)
#error "ENOLCK is not defined"
#elif (ENOLCK) <= 0
#error "ENOLCK is not positive in #if"
#endif
#if !defined(ENOLINK)
#error "ENOLINK is not defined"
#elif (ENOLINK) <= 0
#error "ENOLINK is not positive in #if"
#endif
#if !defined(ENOMEM)
#error "ENOMEM is not defined"
#elif (ENOMEM) <= 0
#error "ENOMEM is not positive in #if"
#endif
#if !defined(ENOMSG)
#error "ENOMSG is not defined"
#elif (ENOMSG) <= 0
#error "ENOMSG is not positive in #if"
#endif
#if !defined(ENOPROTOOPT)
#error "ENOPROTOOPT is not defined"
#elif (ENOPROTOOPT) <= 0
#error "ENOPROTOOPT is not positive in #if"
#endif
#if !defined(ENOSPC)
#error "ENOSPC is not defined"
#elif (ENOSPC) <= 0
#error "ENOSPC is not positive in #if"
#endif
#if !defined(ENOSYS)
#error "ENOSYS is not defined"
#elif (ENOSYS) <= 0
#error "ENOSYS is not positive in #if"
#endif
#if !defined(ENOTCONN)
#error "ENOTCONN is not defined"
#elif (ENOTCONN) <= 0
#error "ENOTCONN is not positive in #if"
#endif
#if !defined(ENOTDIR)
#error "ENOTDIR is not defined"
#elif (ENOTDIR) <= 0
#error "ENOTDIR is not positive in #if"
#endif
#if !defined(ENOTEMPTY)
#error "ENOTEMPTY is not defined"
#elif (ENOTEMPTY) <= 0
#error "ENOTEMPTY is not positive in #if"
#endif
#if !defined(ENOTRECOVERABLE)
#error "ENOTRECOVERABLE is not defined"
#elif (ENOTRECOVERABLE) <= 0
#error "ENOTRECOVERABLE is not positive in #if"
#endif
#if !defined(ENOTSOCK)
#error "ENOTSOCK is not defined"
#elif (ENOTSOCK) <= 0
#error "ENOTSOCK is not positive in #if"
#endif
#if !defined(ENOTSUP)
#error "ENOTSUP is not defined"
#elif (ENOTSUP) <= 0
#error "ENOTSUP is not positive in #if"
#endif
#if !defined(ENOTTY)
#error "ENOTTY is not defined"
#elif (ENOTTY) <= 0
#error "ENOTTY is not positive in #if"
#endif
#if !defined(ENXIO)
#error "ENXIO is not defined"
#elif (ENXIO) <= 0
#error "ENXIO is not positive in #if"
#endif
#if !defined(EOPNOTSUPP)
#error "EOPNOTSUPP is not defined"
#elif (EOPNOTSUPP) <= 0
#error "EOPNOTSUPP is not positive in #if"
#endif
#if !defined(EOVERFLOW)
#error "EOVERFLOW is not defined"
#elif (EOVERFLOW) <= 0
#error "EOVERFLOW is not positive in #if"
#endif
#if !defined(EOWNERDEAD)
#error "EOWNERDEAD is not defined"
#elif (EOWNERDEAD) <= 0
#error "EOWNERDEAD is not positive in #if"
#endif
#if !defined(EPERM)
#error "EPERM is not defined"
#elif (EPERM) <= 0
#error "EPERM is not positive in #if"
#endif
#if !defined(EPIPE)
#error "EPIPE is not defined"
#elif (EPIPE) <= 0
#error "EPIPE is not positive in #if"
#endif
#if !defined(EPROTO)
#error "EPROTO is not defined"
#elif (EPROTO) <= 0
#error "EPROTO is not positive in #if"
#endif
#if !defined(EPROTONOSUPPORT)
#error "EPROTONOSUPPORT is not defined"
#elif (EPROTONOSUPPORT) <= 0
#error "EPROTONOSUPPORT is not positive in #if"
#endif
#if !defined(EPROTOTYPE)
#error "EPROTOTYPE is not defined"
#elif (EPROTOTYPE) <= 0
#error "EPROTOTYPE is not positive in #if"
#endif
#if !defined(ERANGE)
#error "ERANGE is not defined"
#elif (ERANGE) <= 0
#error "ERANGE is not positive in #if"
#endif
#if !defined(EROFS)
#error "EROFS is not defined"
#elif (EROFS) <= 0
#error "EROFS is not positive in #if"
#endif
#if !defined(ESOCKTNOSUPPORT)
#error "ESOCKTNOSUPPORT is not defined"
#elif (ESOCKTNOSUPPORT) <= 0
#error "ESOCKTNOSUPPORT is not positive in #if"
#endif
#if !defined(ESPIPE)
#error "ESPIPE is not defined"
#elif (ESPIPE) <= 0
#error "ESPIPE is not positive in #if"
#endif
#if !defined(ESRCH)
#error "ESRCH is not defined"
#elif (ESRCH) <= 0
#error "ESRCH is not positive in #if"
#endif
#if !defined(ESTALE)
#error "ESTALE is not defined"
#elif (ESTALE) <= 0
#error "ESTALE is not positive in #if"
#endif
#if !defined(ETIMEDOUT)
#error "ETIMEDOUT is not defined"
#elif (ETIMEDOUT) <= 0
#error "ETIMEDOUT is not positive in #if"
#endif
#if !defined(ETXTBSY)
#error "ETXTBSY is not defined"
#elif (ETXTBSY) <= 0
#error "ETXTBSY is not positive in #if"
#endif
#if !defined(EWOULDBLOCK)
#error "EWOULDBLOCK is not defined"
#elif (EWOULDBLOCK) <= 0
#error "EWOULDBLOCK is not positive in #if"
#endif
#if !defined(EXDEV)
#error "EXDEV is not defined"
#elif (EXDEV) <= 0
#error "EXDEV is not positive in #if"
#endif

_Static_assert(_Generic((E2BIG), int: 1, default: 0), "E2BIG does not have type int");
_Static_assert((E2BIG) > 0, "E2BIG is not a positive constant expression");
_Static_assert(_Generic((EACCES), int: 1, default: 0), "EACCES does not have type int");
_Static_assert((EACCES) > 0, "EACCES is not a positive constant expression");
_Static_assert(_Generic((EADDRINUSE), int: 1, default: 0), "EADDRINUSE does not have type int");
_Static_assert((EADDRINUSE) > 0, "EADDRINUSE is not a positive constant expression");
_Static_assert(_Generic((EADDRNOTAVAIL), int: 1, default: 0), "EADDRNOTAVAIL does not have type int");
_Static_assert((EADDRNOTAVAIL) > 0, "EADDRNOTAVAIL is not a positive constant expression");
_Static_assert(_Generic((EAFNOSUPPORT), int: 1, default: 0), "EAFNOSUPPORT does not have type int");
_Static_assert((EAFNOSUPPORT) > 0, "EAFNOSUPPORT is not a positive constant expression");
_Static_assert(_Generic((EAGAIN), int: 1, default: 0), "EAGAIN does not have type int");
_Static_assert((EAGAIN) > 0, "EAGAIN is not a positive constant expression");
_Static_assert(_Generic((EALREADY), int: 1, default: 0), "EALREADY does not have type int");
_Static_assert((EALREADY) > 0, "EALREADY is not a positive constant expression");
_Static_assert(_Generic((EBADF), int: 1, default: 0), "EBADF does not have type int");
_Static_assert((EBADF) > 0, "EBADF is not a positive constant expression");
_Static_assert(_Generic((EBADMSG), int: 1, default: 0), "EBADMSG does not have type int");
_Static_assert((EBADMSG) > 0, "EBADMSG is not a positive constant expression");
_Static_assert(_Generic((EBUSY), int: 1, default: 0), "EBUSY does not have type int");
_Static_assert((EBUSY) > 0, "EBUSY is not a positive constant expression");
_Static_assert(_Generic((ECANCELED), int: 1, default: 0), "ECANCELED does not have type int");
_Static_assert((ECANCELED) > 0, "ECANCELED is not a positive constant expression");
_Static_assert(_Generic((ECHILD), int: 1, default: 0), "ECHILD does not have type int");
_Static_assert((ECHILD) > 0, "ECHILD is not a positive constant expression");
_Static_assert(_Generic((ECONNABORTED), int: 1, default: 0), "ECONNABORTED does not have type int");
_Static_assert((ECONNABORTED) > 0, "ECONNABORTED is not a positive constant expression");
_Static_assert(_Generic((ECONNREFUSED), int: 1, default: 0), "ECONNREFUSED does not have type int");
_Static_assert((ECONNREFUSED) > 0, "ECONNREFUSED is not a positive constant expression");
_Static_assert(_Generic((ECONNRESET), int: 1, default: 0), "ECONNRESET does not have type int");
_Static_assert((ECONNRESET) > 0, "ECONNRESET is not a positive constant expression");
_Static_assert(_Generic((EDEADLK), int: 1, default: 0), "EDEADLK does not have type int");
_Static_assert((EDEADLK) > 0, "EDEADLK is not a positive constant expression");
_Static_assert(_Generic((EDESTADDRREQ), int: 1, default: 0), "EDESTADDRREQ does not have type int");
_Static_assert((EDESTADDRREQ) > 0, "EDESTADDRREQ is not a positive constant expression");
_Static_assert(_Generic((EDOM), int: 1, default: 0), "EDOM does not have type int");
_Static_assert((EDOM) > 0, "EDOM is not a positive constant expression");
_Static_assert(_Generic((EDQUOT), int: 1, default: 0), "EDQUOT does not have type int");
_Static_assert((EDQUOT) > 0, "EDQUOT is not a positive constant expression");
_Static_assert(_Generic((EEXIST), int: 1, default: 0), "EEXIST does not have type int");
_Static_assert((EEXIST) > 0, "EEXIST is not a positive constant expression");
_Static_assert(_Generic((EFAULT), int: 1, default: 0), "EFAULT does not have type int");
_Static_assert((EFAULT) > 0, "EFAULT is not a positive constant expression");
_Static_assert(_Generic((EFBIG), int: 1, default: 0), "EFBIG does not have type int");
_Static_assert((EFBIG) > 0, "EFBIG is not a positive constant expression");
_Static_assert(_Generic((EHOSTUNREACH), int: 1, default: 0), "EHOSTUNREACH does not have type int");
_Static_assert((EHOSTUNREACH) > 0, "EHOSTUNREACH is not a positive constant expression");
_Static_assert(_Generic((EIDRM), int: 1, default: 0), "EIDRM does not have type int");
_Static_assert((EIDRM) > 0, "EIDRM is not a positive constant expression");
_Static_assert(_Generic((EILSEQ), int: 1, default: 0), "EILSEQ does not have type int");
_Static_assert((EILSEQ) > 0, "EILSEQ is not a positive constant expression");
_Static_assert(_Generic((EINPROGRESS), int: 1, default: 0), "EINPROGRESS does not have type int");
_Static_assert((EINPROGRESS) > 0, "EINPROGRESS is not a positive constant expression");
_Static_assert(_Generic((EINTR), int: 1, default: 0), "EINTR does not have type int");
_Static_assert((EINTR) > 0, "EINTR is not a positive constant expression");
_Static_assert(_Generic((EINVAL), int: 1, default: 0), "EINVAL does not have type int");
_Static_assert((EINVAL) > 0, "EINVAL is not a positive constant expression");
_Static_assert(_Generic((EIO), int: 1, default: 0), "EIO does not have type int");
_Static_assert((EIO) > 0, "EIO is not a positive constant expression");
_Static_assert(_Generic((EISCONN), int: 1, default: 0), "EISCONN does not have type int");
_Static_assert((EISCONN) > 0, "EISCONN is not a positive constant expression");
_Static_assert(_Generic((EISDIR), int: 1, default: 0), "EISDIR does not have type int");
_Static_assert((EISDIR) > 0, "EISDIR is not a positive constant expression");
_Static_assert(_Generic((ELOOP), int: 1, default: 0), "ELOOP does not have type int");
_Static_assert((ELOOP) > 0, "ELOOP is not a positive constant expression");
_Static_assert(_Generic((EMFILE), int: 1, default: 0), "EMFILE does not have type int");
_Static_assert((EMFILE) > 0, "EMFILE is not a positive constant expression");
_Static_assert(_Generic((EMLINK), int: 1, default: 0), "EMLINK does not have type int");
_Static_assert((EMLINK) > 0, "EMLINK is not a positive constant expression");
_Static_assert(_Generic((EMSGSIZE), int: 1, default: 0), "EMSGSIZE does not have type int");
_Static_assert((EMSGSIZE) > 0, "EMSGSIZE is not a positive constant expression");
_Static_assert(_Generic((EMULTIHOP), int: 1, default: 0), "EMULTIHOP does not have type int");
_Static_assert((EMULTIHOP) > 0, "EMULTIHOP is not a positive constant expression");
_Static_assert(_Generic((ENAMETOOLONG), int: 1, default: 0), "ENAMETOOLONG does not have type int");
_Static_assert((ENAMETOOLONG) > 0, "ENAMETOOLONG is not a positive constant expression");
_Static_assert(_Generic((ENETDOWN), int: 1, default: 0), "ENETDOWN does not have type int");
_Static_assert((ENETDOWN) > 0, "ENETDOWN is not a positive constant expression");
_Static_assert(_Generic((ENETRESET), int: 1, default: 0), "ENETRESET does not have type int");
_Static_assert((ENETRESET) > 0, "ENETRESET is not a positive constant expression");
_Static_assert(_Generic((ENETUNREACH), int: 1, default: 0), "ENETUNREACH does not have type int");
_Static_assert((ENETUNREACH) > 0, "ENETUNREACH is not a positive constant expression");
_Static_assert(_Generic((ENFILE), int: 1, default: 0), "ENFILE does not have type int");
_Static_assert((ENFILE) > 0, "ENFILE is not a positive constant expression");
_Static_assert(_Generic((ENOBUFS), int: 1, default: 0), "ENOBUFS does not have type int");
_Static_assert((ENOBUFS) > 0, "ENOBUFS is not a positive constant expression");
_Static_assert(_Generic((ENODEV), int: 1, default: 0), "ENODEV does not have type int");
_Static_assert((ENODEV) > 0, "ENODEV is not a positive constant expression");
_Static_assert(_Generic((ENOENT), int: 1, default: 0), "ENOENT does not have type int");
_Static_assert((ENOENT) > 0, "ENOENT is not a positive constant expression");
_Static_assert(_Generic((ENOEXEC), int: 1, default: 0), "ENOEXEC does not have type int");
_Static_assert((ENOEXEC) > 0, "ENOEXEC is not a positive constant expression");
_Static_assert(_Generic((ENOLCK), int: 1, default: 0), "ENOLCK does not have type int");
_Static_assert((ENOLCK) > 0, "ENOLCK is not a positive constant expression");
_Static_assert(_Generic((ENOLINK), int: 1, default: 0), "ENOLINK does not have type int");
_Static_assert((ENOLINK) > 0, "ENOLINK is not a positive constant expression");
_Static_assert(_Generic((ENOMEM), int: 1, default: 0), "ENOMEM does not have type int");
_Static_assert((ENOMEM) > 0, "ENOMEM is not a positive constant expression");
_Static_assert(_Generic((ENOMSG), int: 1, default: 0), "ENOMSG does not have type int");
_Static_assert((ENOMSG) > 0, "ENOMSG is not a positive constant expression");
_Static_assert(_Generic((ENOPROTOOPT), int: 1, default: 0), "ENOPROTOOPT does not have type int");
_Static_assert((ENOPROTOOPT) > 0, "ENOPROTOOPT is not a positive constant expression");
_Static_assert(_Generic((ENOSPC), int: 1, default: 0), "ENOSPC does not have type int");
_Static_assert((ENOSPC) > 0, "ENOSPC is not a positive constant expression");
_Static_assert(_Generic((ENOSYS), int: 1, default: 0), "ENOSYS does not have type int");
_Static_assert((ENOSYS) > 0, "ENOSYS is not a positive constant expression");
_Static_assert(_Generic((ENOTCONN), int: 1, default: 0), "ENOTCONN does not have type int");
_Static_assert((ENOTCONN) > 0, "ENOTCONN is not a positive constant expression");
_Static_assert(_Generic((ENOTDIR), int: 1, default: 0), "ENOTDIR does not have type int");
_Static_assert((ENOTDIR) > 0, "ENOTDIR is not a positive constant expression");
_Static_assert(_Generic((ENOTEMPTY), int: 1, default: 0), "ENOTEMPTY does not have type int");
_Static_assert((ENOTEMPTY) > 0, "ENOTEMPTY is not a positive constant expression");
_Static_assert(_Generic((ENOTRECOVERABLE), int: 1, default: 0), "ENOTRECOVERABLE does not have type int");
_Static_assert((ENOTRECOVERABLE) > 0, "ENOTRECOVERABLE is not a positive constant expression");
_Static_assert(_Generic((ENOTSOCK), int: 1, default: 0), "ENOTSOCK does not have type int");
_Static_assert((ENOTSOCK) > 0, "ENOTSOCK is not a positive constant expression");
_Static_assert(_Generic((ENOTSUP), int: 1, default: 0), "ENOTSUP does not have type int");
_Static_assert((ENOTSUP) > 0, "ENOTSUP is not a positive constant expression");
_Static_assert(_Generic((ENOTTY), int: 1, default: 0), "ENOTTY does not have type int");
_Static_assert((ENOTTY) > 0, "ENOTTY is not a positive constant expression");
_Static_assert(_Generic((ENXIO), int: 1, default: 0), "ENXIO does not have type int");
_Static_assert((ENXIO) > 0, "ENXIO is not a positive constant expression");
_Static_assert(_Generic((EOPNOTSUPP), int: 1, default: 0), "EOPNOTSUPP does not have type int");
_Static_assert((EOPNOTSUPP) > 0, "EOPNOTSUPP is not a positive constant expression");
_Static_assert(_Generic((EOVERFLOW), int: 1, default: 0), "EOVERFLOW does not have type int");
_Static_assert((EOVERFLOW) > 0, "EOVERFLOW is not a positive constant expression");
_Static_assert(_Generic((EOWNERDEAD), int: 1, default: 0), "EOWNERDEAD does not have type int");
_Static_assert((EOWNERDEAD) > 0, "EOWNERDEAD is not a positive constant expression");
_Static_assert(_Generic((EPERM), int: 1, default: 0), "EPERM does not have type int");
_Static_assert((EPERM) > 0, "EPERM is not a positive constant expression");
_Static_assert(_Generic((EPIPE), int: 1, default: 0), "EPIPE does not have type int");
_Static_assert((EPIPE) > 0, "EPIPE is not a positive constant expression");
_Static_assert(_Generic((EPROTO), int: 1, default: 0), "EPROTO does not have type int");
_Static_assert((EPROTO) > 0, "EPROTO is not a positive constant expression");
_Static_assert(_Generic((EPROTONOSUPPORT), int: 1, default: 0), "EPROTONOSUPPORT does not have type int");
_Static_assert((EPROTONOSUPPORT) > 0, "EPROTONOSUPPORT is not a positive constant expression");
_Static_assert(_Generic((EPROTOTYPE), int: 1, default: 0), "EPROTOTYPE does not have type int");
_Static_assert((EPROTOTYPE) > 0, "EPROTOTYPE is not a positive constant expression");
_Static_assert(_Generic((ERANGE), int: 1, default: 0), "ERANGE does not have type int");
_Static_assert((ERANGE) > 0, "ERANGE is not a positive constant expression");
_Static_assert(_Generic((EROFS), int: 1, default: 0), "EROFS does not have type int");
_Static_assert((EROFS) > 0, "EROFS is not a positive constant expression");
_Static_assert(_Generic((ESOCKTNOSUPPORT), int: 1, default: 0), "ESOCKTNOSUPPORT does not have type int");
_Static_assert((ESOCKTNOSUPPORT) > 0, "ESOCKTNOSUPPORT is not a positive constant expression");
_Static_assert(_Generic((ESPIPE), int: 1, default: 0), "ESPIPE does not have type int");
_Static_assert((ESPIPE) > 0, "ESPIPE is not a positive constant expression");
_Static_assert(_Generic((ESRCH), int: 1, default: 0), "ESRCH does not have type int");
_Static_assert((ESRCH) > 0, "ESRCH is not a positive constant expression");
_Static_assert(_Generic((ESTALE), int: 1, default: 0), "ESTALE does not have type int");
_Static_assert((ESTALE) > 0, "ESTALE is not a positive constant expression");
_Static_assert(_Generic((ETIMEDOUT), int: 1, default: 0), "ETIMEDOUT does not have type int");
_Static_assert((ETIMEDOUT) > 0, "ETIMEDOUT is not a positive constant expression");
_Static_assert(_Generic((ETXTBSY), int: 1, default: 0), "ETXTBSY does not have type int");
_Static_assert((ETXTBSY) > 0, "ETXTBSY is not a positive constant expression");
_Static_assert(_Generic((EWOULDBLOCK), int: 1, default: 0), "EWOULDBLOCK does not have type int");
_Static_assert((EWOULDBLOCK) > 0, "EWOULDBLOCK is not a positive constant expression");
_Static_assert(_Generic((EXDEV), int: 1, default: 0), "EXDEV does not have type int");
_Static_assert((EXDEV) > 0, "EXDEV is not a positive constant expression");

int main(void)
{
	pc_begin("XBD.errno.h.001");
	pc_test();
	pc_pass();
}
