#include <sys/time.h>
#include <fuse.h>
#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define FUSE_USE_VERSION 28
#define HAVE_SETXATTR
#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif

#ifdef linux
#define _XOPEN_SOURCE 500
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif



char c_key[90] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";
static const char *dirpath = "/home/xd/Documents";

void c_encrypt(char* str);
void c_decrypt(char* str);

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	int res;

	char fpath[1000];
	sprintf(fpath, "%s%s", dirpath, path);
	printf("attr [%s] [%s]\n", dirpath, path);

	int mode_encr=0;
	char xpath[1000];

	sprintf(xpath, "%s", fpath);
	printf("attr %s\n", xpath);

	char modpath[1000];
	sprintf(modpath, "%s", "/");
	/strcpy(modpath, "/");

	int i, s, sc;
	s=0;sc=0;
	for (i=0;i<strlen(xpath);i++)
		if(xpath[i]=='/') s++;

	char *tok, *last;
	last = tok = strtok(xpath, "/");

	while(tok != NULL){
		if(s==1){
			strcat(modpath, tok);
			strcat(modpath, "/");
			break;
		}
		if(strstr(tok, "encv1_")){
			printf("mode enc -> enc 1\n");
			mode_encr = 1;
		}
		sc++;
		last = tok;
		if(sc<s){
			strcat(modpath, tok);
			strcat(modpath, "/");
		}
		tok = strtok(NULL, "/");
	}
	if (mode_encr == 1)
		c_decrypt(last);
	strcat(modpath, last);
	strcpy(xpath, modpath);
	sprintf(xpath, "%s", modpath);=

	if(strstr(xpath, "encv1_"))
	mode_encr = 1;
	else if (strstr(xpath, "encv2_"))
	mode_encr = 2;

	switch (mode_encr){
	 	case 1:
	 		s=0;sc=0;
	 		for (i=0;i<strlen(xpath);i++)
	 			if(xpath[i]=='/') s++;
	 		strcpy(modpath, "/");
	 		char *tok;
	 		last = tok = strtok(xpath, "/");
	 		while(tok != NULL){
	 			sc++;
	 			last = tok;
	 			if(sc!=s){
	 				strcat(modpath, tok);
	 				strcat(modpath, "/");
	 			}
	 			tok = strtok(NULL, "/");
	 		}
	 		c_decrypt(last);
	 		strcat(modpath, last);
	 		strcpy(xpath, modpath);
	 }

	res = lstat(xpath, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_access(const char *path, int mask)
{
	int res;

	res = access(path, mask);
	if (res == -1)
		return -errno;
	return 0;
}


static int xmp_readlink(const char *path, char *buf, size_t size)
{
	int res;
	res = readlink(path, buf, size - 1);
	if (res == -1)
		return -errno;

	buf[res] = '\0';
	return 0;
}


static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000];
    if(strcmp(fpath,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else {
        sprintf(fpath, "%s/%s", dirpath, path);
    }

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	int mode_encr=0;
	char xpath[1000];

	sprintf(xpath, "%s", path);

	if(strstr(xpath, "encv1_")){
		printf("mode enc jadi 1\n");
		mode_encr = 1;
	}
	else if (strstr(xpath, "encv2_"))
		mode_encr = 2;

	printf("readdir [%s]\n", fpath);
	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	char enc[1000];
	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;

		sprintf(enc, "%s", de->d_name);
		if(mode_encr == 1){
			c_encrypt(enc);
		}

		printf("readdir %s | %s\n", xpath, enc);
		if (filler(buf, enc, &st, 0))
			break;
	}

	closedir(dp);
	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int res;
	if (S_ISREG(mode)) {
		res = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(path, mode);
	else
		res = mknod(path, mode, rdev);
	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;
	res = mkdir(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}


static int xmp_rmdir(const char *path)
{
	int res;
	res = rmdir(path);
	if (res == -1)
		return -errno;

	return 0;
}


static int xmp_rename(const char *from, const char *to)
{
	int res;
	res = rename(from, to);
	if (res == -1)
		return -errno;
	return 0;
}



static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	printf("open\n");
	int res;
	res = open(path, fi->flags);
	if (res == -1)
		return -errno;
	close(res);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	char fpath[1000];
	if (strcmp(path, "/") == 0) {
		path = dirpath;
		sprintf(fpath, "%s", path);
	}else
		sprintf(fpath, "%s%s", dirpath, path);

	int fd;
	int res;
	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;
	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;
	close(fd);
	return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;

	(void) fi;
	fd = open(path, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi) {

    (void) fi;

    int res;
    res = creat(path, mode);
    if(res == -1)
	return -errno;

    close(res);

    return 0;
}




static int xmp_fsync(const char *path, int isdatasync,
		     struct fuse_file_info *fi)
{
	(void) path;
	(void) isdatasync;
	(void) fi;
	return 0;
}

#ifdef HAVE_SETXATTR
static int xmp_setxattr(const char *path, const char *name, const char *value,
			size_t size, int flags)
{
	int res = lsetxattr(path, name, value, size, flags);
	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_getxattr(const char *path, const char *name, char *value,
			size_t size)
{
	int res = lgetxattr(path, name, value, size);
	if (res == -1)
		return -errno;
	return res;
}

static int xmp_listxattr(const char *path, char *list, size_t size)
{
	int res = llistxattr(path, list, size);
	if (res == -1)
		return -errno;
	return res;
}

static int xmp_removexattr(const char *path, const char *name)
{
	int res = lremovexattr(path, name);
	if (res == -1)
		return -errno;
	return 0;
}
#endif

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.access		= xmp_access,
	.readlink	= xmp_readlink,
	.readdir	= xmp_readdir,
	.mknod		= xmp_mknod,
	.mkdir		= xmp_mkdir,
	.symlink	= xmp_symlink,
	.unlink		= xmp_unlink,
	.rmdir		= xmp_rmdir,
	.rename		= xmp_rename,
	.link		= xmp_link,
	.chmod		= xmp_chmod,
	.chown		= xmp_chown,
	.truncate	= xmp_truncate,
	.utimens	= xmp_utimens,
	.open		= xmp_open,
	.read		= xmp_read,
	.write		= xmp_write,
	.statfs		= xmp_statfs,
	.create     = xmp_create,
	.release	= xmp_release,
	.fsync		= xmp_fsync,

#ifdef HAVE_SETXATTR
	.setxattr	    = xmp_setxattr,
	.getxattr	    = xmp_getxattr,
	.listxattr	    = xmp_listxattr,
	.removexattr	= xmp_removexattr,
#endif
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}

void c_encrypt(char* str)
{
    if(!strcmp(str, ".") || !strcmp(str, "..")) return;

    int i, j;
    int len = strlen(str);

    for(i = 0; i < len; i++){

        for(j = 0; j < 87; j++){

            if(str[i] == c_key[j]){
                str[i] = c_key[(j + len) % 87];
                break;
            }
        }
    }
}

void c_decrypt(char* str)
{
    if(!strcmp(str, ".") || !strcmp(str, "..")) return;

    int i, j;
    int len = strlen(str);

    for(i = 0 ; i < len; i++) {

        for(j = 0; j < 87; j++){

            if(str[i] == c_key[j]){
                str[i] = c_key[(j + 87 - len) % 87];
                break;
            }
        }
    }
}
