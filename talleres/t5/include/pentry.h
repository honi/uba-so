#ifndef __PENTRY_H__
#define __PENTRY_H__

#include <iostream>

class PartitionEntry
{
	friend std::ostream & operator<<(std::ostream & output, const PartitionEntry & pentry);
private:
	const unsigned char * raw_data;
public:
	PartitionEntry(const unsigned char * data);

	unsigned char 	status() const;
	unsigned char 	start_head() const;
	unsigned short 	start_cylinder() const;
	unsigned char 	start_sector() const;
	unsigned char 	type() const;
	unsigned char 	last_head() const;
	unsigned short 	last_cylinder() const;
	unsigned char 	last_sector() const;
	unsigned int	start_lba() const;
	unsigned int	number_of_sectors() const;
};

#endif