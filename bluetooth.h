#define BAUD 115200						// Baud is set to 115200 bps on FireFly.

/* Init bluetooth. */
void bt_init( unsigned long f_cpu );
/* Send a char over bluetooth, */
void bt_transmit( uint8_t data );
/* Polling for receiving data on bluetooth. */
uint8_t bt_receive( void );