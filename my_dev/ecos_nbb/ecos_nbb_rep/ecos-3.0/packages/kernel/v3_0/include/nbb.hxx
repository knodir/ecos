
//==========================================================================
//
//      nbb.hxx
//
//      Non-Blocking Buffer (NBB) class declarations
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
// Contributors:  
// Date:        2010-02-01
// Purpose:     Define NBB class interfaces
// Description: The classes defined here provide the APIs for NBBs.
// Usage:       #include <cyg/kernel/nbb.hxx>
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <cyg/kernel/ktypes.h>
#include <cyg/infra/cyg_ass.h>            // assertion macros
#include <cyg/kernel/thread.hxx>
#include <cyg/kernel/kapi.h>
#include <cyg/kernel/nbbt.hxx>

// -------------------------------------------------------------------------
// Message/Mail Box.  This class implements a queue of void * items using
// the Cyg_Mbox<Type, QSize> template class.

#ifndef CYGNUM_KERNEL_SYNCH_NBB_QUEUE_SIZE
// default is 10 elements
#define CYGNUM_KERNEL_SYNCH_NBB_QUEUE_SIZE (10)
#endif

// Cyg_Mbox has a fixed size array of (void *)s; one size fits all.
// Because of this, we can simplify the API by returning a NULL for
// "failed" conditions.  Ergo a NULL message is illegal.  BFD.

class Cyg_NBB
{
public:
	Cyg_NBBt<void *> m;

public:
	CYGDBG_DEFINE_CHECK_THIS
    
	Cyg_NBB();                         // Constructor
	~Cyg_NBB();                        // Destructor
    
	//cyg_nbb_stat_t get( void * ref_to_item );
	//void * get( cyg_nbb_stat_t & stat ); // return pointer to the item	
	void * get( cyg_bool & stat ); // return pointer to the item	

	//cyg_nbb_stat_t put( const T &ptr_to_item, T &ptr_to_defunct_item );
	cyg_bool put( void *ptr_to_item ); // put pointer value to the slot

	/*void *      peek_item();            // Get next item to be returned
                                        // without removing it

	inline cyg_count32 peek()                  // Get current count value
	{
		return 15;//m.peek();
	}

	inline  cyg_bool    waiting_to_get()        // Any threads waiting to get?
	{
		return 1;// m.waiting_to_get();
	}
	inline cyg_bool    waiting_to_put()        // Any threads waiting to put?
	{
		return 1;//m.waiting_to_put();
	}*/
};

// -------------------------------------------------------------------------
// End of nbb.hxx