
#ifndef F_CPU
#define F_CPU	16000000ul
#endif


//----Libraries----//
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "servo.h"
#include "h_bridge.h"

/*
 * Hieronder kan je de pin van de onderdelen
 * veranderen.
 * Niet vergeten de poort aan te passen in
 * alle functies, ook de MAIN functie
 */
//----Definities---//
#define debounceTime    10      // in ms
#define alarm_brug      PE5     // D pin 3
#define knipper_rood1   PG1     // D pin 40
#define knipper_rood2   PG0     // D pin 41
#define knop_open       PC3     // D pin 34
#define knop_dicht      PC5     // D pin 32
#define knop_geel       PC7     // D pin 30
#define knop_stop_o     PA6     // D pin 28
#define knop_stop_d     PA4     // D pin 26
#define knop_boot_L     PK0     // A pin 8
#define knop_boot_R     PK1     // A pin 9

//----LED's voor de brug----//
#define LED_Geel_1      PB1     // D pin 52
#define LED_Geel_2      PB2     // D pin 51
#define LED_Rood_1R     PL1     // D pin 48
#define LED_GroenR      PL3     // D pin 46
#define LED_Rood_2R     PL5     // D pin 44
#define LED_Rood_1L     PL0     // D pin 49
#define LED_GroenL      PL2     // D pin 47
#define LED_Rood_2L     PL4     // D pin 45

//----LED's voor de besturing----//
#define Best_Groen_1    PA5     // D pin 27
#define Best_Groen_2    PC4     // D pin 33
#define Best_Blauw_1    PC6     // D pin 31
#define Best_Blauw_2    PC0     // D pin 37
#define Best_Rood_1     PA7     // D pin 29
#define Best_Rood_2     PC2     // D pin 35
#define Best_Geel       PA3     // D pin 25

//----Functies----//
void init(void)
{
	init_h_bridge();
	init_servo();
}

void knipperlicht_rood(int knip_aan)
{
    if (knip_aan == 1)
    {
        PORTG &= ~(1<<knipper_rood1);
        PORTG |= (1<<knipper_rood2);
        _delay_ms(200);
        PORTG |= (1<<knipper_rood1);
        PORTG &= ~(1<<knipper_rood2);
        _delay_ms(200);
    }
}

void knipperlicht_rood_uit(int knip_uit)
{
    if (knip_uit == 1)
    {
        PORTG &= ~(1<<knipper_rood1);
        PORTG &= ~(1<<knipper_rood2);
    }
}

void slagbomen_dicht_R(int slag_dicht_R)
{
    if (slag_dicht_R == 1)
    {
        servo1_set_percentage(-30);
    }
}

void slagbomen_dicht_L(int slag_dicht_L)
{
    if (slag_dicht_L == 1)
    {
        servo2_set_percentage(-30);
    }
}

void slagbomen_open(int slag_open)
{
    if (slag_open == 1)
    {
        servo1_set_percentage(30);
        servo2_set_percentage(30);
    }
}

void brug_open(int b_open)
{
    if (b_open == 1)
    {
        h_bridge_set_percentage(30);
        _delay_ms(100);
    }
    else if (b_open == 0)
    {
        h_bridge_set_percentage(0);
        _delay_ms(100);
    }
}

void brug_dicht(int b_dicht)
{
    if (b_dicht == 1)
    {
        h_bridge_set_percentage(-30);
        _delay_ms(100);
    }
    else if (b_dicht == 0)
    {
        h_bridge_set_percentage(0);
        _delay_ms(100);
    }
}

void lichten_binnenkort_open(int Binnenkort_open_aan)
{
    if (Binnenkort_open_aan == 1)
    {
        PORTL |= (1<<LED_Rood_1R);     // aan
        PORTL |= (1<<LED_GroenR);       // aan
        PORTL &= ~(1<<LED_Rood_2R);      // uit
        //--linker lichten er bij--//
        PORTL |= (1<<LED_Rood_1L);     // aan
        PORTL |= (1<<LED_GroenL);       // aan
        PORTL &= ~(1<<LED_Rood_2L);      // uit
    }
}

void lichten_verboden(int verboden)
{
    if (verboden == 1)
    {
        PORTL |= (1<<LED_Rood_1R);     // aan
        PORTL &= ~(1<<LED_GroenR);      // uit
        PORTL &= ~(1<<LED_Rood_2R);      // uit
        //----------------------//
        PORTL |= (1<<LED_Rood_1L);     // aan
        PORTL &= ~(1<<LED_GroenL);      // uit
        PORTL &= ~(1<<LED_Rood_2L);      // uit
    }
}

void lichten_doorvaart_R(int doorvaart_R)
{
    if (doorvaart_R == 1)
    {
        PORTL &= ~(1<<LED_Rood_1R);     // uit
        PORTL |= (1<<LED_GroenR);       // aan
        PORTL &= ~(1<<LED_Rood_2R);     // uit
        //----------------------//
        PORTL |= (1<<LED_Rood_1L);      // aan
        PORTL |= (1<<LED_GroenL);       // aan
        PORTL &= ~(1<<LED_Rood_2L);     // uit
        //----------------------//
    }
}

void lichten_doorvaart_L(int doorvaart_L)
{
    if (doorvaart_L == 1)
    {
        PORTL |= (1<<LED_Rood_1R);      // aan
        PORTL |= (1<<LED_GroenR);       // aan
        PORTL &= ~(1<<LED_Rood_2R);     // uit
        //----------------------//
        PORTL &= ~(1<<LED_Rood_1L);     // uit
        PORTL |= (1<<LED_GroenL);       // aan
        PORTL &= ~(1<<LED_Rood_2L);     // uit
    }
}

void geel_1_aan(int toegestaan)
{
    if (toegestaan == 1)
    {
        PORTB |= (1<<LED_Geel_1);       // aan
        PORTB |= (1<<LED_Geel_2);      // uit
    }
}

void geel_1_uit(int toegestaan)
{
    if (toegestaan == 1)
    {
        PORTB &= ~(1<<LED_Geel_1);      // uit
        PORTB &= ~(1<<LED_Geel_2);      // uit
    }
}

void geel_2_aan(int geel_2_a)
{
    if (geel_2_a == 1)
    {
        PORTB |= (1<<LED_Geel_1);   // geel 1 aan
        PORTB &= ~(1<<LED_Geel_2);   // geel 2 aan

        PORTL |= (1<<LED_Rood_1R);  // rood 1R aan
        PORTL &= ~(1<<LED_GroenR);  // groen R uit
        PORTL |= (1<<LED_Rood_2R);  // rood 2R aan
        //---------------------//
        PORTL |= (1<<LED_Rood_1L);  // rood 1L aan
        PORTL &= ~(1<<LED_GroenL);  // groen L uit
        PORTL |= (1<<LED_Rood_2L);  // rood 2L aan
    }
}

void geel_2_uit(int geel_2_u)
{
    if(geel_2_u == 1)
    {
        PORTB &= ~(1<<LED_Geel_1);  // uit
        PORTB &= ~(1<<LED_Geel_2);  // uit
    }
}

void alarm_aan(int alarm_a)
{
    if(alarm_a == 1)
    {
        PORTE |= (1<<alarm_brug);  // aan
    }
}

void alarm_uit(int alarm_u)
{
    if(alarm_u == 1)
    {
        PORTE |= (1<<alarm_brug);   // uit
    }
}

int main(void)
{
	//----Configureer pins als output----//
    init();

    DDRG |= (1<<knipper_rood1);
    DDRG |= (1<<knipper_rood2);
    DDRL |= (1<<LED_Rood_1R);
    DDRL |= (1<<LED_Rood_2R);
    DDRL |= (1<<LED_GroenR);
    DDRL |= (1<<LED_Rood_1L);
    DDRL |= (1<<LED_Rood_2L);
    DDRL |= (1<<LED_GroenL);
    DDRB |= (1<<LED_Geel_1);
    DDRB |= (1<<LED_Geel_2);
    DDRE |= (1<<alarm_brug);

    DDRA |= (1<<Best_Groen_1);
    DDRC |= (1<<Best_Groen_2);
    DDRC |= (1<<Best_Blauw_1);
    DDRC |= (1<<Best_Blauw_2);
    DDRA |= (1<<Best_Rood_1);
    DDRC |= (1<<Best_Rood_2);
    DDRA |= (1<<Best_Geel);

    PORTA &= ~(1<<Best_Groen_1);
    PORTC &= ~(1<<Best_Groen_2);
    PORTC &= ~(1<<Best_Blauw_1);
    PORTC &= ~(1<<Best_Blauw_2);
    PORTA &= ~(1<<Best_Rood_1);
    PORTC &= ~(1<<Best_Rood_2);
    PORTA &= ~(1<<Best_Geel);

    PORTG |= (1<<knipper_rood1);
    PORTG |= (1<<knipper_rood2);
    PORTL |= (1<<LED_Rood_1R);
    PORTL |= (1<<LED_Rood_2R);
    PORTL |= (1<<LED_GroenR);
    PORTL |= (1<<LED_Rood_1L);
    PORTL |= (1<<LED_Rood_2L);
    PORTL |= (1<<LED_GroenL);
    PORTB |= (1<<LED_Geel_1);
    PORTB |= (1<<LED_Geel_2);

    PORTE &= ~(1<<alarm_brug);
    PORT_LPWM &= ~(1<<PIN_LPWM);
    PORT_RPWM &= ~(1<<PIN_RPWM);
    DDRC &= ~(1<<knop_dicht);
    DDRC &= ~(1<<knop_open);
    DDRC &= ~(1<<knop_geel);
    DDRA &= ~(1<<knop_stop_o);
    DDRA &= ~(1<<knop_stop_d);
    DDRK &= ~(1<<knop_boot_L);
    DDRK &= ~(1<<knop_boot_R);


    //----Main while loop----//
    while(1)
    {
        //----Declaratie van variabelen----//
        int bediening_aan = 1;
        int knop_stop_open = 0;
        int knop_brugOpen = 0;
        int knop_brugDicht = 0;
        int knop_geel_LED = 0;
        int knop_stop_dicht = 0;
        int situatie = 1;
        int knop_bootR = 0;
        int knop_bootL = 0;

        //slagbomen_open(1);
        h_bridge_set_percentage(0);

        while(bediening_aan != 0)
        {

            //----Configureer knoppen----//
            if (PINC & (1<<knop_open))
            {
                _delay_ms(10);
                knop_brugOpen = 10;
            }
            if (PINC & (1<<knop_dicht))
            {
                _delay_ms(10);
                knop_brugDicht = 10;
            }
            if (PINC & (1<<knop_geel))
            {
                _delay_ms(10);
                knop_geel_LED = 10;
            }
            if (PINA & (1<<knop_stop_o))
            {
                _delay_ms(10);
                knop_stop_open = 10;
            }
            if (PINA & (1<<knop_stop_d))
            {
                _delay_ms(10);
                knop_stop_dicht = 10;
            }
            if (PINK & (1<<knop_boot_L))
            {
                _delay_ms(10);
                knop_bootL = 10;
            }
            if (PINK & (1<<knop_boot_R))
            {
                _delay_ms(10);
                knop_bootR = 10;
            }

            //----Switch statement----//

            switch(situatie)
            {
                case 1:

                    PORTA |= (1<<Best_Groen_1);
                    alarm_uit(1);
                    lichten_verboden(1);
                    geel_1_aan(1);
                    knipperlicht_rood_uit(1);

                    if (knop_bootL == 10)
                    {
                        PORTA |= (1<<Best_Rood_1);
                        knop_bootL = 0;
                    }
                    if (knop_bootR == 10)
                    {
                        PORTC |= (1<<Best_Rood_2);
                        knop_bootR =0;
                    }

                    if (knop_brugOpen == 10)
                    {
                        situatie = 2;
                    }
                    else
                    {
                        situatie = 1;
                    }
                    break;

                case 2:

                    alarm_aan(1);
                    PORTA &= ~(1<<Best_Groen_1);
                    PORTC |= (1<<Best_Blauw_1);
                    PORTA |= (1<<Best_Rood_1);
                    PORTC |= (1<<Best_Rood_2);

                    for(int i = 0; i < 10; i++)
                    {
                        lichten_binnenkort_open(1);
                        knipperlicht_rood(1);
                        h_bridge_set_percentage(30);

                    }

                    knipperlicht_rood_uit(1);
                    alarm_uit(1);
                    h_bridge_set_percentage(0);
                    PORTC &= ~(1<<Best_Blauw_1);
                    PORTC |= (1<<Best_Groen_2);
                    situatie = 3;
                    break;

                case 3:

                    PORTA &= ~(1<<Best_Rood_1);
                    PORTC &= ~(1<<Best_Rood_2);

                    lichten_doorvaart_R(1);
                    _delay_ms(10000);

                    situatie = 4;
                    break;

                case 4:

                    lichten_doorvaart_L(1);
                    _delay_ms(10000);

                    situatie = 5;

                case 5:

                    lichten_verboden(1);

                    if (knop_brugDicht == 10)
                    {
                        situatie = 6;
                    }
                    break;

                case 6:

                    PORTC &= ~(1<<Best_Groen_2);
                    PORTC |= (1<<Best_Blauw_2);
                    knop_brugDicht = 0;
                    for(int x = 0; x < 6; x++)
                    {
                        h_bridge_set_percentage(-15);
                        lichten_verboden(1);
                        knipperlicht_rood(1);
                    }
                    PORTA |= (1<<Best_Groen_1);

                    situatie = 7;
                    break;

                case 7:

                    knop_brugOpen = 0;
                    h_bridge_set_percentage(0);
                    PORTC &= ~(1<<Best_Blauw_2);

                    situatie = 8;
                    break;

                case 8:

                    knipperlicht_rood_uit(1);
                    lichten_verboden(1);
                    geel_1_aan(1);

                    if (knop_bootL == 10)
                    {
                        PORTA |= (1<<Best_Rood_1);
                        knop_bootL = 0;
                    }
                    if (knop_bootR == 10)
                    {
                        PORTC |= (1<<Best_Rood_2);
                        knop_bootR =0;
                    }

                    if (knop_geel_LED == 10)
                    {
                        situatie = 9;
                    }
                    else if (knop_brugOpen == 10)
                    {
                        knop_brugOpen = 0;
                        situatie = 1;
                    }
                    else
                    {
                        situatie = 8;
                    }
                    break;

                case 9:

                    PORTA &= ~(1<<Best_Groen_1);
                    knop_geel_LED = 0;
                    PORTA |= (1<<Best_Geel);
                    geel_2_aan(1);

                    if (knop_bootL == 10)
                    {
                        PORTA |= (1<<Best_Rood_1);
                        knop_bootL = 0;
                    }
                    if (knop_bootR == 10)
                    {
                        PORTC |= (1<<Best_Rood_2);
                        knop_bootR =0;
                    }

                    if (knop_geel_LED == 10)
                    {
                        knop_geel_LED = 0;
                        geel_2_uit(1);
                        situatie = 1;
                    }
                    else if (knop_brugOpen == 10)
                    {
                        PORTA &= ~(1<<Best_Geel);
                        knop_brugOpen = 0;
                        situatie = 1;
                    }
                    else
                    {
                        situatie = 9;
                    }
                    break;
            }
        }

    }



    return 0;
}

