#include "common.h"
#include "dwt.h"

void DWT_DelayInit(void) {
    /* enable DEM */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    
    /* enable counter */
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

/*
    Limitation: at most 25 sec delay under 168MHz core clock.
*/
void DWT_DelayUs(uint32_t us) {
    uint32_t cnt = us * (SystemCoreClock/1000000); 
    uint32_t startCnt = DWT->CYCCNT;
    uint32_t endCnt = startCnt + cnt;      
    if(endCnt > startCnt)  {
        while(DWT->CYCCNT < endCnt)
            ;       
    }
    else {
        while(DWT->CYCCNT > endCnt)
            ;
        while(DWT->CYCCNT < endCnt)
            ;
    }
}
