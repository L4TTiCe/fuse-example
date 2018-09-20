#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <string.h>
#include <errno.h>

static const char *filepath = "/file";
static const char *filename = "file";
static const char *filecontent = "I'm the content of the only file available there\n";

// The getattr callback is in charge of reading the metadata of a given path,
// this callback is always called before any operation made on the filesystem.

static int getattr_callback(const char *path, struct stat *stbuf) {
  memset(stbuf, 0, sizeof(struct stat));

  if (strcmp(path, "/") == 0) {         // if the value of path equals to root /, we declare it as a directory and return.
    stbuf->st_mode = S_IFDIR | 0755;    // <- Permission
    stbuf->st_nlink = 2;                // <- 2 stands for directory
    return 0;
  }

  if (strcmp(path, filepath) == 0) {    // if the value of path equals to filepath /file, 
    stbuf->st_mode = S_IFREG | 0777;    // we declare it as a file and explicit its size and then return.
    stbuf->st_nlink = 1;                // <- 1 stands for file
    stbuf->st_size = strlen(filecontent);
    return 0;
  }

  return -ENOENT;   // Nothing in Directory
}

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
  (void) offset;
  (void) fi;

  // Since at the moment the only available directory is /, this function always return its representation
  
  // we are doing it by filling buf with the two links for the upper directory .. and
  // current directory . and with the only file we have: file.
  
  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);

  filler(buf, filename, NULL, 0);

  return 0;
}

// The open callback is called when the system requests for a file to be opened. Since we don’t have real file 
// but only in-memory representations, we are going to implement this callback just because is needed for FUSE to work and therefore return 0.

static int open_callback(const char *path, struct fuse_file_info *fi) {
  return 0;
}

static int read_callback(const char *path, char *buf, size_t size, off_t offset,
    struct fuse_file_info *fi) {
    
    // It should return exactly the number of bytes requested and fill the second argument buf with the content of those bytes.

  if (strcmp(path, filepath) == 0) {
    size_t len = strlen(filecontent);
    if (offset >= len) {
      return 0;
    }

    if (offset + size > len) {
      memcpy(buf, filecontent + offset, len - offset);
      return len - offset;
    }

    memcpy(buf, filecontent + offset, size);        // copy the filecontent into the buf and then return the requested number of bytes.
    return size;
  }

  return -ENOENT;
}

static struct fuse_operations fuse_example_operations = {   // Contains all operations supported
  .getattr = getattr_callback,
  .open = open_callback,
  .read = read_callback,
  .readdir = readdir_callback,
};

int main(int argc, char *argv[])
{
  return fuse_main(argc, argv, &fuse_example_operations, NULL);
}
