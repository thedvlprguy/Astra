#include "ext2.hpp"
#include "../drivers/ata.cpp"

Ext2::Ext2() : block_size(0), superblock_offset(1024) {}

bool Ext2::mount() {
    uint8_t buffer[512];
    ata_read_sector(2, buffer);
    Ext2Superblock* sb = (Ext2Superblock*)(buffer);
    if (sb->magic != 0xEF53) {
        return false;
    }
    superblock = *sb; // <-- IMPORTANT FIX
    block_size = 1024 << sb->log_block_size;
    return true;
}

void Ext2::read_inode(uint32_t inode_num, Ext2Inode* inode) {
    uint32_t group = (inode_num - 1) / superblock.inodes_per_group;
    uint32_t index = (inode_num - 1) % superblock.inodes_per_group;
    uint32_t block = (superblock.inodes_per_group * group + index) * sizeof(Ext2Inode) / block_size;
    uint8_t buffer[512];
    ata_read_sector(block + 2, buffer); // Adjust sector as needed
    *inode = *(Ext2Inode*)(buffer + (index % (block_size / sizeof(Ext2Inode))) * sizeof(Ext2Inode));
}

void Ext2::list_root_dir() {
    Ext2Inode root_inode;
    read_inode(2, &root_inode);
    uint8_t buffer[512];
    ata_read_sector(root_inode.block[0], buffer);
    Ext2DirEntry* dir = (Ext2DirEntry*)buffer;
    uint16_t* vga = (uint16_t*)0xb8000;
    int vga_offset = 160;
    while (dir->inode && dir->rec_len) {
        for (int i = 0; i < dir->name_len; i++) {
            vga[vga_offset++] = (0x0f << 8) | dir->name[i];
        }
        vga[vga_offset++] = (0x0f << 8) | ' ';
        dir = (Ext2DirEntry*)((uint8_t*)dir + dir->rec_len);
    }
}

Ext2 fs;  // global instance
