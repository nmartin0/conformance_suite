# Triage of build failures, third system, group build0

306 assertions ended UNINITIATED. Counts: TEST-BUG 3, SYSTEM 303.

## Error groups

| count | first error | class | reason |
|---|---|---|---|
| 183 | call to undeclared function 'CMPLX'; ISO C99 and later do not support  | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| 48 | use of undeclared identifier 'IXANY' | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| 39 | 'aio.h' file not found | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| 15 | call to undeclared function 'clock_nanosleep'; ISO C99 and later do no | SYSTEM | clock_nanosleep() (CX, required) not declared |
| 6 | call to undeclared function 'sigtimedwait'; ISO C99 and later do not s | SYSTEM | sigtimedwait() (CX, required) not declared |
| 5 | variable has incomplete type 'struct sigevent' | SYSTEM | struct sigevent, SIGEV_THREAD and the timer_* functions (required) are not declared by <signal.h>/<time.h> |
| 4 | call to undeclared function 'at_quick_exit'; ISO C99 and later do not  | SYSTEM | at_quick_exit() (required in <stdlib.h>) not declared |
| 3 | (none: script died before its first result) | TEST-BUG | the function name r is a preset shell alias in some shells, so defining it is a syntax error; renamed to run_case |
| 2 | 'threads.h' file not found | SYSTEM | <threads.h> (required by the standard) is absent |
| 1 | use of undeclared identifier 'SIGRTMIN' | SYSTEM | SIGRTMIN (required in <signal.h>) not defined |

The standard (checked in the page of each header and function: no option shading beyond CX, which is base) requires every symbol in the SYSTEM groups under the feature macros the driver defines. Their absence makes UNINITIATED the honest outcome for a function module (see docs/linux-deviations.md for the same convention). XBD.complex.h.* (buildfail FAIL) already reports a missing CMPLX as FAIL.

## Per assertion

| ID | class | reason |
|---|---|---|
| XBD.11.1-interface-characteristics.011 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.012 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.013 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.014 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.015 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.016 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.017 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.018 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.019 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.020 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.021 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.022 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.023 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.024 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.025 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.026 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.027 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.028 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.029 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.030 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.031 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.032 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.033 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.034 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.035 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.036 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.037 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.038 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.1-interface-characteristics.040 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.002 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.003 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.004 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.005 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.006 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.007 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.008 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.009 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.010 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.015 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.017 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.018 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.019 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.020 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.021 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.022 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.023 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.024 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.11.2-terminal-parameters.025 | SYSTEM | IXANY (required in <termios.h>, not shaded) is not defined |
| XBD.4.2-concurrent-execution.001 | SYSTEM | clock_nanosleep() (CX, required) not declared |
| XCU.awk.003 | TEST-BUG | the function name r is a preset shell alias in some shells, so defining it is a syntax error; renamed to run_case |
| XCU.find.031 | TEST-BUG | the function name r is a preset shell alias in some shells, so defining it is a syntax error; renamed to run_case |
| XCU.sed.003 | TEST-BUG | the function name r is a preset shell alias in some shells, so defining it is a syntax error; renamed to run_case |
| XSH.2.4.1-signal-generation.002 | SYSTEM | sigtimedwait() (CX, required) not declared |
| XSH.2.4.1-signal-generation.006 | SYSTEM | sigtimedwait() (CX, required) not declared |
| XSH.2.4.1-signal-generation.007 | SYSTEM | sigtimedwait() (CX, required) not declared |
| XSH.2.4.2-realtime-signals.002 | SYSTEM | struct sigevent, SIGEV_THREAD and the timer_* functions (required) are not declared by <signal.h>/<time.h> |
| XSH.2.4.2-realtime-signals.003 | SYSTEM | struct sigevent, SIGEV_THREAD and the timer_* functions (required) are not declared by <signal.h>/<time.h> |
| XSH.2.4.2-realtime-signals.006 | SYSTEM | struct sigevent, SIGEV_THREAD and the timer_* functions (required) are not declared by <signal.h>/<time.h> |
| XSH.2.4.3-signal-actions.003 | SYSTEM | SIGRTMIN (required in <signal.h>) not defined |
| XSH.2.4.3-signal-actions.016 | SYSTEM | struct sigevent, SIGEV_THREAD and the timer_* functions (required) are not declared by <signal.h>/<time.h> |
| XSH.2.4.3-signal-actions.018 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.2.9-threads.005 | SYSTEM | <threads.h> (required by the standard) is absent |
| XSH.2.9-threads.007 | SYSTEM | <threads.h> (required by the standard) is absent |
| XSH.2.9-threads.021 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.2.9-threads.034 | SYSTEM | struct sigevent, SIGEV_THREAD and the timer_* functions (required) are not declared by <signal.h>/<time.h> |
| XSH.CMPLX.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.CMPLX.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.CMPLX.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.CMPLX.004 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH._Exit.007 | SYSTEM | sigtimedwait() (CX, required) not declared |
| XSH._Exit.013 | SYSTEM | at_quick_exit() (required in <stdlib.h>) not declared |
| XSH._Exit.019 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_cancel.001 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_cancel.002 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_cancel.003 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_cancel.004 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_error.001 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_error.002 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_error.003 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_error.004 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_fsync.002 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_fsync.003 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_fsync.004 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_read.001 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_read.002 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_read.003 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_read.004 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_read.005 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_read.006 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_read.008 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_read.009 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_return.001 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_return.002 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_return.003 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_return.004 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_suspend.001 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_suspend.002 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_suspend.003 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_suspend.004 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_suspend.005 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_suspend.006 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_write.001 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_write.002 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_write.003 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_write.004 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_write.005 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_write.006 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.aio_write.007 | SYSTEM | <aio.h> (required, no option shading on the header) is absent |
| XSH.alarm.006 | SYSTEM | sigtimedwait() (CX, required) not declared |
| XSH.alarm.007 | SYSTEM | sigtimedwait() (CX, required) not declared |
| XSH.at_quick_exit.001 | SYSTEM | at_quick_exit() (required in <stdlib.h>) not declared |
| XSH.at_quick_exit.002 | SYSTEM | at_quick_exit() (required in <stdlib.h>) not declared |
| XSH.at_quick_exit.003 | SYSTEM | at_quick_exit() (required in <stdlib.h>) not declared |
| XSH.cabs.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cabs.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cabs.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cabs.004 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacos.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacos.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacos.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacos.004 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacos.005 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacos.006 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacos.007 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacos.008 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacos.009 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacos.010 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacos.011 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacos.012 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacos.013 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacos.014 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacos.015 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacos.016 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacosh.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacosh.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacosh.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacosh.004 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacosh.005 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacosh.006 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacosh.007 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacosh.008 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacosh.009 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacosh.010 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacosh.011 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacosh.012 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacosh.013 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacosh.014 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacosh.015 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cacosh.016 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.carg.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.carg.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.carg.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casin.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casin.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casin.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casin.004 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casin.005 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casin.006 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casin.007 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casin.008 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casin.009 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casin.010 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casin.011 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casin.012 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casinh.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casinh.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casinh.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casinh.004 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casinh.005 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casinh.006 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casinh.007 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casinh.008 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casinh.009 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casinh.010 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casinh.011 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.casinh.012 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catan.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catan.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catan.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catan.004 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catan.005 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catan.006 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catan.007 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catan.008 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catan.009 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catan.010 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catan.011 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catan.012 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catan.013 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catanh.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catanh.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catanh.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catanh.004 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catanh.005 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catanh.006 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catanh.007 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catanh.008 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catanh.009 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catanh.010 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catanh.011 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catanh.012 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catanh.013 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.catanh.014 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccos.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccos.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccos.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccos.004 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccos.005 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccos.006 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccos.007 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccos.008 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccos.009 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccos.010 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccos.011 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccos.013 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccosh.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccosh.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccosh.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccosh.004 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccosh.005 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccosh.006 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccosh.007 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccosh.008 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccosh.009 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccosh.010 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccosh.011 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.ccosh.013 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cexp.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cexp.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cexp.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cexp.004 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cexp.005 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cexp.006 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cexp.007 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cexp.008 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cexp.009 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cexp.010 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cexp.011 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cexp.012 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cexp.013 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cexp.014 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cimag.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cimag.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cimag.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.clock_nanosleep.001 | SYSTEM | clock_nanosleep() (CX, required) not declared |
| XSH.clock_nanosleep.002 | SYSTEM | clock_nanosleep() (CX, required) not declared |
| XSH.clock_nanosleep.003 | SYSTEM | clock_nanosleep() (CX, required) not declared |
| XSH.clock_nanosleep.004 | SYSTEM | clock_nanosleep() (CX, required) not declared |
| XSH.clock_nanosleep.005 | SYSTEM | clock_nanosleep() (CX, required) not declared |
| XSH.clock_nanosleep.006 | SYSTEM | clock_nanosleep() (CX, required) not declared |
| XSH.clock_nanosleep.007 | SYSTEM | clock_nanosleep() (CX, required) not declared |
| XSH.clock_nanosleep.008 | SYSTEM | clock_nanosleep() (CX, required) not declared |
| XSH.clock_nanosleep.009 | SYSTEM | clock_nanosleep() (CX, required) not declared |
| XSH.clock_nanosleep.010 | SYSTEM | clock_nanosleep() (CX, required) not declared |
| XSH.clock_nanosleep.011 | SYSTEM | clock_nanosleep() (CX, required) not declared |
| XSH.clock_nanosleep.012 | SYSTEM | clock_nanosleep() (CX, required) not declared |
| XSH.clock_nanosleep.013 | SYSTEM | clock_nanosleep() (CX, required) not declared |
| XSH.clock_nanosleep.014 | SYSTEM | clock_nanosleep() (CX, required) not declared |
| XSH.clog.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.clog.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.clog.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.clog.004 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.clog.005 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.clog.006 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.clog.007 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.clog.008 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.clog.009 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.clog.010 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.clog.011 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.clog.012 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.clog.013 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.clog.014 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cproj.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cproj.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.cproj.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.creal.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.creal.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.creal.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csin.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csin.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csin.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csin.004 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csin.005 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csin.006 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csin.007 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csin.008 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csin.009 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csin.010 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csin.011 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csinh.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csinh.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csinh.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csinh.004 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csinh.005 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csinh.006 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csinh.007 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csinh.008 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csinh.009 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csinh.010 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csinh.011 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csinh.012 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csqrt.001 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csqrt.002 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csqrt.003 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csqrt.004 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |
| XSH.csqrt.005 | SYSTEM | CMPLX/CMPLXF/CMPLXL (required in <complex.h>) not defined |

## Changes

- modules/xcu/awk/t/003-fields.sh, modules/xcu/find/t/031-perm-symbolic.sh, modules/xcu/sed/t/003-addresses.sh: helper function `r` renamed `run_case`. In the Korn shell `r` is a preset alias (the fc page mentions it), so `r() {` is a syntax error there. No other test defines `r`.
- Re-run on this machine (flock, --only, fresh results dirs /tmp/obtriage-ID): XCU.awk.003 PASS, XCU.find.031 PASS, XCU.sed.003 PASS.
- python3 ci/lint.py: problem count unchanged apart from the existing "declares no MUTANT" lines (no other lint problem mentions these files).

## Not changed, with the option left open

- 179 tests in modules/xsh/c* use CMPLX only to build their arguments (shared mt.h, 56 copies, several modules belong to other groups). They could build the argument from a two-element array, since the standard gives a complex type the representation of that array, which would decouple them from a missing CMPLX (XBD.complex.h.007 and XSH.CMPLX.* still report it). Not done: the convention does not call for it and it touches files outside this list.
- 46 terminal tests (XBD.11.1, XBD.11.2) use IXANY only in a mask that clears flags; a `#ifdef IXANY` guard would let them run. Not done for the same reason. XBD.11.2.007 and .009 and the termios.h header check need IXANY itself.
- aio, clock_nanosleep, sigtimedwait, at_quick_exit, timer and threads.h users are also incidental in a few tests (XBD.4.2.001, XSH.alarm.006/007, XSH._Exit.007/013/019, XSH.2.9-threads.021) but those interfaces are required, so the build failure is the system.
