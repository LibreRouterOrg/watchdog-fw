/*
    Copyright (C) 2018  Santiago Piccinini <spiccinini at altermundi punto com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#define F_CPU           (9600000UL / 8)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define WATCHDOG_KICK_IN_PIN     PB1
#define WATCHDOG_KICK_IN_DD_PORT DDRB
#define WATCHDOG_KICK_IN_PORT_IN PINB
#define WATCHDOG_KICK_IN_PORT    PORTB
#define WATHDOG_IN_INT      INT0
#define WATHDOG_INT_vect    INT0_vect
#define WATHDOG_INT_FLAG    INTF1

// RESET:
// if RESET_OUT_PIN is low then the board resets (negated logic)
#define RESET_OUT_PIN       PB4
#define RESET_OUT_DD_PORT   DDRB
#define RESET_OUT_PORT      PORTB
#define RESET_OUT_PORT_IN   PINB
#define ANY_LOGIC_CHANGE_INT 0x01

//
#define WATCHDOG_TIME_S     (10*60)
//

volatile uint32_t elapsed_without_kick_s = 0;

// KICK received
ISR(WATHDOG_INT_vect)
{
    elapsed_without_kick_s = 0;
}

void init(void) {
    // set RESET OUT to 1 before enabling as output
    RESET_OUT_PORT |= _BV(RESET_OUT_PIN);
    // set PIN mode to 'output'
    DDRB |= _BV(RESET_OUT_PIN);

    PORTB |=(1<<PB4);
    MCUCR |= ANY_LOGIC_CHANGE_INT;
    GIMSK |= _BV(INT0); // Enable INT0 interrupt (Watchdog input)
    // enable interrupts
    sei();
}

int main(void)
{

    init();

    while(1) {
        _delay_ms(1000);
        elapsed_without_kick_s += 1;

        if (elapsed_without_kick_s >= WATCHDOG_TIME_S) {
            elapsed_without_kick_s = 0;
            
            // send reset pulse
            RESET_OUT_PORT &= ~_BV(RESET_OUT_PIN);
            _delay_ms(10);
            RESET_OUT_PORT |= _BV(RESET_OUT_PIN);
        }
    }

    return 0;
}
