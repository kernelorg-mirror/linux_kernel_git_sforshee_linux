// SPDX-License-Identifier: GPL-2.0

#include "bcachefs.h"
#include "btree_update.h"
#include "errcode.h"
#include "fs.h"
#include "fscaps.h"
#include "inode.h"
#include "str_hash.h"
#include "xattr.h"

#include <linux/dcache.h>
#include <linux/xattr.h>
#include <linux/capability.h>

int bch2_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    struct vfs_caps *caps)
{
	struct inode *vinode = d_inode(dentry);
	struct bch_inode_info *inode = to_bch_ei(vinode);
	struct bch_fs *c = inode->v.i_sb->s_fs_info;
	struct vfs_ns_cap_data nscaps;
	int ret;

	ret = bch2_trans_do(c, NULL, NULL, 0,
		bch2_xattr_get_trans(trans, inode, XATTR_CAPS_SUFFIX, &nscaps,
				     sizeof(nscaps),
				     KEY_TYPE_XATTR_INDEX_SECURITY));
	ret = bch2_err_class(ret);
	if (ret < 0)
		return ret;

	return vfs_caps_from_xattr(&nop_mnt_idmap, i_user_ns(vinode), caps,
				   &nscaps, ret);
}

int bch2_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    const struct vfs_caps *caps, int setxattr_flags)
{
	struct inode *vinode = d_inode(dentry);
	struct bch_inode_info *inode = to_bch_ei(vinode);
	struct bch_fs *c = inode->v.i_sb->s_fs_info;
	struct bch_hash_info hash = bch2_hash_info_init(c, &inode->ei_inode);
	struct bch_inode_unpacked inode_u;
	struct vfs_ns_cap_data nscaps, *value = NULL;
	ssize_t size = 0;
	int ret;

	if (caps) {
		value = &nscaps;
		size = vfs_caps_to_xattr(&nop_mnt_idmap, i_user_ns(vinode), caps,
					 value, sizeof(*value));
		if (size < 0)
			return size;
	}

	ret = bch2_trans_run(c,
		commit_do(trans, NULL, NULL, 0,
			bch2_xattr_set(trans, inode_inum(inode), &inode_u,
				       &hash, XATTR_CAPS_SUFFIX, value, size,
				       KEY_TYPE_XATTR_INDEX_SECURITY,
				       setxattr_flags)) ?:
		(bch2_inode_update_after_write(trans, inode, &inode_u, ATTR_CTIME), 0));

	return bch2_err_class(ret);
}
