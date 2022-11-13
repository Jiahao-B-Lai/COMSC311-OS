////////////////////////////////////////////////////////////////////////////////
//
//  File           : cart_driver.c
//  Description    : This is the implementation of the standardized IO functions
//                   for used to access the CART storage system.
//
//  Author         : [Jiahao Lai]
//  PSU email      : [jvl6364@psu.edu]
//

// Includes
#include <stdlib.h>
#include <string.h>

// Project Includes
#include "cart_driver.h"
#include "cart_controller.h"
#include "cmpsc311_log.h"

// Implementation
#define CART_MAX_FILE_FRAMES 100

struct FileHandler {
  char name[CART_MAX_PATH_LENGTH];
  size_t frames[CART_MAX_FILE_FRAMES];
  int open;
  size_t size;
  size_t seek;
};

int cart_wrapper(CartOpCodes opcode, uint16_t CT1, uint16_t FM1, void* buff);

static struct FileHandler *files;
static size_t used_frames = 0;
static size_t n_files = 0;
static CartridgeIndex current_cartrige = 0;

////////////////////////////////////////////////////////////////////////////////
//
// Function     : cart_poweron
// Description  : Startup up the CART interface, initialize filesystem
//
// Inputs       : none
// Outputs      : 0 if successful, -1 if failure

int32_t cart_poweron(void) {
  int i;

  // init
  if (cart_wrapper(CART_OP_INITMS, 0, 0, NULL) < 0) {
    return -1;
  }

  // zero
  for (i = 0; i < CART_MAX_CARTRIDGES; i++) {
    if (cart_wrapper(CART_OP_LDCART, i, 0, NULL) < 0) {
      return -1;
    }
    if (cart_wrapper(CART_OP_BZERO, 0, 0, NULL) < 0) {
      return -1;
    }
  }

  // start at cartrige 0
  if (cart_wrapper(CART_OP_LDCART, 0, 0, NULL) < 0) {
    return -1;
  }
  current_cartrige = 0;

  // allocate meta structure
  files = calloc(CART_MAX_TOTAL_FILES, sizeof(struct FileHandler));
  for (i = 0; i < CART_MAX_TOTAL_FILES; i++) {
    int j;
    for (j = 0; j < CART_MAX_FILE_FRAMES; j++) {
      files[i].frames[j] = SIZE_MAX;
    }
  }

  // Return successfully
  return(0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : cart_poweroff
// Description  : Shut down the CART interface, close all files
//
// Inputs       : none
// Outputs      : 0 if successful, -1 if failure

int32_t cart_poweroff(void) {

  // poweroff
  if (cart_wrapper(CART_OP_POWOFF, 0, 0, NULL) < 0) {
    return -1;
  }

  // release meta structure
  free(files);

  // Return successfully
  return(0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : cart_open
// Description  : This function opens the file and returns a file handle
//
// Inputs       : path - filename of the file to open
// Outputs      : file handle if successful, -1 if failure

int16_t cart_open(char *path) {
  int16_t fd = 0;
  // try to find
  while (fd < n_files) {
    if (strcmp(files[fd].name, path) == 0) {
      files[fd].open = 1;
      files[fd].seek = 0;
      return fd;
    }
    fd++;
  }
  // create a file
  if (n_files >= CART_MAX_TOTAL_FILES) {
    // logMessage(LOG_ERROR_LEVEL, "cart_open failed: files full enough");
    return -1;
  }
  // initialization
  strcpy(files[fd].name, path);
  files[fd].open = 1;
  files[fd].size = 0;
  files[fd].seek = 0;
  n_files++;

  // THIS SHOULD RETURN A FILE HANDLE
  return fd;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : cart_close
// Description  : This function closes the file
//
// Inputs       : fd - the file descriptor
// Outputs      : 0 if successful, -1 if failure

int16_t cart_close(int16_t fd) {
  if (fd < 0 || fd >= n_files || !files[fd].open) {
    // logMessage(LOG_ERROR_LEVEL, "cart_close failed: invalid fd");
    return -1;
  }

  files[fd].open = 0;
  files[fd].seek = 0;

  // Return successfully
  return (0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : cart_read
// Description  : Reads "count" bytes from the file handle "fh" into the 
//                buffer "buf"
//
// Inputs       : fd - filename of the file to read from
//                buf - pointer to buffer to read into
//                count - number of bytes to read
// Outputs      : bytes read if successful, -1 if failure

int32_t cart_read(int16_t fd, void *buf, int32_t count) {
  size_t current_pos;
  size_t frame_start, frame_end;
  size_t bytes_to_read, bytes_have_read;
  uint8_t frame[CART_FRAME_SIZE];

  if (fd < 0 || fd >= n_files || !files[fd].open) {
    // logMessage(LOG_ERROR_LEVEL, "cart_read failed: invalid fd");
    return -1;
  }

  if (count == 0) {
    return 0;
  }

  // truncate if read beyond EOF
  if (count + files[fd].seek > files[fd].size) {
    count = files[fd].size - files[fd].seek;
  }

  // set up for reading
  bytes_to_read = count;
  bytes_have_read = 0;
  current_pos = files[fd].seek;

  // thr first frame is not start at position 0
  frame_start = files[fd].seek % CART_FRAME_SIZE;
  while (bytes_to_read > 0) {
    // get the cartrige and frame id
    int frame_idx = files[fd].frames[(int)(current_pos / CART_FRAME_SIZE)];
    int cartrige_idx = frame_idx / CART_CARTRIDGE_SIZE;
    if (current_cartrige != cartrige_idx) {
      if (cart_wrapper(CART_OP_LDCART, cartrige_idx, 0, NULL) < 0) {
        return -1;
      }
      current_cartrige = cartrige_idx;
    }
    // get the ending position inside the frame
    frame_end = frame_start + bytes_to_read;
    if (frame_end > CART_FRAME_SIZE) {
      frame_end = CART_FRAME_SIZE;
    }
    // read frame
    if (cart_wrapper(CART_OP_RDFRME, 0, frame_idx % CART_CARTRIDGE_SIZE, frame) < 0) {
      return -1;
    }
    // copy to buffer
    memcpy(buf + bytes_have_read, frame + frame_start, frame_end - frame_start);
    bytes_have_read += frame_end - frame_start;
    bytes_to_read -= frame_end - frame_start;
    current_pos += frame_end - frame_start;
    frame_start = 0;
  }

  // update position
  files[fd].seek = current_pos;

  // Return successfully
  return (count);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : cart_write
// Description  : Writes "count" bytes to the file handle "fh" from the 
//                buffer  "buf"
//
// Inputs       : fd - filename of the file to write to
//                buf - pointer to buffer to write from
//                count - number of bytes to write
// Outputs      : bytes written if successful, -1 if failure

int32_t cart_write(int16_t fd, void *buf, int32_t count) {
  size_t current_pos;
  size_t frame_start, frame_end;
  size_t bytes_to_write, bytes_have_written;
  uint8_t frame[CART_FRAME_SIZE];

  if (fd < 0 || fd >= n_files || !files[fd].open) {
    // logMessage(LOG_ERROR_LEVEL, "cart_write failed: invalid fd");
    return -1;
  }

   if (fd < 0 || fd >= n_files || !files[fd].open) {
    // logMessage(LOG_ERROR_LEVEL, "cart_read failed: invalid fd");
    return -1;
  }

  if (count == 0) {
    return 0;
  }

  // set up for writing
  bytes_to_write = count;
  bytes_have_written = 0;
  current_pos = files[fd].seek;

  // the first frame is not starting at position 0
  frame_start = files[fd].seek % CART_FRAME_SIZE;
  while (bytes_to_write > 0) {
    int frame_idx;
    int cartrige_idx;
    // get the cartrige and frame idx
    // if not exist, allocate one
    if (files[fd].frames[(int)(current_pos / CART_FRAME_SIZE)] == SIZE_MAX) {
      files[fd].frames[(int)(current_pos / CART_FRAME_SIZE)] = used_frames;
      used_frames++;
    }
    frame_idx = files[fd].frames[(int)(current_pos / CART_FRAME_SIZE)];
    cartrige_idx = frame_idx / CART_CARTRIDGE_SIZE;
    if (current_cartrige != cartrige_idx) {
      if (cart_wrapper(CART_OP_LDCART, cartrige_idx, 0, NULL) < 0) {
        return -1;
      }
      current_cartrige = cartrige_idx;
    }
    // get ending position inside the frame
    frame_end = frame_start + bytes_to_write;
    if (frame_end > CART_FRAME_SIZE) {
      frame_end = CART_FRAME_SIZE;
    }
    // read first
    if (cart_wrapper(CART_OP_RDFRME, 0, frame_idx % CART_CARTRIDGE_SIZE, frame) < 0) {
      return -1;
    }
    // copy from buffer
    memcpy(frame + frame_start, buf + bytes_have_written, frame_end - frame_start);
    // write back
    if (cart_wrapper(CART_OP_WRFRME, 0, frame_idx % CART_CARTRIDGE_SIZE, frame) < 0) {
      return -1;
    }
    bytes_have_written += frame_end - frame_start;
    bytes_to_write -= frame_end - frame_start;
    current_pos += frame_end - frame_start;
    frame_start = 0;
  }

  // update position and file size
  files[fd].seek = current_pos;
  if (files[fd].size < files[fd].seek) {
    files[fd].size = files[fd].seek;
  }

  // Return successfully
  return (count);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : cart_seek
// Description  : Seek to specific point in the file
//
// Inputs       : fd - filename of the file to write to
//                loc - offfset of file in relation to beginning of file
// Outputs      : 0 if successful, -1 if failure

int32_t cart_seek(int16_t fd, uint32_t loc) {
  if (fd < 0 || fd >= n_files || !files[fd].open) {
    // logMessage(LOG_ERROR_LEVEL, "cart_seek failed: invalid fd");
    return -1;
  }
  if (loc > files[fd].size) {
    // logMessage(LOG_ERROR_LEVEL, "cart_seek failed: invalid loc");
    return -1;
  }
  files[fd].seek = loc;

  // Return successfully
  return (0);
}


////////////////////////////////////////////////////////////////////////////////
//
// Function     : cart_wrapper
// Description  : Wrapper for cart instructions
//
int cart_wrapper(CartOpCodes opcode, uint16_t CT1, uint16_t FM1, void* buff) {
  // clear reg
  uint64_t reg = 0x0;
  int ret;
  // set up KY1
  reg |= (uint64_t)opcode << 56;
  // set up CT1
  reg |= (uint64_t)CT1 << 31;
  // set up FM1
  reg |= (uint64_t)FM1 << 15;
  // execute
  reg = cart_io_bus(reg, buff);
  // get return code
  ret = (reg >> 47) & 0x1;
  if (ret < 0) {
    // logMessage(LOG_ERROR_LEVEL, "cart io bus failed: opcode %d", opcode);
  }
  return ret;
}
