/*
 * --------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <p-sam@d3vs.net>, <natinusala@gmail.com>, <m4x@m4xw.net>
 * wrote this file. As long as you retain this notice you can do whatever you
 * want with this stuff. If you meet any of us some day, and you think this
 * stuff is worth it, you can buy us a beer in return.  - The sys-clk authors
 * --------------------------------------------------------------------------
 */

#define NX_SERVICE_ASSUME_NON_DOMAIN

#include "nxExt/tc.h"
#include <stdatomic.h>

static Service g_tcSrv;
static atomic_uint_least64_t g_refCnt;

Result tcInitialize(void)
{
    atomic_fetch_add(&g_refCnt, 1);

    if (serviceIsActive(&g_tcSrv))
    {
        return 0;
    }

    Result rc = smGetService(&g_tcSrv, "tc");

    if (R_FAILED(rc))
    {
        tcExit();
    }

    return rc;
}

void tcExit(void)
{
    if (atomic_fetch_sub(&g_refCnt, 1) == 1)
    {
        serviceClose(&g_tcSrv);
    }
}

Result tcGetSkinTemperatureMilliC(s32 *out_millis)
{
    return serviceDispatchOut(&g_tcSrv, 9, *out_millis);
}

