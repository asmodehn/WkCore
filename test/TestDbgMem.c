#include "dbgmem.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

const char* str[] =
{
    "Lorem ipsum dolor sit amet, consectetuer adipiscing elit.",
    "Donec feugiat convallis nibh.",
    "Ut nonummy urna at enim.",
    "Morbi tristique elit quis sem.",
    "Sed eget mauris molestie ipsum consequat vulputate.",
    "Quisque tincidunt arcu et diam.",
    "Integer aliquam nonummy mi.",
};

#define NUMELEMS(a) (sizeof((a))/sizeof((a)[0]))

char* Strdup(const char* p)
{
    char* ret = malloc(strlen(p)+1);
    assert( ret != NULL && "Out of memory" );
    strcpy(ret, p);
    return ret;
}

void string_test()
{
    int i = 0;
    int j = 0;
    char* ptrs[ 1024 ];
    char* big_string, * curptr;

    printf("String test...\n");
    memset(ptrs, 0, sizeof(ptrs));
    for ( i = 0; i < NUMELEMS(ptrs); i ++ )
    {
        ptrs[i] = Strdup( str[ i % NUMELEMS(str) ] );
    }

    for ( i = 0; i < NUMELEMS(ptrs); i ++ )
        free( ptrs[i] );

    assert( dbgmem_dump_blocks() );
    printf("End string test 1\n");

    big_string = curptr = NULL;
#define NUM_ITERATIONS 100		/* fails on 17 on my machine!  Try a higher number if it doesn't on yours! */
    /* 100 on mine and still doesnt fail... - AV */
    for ( j = 0; j < NUM_ITERATIONS; j ++ )
    {
        for ( i = 0; i < NUMELEMS(str); i ++ )
        {
            if ( big_string == NULL )
            {
                big_string = malloc( strlen(str[i]) + 1 );
                assert( big_string != NULL );
                curptr = big_string;
            }
            else
            {
                big_string = realloc( big_string, strlen(big_string)+strlen(str[i])+1 );
                curptr = big_string + strlen(big_string);
            }

            strcpy(curptr, str[i]);
            curptr += strlen(str[i]);
        }
    }

    curptr = big_string;
    for ( j = 0; j < NUM_ITERATIONS; j ++ )
    {
        for ( i = 0; i < NUMELEMS(str); i ++ )
        {
            int len = strlen(str[i]);
            assert( memcmp(curptr, str[i], len) == 0 );
            curptr += len;
        }
    }
    free( big_string );

    assert( dbgmem_dump_blocks() );
    printf("End string test 2\n");
}


/* -------------Linked list test------------ */
struct Node
{
    struct Node* next;
    struct Node* prev;
    void* data;
};

static struct Node* mknode(void* data)
{
    struct Node* p = malloc( sizeof(struct Node) );
    p->data = Strdup(data);
    p->next = p->prev = NULL;
    return p;
}

void list_test()
{
    /* linked list test, insert in order */
    int i = 0, iters = 0;
    struct Node* list = NULL;
    struct Node* tmp = NULL;
    const int MaxIterations = 200000;
    clock_t start, end;

    /*#define SORTED*/ /* Uncomment to make it sorted.  A sorted list will make the deallocation a little more random! */
    start = clock();
    printf ("Running %d iterations of sorted linked-list test (This should be relatively slow).\n", MaxIterations);

    for ( iters = 0; iters < MaxIterations; iters ++ )
    {
        if ( iters % 100 == 0 )
            fputc((iters % 1000 == 0) ? '*' : '.', stdout);
        for ( i = 0; i < NUMELEMS(str); i ++ )
        {
            struct Node* node = mknode( (char*)str[i]);
            if ( list == NULL )
            {
                /* list empty */
                list = node;
                continue;
            }

            /* insert in order */
            tmp = list;
            for ( ;; )
            {
#ifdef SORTED
                if ( strcmp(node->data, tmp->data) > 0 )
                {
#else
                if ( 0 )
                {
#endif
                    if ( tmp->next == NULL )
                    {
                        /* append to tail */
                        node->prev = tmp;
                        tmp->next = node;
                        break;
                    }
                    tmp = tmp->next;
                }
                else
                {
                    /* add before tmp */
                    node->next = tmp;
                    node->prev = tmp->prev;
                    if ( tmp->prev != NULL )
                        tmp->prev->next = node;
                    tmp->prev = node;
                    if ( tmp == list )
                        list = node;	/* we are now the head */
                    break;
                }
            }
        }
    }

    end = clock();
    printf("\nElapsed: %lusecs.\n", (end-start)/CLOCKS_PER_SEC);

    /* dump the list */
    /*
    printf("Dumping ordered-list\n\n");
    for ( tmp = list; tmp != NULL; tmp = tmp->next )
    	printf("%s\n", tmp->data);
    */

    /* free list */
    start = clock();
    printf("Freeing list. This speed is dependent on the allocator now!");
    for ( tmp = list; tmp != NULL; )
    {
        struct Node* next = tmp->next;
        free(tmp->data);
        free(tmp);
        tmp = next;
    }
    end = clock();
    printf("\nElapsed: %lu secs.\n", (end-start)/CLOCKS_PER_SEC);
    assert( dbgmem_dump_blocks() );
}

int main()
{

    char * str2;

    dbgmem_debug_heap_init();
    atexit ( dbgmem_debug_heap_fini );

    str2 = _strdup(str[2]);	/* DOES NOT CATCH THE LEAK! -> try again - AV */

    string_test();
    list_test();

    dbgmem_dump_blocks();
    return 0;
}
