#include "pageprovider.h"
#include "system.h"
#include "synch.h"

PageProvider::PageProvider(int numPages)
{
    pages = new BitMap(numPages);
    page_providing_waiting_room = new Semaphore("page providing waiting room", 1);
    reserved_pages=0;
}

PageProvider::~PageProvider()
{
    delete pages;
}

int PageProvider::GetEmptyPage(){
    page_providing_waiting_room->P();
    int next_free_index = pages->Find();
    if (next_free_index<0){
        return -1;
    }
    memset((next_free_index * PageSize)+(machine->mainMemory), 0, PageSize);
    reserved_pages++;
    page_providing_waiting_room->V();
    return next_free_index;
}

void PageProvider::ReleasePage(int page_index){
    page_providing_waiting_room->P();
    reserved_pages--;
    pages->Clear(page_index);
    page_providing_waiting_room->V();
}

int PageProvider::NumAvailPage(){
    return pages->NumClear();
}