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
		this->_sizeOfDir = sizeof(myfs_header);
	}
}

void MyFs::format() {

	// put the header in place
	struct myfs_header header;
	strncpy(header.magic, MYFS_MAGIC, sizeof(header.magic));
	header.version = CURR_VERSION;
	blkdevsim->write(0, sizeof(header), (const char*)&header);
}

void MyFs::create_file(std::string path_str, bool directory) {
	if(directory)
		throw std::runtime_error("not implemented");
	if(path_str.length() > FILE_NAME_SIZE)
		throw std::runtime_error("File name bigger than 10 chars");
	if(this->_sizeOfDir + TOTAL_FILE_SIZE > 1024 * 1024)
		throw std::runtime_error("Disk is full");
	dir_list_entry new_file;
	new_file.name = path_str;
	new_file.is_dir = directory;
	new_file.file_size = 0;
	this->blkdevsim->write(this->_sizeOfDir + 1, FILE_NAME_SIZE, path_str.c_str());
	this->_sizeOfDir += TOTAL_FILE_SIZE;
	this->_dir.insert(new_file); //TODO: FIX THIS PROBLEM
}

std::string MyFs::get_content(std::string path_str) {
	if(path_str.size() > FILE_NAME_SIZE)
		throw std::runtime_error("File name is bigger than 10");
	char* ans = NULL;
	int addr = sizeof(myfs_header) + FILE_NAME_SIZE + 1;
	for(int i = 0; i < this->_dir.size(); i++)
	{
		if(this->_dir[i].name == path_str)
		{
			this->blkdevsim->read(addr, this->_dir[i].file_size - FILE_NAME_SIZE, ans);
			break;
		}
		else
			addr += TOTAL_FILE_SIZE;
	}
	if(ans == NULL)
		throw std::runtime_error("File not found");
	else
		return std::string(ans);
}

void MyFs::set_content(std::string path_str, std::string content) {
	if(path_str.size() > FILE_NAME_SIZE)
		throw std::runtime_error("File name size must be 10 or less");
	if(content.size() > TOTAL_FILE_SIZE - FILE_NAME_SIZE)
		throw std::runtime_error("The contant of the file must be 1014 or less");
	int addr = sizeof(myfs_header) + FILE_NAME_SIZE + 1;
	for(int i = 0; i < this->_dir.size(); i++)
	{
		if(this->_dir[i].name == path_str)
		{
			this->blkdevsim->write(addr, TOTAL_FILE_SIZE - FILE_NAME_SIZE, content.c_str());
			this->_dir[i].file_size = FILE_NAME_SIZE + content.size();
		}
		else
			addr += TOTAL_FILE_SIZE;
	}
}

MyFs::dir_list MyFs::list_dir(std::string path_str) {
	if(path_str.size())
		throw std::runtime_error("Not implemented");
	return this->_dir;
}

