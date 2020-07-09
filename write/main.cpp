#include "../include/sharememory.hpp"
#include <memory>
using namespace std;

int main(int argc,char* argv[])
{

	std::shared_ptr<sharememory> shm = std::make_shared<sharememory>();

	bool flag = shm->memory_init();
	int i =30;
	while(flag == true &&  i < 1000 )
	{
			shm->memory_write(11,12,13,14+2*i);
			i++;
		
	}
	shm->memory_closefd();
	return 0;
}
