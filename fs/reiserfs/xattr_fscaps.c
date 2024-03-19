// SPDX-License-Identifier: GPL-2.0

#include <linux/fs.h>
#include <linux/xattr.h>
#include <linux/capability.h>
#include "reiserfs.h"
#include "xattr.h"
#include "fscaps.h"

int reiserfs_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
			struct vfs_caps *caps)
{
	struct inode *inode = d_inode(dentry);
	struct vfs_ns_cap_data nscaps;
	int size;

	if (IS_PRIVATE(inode))
		return -EPERM;

	size = reiserfs_xattr_get(inode, XATTR_NAME_CAPS, &nscaps,
				  sizeof(nscaps));
	if (size < 0)
		return size;

	return vfs_caps_from_xattr(&nop_mnt_idmap, i_user_ns(inode), caps,
				   &nscaps, size);
}

int reiserfs_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
			const struct vfs_caps *caps, int setxattr_flags)
{
	struct inode *inode = d_inode(dentry);
	struct vfs_ns_cap_data nscaps, *value = NULL;
	ssize_t size = 0;

	if (IS_PRIVATE(inode))
		return -EPERM;

	if (caps) {
		value = &nscaps;
		size = vfs_caps_to_xattr(&nop_mnt_idmap, i_user_ns(inode), caps,
					 value, sizeof(*value));
		if (size < 0)
			return size;
	}

	return reiserfs_xattr_set(inode, XATTR_NAME_CAPS, value, size,
				  setxattr_flags);
}
