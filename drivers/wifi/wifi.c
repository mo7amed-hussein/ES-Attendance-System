#include "wifi.h"
#include "../uart/usart0.h"
#include "../uart/softuart.h"
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
//wifi constants
unsigned char wifi_response_OK[]="OK";
unsigned char data_updated_res[]="+IPD";
unsigned char wifi_response_noAP[]="noAP";
unsigned char wifi_response_ALRY_CONN[]="ALREADY CONNECT";
unsigned char wifi_response_CONN[]="CONNECT";
unsigned char wifi_ping_timeout[]="+";
//wifi variables 
unsigned char wifiBuffer[WIFI_BUFFER_SIZE];
wifiError _wifiError= NOERROR ;
unsigned char wifiErrorStr[20];
boolean initWifi()
{
	flushBuffer(wifiBuffer);
	initUsart0(ESP_BAUDE_RATE);
	if(checkHW()==False)
	{
		return False;
	}
	
	disableEcho();
	
	if(setClientMode()==False)
	{
		return False;
	}
	return True;
}


boolean checkHW(void)
{
	flushBuffer(wifiBuffer);
	//send test AT command
	putsUsart0("AT\r\n");
	//wait for wifi response
	_delay_ms(RESPONSE_DELAY);
	if(strstr(wifiBuffer,wifi_response_OK))
	{
		_wifiError = NOERROR;
		return True;
	}
	_wifiError = HARDWAREERROR;
	return False;	
}

void flushBuffer(unsigned char *buf)
{
	memset(buf,'\0',WIFI_BUFFER_SIZE);
}

boolean disableEcho(void)
{
	flushBuffer(wifiBuffer);
	//send AT command
	putsUsart0("ATE0\r\n");
	//wait for wifi response
	_delay_ms(RESPONSE_DELAY);
	if(strstr(wifiBuffer,wifi_response_OK))
	{
		_wifiError = NOERROR;
		return True;
	}
	_wifiError = ECHOERROR;
	return False;
}

boolean setClientMode(void)
{
	flushBuffer(wifiBuffer);
	
	if(_wifiError != NOERROR)
	{
		return False;
	}
	//send AT command
	putsUsart0("AT+CWMODE=1\r\n");
	//wait for wifi response
	_delay_ms(RESPONSE_DELAY);
	if(strstr(wifiBuffer,wifi_response_OK))
	{
		_wifiError = NOERROR;
		return True;
	}
	_wifiError = MODEERROR;
	return False;
}

boolean connectToNetwork(unsigned char *ssid ,unsigned char *pwd)
{
	flushBuffer(wifiBuffer);
	
	if(_wifiError != NOERROR)
	{
		return False;
	}
	
	unsigned char command[100];
	sprintf(command,"AT+CWJAP=\"%s\",\"%s\"\r\n\0",ssid,pwd);
	//softuart_puts(command);
	putsUsart0(command);
	_delay_ms(RESPONSE_DELAY);
	_delay_ms(RESPONSE_DELAY);
	_delay_ms(RESPONSE_DELAY);
	_delay_ms(RESPONSE_DELAY);
	_delay_ms(RESPONSE_DELAY);
	_delay_ms(RESPONSE_DELAY);
	if(strstr(wifiBuffer,wifi_response_OK))
	{
		_wifiError = NOERROR;
		return True;
	}
	_wifiError = CONNECTERROR;
	return False;
}

unsigned char * getLastError(void)
{
	switch(_wifiError)
	{
		case NOERROR:
		strcpy(wifiErrorStr,"No Errors\n");
		break;
		case HARDWAREERROR:
		strcpy(wifiErrorStr,"Hardware Error\n");
		break;
		case ECHOERROR:
		strcpy(wifiErrorStr,"Echo Error\n");
		break;
		case MODEERROR:
		strcpy(wifiErrorStr,"Set Mode Error\n");
		break;
		case CONNECTERROR:
		strcpy(wifiErrorStr,"Network connection Error\n");
		break;
		case STARTCONNECTERROR:
		strcpy(wifiErrorStr,"Client Connect Error\n");
		break;
		case NUMOFBYTESERROR:
		strcpy(wifiErrorStr,"Error While Sending Data Length\n");
		break;
		case UPDATEDATAERROR:
		strcpy(wifiErrorStr,"Update Date Error\n");
		break;
		case PINGERROR:
		strcpy(wifiErrorStr,"Ping Error\n");
		break;
		default:
		strcpy(wifiErrorStr,"Undefined Error\n");
	}
}

boolean connectHost(void)
{
	flushBuffer(wifiBuffer);
	
	if(_wifiError != NOERROR)
	{
		return False;
	}
	
	putsUsart0("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
	_delay_ms(RESPONSE_DELAY);
	if(strstr(wifiBuffer,wifi_response_OK) || strstr(wifiBuffer,wifi_response_CONN))
	{
		_wifiError = NOERROR;
		return True;
	}
	_wifiError = STARTCONNECTERROR;
	return False;
}

boolean initDataSend(unsigned short len)
{
	flushBuffer(wifiBuffer);
	
	if(_wifiError != NOERROR)
	{
		return False;
	}
	unsigned char command[20];
	sprintf(command,"AT+CIPSEND=%d\r\n\0",len);
	putsUsart0(command);
	_delay_ms(RESPONSE_DELAY);
	if(strstr(wifiBuffer,wifi_response_OK))
	{
		_wifiError = NOERROR;
		return True;
	}
	_wifiError = NUMOFBYTESERROR;
	return False;
}

boolean updateData(unsigned short field)
{
	if(_wifiError != NOERROR)
	{
		return False;
	}
	unsigned char command[150];
	sprintf(command,"GET https://api.thingspeak.com/update?api_key=%s&field1=%d\\r\\nHost:api.thingspeak.com\r\n\r\n\0",API_KEY,field);
	//softuart_puts(command);
	//_delay_ms(500);
	//sprintf(tmp,"len of command is %d \n",strlen(command));
	//_delay_ms(500);
	//softuart_puts(tmp);
	//_delay_ms(500);
	//softuart_puts("connecting to host\n");
	//_delay_ms(RESPONSE_DELAY);
	if(connectHost()==False)
	{
		return False;
	}
	
	//softuart_puts("init send data\n");
	//_delay_ms(RESPONSE_DELAY);
	if(initDataSend(strlen(command))==False)
	{
		return False;
	}
	//softuart_puts("send data\n");
	//_delay_ms(RESPONSE_DELAY);
	flushBuffer(wifiBuffer);
	
	putsUsart0(command);
	_delay_ms(RESPONSE_DELAY);
	if(strstr(wifiBuffer,data_updated_res) || strstr(wifiBuffer,wifi_response_OK))
	{
		_wifiError = NOERROR;
		return True;
	}
	_wifiError = UPDATEDATAERROR;
	return False;
	
}

boolean checkInternet(void)
{
	flushBuffer(wifiBuffer);
	
	if(_wifiError != NOERROR)
	{
		return False;
	}
	
	putsUsart0("AT+PING=\"8.8.8.8\"\r\n");
	_delay_ms(RESPONSE_DELAY);
	if(strstr(wifiBuffer,wifi_ping_timeout))
	{
		//softuart_puts("host connected>>\n");
		_wifiError = NOERROR;
		return True;
	}
	_wifiError = PINGERROR;
	return False;
}