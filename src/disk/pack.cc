/*

Pack files into a single disk image.

Copyright (C) 2019, Guillaume Gonnet

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include "disk/disk.h"
#include "commands.h"

#include <libhfs/hfs.h>
#include <libhfs/data.h>

#include <path.hpp>
#include <fstream>
#include <algorithm>
#include <unordered_set>

namespace maconv {
namespace disk {


// Type for seen files.
using SeenList = std::unordered_set<std::string>;

// Pack a directory into the disk.
static void PackDirectory(const Path &localp, hfsvol *vol);



// Write a single fork to an HFS file.
static void WriteFork(hfsfile *hfile, fs::File &file, bool is_res)
{
    int size = is_res ? file.res_size : file.data_size;
    uint8_t *data = is_res ? file.res : file.data;

    hfs_setfork(hfile, is_res ? 1 : 0);
    hfs_write(hfile, data, size);
}


// Pack a file into the disk.
static void PackFile(const Path &file, hfsvol *vol, SeenList &seen)
{
    // Unpack the local file.
    UnPacked u = UnPackLocalFile(file.string(), false);

    // Insert file name(s) to seen list.
    seen.insert(u.n1);
    if (!u.n2.empty()) seen.insert(u.n2);


    // Sanitize file name, type and creator.
    auto filename = u.file.filename.substr(0, 31);
    std::replace(filename.begin(), filename.end(), ':', '_');

    char type[5] = {0}, creator[5] = {0};
    d_putsl((unsigned char *)type, u.file.type);
    d_putsl((unsigned char *)creator, u.file.creator);

    // Create the file on the disk and write forks.
    hfsfile *hfile = hfs_create(vol, filename.c_str(), type, creator);
    if (hfile == nullptr)
        StopOnError("can't create HFS file %s", filename.c_str());

    WriteFork(hfile, u.file, false);
    WriteFork(hfile, u.file, true);


    // Set other attributes.
    hfsdirent ent;
    hfs_fstat(hfile, &ent);

    ent.crdate = u.file.creation_date;
    ent.mddate = u.file.modif_date;

    ent.fdflags = u.file.flags;
    ent.fdflags &= ~HFS_FNDR_ISINVISIBLE;

    hfs_fsetattr(hfile, &ent);
    hfs_close(hfile);
}



// Create a directory on the HFS disk and pack it.
static void CreateAndPackDirectory(const Path &localp, hfsvol *vol)
{
    unsigned long current = hfs_getcwd(vol);

    auto dirname = Path(localp).trim().filename().substr(0, 31);
    std::replace(dirname.begin(), dirname.end(), ':', '_');

    if (hfs_mkdir(vol, dirname.c_str()) != 0)
        StopOnError("can't create HFS folder %s", dirname.c_str());
    hfs_chdir(vol, dirname.c_str());

    PackDirectory(localp, vol);
    hfs_setcwd(vol, current);
}


// Pack a directory into the disk.
static void PackDirectory(const Path &localp, hfsvol *vol)
{
    SeenList seen;

    for (auto &file : Path::listdir(localp)) {
        if (seen.count(file.string()) == 1) continue;

        if (file.is_file())
            PackFile(file, vol, seen);
        else if (file.is_directory())
            CreateAndPackDirectory(file, vol);
    }
}



// Create a new file disk and mount it.
static hfsvol *CreateAndMountNewDisk(const std::string &filename,
    const std::string &volname)
{
    // Sanitize the volume name.
    auto clean_name = volname.substr(0, 27);
    std::replace(clean_name.begin(), clean_name.end(), ':', '_');

    // Truncate the disk file.
    int size = 20971520;
    std::ofstream(filename, std::ios::binary | std::ios::trunc)
        .seekp(size-1).put(0);

    // Create and mount the file.
    if (hfs_format(filename.c_str(), 0, 0, clean_name.c_str(), 0, NULL) != 0)
        StopOnError("can't format disk file to HFS");

    hfsvol *vol = hfs_mount(filename.c_str(), 0, HFS_MODE_RDWR);
    if (vol == nullptr)
        StopOnError("can't mount output disk image");

    return vol;
}


// Pack files into a single disk image.
void PackDiskImage(const std::string &folder, const std::string &out,
    const std::string &volname)
{
    hfsvol *vol = CreateAndMountNewDisk(out, volname);
    PackDirectory(folder, vol);
    hfs_umount(vol);
}



} // namespace disk
} // namespace maconv
