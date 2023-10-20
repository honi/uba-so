#ifndef __EXT2FS_H__
#define __EXT2FS_H__

#include "fs.h"
#include "hdd.h"
#include <iostream>

#define EXT2_MAX_OPEN_FILES	64

#define EXT2_SUPER_MAGIC 0xEF53

#define EXT2_VALID_FS	1	// Unmounted cleanly
#define EXT2_ERROR_FS	2	// Errors 

#define EXT2_ERRORS_CONTINUE	1	// continue as if nothing happened
#define EXT2_ERRORS_RO			2	// remount read-only
#define EXT2_ERRORS_PANIC		3	// cause a kernel panic

#define EXT2_OS_LINUX	0	// Linux
#define EXT2_OS_HURD	1	// GNU HURD
#define EXT2_OS_MASIX	2	// MASIX
#define EXT2_OS_FREEBSD	3	// FreeBSD
#define EXT2_OS_LITES	4	// Lites

#define EXT2_GOOD_OLD_REV	0	// Revision 0
#define EXT2_DYNAMIC_REV	1	// Revision 1 with variable inode sizes, extended attributes, etc.

#define EXT2_FEATURE_COMPAT_DIR_PREALLOC	0x0001	// Block pre-allocation for new directories
#define EXT2_FEATURE_COMPAT_IMAGIC_INODES	0x0002	//  
#define EXT3_FEATURE_COMPAT_HAS_JOURNAL		0x0004	// An Ext3 journal exists
#define EXT2_FEATURE_COMPAT_EXT_ATTR		0x0008	// Extended inode attributes are present
#define EXT2_FEATURE_COMPAT_RESIZE_INO		0x0010	// Non-standard inode size used
#define EXT2_FEATURE_COMPAT_DIR_INDEX		0x0020	// Directory indexing (HTree)

#define EXT2_FEATURE_INCOMPAT_COMPRESSION	0x0001	// Disk/File compression is used
#define EXT2_FEATURE_INCOMPAT_FILETYPE		0x0002	//  
#define EXT3_FEATURE_INCOMPAT_RECOVER		0x0004	//  
#define EXT3_FEATURE_INCOMPAT_JOURNAL_DEV	0x0008	//  
#define EXT2_FEATURE_INCOMPAT_META_BG		0x0010	// 

#define EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER	0x0001	// Sparse Superblock
#define EXT2_FEATURE_RO_COMPAT_LARGE_FILE	0x0002	// Large file support, 64-bit file size
#define EXT2_FEATURE_RO_COMPAT_BTREE_DIR	0x0004	// Binary tree sorted directory files

#define EXT2_LZV1_ALG	0x00000001 
#define EXT2_LZRW3A_ALG	0x00000002 
#define EXT2_GZIP_ALG	0x00000004 
#define EXT2_BZIP2_ALG	0x00000008 
#define EXT2_LZO_ALG	0x00000010

#define EXT2_FT_UNKNOWN		0	// Unknown File Type
#define EXT2_FT_REG_FILE	1	// Regular File
#define EXT2_FT_DIR			2	// Directory File
#define EXT2_FT_CHRDEV		3	// Character Device
#define EXT2_FT_BLKDEV		4	// Block Device
#define EXT2_FT_FIFO		5	// Buffer File
#define EXT2_FT_SOCK		6	// Socket File
#define EXT2_FT_SYMLINK		7	// Symbolic Link

#define EXT2_SECRM_FL			0x00000001	// secure deletion
#define EXT2_UNRM_FL			0x00000002	// record for undelete
#define EXT2_COMPR_FL			0x00000004	// compressed file
#define EXT2_SYNC_FL			0x00000008	// synchronous updates
#define EXT2_IMMUTABLE_FL		0x00000010	// immutable file
#define EXT2_APPEND_FL			0x00000020	// append only
#define EXT2_NODUMP_FL			0x00000040	// do not dump/delete file
#define EXT2_NOATIME_FL			0x00000080	// do not update .i_atime
 //-- Reserved for compression usage --
#define EXT2_DIRTY_FL			0x00000100	// Dirty (modified)
#define EXT2_COMPRBLK_FL		0x00000200	// compressed blocks
#define EXT2_NOCOMPR_FL			0x00000400	// access raw compressed data
#define EXT2_ECOMPR_FL			0x00000800	// compression error
 //-- End of compression flags --
#define EXT2_BTREE_FL			0x00001000	// b-tree format directory
#define EXT2_INDEX_FL			0x00001000	// hash indexed directory
#define EXT2_IMAGIC_FL			0x00002000	// AFS directory
#define EXT3_JOURNAL_DATA_FL	0x00004000	// journal file data
#define EXT2_RESERVED_FL		0x80000000	// reserved for ext2 library

// -- file format --
#define EXT2_S_IFSOCK	0xC000	// socket
#define EXT2_S_IFLNK	0xA000	// symbolic link
#define EXT2_S_IFREG	0x8000	// regular file
#define EXT2_S_IFBLK	0x6000	// block device
#define EXT2_S_IFDIR	0x4000	// directory
#define EXT2_S_IFCHR	0x2000	// character device
#define EXT2_S_IFIFO	0x1000	// fifo
// -- process execution user/group override --
#define EXT2_S_ISUID	0x0800	// Set process User ID
#define EXT2_S_ISGID	0x0400	// Set process Group ID
#define EXT2_S_ISVTX	0x0200	// sticky bit
// -- access rights --
#define EXT2_S_IRUSR	0x0100	// user read
#define EXT2_S_IWUSR	0x0080	// user write
#define EXT2_S_IXUSR	0x0040	// user execute
#define EXT2_S_IRGRP	0x0020	// group read
#define EXT2_S_IWGRP	0x0010	// group write
#define EXT2_S_IXGRP	0x0008	// group execute
#define EXT2_S_IROTH	0x0004	// others read
#define EXT2_S_IWOTH	0x0002	// others write
#define EXT2_S_IXOTH	0x0001	// others execute

#define INODE_ISDIR(inode) (inode->mode & EXT2_S_IFDIR)
#define DENT_ISDIR(dent) (dent->file_type & EXT2_FT_DIR)

#define EXT2_RDIR_INODE_NUMBER	2

struct __attribute__((__packed__)) Ext2FSSuperblock
{
	unsigned int inodes_count;
	unsigned int blocks_count;
	unsigned int reserved_blocks_count;
	unsigned int free_blocks_count;
	unsigned int free_inodes_count;
	unsigned int first_data_block;
	unsigned int log_block_size;
	unsigned int log_fragment_size;
	unsigned int blocks_per_group;
	unsigned int fragments_per_group;
	unsigned int inodes_per_group;
	unsigned int mount_time;
	unsigned int write_time;
	unsigned short mount_count;
	unsigned short max_mount_count;
	unsigned short magic_bytes;
	unsigned short state;
	unsigned short errors;
	unsigned short minor_revision_level;
	unsigned int lastcheck;
	unsigned int checkinterval;
	unsigned int creator_os;
	unsigned int revision_level;
	unsigned short default_reserved_userid;
	unsigned short default_reserved_groupid;
	// -- EXT2_DYNAMIC_REV Specific --
	unsigned int first_inode;
	unsigned short inode_size;
	unsigned short block_group_number;
	unsigned int feature_compatibility;
	unsigned int feature_incompatibility;
	unsigned int readonly_feature_compatibility;
	char  uuid[16];
	char volume_name[16];
	char last_mounted[64];
	unsigned int algo_bitmap;
	// Performance hints
	unsigned char prealloc_blocks;
	unsigned char prealloc_dir_blocks;
	char alignment[2];
	// Journaling support
	char journal_uuid[16];
	unsigned int journal_inode;
	unsigned int journal_device;
	unsigned int last_orphan;
	// Directory indexing support
	unsigned int hash_seed[4];
	unsigned char default_hash_version;
	char padding[3];
	// Other options
	unsigned int default_mount_options;
	unsigned int first_meta_bg;
	char unused[760];
};

std::ostream & operator<<(std::ostream & output, const struct Ext2FSSuperblock & superblock);

struct __attribute__((__packed__)) Ext2FSBlockGroupDescriptor
{
	unsigned int block_bitmap;
	unsigned int inode_bitmap;
	unsigned int inode_table;
	short free_blocks_count;
	short free_inodes_count;
	short used_dirs_count;
	short padding;
	char reserved[12];	
};

struct __attribute__((__packed__)) Ext2FSDirEntry {
	unsigned int inode;
	unsigned short record_length;
	unsigned char name_length;
	unsigned char file_type;
	char name[];
};

std::ostream & operator<<(std::ostream & output, const struct Ext2FSDirEntry & dent);

struct __attribute__((__packed__)) Ext2FSInode {
	unsigned short mode;
	unsigned short uid;
	unsigned int size;
	unsigned int atime;
	unsigned int ctime;
	unsigned int mtime;
	unsigned int dtime;
	unsigned short gid;
	unsigned short links_count;
	unsigned int blocks;
	unsigned int flags;
	unsigned int os_dependant_1;
	unsigned int block[15];
	unsigned int generation;
	unsigned int file_acl;
	unsigned int directory_acl;
	unsigned int faddr;
	unsigned int os_dependant_2[3];
};



std::ostream & operator<<(std::ostream & output, const struct Ext2FSBlockGroupDescriptor & bg_descriptor);
std::ostream & operator<<(std::ostream & output, const struct Ext2FSInode & inode);

class Ext2FS:FS
{
public:
	// Constructors & Destructor
	Ext2FS(HDD & disk, unsigned char pnumber);
	~Ext2FS();

	fd_t open(const char * path, const char * mode);
	int read(fd_t filedesc, unsigned char * buffer, int size);
	int write(fd_t filedesc, const unsigned char * buffer, int size);
	int seek(fd_t filedesc, int offset);
	int tell(fd_t filedesc);
	void close(fd_t filedesc);

	// Methods below were made public with the only purpouse of testing
	// They should be private
	struct Ext2FSSuperblock * superblock();
	unsigned int block_groups();
	struct Ext2FSBlockGroupDescriptor * block_group(unsigned int index);
	struct Ext2FSInode * inode_for_path(const char * path);
	unsigned int blockaddr2sector(unsigned int block);

	// Unimplemented methods
	unsigned int get_block_address(struct Ext2FSInode * inode, unsigned int block_number);
	void read_block(unsigned int block_address, unsigned char * buffer);
	struct Ext2FSInode * get_file_inode_from_dir_inode(struct Ext2FSInode * from, const char * filename);
	struct Ext2FSInode * load_inode(unsigned int inode_number);
private:
	HDD & _hdd;
	unsigned char _partition_number;
	struct Ext2FSSuperblock * _superblock;
	struct Ext2FSBlockGroupDescriptor * _bgd_table;
	unsigned int _block_groups;

	unsigned int blockgroup_for_inode(unsigned int inode);
	unsigned int blockgroup_inode_index(unsigned int inode);
	fd_t get_free_fd();
	void mark_fd_as_used(fd_t fd);
	void mark_fd_as_free(fd_t fd);


	// fd_t indexed inode array
	struct Ext2FSInode _open_files[EXT2_MAX_OPEN_FILES];
	// bitmap for recording free/used status for each fd
	unsigned int _fd_status[(EXT2_MAX_OPEN_FILES / sizeof(unsigned int)) + ((EXT2_MAX_OPEN_FILES % sizeof(unsigned int))?1:0)];
	unsigned int _seek_memory[EXT2_MAX_OPEN_FILES];
};

#endif