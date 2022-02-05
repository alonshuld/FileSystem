#include "myfs.h"
#include <string.h>
#include <iostream>
#include <math.h>
#include <sstream>

const char *MyFs::MYFS_MAGIC = "MYFS";

MyFs::MyFs(BlockDeviceSimulator *blkdevsim_):blkdevsim(blkdevsim_) {
	struct myfs_header header;
	blkdevsim->read(0, sizeof(header), (char *)&header);

	if (strncmp(header.magic, MYFS_MAGIC, sizeof(header.magic)) != 0 ||
	    (header.version != CURR_VERSION)) {
		std::cout << "Did not find myfs instance on blkdev" << std::endl;
		std::cout << "Creating..." << std::endl;
		format();
		std::cout << "Finished!" << std::endl;
	}
	this->_sizeOfDir = sizeof(header);
}

void MyFs::format() {

	// put the header in place
	struct myfs_header header;
	strncpy(header.magic, MYFS_MAGIC, sizeof(header.magic));
	header.version = CURR_VERSION;
	blkdevsim->write(0, sizeof(header), (const char*)&header);

	// TODO: put your format code here
}

void MyFs::create_file(std::string path_str, bool directory) {
	if(directory || path_str.length() > 10)
		throw std::runtime_error("not implemented");
	dir_list_entry new_file;
	new_file.name = path_str;
	new_file.is_dir = directory;
	new_file.file_size = path_str.size();
	this->_sizeOfDir += new_file.file_size;
}

std::string MyFs::get_content(std::string path_str) {
	char* ans = NULL;
	throw std::runtime_error("not implemented");
	int addr = sizeof(myfs_header) + 1;
	for(int i = 0; i < this->_dir.size(); i++)
	{
		if(this->_dir[i].name == path_str)
		{
			this->blkdevsim->read(addr, this->_dir[i].file_size - this->_dir[i].name.size(), ans);
			break;
		}
		else
			addr += this->_dir[i].file_size;
	}
	if(ans == NULL)
		return "Error: File not found";
	else
		return std::string(ans);
}

void MyFs::set_content(std::string path_str, std::string content) {
	throw std::runtime_error("not implemented");
}

MyFs::dir_list MyFs::list_dir(std::string path_str) {
	return this->_dir;
}

