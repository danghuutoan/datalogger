#ifndef __SD_CARD_H__
#define __SD_CARD_H__
#include "diskio.h"

DSTATUS MMC_disk_status(void);
DSTATUS MMC_disk_initialize(void);
DRESULT MMC_disk_read(BYTE *buff,		/* Data buffer to store read data */
											DWORD sector,	/* Sector address in LBA */
											UINT count		/* Number of sectors to read */);
DRESULT MMC_disk_write(	const BYTE *buff,	/* Data to be written */
												DWORD sector,		/* Sector address in LBA */
												UINT count			/* Number of sectors to write */);

DRESULT MMC_disk_ioctl (
												BYTE ctrl,		/* Control code */
												void *buff		/* Buffer to send/receive control data */
);
DWORD get_fattime (void);	
#ifndef RAMFUNC
#define RAMFUNC
#endif
RAMFUNC void disk_timerproc (void);												
#endif
