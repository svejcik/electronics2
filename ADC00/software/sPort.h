/*#include <asm/io.h>*/
#define base0 0x378           /* default printer port base address */
#define statusport(port) (port+1) 
#define controlport(port) (port+2)
#define eppaport(port) (port+3)
#define eppdport(port) (port+4)
struct pPort {
  /* Bit-maps. "in" means read-only.
     "out" means write-only.
   control:
    0: nStrobe (out/Inverted)
    1: Linefeed (out/Inverted)
    2: nInitialize (out)
    3: nSelect-Printer (out/Inverted)
   status:
    0:
    1:
    2:
    3: nError (In)
    4: Select (In)
    5: Paper-out (In)
    6: nAck (In)
    7: Busy (In/Inverted)
   data:
    0-7: Data 0-7 (In/Out)
  */
  int baseRegister;
  int statusRegister;
  int controlRegister;
  int eppaRegister;
  int eppdRegister;
};
struct pPort readRegisters(void);
void printParallelRegisters(struct pPort);
