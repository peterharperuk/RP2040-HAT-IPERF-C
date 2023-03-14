/**
 * Copyright (c) 2022 WIZnet Co.,Ltd
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * ----------------------------------------------------------------------------------------------------
 * Includes
 * ----------------------------------------------------------------------------------------------------
 */
#include <stdio.h>

#include "port_common.h"

#include "wizchip_conf.h"
#include "socket.h"
#include "w5x00_spi.h"
#include "w5x00_lwip.h"

#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "lwip/dhcp.h"

#include "lwip/apps/lwiperf.h"
#include "lwip/etharp.h"

/**
 * ----------------------------------------------------------------------------------------------------
 * Macros
 * ----------------------------------------------------------------------------------------------------
 */
/* Clock */
#define PLL_SYS_KHZ (133 * 1000)
//#define PLL_SYS_KHZ (144 * 1000)

/* Socket */
#define SOCKET_MACRAW 0

/* Port */
#define PORT_LWIPERF 5001

/**
 * ----------------------------------------------------------------------------------------------------
 * Variables
 * ----------------------------------------------------------------------------------------------------
 */
/* Network */
extern uint8_t mac[6];
static ip_addr_t g_ip;
static ip_addr_t g_mask;
static ip_addr_t g_gateway;
static struct dhcp g_dhcp_client;

/* LWIP */
struct netif g_netif;
lwiperf_report_fn fn;

/* WIZ5X00 PHY configuration for link speed 100MHz */
wiz_PhyConf gPhyConf = {.by = PHY_CONFBY_SW,
                        .mode = PHY_MODE_MANUAL,
                        .speed = PHY_SPEED_100,
                        .duplex = PHY_DUPLEX_FULL};

/**
 * ----------------------------------------------------------------------------------------------------
 * Functions
 * ----------------------------------------------------------------------------------------------------
 */
/* Clock */
static void set_clock_khz(void);

static void iperf_report(void *arg, enum lwiperf_report_type report_type,
                         const ip_addr_t *local_addr, u16_t local_port, const ip_addr_t *remote_addr, u16_t remote_port,
                         u32_t bytes_transferred, u32_t ms_duration, u32_t bandwidth_kbitpsec) {
    static uint32_t total_iperf_megabytes = 0;
    uint32_t mbytes = bytes_transferred / 1024 / 1024;
    float mbits = bandwidth_kbitpsec / 1000.0;

    total_iperf_megabytes += mbytes;

    printf("Completed iperf transfer of %d MBytes @ %.1f Mbits/sec\n", mbytes, mbits);
    printf("Total iperf megabytes since start %d Mbytes\n", total_iperf_megabytes);
}

/**
 * ----------------------------------------------------------------------------------------------------
 * Main
 * ----------------------------------------------------------------------------------------------------
 */
int main()
{
    /* Initialize */
    int8_t retval = 0;
    uint8_t *pack = malloc(ETHERNET_MTU);
    uint16_t pack_len = 0;
    struct pbuf *p = NULL;

    // Initialize network configuration
    IP4_ADDR(&g_ip, 0, 0, 0, 0);
    IP4_ADDR(&g_mask, 255, 255, 255, 0);
    IP4_ADDR(&g_gateway, 192, 168, 0, 1);
    set_clock_khz();

    // Initialize stdio after the clock change
    stdio_init_all();

    sleep_ms(1000 * 3); // wait for 3 seconds

    wizchip_spi_initialize((PLL_SYS_KHZ / 4) * 1000);
    wizchip_cris_initialize();

    wizchip_reset();
    wizchip_initialize();
    wizchip_check();

    // Set ethernet chip MAC address
    setSHAR(mac);
    ctlwizchip(CW_SET_PHYCONF, &gPhyConf);
    ctlwizchip(CW_RESET_PHY, 0);

    // Initialize LWIP in NO_SYS mode
    lwip_init();

    netif_add(&g_netif, &g_ip, &g_mask, &g_gateway, NULL, netif_initialize, netif_input);
    g_netif.name[0] = 'e';
    g_netif.name[1] = '0';

    // Assign callbacks for link and status
    netif_set_link_callback(&g_netif, netif_link_callback);
    netif_set_status_callback(&g_netif, netif_status_callback);

    // MACRAW socket open
    retval = socket(SOCKET_MACRAW, Sn_MR_MACRAW, PORT_LWIPERF, 0x00);

    if (retval < 0)
        printf(" MACRAW socket open failed\n");

    // Set the default interface and bring it up
    netif_set_link_up(&g_netif);
    netif_set_up(&g_netif);

    dhcp_set_struct(&g_netif, &g_dhcp_client);
    dhcp_start(&g_netif);

    printf("Waiting for IP address\n");
    bool have_ip_address = false;
    absolute_time_t ip_address_timeout = make_timeout_time_ms(30000); // 30s timeout

    /* Infinite loop */
    while (1)
    {
        if (!have_ip_address) {
            if (time_reached(ip_address_timeout)) {
                printf("Failed to get IP address\n");
                return -1;
            }
            have_ip_address = (ip_2_ip4(&g_netif.ip_addr)->addr != 0);
            if (have_ip_address) {
                // Start lwiperf server
                printf("\nReady, running iperf server at %s\n", ip4addr_ntoa(netif_ip4_addr(&g_netif)));
                if (!fn) fn = iperf_report;
                lwiperf_start_tcp_server_default(fn, NULL);
            }
        }
        getsockopt(SOCKET_MACRAW, SO_RECVBUF, &pack_len);
        if (pack_len > 0)
        {
            pack_len = recv_lwip(SOCKET_MACRAW, (uint8_t *)pack, pack_len);
            if (pack_len)
            {
                p = pbuf_alloc(PBUF_RAW, pack_len, PBUF_POOL);
                pbuf_take(p, pack, pack_len);
            }
            else
                printf(" No packet received\n");

            if (pack_len && p != NULL)
            {
                LINK_STATS_INC(link.recv);
                if (g_netif.input(p, &g_netif) != ERR_OK)
                    pbuf_free(p);
            }
        }
        /* Cyclic lwIP timers check */
        sys_check_timeouts();
    }
}

/**
 * ----------------------------------------------------------------------------------------------------
 * Functions
 * ----------------------------------------------------------------------------------------------------
 */
/* Clock */
static void set_clock_khz(void)
{
    // set a system clock frequency in khz
    set_sys_clock_khz(PLL_SYS_KHZ, true);

    // configure the specified clock
    clock_configure(
        clk_peri,
        0,                                                // No glitchless mux
        CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, // System PLL on AUX mux
        PLL_SYS_KHZ * 1000,                               // Input frequency
        PLL_SYS_KHZ * 1000                                // Output (must be same as no divider)
    );
}
