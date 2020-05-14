#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String responseHTML = ""
                      "<!DOCTYPE html><html><head><title>Portalit</title>"
                      "<script>"
                      "function verInfo(){document.getElementById('contraseña').style.display='none';document.getElementById('info').style.display='block';document.getElementById('usos').style.display='none'};"
                      "function verUsos(){document.getElementById('contraseña').style.display='none';document.getElementById('info').style.display='none';document.getElementById('usos').style.display='block'};"
                      "function verRed(){document.getElementById('contraseña').style.display='block';document.getElementById('info').style.display='none';document.getElementById('usos').style.display='none'};"

                      "</script>"
                      "</head><body>"
                      "<img class='imagen' alt='' src='data:image/png;base64,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' />"
                      "<h1>Portal cautivo móvil</h1>"
                      "<p class='parrafo'>Proyecto pensado para emprendedores, comercios, eventos, locaciones con poco acceso a internet, educación, activismo y hacking ético.</p>"

                      "<button onclick='verInfo()'>¿Qué es este portal?</button>"
                      "<button onclick='verUsos()'>Usos y limitaciones</button>" 
                      "<button onclick='verRed()'>Red y Contraseña</button>"
                      
                      "<div id='info' style='display:none;'>"
                      "<h1 style=color:rgba(0,79,128,1);>"
                      "Usted se encuentra en un portal cautivo móvil programado en un ESP8266, este es un pequeño servidor capaz de almacenar y distrubir información."
                      " En este prototipo podemos observar como se pueden cargar imágenes en formato BASE64, otorgar estilos a la página del portal cautivo y funcionalidaes offline en JavaScript.</h1>"                 
                      "</div>"

                      "<div id='usos' style='display:none;'>"
                      "<h1 style=color:rgba(0,79,128,1);>"
                      "Los usos que se le pueden otorgar al portal cautivo móvil son varios, pasando por todas las variables mencionadas en el párrafo superior de la página, específicamente"
                      " podemos utilizarlo para posicionar nuestra marca, anunciar un evento in situ, redireccionar usuarios a otra red (oculta o no), comunicar de manera pasiva mensajes a la comunidad de manera anónima o no. </h1>"                 
                      "</div>"
                      "</div>"

                      "<div id='contraseña' style='display:none;'>"
                      "<h1>Red:<h1>"
                      "<h1 style=color:rgba(0,79,128,1);>PortalitWiFi<h1>"
                      "<h1>Contraseña:<h1>"
                      "<h1 style=color:rgba(0,79,128,1);>portal1234movil<h1>"
                      "</div>"
                      
                      "<style>body{background:white;}button{heigth:15vh;width:33%;font-size:2em;background-color:orange;color:white;border-radius:90px;padding:5%;}p{color:rgba(0,79,128,1);font-size:4em;margin-top:10%;text-align:center;}h1{width:100%;text-align:center;color:orange;}h2{width:100%;text-align:center;parrafo{font-weight:bold;};color:orange}h3{color:rgba(0,79,128,1);width:100%;text-align:center;}.imagen{width:50%;margin-left:25%;margin-top:10%;margin-bottom:10%;}</style>"
                      "</body></html>";
                      

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Portalit");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // replay to all requests with same HTML
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", responseHTML);
  });
  webServer.begin();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}
