// SPDX-License-Identifier: GPL-2.0-or-later

#include <linux/fs.h>
#include <linux/capability.h>
#include "jfs_incore.h"
#include "jfs_txnmgr.h"
#include "jfs_xattr.h"
#include "jfs_fscaps.h"

int jfs_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		   struct vfs_caps *caps)
{
	struct inode *inode = d_inode(dentry);
	struct vfs_ns_cap_data nscaps;
	int size;

	size = __jfs_getxattr(inode, XATTR_NAME_CAPS, &nscaps, sizeof(nscaps));
	if (size < 0)
		return size;

	return vfs_caps_from_xattr(&nop_mnt_idmap, i_user_ns(inode), caps,
				   &nscaps, size);
}

int jfs_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		   const struct vfs_caps *caps, int setxattr_flags)
{
	struct inode *inode = d_inode(dentry);
	struct jfs_inode_info *ji = JFS_IP(inode);
	tid_t tid;
	struct vfs_ns_cap_data nscaps, *value = NULL;
	ssize_t size = 0;
	int ret;

	if (caps) {
		value = &nscaps;
		size = vfs_caps_to_xattr(&nop_mnt_idmap, i_user_ns(inode), caps,
					 value, sizeof(*value));
		if (size < 0)
			return size;
	}

	tid = txBegin(inode->i_sb, 0);
	mutex_lock(&ji->commit_mutex);
	ret = __jfs_setxattr(tid, inode, XATTR_NAME_CAPS, value, size,
			     setxattr_flags);
	if (!ret)
		ret = txCommit(tid, 1, &inode, 0);
	txEnd(tid);
	mutex_unlock(&ji->commit_mutex);

	return ret;
}
