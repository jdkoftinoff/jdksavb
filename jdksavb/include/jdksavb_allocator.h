#pragma once

/*
  Copyright (c) 2014, J.D. Koftinoff Software, Ltd.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

   3. Neither the name of J.D. Koftinoff Software, Ltd. nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/

#include "jdksavb_world.h"

#if JDKSAVB_ENABLE_MICROSUPPORT
#define jdksavb_new us_new
#define jdksavb_delete us_delete
#define jdksavb_new_array us_new_array
#define jdksavb_round_size us_round_size
#define jdksavb_allocator us_allocator_s
#define jdksavb_simple_allocator us_simple_allocator_s
#define jdksavb_simple_allocator_init us_simple_allocator_init
#define jdksavb_simple_allocator_reset us_simple_allocator_reset
#else

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup util Utilities */
/*@{*/

#ifndef JDKSAVB_ENABLE_MALLOC
/// If JDKSAVB_ENABLE_MALLOC compile time define is set to 0, then no usage of malloc and free will be used
#define JDKSAVB_ENABLE_MALLOC 1
#endif

/** \addtogroup alloc Memory Allocator */
/*@{*/

/** jdksavb_new

    A define which simulates c++ new() operator, except it accepts an
    allocator to allocate from.
    No initializers of the objects are called.
*/

#define jdksavb_new( ALLOCATOR, T ) ( T * )( ( ALLOCATOR )->alloc( ( ALLOCATOR ), (int32_t)sizeof( T ), 1 ) )

#define jdksavb_delete( ALLOCATOR, PTR )                                                                                       \
    do                                                                                                                         \
    {                                                                                                                          \
        if ( ( ALLOCATOR ) != 0 && ( PTR ) != 0 )                                                                              \
            ( ALLOCATOR )->free( ( ALLOCATOR ), ( PTR ) );                                                                     \
    } while ( false )

/** jdksavb_new_array

    A define which simulates c++ new [] operator, except it accepts an
    allocator to allocate from.
*/

#define jdksavb_new_array( ALLOCATOR, T, COUNT )                                                                               \
    ( T * )( ( ALLOCATOR )->alloc( ( ALLOCATOR ), (int32_t)sizeof( T ), ( int32_t )( COUNT ) ) )

/** jdksavb_round_size

    Calculate size in bytes rounded up to the nearest 32bit word size.
*/
#define jdksavb_round_size( VALUE )                                                                                            \
    ( ( ( VALUE ) + ( uint32_t )( sizeof( uint32_t ) ) - 1 ) & ( uint32_t ) ~( sizeof( uint32_t ) - 1 ) )

/** jdksavb_allocator

    The jdksavb_allocator implements a
    simple growing stack of allocations within a fixed buffer size
*/
struct jdksavb_allocator
{
    /**
       Destroy the allocator.
    */

    void ( *destroy )( struct jdksavb_allocator *self );

    /**
       Allocate memory from the allocator.
    */
    void *( *alloc )( struct jdksavb_allocator *self, int32_t length, int32_t count );

    void ( *free )( struct jdksavb_allocator *self, const void *ptr );
};

struct jdksavb_simple_allocator
{
    struct jdksavb_allocator base;
    /** pointer to the raw memory pool to allocate from */
    void *raw_memory;

    /** length of the raw memory pool */
    int32_t raw_memory_length;

    /** the current position in the raw memory pool */
    int32_t current_position;
};

/**
   jdksavb_simple_allocator_init
   Initialize an jdksavb_allocator_t object, attaching it to a block of
   memory
   with a specific size in bytes

   @param self object to initialize
   @param raw_memory pointer to raw memory
   @param raw_memory_length size of raw memory in bytes
   @returns pointer initialized object
*/

struct jdksavb_allocator *
    jdksavb_simple_allocator_init( struct jdksavb_simple_allocator *self, void *raw_memory, int32_t raw_memory_length );

void jdksavb_simple_allocator_reset( struct jdksavb_simple_allocator *self );

/** jdksavb_simple_allocator_destroy
    simple allocators are allocated from raw memory and have no need to
    do anything to destroy the pool of memory
*/

void jdksavb_simple_allocator_destroy( struct jdksavb_allocator *self );

/** jdksavb_simple_allocator_alloc

    allocate some memory from the allocator's memory pool.

    @param self allocator to allocate from
    @param length in bytes of object to allocate
    @param count count of objects to allocate
    @returns memory void * or NULL if error
*/
void *jdksavb_simple_allocator_alloc( struct jdksavb_allocator *self, int32_t length, int32_t count );

void jdksavb_simple_allocator_free( struct jdksavb_allocator *self, const void *ptr );

#if JDKSAVB_ENABLE_MALLOC

struct jdksavb_malloc_allocator
{
    struct jdksavb_allocator base;
};

/**
   jdksavb_malloc_allocator_init
   simple wrapper of malloc/free

   @param self object to initialize
   @returns pointer initialized object
*/

struct jdksavb_allocator *jdksavb_malloc_allocator_init( struct jdksavb_malloc_allocator *self );

/** jdksavb_malloc_allocator_destroy
    simple allocators are allocated from raw memory and have no need to
    do anything to destroy the pool of memory
*/

void jdksavb_malloc_allocator_destroy( struct jdksavb_allocator *self );

void *jdksavb_malloc_allocator_alloc( struct jdksavb_allocator *self, int32_t length, int32_t count );

void *jdksavb_malloc_allocator_realloc( struct jdksavb_allocator *self, const void *orig_ptr, int32_t length, int32_t count );

void jdksavb_malloc_allocator_free( struct jdksavb_allocator *self, const void *ptr );

#endif

/*@}*/

/*@}*/

#ifdef __cplusplus
}
#endif
#endif
