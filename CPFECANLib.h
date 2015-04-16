/*
 * CPFECANLib.h
 *
 *  Created on: Apr 15, 2015
 *      Author: thomaswillson
 */

#ifndef CPFECANLIB_H_
#define CPFECANLIB_H_

#include <stdint.h>

class CPFECANLib {
public:
   typedef union {
      uint32_t extended;
      uint16_t standard;
   } MSG_ID;

   typedef struct {
      MSG_ID identifier;
      uint8_t dlc;
      uint8_t rtr;
      uint8_t ide;
      uint8_t* data;
   } MSG;

   enum struct CHANNEL_STATE {
      ENABLED, STANDBY, LISTENING
   };

   enum struct MOB_MODE {
      DISABLED, TX, RX, AUTOMATIC_REPLY, FRAME_BUFFER_RX
   };

   enum struct ERROR_TYPE {
      BIT_ERROR, STUFF_ERROR, CRC_ERROR, FORM_ERROR, ACK_ERROR
   };

   void disableAllMOB();
   void disableMOB(uint8_t n);
   void enableMOBAsTX(uint8_t n, MSG *msg);
   void enableMOBAsRX(uint8_t n, MSG *msg, MSG_ID mask);
   void enableMOBAsFrameBuffer(uint8_t n, MSG *msg);
};

#endif /* CPFECANLIB_H_ */
