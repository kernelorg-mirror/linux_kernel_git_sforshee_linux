/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef _H_JFS_FSCAPS
#define _H_JFS_FSCAPS

int jfs_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		   struct vfs_caps *caps);
int jfs_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		   const struct vfs_caps *caps, int setxattr_flags);

#endif /* _H_JFS_FSCAPS */
