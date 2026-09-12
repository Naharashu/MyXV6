#include "pci.h"

uint32_t
pci_config_address(uint bus, uint device, uint function, uint offset)
{
  return 0x80000000 | (bus << 16) | (device << 11) |
         (function << 8) | (offset & 0xFC);
}

uint32_t
pci_read32(uint bus, uint dev, uint func, uint offset)
{
  outl(0xCF8, pci_config_address(bus, dev, func, offset));
  return inl(0xCFC);
}

uint
pci_bar_base(struct pci_device *dev, int index)
{
  uint bar = dev->bar[index];

  if(bar & 1)
    return bar & ~3;
  return bar & ~0xf;
}

int
pci_find(uint vendor_id, uint device_id, struct pci_device *pcidev)
{
  int bus, dev, func;
  uint id, vendor, device, irq, classreg;

  for(bus = 0; bus < 256; bus++){
    for(dev = 0; dev < 32; dev++){
      for(func = 0; func < 8; func++){
        id = pci_read32(bus, dev, func, 0);
        vendor = id & 0xffff;
        if(vendor == 0xffff)
          continue;

        device = id >> 16;
        if(vendor_id != vendor || device_id != device)
          continue;

        pcidev->device = device;
        pcidev->vendor = vendor;
        pcidev->bus = bus;
        pcidev->func = func;
        pcidev->dev = dev;
        irq = pci_read32(bus, dev, func, 0x3C) & 0xff;
        pcidev->irq = irq;
        classreg = pci_read32(bus, dev, func, 0x08);
        pcidev->class = (classreg >> 24) & 0xff;
        pcidev->subclass = (classreg >> 16) & 0xff;
        pcidev->prog_if = (classreg >> 8) & 0xff;
        pcidev->revision = classreg & 0xff;
        pcidev->bar[0] = pci_read32(bus, dev, func, 0x10);
        pcidev->bar[1] = pci_read32(bus, dev, func, 0x14);
        pcidev->bar[2] = pci_read32(bus, dev, func, 0x18);
        pcidev->bar[3] = pci_read32(bus, dev, func, 0x1C);
        pcidev->bar[4] = pci_read32(bus, dev, func, 0x20);
        pcidev->bar[5] = pci_read32(bus, dev, func, 0x24);
        return 0;
      }
    }
  }
  return 1;
}

void
pci_write32(uint bus, uint dev, uint func, uint offset, uint value)
{
  outl(0xCF8, pci_config_address(bus, dev, func, offset));
  outl(0xCFC, value);
}

void
pci_enable_busmaster(struct pci_device *dev)
{
  uint command = pci_read32(dev->bus, dev->dev, dev->func, PCI_COMMAND);

  command |= PCI_CMD_BUSMASTER;
  pci_write32(dev->bus, dev->dev, dev->func, PCI_COMMAND, command);
}
