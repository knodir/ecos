//==========================================================================
//
//      nbb.cxx
//
//      Non-Blocking Buffer (NBB) class implementation
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
// Author(s):   hmt
// Contributors:        hmt
// Date:        2010-02-01
// Purpose:     NBB implementation
// Description: This file contains the implementations of the NBB class
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/kernel.h>

#include <cyg/kernel/ktypes.h>         // base kernel types
#include <cyg/infra/cyg_trac.h>        // tracing macros
#include <cyg/infra/cyg_ass.h>         // assertion macros
#include <cyg/kernel/instrmnt.h>       // instrumentation

#include <cyg/kernel/thread.inl>       // Cyg_Thread inlines

#include <cyg/kernel/nbb.hxx>         // our own header
#define CYGIMP_NBBT_INLINE            // of implementation

#include <cyg/kernel/nbbt.inl>        // mbox template implementation

// -------------------------------------------------------------------------
// This module exists to cause exactly one instance of these functions to
// exist; this is just like a vanilla class, except we use the template
// class to acquire an implementation.  The template functions are inlined
// in each of these methods.


// -------------------------------------------------------------------------
// Constructor
Cyg_NBB::Cyg_NBB()
{
}

// -------------------------------------------------------------------------
// Destructor
Cyg_NBB::~Cyg_NBB()
{
}



// -------------------------------------------------------------------------
// debugging/assert function

#ifdef CYGDBG_USE_ASSERTS
	cyg_bool 
	Cyg_NBB::check_this(cyg_assert_class_zeal zeal) const
	{
		return m.check_this(zeal);
	}
#endif

// -------------------------------------------------------------------------
// now the members themselves:

//cyg_nbb_stat_t Cyg_NBB::get( void * ref_to_item )
//void * Cyg_NBB::get( cyg_nbb_stat_t & stat )
void * Cyg_NBB::get( cyg_bool & res )
{
	CYG_REPORT_FUNCTION();
	//CYG_TRACE1(1, "inside Cyg_NBB::get()", true);
	
	void * ref_to_item = NULL;
	cyg_bool result = true;
        Cyg_Thread *self = Cyg_Thread::self();

	// Loop while the NBB is empty, sleeping each time around
	// the loop. This copes with the possibility of a higher priority
	// thread grabbing the message between the wakeup in unlock() and
	// this thread actually starting.
    
	//while( result && (0 == count) ) {
	//while( result && ((stat = m.get(ref_to_item)) != READ_DONE ) ) {
	cyg_nbb_stat_t stat = m.get(ref_to_item);
	
	if ( stat == READ_DONE ) {
		res = true;
		CYG_TRACE1(1, "READ_DONE ", true); 
	} else {
	/*	res = false;
		CYG_TRACE1(1, "BUFFER_EMPTY", true); 
	} */
		while ( result )
		{
			self->set_sleep_reason( Cyg_Thread::WAIT );
			self->sleep();
			m.get_threadq.enqueue( self );

			CYG_INSTRUMENT_MBOXT(WAIT, this, 5);
        
			CYG_ASSERTCLASS( this, "Bad this pointer");

			// Allow other threads to run
			Cyg_Scheduler::reschedule();
			CYG_TRACE1(1, "AFTER RESCHED", true); 

			switch( self->get_wake_reason() )
			{
				case Cyg_Thread::DESTRUCT:
				case Cyg_Thread::BREAK:
					result = false;
					break;
		
				case Cyg_Thread::EXIT:            
					self->exit();
					break;
				default:
					break;
			}
		}

		if ( result ) 
		{
			CYG_INSTRUMENT_MBOXT(GOT, this, 5);
			stat = m.get(ref_to_item);
			m.wakeup_waiter( m.put_threadq );
		}
		CYG_ASSERTCLASS( this, "Bad this pointer");
		//CYG_REPORT_RETVAL( result );
	}
	if ( stat == READ_DONE ){
		CYG_TRACE1(1, "READ_DONE", true); 
	} else if ( stat == BUFFER_EMPTY ){
		CYG_TRACE1(1, "BUFFER_EMPTY", true); 
	} else {
		CYG_TRACE1(1, "SMTH_ELSE", true); 
	}
	
	CYG_TRACE1(1, "ref_to_item = %d", (int *) ref_to_item);
	
	return ref_to_item;
}

//cyg_nbb_stat_t Cyg_NBB<T>::put( T &ptr_to_item, T &ptr_to_defunct_item )
cyg_bool Cyg_NBB::put( void *ptr_to_item )
{
	CYG_REPORT_FUNCTION();
	//CYG_TRACE1(1, "inside Cyg_NBB put()", true);
	
	cyg_bool res = false;
	cyg_bool result = true;
	
	void *ptr_to_defunct_item = NULL;
	
	cyg_nbb_stat_t stat = m.put( ptr_to_item, ptr_to_defunct_item );
	
	if (( stat == BUFFER_FULL ) ||
		( stat == BUFFER_FULL_BUT_CONSUMER_READING))
	{
		CYG_TRACE1(1, "Inside BUFFER_FULL", true); 
		Cyg_Thread *self = Cyg_Thread::self();

		CYG_INSTRUMENT_MBOXT(PUT, this, 5);
		CYG_ASSERTCLASS( this, "Bad this pointer");

		//while ( result && (uc == ac) ) 
		//while ( result && (m.put( ptr_to_item, ptr_to_defunct_item ) != INSERT_DONE ) ) 
		while ( result ) 
		{
			CYG_TRACE1(1, "BUFFER_FULL & waiting for INSERT_DONE", true); 
			self->set_sleep_reason( Cyg_Thread::WAIT );
			self->sleep();
			m.put_threadq.enqueue( self );

			CYG_INSTRUMENT_MBOXT(WAIT, this, 5);

			// Allow other threads to run
			Cyg_Scheduler::reschedule();

			CYG_ASSERTCLASS( this, "Bad this pointer");        

			switch( self->get_wake_reason() )
			{
				case Cyg_Thread::DESTRUCT:
				case Cyg_Thread::BREAK:
					result = false;
					break;
				case Cyg_Thread::EXIT:
					self->exit();
					break;

				default:
					break;
			}
		}

		if ( result ) 
		{
			stat = m.put( ptr_to_item, ptr_to_defunct_item );
			m.wakeup_waiter( m.get_threadq );
		}
		CYG_ASSERTCLASS( this, "Bad this pointer");    
    
		//CYG_REPORT_RETVAL( result );
		res = false;
		
	} else if ( stat == INSERT_DONE ) {
		res = true;
		CYG_TRACE1(1, "INSERT_DONE", true); 
	} else {
		CYG_TRACE1(1, "SMTH_ELSE", true); 
	}
	
	//CYG_TRACE1(1, "ptr_to_item = %d\n", (int *) ptr_to_item );
	//CYG_TRACE1(1, "ptr_to_def_item = %d\n", (int *) ptr_to_defunct_item );
	
	//CYG_TRACE1(1, "res_true = %d\n", res);
	
	return res;
}

/*
cyg_bool Cyg_NBB::tryput( void *item )
{
	CYG_TRACE1(1, "inside Cyg_NBB tryput()", true);
	return 1;//m.tryput( item );
}

void *
Cyg_NBB::peek_item()                   // Get next item to be returned
{
	CYG_TRACE1(1, "inside Cyg_NBB peek_item()", true);
	void *p=NULL;
	//if ( ! m.peek_item( p ) )
	//return NULL;
	return p;
}*/

// -------------------------------------------------------------------------
// EOF nbb.cxx