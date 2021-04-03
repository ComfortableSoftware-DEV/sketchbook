#include "stk500.h"
#include "debug.h"

int stk500_send(uint8_t* buf, uint16_t len) {
  int bytesSent;

  bytesSent = Serial.write(buf, len);

//  DEBUGP(F("Bytes Sent: "));
//  DEBUGPLN(bytesSent);
//  for (int i = 0; i < len; i++) {
//    DEBUGPLN(buf[i], HEX);
//  }

  return bytesSent;
}

int stk500_recv(uint8_t* buf, uint16_t len) {
  int bytesRead;

  bytesRead = Serial.readBytes(buf, len);
  if (bytesRead < 0) {
    error(ERROR_NO_PGMR);
    return -1;
  }

  return bytesRead;
}

void stk500_drain() {
  char drained;
  while (Serial.available() > 0)
  {
    drained = Serial.read();
//    DEBUGP(F("draining: "));
//    DEBUGP(drained, HEX);
//    DEBUGP(F(" ("));
//    DEBUGP(drained);
//    DEBUGPLN(F(")"));
  }
}

int stk500_getsync() {
  uint8_t buf[2], resp[2];
  int bytesRead, attempt;

  /*
   * get in sync */
  buf[0] = Cmnd_STK_GET_SYNC;
  buf[1] = Sync_CRC_EOP;

  /*
   * First send and drain a few times to get rid of line noise
   */

//  DEBUGPLN(F("Draining once..."));
  stk500_send(buf, 2);
  stk500_drain();
//  DEBUGPLN(F("Draining twice..."));
  stk500_send(buf, 2);
  stk500_drain();

  for (attempt = 0; attempt < STK_MAX_SYNC_ATTEMPTS; attempt++) {
//    DEBUGP(F("Sending GET SYNC... attempt "));
//    DEBUGPLN(attempt);

    stk500_send(buf, 2);
    bytesRead = stk500_recv(resp, 1);
    if (bytesRead < 0) {
      error(ERROR_UNKNOWN_RESP);
      return -1;
    } else if (bytesRead == 0) {
//      DEBUGPLN(F("No Bytes Read"));
    } else if (bytesRead > 0) {
//      DEBUGP(F("Bytes Read: "));
//      DEBUGPLN(bytesRead);
//      DEBUGP(F("Response: "));
//      DEBUGPLN(resp[0], HEX);

      if (resp[0] == Resp_STK_INSYNC) {
//        DEBUGPLN(F("In Sync"));
        break;
      }
    }
  }
  if (attempt == STK_MAX_SYNC_ATTEMPTS) {
    stk500_drain();
    return -1;
  }

  bytesRead = stk500_recv(resp, 1);
  if (bytesRead < 0) {
    error(ERROR_UNKNOWN_RESP);
    return -1;
  }
  if (resp[0] != Resp_STK_OK) {
    error1(ERROR_NOT_OK, resp[0]);
    return -1;
  } else {
//    DEBUGPLN(F("Response OK"));
  }

  DEBUGPLN(F("Sync Achieved"));
  return 0;
}

static int stk500_getparm(uint16_t parm, uint16_t* value) {
  byte buf[16];
  uint16_t v;
  int tries = 0;

 retry:
  tries++;
  buf[0] = Cmnd_STK_GET_PARAMETER;
  buf[1] = parm;
  buf[2] = Sync_CRC_EOP;

  stk500_send(buf, 3);

  if (stk500_recv(buf, 1) < 0)
    exit(1);
  if (buf[0] == Resp_STK_NOSYNC) {
    if (tries > 33) {
      error(ERROR_NO_SYNC);
      return -1;
    }
   if (stk500_getsync() < 0)
      return -1;

    goto retry;
  }
  else if (buf[0] != Resp_STK_INSYNC) {
    error1(ERROR_PROTO_SYNC,buf[0]);
    return -2;
  }

  if (stk500_recv(buf, 1) < 0)
    exit(1);
  v = buf[0];

  if (stk500_recv(buf, 1) < 0)
    exit(1);
  if (buf[0] == Resp_STK_FAILED) {
    error1(ERROR_PARM_FAILED,v);
    return -3;
  }
  else if (buf[0] != Resp_STK_OK) {
    error1(ERROR_NOT_OK,buf[0]);
    return -3;
  }

  *value = v;

  return 0;
}

static int stk500_loadaddr(uint16_t addr) {
  unsigned char buf[16];
  int tries = 0;

retry:

  tries++;
  buf[0] = Cmnd_STK_LOAD_ADDRESS;
  buf[1] = addr & 0xff;
  buf[2] = (addr >> 8) & 0xff;
  buf[3] = Sync_CRC_EOP;


  stk500_send(buf, 4);

  if (stk500_recv(buf, 1) < 0) {
    error(ERROR_UNKNOWN_RESP);
    return -1;
  }
  if (buf[0] == Resp_STK_NOSYNC) {
    if (tries > 33) {
      error(ERROR_NO_SYNC);
      return -1;
    }
    if (stk500_getsync() < 0)
      return -1;
    goto retry;
  }
  else if (buf[0] != Resp_STK_INSYNC) {
    error1(ERROR_PROTO_SYNC, buf[0]);
    return -1;
  }

  if (stk500_recv(buf, 1) < 0) {
    error(ERROR_UNKNOWN_RESP);
    return -1;
  }
  if (buf[0] == Resp_STK_OK) {
    DEBUGP(F("Address Loaded: "));
    DEBUGPLN(addr, HEX);
    return 0;
  }

  error1(ERROR_PROTO_SYNC, buf[0]);
  return -1;
}

int stk500_paged_write(AVRMEM* m, uint16_t n_bytes) {
  unsigned char cmd_buf[16]; //just the header
  int memtype;
  int block_size;
  int tries;
  unsigned int n;
  unsigned int i;
  int flash;

  // EEPROM isn't supported
  memtype = 'F';
  flash = 1;

  /* build command block and send data separately on arduino */

  i = 0;
  cmd_buf[i++] = Cmnd_STK_PROG_PAGE;
  cmd_buf[i++] = (STK_PAGE_SIZE >> 8) & 0xff;
  cmd_buf[i++] = STK_PAGE_SIZE & 0xff;
  cmd_buf[i++] = memtype;
  stk500_send(cmd_buf,4);
  stk500_send(&m->buf[0], STK_PAGE_SIZE);
  cmd_buf[0] = Sync_CRC_EOP;

  // Send!
  stk500_send(cmd_buf, 1);

  if (stk500_recv(cmd_buf, 1) < 0) {
    error(ERROR_UNKNOWN_RESP);
    return -2;
  }

  if (cmd_buf[0] == Resp_STK_NOSYNC) {
    error(ERROR_NO_SYNC);
    return -3;
  } else if (cmd_buf[0] != Resp_STK_INSYNC) {
    error1(ERROR_PROTO_SYNC, cmd_buf[0]);
    return -4;
  }

  if (stk500_recv(cmd_buf, 1) < 0) {
    error(ERROR_UNKNOWN_RESP);
    return -2;
  }
  if (cmd_buf[0] != Resp_STK_OK) {
    error1(ERROR_NOT_OK,cmd_buf[0]);
    return -5;
  }

  DEBUGP(F("Wrote Bytes: "));
  DEBUGPLN(n_bytes);
  return n_bytes;
}

/*
 * issue the 'program enable' command to the AVR device
 */
static int stk500_program_enable()
{
  unsigned char buf[16];
  int tries = 0;

retry:

  tries++;

  buf[0] = Cmnd_STK_ENTER_PROGMODE;
  buf[1] = Sync_CRC_EOP;

  stk500_send( buf, 2);
  if (stk500_recv( buf, 1) < 0) {
    error(ERROR_UNKNOWN_RESP);
    return -1;
  }
  if (buf[0] == Resp_STK_NOSYNC) {
    if (tries > 33) {
      error(ERROR_NO_SYNC);
      return -1;
    }
    if (stk500_getsync() < 0) {
      error(ERROR_NO_SYNC);
      return -1;
    }
    goto retry;
  } else if (buf[0] != Resp_STK_INSYNC) {
    error1(ERROR_PROTO_SYNC, buf[0]);
    return -1;
  }

  if (stk500_recv( buf, 1) < 0) {
    error(ERROR_UNKNOWN_RESP);
    return -1;
  }
  if (buf[0] == Resp_STK_OK) {
    DEBUGPLN(F("Program Mode Enabled"));
    return 0;
  } else if (buf[0] == Resp_STK_NODEVICE) {
    error(ERROR_NO_DEVICE);
    return -1;
  }

  if(buf[0] == Resp_STK_FAILED) {
    error(ERROR_NO_PROGMODE);
    return -1;
  }

  error1(ERROR_UNKNOWN_RESP,buf[0]);
  return -1;
}

static void stk500_disable()
{
  unsigned char buf[16];
  int tries=0;

retry:

  tries++;

  buf[0] = Cmnd_STK_LEAVE_PROGMODE;
  buf[1] = Sync_CRC_EOP;

  stk500_send( buf, 2);
  if (stk500_recv( buf, 1) < 0) {
    error(ERROR_UNKNOWN_RESP);
    return;
  }
  if (buf[0] == Resp_STK_NOSYNC) {
    if (tries > 33) {
      error(ERROR_NO_SYNC);
      return;
    }
    if (stk500_getsync() < 0)
      return;
    goto retry;
  }
  else if (buf[0] != Resp_STK_INSYNC) {
    error1(ERROR_PROTO_SYNC, buf[0]);
    return;
  }

  if (stk500_recv( buf, 1) < 0) {
    error(ERROR_UNKNOWN_RESP);
    return;
  }
  if (buf[0] == Resp_STK_OK) {
    DEBUGPLN(F("Program Mode Disabled"));
    return;
  }
  else if (buf[0] == Resp_STK_NODEVICE) {
    error(ERROR_NO_DEVICE);
    return;
  }

  error1(ERROR_UNKNOWN_RESP, buf[0]);

  return;
}

/* read signature bytes - arduino version */
static int stk500_read_sig_bytes(AVRMEM* m) {
  unsigned char buf[32];

  /* Signature byte reads are always 3 bytes. */

  if (m->size < 3) {
    DEBUGPLN("memsize too small for sig byte read");
    return -1;
  }

  buf[0] = Cmnd_STK_READ_SIGN;
  buf[1] = Sync_CRC_EOP;

  stk500_send(buf, 2);

  if (stk500_recv(buf, 5) < 0) {
    return -1;
  }
  if (buf[0] == Resp_STK_NOSYNC) {
    error(ERROR_NO_SYNC);
    return -1;
  } else if (buf[0] != Resp_STK_INSYNC) {
    error1(ERROR_PROTO_SYNC,buf[0]);
    return -2;
  }
  if (buf[4] != Resp_STK_OK) {
    error1(ERROR_NOT_OK,buf[4]);
    return -3;
  }

  m->buf[0] = buf[1];
  m->buf[1] = buf[2];
  m->buf[2] = buf[3];

  return 3;
}
