# Stuffit format

All intergers are big-endians.


## Archive header

| **Offset** | **Length** | **Contents** |
|:-----------|:-----------|:-------------|
| 00         | Word       | Magic number 1 (see below)  |
| 04         | Half       | Number of entries in root directory |
| 06         | Word       | Total size of archive |
| 10         | Word       | Magic number 2 (always `0x724c6175`) |
| 14         | Byte       | Version |
| 15         | Byte       | Unknown |
| 16         | Word       | Header size (if version not `1`) |
| 20         | Half       | CRC-16 of header |

Magic number 1 must be one of the following values: `SIT!`, `ST46`, `ST50`,
`ST60`, `ST65`, `STin`, `STi2`, `STi3`, `STi4`.


## File / folder header

| **Offset** | **Length** | **Contents** |
|:-----------|:-----------|:-------------|
| 000        | Byte       | Resource fork compression method |
| 001        | Byte       | Data fork compression method |
| 002        | Byte       | File name length (in range 1-31) |
| 003        | 63 bytes   | File name (remaning bytes are zero)  |
| 066        | Word       | Mac OS file type |
| 070        | Word       | Mac OS file creator |
| 074        | Half       | Mac OS Finder flags |
| 076        | Word       | Creation date (Mac OS format) |
| 080        | Word       | Modification date (Mac OS format) |
| 084        | Word       | Resource fork uncompressed length |
| 088        | Word       | Data fork uncompressed length |
| 092        | Word       | Resource fork compressed length |
| 096        | Word       | Data fork compressed length |
| 100        | Half       | Resource fork CRC-16 |
| 102        | Half       | Data fork CRC-16 |
| 104        | 6 bytes    | Unknown |
| 110        | Half       | Header CRC-16 |


---------------------------

**Sources**

https://github.com/mietek/theunarchiver/wiki/StuffItFormat
