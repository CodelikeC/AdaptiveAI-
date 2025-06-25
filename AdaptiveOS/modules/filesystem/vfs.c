#include "vfs.h"
#include <stdbool.h>
#include <string.h> 
#include <stdio.h>

#define MAX_FILES 128 

static VFile file_table[MAX_FILES];
static FileHandle file_handles[MAX_FILE_HANDLES];  

void vfs_init()
{
    memset(file_table, 0, sizeof(file_table));
    memset(file_handles, 0, sizeof(file_handles)); 
    printf("[VFS] Initialized\n");  
}

int vfs_create_file(const char *name, FileType type)
{
    for (int i = 0 ; i < MAX_FILES ; ++i)
    {
        if (!file_table[i].in_use)
        {
            strncpy(file_table[i].name, name, MAX_PATH_LENGTH); 
            file_table[i].type = type; 
            file_table[i].size = 0; 
            file_table[i].data = malloc(1024);  
            file_table[i].in_use = 1; 

            printf("[VFS] File '%s' created.\n", name) ;
            return 0; 
        }
    }
    return -1; 
}

int vfs_open(const char *name)
{
    for (int i = 0 ; i< MAX_FILES ; ++i)
    {
        if (file_table[i].in_use && strcmp(file_table[i].name, name) == 0) {
            for (int j = 0; j < MAX_FILE_HANDLES; ++j) {
                if (!file_handles[j].is_open) {
                    file_handles[j].file = &file_table[i];
                    file_handles[j].position = 0;
                    file_handles[j].is_open = 1;
                    return j;
                }
            }
        }
    }
    return -1; // not found or too many open
}


int vfs_read(int fd, void *buffer, int size)
{
    if (fd< 0 || fd >= MAX_FILE_HANDLES || !file_handles[fd].is_open)
    return -1; 

    VFile *file = file_handles[fd].file; 
    int readable = file -> size - file_handles[fd].position; 
    int to_read = (size < readable) ? size : readable; 
    memcpy(buffer, file-> data + file_handles[fd].position, to_read);
    file_handles[fd].position += to_read; 
    return to_read; 
    
}

int vfs_write(int fd, const void *buffer, int size)
{
    if (fd < 0 || fd >=MAX_FILE_HANDLES || !file_handles[fd].position)
    return -1; 

    VFile *file = file_handles[fd].file; 
    memcpy(file -> data + file_handles[fd].position, buffer, size);
    file_handles[fd].position += size; 
    file ->  size  += size; 
    return size;  
}

void vfs_close(int fd)
{
    if (fd >= 0 && fd < MAX_FILE_HANDLES && file_handles[fd].is_open)
    {
        file_handles[fd].is_open = 0; 
        file_handles[fd].file = NULL; 
        file_handles[fd].position = 0; 
    }
}

void vfs_list_files()
{
    printf("[VFS] File list.\n"); 
    for (int i = 0 ; i < MAX_FILES ; ++i)
    {
        if (file_table[i].in_use)
        {
            printf("- %s (%s)\n", file_table[i].name,
                file_table[i].type == FILE_TYPE_DIRECTORY ? "DIR": 
                file_table[i].type == FILE_TYPE_DEVICE ? "DIR" : "FILE"
            ); 
        }
    }
}