#ifndef PCI_H
#define PCI_H

// nfoxers - PCI

#include <stdint.h>

/* total abstraction */

// PCI Vendors
#define VENDOR_INTEL                    0x8086

// PCI Classes
#define PCI_CLASS_LEGACY                0x00
#define PCI_CLASS_STORAGE               0x01
#define PCI_CLASS_NETWORK               0x02
#define PCI_CLASS_DISPLAY               0x03
#define PCI_CLASS_MULTIMEDIA            0x04
#define PCI_CLASS_MEMORY                0x05
#define PCI_CLASS_BRIDGE_DEVICE         0x06
#define PCI_CLASS_COMMUNICATION         0x07
#define PCI_CLASS_PERIHPERALS           0x08
#define PCI_CLASS_INPUT_DEVICES         0x09
#define PCI_CLASS_DOCKING_STATION       0x0a
#define PCI_CLASS_PROCESSOR             0x0b
#define PCI_CLASS_SERIAL_BUS            0x0c
#define PCI_CLASS_WIRELESS              0x0d
#define PCI_CLASS_INTELLIGENT_IO        0x0e
#define PCI_CLASS_SATELLITE             0x0f
#define PCI_CLASS_CRYPT                 0x10
#define PCI_CLASS_SIGNAL_PROCESSING     0x11
#define PCI_CLASS_UNDEFINED             0xff

// Undefined Class
#define PCI_UNCLASSIFIED                0x0000
#define PCI_VGA_COMPATIBLE              0x0001

// Mass Storage Controller
#define PCI_STORAGE_SCSI                0x0100
#define PCI_STORAGE_IDE                 0x0101
#define PCI_STORAGE_FLOPPY              0x0102
#define PCI_STORAGE_IPI                 0x0103
#define PCI_STORAGE_RAID                0x0104
#define PCI_STORAGE_ATA                 0x0105
#define PCI_STORAGE_SATA                0x0106
#define PCI_STORAGE_OTHER               0x0180

// Network Controller
#define PCI_NETWORK_ETHERNET            0x0200
#define PCI_NETWORK_TOKEN_RING          0x0201
#define PCI_NETWORK_FDDI                0x0202
#define PCI_NETWORK_ATM                 0x0203
#define PCI_NETWORK_ISDN                0x0204
#define PCI_NETWORK_WORLDFIP            0x0205
#define PCI_NETWORK_PICGMG              0x0206
#define PCI_NETWORK_OTHER               0x0280

// Display Controller
#define PCI_DISPLAY_VGA                 0x0300
#define PCI_DISPLAY_XGA                 0x0301
#define PCI_DISPLAY_3D                  0x0302
#define PCI_DISPLAY_OTHER               0x0380

// Multimedia Controller
#define PCI_MULTIMEDIA_VIDEO            0x0400
#define PCI_MULTIMEDIA_AUDIO            0x0401
#define PCI_MULTIMEDIA_PHONE            0x0402
#define PCI_MULTIMEDIA_AUDIO_DEVICE     0x0403
#define PCI_MULTIMEDIA_OTHER            0x0480

// Memory Controller
#define PCI_MEMORY_RAM                  0x0500
#define PCI_MEMORY_FLASH                0x0501
#define PCI_MEMORY_OTHER                0x0580

// Bridge Device
#define PCI_BRIDGE_HOST                 0x0600
#define PCI_BRIDGE_ISA                  0x0601
#define PCI_BRIDGE_EISA                 0x0602
#define PCI_BRIDGE_MCA                  0x0603
#define PCI_BRIDGE_PCI                  0x0604
#define PCI_BRIDGE_PCMCIA               0x0605
#define PCI_BRIDGE_NUBUS                0x0606
#define PCI_BRIDGE_CARDBUS              0x0607
#define PCI_BRIDGE_RACEWAY              0x0608
#define PCI_BRIDGE_OTHER                0x0680

// Simple Communication Controller
#define PCI_COMM_SERIAL                 0x0700
#define PCI_COMM_PARALLEL               0x0701
#define PCI_COMM_MULTIPORT              0x0702
#define PCI_COMM_MODEM                  0x0703
#define PCI_COMM_GPIB                   0x0704
#define PCI_COMM_SMARTCARD              0x0705
#define PCI_COMM_OTHER                  0x0780

// Base System Peripherals
#define PCI_SYSTEM_PIC                  0x0800
#define PCI_SYSTEM_DMA                  0x0801
#define PCI_SYSTEM_TIMER                0x0802
#define PCI_SYSTEM_RTC                  0x0803
#define PCI_SYSTEM_PCI_HOTPLUG          0x0804
#define PCI_SYSTEM_SD                   0x0805
#define PCI_SYSTEM_OTHER                0x0880

// Input Devices
#define PCI_INPUT_KEYBOARD              0x0900
#define PCI_INPUT_PEN                   0x0901
#define PCI_INPUT_MOUSE                 0x0902
#define PCI_INPUT_SCANNER               0x0903
#define PCI_INPUT_GAMEPORT              0x0904
#define PCI_INPUT_OTHER                 0x0980

// Docking Stations
#define PCI_DOCKING_GENERIC             0x0a00
#define PCI_DOCKING_OTHER               0x0a80

// Processors
#define PCI_PROCESSOR_386               0x0b00
#define PCI_PROCESSOR_486               0x0b01
#define PCI_PROCESSOR_PENTIUM           0x0b02
#define PCI_PROCESSOR_ALPHA             0x0b10
#define PCI_PROCESSOR_POWERPC           0x0b20
#define PCI_PROCESSOR_MIPS              0x0b30
#define PCI_PROCESSOR_CO                0x0b40

// Serial Bus Controllers
#define PCI_SERIAL_FIREWIRE             0x0c00
#define PCI_SERIAL_ACCESS               0x0c01
#define PCI_SERIAL_SSA                  0x0c02
#define PCI_SERIAL_USB                  0x0c03
#define PCI_SERIAL_FIBER                0x0c04
#define PCI_SERIAL_SMBUS                0x0c05

#define PCI_SERIAL_USB_UHCI             0x00
#define PCI_SERIAL_USB_OHCI             0x10
#define PCI_SERIAL_USB_EHCI             0x20
#define PCI_SERIAL_USB_XHCI             0x30
#define PCI_SERIAL_USB_OTHER            0x80

// Wireless Controllers
#define PCI_WIRELESS_IRDA               0x0d00
#define PCI_WIRLESSS_IR                 0x0d01
#define PCI_WIRLESSS_RF                 0x0d10
#define PCI_WIRLESSS_BLUETOOTH          0x0d11
#define PCI_WIRLESSS_BROADBAND          0x0d12
#define PCI_WIRLESSS_ETHERNET_A         0x0d20
#define PCI_WIRLESSS_ETHERNET_B         0x0d21
#define PCI_WIRELESS_OTHER              0x0d80

// Intelligent I/O Controllers
#define PCI_INTELLIGENT_I2O             0x0e00

// Satellite Communication Controllers
#define PCI_SATELLITE_TV                0x0f00
#define PCI_SATELLITE_AUDIO             0x0f01
#define PCI_SATELLITE_VOICE             0x0f03
#define PCI_SATELLITE_DATA              0x0f04

// Encryption/Decryption Controllers
#define PCI_CRYPT_NETWORK               0x1000
#define PCI_CRYPT_ENTERTAINMENT         0x1001
#define PCI_CRYPT_OTHER                 0x1080

// Data Acquisition and Signal Processing Controllers
#define PCI_SP_DPIO                     0x1100
#define PCI_SP_OTHER                    0x1180

struct pci_bus {
  struct pci_bus *parent;   // parent bus
  struct pci_bus *children; // chain of bridges in this bus
  struct pci_bus *next;     // chain of all p2p buses

  struct pci_dev *self;
  struct pci_dev *devices; // devices in this bridge

  void *sysdata; // hook for system specific extension

  uint8_t bus;         // bus number
  uint8_t primary;     // primary bridge number
  uint8_t secondary;   // secondary bridge number
  uint8_t subordinate; // max subordinates
};

struct pci_dev {
  struct pci_bus *bus;     // bus this device is on
  struct pci_dev *sibling; // next device on this bus
  struct pci_dev *next;    // chain of all devices

  void *sysdata;

  uint32_t devfn;  // encoded device & function index
  uint16_t vendor; // vendor id
  uint16_t device; // device id
  uint32_t class;  // 3 bytes: base, sub, prog-if
  uint32_t master : 1;

  uint8_t irq;
};

/* technical detail */

// todo: rename and document nicer for better judgment

struct pci_common_hdr {
  uint16_t vendid;
  uint16_t devid;
  uint16_t cmd;
  uint16_t status;
  uint8_t revid;
  uint8_t prog_if;
  uint8_t subclass;
  uint8_t clas;
  uint8_t clz;
  uint8_t lt;
  uint8_t htype;
  uint8_t bist;
} __attribute__((packed));

// regular device
struct pci_hdr_dev {
  uint32_t bar[6];
  uint32_t cardptr;
  uint16_t subvendid;
  uint16_t subid;
  uint32_t erombs;
  uint8_t capptr;
  uint8_t res[7];
  uint8_t iline;
  uint8_t ipin;
  uint8_t mingrant;
  uint8_t maxlat;
} __attribute__((packed));

// bridge device
struct pci_hdr_brd {
  uint32_t bar[2];
  uint8_t p_busnum;
  uint8_t s_busnum;
  uint8_t ss_busnum;
  uint8_t s_lt;
  uint8_t iobase;
  uint8_t iolim;
  uint16_t s_stat;
  uint16_t membase;
  uint16_t memlim;
  uint16_t pmembase;
  uint16_t pmemlim;
  uint32_t pupperbase;
  uint32_t pupperlim;
  uint16_t ioupperbase;
  uint16_t ioupperlim;
  uint8_t capptr;
  uint8_t res[3];
  uint32_t erombs;
  uint8_t iline;
  uint8_t ipin;
  uint16_t bctrl;
} __attribute__((packed));

// cardbus bridge
struct pci_hdr_crd {
  uint32_t cardbus_addr;
  uint8_t cl_offset;
  uint8_t res;
  uint16_t s_stat;
  uint8_t pcibusnum;
  uint8_t cbusnum;
  uint8_t ss_busnum;
  uint8_t cbus_lt;
  uint32_t membase0;
  uint32_t memlim0;
  uint32_t membase1;
  uint32_t memlim1;
  uint32_t iobase0;
  uint32_t iolim0;
  uint32_t iobase1;
  uint32_t iolim1;
  uint8_t iline;
  uint8_t ipin;
  uint16_t bctrl;
} __attribute__((packed));

struct pci_hdr {
  struct pci_common_hdr common;
  union {
    struct pci_hdr_dev dev;
    struct pci_hdr_brd bridge;
    struct pci_hdr_crd card_bridge;
  }; // anonymous union trick
} __attribute__((packed));

/* functions */

uint8_t enumerate_pcidev(struct pci_bus *parent, uint8_t dev, uint8_t func);
void enumerate_pcibus(struct pci_bus *bus);
void enumerate_pci();

void pci_lspci();

#endif
