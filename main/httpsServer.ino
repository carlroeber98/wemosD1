#include <ESP8266WebServerSecure.h>
#include <ESP8266mDNS.h>

ESP8266WebServer server(80);
//BearSSL::ESP8266WebServerSecure server(443);

static const char serverCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDKDCCAhACCQCokIZJhcTPKDANBgkqhkiG9w0BAQsFADBWMQswCQYDVQQGEwJE
RTEUMBIGA1UECAwLRGV1dHNjaGxhbmQxDzANBgNVBAcMBkJlcmxpbjEPMA0GA1UE
CgwGU21hcnRDMQ8wDQYDVQQDDAZTbWFydEMwHhcNMTkwNTIwMTQ0NDA3WhcNMjEw
NTA5MTQ0NDA3WjBWMQswCQYDVQQGEwJERTEUMBIGA1UECAwLRGV1dHNjaGxhbmQx
DzANBgNVBAcMBkJlcmxpbjEPMA0GA1UECgwGU21hcnRDMQ8wDQYDVQQDDAZTbWFy
dEMwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC2THurWIVBwnmphzaN
AUktog3N87gvd9QZMw02H544K+y0+vG0mfFxc40tkJYLu2TolnL3dC6iaZhGESz5
3RwXu9rGLbOgzrQB7M6s2ofXx1Acvk2ZYJ4ncXP+hczGFituc35U/t7I3lX68Pfp
/JcBCIiD8uiAM3Od3JlOeHkvIHHwr/Iaa5DKn4ZB7yPhZ2VdkkCq+MZiwzqkDxco
tsAjEMnJSEcvc5oV311ORTlTWYefwphl8AlVlLc0/sYTEhTcR95wCaZSYLl9gY+E
stf9G4Fm3Wfi8+b3GIsY1V7GHp6dRZ0ZflkFTShV+myeRFqEa78uqEcD0MgqcLJI
QALnAgMBAAEwDQYJKoZIhvcNAQELBQADggEBAG/EMJUfXVW4db7/9104dgyi/sQw
TUTKNF2kMwhP2WoZUkq1M30cW075V3WOe+RzdrxSwhLQDdMQ6T1guAHFlVdcQ7Xg
DpNqKBdVr1BcFMMqUVfR7ZrujJrYy065MWJsA56XSpLCXE7HL3h/vVcwXK4lXXLY
fc7FtSGtmdYmW70hRZ9Gvsq6LXarMVuB6lEHucP8YlKfXBKUYiangnqh/spPaZSJ
oUbsVsDCMXKpoHnpPEhr2tT46NnKrgYofJHEe33cCWW5yxIHpeY+rWgHLU0bJNGY
rog+hSc2Vl89jW+N6x6hG9BLjlyzSPEodIDr5lx2vQ7Y+Wx1tmR5qrwcpao=
-----END CERTIFICATE-----
)EOF";

static const char serverKey[] PROGMEM =  R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAtkx7q1iFQcJ5qYc2jQFJLaINzfO4L3fUGTMNNh+eOCvstPrx
tJnxcXONLZCWC7tk6JZy93QuommYRhEs+d0cF7vaxi2zoM60AezOrNqH18dQHL5N
mWCeJ3Fz/oXMxhYrbnN+VP7eyN5V+vD36fyXAQiIg/LogDNzndyZTnh5LyBx8K/y
GmuQyp+GQe8j4WdlXZJAqvjGYsM6pA8XKLbAIxDJyUhHL3OaFd9dTkU5U1mHn8KY
ZfAJVZS3NP7GExIU3EfecAmmUmC5fYGPhLLX/RuBZt1n4vPm9xiLGNVexh6enUWd
GX5ZBU0oVfpsnkRahGu/LqhHA9DIKnCySEAC5wIDAQABAoIBAHm8EUmWz0XejEf9
FxyKI0fp07YYboKkTmqVgNkHvCxeiK9CdG3Z7zWHqFspFkm3VedLsHI2j329bgY8
KgCg+sWOmMWGd1GHNwWiEVG0jMo3OvQ20LL/ph1I0eDBkMEmxNH5Jf+0Qyl374qn
p6YFGwGBpUgXydLyiN5+FZIlOSeSLPiHG2zcRE2k+ONWKgLTG8P0CFTPFQX4XMtU
CHZM0wCkeDnhUTTACza5Hw0XKL26Ir26kfSU8M1O1KU6lEH8lJMD6X/Y2a26XxcS
W6uEzOYVWy95pTs7Go7t/PJNUttoILa1mj5Q611vNS1qyQmV1fMCICY5K+Oq6mAe
DsCrlUECgYEA72baMAHIpfoSvgSyvYU0bsDnhJSihPUm+yNrrUdz9g8+qZunsnDD
PbJo9pvI+zQqx0WnNL26IcTvR5pTJ8aTDLFhHJGNJb4dwEWn8oZC8nrWT+L2rm+w
G7ITSV/TYildwv/GipLgwjdK36Vy+jrlCmYzXwMnP/AwH752QJYBibkCgYEAwvAb
sh2NRF8zksW/KiaWUgjKk/pPLbh//X92rWq9yUX3Xt8p+xOd/lW5zRMtYULBJr5Q
XV4vXkxBX6ttlLnC0zJE3KlV0OngHHiJpT4T2G+P2Yh1hQh5LaWy5UxJTDtyMTxz
TCf5gskUiu0hdocJrZzH4cVK9yM3wG8ccsevwZ8CgYAPbSI8G7FROKcChYz6911k
kZuZiHZceOuVHTgwBz2qqlrQVQ5eSjPagueOlIVD4p2rD4rxtBQNXwJIK+KV2o7Z
L4jhbWOZ/gEuxY5H62ze8RlkNmzM2rePYU8q2Djnx2CaUx75CPPgr+1sMd4Rz7+4
htNimPJrhU+kjAJUsRXSSQKBgQCpJjWyCUiCUmVq7yHgjlkTV8OIQcOgtZ6Bta+h
7KQ/2PE8JkLENZJ7laFCnGhdI9J8LtmHGunZVivHuvW7/rBaGB1+p4TqYXXw3nr5
onMP9EYLCQiIZgDYJhakNtzHRDHjHUQiifFMlfS4U+kcczJnrkoW0t4osit3VDKI
OlN8mwKBgE0zgh5d4fE+GO5t1o+3Q0wvAreUS6VpgSfNEZN/XoXcKpfUZpCifaxI
G3uzRZ2UScfurHCaU9/IN6KuC/XKflsZEQ77UfF4mn7xOaeKjmun/8Qk8X2fogcw
2OWhxxgY2P2r7tm2PqrnrD4vnqcjTJNjTmTuy4rOKUmNjeDpkPeo
-----END RSA PRIVATE KEY-----
)EOF";

const char* user = "admin";
const char* pswd = "esp8266";

void initHttpServer(boolean secure){

  if (secure) {
    //configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    //Serial.println(millis());
    //server.setRSACert(new BearSSL::X509List(serverCert), new BearSSL::PrivateKey(serverKey));
  }

  server.on("/status", [](){
    server.send(200, "text/plain", "Status: OK");
  });
  
  server.on("/send", [](){
    if (!server.authenticate(user, pswd)){
      return server.requestAuthentication(DIGEST_AUTH, "SmartC-Wemos Authentification", "Authentication Failed");
    }
    if(server.args() == 2){
      getAndSendHexCode(server.arg("type").c_str(), strdup(server.arg("code").c_str()));
      server.send(202, "text/plain", "Parameters successful transmitted and parsed");
    }else{
      server.send(404, "text/plain", "Parameters have a wrong format");
    }
  });

  server.on("/off", [](){
    if (!server.authenticate(user, pswd)){
      return server.requestAuthentication(DIGEST_AUTH, "SmartC-Wemos Authentification", "Authentication Failed");
    }
    server.send(200, "text/plain", "OFF");
  });

  server.on("/getactiontime", [](){
    if (!server.authenticate(user, pswd)){
      return server.requestAuthentication(DIGEST_AUTH, "SmartC-Wemos Authentification", "Authentication Failed");
    }
    server.send(200, "text/plain", getActionTime());
  });
  
   server.on("/setaction", [](){
      if (!server.authenticate(user, pswd)){
      return server.requestAuthentication(DIGEST_AUTH, "SmartC-Wemos Authentification", "Authentication Failed");
    }
    if(server.args() == 2){
      //setAction(strdup(server.arg("type").c_str()), strdup(server.arg("time").c_str()));
      server.send(202, "text/plain", "Parameters successful transmitted and parsed");
    }else{
       server.send(404, "text/plain", "Code ");
    }
  });

  server.onNotFound([]() {
    server.send(404, "text/plain", "File Not Found\n\n");
  });

  server.begin();
  //Serial.println("HTTPS server started");
  Serial.println("HTTP server started");
}

void serverLoop(){
  server.handleClient();
}
