/*
 * libhfs - library for reading and writing Macintosh HFS volumes
 * Copyright (C) 1996-1998 Robert Leslie
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id: volume.h,v 1.7 1998/11/02 22:09:12 rob Exp $
 */

void v_init(hfsvol *, int);

int v_open(hfsvol *, const char *, int);
int v_flush(hfsvol *);
int v_close(hfsvol *);

int v_same(hfsvol *, const char *);
int v_geometry(hfsvol *, int);

int v_readmdb(hfsvol *);
int v_writemdb(hfsvol *);

int v_readvbm(hfsvol *);
int v_writevbm(hfsvol *);

int v_mount(hfsvol *);
int v_dirty(hfsvol *);

int v_catsearch(hfsvol *, unsigned long, const char *,
		CatDataRec *, char *, node *);
int v_extsearch(hfsfile *, unsigned int, ExtDataRec *, node *);

int v_getthread(hfsvol *, unsigned long, CatDataRec *, node *, int);

# define v_getdthread(vol, id, thread, np)  \
    v_getthread(vol, id, thread, np, cdrThdRec)
# define v_getfthread(vol, id, thread, np)  \
    v_getthread(vol, id, thread, np, cdrFThdRec)

int v_putcatrec(const CatDataRec *, node *);
int v_putextrec(const ExtDataRec *, node *);

int v_allocblocks(hfsvol *, ExtDescriptor *);
int v_freeblocks(hfsvol *, const ExtDescriptor *);

int v_resolve(hfsvol **, const char *, CatDataRec *, unsigned long *, char *, node *);

int v_adjvalence(hfsvol *, unsigned long, int, int);
int v_mkdir(hfsvol *, unsigned long, const char *);

int v_scavenge(hfsvol *);
