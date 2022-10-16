
#ifndef F_CPU
#define F_CPU	16000000ul
#endif


//----Libraries----//
#include <util/delay.h>
#include <avr/io.h>
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
#define knipper_rood1   PB7     // D pin 13
#define knipper_rood2   PB6     // D pin 12
#define knop_open       PF1     // A pin 1
#define knop_dicht      PF2     // A pin 2
#define knop_geel       PF3     // A pin 3
#define knop_geel_2     PF4     // A pin 4
#define knop_stop_o     PK0     // A pin 8
#define knop_stop_d     PK1     // A pin 9
#define alarm_brug      PE5     // D pin 3

#define LED_Geel_1      PC1     // D pin 36
#define LED_Geel_2      PC0     // D pin 37
#define LED_Rood_1R     PL4     // D pin 45
#define LED_GroenR      PL3     // D pin 46
#define LED_Rood_2R     PL5     // D pin 44
#define LED_Rood_1L     PL6     // D pin 43
#define LED_GroenL      PL7     // D pin 42
#define LED_Rood_2L     PG0     // D pin 41

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
        PORTB &= ~(1<<knipper_rood1);
        PORTB |= (1<<knipper_rood2);
        _delay_ms(200);
        PORTB |= (1<<knipper_rood1);
        PORTB &= ~(1<<knipper_rood2);
        _delay_ms(200);
    }
}

void knipperlicht_rood_uit(int knip_uit)
{
    if (knip_uit == 1)
    {
        PORTB |= (1<<knipper_rood1);
        PORTB |= (1<<knipper_rood2);
    }
}

void slagbomen_dicht_R(int slag_dicht_R)
{
    if (slag_dicht_R == 1)
    {
        servo1_set_percentage(-80);
    }
}

void slagbomen_dicht_L(int slag_dicht_L)
{
    if (slag_dicht_L == 1)
    {
        servo2_set_percentage(-80);
    }
}

void slagbomen_open(int slag_open)
{
    if (slag_open == 1)
    {
        servo1_set_percentage(80);
        servo2_set_percentage(80);
    }
}

void brug_open(int b_open)
{
    if (b_open == 1)
    {
        h_bridge_set_percentage(33);
    }
    else if (b_open == 0)
    {
        h_bridge_set_percentage(0);
    }
}

void brug_dicht(int b_dicht)
{
    if (b_dicht == 1)
    {
        h_bridge_set_percentage(-33);
    }
    else if (b_dicht == 0)
    {
        h_bridge_set_percentage(0);
    }
}

void lichten_binnenkort_open(int Binnenkort_open_aan)
{
    if (Binnenkort_open_aan == 1)
    {
        PORTL &= ~(1<<LED_Rood_1R);     // aan
        PORTL |= (1<<LED_GroenR);       // aan
        PORTL |= (1<<LED_Rood_2R);      // uit
        //--linker lichten er bij--//
        PORTL &= ~(1<<LED_Rood_1L);     // aan
        PORTL |= (1<<LED_GroenL);       // aan
        PORTG |= (1<<LED_Rood_2L);      // uit
    }
}

void lichten_verboden(int verboden)
{
    if (verboden == 1)
    {
        PORTL &= ~(1<<LED_Rood_1R);     // aan
        PORTL &= ~(1<<LED_GroenR);      // uit
        PORTL |= (1<<LED_Rood_2R);      // uit
        //----------------------//
        PORTL &= ~(1<<LED_Rood_1L);     // aan
        PORTL &= ~(1<<LED_GroenR);      // uit
        PORTG |= (1<<LED_Rood_2R);      // uit
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
        PORTG &= ~(1<<LED_Rood_2L);     // uit
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
        PORTG &= ~(1<<LED_Rood_2L);     // uit
    }
}

void geel_1_aan(int toegestaan)
{
    if (toegestaan == 1)
    {
        PORTC |= (1<<LED_Geel_1);       // aan
        PORTC &= ~(1<<LED_Geel_2);      // uit
    }
}

void geel_1_uit(int toegestaan)
{
    if (toegestaan == 1)
    {
        PORTC &= ~(1<<LED_Geel_1);      // uit
        PORTC &= ~(1<<LED_Geel_2);      // uit
    }
}

void geel_2_aan(int geel_2_a)
{
    if (geel_2_a == 1)
    {
        PORTC |= (1<<LED_Geel_1);   // geel 1 aan
        PORTC |= (1<<LED_Geel_2);   // geel 2 aan

        PORTL |= (1<<LED_Rood_1R);  // rood 1R aan
        PORTL &= ~(1<<LED_GroenR);  // groen R uit
        PORTL |= (1<<LED_Rood_2R);  // rood 2R aan
        //---------------------//
        PORTL |= (1<<LED_Rood_1L);  // rood 1L aan
        PORTL &= ~(1<<LED_GroenL);  // groen L uit
        PORTG |= (1<<LED_Rood_2L);  // rood 2L aan
    }
}

void geel_2_uit(int geel_2_u)
{
    if(geel_2_u == 1)
    {
        PORTC &= ~(1<<LED_Geel_1);  // uit
        PORTC &= ~(1<<LED_Geel_2);  // uit
    }
}

void alarm_aan(int alarm_a)
{
    if(alarm_a == 1)
    {
        PORTE &= ~(1<<alarm_brug);  // aan
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
    DDRB |= (1<<knipper_rood1);
    DDRB |= (1<<knipper_rood2);
    DDRL |= (1<<LED_Rood_1R);
    DDRL |= (1<<LED_Rood_2R);
    DDRL |= (1<<LED_GroenR);
    DDRL |= (1<<LED_Rood_1L);
    DDRG |= (1<<LED_Rood_2L);
    DDRL |= (1<<LED_GroenL);
    DDRC |= (1<<LED_Geel_1);
    DDRC |= (1<<LED_Geel_2);
    DDRE |= (1<<alarm_brug);
    DDR_RPWM |= (1<<PIN_RPWM);
    DDR_LPWM |= (1<<PIN_LPWM);

    PORTB |= (1<<knipper_rood1);
    PORTB |= (1<<knipper_rood2);
    PORTL |= (1<<LED_Rood_1R);
    PORTL |= (1<<LED_Rood_2R);
    PORTL |= (1<<LED_GroenR);
    PORTL |= (1<<LED_Rood_1L);
    PORTG |= (1<<LED_Rood_2L);
    PORTL |= (1<<LED_GroenL);
    PORTC |= (1<<LED_Geel_1);
    PORTC |= (1<<LED_Geel_2);

    PORTE |= (1<<alarm_brug);
    PORT_LPWM |= (1<<PIN_LPWM);
    PORT_RPWM |= (1<<PIN_RPWM);
    DDRF &= ~(1<<knop_dicht);
    DDRF &= ~(1<<knop_open);
    DDRF &= ~(1<<knop_geel);
    DDRF &= ~(1<<knop_geel_2);
    DDRK &= ~(1<<knop_stop_o);
    DDRK &= ~(1<<knop_stop_d);

    //----Main while loop----//
    while(1)
    {
        //----Declaratie van variabelen----//
        int bediening_aan = 1;
        int knop_brugOpen = 0;
        int knop_brugDicht = 0;
        int knop_geel_LED = 0;
        int knop_stop_open = 0;
        int knop_stop_dicht = 0;
        int situatie = 1;

        slagbomen_open(1);
        h_bridge_set_percentage(0);

        while(bediening_aan != 0)
        {
            //----Configureer knoppen----//
            if (!(PINF & (1<<knop_open)))
            {
                _delay_ms(debounceTime);
                knop_brugOpen = 1;

            }
            if (!(PINF & (1<<knop_dicht)))
            {
                _delay_ms(debounceTime);
                knop_brugDicht = 1;
            }
            if (!(PINF & (1<<knop_geel)))
            {
                _delay_ms(debounceTime);
                knop_geel_LED = 1;
            }
            if (!(PINK & (1<<knop_stop_o)))
            {
                _delay_ms(debounceTime);
                knop_stop_open = 1;
            }
            if (!(PINK & (1<<knop_stop_d)))
            {
                _delay_ms(debounceTime);
                knop_brugDicht = 1;
            }

            //----Switch statement----//
            switch(situatie)
            {
                case 1:

                    slagbomen_open(1);
                    lichten_verboden(1);
                    geel_1_aan(1);

                    if (knop_brugOpen == 1)
                    {
                        for (int i = 0; i < 5; i++)       // als delay voor de linker slagbomen
                        {
                            geel_1_uit(1);
                            alarm_aan(1);
                            lichten_binnenkort_open(1);
                            knipperlicht_rood(1);
                            slagbomen_dicht_R(1);

                        }
                        alarm_uit(1);
                        slagbomen_dicht_L(1);

                        situatie = 2;
                    }
                    else if (knop_geel_LED == 1)
                    {
                        situatie = 6;
                    }
                    else
                    {
                        situatie = 1;
                    }
                    break;

                case 2:

                    lichten_binnenkort_open(1);
                    knipperlicht_rood(1);
                    brug_open(1);

                    if (knop_stop_open == 1)
                    {
                        brug_open(0);

                        situatie = 3;
                    }
                    else
                    {
                        situatie = 2;
                    }
                    break;

                case 3:


                    lichten_doorvaart_R(1);
                    knipperlicht_rood_uit(1);

                    lichten_doorvaart_L(1);
                    knipperlicht_rood_uit(1);

                    lichten_verboden(1);
                    if (knop_brugDicht == 1)
                    {
                        situatie = 4;
                    }
                    else
                    {
                        situatie = 3;
                    }
                    break;

                case 4:

                    alarm_aan(1);       // alarm blijft waarschijnlijk te lang aan
                    lichten_verboden(1);
                    knipperlicht_rood(1);
                    brug_dicht(1);

                    if (knop_stop_dicht == 1)
                    {
                        brug_dicht(0);
                        alarm_uit(1);

                        situatie = 5;
                    }
                    else
                    {
                        situatie = 4;
                    }
                    break;

                case 5:

                    slagbomen_open(1);
                    knipperlicht_rood_uit(1);
                    lichten_verboden(1);
                    geel_1_aan(1);

                    if (knop_geel_LED == 1)
                    {
                        situatie = 6;
                    }
                    else
                    {
                        situatie = 5;
                    }

                    break;

                case 6:


                    geel_2_aan(1);
                    knop_geel_LED = 0;

                    if (knop_geel_LED == 1)
                    {
                        geel_2_uit(1);
                        situatie = 1;
                    }
                    else
                    {
                        situatie = 6;
                    }
                    break;
            }
        }

    }



    return 0;
}

