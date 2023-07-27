#include "SpyManagePrivate.h"
#include "SpyWorker.h"

SpyManagePrivate::SpyManagePrivate()
{
    ThreadWork = new SpyWorker();
}

SpyManagePrivate::~SpyManagePrivate()
{
}

bool SpyManagePrivate::existWorker(unsigned int interval)
{
    if (loadedDll == false || interval == 0)
        return false;

    auto search = workerMap.find(interval);
    if (search != workerMap.end())
        return true;

    return false;
}

SpyWorker* SpyManagePrivate::getWorker()
{

    return ThreadWork;
}
