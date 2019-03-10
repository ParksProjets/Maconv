# MacBinary format

MacBinary is a file format that combines the two forks of a classic Mac OS file
into a single file, along with HFS's extended metadata. It was used to share Mac
OS files over FTP, Web or e-mail because it could be stored on computers that
run operating systems with no HFS support, such as Unix or Windows.

This file describes a summary of MacBinnary I, II and III.  
All integers are big-endians.

Data are arranged as follows:
> `data = <header 128B> + <data fork> + <padding 1> + <res fork> + <padding 2>`


## Header – `<header 128B>`

| **Offset** | **Length** | **Contents** |
|:-----------|:-----------|:-------------|
| 000        | Byte       | Always zero |
| 001        | Byte       | Length of filename (in the range 1-31) |
| 002        | 63 Bytes   | Filename (remaning bytes are zero) |
| 065        | Word       | File type (4 characters) |
| 069        | Word       | File creator (4 characters) |
| 073        | Byte       | Finder flags: Bit 7 - isAlias. Bit 6 - isInvisible. Bit 5 - hasBundle. Bit 4 - nameLocked. Bit 3 - isStationery. Bit 2 - hasCustomIcon. Bit 1 - reserved. Bit 0 - hasBeenInited. |
| 074        | Byte       | Always zero |
| 075        | Half       | File's vertical position within its window |
| 077        | Half       | File's horizontal position within its window |
| 079        | Half       | File's window or folder ID. |
| 081        | Byte       | "Protected" flag (in low order bit) |
| 082        | Byte       | Always zero |
| 083        | Word       | Data Fork length in B (zero if no Data Fork) |
| 087        | Word       | Resource Fork length in B (zero if no R.F.) |
| 091        | Word       | File's creation date |
| 095        | Word       | File's "last modified" date |
| 099        | Half       | Length of Get Info comment to be sent after the resource fork (if implemented, see below). |
| 101 **²**  | Byte       | Finder Flags, bits 0-7. (Bits 8-15 are already in byte 73)  Bit 7 - hasNoInits Bit 6 - isShared Bit 5 - requiresSwitchLaunch Bit 4 - ColorReserved Bits 1-3 - color Bit 0 - isOnDesk |
| 102 **³**  | Word       | Signature for indentification purposes (always `mBIN`) |
| 106 **³**  | Byte       | Script of file name (from the fdScript field of an fxInfo record) |
| 107 **³**  | Byte       | Extended Finder flags (from the fdXFlags field of an fxInfo record) |
| 108        | 8 Bytes    | Unused (must be zeroed by creators, must be ignored by readers) |
| 116 **²**  | Word       | Length of total files when packed files are unpacked. As of the writing of this document, this field has never been used. |
| 120 **²**  | Half       | Length of a secondary header. If this is non-zero, skip this many bytes (rounded up to the next multiple of 128). This is for future expansion only, when sending files with MacBinary, this word should be zero. |
| 122 **²**  | Byte       | Version number of MacBinary (`129` for MacBinary II, `130` for MacBinary III) |
| 123 **²**  | Byte       | Minimum MacBinary version needed to read this file (set this value at 129 for backwards compatibility with MacBinary II) |
| 124 **²**  | Half       | CRC-16 of previous 124 bytes |
| 126        | Half       | Padding (always zero) |

**²** These fields have been added in MacBinary II.  
**³** These fields have been added in MacBinary III.

Cyclic redundancy check (CRC) used in header is CRC-16-CCITT, i.e. uses
polynomial number `0x1021` and starts with `0`.


## Data and ressource forks

Data fork directly follows the header (at byte 128). The length of these data
(which can be zero) must correspond with the length given in the header (at byte
83).

Data are completed with some padding bytes (usually `0x00` but some
implementations use `0x7F`) until the total length of file is a multiple of 128.
If the total length is already a multiple of 128 after adding the data fork, no
padding is added.

The ressource fork follows the same rules (i.e. padding bytes are added after
the fork to keep the total length a multiple of 128).


---------------------------

**Sources**

https://en.wikipedia.org/wiki/MacBinary
https://github.com/mietek/theunarchiver/wiki/MacBinarySpecs
https://files.stairways.com/other/macbinaryii-standard-info.txt
