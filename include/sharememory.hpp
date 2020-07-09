#ifndef __SHAREMEMORY_HPP__
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>           
#include <string.h>
#include <string>
#include <iostream>
#include <error.h>
#include "shared_datastruct.hpp"

#define SHM_NAME 	"link_manager"

class sharememory{
public:
	sharememory();
	~sharememory();
	bool memory_init();

	bool memory_open(std::string name);
	int  memory_setsize(int size);

	bool memory_getsegment();

	void memory_write(int ip_address,int port,unsigned short server_id, int op_id);
	void dump();
	void memory_read();
	void memory_closefd();
	void memory_close();
private:
	int mfd;
	LM_POOL *mpool;
	void* msegment;
	int msize;
};
#endif