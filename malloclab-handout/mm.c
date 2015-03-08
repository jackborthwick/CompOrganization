/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "GINYU FORCE",
    /* First member's full name */
    "Jack Borthwick",
    /* First member's email address */
    "jackmckeanborthwick@gmail.com",
    /* Second member's full name (leave blank if none) */
    "LORD FRIEZA",
    /* Second member's email address (leave blank if none) */
    "All Hail Planet Vegeta"
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* global vars */
#define WSIZE 4  //word header size
#define DSIZE 8 // double word size
#define CHUNKSIZE (1<<12) //extend heap by this amount

#define MAX(x,y) ((x) > (y)? (x) : (y))
//pack size and allocated bit into word
#define PACK(size, alloc) ((size) | (alloc))

//read and write a word at address p
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

//read the size and allocated fields from address p
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

//given block ptr bp, compute address of header and foots
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp)  ((char *)(bp) +GET_SIZE(HDRP(bp))-DSIZE)

//given block ptr bp compue address of next and previous blocks
#define NEXT_BLKBP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKBP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

int coalesceCount;
static void *coalesce(void *bp){
	size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKBP(bp)));
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKBP(bp)));
	size_t size = GET_SIZE(HDRP(bp));

	if (prev_alloc && next_alloc){
		return bp;
	}
	else if (prev_alloc && !next_alloc){
		size += GET_SIZE(HDRP(NEXT_BLKBP(bp)));
		PUT(HDRP(bp), PACK(size,0));
		PUT(FTRP(bp), PACK(size,0));
	}
	else if (!prev_alloc && next_alloc){
		size += GET_SIZE(HDRP(PREV_BLKBP(bp)));
		PUT(FTRP(bp), PACK(size, 0));
		PUT(HDRP(PREV_BLKBP(bp)), PACK(size, 0));
		bp = PREV_BLKBP(bp);
	}
	else{
		size += GET_SIZE(HDRP(PREV_BLKBP(bp))) + GET_SIZE(FTRP(NEXT_BLKBP(bp)));
		PUT(HDRP(PREV_BLKBP(bp)), PACK(size, 0));
		PUT(FTRP(NEXT_BLKBP(bp)), PACK(size, 0));
		bp = PREV_BLKBP(bp);
	}
	return bp;
}


static void *extend_heap(size_t words){
	char *bp;
	size_t size;

	//allocates even number of words to maintain alignment
	size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
	if ((long)(bp = mem_sbrk(size)) == -1)
		return NULL;

	//initialize freeblock header/footer and epilogue header
	PUT(HDRP(bp), PACK(size, 0)); //free block header
	PUT(FTRP(bp), PACK(size, 0));  //free block footer
	PUT(HDRP(NEXT_BLKBP(bp)), PACK(0, 1)); //new epilogue header

	//coalesce if previous block was free
	return coalesce(bp);
}

/* 
 * mm_init - initialize the malloc package.
 */
static char *heap_listp;
int mm_init(void)
{
	if ((heap_listp =  mem_sbrk(4*WSIZE)) == (void *)-1)
		return -1;
	PUT(heap_listp, 0);			//alligntment padding
	PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1)); //prologue header
    PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1)); //prologue footer
    PUT(heap_listp + (3*WSIZE), PACK(0,1)); //epilogue header
    heap_listp += (2*WSIZE);

    //extend empty head with a free block of chunk size
    if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
    	return -1;
    return 0;
}
static void *find_fit(size_t asize){
	void *bp;
	for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKBP(bp)){
		if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))){
			return bp;
		}
	}
	return NULL; //no fit
}

static void place(void *bp, size_t asize)
{
	size_t csize = GET_SIZE(HDRP(bp));
	if ((csize - asize) >= (2*DSIZE)){
		PUT(HDRP(bp), PACK(asize, 1));
		PUT(FTRP(bp), PACK(asize,1));
		bp=NEXT_BLKBP(bp);
		PUT(HDRP(bp), PACK(csize - asize, 0));
		PUT(FTRP(bp), PACK(csize - asize, 0));
	}
	else {
		PUT(HDRP(bp), PACK(csize, 1));
		PUT(FTRP(bp), PACK(csize, 1));
	}
}
int checkFree(void *ptr){
	//size_t prev = GET_ALLOC(FTRP(PREV_BLKBP(ptr)));
	size_t next = GET_ALLOC(HDRP(NEXT_BLKBP(ptr)));
	size_t curr = GET_ALLOC(HDRP(ptr));
	int areFree=0;
	if (!curr && !next){
		areFree=1;
	}
	return areFree;
}
//heap checker
int heapchecker()
{
	void *ptr = heap_listp;
	int contiguousFrees = 0;
	while(GET_SIZE(HDRP(ptr))>0){
		if(checkFree(ptr)){
			contiguousFrees++;
		}
		ptr=NEXT_BLKBP(ptr);
	}
	printf("%d\n", contiguousFrees);
	return 0;
}









/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
	size_t asize;
	size_t newsize;
	char *bp;

	if (size==0)
		return NULL;

	if(size <= DSIZE){
		asize = 2*DSIZE;
	}
	else
		asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);

	if ((bp = find_fit(asize)) != NULL){
		place(bp, asize);
		return bp;
	}

	newsize = MAX(asize,CHUNKSIZE);
	if((bp = extend_heap(newsize/WSIZE)) == NULL)
		return NULL;
	place(bp, asize);
	//heapchecker();
	return bp;
    //int newsize = ALIGN(size + SIZE_T_SIZE);
    //void *p = mem_sbrk(newsize);
    //if (p == (void *)-1)
	//return NULL;
    //else {
    //    *(size_t *)p = size;
    //    return (void *)((char *)p + SIZE_T_SIZE);
    //}
}


/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *bp)
{
	size_t size = GET_SIZE(HDRP(bp));

	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));
	if (coalesceCount%4 == 0){
		coalesce(bp);
	}
	else{
		coalesceCount++;
		//printf("%d",coalesceCount );
	} 
	//printf("%d",coalesceCount );
}







/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */


void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    if (ptr == NULL){
    	return NULL;
    }
    if (size==0){
    	mm_free(ptr);
    	return NULL;
    }
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    if(size <= DSIZE){  //if the input is less than the min
		copySize = 2*DSIZE; //set size to d*2
	}
	else  //else make the size same way we did in malloc
		copySize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}








