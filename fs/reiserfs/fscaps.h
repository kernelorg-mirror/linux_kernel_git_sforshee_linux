/* SPDX-License-Identifier: GPL-2.0 */

#include <linux/fs.h>
#include <linux/capability.h>

#ifdef CONFIG_REISERFS_FS_SECURITY

int reiserfs_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
			struct vfs_caps *caps);
int reiserfs_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
			const struct vfs_caps *caps, int setxattr_flags);

#else

#define reiserfs_get_fscaps NULL
#define reiserfs_set_fscaps NULL

#endif
