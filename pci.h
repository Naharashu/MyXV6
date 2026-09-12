#ifndef PCI_H
#define PCI_H

#include "libc/stdint.h"
#include "types.h"
#include "x86.h"

// So PCI can have max 32 devices(0..31)

enum PCIclass {
    PCI_UNCLASSIFIED,
    PCI_STORAGE, // ATA, SATA, IDE
    PCI_NETWORK,
    PCI_DISPLAY_CONTROLLER, // VGA, 3D controllers
    PCI_MULTIMEDIA,
    PCI_MEM_CONTROLLER, // RAM
    PCI_BRIDGE,
    PCI_COMMUNICATION,
    PCI_SYS_PERIPHERAL,
    PCI_INPUT_DEV,
    PCI_DOCSTATION,
    PCI_PROCESSOR,
    PCI_SERIAL_BUS,
    PCI_WIRELESS,
};

enum PCIsubclass {
    // PCI_UNCLASSIFIED (00h)
    PCI_UNCLASSIFIED_NON_VGA = 0x0000,
    PCI_UNCLASSIFIED_VGA = 0x0001,
    PCI_UNCLASSIFIED_IMAGE_COPROC = 0x0005,

    // PCI_STORAGE (01h)
    PCI_STORAGE_SCSI = 0x0100,
    PCI_STORAGE_IDE = 0x0101,
    PCI_STORAGE_FLOPPY = 0x0102,
    PCI_STORAGE_IPI = 0x0103,
    PCI_STORAGE_RAID = 0x0104,
    PCI_STORAGE_ATA = 0x0105,
    PCI_STORAGE_SATA = 0x0106,
    PCI_STORAGE_SAS = 0x0107,
    PCI_STORAGE_NVME = 0x0108,
    PCI_STORAGE_OTHER = 0x0180,

    // PCI_NETWORK (02h)
    PCI_NETWORK_ETHERNET = 0x0200,
    PCI_NETWORK_TOKEN_RING = 0x0201,
    PCI_NETWORK_FDDI = 0x0202,
    PCI_NETWORK_ATM = 0x0203,
    PCI_NETWORK_ISDN = 0x0204,
    PCI_NETWORK_WORLDFIP = 0x0205,
    PCI_NETWORK_PICMG = 0x0206,
    PCI_NETWORK_INFINIBAND = 0x0207,
    PCI_NETWORK_OTHER = 0x0280,

    // PCI_DISPLAY_CONTROLLER (03h)
    PCI_DISPLAY_VGA = 0x0300,
    PCI_DISPLAY_XGA = 0x0301,
    PCI_DISPLAY_3D = 0x0302,
    PCI_DISPLAY_OTHER = 0x0380,

    // PCI_MULTIMEDIA (04h)
    PCI_MULTIMEDIA_VIDEO = 0x0400,
    PCI_MULTIMEDIA_AUDIO = 0x0401,
    PCI_MULTIMEDIA_TELEPHONY = 0x0402,
    PCI_MULTIMEDIA_HDAUDIO = 0x0403,
    PCI_MULTIMEDIA_OTHER = 0x0480,

    // PCI_MEM_CONTROLLER (05h)
    PCI_MEM_RAM = 0x0500,
    PCI_MEM_FLASH = 0x0501,
    PCI_MEM_OTHER = 0x0580,

    // PCI_BRIDGE (06h)
    PCI_BRIDGE_HOST = 0x0600,
    PCI_BRIDGE_ISA = 0x0601,
    PCI_BRIDGE_EISA = 0x0602,
    PCI_BRIDGE_MCA = 0x0603,
    PCI_BRIDGE_PCI = 0x0604,
    PCI_BRIDGE_PCMCIA = 0x0605,
    PCI_BRIDGE_NUBUS = 0x0606,
    PCI_BRIDGE_CARDBUS = 0x0607,
    PCI_BRIDGE_RACEWAY = 0x0608,
    PCI_BRIDGE_SEMI_TRANSPARENT = 0x0609,
    PCI_BRIDGE_INFINIBAND = 0x060A,
    PCI_BRIDGE_OTHER = 0x0680,

    // PCI_COMMUNICATION (07h)
    PCI_COMM_SERIAL = 0x0700,
    PCI_COMM_PARALLEL = 0x0701,
    PCI_COMM_MULTIPORT = 0x0702,
    PCI_COMM_MODEM = 0x0703,
    PCI_COMM_GPIB = 0x0704,
    PCI_COMM_SMART_CARD = 0x0705,
    PCI_COMM_OTHER = 0x0780,

    // PCI_SYS_PERIPHERAL (08h)
    PCI_SYS_PIC = 0x0800,
    PCI_SYS_DMA = 0x0801,
    PCI_SYS_TIMER = 0x0802,
    PCI_SYS_RTC = 0x0803,
    PCI_SYS_HOTPLUG = 0x0804,
    PCI_SYS_SD_HOST = 0x0805,
    PCI_SYS_IOMMU = 0x0806,
    PCI_SYS_OTHER = 0x0880,

    // PCI_INPUT_DEV (09h)
    PCI_INPUT_KEYBOARD = 0x0900,
    PCI_INPUT_PEN = 0x0901,
    PCI_INPUT_MOUSE = 0x0902,
    PCI_INPUT_SCANNER = 0x0903,
    PCI_INPUT_GAMEPORT = 0x0904,
    PCI_INPUT_OTHER = 0x0980,

    // PCI_DOCSTATION (0Ah)
    PCI_DOCK_GENERIC = 0x0A00,
    PCI_DOCK_OTHER = 0x0A80,

    // PCI_PROCESSOR (0Bh)
    PCI_CPU_386 = 0x0B00,
    PCI_CPU_486 = 0x0B01,
    PCI_CPU_PENTIUM = 0x0B02,
    PCI_CPU_ALPHA = 0x0B10,
    PCI_CPU_POWERPC = 0x0B20,
    PCI_CPU_MIPS = 0x0B30,
    PCI_CPU_COPROC = 0x0B40,
    PCI_CPU_OTHER = 0x0B80,

    // PCI_SERIAL_BUS (0Ch)
    PCI_SERIAL_FIREWIRE = 0x0C00,
    PCI_SERIAL_ACCESS_BUS = 0x0C01,
    PCI_SERIAL_SSA = 0x0C02,
    PCI_SERIAL_USB = 0x0C03,
    PCI_SERIAL_FIBRE_CHANNEL = 0x0C04,
    PCI_SERIAL_SMBUS = 0x0C05,
    PCI_SERIAL_INFINIBAND = 0x0C06,
    PCI_SERIAL_IPMI = 0x0C07,
    PCI_SERIAL_SERCOS = 0x0C08,
    PCI_SERIAL_CANBUS = 0x0C09,

    // PCI_WIRELESS (0Dh)
    PCI_WIRELESS_IDEN = 0x0D00,
    PCI_WIRELESS_IRDA = 0x0D01,
    PCI_WIRELESS_RF = 0x0D10,
    PCI_WIRELESS_BLUETOOTH = 0x0D11,
    PCI_WIRELESS_BROADBAND = 0x0D20,
    PCI_WIRELESS_OTHER = 0x0D80
};


struct pci_device {
    uint bus;
    uint dev;
    uint func;

    uint vendor;
    uint device;

    uint irq;

    uint bar[6];

    uint class;
    uint subclass;
    uint prog_if;
    uint revision;
};

uint32_t pci_config_address(uint bus, uint device, uint function, uint offset);
uint32_t pci_read32(uint bus, uint dev, uint func, uint offset);
uint pci_bar_base(struct pci_device *dev, int index);
int pci_find(uint vendor, uint device, struct pci_device *pcidev);
void pci_write32(uint bus, uint dev, uint func, uint offset, uint value);
void pci_enable_busmaster(struct pci_device *dev);

#define PCI_COMMAND 0x04

#define PCI_CMD_IO (1 << 0)
#define PCI_CMD_MEMORY (1 << 1)
#define PCI_CMD_BUSMASTER (1 << 2)

#define RTL_REG_COMMAND   0x37   // Chip Command Register offset (1 byte)

#define RTL_CMD_BUFRESET  0x10   // Reset the Rx buffer (Bit 4)
#define RTL_CMD_TE        0x04   // Transmitter Enable (Bit 2)
#define RTL_CMD_RE        0x02   // Receiver Enable (Bit 1)
#define RTL_CMD_BUFEMTPY  0x01   // Rx Buffer Empty flag (Bit 0, Read Only)


#endif