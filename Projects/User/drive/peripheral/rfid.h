
#ifndef __SAFE_RFID_H__
#define __SAFE_RFID_H__

#include <stdint.h>
#include "safety.h"


class safe_rfid : public safety {
	
public:
	virtual void init(void);
	virtual void reset(void){} // ¸Ãº¯ÊýÎª¿Õ
	virtual int8_t add(uint16_t &id, const uint16_t timeout);
	virtual int8_t remove(const uint16_t id_start, const uint16_t id_end);
	virtual int8_t verify(uint16_t &id, const uint16_t timeout);
	virtual int8_t poll(uint16_t &id);
	virtual int8_t read(uint16_t &id){return 0;}

private:
};

#endif
