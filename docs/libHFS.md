# Documentation of libHFS

You can find the original API of this library in file `vendors/libhfs/libhfs.txt`.


## File paths

File/directory names are up to 31 characters long, volume names up to 27
characters long. In both cases, they must not contain `:` (directory separator).

An abolute path must begin with the name of the volume containing the desired
file (as if it was a folder).

Relative paths start with the `:` character (similar to `./` in Unix paths). A
path that doesn't contain `:` character is also considered as a relative path.

The directory sperator is the `:` character. When alone, this character refers
to the current directory (as `.` on Unix). When there is a sequence of **N** `:`
(which follow each other), the path refers to the **N-1**-th parent (as `../`
**N-1** times on Unix).


## Directories

On a volume, each directory has a unqiue ID.

ID `0` is reserved for a meta-directory containing all mounted volumes.  
ID `2` is the root directory (of each volume).

Note: the root directory can't be deleted.


## Dates and times

Dates and times are in classic Mac OS format. Classic Mac OS times are 32 bits
unsigned integers that count the number of seconds since January 1, 1904 00:00
UTC.
