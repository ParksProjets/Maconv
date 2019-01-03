# AppleSingle format

AppleSingle is a file format developed to store Mac OS "dual-forked" files on
the Unix filesystem. AppleSingle is similar in concept to the more popular
MacBinary format, in that the resource and data forks are combined together with
a header containing the Finder information. In fact, the format is so similar,
it seemed there were no reason why Apple did not simply use MacBinary instead,
which by that point, was widely known and used.

All integers are big-endians.

Data are arranged as follows:
> `data = <file header> + <entry 1> + ... + <entry N> + <data 1> + ... + <data N>`


## File header

| **Offset** | **Length** | **Contents** |
|:-----------|:-----------|:-------------|
| 00         | Word       | Magic number (always `0x00051600`) |
| 04         | Word       | Version number (always `0x00020000`) |
| 08         | 16 Bytes   | Always zero |
| 24         | Half       | Number of entries |


## Entry

| **Offset** | **Length** | **Contents** |
|:-----------|:-----------|:-------------|
| 00         | Word       | Entry ID (see below) |
| 04         | Word       | Data offset |
| 08         | Word       | Data length (can be zero) |


Available entry IDs are described in the following table.

| **Entry name**      | **ID** | **Description** |
|:--------------------|:-------|:----------------|
| Data Fork           | 1      | Magic number (always `0x00051600`)  |
| Resource Fork       | 2      | Version number (always `0x00020000`)  |
| Real Name           | 3      | File’s name as created on home file system  |
| Comment             | 4      | Standard Macintosh comment |
| Icon, B&W           | 5      | Standard Macintosh black and white icon |
| Icon, Color         | 6      | Macintosh color icon |
| File Dates Info     | 8      | File creation date, modification date, and so on |
| Finder Info         | 9      | Standard Macintosh Finder information |
| Macintosh File Info | 10     | Macintosh file information, attributes, and so on |
| ProDOS File Info    | 11     | ProDOS file information, attributes, and so on |
| MS-DOS File Info    | 12     | MS-DOS file information, attributes, and so on |
| Short Name          | 13     | AFP short name |
| AFP File Info       | 14     | AFP file information, attributes, and so on |
| Directory ID        | 15     | AFP directory ID |


---------------------------

**Sources**

https://en.wikipedia.org/wiki/AppleSingle_and_AppleDouble_formats
http://kaiser-edv.de/documents/AppleSingle_AppleDouble.pdf
