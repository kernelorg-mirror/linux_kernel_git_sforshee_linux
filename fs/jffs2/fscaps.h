/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __JFFS2_FSCAPS_H__
#define __JFFS2_FSCAPS_H__

#ifdef CONFIG_JFFS2_FS_SECURITY

#include <linux/fs.h>
#include <linux/capability.h>

int jffs2_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		     struct vfs_caps *caps);
int jffs2_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		     const struct vfs_caps *caps, int setxattr_flags);
#else

#define jffs2_get_fscaps (NULL)
#define jffs2_set_fscaps (NULL)

#endif

#endif /* __JFFS2_FSCAPS_H__ */
