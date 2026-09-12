#ifndef RT8139_DRIVER_H
#define RT8139_DRIVER_H

#include "types.h"
#include "defs.h"
#include "memlayout.h"
#include "pci.h"

#define RTL_CMD 0x37
#define RTL_CMD_RST 0x10
#define RTL_RBSTART 0x30
#define RTL_RCR 0x44
#define RTL_RCR_RBLEN_8K (0 << 11)
#define RTL_RX_SIZE (8192 + 16)

static uint rtl_io;
static uchar rxbuf[RTL_RX_SIZE] __attribute__((aligned(16)));

int rtl8139_init(void) {
    struct pci_device dev;

    if (pci_find(0x10EC, 0x8139, &dev) != 0) {
        cprintf("rtl8139: not found\n");
        return -1;
    }

    rtl_io = pci_bar_base(&dev, 0);

    cprintf("rtl8139: %d:%d.%d\n", dev.bus, dev.dev, dev.func);
    cprintf("rtl8139: io=%x irq=%d\n", rtl_io, dev.irq);

    pci_enable_busmaster(&dev);

    // Reset
    outb(rtl_io + RTL_CMD, RTL_CMD_RST);

    while (inb(rtl_io + RTL_CMD) & RTL_CMD_RST)
        ;

    cprintf("rtl8139: reset OK\n");

    // Read MAC
    uchar mac[6];

    for (int i = 0; i < 6; i++)
        mac[i] = inb(rtl_io + i);

    cprintf("rtl8139: MAC=%x:%x:%x:%x:%x:%x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    outl(rtl_io + RTL_RBSTART, V2P(rxbuf));
    outl(rtl_io + RTL_RCR, RTL_RCR_RBLEN_8K);
    outb(rtl_io + RTL_CMD, RTL_CMD_RE | RTL_CMD_TE);
    return 0;
}

#endif