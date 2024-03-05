// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file contains vfs inode ops for the 9P2000 protocol.
 *
 *  Copyright (C) 2004 by Eric Van Hensbergen <ericvh@gmail.com>
 *  Copyright (C) 2002 by Ron Minnich <rminnich@lanl.gov>
 */

#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/xattr.h>
#include <linux/capability.h>
#include <net/9p/9p.h>
#include <net/9p/client.h>

#include "v9fs.h"
#include "xattr.h"

int v9fs_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    struct vfs_caps *caps)
{
	struct inode *inode = d_inode(dentry);
	struct vfs_ns_cap_data nscaps;
	ssize_t size;

	size = v9fs_xattr_get(dentry, XATTR_NAME_CAPS, &nscaps, sizeof(nscaps));
	if (size < 0)
		return size;
	if (size == 0)
		return -ENODATA;

	return vfs_caps_from_xattr(&nop_mnt_idmap, i_user_ns(inode), caps,
				   &nscaps, size);
}

int v9fs_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
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

	return v9fs_xattr_set(dentry, XATTR_NAME_CAPS, value, size,
			      setxattr_flags);
}
