/*-------------------------------------------------------------------------*/
/**
   @file    mmapi.h
   @author  N. Devillard
   @brief   Interface mmap/munmap

   This pair of functions offers a hopefully simpler interface
   to map/unmap files into memory using the system mmap/munmap calls.

   When opening a new mapped file, a maximum size (in bytes) can be provided
   to limit the mapping the first max_size bytes of the file. Providing
   max_size=0 maps the entire file into memory.

   A mapped file can be opened in two modes:
   MAP_PRIVATE: all modifications to the returned buffer will only be
   visible to the calling process.
   MAP_SHARED: all modifications are visible to all processes mapping
   the same file and will be propagated to disk.

   A common use of these functions is to map special devices like
   /dev/zero to allocate a block of memory.

*/
/*--------------------------------------------------------------------------*/
#ifndef _MMAPI_H_
#define _MMAPI_H_

/*---------------------------------------------------------------------------
   								New types
 ---------------------------------------------------------------------------*/
typedef struct _mmap_t_ {
    int        fd ;
    char  *    buf ;
    size_t     size ;
    char  *    filename ;
} mmap_t ;

/*---------------------------------------------------------------------------
  							Function prototypes
 ---------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/**
  @brief	Map a file to memory
  @param    filename    Name of the file to map
  @param    max_size    Maximum number of bytes to map from file start
  @param    mode        MAP_SHARED or MAP_PRIVATE
  @return   Pointer to a newly allocated mmap_t structure

  The returned pointer is a newly allocated mmap_file structure that must
  be deallocated by the caller using mmap_close(). Returns NULL in case of
  error.

  It max_size has a non-zero value, the mapping will be limited to the
  first max_size bytes of the file. If max_size is zero, the whole file
  will be mapped to memory.

  mode can be:
  - MAP_PRIVATE: changes to the mapped file are only visible to the caller
  and will not propagate to other processes reading the same file or on the
  filesystem.
  - MAP_SHARED: changes to the mapped file are shared with other processes
  mapping the same file and eventually carried out to the underlying file
  on the filesystem
  
 */
/*--------------------------------------------------------------------------*/
mmap_t * mmap_open(char * filename, size_t max_size, int mode);

/*-------------------------------------------------------------------------*/
/**
  @brief	Close an open mmap_file structure
  @param    mm      Memory-mapped file to close
  @return   void
 */
/*--------------------------------------------------------------------------*/
void mmap_close(mmap_t * mm);

#endif
/* vim: set ts=4 et sw=4 tw=75 */
