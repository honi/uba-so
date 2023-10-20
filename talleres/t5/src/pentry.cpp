#include "pentry.h"
#include <iostream>

PartitionEntry::PartitionEntry(const unsigned char * data):raw_data(data)
{
}

unsigned char 	PartitionEntry::status() const 
{
	return (unsigned char) this->raw_data[0];
}

unsigned char 	PartitionEntry::start_head() const 
{
	return (unsigned char) this->raw_data[1];
}

unsigned short 	PartitionEntry::start_cylinder() const 
{
	unsigned short cylinder = 0;
	cylinder = (this->raw_data[2] & 0xC0) << 2;
	cylinder = cylinder | this->raw_data[3];
	return cylinder;
}

unsigned char 	PartitionEntry::start_sector() const 
{
	return (this->raw_data[2] & 0x3F);
}

unsigned char 	PartitionEntry::type() const 
{
	return this->raw_data[4];
}

unsigned char 	PartitionEntry::last_head() const 
{
	return (unsigned char) this->raw_data[5];
}

unsigned short 	PartitionEntry::last_cylinder() const 
{
	unsigned short cylinder = 0;
	cylinder = (this->raw_data[6] & 0xC0) << 2;
	cylinder = cylinder | this->raw_data[7];
	return cylinder;
}

unsigned char 	PartitionEntry::last_sector() const 
{
	return (this->raw_data[6] & 0x3F);
}

unsigned int	PartitionEntry::start_lba() const 
{
	return * ((unsigned int *) (this->raw_data + 8));
}

unsigned int	PartitionEntry::number_of_sectors() const 
{
	return * ((unsigned int *) (this->raw_data + 0xC));
}

std::ostream & operator<<(std::ostream & output, const PartitionEntry & pentry)
{
	output << "Status: 		0x" << std::hex << (unsigned int) pentry.status() << std::endl;

	output << "Start HEAD:		0x"  << (unsigned int) pentry.start_head() << std::endl;
	output << "Start CYL:		0x"  << (unsigned int) pentry.start_cylinder() << std::endl;
	output << "Start SECTOR: 		0x"  << (unsigned int) pentry.start_sector() << std::endl;

	output << "Type: 			0x"  << (unsigned int) pentry.type() << std::endl;

	output << "Last HEAD: 		0x"  << (unsigned int) pentry.last_head() << std::endl;
	output << "Last CYL: 		0x"  << (unsigned int) pentry.last_cylinder() << std::endl;
	output << "Last SECTOR: 		0x"  << (unsigned int) pentry.last_sector() << std::endl;

	
	output << "Start LBA: 		" << std::dec << pentry.start_lba() << std::endl;
	output << "#Sectors: 		" << pentry.number_of_sectors() << std::endl;

	return output;
}