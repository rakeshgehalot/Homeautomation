#define F_CPU 1000000
#define baud_rate 1200
#define prescale ((F_CPU/(16*baud_rate)) -1)

#include<avr/io.h>
#include<util/delay.h>

unsigned char s0[]="ATD7696060136;\r",s1[]="ATH\r", s6[]="AT+CMGS=\"887210****\"\r", s7[]="NAMASTEY\r",j,s4[]="AT+CMGR=1\r",msg[100];
unsigned int k=0,i,x=0,y=0;

void lcd_showvalue(unsigned char num)	//prints the decimal 3digit value of num
{
	unsigned char H=0,T=0,O=0;
	H=num/100;
	T=(num - (H*100))/10;
	O=(num - (H*100) - (T*10));
	
	lcd_char(H+0x30);
	lcd_char(T+0x30);
	lcd_char(O+0x30);
}


void lcd_cmd(unsigned int cmd)
{
_delay_ms(5);
PORTB=cmd;
PORTB &= ~(1<<PB0);
PORTB|=(1<<PB2);
PORTB &=~(1<<PB1);
//PORTB=cmd & 0xF2;
_delay_ms(1);
PORTB &= ~(1<<PB0);
PORTB &= ~(1<<PB2);
//PORTB=cmd & 0xF0;
}
void lcd_data(unsigned int data)
{
_delay_ms(5);
PORTB=data;
//PORTB&=~(0<<PB0);
//PORTB|=(1<<PB1);
PORTB|=(1<<PB0)|(1<<PB2);
PORTB &=~(1<<PB1);
//PORTB|=(1 | PB0)|(1<<PB1);
_delay_ms(1);
//PORTB|=(0<<PB1);
//PORTB|=(1<<PB0)|(0<<PB1);
PORTB|=(1<<PB0);
PORTB &= ~(1<<PB2);
}
void bit_cmd(unsigned int cmd)
{
unsigned int a,b;
a=cmd & 0xF0;
lcd_cmd(a);
b=(cmd<<4) & 0xF0;
lcd_cmd(b);
}
void lcd_char(unsigned int data)
{
unsigned int a,b;
a=data & 0xF0;
lcd_data(a);
b=(data<<4) & 0xF0;
lcd_data(b);
}

void USART_init(void)
{
UCSRB=0b00011000;
UCSRC=0b10100110;
UBRRL=prescale;
UBRRH=(prescale>>8);
}
void UART0_Tx_Char(unsigned int data)
{
while((UCSRA & (1<<UDRE))==0);
UDR=data;
}

unsigned int UART0_Rx_Char(void)
{
while((UCSRA & (1<<RXC))==0);
return(UDR);
}

void main()
{
	DDRB=0b11111111;
	/*bit_cmd(0x02);
	bit_cmd(0x28);
	bit_cmd(0x0E);
	bit_cmd(0x01);
	bit_cmd(0x06);
	bit_cmd(0x86);
	bit_cmd(0x80);*/
	USART_init();
	
	while(1)
	{
		if(x==1)
		{
			PORTB=0b00001000;
		}
		else if(y==1)
		{
			PORTB=0b00000000;
		}
		while(UDR==0xBA)
		{
			
			for(i=0;i<10;i++)
			{
				msg[i]=UART0_Rx_Char();
			}
			for(i=0;s1[i]!='\0';i++)
			{
				UART0_Tx_Char(s1[i]);
			}
			//lcd_char('A');
			/*for(i=0;i<5;i++)
			{
				lcd_showvalue(msg[i]);
			}
			for(i=0;i<20;i++)
			{
				msg[i]=UART0_Rx_Char();
			}*/
			if(msg[0]==0x33)
			{
				x=1;
				y=0;
				//lcd_char('A');
			}
			else if(msg[0]==0x36)
			{
				y=1;
				x=0;
			//lcd_char('B');
			
			}
		}
	}
	
}
