/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __F2FS_FSCAPS_H__
#define __F2FS_FSCAPS_H__

#include <linux/fs.h>
#include <linux/capability.h>

#ifdef CONFIG_F2FS_FS_SECURITY

int f2fs_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    struct vfs_caps *caps);
int f2fs_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    const struct vfs_caps *caps, int setxattr_flags);

#else

#define f2fs_get_fscaps NULL
#define f2fs_set_fscaps NULL

#endif

#endif /* __F2FS_FSCAPS_H__ */
