// #define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "spi_core.h"


/**
 * Test adxl362 accelerometer using SPI
 */

void gsensor_check(SpiCore *spi_p, GpoCore *led_p) {
   const uint8_t RD_CMD = 0x0b;
   const uint8_t PART_ID_REG = 0x02;
   const uint8_t DATA_REG = 0x08;
   const float raw_max = 127.0 / 2.0;  //128 max 8-bit reading for +/-2g

   int8_t xraw, yraw, zraw;
   float x, y, z;
   int id;

   spi_p->set_freq(400000);
   spi_p->set_mode(0, 0);
   // check part id
   spi_p->assert_ss(0);    // activate
   spi_p->transfer(RD_CMD);  // for read operation
   spi_p->transfer(PART_ID_REG);  // part id address
   id = (int) spi_p->transfer(0x00);
   spi_p->deassert_ss(0);
   // read 8-bit x/y/z g values once
   spi_p->assert_ss(0);    // activate
   spi_p->transfer(RD_CMD);  // for read operation
   spi_p->transfer(DATA_REG);  //
   xraw = spi_p->transfer(0x00);
   yraw = spi_p->transfer(0x00);
   zraw = spi_p->transfer(0x00);
   spi_p->deassert_ss(0);
   x = (float) xraw / raw_max;
   y = (float) yraw / raw_max;
   z = (float) zraw / raw_max;
   uart.disp("x/y/z axis g values: ");
   uart.disp(xraw, 3);
   uart.disp(" / ");
   uart.disp(y, 3);
   uart.disp(" / ");
   uart.disp(z, 3);
   uart.disp("\n\r");

    if(x<1.1 && x>1){
	   led_p->write(1,3);
   }
   if(x<0.1 && x>0){
	   led_p->write(1,2);
   }
   if(x<-1 && x>-1.1){
	   led_p->write(1,1);
   }
   if(x<0 && x>-0.1){
	   led_p->write(1,0);
   }

   //0 
}
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
SpiCore spi(get_slot_addr(BRIDGE_BASE, S9_SPI));



int main() {

   while (1) {
      gsensor_check(&spi, &led);
      sleep_ms(100);
   } //while
} //main