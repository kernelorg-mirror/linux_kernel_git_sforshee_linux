// SPDX-License-Identifier: GPL-2.0-only

#include <linux/fs.h>
#include <linux/capability.h>

#include "ocfs2.h"
#include "alloc.h"
#include "inode.h"
#include "xattr.h"
#include "fscaps.h"

int ocfs2_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		     struct vfs_caps *caps)
{
	struct inode *inode = d_inode(dentry);
	struct vfs_ns_cap_data nscaps;
	int size;

	size = ocfs2_xattr_get(inode, OCFS2_XATTR_INDEX_SECURITY,
			       XATTR_CAPS_SUFFIX, &nscaps, sizeof(nscaps));
	if (size < 0)
		return size;

	return vfs_caps_from_xattr(&nop_mnt_idmap, i_user_ns(inode), caps,
				   &nscaps, size);
}

int ocfs2_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		     const struct vfs_caps *caps, int setxattr_flags)
{
	struct inode *inode = d_inode(dentry);
	struct vfs_ns_cap_data nscaps, *value = NULL;
	ssize_t size = 0;

	if (caps) {
		value = &nscaps;
		size = vfs_caps_to_xattr(&nop_mnt_idmap, i_user_ns(inode), caps,
					 value, sizeof(*value));
		if (size < 0)
			return size;
	}

	return ocfs2_xattr_set(inode, OCFS2_XATTR_INDEX_SECURITY,
			       XATTR_CAPS_SUFFIX, value, size, setxattr_flags);
}
