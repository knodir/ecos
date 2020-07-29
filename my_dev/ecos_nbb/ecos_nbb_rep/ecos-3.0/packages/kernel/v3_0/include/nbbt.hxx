#ifndef CYGONCE_KERNEL_NBBT_HXX
#define CYGONCE_KERNEL_NBBT_HXX

//==========================================================================
//
//      nbbt.hxx
//
//      NBBt (Non Blocking Buffer) class declarations
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
// Author(s):   nodir
// Contributors:      nodir
// Date:        2010-05-15
// Purpose:     Define NBBt class interfaces
// Description: The classes defined here provide the APIs for nbbtes.
// Usage:       #include <cyg/kernel/nbbt.hxx>
//              #include <cyg/kernel/nbbt.inl>
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <cyg/kernel/ktypes.h>
#include <cyg/infra/cyg_ass.h>            // assertion macros
#include <cyg/kernel/thread.hxx>

// -------------------------------------------------------------------------
// Message/Mail Box.  This template implements a queue of T's.
// Implemented as a template for maximal flexibility; one would hope
// that only one, with T==(void *) and the same number of them,
// is ever used without very good reason.

// Cyg_Mboxt has a fixed size array of T's; one size fits all.

template <class T>
class Cyg_NBBt
{
public:
	volatile cyg_uint32     uc; // Update counter
	volatile cyg_uint32     last_uc; // Last update counter	
	
	volatile cyg_uint32     ac; // Acknowledge counter
	volatile cyg_uint32     last_ac; // Last acknowledge counter

	T item_queue[CYGNUM_KERNEL_SYNCH_NBB_QUEUE_SIZE];

	Cyg_ThreadQueue     get_threadq;    // Queue of waiting threads to get
	Cyg_ThreadQueue     put_threadq;    // Queue of waiting threads to put
	
public:

	CYGDBG_DEFINE_CHECK_THIS
    
	Cyg_NBBt();                        // Constructor
	~Cyg_NBBt();                       // Destructor
        
	cyg_nbb_stat_t    put( const T ptr_to_item, T &def_item );    // put an item; wait if full
	//cyg_bool    tryput( const T item ); // fails if Q full
	
	cyg_nbb_stat_t    get( T &ritem );        // get an item; wait if none
	
	// private utility functions
	// wake up a thread from some queue
	inline void         wakeup_waiter( Cyg_ThreadQueue &q );

	//cyg_bool    tryget( T &ritem );     // just one attempt

	//cyg_bool    peek_item( T &ritem );  // get next item without
                                        // removing it

	/*inline
	cyg_bool    waiting_to_get()        // Any threads waiting?
	{
		return ! get_threadq.empty();
	}*/
	

};

// -------------------------------------------------------------------------
#endif // ifndef CYGONCE_KERNEL_NBBT_HXX
// EOF nbbt.hxx
