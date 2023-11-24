void my_copy_ext2(char* src, char* dst) {
    Ext2FSInode src_inode = inode_for_path(src);
    Ext2FSInode dst_inode = inode_for_path(dst);

    unsigned char* buffer = (unsigned char*) malloc(BLOCK_SIZE);
    uint num_blocks = ceil(dst_inode.size / BLOCK_SIZE);

    for (uint block_num = 0; block_num < num_blocks; block_num++) {
        uint src_block_addr = get_block_address(src_inode, block_num);
        uint dst_block_addr = get_free_block_address(dst_inode);
        add_block_address_to_inode(dst_inode, dst_block_addr, block_num);
        read_block(src_block_addr, buffer);
        write_block(dst_block_addr, buffer);
    }

    free(buffer);
}

void my_copy_fat(char* src, char* dst) {
    uint src_block_addr = get_init_block_for_path(src);
    uint dst_block_addr = get_init_block_for_path(dst);
    uint src_block_num = src_block_addr / BLOCK_SIZE;
    uint dst_block_num = dst_block_addr / BLOCK_SIZE;

    unsigned char* buffer = (unsigned char*) malloc(BLOCK_SIZE);

    while (1) {
        read_block(src_block_addr, buffer);
        write_block(dst_block_addr, buffer);

        if (FAT[src_block_num] == EOF) {
            FAT[dst_block_num] = EOF;
            break;
        } else {
            src_block_addr = FAT[src_block_num] * BLOCK_SIZE;
            dst_block_addr = get_free_block_address();
            uint next_block_num = dst_block_addr / BLOCK_SIZE;
            FAT[dst_block_num] = next_block_num
            dst_block_num = next_block_num;
        }
    }

    free(buffer);
}
