// SPDX-License-Identifier: GPL-2.0-only

#include <linux/fs.h>
#include <linux/xattr.h>
#include <linux/capability.h>
#include <linux/gfs2_ondisk.h>

#include "gfs2.h"
#include "incore.h"
#include "fscaps.h"
#include "xattr.h"
#include "glock.h"
#include "quota.h"
#include "rgrp.h"

int gfs2_get_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    struct vfs_caps *caps)
{
	struct inode *inode = d_inode(dentry);
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_holder gh;
	struct vfs_ns_cap_data nscaps;
	int ret;

	if (!gfs2_glock_is_locked_by_me(ip->i_gl)) {
		ret = gfs2_glock_nq_init(ip->i_gl, LM_ST_SHARED, LM_FLAG_ANY, &gh);
		if (ret)
			return ret;
	} else {
		gfs2_holder_mark_uninitialized(&gh);
	}
	ret = __gfs2_xattr_get(inode, XATTR_CAPS_SUFFIX, &nscaps,
			       sizeof(nscaps), GFS2_EATYPE_SECURITY);
	if (gfs2_holder_initialized(&gh))
		gfs2_glock_dq_uninit(&gh);

	if (ret < 0)
		return ret;

	return vfs_caps_from_xattr(&nop_mnt_idmap, i_user_ns(inode), caps,
				   &nscaps, ret);
}

int gfs2_set_fscaps(struct mnt_idmap *idmap, struct dentry *dentry,
		    const struct vfs_caps *caps, int setxattr_flags)
{
	struct inode *inode = d_inode(dentry);
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_holder gh;
	struct vfs_ns_cap_data nscaps, *value = NULL;
	ssize_t size = 0;
	int ret;

	ret = gfs2_qa_get(ip);
	if (ret)
		return ret;

	if (caps) {
		value = &nscaps;
		size = vfs_caps_to_xattr(&nop_mnt_idmap, i_user_ns(inode),
					 caps, value, sizeof(*value));
		if (size < 0) {
			ret = size;
			goto out;
		}
	}

	if (!gfs2_glock_is_locked_by_me(ip->i_gl)) {
		ret = gfs2_glock_nq_init(ip->i_gl, LM_ST_EXCLUSIVE, 0, &gh);
		if (ret)
			goto out;
	} else {
		if (WARN_ON_ONCE(ip->i_gl->gl_state != LM_ST_EXCLUSIVE)) {
			ret = -EIO;
			goto out;
		}
		gfs2_holder_mark_uninitialized(&gh);
	}
	ret = __gfs2_xattr_set(inode, XATTR_CAPS_SUFFIX, value, size,
			       setxattr_flags, GFS2_EATYPE_SECURITY);
	if (gfs2_holder_initialized(&gh))
		gfs2_glock_dq_uninit(&gh);

out:
	gfs2_qa_put(ip);
	return ret;
}
