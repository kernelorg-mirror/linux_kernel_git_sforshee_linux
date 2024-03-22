// SPDX-License-Identifier: GPL-2.0-only

#include "ubifs.h"
#include <linux/fs.h>
#include <linux/xattr.h>
#include <linux/capability.h>

int ubifs_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		     struct vfs_caps *caps)
{
	struct inode *inode = d_inode(dentry);
	struct vfs_ns_cap_data nscaps;
	int size;

	size = ubifs_xattr_get(inode, XATTR_NAME_CAPS, &nscaps, sizeof(nscaps));
	if (size < 0)
		return size;

	return vfs_caps_from_xattr(&nop_mnt_idmap, i_user_ns(inode), caps,
				   &nscaps, size);
}

int ubifs_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		     const struct vfs_caps *caps, int setxattr_flags)
{
	struct inode *inode = d_inode(dentry);
	struct vfs_ns_cap_data nscaps;
	ssize_t size;

	if (!caps)
		return ubifs_xattr_remove(inode, XATTR_NAME_CAPS);

	size = vfs_caps_to_xattr(&nop_mnt_idmap, i_user_ns(inode), caps,
				 &nscaps, sizeof(nscaps));
	if (size < 0)
		return size;

	return ubifs_xattr_set(inode, XATTR_NAME_CAPS, &nscaps, sizeof(nscaps),
			       setxattr_flags, true);
}
