#ifndef EXT2_HPP
#define EXT2_HPP

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

struct Ext2Superblock {
    uint32_t inodes_count;
    uint32_t blocks_count;
    uint32_t reserved_blocks;
    uint32_t free_blocks;
    uint32_t free_inodes;
    uint32_t first_data_block;
    uint32_t log_block_size;
    uint32_t log_frag_size;
    uint32_t blocks_per_group;
    uint32_t frags_per_group;
    uint32_t inodes_per_group;
    uint32_t mtime;
    uint32_t wtime;
    uint16_t mnt_count;
    uint16_t max_mnt_count;
    uint16_t magic;
    // ... other fields
} __attribute__((packed));

struct Ext2Inode {
    uint16_t mode;
    uint16_t uid;
    uint32_t size;
    uint32_t atime;
    uint32_t ctime;
    uint32_t mtime;
    uint32_t dtime;
    uint16_t gid;
    uint16_t links_count;
    uint32_t blocks;
    uint32_t flags;
    uint32_t block[15];
    // ... other fields
} __attribute__((packed));

struct Ext2DirEntry {
    uint32_t inode;
    uint16_t rec_len;
    uint8_t name_len;
    uint8_t file_type;
    char name[255];
} __attribute__((packed));

class Ext2 {
public:
    Ext2();
    bool mount();
    void list_root_dir();
    void read_inode(uint32_t inode_num, Ext2Inode* inode);

private:
    Ext2Superblock superblock;
    uint32_t block_size;
    uint32_t superblock_offset;
};

extern Ext2 fs;

#endif
