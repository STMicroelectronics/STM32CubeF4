"ssl_server.exe" PC utility is a part of mbedTLS-2.4.0 examples, it’s built using  MinGW.
more details can be found in the link below:
https://tls.mbed.org/kb/compiling-and-building/compiling-mbedtls-in-mingw

This utility is intended to be used with the following applications
   - Projects\STM324xG_EVAL\Applications\mbedTLS\SSL_Client
   - Projects\STM32F4x9I_EVAL\Applications\mbedTLS\SSL_Client
   - Projects\STM32F469I_EVAL\Applications\mbedTLS\SSL_Client

How to use:
===========
- open a Windows command console
- get your machine ip address by typing the command "ipconfig",
  then check the "IPv4 Address" field.

- define the enviroment variable "MY_IP_ADDRESS" to contain your machine IP address
  retrieved by the command "ipconfig", follow the steps in the link below:
  https://www.microsoft.com/resources/documentation/windows/xp/all/proddocs/en-us/sysdm_advancd_environmnt_addchange_variable.mspx?mfr=true
- run the ssl_server.exe you should see messages similar to:
  
  . Loading the server cert. and key... ok
  . Bind on https://<your_ip_address>:4433/ ... ok
  . Seeding the random number generator... ok
  . Setting up the SSL data.... ok
  . Waiting for a remote connection ...


  

  
 