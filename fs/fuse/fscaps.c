// SPDX-License-Identifier: GPL-2.0

#include "fuse_i.h"

#include <linux/capability.h>

int fuse_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    struct vfs_caps *caps)
{
	struct inode *inode = d_inode(dentry);
	struct vfs_ns_cap_data nscaps;
	int size;

	if (fuse_is_bad(inode))
		return -EIO;

	size = fuse_getxattr(inode, XATTR_NAME_CAPS, &nscaps, sizeof(nscaps));
	if (size < 0)
		return size;

	return vfs_caps_from_xattr(idmap, i_user_ns(inode), caps, &nscaps,
				   size);
}

int fuse_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    const struct vfs_caps *caps, int setxattr_flags)
{
	struct inode *inode = d_inode(dentry);
	struct vfs_ns_cap_data nscaps, *value = NULL;
	ssize_t size = 0;

	if (fuse_is_bad(inode))
		return -EIO;

	if (caps) {
		value = &nscaps;
		size = vfs_caps_to_xattr(idmap, i_user_ns(inode), caps, value,
					 sizeof(*value));
		if (size < 0)
			return size;
	}

	return fuse_setxattr(inode, XATTR_NAME_CAPS, value, size,
			     setxattr_flags, 0);
}
