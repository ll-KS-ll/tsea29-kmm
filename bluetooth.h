#define F_CPU 8000000UL
#define BAUD 115200						// Baud is set to 115200 bps on FireFly.
//#define BAUDRATE (((F_CPU / (BAUD * 16UL))) - 1)	// Calculate baud rate to put in UBRR
#define BAUDRATE 4	// KS calculated baud rate of ~3.4 but 4 is a better (still not correct) sync.

/* Init bluetooth. */
void bt_init();
/* Send a char over bluetooth, */
void bt_transmit( uint8_t data );
/* Polling for receiving data on bluetooth. */
uint8_t bt_receive( void );