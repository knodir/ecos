#ifndef CYGONCE_KERNEL_NBBT_INL
#define CYGONCE_KERNEL_NBBT_INL
//==========================================================================
//
//      nbbt.inl
//
//      NBBT NBB template class implementation
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 1998, 1999, 2000, 2001, 2002, 2006 Free Software Foundation, Inc.
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
// Author(s):   nodir
// Contributors:        nodir
// Date:        2010-05-15
// Purpose:     NBBt template implementation
// Description: This file contains the implementations of the nbbt
//              template classes.
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/kernel.h>

#include <cyg/kernel/ktypes.h>         // base kernel types
#include <cyg/infra/cyg_trac.h>        // tracing macros
#include <cyg/infra/cyg_ass.h>         // assertion macros
#include <cyg/kernel/instrmnt.h>       // instrumentation

#include <cyg/kernel/nbbt.hxx>        // our header

#include <cyg/kernel/thread.inl>       // thread inlines
#include <cyg/kernel/sched.inl>        // scheduler inlines
#include <cyg/kernel/clock.inl>        // clock inlines

// -------------------------------------------------------------------------
// inline function for awakening waiting threads

template <class T>
inline void
Cyg_NBBt<T>::wakeup_waiter( Cyg_ThreadQueue &q )
{
    if( !q.empty() ) {
        // The queue is non-empty, so grab the next thread and wake it up.
        Cyg_Thread *thread = q.dequeue();

        CYG_ASSERTCLASS( thread, "Bad thread pointer");

        thread->set_wake_reason( Cyg_Thread::DONE );
        thread->wake();
        CYG_INSTRUMENT_MBOXT(WAKE, this, thread);        
    }
}

// -------------------------------------------------------------------------
// Constructor

template <class T>
Cyg_NBBt<T>::Cyg_NBBt()
{
	CYG_REPORT_FUNCTION();
	uc = 0; // Update counter
	last_uc = 0; // Last update counter	
	
	ac = 0; // Acknowledge counter
	last_ac = 0; // Last acknowledge counter
}

// -------------------------------------------------------------------------
// Destructor

template <class T>
Cyg_NBBt<T>::~Cyg_NBBt()
{
	CYG_REPORT_FUNCTION();
	//CYG_ASSERT( 0 == count, "Deleting NBBt with messages");
	CYG_ASSERT( get_threadq.empty(), "Deleting NBBt with threads waiting to get");
	CYG_ASSERT( put_threadq.empty(), "Deleting NBBt with threads waiting to put");
}

// -------------------------------------------------------------------------
// debugging/assert function

#ifdef CYGDBG_USE_ASSERTS

template <class T>
cyg_bool 
Cyg_NBBt<T>::check_this(cyg_assert_class_zeal zeal) const
{
    CYG_REPORT_FUNCTION();
        
    if ( Cyg_Thread::DESTRUCT == Cyg_Thread::self()->get_wake_reason() )
        // then the whole thing is invalid, and we know it.
        // so return OK, since this check should NOT make an error.
        return true;

    // check that we have a non-NULL pointer first
    if( this == NULL ) return false;

#if 0 // thread queues do not have checking funcs.
    if ( ! get_threadq.check_this( zeal ) ) return false;
    if ( ! put_threadq.check_this( zeal ) ) return false;
#endif

    switch( zeal )
    {
    case cyg_system_test:
    case cyg_extreme:
    case cyg_thorough:
    case cyg_quick:
    case cyg_trivial:
        // plenty of scope for fencepost problems here
        return false;
        
    case cyg_none:
    default:
        break;
    };
    return true;
}

#endif


// -------------------------------------------------------------------------
// From here downwards, these are the major functions of the template; if
// being genuinely used as a template they should probably not be inlined.
// If being used to construct a specific class, with explicit functions,
// then they should be.  This is controlled by:

#ifdef CYGIMP_NBBT_INLINE
#define CYG_NBBT_INLINE inline
#else
#define CYG_NBBT_INLINE
#endif


// -------------------------------------------------------------------------
// Get an item, or wait for one to arrive

template <class T>
CYG_NBBT_INLINE cyg_nbb_stat_t
Cyg_NBBt<T>::get( T &ritem )
{
	CYG_REPORT_FUNCTION();
	
	const cyg_uint32 buffer_size = (cyg_uint32) CYGNUM_KERNEL_SYNCH_NBB_QUEUE_SIZE;
	//CYG_TRACE1(1, "buffer_size = %d\n", buffer_size);
	
	cyg_uint32 temp_uc = uc; // 01
	
	if (temp_uc == last_ac) // 02
	{
		return BUFFER_EMPTY;
	}
	
	if (temp_uc-last_ac == 1) // 03
	{
		return BUFFER_EMPTY_BUT_PRODUCER_INSERTING;
	}
	
	ac = last_ac+1; // 04
	
	ritem = item_queue[ (last_ac/2) % buffer_size ]; // 05
	
	//CYG_TRACE1(1, "ritem = %d", (int *) ritem);
	
	ac = last_ac+2; // 06
	
	last_ac = ac; // 07
	
	CYG_TRACE1(1, "ac = %d\n", ac);
	
	return READ_DONE; // 08
}


/*template <class T>
CYG_NBBT_INLINE cyg_bool
Cyg_NBBt<T>::tryget( T &ritem )
{
    CYG_REPORT_FUNCTION();
        
    CYG_ASSERTCLASS( this, "Bad this pointer");
    
    CYG_INSTRUMENT_MBOXT(TRY, this, count);
    
    cyg_bool result = true;//( 0 < count );
    
    // If the mboxt is not empty, grab an item and return it.
    if ( result ) {
        ritem = itemqueue[ 0 ];
    }

    return result;    
}*/

// -------------------------------------------------------------------------
// get next item without removing it
/*template <class T>
CYG_NBBT_INLINE cyg_bool
Cyg_NBBt<T>::peek_item( T &ritem )
{
    CYG_REPORT_FUNCTION();
        
    CYG_ASSERTCLASS( this, "Bad this pointer");
    
    CYG_INSTRUMENT_MBOXT(TRY, this, count);
    
    cyg_bool result = true;//( 0 < count );
    
    // If the mboxt is not empty, grab an item and return it.
    if ( result )
        ritem = itemqueue[ 0 ];

    return result;    
}
*/

template <class T>
CYG_NBBT_INLINE cyg_nbb_stat_t
Cyg_NBBt<T>::put( const T ptr_to_item, T &def_item )
{
	CYG_REPORT_FUNCTION();
	//item_queue[ 0 ] = item;
	
	CYG_INSTRUMENT_MBOXT(GET, this, 5);
	
	const cyg_uint32 buffer_size = (cyg_uint32) CYGNUM_KERNEL_SYNCH_NBB_QUEUE_SIZE;
	//CYG_TRACE1(1, "buffer_size=%d\n", buffer_size);
	
	cyg_uint32 temp_ac = ac; // 01
	
	if ( (last_uc-temp_ac) == (buffer_size*2) ) // 02
	{
		def_item = NULL;
		return BUFFER_FULL;
	}
	
	if ( (last_uc-temp_ac) == (2*buffer_size-1) ) // 03
	{
		def_item = NULL;
		return BUFFER_FULL_BUT_CONSUMER_READING;
	}
	
	uc = last_uc+1; // 04
	
	def_item = item_queue[(last_uc/2) % buffer_size]; // 05
	
	item_queue[ (last_uc/2) % buffer_size] = ptr_to_item; // 06
	
	//def_item = item_queue[(last_uc/2) % buffer_size]; // 05 just to test
	
	//CYG_TRACE1(1, "current item = %d", (int *) item_queue[ (last_uc/2) % buffer_size] );
	//CYG_TRACE1(1, "index = %d", ((last_uc/2) % buffer_size));
	//CYG_TRACE1(1, "def_item=%d", (int *) def_item );
	
	uc = last_uc+2; // 07
	
	last_uc = uc; // 08
	
	CYG_TRACE1(1, "last_uc=%d\n", last_uc);
	CYG_TRACE1(1, "temp_ac=%d\n", temp_ac);
	
	//CYG_TRACE1(1, "my_ind=%d", my_ind);
	//CYG_TRACE1(1, "uc=%d", uc);
	
	return INSERT_DONE; // 09
}

// -------------------------------------------------------------------------
// Try to put an item in the queue and return success; queue may be full.

/*template <class T>
CYG_NBBT_INLINE cyg_bool
Cyg_NBBt<T>::tryput( const T item )
{
    CYG_REPORT_FUNCTION();
        
    CYG_INSTRUMENT_MBOXT(PUT, this, count);
    CYG_ASSERTCLASS( this, "Bad this pointer");

    //if ( size == count ) {
    //    Cyg_Scheduler::unlock();        // unlock, maybe switch threads
    //    return false;                   // the mboxt is full
    //}

    itemqueue[ 0 ] = item;

    CYG_ASSERTCLASS( this, "Bad this pointer");    

    wakeup_waiter( get_threadq );
    
    return true;
}
*/

// -------------------------------------------------------------------------
#endif // ifndef CYGONCE_KERNEL_NBBT_INL
// EOF nbbt.inl
