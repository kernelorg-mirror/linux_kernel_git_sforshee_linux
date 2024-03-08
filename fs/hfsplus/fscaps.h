/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _LINUX_HFSPLUS_FSCAPS_H
#define _LINUX_HFSPLUS_FSCAPS_H

#include <linux/fs.h>
#include <linux/capability.h>

int hfsplus_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		       struct vfs_caps *caps);
int hfsplus_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		       const struct vfs_caps *caps, int setxattr_flags);

#endif

