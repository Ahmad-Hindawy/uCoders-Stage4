#include "DIO_priv.h"
#include "DIO_config.h"
#include "DIO_Init.h"
#include "utils.h"
#include "STD_TYPES.h"
#define PORTA_DIRECTION CONCBIT(DIO_PIN7_DIR,DIO_PIN6_DIR,DIO_PIN5_DIR,DIO_PIN4_DIR,DIO_PIN3_DIR,DIO_PIN2_DIR,DIO_PIN1_DIR,DIO_PIN0_DIR);
#define PORTB_DIRECTION CONCBIT(DIO_PIN15_DIR,DIO_PIN14_DIR,DIO_PIN13_DIR,DIO_PIN12_DIR,DIO_PIN11_DIR,DIO_PIN10_DIR,DIO_PIN9_DIR,DIO_PIN8_DIR);
#define PORTC_DIRECTION CONCBIT(DIO_PIN23_DIR,DIO_PIN22_DIR,DIO_PIN21_DIR,DIO_PIN20_DIR,DIO_PIN19_DIR,DIO_PIN18_DIR,DIO_PIN17_DIR,DIO_PIN16_DIR);
#define PORTD_DIRECTION CONCBIT(DIO_PIN31_DIR,DIO_PIN30_DIR,DIO_PIN29_DIR,DIO_PIN28_DIR,DIO_PIN27_DIR,DIO_PIN26_DIR,DIO_PIN25_DIR,DIO_PIN24_DIR);
// repeat this for other ports

void DIO_voidInt(void){//defines the direction of all ports according to config file
    DDRA = PORTA_DIRECTION; // repeat this for other ports
    DDRB = PORTB_DIRECTION;
    DDRC = PORTC_DIRECTION;
    DDRD = PORTD_DIRECTION;
}

void DIO_voidSetPortDir(u8 Local_PortNumber, u8 Local_Dir)
{
    if(Local_PortNumber == 0)
	{
	    DDRA = Local_Dir;

    }else if(Local_PortNumber == 1)
    {
        DDRB = Local_Dir;
    }else if(Local_PortNumber == 2)
    {
        DDRC = Local_Dir;
    }else if(Local_PortNumber == 3)
    {
        DDRD = Local_Dir;
    }
}
void DIO_voidSetPort(u8 Local_PortNumber,u8 Local_Value)
{
    if(Local_PortNumber == 0)
	{
	    PORTA = Local_Value;

    }else if(Local_PortNumber == 1)
    {
        PORTB = Local_Value;
    }else if(Local_PortNumber == 2)
    {
        PORTC = Local_Value;
    }else if(Local_PortNumber == 3)
    {
        PORTD = Local_Value;
    }


}

void DIO_voidSetPin(u8 local_pinNumber,u8 local_value){
    //portA
    if(local_pinNumber<8){
        if(local_value == DIO_HIGH){
            SETBIT(PORTA,local_pinNumber);
        }
        else if (local_value == DIO_LOW){
            CLRBIT(PORTA,local_pinNumber);
        }
    }
    //portB
    else if(local_pinNumber<16){
        if(local_value == DIO_HIGH){
            SETBIT(PORTB,local_pinNumber-8);
        }
        else if (local_value == DIO_LOW){
            CLRBIT(PORTB,local_pinNumber-8);//to be from 0 to 8
        }
    }
    //portC
    else if(local_pinNumber<24){
        if(local_value == DIO_HIGH){
            SETBIT(PORTC,local_pinNumber-16);
        }
        else if (local_value == DIO_LOW){
            CLRBIT(PORTC,local_pinNumber-16);
        }
    }
    //portD
    else if(local_pinNumber<32){
        if(local_value == DIO_HIGH){
            SETBIT(PORTD,local_pinNumber-24);
        }
        else if (local_value == DIO_LOW){
           CLRBIT(PORTD,local_pinNumber-24);
        }
    }
}

u8 DIO_u8GetPin(u8 local_pinNumber){
    //portA
    if(local_pinNumber<8){
       if(GETBIT(PINA,local_pinNumber) == 1)
                return DIO_HIGH;
            else if(GETBIT(PINA,local_pinNumber) == 0)
                return DIO_LOW;
    }
    //portB
    else if(local_pinNumber<16){
            if(GETBIT(PINB,local_pinNumber-8) == 1)
                return DIO_HIGH;
            else if(GETBIT(PINB,local_pinNumber-8) == 0)
                return DIO_LOW;
    }
    //portC
    else if(local_pinNumber<24){
        if(GETBIT(PINC,local_pinNumber-16) == 1)
                return DIO_HIGH;
            else if(GETBIT(PINC,local_pinNumber-16) == 0)
                return DIO_LOW;
    }
    //portD
    else if(local_pinNumber<32){
        if(GETBIT(PIND,local_pinNumber-24) == 1)
                return DIO_HIGH;
            else if(GETBIT(PIND,local_pinNumber-24) == 0)
                return DIO_LOW;
    }
}

u8 DIO_u8GetPinInPort(u8 Local_PortNumber, u8 Local_PinNumber)
{
    u8 ret=DIO_LOW;

    if(Local_PortNumber == 0)
	{
	    ret = GETBIT(PINA, Local_PinNumber);

    }else if(Local_PortNumber == 1)
    {
	    ret = GETBIT(PINB, Local_PinNumber);

    }else if(Local_PortNumber == 2)
    {
	    ret = GETBIT(PINC, Local_PinNumber);

    }else if(Local_PortNumber == 3)
    {
	    ret = GETBIT(PIND, Local_PinNumber);
    }

    return ret;

}
