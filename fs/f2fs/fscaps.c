// SPDX-License-Identifier: GPL-2.0

#include <linux/f2fs_fs.h>
#include "f2fs.h"
#include "xattr.h"
#include "fscaps.h"

int f2fs_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    struct vfs_caps *caps)
{
	struct inode *inode = d_inode(dentry);
	struct vfs_ns_cap_data nscaps;
	int size;

	size = f2fs_getxattr(inode, F2FS_XATTR_INDEX_SECURITY,
			     XATTR_CAPS_SUFFIX, &nscaps, sizeof(nscaps), NULL);
	if (size < 0)
		return size;

	return vfs_caps_from_xattr(idmap, i_user_ns(inode), caps, &nscaps,
				   size);
}

int f2fs_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    const struct vfs_caps *caps, int setxattr_flags)
{
	struct inode *inode = d_inode(dentry);
	struct vfs_ns_cap_data nscaps, *value = NULL;
	ssize_t size = 0;

	if (caps) {
		value = &nscaps;
		size = vfs_caps_to_xattr(idmap, i_user_ns(inode), caps, value,
					 sizeof(*value));
		if (size < 0)
			return size;
	}

	return f2fs_setxattr(inode, F2FS_XATTR_INDEX_SECURITY,
			     XATTR_CAPS_SUFFIX, value, size, NULL,
			     setxattr_flags);
}
