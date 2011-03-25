/*-------------------------------------------------------------------------*/
/**
   @file    mmapi.c
   @author  N. Devillard
   @brief   Interface to map/unmap local files
*/
/*--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
   								Includes
 ---------------------------------------------------------------------------*/
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mmapi.h"

/* mmap_open ---------------------------------------------------------------*/
mmap_t * mmap_open(char * filename, size_t max_size, int mode)
{
	mmap_t  	*	mm ;
	char		*	buf ;
	int				fd ;
	struct stat		fileinfo ;

    /* Find out file size in bytes (POSIX compliant) */
    if (stat(filename, &fileinfo)!=0) {
        perror("stat");
        return NULL ;
    }

    if (max_size==0 && fileinfo.st_size>0) {
        /* max_size not requested and file has non-zero size:
         * map the whole contents
         */
        max_size=fileinfo.st_size;
    }
    if (fileinfo.st_size>0 && max_size>fileinfo.st_size) {
        max_size=fileinfo.st_size;
    }
	if ((fd = open(filename, O_RDONLY)) == -1) {
        perror("open");
		return NULL ;
	}
	buf = (char*)mmap(0,
					  max_size,
					  PROT_READ | PROT_WRITE,
					  mode,
					  fd,
					  0) ;
    close(fd);
	if (buf==(char*)-1) { perror("mmap") ; return NULL ; }

	mm = malloc(sizeof(mmap_t)) ;
	mm->fd 	 = fd ;
	mm->buf  = buf ;
	mm->size = max_size ;
    mm->filename = strdup(filename) ;
	return mm ;
}

/* mmap_close() ------------------------------------------------------------*/
void mmap_close(mmap_t * mm)
{
	if (mm==NULL) return ;
	if (munmap(mm->buf, mm->size)!=0) perror("munmap") ;
    if (mm->filename) free(mm->filename) ;
	free(mm);
	return ;
}

#ifdef MAIN
int main(int argc, char *argv[])
{
    char    * filename ;
    size_t    max_size ;
    mmap_t  * mm ;

    if (argc<2) {
        printf("use: %s filename [maxsize]\n", argv[0]);
        return 1 ;
    }
    filename = argv[1];
    max_size = 0 ;
    if (argc>2) {
        max_size = (size_t)atoi(argv[2]);
    }

    mm = mmap_open(filename, max_size, MAP_PRIVATE);
    if (!mm) {
        printf("cannot map: %s\n", filename);
        return -1 ;
    }
    printf("--contents: %s\n", argv[1]);
    printf("%s\n", mm->buf);
    printf("--\n");
    mmap_close(mm);
    return 0 ;
}
#endif
/* vim: set ts=4 et sw=4 tw=75 */
