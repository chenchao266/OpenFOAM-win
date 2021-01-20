//2017-09-22 FSD blueCAPE Lda: Created this file for adding a hack for 'mode_t'


// Use the same definition as on Linux
#ifndef _HACK4_MODE_T_
#define _HACK4_MODE_T_
#include <stdint.h>
typedef unsigned int mode_t;
typedef unsigned int pid_t;
typedef int64_t offset_t;
 
#define __p_sig_fn_t  _crt_signal_t
#endif
