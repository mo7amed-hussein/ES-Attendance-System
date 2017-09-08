#ifndef _WIFI_
#define _WIFI_

#define ESP_BAUDE_RATE 9600 //wifi baudrate
#define RESPONSE_DELAY  1000 //delay for wifi response
#define WIFI_BUFFER_SIZE 100 // wifi buffer size
#define API_KEY "JTHW9D3JUUKW808Y"
//wifi errors constants
typedef enum{NOERROR,HARDWAREERROR,ECHOERROR,
	MODEERROR,CONNECTERROR,STARTCONNECTERROR,
	NUMOFBYTESERROR,UPDATEDATAERROR,PINGERROR} wifiError;

typedef enum{True ,False} boolean; //boolean type	
/*
	initialize wifi
	@return true if success else false
*/
boolean initWifi();

/*
	connect to specific network
	@param ssid network ssid
	@param pwd network password
	@return true if connection success else false
*/
boolean connectToNetwork(unsigned char *ssid ,unsigned char *pwd);

/*
	empty buffer
	@param buf pointer to buffer tobe flushed
*/
void flushBuffer(unsigned char *buf);

/*
	check hardware connection
	@return true if connection verfied else false
*/
boolean checkHW(void);

/*
	disable command echo
	@return true if done else false
*/

boolean disableEcho(void);


/*
	set client mode
	@return true if done else false
*/
boolean setClientMode(void);

/*
	return string of last occured error
	@return pointer to string
*/

unsigned char * getLastError(void);

/*
	establish connection with host
	@return true if connection success else false
*/

boolean connectHost(void);

/*
	signal module of sending data, send data length
	@param len data length
	@return true if success else false
*/
boolean initDataSend(unsigned short len);

/*
	update fields
	@param field new value
	@return true if success else false
*/

boolean updateData(unsigned short field);

/*
	check connectivity to internet
	@return true if connected else false
*/
boolean checkInternet(void);


#endif