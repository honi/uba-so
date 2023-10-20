#include "mbr.h"
#include <assert.h>

#include "pentry.h"

#define PENTRY_SIZE			16
#define BOOTCODE_SIZE 		446
#define SIGNATURE_OFFSET 	510

MBR::MBR(const unsigned char * data):raw_data(data)
{}

const PartitionEntry & MBR::operator[](unsigned int partition_index) const
{
	assert(partition_index <= 3);

	return * (new PartitionEntry(this->raw_data + (BOOTCODE_SIZE + partition_index * PENTRY_SIZE)));
}

unsigned short MBR::signature() const
{
	return * ((unsigned short *) this->raw_data + SIGNATURE_OFFSET);
}