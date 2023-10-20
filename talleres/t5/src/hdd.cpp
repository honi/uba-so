#include "hdd.h"
#include "mbr.h"
#include <stdio.h>
#include <assert.h>

HDD::HDD(const char * filename):hdd_fp(NULL)
{ 
	int res_open = this->open(filename);
	assert(res_open == HDD_SUCCESS);
}

HDD::~HDD()
{
	this->close();
}

const PartitionEntry & HDD::operator[](unsigned int index) const
{
	unsigned char * buffer = new unsigned char[SECTOR_SIZE];
	this->read(0, buffer);
	MBR mbr(buffer);
	return mbr[index];
}

int HDD::open(const char * filename)
{
	if(this->hdd_fp) return HDD_ERROR_DISK_ALREDY_OPEN;

	this->hdd_fp = fopen(filename, "rb+");
	if(this->hdd_fp) return HDD_SUCCESS;
	return HDD_ERROR_GENERAL_ERROR;
}

int HDD::read(unsigned int lba, unsigned char * buffer) const
{
	// Check wether disk is open
	if(!this->hdd_fp) return HDD_ERROR_DISK_ISNT_OPEN;

	// Move head to "lba" sector
	int unsuccessful = fseek(this->hdd_fp, lba * SECTOR_SIZE, SEEK_SET);
	if(unsuccessful) return HDD_ERROR_GENERAL_ERROR;

	// Read one sector
	int read = fread(buffer, sizeof(char), SECTOR_SIZE, this->hdd_fp);
	if(read != SECTOR_SIZE) return HDD_ERROR_GENERAL_ERROR;

	return HDD_SUCCESS;
}

int HDD::write(unsigned int lba, const unsigned char * buffer)
{
	// Check wether disk is open
	if(!this->hdd_fp) return HDD_ERROR_DISK_ISNT_OPEN;

	// Move head to "lba" sector
	int unsuccessful = fseek(this->hdd_fp, lba * SECTOR_SIZE, SEEK_SET);	// Me muevo al sector "lba"
	if(unsuccessful) return HDD_ERROR_GENERAL_ERROR;

	// Write one sector
	int writen = fwrite((const void *) buffer, sizeof(char), SECTOR_SIZE, this->hdd_fp);
	if(writen != SECTOR_SIZE) return HDD_ERROR_GENERAL_ERROR;

	return HDD_SUCCESS;
}

int HDD::close()
{
	if(!this->hdd_fp) return HDD_ERROR_DISK_ISNT_OPEN;

	if(fclose(this->hdd_fp) != 0) return HDD_ERROR_GENERAL_ERROR;

	return HDD_SUCCESS;
}