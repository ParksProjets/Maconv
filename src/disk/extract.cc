/*

Extract a disk image.

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

#include <make_unique.hpp>
#include <path.hpp>
#include <cstdio>

namespace maconv {
namespace disk {



// Extract a single fork.
static void ExtractFork(hfsfile *hfile, const hfsdirent &ent, fs::File &file,
    bool is_res)
{
    // Allocate buffer.
    long unsigned int size = is_res ? ent.u.file.rsize : ent.u.file.dsize;
    auto buffer = std::make_unique<uint8_t[]>(size);

    // Read the data.
    hfs_setfork(hfile, is_res ? 1 : 0);
    if (hfs_read(hfile, buffer.get(), size) != size)
        StopOnError("can't read %s (%d) from HFS disk", file.filename.c_str(), is_res);

    // Fill file information.
    if (is_res) {
        file.res = buffer.get();
        file.res_size = size;
    } else {
        file.data_size = size;
        file.data = buffer.get();
    }

    // Save the buffer to the memory pool.
    file.mem_pool.push_back(std::move(buffer));
}


// Extract a file from a disk.
static void ExtractFile(Path &localp, hfsvol *vol, const hfsdirent &ent)
{
    Path::makedirs(localp);
    fs::File file;

    // Extract file information.
    file.Reset();
    file.type = d_getsl((const unsigned char *)ent.u.file.type);
    file.creator = d_getsl((const unsigned char *)ent.u.file.creator);
    file.flags = ent.fdflags;
    file.filename = ent.name;
    file.creation_date = ent.crdate;
    file.modif_date = ent.mddate;

    // Extract the two forks.
    hfsfile *hfile = hfs_open(vol, ent.name);
    ExtractFork(hfile, ent, file, false);
    ExtractFork(hfile, ent, file, true);
    hfs_close(hfile);

    // Save the file.
    auto out = Path::join(localp, GetFilenameFor(ent.name, prefered_conv));
    PackLocalFile(file, out.string(), prefered_conv);
}



// Extract a directory from the disk.
static void ExtractDirectory(Path localp, hfsvol *vol, unsigned long id)
{
    unsigned long current = hfs_getcwd(vol);
    hfs_setcwd(vol, id);

    hfsdir *dir = hfs_opendir(vol, ":");
    hfsdirent ent;

    while (hfs_readdir(dir, &ent) != -1) {
        if (ent.fdflags & HFS_FNDR_ISINVISIBLE)
            continue;

        if (ent.flags & HFS_ISDIR)
            ExtractDirectory(Path::join(localp, ent.name), vol, ent.cnid);
        else
	        ExtractFile(localp, vol, ent);
    }

    hfs_closedir(dir);
    hfs_setcwd(vol, current);
}



// Extract a disk file.
static void ExtractDiskFrom(const std::string &name, const std::string &out)
{
    hfsvol *vol = hfs_mount(name.c_str(), 0, HFS_MODE_RDONLY);
    ExtractDirectory(out, vol, HFS_CNID_ROOTDIR);
    hfs_umount(vol);
}


// Extract a disk file.
void ExtractDisk(UnPacked &u, const std::string &out_folder)
{
    // If it's not a "raw" file: extract the file directly.
    if (u.file.is_raw)
        ExtractDiskFrom(u.n1, out_folder);

    // Else, save the data fork to a temporary file first and then extract.
    char dirname[] = "/tmp/maconv.XXXXXX";
    if (mkdtemp(dirname) == nullptr)
        StopOnError("can't create temporary folder for HFS disk");

    std::string name = std::string {dirname} + "/disk.dsk";
    PackLocalFile(u.file, name, GetConverter("data"));
    ExtractDiskFrom(name, out_folder);

    std::remove(name.c_str());
    rmdir(dirname);
}



// Is a file a disk file?
bool IsFileDisk(const std::string &name)
{
    auto ext = Path(name).extension();
    return ext == "dsk" || ext == "img";
}



} // namespace disk
} // namespace maconv
