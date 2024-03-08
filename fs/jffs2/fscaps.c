// SPDX-License-Identifier: GPL-2.0

#include <linux/fs.h>
#include <linux/xattr.h>
#include <linux/capability.h>
#include "nodelist.h"
#include "xattr.h"
#include "fscaps.h"

int jffs2_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		     struct vfs_caps *caps)
{
	struct inode *inode = d_inode(dentry);
	struct vfs_ns_cap_data nscaps;
	int size;

	size = do_jffs2_getxattr(inode, JFFS2_XPREFIX_SECURITY,
				 XATTR_CAPS_SUFFIX, (char *)&nscaps,
				 sizeof(nscaps));
	if (size < 0)
		return size;

	return vfs_caps_from_xattr(&nop_mnt_idmap, i_user_ns(inode), caps,
				   &nscaps, size);
}

int jffs2_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		     const struct vfs_caps *caps, int setxattr_flags)
{
	struct inode *inode = d_inode(dentry);
	struct vfs_ns_cap_data nscaps, *value = NULL;
	int size = 0;

	if (caps) {
		value = &nscaps;
		size = vfs_caps_to_xattr(&nop_mnt_idmap, i_user_ns(inode), caps,
					 value, sizeof(*value));
		if (size < 0)
			return size;
	}

	return do_jffs2_setxattr(inode, JFFS2_XPREFIX_SECURITY,
				 XATTR_CAPS_SUFFIX, (char *)value, size,
				 setxattr_flags);
}
