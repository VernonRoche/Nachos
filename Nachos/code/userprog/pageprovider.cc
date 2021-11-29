#include "pageprovider.h"
#include "system.h"

PageProvider::PageProvider(int numPages)
{
    pages = new BitMap(numPages);
}

PageProvider::~PageProvider()
{
    delete pages;
}

int PageProvider::GetEmptyPage(){
    int next_free_index = pages->Find();
    if (next_free_index<0){
        return -1;
    }
    memset((next_free_index * PageSize)+(machine->mainMemory), 0, PageSize);
    return next_free_index;
}

void PageProvider::ReleasePage(int page_index){
    pages->Clear(page_index);
}

int PageProvider::NumAvailPage(){
    return pages->NumClear();
}