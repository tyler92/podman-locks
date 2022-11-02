#include "shm_lock.h"
#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int get_owner_thread(const pthread_mutex_t *mutex)
{
	// NOTE: It's not portable way at all
	return mutex->__data.__owner;
}

int main()
{
	int error = 0;
	shm_struct_t *mem;
	int owner;

	int uid = geteuid();
	char shm_buffer[PATH_MAX];
	const char *shm_path = NULL;

	if(uid != 0) {
		sprintf(shm_buffer, "libpod_rootless_lock_%d", uid);
		shm_path = shm_buffer;
	} else {
		shm_path = "/libpod_lock";
	}

	printf("Open %s ...\n", shm_path);
	mem = open_lock_shm(strdup(shm_path), 2048, &error);

	if(!mem) {
		printf("Failed to open %s: %s\n", shm_path, strerror(-error));
		return 1;
	}

	owner = get_owner_thread(&mem->segment_lock);

	if(owner) {
		printf("Segment lock owner is thread %d\n", owner);
	}

	for(int i = 0, k = 0; i < 2048 / BITMAP_SIZE; ++i) {
		for(int j = 0; j < BITMAP_SIZE; ++j) {
			owner = get_owner_thread(&mem->locks[i].locks[j]);

			if(owner) {
				printf("Lock #%d owner is thread %d\n", k, owner);
			}

			++k;
		}
	}

	return 0;
}
