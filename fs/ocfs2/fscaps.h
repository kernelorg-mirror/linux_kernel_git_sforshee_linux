/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef OCFS2_FSCAPS_H
#define OCFS2_FSCAPS_H

#include <linux/fs.h>
#include <linux/capability.h>

int ocfs2_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		     struct vfs_caps *caps);
int ocfs2_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		     const struct vfs_caps *caps, int setxattr_flags);

#endif /* OCFS2_FSCAPS_H */
