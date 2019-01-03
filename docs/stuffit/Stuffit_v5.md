# Stuffit 5 format

All intergers are big-endians.


## Archive header

| **Offset** | **Length** | **Contents** |
|:-----------|:-----------|:-------------|
| 000        | 80 bytes   | Magic string (always `StuffIt (c)1997-???? Aladdin Systems, Inc., http://www.aladdinsys.com/StuffIt/` followed by `0x0D` `0x0A`, where characters marked `?` can vary)  |
| 080        | Half       | Unknown |
| 082        | Byte       | Version (always `5`) |
| 083        | Byte       | Flags (`0x80` = encrypted) |
| 084        | Word       | Total size of archive |
| 088        | Word       | Unknown |
| 092        | Half       | Number of entries in root directory |
| 094        | Word       | Offset of first entry in root directory |
| 098        | Half       | Header CRC-16? |
| 100        | ?          | Unknown data until first entry |


## Entry header

### Base header for file and folder

Both file and folder headers start with the following header.

| **Offset** | **Length** | **Contents** |
|:-----------|:-----------|:-------------|
| 00         | Word       | Magic number (always `0xA5A5A5A5`)  |
| 04         | Byte       | Version |
| 05         | Byte       | Unknown (but certainly `0x00`) |
| 06         | Half       | Header size |
| 08         | Byte       | Unknown |
| 09         | Byte       | Entry flags/type |
| 10         | Word       | Creation date (Mac OS format) |
| 14         | Word       | Modification date (Mac OS format) |
| 18         | Word       | Offset of previous entry |
| 22         | Word       | Offset of next entry |
| 26         | Word       | Offset of directory entry |
| 30         | Half       | Name size (called **N** after) |
| 32         | Half       | Header CRC-16 |


### File specific header

A file (when `flags` doesn't contain `0x40`) continues with the following header.

| **Offset** | **Length** | **Contents** |
|:-----------|:-----------|:-------------|
| 34         | Word       | Data fork uncompressed length |
| 38         | Word       | Data fork compressed length |
| 42         | Half       | Data fork CRC-16 (Set to zero for method 15) |
| 44         | Half       | Unknown |
| 46         | Byte       | Data fork compression method (only `0`, `13` or `15`) |
| 47         | Byte       | Password data lenght (called **M** after) |
| 48         | **M**      | Password information |
| 48+**M**   | **N**      | Filename |
| 48+**M**+**N** | **K**  | Comment size (called **K** after) |
| 48         | **N**      | Filename |
| 48         | **N**      | Filename |


### Folder specific header

A folder (when `flags` contains `0x40`) continues with the following header.

| **Offset** | **Length** | **Contents** |
|:-----------|:-----------|:-------------|
| 34         | Word       | Offset of first entry in folder |
| 38         | Word       | Size of complete directory |
| 42         | Word       | Unknown |
| 46         | Half       | Number of files in folder |
| 48         | **N**      | Folder name |
| 48+**N**   | ?          | Unknown data until next entry |

Note: if the offset of first entry is `0xFFFFFFFF`, this folder must be skiped
after reading 48 bytes of header (the usefulness of these folders is unclear).


---------------------------

**Sources**

https://github.com/mietek/theunarchiver/wiki/StuffIt5Format
https://github.com/mietek/theunarchiver/blob/master/XADMaster/XADStuffIt5Parser.m#L29
