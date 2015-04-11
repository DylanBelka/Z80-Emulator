#ifndef Z80_CPU_H
#define Z80_CPU_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

#define NUM_PORTS 16 

/*
Resources:
http://clrhome.org/table/
http://fms.komkon.org/GameBoy/Tech/Software.html
http://gameboy.mongenel.com/dmg/asmmemmap.html
http://tutorials.eeems.ca/ASMin28Days/lesson/day04.html
http://www.zophar.net/fileuploads/2/10807fvllz/z80-1.txt
*/

class CPU
{
public:
	CPU();
	~CPU();

	void test();
	void emulateCycle();

// non-CPU specific functions
private:
	bool loadROM(const std::string& fileName);

// registers
private:
	// 8 bit registers
	signed char A;
	signed char B;
	signed char C;
	signed char D;
	signed char E;
	signed char H;
	signed char L;

	unsigned char F;		// flag register

	// decode flag register bits
	inline bool sign() { return F & 0x80; }
	inline bool zero() { return F & 0x40; }
	inline bool half_carry() { return F & 0x10; }
	inline bool parity() { return F & 0x4; }
#define overflow() parity()
	inline bool N() { return F & 0x2; } // add or subtract
	inline bool carry() { return F & 0x1; }

	// 16 bit registers
	inline short AF() { return ((A << 8) | (F & 0xFF)); }
	inline short BC() { return ((B << 8) | (C & 0xFF)); }
	inline short DE() { return ((D << 8) | (E & 0xFF)); }
	inline short HL() { return ((H << 8) | (L & 0xFF)); }

	inline void AF(signed short val) { A = ((val >> 8) & 0xFF); F = (char)val; } // For Hb: shift the value up and mask off lower bits
	inline void BC(signed short val) { B = ((val >> 8) & 0xFF); C = (char)val; } // For Lb: cast to char which automatically masks upper bits
	inline void DE(signed short val) { D = ((val >> 8) & 0xFF); E = (char)val; }
	inline void HL(signed short val) { H = ((val >> 8) & 0xFF); L = (char)val; }

	char I;		// interrupt page address register
	short IX, IY;	// 16 bit index registers ~!GB
	unsigned short PC;		// program counter register
	char R;		// memory refresh register TODO: implement this
	unsigned short SP;		// stack pointer

	char* mem;
	char ports[NUM_PORTS];

// Flag helper functions
private:
	inline void updateSign(short reg);
	inline void resetSign();
	inline void setSign();
	 
	inline void updateZero(short reg);
	inline void resetZero();
	inline void setZero();
	 
	inline void updateHC(short reg);
	inline void resetHC();
	inline void setHC();
	 
	inline void updateParity(short reg);
	inline void resetParity();
	inline void setParity();

	inline void updateOverflow(short reg);
	inline void resetOverflow();
	inline void setOverflow();

	inline void updateN(bool add);
	inline void resetN();
	inline void setN();

	inline void updateCarry(short reg);
	inline void resetCarry();
	inline void setCarry();

// opcode functions
private:
	inline void jr(bool cond, signed char to, unsigned char opsize);
	void jp(bool cond, signed short to, unsigned char opsize);
	void cmp(const char val);
	void ret(bool cond);
	void call(bool cond);
	void rst(const unsigned char mode);

	inline const short load16();
	inline const short get16();
	inline const short get16(const short where);
	inline void set16(unsigned short& dst, const short val);

	void decodeIXInstruction(char opcode);
	void decodeIYInstruction(char opcode);
	void decodeExtendedInstruction(char opcode);
	void decodeBitInstruction(char opcode);

// interrupt functions TODO: implement these
private:
	void halt();
};

#endif