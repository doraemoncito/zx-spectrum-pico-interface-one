/*
 * ZX Pico IF1 Firmware, a Raspberry Pi Pico based ZX Interface One emulator
 * Copyright (C) 2023 Derek Fountain
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/* These are the FAT filesystem library headers */
#include "f_util.h"
#include "ff.h"
#include "rtc.h"
#include "sd_card.h"
#include "hw_config.h"

#include <stdint.h>
#include "sd_card.h"

/* There's only one SD card reader on the device, and this is it's file object */
static FIL fil;
static sd_card_t *pSD;

static uint8_t sd_card_mounted = 0;

uint8_t query_ds_card_mounted( void )
{
  return sd_card_mounted;
}


uint8_t mount_sd_card( void )
{
  sd_card_mounted = 0;

  /* FatFs, on the SD card. Set up via hw_config.c */
  pSD = sd_get_by_num( 0 );
  if( f_mount( &pSD->fatfs, pSD->pcName, 1 ) != FR_OK )
    return 1;

  sd_card_mounted = 1;
  return 0;
}


uint8_t unmount_sd_card( void )
{
  if( sd_card_mounted )
    f_unmount(pSD->pcName);

  sd_card_mounted = 0;
  return 0;
}


uint8_t read_mdr_file( uint8_t *filename, uint8_t *buffer, uint32_t max_length, uint32_t *bytes_read_ptr )
{
  FIL fsrc;
  UINT bytes_read;

  FRESULT fr = f_open( &fsrc, filename, FA_READ );
  if( fr )
    return (uint8_t)fr;

  *bytes_read_ptr = 0;
  f_read( &fsrc, buffer, max_length, &bytes_read );

  f_close(&fsrc);

  *bytes_read_ptr = bytes_read;

  return 0;
}