// SPDX-License-Identifier: GPL-2.0

#include <linux/fs.h>
#include <linux/capability.h>

#ifndef _EXT4_FSCAPS_H
#define _EXT4_FSCAPS_H

#ifdef CONFIG_EXT4_FS_SECURITY

int ext4_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    struct vfs_caps *caps);
int ext4_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    const struct vfs_caps *caps, int setxattr_flags);

#else

#define ext4_get_fscaps NULL
#define ext4_set_fscaps NULL

#endif

#endif /* _EXT4_FSCAPS_H */
