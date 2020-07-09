#ifndef __SHARED_DATASTRUCT_HPP__
#include <stdio.h>

#define MAXSIZE  255

typedef struct _data_info{
	unsigned short  server_id;
	unsigned short  op_id;
	int      ip_address;
	int      port;
}LM_INFO;


typedef struct data_pool{
	 pthread_mutex_t mutex_;
	 unsigned char ref_count;	
	 LM_INFO  info[MAXSIZE];
}LM_POOL;
#endif
