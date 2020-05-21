#INCLUDE <16F877a.H>  //PIC utilizado
#DEVICE ADC = 10
#INCLUDE <MATH.H>    //Libreria con funciones matematicas
#FUSES   NOWDT       //No Watch Dog Timer
#FUSES   XT          //
#FUSES   NOPUT       //No Power Up Timer
#FUSES   NOPROTECT   //Code not protected from reading
#FUSES   NOBROWNOUT  //No brownout reset
#FUSES   NOLVP       //No low voltage prgming, B3(PIC16) or B5(PIC18) used FOR I/O
#FUSES   NOCPD       //No EE protection
#use  delay(clock=4000000)
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7, parity=n, bits=8, stop=1)

//Puertos y Registros//
#byte porta = 0x05
#byte portb = 0x06
#byte portc = 0x07

/* Variables */
float vd;   // Voltaje leido por el ADC
float va;   // Voltaje de la temperatura ambiente
float vt;   // Voltaje en la termocupla
float temp; // Temperatura
float a0,a1,a2,a3,a4,a5,a6,a7,a8;

/* Funciones */
void lectura_adc(void);
void tabla_j(void);
void tabla_k(void);

void main()
{
   float vt_mv;
   set_tris_a (0xff);
   set_tris_b (0x00);
   set_tris_c (0xfe);
   portb = 0x00;
   output_high(pin_c0);
   setup_adc (adc_clock_internal);
   setup_adc_ports (an0);
   for(;;)
   {
      lectura_adc(); 
      vt = vd/101;
      vt_mv = vt*1000;      
      if(input(pin_a1)==1)
      {
         tabla_k();
         printf("Voltaje de la termocupla K: %fmV\r\n",vt_mv);
      }
      else
      {
         tabla_j();
         printf("Voltaje de la termocupla J: %fmV\r\n",vt_mv);
      }
      temp = temp + 15;
      if(temp<254)
      {
         portb = temp;
         printf("Temperatura de la temocupla: %fC\r\n",temp);
         delay_ms(1000);
      }
      else
      {
         portb=0xFF;
      }
   }   
}

void lectura_adc(void)
{
   int16 i;
   float vd_mv;
   set_adc_channel(0);
   delay_us(10);
   i = read_adc(adc_start_and_read);   
   printf("Lectura ADC: %ld\r\n",i);
   vd = 5.0*i/1024.0;
   vd_mv = vd*1000;
   printf("Voltaje de la LM324: %fmV\r\n",vd_mv);
}

void tabla_j(void)
{
   a0 = -0.048868252;
   a1 = 19873.14503;       
   a2 = -218614.5353;
   a3 = 11569199.78;      
   a4 = -264917531.4;      
   a5 = 2018441314;   
   temp =a0+vt*(a1+vt*(a2+vt*(a3+vt*(a4+a5*vt))));  
}

void tabla_k(void)
{
   a0 = 0.226584602;
   a1 = 24152.10900;       
   a2 = 67233.4248;
   a3 = 2210340.682;      
   a4 = -860963914.9;      
   a5 = 4.83506E+10;
   a6 = -1.18452E+12;
   a7 = 1.38690E+13;
   a8 = -6.33708E+13;
   temp =a0+vt*(a1+vt*(a2+vt*(a3+vt*(a4+vt*(a5+vt*(a6+vt*(a7+a8*vt)))))));
}
