#include "dbgmem.h"
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#ifndef min
#	define min(a,b) ((a) < (b)) ? (a) : (b)
#endif


/* re-ordered parameters!  for varargs, format string always come before varargs, so to avoid confusion,
let's keep it that way.
e.g. dbgmem_log("This is a log: %s, %d\n", "filename", 5, "This is the actual string", 5);
Very confusing which are the parameters!
*/
void dbgmem_log(const char * file, int line, const char * fmt, ...)
{
    va_list argptr;
    va_start( argptr, fmt );

    /* TODO: if multi-threaded, lock this section.  Don't want multiple threads logging to console at the same time */
    printf("%s:%d: ", file, line);
    vprintf(fmt, argptr);

    va_end(argptr);
}

/* Definition of the structure describing memory */

/* Double chained circular list manipulation macros */
typedef struct dbgmem_list_entry_s
{
    struct dbgmem_list_entry_s * next;
    struct dbgmem_list_entry_s * prev;
} dbgmem_list_entry_t;

#define dbgmem_list_init( entry ) \
	do { (entry)->prev = (entry)->next = (entry); } while ( 0 )
#define dbgmem_list_isempty( list ) \
	((list)->next == (list))
#define dbgmem_list_entry( ptr, type, member ) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))
#define dbgmem_list_head( list ) \
    (list)->next
/* add tail, seems to add to head?? - LM */
#define dbgmem_list_add_tail( list, entry ) \
	do { (entry)->next = (list); \
    (entry)->prev = (list)->prev; \
    (list)->prev->next = (entry); \
	(list)->prev = (entry); } while ( 0 )
#define dbgmem_list_remove( entry ) \
	do { assert( (entry)->next != 0 ); \
    (entry)->prev->next = (entry)->next; \
	(entry)->next->prev = (entry)->prev; } while ( 0 )

static const unsigned long DBGMEM_CHECKER = 0x01020304; /* static const here to be able to get &DBGMEM_CHECKER for memory ops */
#define DBGMEM_CHECKER_SIZE 4	/* A way to be assured that checker and size matches actually ??? */

#define DBGMEM_FILENAME_MAXLENGTH 32
typedef struct dbgmem_block_s
{
    unsigned long checker;
    dbgmem_list_entry_t list;
    size_t size;
    char filename[DBGMEM_FILENAME_MAXLENGTH+1];
    int line;
} dbgmem_block_t;

/* Managing a memory pool */
static dbgmem_list_entry_t dbgmem_blocklist;

void dbgmem_debug_heap_init(void)
{
    memset(&dbgmem_blocklist, 0, sizeof(dbgmem_list_entry_t));
    dbgmem_list_init(&dbgmem_blocklist);
}

void dbgmem_hexdump( void* block, size_t bytes )
{
    unsigned char* p = (unsigned char*)block;
    unsigned char* end = p + bytes;
    const char* hextable = "0123456789abcdef";	/* Lookup table for Speed! :-) */

    printf("Dump: \t");
    for ( ; p < end; ++ p )
    {
        if ( isalnum(*p) )
            fputc(*p, stdout);
        else
        {
            fputc('\\', stdout);
            fputc('x', stdout);
            fputc(hextable[ (*p >> 4) & 0x0f ], stdout);
            fputc(hextable[ *p &0x0f ], stdout);
        }
    }
    fputc('\n', stdout);
}

int dbgmem_dump_blocks()
{
    dbgmem_list_entry_t* entry = dbgmem_list_head( &dbgmem_blocklist );
    while ( entry != &dbgmem_blocklist )
    {
        dbgmem_block_t* block = dbgmem_list_entry( entry, dbgmem_block_t, list );
        dbgmem_log(block->filename, block->line, "Leaked %d bytes [0x%08x]\n", block->size, block + 1);
        dbgmem_hexdump( block + 1, min(100, block->size) );
        entry = entry->next;
    }

    if ( dbgmem_list_isempty(&dbgmem_blocklist ) )
    {
        printf("No memory leaks detected.\n");
        return 1;
    }
    return 0;
}

void dbgmem_debug_heap_fini(void)
{
    dbgmem_dump_blocks();
}


void * dbgmem_store( size_t size, const char* filename, int line )
{
    dbgmem_block_t * block;

    block = (dbgmem_block_t*) malloc(sizeof(dbgmem_block_t) + size + DBGMEM_CHECKER_SIZE);

    if (block == NULL)
    {
        dbgmem_log(filename, line, "Error: malloc returned NULL");
        return NULL;
    }

    block -> checker = DBGMEM_CHECKER;
    block -> size = size;
    strncpy( block -> filename, filename, DBGMEM_FILENAME_MAXLENGTH );
    block->filename[DBGMEM_FILENAME_MAXLENGTH] = '\0';	/* Read strncpy doc, does not null terminate if exceeds length */
    block -> line = line;

    /* Copy the checker at the end of the memory block */
    memcpy( (char*)block + sizeof(*block) + size, &DBGMEM_CHECKER, sizeof( DBGMEM_CHECKER ) );

    dbgmem_list_add_tail(&dbgmem_blocklist, &block -> list);

    return block + 1;
}

void * dbgmem_mvstore( void* ptr, size_t size, const char* filename, int line )
{
    dbgmem_block_t * block;

    if ( ptr == NULL ) return NULL;

    block = (dbgmem_block_t *) ptr - 1 ;
    if ( block -> checker != DBGMEM_CHECKER )
    {
        dbgmem_log(filename, line, "Error: Corrupted Memory.");
        assert(0);
    }

    /* Compare the checker at the end of the memory block */
    if ( 0 != memcmp( (char*)ptr + block->size, &DBGMEM_CHECKER, sizeof( DBGMEM_CHECKER ) ) )
    {
        dbgmem_log(filename, line, "Error: Checker corrupted.");
        assert(0);
    }

    block = (dbgmem_block_t*) realloc(block, sizeof(dbgmem_block_t) + size + DBGMEM_CHECKER_SIZE);

    if ( block -> checker != DBGMEM_CHECKER )
    {
        dbgmem_log(filename, line, "Error: Corrupted Memory after realloc.");
        assert(0);
    }

    if (block == NULL)
    {
        dbgmem_log(filename, line, "Error: realloc returned NULL");
        return NULL;
    }


    block -> size = size;
    strncpy( block -> filename, filename, DBGMEM_FILENAME_MAXLENGTH );
    block->filename[DBGMEM_FILENAME_MAXLENGTH] = '\0';	/* Read strncpy doc, does not null terminate if exceeds length */
    block -> line = line;

    /* Copy the checker at the end of the memory block */
    memcpy( (char*)block + sizeof(*block) + size, &DBGMEM_CHECKER, sizeof( DBGMEM_CHECKER ) );

    /* position in list remains the same */

    return block + 1;
}


void dbgmem_remove( void* ptr, const char* filename, int line  )
{
    dbgmem_block_t * block;

    if ( ptr == NULL ) return;

    block = (dbgmem_block_t *) ptr - 1 ;
    if ( block -> checker != DBGMEM_CHECKER )
    {
        dbgmem_log(filename, line, "Error: Corrupted Memory.");
        assert(0);
    }

    /* Compare the checker at the end of the memory block */
    if ( 0 != memcmp( (char*)ptr + block->size, &DBGMEM_CHECKER, sizeof( DBGMEM_CHECKER ) ) )
    {
        dbgmem_log(filename, line, "Error: Checker corrupted.");
        assert(0);
    }

    /* This looks painfully slow on a huge system!! */
    dbgmem_list_remove(&block -> list);
    free(block);
}


/* To overload C standard memory functions */
void * dbgmem_calloc( size_t num, size_t size , const char* filename, int line)
{
    void* ptr;
    size_t tsize;

    if ( no_dbgmem )
    {
        return calloc( num, size );
    }

    tsize = num * size;
    ptr = dbgmem_store( size, filename, line);

    if ( ptr == NULL )
    {
        dbgmem_log(filename, line, "calloc of %d bytes [0x%08x] FAILED !\n", tsize, ptr);
        return NULL;
    }

    memset( ptr, 0, size ); /* calloc specificity : all bits 0 */
    dbgmem_log(filename, line, "calloc of %d bytes [0x%08x] OK.\n", tsize, ptr);
    return ptr;
}

void * dbgmem_malloc( size_t size, const char* filename, int line)
{
    void* ptr;
    if ( no_dbgmem ) return malloc( size );

    ptr = dbgmem_store( size, filename, line);
    if ( ptr == NULL )
    {
        /*dbgmem_log(filename, line, "malloc of %d bytes [0x%08x] FAILED !\n", size, ptr);*/
        return NULL;
    }

    /*dbgmem_log( filename, line, "malloc of %d bytes [0x%08x] OK.\n", size, ptr);*/
    return ptr;
}

void * dbgmem_realloc( void *ptr, size_t size, const char* filename, int line)
{
    void* newptr;

    if ( no_dbgmem ) return realloc( ptr, size );
    if ( ptr == NULL ) return dbgmem_malloc( size, filename, line);

    if ( size == 0 )
    {
        dbgmem_remove( ptr, filename, line);
        /*dbgmem_log(filename, line, "memory freed on realloc of %d bytes [0x%08x]",size, ptr);*/
        return NULL;
    }

    newptr = dbgmem_mvstore(ptr, size, filename, line);

    /*dbgmem_log( filename, line, "realloc of %d bytes [0x%08x] OK.\n", size, ptr);*/
    return newptr;
}

char* dbgmem_strdup( const char* str, const char* filename, int line )
{
    int size;
    char* ptr;
    assert( str );

    if ( no_dbgmem ) return _strdup( str );

    size = strlen( str ) + 1;
    ptr = (char*)dbgmem_store(size, filename, line);
    if ( ptr == NULL )
    {
        /*dbgmem_log(filename, line, "strdup of %d bytes for %s [0x%08x] FAILED !\n", size, str, ptr);*/
        return NULL;
    }

    /*dbgmem_log( filename, line, "strdup of %d bytes for %s [0x%08x] OK.\n", size, str, ptr);*/

    //TODO : Windows : strcpy_s
    strcpy( ptr, str );

    return ptr;
}

void dbgmem_free(void *ptr, const char* filename, int line)
{
    if ( no_dbgmem )
        free( ptr );
    else if ( ptr )
    {
        dbgmem_remove( ptr, filename, line);
        /*dbgmem_log( filename, line, "free [0x%08x] OK.\n", ptr);*/
    }
}
