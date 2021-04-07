#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdint.h>

namespace mongo {

struct spdk_file_fd {
    void *mFile;
};

/* wiredtiger interface */
void SpdkInitializeThread(void);
void spdk_init_env(/* WT_SESSION_IMPL *session, */ const char *conf, 
		 const char *bdev, uint64_t cache_size_in_mb);
void spdk_close_env();

int spdk_fs_directory_list(const char *directory,
  const char *prefix, char ***dirlistp, uint32_t *countp, bool single);
int spdk_fs_directory_list_free(char **dirlist, uint32_t count);
int spdk_open_file(struct spdk_file_fd **fd, const char *name,
  int file_type);
int spdk_fs_exist(const char *name);
int spdk_fs_remove(const char *name);
int spdk_fs_rename(const char *from, const char *to);
int spdk_fs_size(const char *name, off_t *sizep);
int spdk_close_file(struct spdk_file_fd **fd);
// int spdk_lock_file(WT_SESSION *wt_session, bool lock); // TODO :unsupport
int spdk_size_file(struct spdk_file_fd *fd, off_t *sizep);
int spdk_sync_file(struct spdk_file_fd *fd);
int64_t spdk_read_file(struct spdk_file_fd *fd, off_t offset, size_t len, void *buf);
int spdk_write_file(struct spdk_file_fd *fd, off_t offset, size_t len, 
    void *buf /*, uint64_t stream_id*/);
int spdk_truncate_file(struct spdk_file_fd *fd, off_t len);

} // 