/* SPDX-License-Identifier: GPL-2.0 */

#include <linux/fs.h>
#include <linux/capability.h>

#ifdef CONFIG_EXT2_FS_SECURITY

int ext2_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    struct vfs_caps *caps);
int ext2_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    const struct vfs_caps *caps, int setxattr_flags);

#else

#define ext2_get_fscaps NULL
#define ext2_set_fscaps NULL

#endif
