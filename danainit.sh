#!/bin/sh
# This script initialises the hardware on an AlphaSmart Dana and sets up
# the RAM. After running this you get 4MB at 0x00000000 which you can use
# to run other code (such as EmuTOS).
PORT=/dev/ttyUSB0
./dbztool -p $PORT setreg \
  SCR 0x00 \
  WATCHDOG 0 \
  ICEMCR 0x08 \
  ICEMSR 0 \
  IODCR 0 \
  PBSEL 0x60 \
  PBDIR 0x60 \
  PBDATA 0 \
  PBPUEN 0 \
  PCSEL 0 \
  PCDIR 0 \
  PCDATA 0 \
  PCPDEN 0 \
  PDDIR 0 \
  PDDATA 0 \
  PDPUEN 0x1f \
  PDSEL 0x10 \
  PDPOL 0 \
  PDIRQEN 0 \
  PDKBEN 0 \
  PDIRQEG 0 \
  PESEL 0xc8 \
  PEDIR 0xc8 \
  PEDATA 8 \
  PEPUEN 0x10 \
  PFSEL 0xc5 \
  PFDIR 0x44 \
  PFDATA 0 \
  PFPUEN 0 \
  PGSEL 0x3f \
  PGDIR 0x1f \
  PGDATA 0x1d \
  PGPUEN 0 \
  PJSEL 0xcf \
  PJDIR 0xbf \
  PJDATA 0x40 \
  PJPUEN 0 \
  PKSEL 0xfd \
  PKDIR 0xf9 \
  PKDATA 0 \
  PKPUEN 0 \
  PMSEL 0x20 \
  PMDATA 0x20 \
  PMDIR 0x20 \
  PMPUEN 0 \
  CSCTRL2 0xd000 \
  CSGBA 0x8000 \
  CSGBB 0x0000 \
  CSGBC 0x8000 \
  CSGBD 0x0000 \
  CSUGBA 0x8110 \
  CSA 0x018b \
  CSD 0x1a81 \
  CSB 0x0081 \
  CSC 0x0081 \
  CSGBD 0 \
  CSCTRL1 0x4140 \
  DRAMMC 0x4020 \
  DRAMC 0x8000 \
  SDCTRL 0xc83f \
  SDCTRL 0xd03f \
  SDCTRL 0xd43f 
