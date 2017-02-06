/*
    Copyright � 2017, The AROS Development Team. All rights reserved.
    $Id$
*/
#ifndef __EXEC_PLATFORM_H
#define __EXEC_PLATFORM_H

// needed to determine if this is an smp build
#include <aros/config.h>

#if defined(__AROSEXEC_SMP__)
#include <aros/types/spinlock_s.h>
#include <utility/hooks.h>

extern struct Hook Exec_TaskSpinLockFailHook;
extern void Exec_TaskSpinUnlock(spinlock_t *);

extern void Kernel_40_KrnSpinInit(spinlock_t *, void *);
#define EXEC_SPINLOCK_INIT(a) Kernel_40_KrnSpinInit((a), NULL)
extern spinlock_t *Kernel_43_KrnSpinLock(spinlock_t *, struct Hook *, ULONG, void *);
#define EXEC_SPINLOCK_LOCK(a,b) Kernel_43_KrnSpinLock((a), NULL, (b), NULL)
#define EXECTASK_SPINLOCK_LOCK(a,b) Kernel_43_KrnSpinLock((a), &Exec_TaskSpinLockFailHook, (b), NULL)
extern void Kernel_44_KrnSpinUnLock(spinlock_t *, void *);
#define EXEC_SPINLOCK_UNLOCK(a) Kernel_44_KrnSpinUnLock((a), NULL)
#define EXECTASK_SPINLOCK_UNLOCK(a) Kernel_44_KrnSpinUnLock((a), NULL); \
            Exec_TaskSpinUnlock((a))

#endif

struct Exec_PlatformData
{
    /* No platform-specific data on x86 */
};

#if defined(__AROSEXEC_SMP__)
#include "tls.h"

#if defined(AROS_NO_ATOMIC_OPERATIONS)
#define IDNESTCOUNT_INC \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            __schd->IDNestCnt++; \
    } while(0)
#define IDNESTCOUNT_DEC \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            __schd->IDNestCnt--; \
    } while(0)
#define TDNESTCOUNT_INC \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            __schd->TDNestCnt++; \
    } while(0)
#define TDNESTCOUNT_DEC \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            __schd->TDNestCnt--; \
    } while(0)
#define FLAG_SCHEDQUANTUM_CLEAR \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            __schd->ScheduleFlags &= ~TLSSF_Quantum; \
    } while(0)
#define FLAG_SCHEDQUANTUM_SET \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            __schd->ScheduleFlags |= TLSSF_Quantum; \
    } while(0)
#define FLAG_SCHEDSWITCH_CLEAR \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            __schd->ScheduleFlags &= ~TLSSF_Switch; \
    } while(0)
#define FLAG_SCHEDSWITCH_SET \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            __schd->ScheduleFlags |= TLSSF_Switch; \
    } while(0)
#define FLAG_SCHEDDISPATCH_CLEAR \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            __schd->ScheduleFlags &= ~TLSSF_Dispatch; \
    } while(0)
#define FLAG_SCHEDDISPATCH_SET \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            __schd->ScheduleFlags |= TLSSF_Dispatch; \
    } while(0)
#else /* !AROS_NO_ATOMIC_OPERATIONS */
#define IDNESTCOUNT_INC \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            AROS_ATOMIC_INC(__schd->IDNestCnt); \
    } while(0)
#define IDNESTCOUNT_DEC \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            AROS_ATOMIC_DEC(__schd->IDNestCnt); \
    } while(0)
#define TDNESTCOUNT_INC \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            AROS_ATOMIC_INC(__schd->TDNestCnt); \
    } while(0)
#define TDNESTCOUNT_DEC \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            AROS_ATOMIC_DEC(__schd->TDNestCnt); \
    } while(0)
#define FLAG_SCHEDQUANTUM_CLEAR \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            AROS_ATOMIC_AND(__schd->ScheduleFlags, ~TLSSF_Quantum); \
    } while(0)
#define FLAG_SCHEDQUANTUM_SET \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            AROS_ATOMIC_OR(__schd->ScheduleFlags, TLSSF_Quantum); \
    } while(0)
#define FLAG_SCHEDSWITCH_CLEAR \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            AROS_ATOMIC_AND(__schd->ScheduleFlags, ~TLSSF_Switch); \
    } while(0)
#define FLAG_SCHEDSWITCH_SET \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            AROS_ATOMIC_OR(__schd->ScheduleFlags, TLSSF_Switch); \
    } while(0)
#define FLAG_SCHEDDISPATCH_CLEAR \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            AROS_ATOMIC_AND(__schd->ScheduleFlags, ~TLSSF_Dispatch); \
    } while(0)
#define FLAG_SCHEDDISPATCH_SET \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            AROS_ATOMIC_OR(__schd->ScheduleFlags, TLSSF_Dispatch); \
    } while(0)
#endif /* !AROS_NO_ATOMIC_OPERATIONS */
#define IDNESTCOUNT_GET \
    ({ \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        LONG __ret = 0; \
        if (__schd) \
            __ret = (__schd->IDNestCnt); \
         __ret;  \
   })
#define IDNESTCOUNT_SET(val) \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            __schd->IDNestCnt = val; \
    } while(0)
#define TDNESTCOUNT_GET \
    ({ \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        LONG __ret = 0; \
        if (__schd) \
            __ret = (__schd->TDNestCnt); \
        __ret;  \
    })
#define TDNESTCOUNT_SET(val) \
    do { \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
            __schd->TDNestCnt = val; \
    } while(0)
#define FLAG_SCHEDQUANTUM_ISSET \
    ({ \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        BOOL __ret = FALSE; \
        if (__schd) \
            __ret = (__schd->ScheduleFlags & TLSSF_Quantum); \
        __ret;  \
    })
#define FLAG_SCHEDSWITCH_ISSET \
    ({ \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        BOOL __ret = FALSE; \
        if (__schd) \
            __ret = (__schd->ScheduleFlags & TLSSF_Switch); \
        __ret;  \
    })
#define FLAG_SCHEDDISPATCH_ISSET \
    ({ \
        tls_t *__tls = TLS_PTR_GET(); \
        BOOL __ret = FALSE; \
        if (__schd) \
            __ret = (__schd->ScheduleFlags & TLSSF_Dispatch); \
        __ret;  \
    })
#define GET_THIS_TASK \
    ({ \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        struct Task *__ret = NULL; \
        if (__schd) \
            __ret = __schd->RunningTask; \
        __ret;  \
    })
#define SET_THIS_TASK(x)         \
    ({ \
        struct X86SchedulerPrivate  *__schd = TLS_GET(ScheduleData); \
        if (__schd) \
        { \
            __schd->RunningTask = (x); \
            KrnSpinLock(&PrivExecBase(SysBase)->TaskRunningSpinLock, NULL, SPINLOCK_MODE_WRITE); \
            AddHead(&PrivExecBase(SysBase)->TaskRunning, (struct Node *)(x)); \
            KrnSpinUnLock(&PrivExecBase(SysBase)->TaskRunningSpinLock);  \
        } \
    })

#define __AROSEXEC_SUPERSCHEDFLAGS__

#else /* !__AROSEXEC_SMP__ */

#ifdef AROS_NO_ATOMIC_OPERATIONS
#define IDNESTCOUNT_INC                 SysBase->IDNestCnt++
#define IDNESTCOUNT_DEC                 SysBase->IDNestCnt--
#define TDNESTCOUNT_INC                 SysBase->TDNestCnt++
#define TDNESTCOUNT_DEC                 SysBase->TDNestCnt--
#define FLAG_SCHEDQUANTUM_CLEAR         SysBase->SysFlags &= ~SFF_QuantumOver
#define FLAG_SCHEDQUANTUM_SET           SysBase->SysFlags |= SFF_QuantumOver
#define FLAG_SCHEDSWITCH_CLEAR          SysBase->AttnResched &= ~ARF_AttnSwitch
#define FLAG_SCHEDSWITCH_SET            SysBase->AttnResched |= ARF_AttnSwitch
#define FLAG_SCHEDDISPATCH_CLEAR        SysBase->AttnResched &= ~ARF_AttnDispatch
#define FLAG_SCHEDDISPATCH_SET          SysBase->AttnResched |= ARF_AttnDispatch
#else
#define IDNESTCOUNT_INC                 AROS_ATOMIC_INC(SysBase->IDNestCnt)
#define IDNESTCOUNT_DEC                 AROS_ATOMIC_DEC(SysBase->IDNestCnt)
#define TDNESTCOUNT_INC                 AROS_ATOMIC_INC(SysBase->TDNestCnt)
#define TDNESTCOUNT_DEC                 AROS_ATOMIC_DEC(SysBase->TDNestCnt)
#define FLAG_SCHEDQUANTUM_CLEAR         AROS_ATOMIC_AND(SysBase->SysFlags, ~SFF_QuantumOver)
#define FLAG_SCHEDQUANTUM_SET           AROS_ATOMIC_OR(SysBase->SysFlags, SFF_QuantumOver)
#define FLAG_SCHEDSWITCH_CLEAR          AROS_ATOMIC_AND(SysBase->AttnResched, ~ARF_AttnSwitch)
#define FLAG_SCHEDSWITCH_SET            AROS_ATOMIC_OR(SysBase->AttnResched, ARF_AttnSwitch)
#define FLAG_SCHEDDISPATCH_CLEAR        AROS_ATOMIC_AND(SysBase->AttnResched, ~ARF_AttnDispatch)
#define FLAG_SCHEDDISPATCH_SET          AROS_ATOMIC_OR(SysBase->AttnResched, ARF_AttnDispatch)
#endif
#define IDNESTCOUNT_GET                 (SysBase->IDNestCnt)
#define IDNESTCOUNT_SET(val)            (SysBase->IDNestCnt=(val))
#define TDNESTCOUNT_GET                 (SysBase->TDNestCnt)
#define TDNESTCOUNT_SET(val)            (SysBase->TDNestCnt=(val))
#define FLAG_SCHEDQUANTUM_ISSET         (SysBase->SysFlags & SFF_QuantumOver)
#define FLAG_SCHEDSWITCH_ISSET          (SysBase->AttnResched & ARF_AttnSwitch)
#define FLAG_SCHEDDISPATCH_ISSET        (SysBase->AttnResched & ARF_AttnDispatch)

#define GET_THIS_TASK                   (SysBase->ThisTask)
#define SET_THIS_TASK(x)                (SysBase->ThisTask=(x))

#endif /* !__AROSEXEC_SMP__ */

#endif /* __EXEC_PLATFORM_H */