/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __FSCAPS_DOT_H__
#define __FSCAPS_DOT_H__

#include <linux/fs.h>
#include <linux/capability.h>

int gfs2_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    struct vfs_caps *caps);
int gfs2_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    const struct vfs_caps *caps, int setxattr_flags);

#endif /* __ACL_DOT_H__ */
