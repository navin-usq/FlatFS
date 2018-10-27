

static int do_access(const char *path, int mask) //allow file access
{
	return 0;
}

static int do_chmod(const char *path, mode_t mode) //do chmod operation
 {
	 int res;
	 res = chmod(path, mode);
	 if (res == -1)
	 	return -errno;

		return 0;
 }

static int do_chown(const char *path, uid_t uid, gid_t gid) //do chown operation
{
  int res;
  res = lchown(path, uid, gid);
  if (res == -1)
    return -errno;

  return 0;
}

static int do_utimens(const char *path, const struct timespec ts[2]) //allow to note creation time of file
{
  int res;
	if (res == -1)
    return -errno;

	return 0;
}
