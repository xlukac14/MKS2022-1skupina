/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#include "lwip/opt.h"

#if LWIP_NETCONN

#include "lwip/sys.h"
#include "lwip/api.h"

#include <string.h>

#define telnet_THREAD_PRIO  ( tskIDLE_PRIORITY + 4 )


static void http_client(char *s, uint16_t size) {
	struct netconn *client;
	struct netbuf *buf;
	ip_addr_t ip;
	uint16_t len = 0;
	IP_ADDR4(&ip, 147, 229, 144, 124);
	const char *request = "GET /ip.php HTTP/1.1\r\n"
			"Host: www.urel.feec.vutbr.cz\r\n"
			"Connection: close\r\n"
			"\r\n\r\n";
	client = netconn_new(NETCONN_TCP);
	if (netconn_connect(client, &ip, 80) == ERR_OK) {
		netconn_write(client, request, strlen(request), NETCONN_COPY);
		// Receive the HTTP response
		s[0] = 0;
		while (len < size && netconn_recv(client, &buf) == ERR_OK) {
			len += netbuf_copy(buf, &s[len], size - len);
			s[len] = 0;
			netbuf_delete(buf);
		}
	} else {
		sprintf(s, "Chyba pripojeni\n");
	}
	netconn_delete(client);
}

void telnet_process_command(char* cmd, struct netconn *conn) {

	char s[256];
    if (strlen(cmd) == 0) return;
	/*
	char *token;  //token bude ukazovat na 1. slovo prikazu strtok...
	token = strtok(cmd, " ");  //strtok - "HELLO_WORLD\n\0" - ulozi string ukonceny \0, " " je vypis deliacich znakov, orezanie stringu vlozenim \0?


	//--------------READ-------------------
	if (strcasecmp(token, "READ") == 0) {
		token = strtok(NULL, " ");
		uint16_t addr = atoi(token);
		uint8_t value;

		HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, addr, I2C_MEMADD_SIZE_16BIT, &value, 1, 1000);
		ssprintf("Address 0x%04x = 0x%02x\n", addr, value);

	}
	//--------------WRITE------------------
	else if (strcasecmp(token, "WRITE") == 0) {
		token = strtok(NULL, " ");
		uint8_t value = atoi(token);

		token = strtok(NULL, " ");
		uint8_t addr = atoi(token);

		HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR, addr, I2C_MEMADD_SIZE_16BIT, &value, 1, 1000);

		while (HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDR, 300, 1000) == HAL_TIMEOUT) {}

		ssprintf("WRITTEN\n");
*/
	//sprintf("Received: %s\n", cmd);  //print - vypis, f - format, %s - string

		char *token;  //token bude ukazovat na 1. slovo prikazu strtok...
		token = strtok(cmd, " ");  //strtok - "HELLO_WORLD\n\0" - ulozi string ukonceny \0, " " je vypis deliacich znakov, orezanie stringu vlozenim \0?

		//-------------HELLO-------------------
		if (strcasecmp(token, "HELLO") == 0) {
			sprintf(s,"Hello World!\r\n");
		}
		//-------------LED1--------------------
		else if (strcasecmp(token, "LED1") == 0) {
			token = strtok(NULL, " ");
			if (strcasecmp(token, "ON") == 0) {
				HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, 1);
			}
			else if (strcasecmp(token, "OFF") == 0) {
				HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, 0);
			}
			sprintf(s,"OK\n");
		}
		//-------------LED2--------------------
		else if (strcasecmp(token, "LED2") == 0) {
			token = strtok(NULL, " ");
			if (strcasecmp(token, "ON") == 0) {
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
			}
			else if (strcasecmp(token, "OFF") == 0) {
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
				sprintf(s,"OK\n");
			}
		}

		//-------------STATUS-------------------
		else if (strcasecmp(token, "STATUS") == 0) {
			//token = strtok(NULL, " ");
			if (HAL_GPIO_ReadPin(LD1_GPIO_Port, LD1_Pin) == 0) {
				sprintf(s,"LED1 is OFF\n");
			}
			else {
				sprintf(s,"LED1 is ON\n");
			}
			if (HAL_GPIO_ReadPin(LD2_GPIO_Port, LD2_Pin) == 0) {
				sprintf(s,"LED2 is OFF\n");
			}
			else {
				sprintf(s,"LED2 is ON\n");
			}

	} else if (strcasecmp(token, "CLIENT") == 0) {
		char sbuf[1024];
		http_client(sbuf, 1024);
		sprintf(s, "%s\r\n", sbuf);
	}
	// %04x -> vypis "0001", kde 0 vypisany znak, 4 je pocet-1

/*	//--------------DUMP-------------------
	else if (strcasecmp(token, "DUMP") == 0) {
		uint8_t value;
		for(int addr = 0; addr < 16; addr++)
		{
			HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, addr, I2C_MEMADD_SIZE_16BIT, &value, 1, 1000);
			ssprintf("%02x", value);
			if((addr % 8 ) == 7) ssprintf("\n");
			else ssprintf(" ");
		}
	}
*/
	else {
		sprintf("Unknown command: %s\n", s);
	}
	netconn_write(conn, s, strlen(s), NETCONN_COPY);
}


static void telnet_byte_available(uint8_t c, struct netconn *conn) {
	static uint16_t cnt;
	static char data[256];
	if (cnt < 256 && c >= 32 && c <= 127)
		data[cnt++] = c;
	if (c == '\n' || c == '\r') {
		data[cnt] = '\0';
		telnet_process_command(data, conn);
		cnt = 0;
	}
}

/*-----------------------------------------------------------------------------------*/
static void telnet_thread(void *arg)
{
  struct netconn *conn, *newconn;
  err_t err, accept_err;
  struct netbuf *buf;
  uint8_t *data;
  u16_t len;
      
  LWIP_UNUSED_ARG(arg);

  /* Create a new connection identifier. */
  conn = netconn_new(NETCONN_TCP);
  
  if (conn!=NULL)
  {  
    /* Bind connection to well known port number 7. */
    err = netconn_bind(conn, NULL, 23);
    
    if (err == ERR_OK)
    {
      /* Tell connection to go into listening mode. */
      netconn_listen(conn);
    
      while (1) 
      {
        /* Grab new connection. */
         accept_err = netconn_accept(conn, &newconn);
    
        /* Process the new connection. */
        if (accept_err == ERR_OK) 
        {

          while (netconn_recv(newconn, &buf) == ERR_OK) 
          {
            do 
            {
            	/*
              netbuf_data(buf, &data, &len);
              netconn_write(newconn, data, len, NETCONN_COPY);
              */
            	netbuf_data(buf, (void**)&data, &len);
            	while (len--) telnet_byte_available(*data++, newconn);
          
            } 
            while (netbuf_next(buf) >= 0);
          
            netbuf_delete(buf);
          }
        
          /* Close connection and discard connection identifier. */
          netconn_close(newconn);
          netconn_delete(newconn);
        }
      }
    }
    else
    {
      netconn_delete(newconn);
    }
  }
}
/*-----------------------------------------------------------------------------------*/

void telnet_init(void)
{
  sys_thread_new("telnet_thread", telnet_thread, NULL, DEFAULT_THREAD_STACKSIZE, telnet_THREAD_PRIO);
}
/*-----------------------------------------------------------------------------------*/

#endif /* LWIP_NETCONN */
