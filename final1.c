// LCD module connections
sbit LCD_RS at RC0_bit;
sbit LCD_EN at RC1_bit;
sbit LCD_D4 at RC2_bit;
sbit LCD_D5 at RC3_bit;
sbit LCD_D6 at RC4_bit;
sbit LCD_D7 at RC5_bit;

sbit LCD_RS_Direction at TRISC0_bit;
sbit LCD_EN_Direction at TRISC1_bit;
sbit LCD_D4_Direction at TRISC2_bit;
sbit LCD_D5_Direction at TRISC3_bit;
sbit LCD_D6_Direction at TRISC4_bit;
sbit LCD_D7_Direction at TRISC5_bit;
// End LCD module connections

#define ENTER 15
#define CLEAR 13

// Keypad module connections
char  keypadPort at PORTD;

int Bruit;
int Gaz;
int limite_gazmax,limite_gazmin;
int limite_bruitmax,limite_bruitmin;
char x [4];
char j[4];
int flag ;
char test=0;
unsigned int temperature = 0;
float mV;
unsigned char txt[15];
unsigned short kp;

void tester_gaz()
{
	if (Gaz>limite_gazmax)
	{
	PORTB.RB3 = 0;
	PORTB.RB4 = 1;
	PORTB.RB5=1;
	Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    Lcd_Out(1,1,"millieu ");
    Lcd_Out(1,1,"pollue ");
	delay_ms(500);
	}

	if (Gaz>limite_gazmin && Gaz<limite_gazmax)
	{
	PORTB.RB3 = 0;
	PORTB.RB4 = 1;
	PORTB.RB5=0;
	Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    Lcd_Out(1,1,"activez ");
    Lcd_Out(1,1,"ventilateur ");
	delay_ms(500);
	}
	if (Gaz<limite_gazmin)
	{
	PORTB.RB3 = 0;
	PORTB.RB4 = 0;
	PORTB.RB5=0;
	}
	
}

void tester_bruit()
{
		if (Gaz<limite_gazmin && Bruit<limite_bruitmin)
		{
						PORTB.RB3=1;
			PORTB.RB2=0;
			PORTB.RB4=0;
			Lcd_Out(1,1,"Bienvenue     ");
            Lcd_Out(2,1,"millieu propre");
            delay_ms(500);
            Lcd_Cmd(_LCD_CLEAR);
            Lcd_Cmd(_LCD_CURSOR_OFF);
            Lcd_Out(1,1,"bruit");
            Lcd_Out(2,1,"acceptable");
            delay_ms(500);
		}
		if (bruit>limite_bruitmin && bruit<limite_bruitmax)
		{
			PORTB.RB2=1;
			PORTB.RB3=0;
			Lcd_Cmd(_LCD_CLEAR);
            Lcd_Cmd(_LCD_CURSOR_OFF);
            Lcd_Out(1,1,"  pollution ");
            Lcd_Out(1,1,"  sonore ");
			delay_ms(500);
		}
		if (bruit>limite_bruitmax)
		{
			PORTB.RB2=1;
			PORTB.RB3=0;
			Lcd_Cmd(_LCD_CLEAR);
            Lcd_Cmd(_LCD_CURSOR_OFF);
            Lcd_Out(1,1,"inaudible");
            Lcd_Out(2,1," millieu ");
            delay_ms(500);
		}
}

void interrupt(void)
{

    if(INTCON.INTF == 1 && INTCON.INTE == 1)    // si l'interruption est du au appui sur RB0
    {   

       INTCON.INTF = 0;
       flag=1;

    }

     if (INTCON.T0IF == 1)  // cliquer deux fois sur le button RA4
    {
    	INTCON.T0IF = 0;
    	
    	PORTB.RB5=1;
    	delay_ms(2000);
    	PORTB.RB5=0;
    	TMR0=254;
    }
}

void main(){
	TRISA = 0xff;
	TRISD = 0xff;
	TRISB.RB0=1;
	TRISB.RB1=1; // light
	TRISB.RB2=0; //LED Yellow
	TRISB.RB3=0; //LED Bleu
	TRISB.RB4=0; //LED Green
	TRISB.RB5=0; //Fan
	TRISB.RB7=0; 

	INTCON.GIE=1;       // activation interruption globale
    INTCON.INTE=1;     // activation interruption sur RB0
    INTCON.T0CS=1;   // en mode compteur
    TMR0=254;
	UART1_Init(9600);
	delay_ms(200);
	Sound_Init(&PORTB, 7);     // Initialize the pin Rb7 for playing sound
	Keypad_Init();   // Initialize Keypad
	ADC_Init();
	Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    PORTB.RB2 = 0;
	PORTB.RB3 = 0;
	PORTB.RB4 = 0;
	PORTB.RB5 = 0;

/*
	START:
       Lcd_Cmd(_LCD_CLEAR);                     // Clear display
       Lcd_Out(1, 1, "Enter limite_bmin");
       limite_bruitmin=0;
       Lcd_Out(2, 1, "limite_bmin: ");
        while(1)
        {
        //Setup new limite min
         do
          kp = Keypad_Key_Click();      // Store key code in kp variable
         while (!kp);
         if ( kp == ENTER )break;
         if (kp > 3 && kp < 8) kp = kp-1;
         if (kp > 8 && kp < 12) kp = kp-2;
         if (kp ==14)kp = 0;
         if ( kp == CLEAR )goto START;
         Lcd_Chr_Cp(kp + '0');
         limite_bruitmin =(10*limite_bruitmin) + kp;
    }
    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);          // Clear display
    Lcd_Cmd(_LCD_CURSOR_OFF);

      Lcd_Out(1, 1, "limite_bruitmin ");
      intToStr( limite_bruitmin,Txt);      //Convert to String
      test=Ltrim(Txt);
      Lcd_Out(1, 3,test);          //Display limite min
      EEPROM_Write(0x02,limite_bruitmin); // Write limite_bruitmin at address 2 of EEPROM
      Lcd_Out(2, 1, "Press # to Cont.");
      //Wait until # is pressed
      kp =0;
      while(kp!=ENTER)
      {
         do
           kp = Keypad_Key_Click();  // Store key code in kp variable
         while(!kp);
      }

    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
*/

	do
	{
		Bruit = ADC_Read(2);
		IntToStr(Bruit,x);
		UART1_Write_text("Bruit value =");
		UART1_Write_text(x);
		strcpy(x,"");
		Gaz = ADC_Read(1);
		IntToStr(Gaz,j);
		UART1_Write_text("Gaz value = ");
		UART1_Write_text(j);
		strcpy(j,"");
		UART1_Write(13);
		delay_ms(500);
		temperature = ADC_Read(0);          // Get 10-bit results of AD conversion
    mV = temperature * 5000.0 /1024.0;  // Convert to miV temperature in Celsuis
    mV = mV/10.0;                        // Convert mV to temperature in Celsuis

		limite_gazmin=511;
		limite_gazmax=800;
		if (flag==1)
		{
  			Lcd_Out(1, 1, "Temperature :");
  			Lcd_Chr(2,8,223);                  // char code for degree
  			Lcd_Chr(2,9,'C');                  // Display "C" for Celsius
			floatToStr(mV, txt);               // Convert temperature to string
            txt[4]=0;
            Lcd_Out(2, 3, txt);                // Display Temperature
            Delay_ms(500);   
            Lcd_Cmd(_LCD_CLEAR);
   			Lcd_Cmd(_LCD_CURSOR_OFF);
            flag==0;     
		}
		if (PORTB.RB1==1) //mode jour
		{
			limite_bruitmin=511;
			limite_bruitmax=800;
			tester_bruit();
			tester_gaz();
		}
		if (PORTB.RB1==0) //mode nuit
		{
			limite_bruitmin=306;
			limite_bruitmax=511;
			tester_bruit();
			tester_gaz();
		}
		if (mv >35 && Gaz>900)
		{
			Sound_Play(880, 300); // Play sound at 880Hz for 300ms
			PORTB=0xff;
			delay_ms(1000);
			PORTB=0x00;
		}
		
	} while (1);
}