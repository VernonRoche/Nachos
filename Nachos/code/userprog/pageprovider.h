#include "bitmap.h"

class PageProvider
{
private:
    /* data */
    BitMap *pages;
public:
    PageProvider(int numPages);
    ~PageProvider();
    int GetEmptyPage();
    void ReleasePage(int page_index);
    int NumAvailPage();
};




