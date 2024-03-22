// SPDX-License-Identifier: GPL-2.0

#include "xfs.h"
#include "xfs_shared.h"
#include "xfs_format.h"
#include "xfs_log_format.h"
#include "xfs_trans_resv.h"
#include "xfs_mount.h"
#include "xfs_inode.h"
#include "xfs_da_format.h"
#include "xfs_da_btree.h"
#include "xfs_attr.h"
#include "xfs_xattr.h"
#include "xfs_fscaps.h"

int
xfs_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
	       struct vfs_caps *caps)
{
	struct inode *inode = d_inode(dentry);
	struct vfs_ns_cap_data nscaps;
	struct xfs_da_args	args = {
		.dp		= XFS_I(inode),
		.attr_filter	= XFS_ATTR_SECURE,
		.name		= XATTR_CAPS_SUFFIX,
		.namelen	= strlen(XATTR_CAPS_SUFFIX),
		.value		= (void *)&nscaps,
		.valuelen	= sizeof(nscaps),
	};
	int err;

	if (xfs_ifork_zapped(XFS_I(inode), XFS_ATTR_FORK))
		return -EIO;

	err = xfs_attr_get(&args);
	if (err)
		return err;

	return vfs_caps_from_xattr(idmap, i_user_ns(inode), caps, &nscaps,
				   args.valuelen);
}

int
xfs_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
	       const struct vfs_caps *caps, int setxattr_flags)
{
	struct inode *inode = d_inode(dentry);
	struct xfs_da_args	args = {
		.dp		= XFS_I(inode),
		.attr_filter	= XFS_ATTR_SECURE,
		.attr_flags	= setxattr_flags,
		.name		= XATTR_CAPS_SUFFIX,
		.namelen	= strlen(XATTR_CAPS_SUFFIX),
	};
	struct vfs_ns_cap_data nscaps;
	ssize_t size;

	if (caps) {
		size = vfs_caps_to_xattr(idmap, i_user_ns(inode), caps,
					 &nscaps, sizeof(nscaps));
		if (size < 0)
			return size;
		args.value = (void *)&nscaps;
		args.valuelen = size;
	}

	return xfs_attr_change(&args);
}
