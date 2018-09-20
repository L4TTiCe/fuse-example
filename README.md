# Simple Filesystem using FUSE

  This project contains a static read only filesystem that contains only 1 file in an otherwise empty directory. As seen in fntlnz's ["Write a filesystem with FUSE"](http://engineering.facile.it/blog/eng/write-filesystem-fuse/) blog post.

## Features
  1.  Implements the following operation callbacks  :
        * getattr 
        * readattr
        * open
        * read
        * readdir
