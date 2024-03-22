// SPDX-License-Identifier: GPL-2.0

#ifndef __XFS_FSCAPS_H__
#define __XFS_FSCAPS_H__

int xfs_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		   struct vfs_caps *caps);
int xfs_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		   const struct vfs_caps *caps, int setxattr_flags);

#endif	/* __XFS_FSCAPS_H__ */
