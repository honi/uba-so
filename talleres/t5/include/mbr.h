#ifndef __MBR_H__
#define __MBR_H__

#include "pentry.h"

class MBR 
{
private:
	const unsigned char * raw_data;
public:
	MBR(const unsigned char * data);
	const PartitionEntry & operator[](unsigned int partition_index) const;
	unsigned short signature() const;
};

#endif