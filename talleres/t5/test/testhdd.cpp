#include <iostream>
#include <assert.h>
#include <cstring>

#include "hdd.h"
#include "mbr.h"
#include "pentry.h"
#include "ext2fs.h"


using namespace std;

void test_file_system(Ext2FS * fs){
	cout << "=== Filesystem Superblock ===" << endl;
	cout << *(fs->superblock()) << endl;

	//Verifico que la informacion de la cantidad de bloques sea la esperada
	assert(fs->superblock()->blocks_count == 102400);
}

void test_hdd(HDD * hdd){
	unsigned char buffer[SECTOR_SIZE];
	hdd->read(0, buffer);
	MBR mbr(buffer);
	const PartitionEntry & pentry = mbr[1];
	cout << "=== Partition Data ===" << endl;
	cout << pentry << endl << endl;

	//Verifico que la LBA empiece donde se espera
	assert(pentry.start_lba() == 4096);
}

void test_block_groups(Ext2FS * fs){
	cout << "=== Block Groups Data ===" << endl;
	unsigned int block_groups = fs->block_groups();
	for(unsigned int i = 0; i < block_groups; i++)
	{
		cout << *(fs->block_group(i)) << endl;
	}
	Ext2FSBlockGroupDescriptor block_group = *(fs->block_group(1));

	//Verifico que el block group 1 tenga la información correcta
	assert(block_group.block_bitmap == 8195);
}

void test_load_inode(Ext2FS * fs){
	cout << "=== Test Load Inode ===" << endl;
    Ext2FSInode *inodo = fs->load_inode(23674);
    assert(inodo->size == 1024);
    inodo = fs->load_inode(23673);
    assert(inodo->size == 26051);
}

void test_get_block_address(Ext2FS * fs){
	cout << "=== Test Get Block Address ===" << endl;
    Ext2FSInode *inodo = fs->load_inode(23674);
    assert(inodo->size == 1024);
    int add = fs->get_block_address(inodo,0);
    assert(add == 52225);

    inodo = fs->load_inode(23673);
    assert(inodo->size == 26051);
    add = fs->get_block_address(inodo,5);
    assert(add == 60982);
}

void test_get_file_inode_from_dir_inode(Ext2FS * fs){
	cout << "=== Test Get File ===" << endl;
    unsigned char caracteres[9];
	fd_t fileDesc = fs->open("/grupos/g1/nota.txt", "cualquiera");
	fs->seek(fileDesc,13999);
	fs->read(fileDesc,caracteres,8);
	caracteres[8]=0;

  if (caracteres[7]==1) {
    cout << "REPROBADO!" << endl;
  } else {
  	cout << "APROBADO!" << endl;
  }
}

int main(int argc, char ** argv)
{
	HDD hdd(argv[1]);

	//Esto lo pueden comentar. Es sólo para ver si descomprimieron bien la imagen de disco
	test_hdd(&hdd);

	Ext2FS * fs = new Ext2FS(hdd, 1);

    //Esto lo pueden comentar. Es sólo para ver si el FS está bien
	test_file_system(fs);

	//Esto lo pueden comentar. Es sólo para ver si el FS está bien
	test_block_groups(fs);

    //Test ejercicios
    test_load_inode(fs);
    test_get_block_address(fs);
    test_get_file_inode_from_dir_inode(fs);

	return 0;
}
