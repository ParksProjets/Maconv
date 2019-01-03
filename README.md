# Maconv

**Maconv** is a Linux software that can convert all kinds of old Macintosh formats,
including MacBinary, Stuffit archives and HFS disk files.


## Usage

Maconv has three sub-commands:
- `maconv c [options] input-file [output-file]`
- `maconv e [options] input-file [output-folder]`
- `maconv d [options] input-folder [output-file]`

The `c` sub-commamd converts a file from a format to another. The `e`
sub-commamd extracts a Stuffit archive (versions 1 and 5) or a HFS disk image.
The `d` sub-commamd creates an HFS disk image from a folder (like  a  file
archiver).

You can get more information about these commands with `maconv -h` or `man
maconv`.


## Supported formats

You can get more information on these format in `docs` folder.

Maconv supports the following container formats: [MacBinary][macbin],
[AppleSingle][as].

Also, Maconv can extract old [Stuffit archives][stuffit] (encoded with
compression method 0, 1, 2, 13 or 15).


## License

This project is under the GPLv3 license.  
See `LICENSE` file at the root of this project for more information.


[macbin]: docs/formats/MacBinary.md
[as]: docs/formats/AppleSingle.md
[stuffit]: docs/stuffit/Stuffit.md
