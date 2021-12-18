# INFO
- PlatformIO + VisualStudio Code
- Finální projekt v adresáři "Final V1"
- Projekt ještě není dokončený

# Ročníkový projekt (Sledovací obojek pro psy) - část 1. Návrh projektu

I. Cíl projektu
----------------------------
### Hlavní informace
- Vyvoření GPS sledovače (převážně pro psy) + jeho lokátor 
- Velikost sledovače by měla být maximálně velikosti mobilního telefonu (aby bylo možné jej umístit na jeho obojek, případně někde do kapsy)
- Skládat by se měl ze dvou částí (sledovač, lokátor)
- Sledovač - bude napájen baterií, krabička o menší velikosti, vysílá signál z místa na kterém se nachází
- Lokátor - na displeji se bude zobrazovat: směr k pozici, vzádlenost, síla baterie, síla signálu (old)
- Lokátor - bude dostávat signál pomocí LoRa zařízení, které pak bude zobrazovat na webovém serveru
- esp32 LORA
### Problémy
- Znovu se naučit jazyk na programování mikrokontrolerů (wiring)
- Zjistit jaké technologie použít
- Zjistit a nakoupit hardware (porovnat ceny Českých a zahraničních obchodů)
- Vytvořit 3d model krabičky pro celý hardware (Late game) >> poté vytisknu na 3d tiskárně (Ender 3 V2)
- LoRa Reciever nepřijímá Packety odeslané LoRa Senderem (vyřešeno)
- Zařízení komunikují, pokud se jejich antény dotýkají jinak ne (vyřešeno, špatná frekvence)
- GPS nechce přijímat data, problém může být v tom, že tam chybí anténa (do teď nevím co je za problém)
- Anténa nainstalována, gps stále předává divné informace (změnil jsem celou gps)
- Doma nechytám dobrý gps signál (s tím nic moc nenadělám :D, nastavil sem že pokud nechytá signál, pošle předem dannou proměnnou) - opraveno
- GPS má hroznou odchylku (1-10km) (opraveno - gps neodesilalo cele souradnice, jenom část)
- Spiffs webserver nechce fungovat, funguje pouze samostatný 
- Sender nefunguje, když je zapnutá jak GPS tak OLED display (napadá mě buď nedostatečný proud, nebo překrývání portů - budu to ignorovat, display není potřeba)
- Problém s připojováním wifi, po opravě wifi vytváří access point (ostatní zařízení se mohou připojit), po zadání lokální IP se uživatel může připojit na webovou stránku
- Mapa nebude fungovat, pokud nebude Esp připojeno k internetu + mapa potřebuje nějaké ID 
- Možnost získat polohu z prohlížeče na vytvoření kompasu na stránce
- Číst sílu signálu od LoRy a převést výstup na (dobrý, normální, špatný signál...)
- Převedení formátu času z 9:15:4 na 09:15:04 kvůli parsování, to stejné s datem
- Automatický update webu, nefungoval JQUERY (vyřešeno, špatná cesta k souboru)

II. Informace získané ve škole
-------------------------------------------
### Informace k projektu
- Zdroje (KONKURENCE, zdroje co pouzivame) ✔
- Problémy se kterymi sme se setkali ✔
- 14 stránek
- Použití (ki cad) ✔
- projekt nemusí být na 100% funkční, musí být vidět postup
- jak debugovat esp
- framework ✔
- SPIFFS ✔
- OTA update
- nepoužívat delay ✔
### Hodnocení projektu
- projekt samotný - __40%__
- dokumentace - __20%__ (všechny informace o projektu, problémy, časové rozvržení, časová náročnost..)
- prezentace - __20%__ ()
- průběžnost - __20%__ (jak se na projektu pracuje postupně)

III. Konkurence
-------------------------------------------
- Eshop [Elektro obojky](https://www.elektro-obojky.cz/gps-obojky-pro-psy/)
### Garmin
* [Garmin](https://www.elektro-obojky.cz/gps-obojky-garmin/)
* 6-14.5km
* 9000 - 29000 kč
* dotykový display
* GPS + GLONASS a VHF
* spustitelný alarm + sdílení polohy
* sledování ostatních obojků
* nastavení povoleného rádiusu
* 3D kompas
* upozornění na štěkot psa
* možnost použití vibrace / tónu / impulzu
* 212g
* režim spánku
### Tracker
* [Tracker](https://www.elektro-obojky.cz/gps-obojky-pro-psy-tracker/)
* neomezený dosah
* 15000 - 25000 kč
* Využití 3G a 2G
* sledování ostatních obojků
* přes mobil
### Dogtrace
* [Dogtrace](https://www.elektro-obojky.cz/gps-obojky-dogtrace/)
* 10000 - 20000 kč
* až 20km
* zvukový lokátor
* možných až 13 obojků
* plně vodotěsný
* 40+ hodin výdrž
* kompas
* ukládání bodů na mapě

IV. Předpokládaný Harmonogram
-------------------------------------------
### Září
* Sepsat nutné zdroje ✔️
* Objednat hardware ✔️
* Zjistit všechny věci co budou potřeba ✔️

### První polovina Říjnu
* Seznámit se s technologiemi ✔️
* Rozpracovat hardware ✔️

### Druhá polovina Říjnu
* Začít psát kód ✔️
* Udělat jednotlivé části kódu ✔️
* Test zákldních funkcí ✔️

### První polovina Listopadu
* Pomalu dokončovat Hardware kód - sestavit kód ✔️
* Fungovaní webu ✔️
* Fungování GPS a LoRa ✔️

### Druhá polovina Listopadu
* Finální úpravy ✔️
* Grafická úprava stránky ❌
* Udělat návrh obalu ❌

### Prosinec (Termín 30.12.2021)
* Sepsat dokumentaci ❌
* Sestavit hardware ❌
* Debugování a chyby ❌
* Vytisknout obal na 3d tiskárně ❌
* Zbytek času na to, pokud něco nebudu stíhat 

IV. Konečný Harmonogram 
-------------------------------------------
* Červen 2021 - udělání Githubu, Získané informace o konkurenci, Cíl projektu
* 15.09.2021 - sepsání hrubého Harmonogramu
* 30.09.2021 - spuštění IDE + první test fungování esp
* 06.10.2021 - Změna IDE + snaha o instalaci SPIFFS
* 12.10.2021 - seznamování se SPIFFS
* 13.10.2021 - SPIFFS funguje, webový server funguje
* 14.10.2021 - úprava kódu, editace webu
* 15.10.2021 - seznamování se s GPS modulem
* 28.10.2021 - zprovoznění LoRa zařízení
* 29.10.2021 - Snaha o komunikaci mezi zařízeními
* 31.10.2021 - Zařízení mezi sebou komunikují 
* 02.11.2021 - Vyřešena komunikace dvou žařízení pomocí LoRa (kominikace na +- 500-800m)
* 03.11.2021 - úprava README.md -> především grafické úpravy
* 04.11.2021 - pokus dostat GPS signál
* 12.11.2021 - změna gps modulu > funguje, dostávám informace z gps, ale nejsou tak přesné jak bych předpokládal
* 15.11.2021 - gps sender by měl být skoro hotový, problém s gps odchylkou
* 16.11.2021 - skládání recieveru + zprovozňování webového serveru
* 18.11.2021 - Zprovoznění GPS + komunikace mezi LoRa zařízeními, drobné úpravy kódu
* 20.11.2021 - Komunikace funguje, webserver funguje a dostává informace
* 26.11.2021 - Práce na Stránce + zobrazení mapy 
* 29.11.2021 - Úprava Wifi kódu + dokončení sender kódu + odstranění delay() v loopu
* 30.11.2021 - úprava Githubu, sepsání problémů + řešení
* 03.12.2021 - úprava komunikace mezi ESP32 zařízeními
* 12.12.2021 - automatické obnovování textu na webu pomocí jquery
* 14.12.2020 - vyřešení maximální velikosti SPIFFS, nahrání map na ESP

# Ročníkový projekt (Sledovací obojek pro psy) - část 2. Sestavování

I. Zporvoznění Arduino IDE pro ESP32 (později PlatformIO)
--------------------------------------
* 30.09.2021 - Zprovoznění IDE, pro práci s esp32 a první test funkčnosti
* Spuštění programu na vyhledání sítě, úspěšně splněno
* Vyřešený problém - systémový monitor vypisoval corruptované výsledky, musel sem změnit počet baudů
* Dále test displaye
* Více informací v adresáři "ESPtest1"

II. Instalace PlatformioIDE + SPIFFS
--------------------------------------
* Změna platformy z Arduino IDE na Platformio IDE ve Visual Studio Code
* 06.10.2021 - Instalace Platformio IDE do Visual Studio Code (pro práci s ESP32)
* Zkouška instalace SPIFFS (Problém s instalací)
* 12.10.2021 - Snaha o spuštění webového serveru na esp32
* 13.10.2021 - webserver funguje, vypis na obrazovku
<div class="row">
  <img src="/images/spiff_webserver/IMG_20211012_222443.jpg" alt="LoRa_vypis" width="300"/>
  <img src="/images/spiff_webserver/IMG_20211012_222452.jpg" alt="LoRa_vypis" width="300"/>
  <img src="/images/spiff_webserver/site.JPG" alt="Stranka" width="375"/>
</div>

III. Komunikace mezi dvěmi LoRa zařízeními
------------------------------------------
* Zařízení komunikují pomocí jednoduchých zpráv
* Problém s tím, že nefunguje využití antény, zařízení komunikují jen, když se antény dotýkají
* problém vyřešen, frekvence zařízení byla jiná, než je daná pro Evropu (finální frekvence nyní 434MHz)
* Test zařízení: potvrení komunikace na zhruba 500-800m
* použitá knihovna "heltec.h" -> kvůli Esp od firmy heltec
<div class="row">
  <img src="/images/lora/IMG_20211102_220334.jpg" alt="LoRa_Sender" width="350"/>
  <img src="/images/lora/IMG_20211102_220342.jpg" alt="LoRa_Reciever" width="350"/>
</div>

IV. Zprovoznění GPS + posílání mezi LoRa zařízeními
---------------------------------------------------
* Zprovoznění GPS zařízení pomocí Adafruit_GPS knihovny
* Zařízení dostává všechny informace, co potřebuju
* Zařízení úspěšně sdílí gps souřadnice s druhým zařízením
<div class="row">
  <img src="/images/gps/gps2.jpg" alt="GPS_output" width="350"/>
</div>
* změna knihovny na TinyGps++.h > lepší funkce, jednoduchost
* GPS úspěsně vypisuje celé sořadnice a aktualizuje se při jakémkoliv pohybu
* LoRa posílá informace mezi sebou a ukazuje je na webserveru

V. Výpis informací na Webový server
------------------------------------
* Informace z gps se rozparsují a posílají na webový server
* Server se zeptá na informaci a esp mu ji pošle
* Stránka se každých 10s refresne (chtělo by refreshovat pouze určitou část)
* edit: nyní se refreshuje pouze vybraný text, časový limit je 3 sekundy

VI. Přístup uživatele
---------------------
* Esp vytvoří vlastní wifi 
* Uživatel se může připojit, zadat do webserveru ip adresu a připojit se na webovou stránku
* IP adresa, heslo a ssid je vypsáno na Espčku, aby se uživatel mohl připojit

# Ročníkový projekt (Sledovací obojek pro psy) - část 3. Finální úpravy a debugování

I. Využití informací z LoRa zařízení
------------------------------------
* Snaha o zobrazení pozice na mapě 
* Převedení RSSI na srozumitelnou informaci (perfektní, dobrý, špatný signál...)
* Vytvoření kompasu ? pomocí informací z webového prohlížeče
* Vypisování času na webu z GPS 
* 

II. Dosah LoRa zařízení + testy komunikace
------------------------------------------
* první test dosál vzdálenosti asi 1,5km (terén docela nepřístupný)

Odkazy a použíté technologie:
-----------------------------
* Markdown - [wikipedie](https://cs.wikipedia.org/wiki/Markdown)
* Micropython - [Micropython](https://randomnerdtutorials.com/micropython-esp32-esp8266-vs-code-pymakr/), [Instalace](https://blog.iservery.com/2020/04/06/instalace-micropython-a-vscode/)
* esp32 debugger - [Debugger](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/jtag-debugging/using-debugger.html), [Github](https://github.com/maakbaas/esp8266-iot-framework)
* esp32 LORA - https://www.ebay.com/itm/173624285281 + gps modul https://www.ebay.com/itm/174930619300
* NMEA - https://en.wikipedia.org/wiki/NMEA_2000
* spiffs - https://randomnerdtutorials.com/esp32-vs-code-platformio-spiffs/
* spiffs webserver - https://randomnerdtutorials.com/esp32-web-server-spiffs-spi-flash-file-system/
* LoRa komunikace - https://github.com/HelTecAutomation/Heltec_ESP32/tree/master/examples/LoRa
* LoRa komunikace 2 - https://randomnerdtutorials.com/esp32-lora-rfm95-transceiver-arduino-ide/
* LoRa prikazy - https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md
* Heltec knihovna-priklady - https://github.com/HelTecAutomation/Heltec_ESP32/tree/master/examples/LoRa
* NMEA - https://github.com/stevemarple/MicroNMEA
* GPS data - https://ww2.mathworks.cn/help/supportpkg/arduino/ref/read-serial-data-from-a-gps-shield-using-arduino-hardware.html //nakonec použitý jiný program
* GPS - https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series/issues/116 //nakonec použitý jiný program
* GPS2 - https://learn.adafruit.com/adafruit-ultimate-gps/arduino-wiring //nakonec použitý jiný program
* GPS3 - https://randomnerdtutorials.com/guide-to-neo-6m-gps-module-with-arduino/ //nakonec použitý jiný program
* parsovani dat GPS - https://learn.adafruit.com/adafruit-ultimate-gps/parsed-data-output
* náhodné YT video - https://www.davidjwatts.com/youtube/GPS-software-serial.ino
* Tiny GPS kód - https://github.com/DzikuVx/esp32_gps_thingy/blob/master/gps_logger.ino //naprosty hrdina tento člověk !!!! ❤️❤️❤️
* rtos vtasks - https://www.youtube.com/watch?v=95yUbClyf3E
* password generator - https://www.codespeedy.com/generate-a-random-password-of-a-specific-length-in-cpp/
* arduino wifi - https://randomnerdtutorials.com/esp32-access-point-ap-web-server/
* esp delay nahrada - https://www.arduino.cc/en/Tutorial/BuiltInExamples/BlinkWithoutDelay
* nastaveni wifi - https://gist.github.com/futechiot/ee0223dd269cbe7d8605ce97d120d7d2
* ESP rest api endpoints - https://www.mischianti.org/2020/05/16/how-to-create-a-rest-server-on-esp8266-and-esp32-startup-part-1/
* Google Maps - https://developers.google.com/maps/documentation/javascript/adding-a-google-map#maps_add_map-html
* jquery help - https://javarevisited.blogspot.com/2016/04/3-ways-to-solve-jquery-uncaught-reference-error-is-not-defined.html
* jquery update - https://stackoverflow.com/questions/33801650/how-do-i-refresh-a-div-content
* rest api - https://github.com/superucitelka/SmartWatch-REST
* partitions - https://github.com/espressif/arduino-esp32/tree/master/tools/partitions
* platformio, partitions - https://docs.platformio.org/en/latest/platforms/espressif32.html#partition-tables





* jen si to tady odlozim https://blog.iservery.com/wp-content/uploads/2014/01/Maturitn%C3%AD_ot%C3%A1zky_TVY_n%C3%A1vrh-.pdf
