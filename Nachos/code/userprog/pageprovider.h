#include "bitmap.h"
class Semaphore;

class PageProvider
{
private:
    /* data */
    BitMap *pages;
    int reserved_pages;
    Semaphore page_providing_waiting_room;

public:
    PageProvider(int numPages);
    ~PageProvider();
    int GetEmptyPage();
    void ReleasePage(int page_index);
    int NumAvailPage();
};




