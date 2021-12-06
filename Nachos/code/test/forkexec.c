/* #include "syscall.h"
main()
{

    ForkExec("../test/putchar");
    ForkExec("../test/putchar");
    //while(1);
}
 */

#include "syscall.h"

main(){
    
    ForkExec("../test/userpages0");
    ForkExec("../test/userpages1");
    
}