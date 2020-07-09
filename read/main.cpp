#include "../include/sharememory.hpp"
#include <memory>
using namespace std;

int main(int argc,char* argv[])
{

	std::shared_ptr<sharememory> shm = std::make_shared<sharememory>();

	bool flag = shm->memory_init();
	
	if(flag == true )
	{
		shm->memory_read();
		//sleep(1);
		
	}
	shm->memory_closefd();
	//shm->memory_close();
	return 0;
}
