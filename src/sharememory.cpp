#include "sharememory.hpp"



sharememory::sharememory()
{
	msegment = NULL;
	msize =0;
}


sharememory::~sharememory()
{
  
}

bool sharememory::memory_init()
{
	mfd =-1;
	int setsize =0;
	bool segflag = false;
	msize = sizeof(LM_POOL);

	bool isopen = memory_open(SHM_NAME);
	if(isopen)
	{
		setsize = memory_setsize(msize);
	}else
	{
		return false;
	}

	if(setsize !=-1)
	{
		segflag = memory_getsegment();

		if(segflag == false)
			return false;
	}else
	{
		return false;
	}

	return true;

}

bool sharememory::memory_open(std::string name)
{
	
 printf("%s,%d\n",__FUNCTION__,__LINE__);
  
	mfd = shm_open(name.c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	  if (mfd > -1) 
	  {
         printf("%s,%d\n",__FUNCTION__,__LINE__);
         return true;
      } else
      {
           printf("%s,%d\n",__FUNCTION__,__LINE__);
         return false;
      }
}


int  sharememory::memory_setsize(int size)
{
   printf("%s,%d\n",__FUNCTION__,__LINE__);
	int flag = ftruncate(mfd, size);
	 if (-1 ==flag ) 
	 {
	 		std::cout<< "sharememory::memory_setsize: " << "ftruncate failed: " << std::endl;
	 }

	 return flag;
      
}


bool sharememory::memory_getsegment(){
   printf("%s,%d\n",__FUNCTION__,__LINE__);
	 msegment = mmap(0, msize,PROT_READ | PROT_WRITE, MAP_SHARED,mfd, 0);
      if(MAP_FAILED == msegment) 
       {
           printf("%s,%d\n",__FUNCTION__,__LINE__);
           return false;
       } else {
           printf("%s,%d\n",__FUNCTION__,__LINE__);
           return true;
       }
}

	
void sharememory::memory_write(int ip_address,int port,unsigned short server_id, int op_id)
{
	 mpool = reinterpret_cast<LM_POOL *>(msegment);
     if (mpool != nullptr) 
     {
        int ret;
        pthread_mutexattr_t attr;
        ret = pthread_mutexattr_init(&attr);

        if (0 == ret)
        {
        	ret = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        	if (0 != ret) 
        	{
	            std::cout << "pthread_mutexattr_setpshared() failed " << ret;
	        }
	        ret = pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);
	        if (0 != ret) 
	        {
	        	std::cout << "pthread_mutexattr_setrobust() failed " << ret;
	        }

        } else
        {
             std::cout << "pthread_mutexattr_init() failed " << ret;
        }
        
        ret = pthread_mutex_init(&mpool->mutex_, (0==ret)?&attr:NULL);
        if (0 != ret) 
        {
        	std::cout << "pthread_mutex_init() failed " << ret;
        }
        
        ret = pthread_mutex_lock(&mpool->mutex_);
        if (0 != ret) 
        {
            std::cout << "pthread_mutex_lock() failed " << ret;
        }

        //write
        mpool->info[mpool->ref_count].server_id = server_id ;
        mpool->info[mpool->ref_count].op_id = op_id ;
        mpool->info[mpool->ref_count].ip_address = ip_address ;
        mpool->info[mpool->ref_count].port = port ;
        mpool->ref_count = mpool->ref_count +1;
       ret = pthread_mutex_unlock(&mpool->mutex_);
       if (0 != ret) 
       {
           std::cout<< "pthread_mutex_unlock() failed "<< ret;
       }

      
   }
}

void sharememory::memory_read()
{
   mpool = reinterpret_cast<LM_POOL *>(msegment);

   printf("%s,%d,%p\n",__FUNCTION__,__LINE__,msegment);
     if (mpool != nullptr) 
     {

         printf("%s,%d,%p\n",__FUNCTION__,__LINE__,msegment);
        int ret;
        pthread_mutexattr_t attr;
        ret = pthread_mutexattr_init(&attr);

        if (0 == ret)
        {
          ret = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
          if (0 != ret) 
          {
              std::cout << "pthread_mutexattr_setpshared() failed " << ret;
          }
          ret = pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);
          if (0 != ret) 
          {
            std::cout << "pthread_mutexattr_setrobust() failed " << ret;
          }

        } else
        {
             std::cout << "pthread_mutexattr_init() failed " << ret;
        }
        
        ret = pthread_mutex_init(&mpool->mutex_, (0==ret)?&attr:NULL);
        if (0 != ret) 
        {
          std::cout << "pthread_mutex_init() failed " << ret;
        }
        
        ret = pthread_mutex_lock(&mpool->mutex_);
        if (0 != ret) 
        {
            std::cout << "pthread_mutex_lock() failed " << ret;
        }

       dump();

       ret = pthread_mutex_unlock(&mpool->mutex_);
       if (0 != ret) 
       {
           std::cout<< "pthread_mutex_unlock() failed "<< ret;
       }
       
   }else
      std::cout<< "~~~~~~~~~~~~~ "<<"bad read: " << std::endl;
       
}

void sharememory::dump()
{
   printf("%s,%d,%p\n",__FUNCTION__,__LINE__,msegment);
	for(int i =0; i< mpool->ref_count;i++)
	{
    printf("%s,%d,%p\n",__FUNCTION__,__LINE__,msegment);
	   	std::cout<<"service_id:"<< mpool->info[i].server_id 
	   			<<" op_id:"<< mpool->info[i].op_id 
	   			<<" ip_address:"<< mpool->info[i].ip_address 
	   			<<" port:"<< mpool->info[i].port <<std::endl;
	}


}

void sharememory::memory_closefd()
{
       if (-1 == ::close(mfd)) 
       {
            std::cout<< "ipcmd::shareMemory:: "<<"close failed: " << std::endl;
       }

}


void sharememory::memory_close()
{

  if (mpool !=NULL ) 
  {
            if (-1 == ::munmap(mpool, sizeof(LM_POOL))) 
            {
                std::cout  << "ipcmd::shareMemory_exit:: "
                        "munmap failed: " << std::endl;
            } else 
            {
                std::cout  <<  "ipcmd::shareMemory_exit:: "
                        "munmap succeeded." << std::endl;
                msegment = nullptr;   
                mpool = nullptr; 
                msize =0;
                mfd =-1;
               shm_unlink(SHM_NAME);
                
            }
      

    }
}