/* SPDX-License-Identifier: GPL-2.0 */

#ifndef BTRFS_FSCAPS_H
#define BTRFS_FSCAPS_H

int btrfs_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		     struct vfs_caps *caps);
int btrfs_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		     const struct vfs_caps *caps, int setxattr_flags);

#endif
