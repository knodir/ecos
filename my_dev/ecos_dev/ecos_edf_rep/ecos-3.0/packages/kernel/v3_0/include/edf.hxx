#ifndef CYGONCE_KERNEL_EDF_HXX
#define CYGONCE_KERNEL_EDF_HXX

//==========================================================================
//
//      edf.hxx
//
//      Earliest Deadline First scheduler class declarations
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Free Software Foundation, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under    
// the terms of the GNU General Public License as published by the Free     
// Software Foundation; either version 2 or (at your option) any later      
// version.                                                                 
//
// eCos is distributed in the hope that it will be useful, but WITHOUT      
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or    
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License    
// for more details.                                                        
//
// You should have received a copy of the GNU General Public License        
// along with eCos; if not, write to the Free Software Foundation, Inc.,    
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.            
//
// As a special exception, if other files instantiate templates or use      
// macros or inline functions from this file, or you compile this file      
// and link it with other works to produce a work based on this file,       
// this file does not by itself cause the resulting work to be covered by   
// the GNU General Public License. However the source code for this file    
// must still be made available in accordance with section (3) of the GNU   
// General Public License v2.                                               
//
// This exception does not invalidate any other reasons why a work based    
// on this file might be covered by the GNU General Public License.         
// -------------------------------------------                              
// ####ECOSGPLCOPYRIGHTEND####                                              
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    Nodir Kodirov
// Contributors: nodir
// Date:         2009-09-10
// Purpose:      Define multilevel queue scheduler implementation
// Purpose:      Define Earliest Deadline First (EDF) scheduler implementation
// Description:  The classes defined here are used as base classes
//               by the common classes that define schedulers and thread
//               things. The EDF scheduler source code is based on MLQ scheduler.
// Usage:        Included according to configuration by
//               <cyg/kernel/sched.hxx>
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <cyg/kernel/ktypes.h>

#include <cyg/infra/clist.hxx>                  // List implementation

// -------------------------------------------------------------------------
// The macro CYGNUM_KERNEL_SCHED_PRIORITIES contains the number of priorities
// supported by the scheduler.

#ifndef CYGNUM_KERNEL_SCHED_PRIORITIES
#define CYGNUM_KERNEL_SCHED_PRIORITIES 32       // define a default
#endif

// set bitmap size
#define CYGNUM_KERNEL_SCHED_BITMAP_SIZE CYGNUM_KERNEL_SCHED_PRIORITIES

// -------------------------------------------------------------------------
// The macro CYGNUM_KERNEL_SCHED_BITMAP_SIZE contains the number of bits that the
// scheduler bitmap should contain. It is derived from the number of prioirity
// levels defined by the configuration.

#if CYGNUM_KERNEL_SCHED_BITMAP_SIZE <= 8
typedef cyg_ucount8 cyg_sched_bitmap;
#elif CYGNUM_KERNEL_SCHED_BITMAP_SIZE <= 16
typedef cyg_ucount16 cyg_sched_bitmap;
#elif CYGNUM_KERNEL_SCHED_BITMAP_SIZE <= 32
typedef cyg_ucount32 cyg_sched_bitmap;
#else
#error Bitmaps greater than 32 bits not currently allowed
#endif

// -------------------------------------------------------------------------
// Customize the scheduler

#define CYGIMP_THREAD_PRIORITY  1       // Threads have changable priorities

#define CYG_THREAD_MIN_PRIORITY (CYGNUM_KERNEL_SCHED_PRIORITIES-1)
#define CYG_THREAD_MAX_PRIORITY 0

// set default scheduling info value for thread constructors.
#define CYG_SCHED_DEFAULT_INFO  CYG_THREAD_MAX_PRIORITY

// -------------------------------------------------------------------------
// scheduler Run queue object

typedef Cyg_CList_T<Cyg_Thread> Cyg_RunQueue;

// -------------------------------------------------------------------------
// Thread queue implementation.
// This class provides the (scheduler specific) implementation of the
// thread queue class.

class Cyg_ThreadQueue_Implementation
    : public Cyg_CList_T<Cyg_Thread>
{
    friend class Cyg_Scheduler_Implementation;
    friend class Cyg_SchedThread_Implementation;

    void                set_thread_queue(Cyg_Thread *thread,
                                         Cyg_ThreadQueue *tq );

protected:

    // API used by Cyg_ThreadQueue

    Cyg_ThreadQueue_Implementation() {};   // Constructor
    
                                        // Add thread to queue
    void                enqueue(Cyg_Thread *thread);

                                        // return first thread on queue
    Cyg_Thread          *highpri();

                                        // remove first thread on queue    
    Cyg_Thread          *dequeue();

                                        // Remove thread from queue
    void                remove(Cyg_Thread *thread);

};

// -------------------------------------------------------------------------
// This class contains the implementation details of the scheduler, and
// provides a standard API for accessing it.

class Cyg_Scheduler_Implementation
    : public Cyg_Scheduler_Base
{
    friend class Cyg_ThreadQueue_Implementation;
    friend class Cyg_SchedThread_Implementation;
    friend class Cyg_HardwareThread;
    friend void cyg_scheduler_set_need_reschedule();
    
    // Mask of which run queues have ready threads
    cyg_sched_bitmap    queue_map;

    // Each run queue is a double linked circular list of threads.
    // These pointers point to the head element of each list.
    Cyg_RunQueue run_queue[CYGNUM_KERNEL_SCHED_PRIORITIES];

#ifdef CYGPKG_KERNEL_SMP_SUPPORT

    // In SMP systems we additionally keep a counter for each priority
    // of the number of pending but not running threads in each queue.
    
    cyg_uint32 pending[CYGNUM_KERNEL_SCHED_PRIORITIES];

    cyg_sched_bitmap pending_map;

#endif    

protected:
    
#ifdef CYGSEM_KERNEL_SCHED_TIMESLICE

    // Timeslice counter. This is decremented on each
    // clock tick, and a timeslice is performed each
    // time it zeroes.
    
    static cyg_ucount32 timeslice_count[CYGNUM_KERNEL_CPU_MAX]
                                        CYGBLD_ANNOTATE_VARIABLE_SCHED;

#endif

    Cyg_Scheduler_Implementation();     // Constructor
    
    // The following functions provide the scheduler implementation
    // interface to the Cyg_Scheduler class. These are protected
    // so that only the scheduler can call them.
    
    // choose a new thread
    Cyg_Thread  *schedule();

    // make thread schedulable
    void        add_thread(Cyg_Thread *thread);

    // make thread un-schedulable
    void        rem_thread(Cyg_Thread *thread);

    // register thread with scheduler
    void        register_thread(Cyg_Thread *thread);

    // deregister thread
    void        deregister_thread(Cyg_Thread *thread);
    
    // Test the given priority for uniqueness
    cyg_bool    unique( cyg_priority priority);

    // Set need_reschedule if the supplied thread is of lower
    // priority than any that are currently running.
    static void set_need_reschedule( Cyg_Thread *thread );
    static void set_need_reschedule();

public:    void set_idle_thread( Cyg_Thread *thread, HAL_SMP_CPU_TYPE cpu );
    
#ifdef CYGSEM_KERNEL_SCHED_TIMESLICE

    // If timeslicing is enbled, define a scheduler
    // entry points to do timeslicing. This will be
    // called from the RTC DSR.
public:    
    void timeslice();
    void timeslice_cpu();

#endif

};

// -------------------------------------------------------------------------
// Cyg_Scheduler_Implementation inlines

inline void Cyg_Scheduler_Implementation::set_need_reschedule()
{
    need_reschedule[CYG_KERNEL_CPU_THIS()] = true;
}





/*class Cyg_Sys_Gen_Info 
{

public:
	Cyg_Sys_Gen_Info();
	static long ms_per_sys_tick;
	
};*/


// -------------------------------------------------------------------------
// Scheduler thread implementation.
// This class provides the implementation of the scheduler specific parts
// of each thread.

class Cyg_SchedThread_Implementation
    : public Cyg_DNode_T<Cyg_Thread>
{
    friend class Cyg_Scheduler_Implementation;
    friend class Cyg_ThreadQueue_Implementation;

protected:

    
#ifdef CYGPKG_KERNEL_SMP_SUPPORT
    HAL_SMP_CPU_TYPE    cpu;            // CPU id of cpu currently running
                                        // this thread, or CYG_KERNEL_CPU_NONE
                                        // if not running.
#endif
    
    Cyg_SchedThread_Implementation(CYG_ADDRWORD sched_info);

    void yield();                       // Yield CPU to next thread

    static void rotate_queue( cyg_priority pri );
                                        // Rotate that run queue

    void to_queue_head( void );         // Move this thread to the head
                                        // of its queue (not necessarily
                                        // a scheduler queue)
    
#ifdef CYGSEM_KERNEL_SCHED_TIMESLICE

    cyg_ucount32 timeslice_count;
        
    void timeslice_save();

    void timeslice_restore();
    
    void timeslice_reset();

#ifdef CYGSEM_KERNEL_SCHED_TIMESLICE_ENABLE

    // This defines whether this thread is subject to timeslicing.
    // If false, timeslice expiry has no effect on the thread.
    
    cyg_bool            timeslice_enabled;

public:
    
    void timeslice_enable();

    void timeslice_disable();
    
#endif

#else

    inline void timeslice_save() {};
    inline void timeslice_restore() {};
    inline void timeslice_reset() {};
    
#endif    
public:
	
    cyg_priority        priority;       // current thread priority
    cyg_tick_count deadline_tick_cnt;
    cyg_tick_count wcet_tick_cnt;
    cyg_tick_count period_tick_cnt;

/* start of the code by nodir.qodirov@gmail.com */
    
/* edf_schedule() used to calculate thread's EDF deadline value from its *wcet* and *period* 
    and assign priority based on calculated value*/
    static void edf_prep( void );
 
/* edf_priority() is used assign [default priorities] to newly created threads
 this value can be dynamically changed during execution */
    static cyg_count32 edf_priority( void );

/* edf_init() initializes clock frequency and basic priority for EDF threads */
    static void edf_init( void );

/* To make CYGNUM_LIBC_MAIN_THREAD_PRIORITY macro available from all places
    we need this macro for EDF scheduler in clock.cxx get_main_thread_priority() line #697 */
    static cyg_priority get_main_thread_priority(void);

/* edf_info pointer to EDF specific variable Cyg_SchedThread_Implementation */
    struct cyg_sched_edf_info_t *edf_info;

/* end of the code by nodir.qodirov@gmail.com */
};

// -------------------------------------------------------------------------
// Cyg_SchedThread_Implementation inlines.

#ifdef CYGSEM_KERNEL_SCHED_TIMESLICE

inline void Cyg_SchedThread_Implementation::timeslice_save()
{
    timeslice_count = Cyg_Scheduler_Implementation::timeslice_count[CYG_KERNEL_CPU_THIS()];
}

inline void Cyg_SchedThread_Implementation::timeslice_restore()
{
    Cyg_Scheduler_Implementation::timeslice_count[CYG_KERNEL_CPU_THIS()] = timeslice_count;
}

inline void Cyg_SchedThread_Implementation::timeslice_reset()
{
    timeslice_count = CYGNUM_KERNEL_SCHED_TIMESLICE_TICKS;
}

#ifdef CYGSEM_KERNEL_SCHED_TIMESLICE_ENABLE

inline void Cyg_SchedThread_Implementation::timeslice_enable()
{
    timeslice_enabled = true;
}

inline void Cyg_SchedThread_Implementation::timeslice_disable()
{
    timeslice_enabled = false;
}

#endif

#endif


// -------------------------------------------------------------------------
#endif // ifndef CYGONCE_KERNEL_EDF_HXX
// EOF edf.hxx
