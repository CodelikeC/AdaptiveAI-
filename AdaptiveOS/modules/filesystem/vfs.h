#ifndef VFS_H 
#define VFS_H 

#include <stdint.h> 
#include <stdbool.h> 

#define MAX_PATH_LENGTH 256 
#define MAX_FILE_HANDLES 64 


typedef enum 
{
    FILE_TYPE_REGULAR, 
    FILE_TYPE_DIRECTORY,
    FILE_TYPE_DEVICE
}FileType; 

typedef struct 
{
    char name[MAX_PATH_LENGTH]; 
    FileType type;
    uint32_t size; 
    uint8_t *data; 
    bool in_use; 
}VFile;

typedef struct 
{
    VFile *file; 
    uint32_t position; 
    bool is_open; 
}FileHandle; 

// API .. // 
void vfs_init(); 
int vfs_create_file(const char *name, FileType type); 
int vfs_open(const char *name); 
int vfs_read(int fd, void *buffer, int size); 
int vfs_write(int fd, const void *buffer, int size); 
void vfs_close(int fd); 
void vfs_list_files(); 


#endif