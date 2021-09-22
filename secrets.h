#include <pgmspace.h>

#define SECRET
#define THINGNAME ""                                                          //change this
const char* http_username = "admin";
const char* http_password = "admin";
const char WIFI_SSID[] = "ELDRADO";                                           //change this
const char WIFI_PASSWORD[] = "amazon1234";                                    //change this
const char AWS_IOT_ENDPOINT[] = "atv6zwxshlj9-ats.iot.us-west-2.amazonaws.com";       //change this

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5M
-----END CERTIFICATE-----
)EOF";

// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUHCjRu9W0iTZ1I50l6SrRcht+zm8wDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIxMDYyMzEzNTU1
OVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAJpBELi9vOsqQ1VKiqDM
vxLkFAA8cQQEYlumR9IyLeii064VzU7GrpmlGvdhQTIh2PVmwclZOIhINoIw/SCN
7ScNUagBTp4vjdeLVxT2oF7vDWzsf3O/M+pD9VWY9xXoeSqp0aO58h5eUg8TppoP
roNIIgbJmFciz/244+Ev+mSQH+jWcbtc7gPnECW4nvR9c+XCSRC1Ig+OX6f3/mjd
RhEQkRyDkCIHhraL+wvfhd3YCcYsos0r1TG85WKFX/glLU6RtVKaClrKycLkdCVD
v/zFRz/XEqzJw3JmSzvJopW26W4z7pxHhFZ91Icyxk2iePZ20VymMJZ1n+tABOx0
jgECAwEAAaNgMF4wHwYDVR0jBBgwFoAUVxewWVrxZcn/qw49torFdoM1CfgwHQYD
VR0OBBYEFO0sAdWKpCj9LbbV1kUaBuP90ljxMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCFUy6YXLbqeIHw7tgtKqORtJxq
Ff+ZC684EUIqLYCdSdYIsSB1Yd8jiVlFQ7TKEV3QOvU18o3/nE2djxsx6+s2z1Pz
51rpgRxcQKRBjfBYFyt9ci3XGrqe5xTP/Kn0t08jd234n6zaTxhEedV+A0+ANHk7
m9i72SICUpS7t/ZFZi/iApRf3gEPP60QCdFSrGZ8K7MJlUadCe9OTYvHivbzyiPC
xc4uPqijSV2LSAPmE04zVmhiKUc3SZaHhOrEiq6t8FnsQVWm4r27CasG/8JRiOQD
TgvhCdSWbpVN2E9ABwlH61CNzjDoOXWE1AZALkKgoamUonTvdSPfhKGxF1u2
-----END CERTIFICATE-----
)KEY";

// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEogIBAAKCAQEAmkEQuL286ypDVUqKoMy/EuQUADxxBARiW6ZH0jIt6KLTrhXN
TsaumaUa92FBMiHY9WbByVk4iEg2gjD9II3tJw1RqAFOni+N14tXFPagXu8NbOx/
c78z6kP1VZj3Feh5KqnRo7nyHl5SDxOmmg+ug0giBsmYVyLP/bjj4S/6ZJAf6NZx
u1zuA+cQJbie9H1z5cJJELUiD45fp/f+aN1GERCRHIOQIgeGtov7C9+F3dgJxiyi
zSvVMbzlYoVf+CUtTpG1UpoKWsrJwuR0JUO//MVHP9cSrMnDcmZLO8milbbpbjPu
nEeEVn3UhzLGTaJ49nbRXKYwlnWf60AE7HSOAQIDAQABAoIBADgoJeFL2iI3EapU
Z+GgiJNlC08GshJHUlORVqZUdFg0JyJCd6sNM/VloUqcUnLg1a6JiVU3MRiUSZve
H9khMkw8UhT63NdD8ryc0sgj/UBygQ29j6zg2BKaSQ+vj4yfRLVee57CjD24EuHA
Qq/ls5/Bu/mCnP5kAxqYnAWYRO0k5nZygyTGpToP1kqUmXCgvbdFFVvvSd9UxUHZ
bjpHPI19Au1OCYn/mMZmQKf+sy93IJDQcoajHz52ZrwwchwqSFt79IsIO93A4P9P
8TQntgzeRjFOr1nfp/htn3sMAxW5MnAUfvsmo6ClBMrzdZyf374BIaqgDpfSWVsa
h10ngRECgYEAyKsuGFi9nUw9RwYb0Su6pw/GY4gm61J+WsUUkx6VI0h/n48xwVwL
Xiadx0PABNvAaNrO5TJ0KOnx+1RW0CESI7gF/JHDWXDLhRQqUeiAdNvkQG38jBxh
ZzkjB+B37APvgoADsySZAEecQXsQrjJvC0l9ZTOpaUm7VSDWJHiZXv0CgYEAxMmS
vnH6kkdwySE2YLxdvn16D3nDVrIXBO7rcbAtQoDf/t08GmrbLYI3tfvpWWRv+gT9
ErYiuXjBjkyjNkXSeKHcvcvfBLaSiUEG+W5mPkdc6//+HqbSahBojMBdwt5X0nil
LLfLtriCxdTXHqPGKfY6M4VGzObkiHiME/rhVFUCgYBY9GlOqagEntNbNL4ID5aC
kF+lp6l5D+yrWEoynZAHQBAzL0sjKCZx6jRvLP1mKfQ1hynIOIF90ZdPjqedQ8vC
wcUFnag3ceUGbVYFUUgAham9VgRqJ/qVw6Jfz4XZvjTiaNU7CLI6iYQfEurmDb1k
B4Pq5OfooKLGrfO/3yY+EQKBgCLOpK4a6v9Es6ZfwYaKyuoOt7BVEUAccAXnXK3X
Q6onsXcbGq5xiCMcjU4bKMMvCL9ocnsEzE+SFGpfE99dqkS8HJdVoDTtNi4g2mL0
LBGH/d+ggApb54vrKxL6U5tMSKxpuo2WVOoU6MylQQeiYEK70r+aK1PC41lgOSav
kXuhAoGABZzuCkObB3N51oPi3c4ZtDa+7mHT16OaOAFxW8cRXw13+vvbsmYldMwL
l5XVBmddR6DB1AdCjX4cTelG2LIhuriaXWnQCP+50gUx5uyJuM/jwwZEY1Z0X7q0
Rsra2O8G08Fl2zg/2LjZui/aaZRxMgVH8GYMdmIMY+wZNTqNkMM=
-----END RSA PRIVATE KEY-----
)KEY";