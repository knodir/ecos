#ifndef CYGONCE_KERNEL_KTYPES_H
#define CYGONCE_KERNEL_KTYPES_H

//==========================================================================
//
//      ktypes.h
//
//      Standard types used in the kernel and its interfaces
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
// Author(s):   nickg from an original by hmt
// Contributors:  nickg
// Date:        1997-09-08
// Purpose:     Define kernel specific types
// Description: Kernel specific types
// Usage:       #include <cyg/kernel/ktypes.h>
//              ...
//              cyg_priority priority; // etc
//              
//####DESCRIPTIONEND####
//
//==========================================================================

// -------------------------------------------------------------------------
// Check that a configuration file is present.

#ifndef CYGONCE_PKGCONF_KERNEL_H
#error "No Configuration file included"
#endif

// -------------------------------------------------------------------------

#include <cyg/infra/cyg_type.h>
#include <stddef.h>              // for size_t

// -------------------------------------------------------------------------
// Integer types:

typedef cyg_int32       cyg_code;       // General return/error/status code

typedef cyg_count32     cyg_priority;   // priority value

typedef cyg_uint32      cyg_vector;     // vector number/descriptor

typedef cyg_uint64      cyg_tick_count; // clock tick count value

struct cyg_sched_edf_info_t 
{
	// deadline of the thread to be finished first execution before deadline value in milliseconds
	cyg_tick_count deadline; 
	// Worst Case Execution Time of the thread in milliseconds
	cyg_tick_count wcet;
	// Thread's frequency 
	// Thread is executed once during specified period in milliseconds
	// Used for feasability of the schedule with given (deadline, wcet and period)
	cyg_tick_count period;
};

// -------------------------------------------------------------------------
// Predefinitions of various kernel classes

#ifdef __cplusplus

class Cyg_Scheduler;
class Cyg_Scheduler_Implementation;

class Cyg_HardwareThread;
class Cyg_SchedThread;
class Cyg_SchedThread_Implementation;
class Cyg_Thread;

class Cyg_ThreadQueue;
class Cyg_ThreadQueue_Implementation;

#endif


// -------------------------------------------------------------------------
// Class and structure conversion macros.
// CYG_CLASSFROMFIELD translates a pointer to a field of a struct or
// class into a pointer to the class.
// CYG_OFFSETOFBASE yields the offset of a base class of a derived
// class.
// CYG_CLASSFROMBASE translates a pointer to a base class into a pointer
// to a selected derived class. The base class object _must_ be part of
// the specified derived class. This is essentially a poor mans version
// of the RTTI dynamic_cast operator.
// Caveat: These macros do not work for virtual base classes.

#define CYG_CLASSFROMFIELD(_type_,_member_,_ptr_)\
    ((_type_ *)((char *)(_ptr_)-((char *)&(((_type_ *)0)->_member_))))

#ifdef __cplusplus

#define CYG_OFFSETOFBASE(_type_,_base_)\
    ((char *)((_base_ *)((_type_ *)4)) - (char *)4)

# define CYG_CLASSFROMBASE(_class_,_base_,_ptr_)\
    ((_class_ *)((char *)(_ptr_) - CYG_OFFSETOFBASE(_class_,_base_)))

#endif

// -------------------------------------------------------------------------
#endif // CYGONCE_KERNEL_KTYPES_H
// EOF ktypes.h

