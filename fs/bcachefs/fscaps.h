/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _BCACHEFS_FSCAPS_H
#define _BCACHEFS_FSCAPS_H

int bch2_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    struct vfs_caps *caps);
int bch2_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    const struct vfs_caps *caps, int setxattr_flags);

#endif /* _BCACHEFS_FSCAPS_H */
