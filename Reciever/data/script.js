// Vytvoření mapy
let lon = document.getElementById('lon').innerText; //získá souřadnice, které jsou již na stránce vypsané
let lat = document.getElementById('lat').innerText; //získá souřadnice, které jsou již na stránce vypsané
let map = L.map('map').setView([lat, lon], 12); //nastaví pohled mapy na zadané souřadnice
L.tileLayer('https://api.mapbox.com/styles/v1/{id}/tiles/{z}/{x}/{y}?access_token=pk.eyJ1IjoibWNrdWt1cmljZWN6IiwiYSI6ImNreGJ3NDR1ejBwdjEyeHBnMmthamhsajgifQ.GAvd9JUztKQTiwEl5V2JIA', {
  attribution: 'Map data &copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors, Imagery © <a href="https://www.mapbox.com/">Mapbox</a>',
  maxZoom: 18, //nastaví zoom na 18
  id: 'mapbox/streets-v11',
  tileSize: 512, //velikost dlaždice
  zoomOffset: -1,
}).addTo(map); //přidá na mapu
let marker = L.marker([lat, lon]).addTo(map); // vytvoří nový ukazatek podle získaných informací

// Update informací

function updateDiv() { // funkce pro obnovování textu a mapy
  //mapa
  lon = document.getElementById('lon').innerText; //získá souřadnice, které jsou již na stránce vypsané
  lat = document.getElementById('lat').innerText; //získá souřadnice, které jsou již na stránce vypsané
  map.removeLayer(marker); //odstraní ukazatel na mapě
  marker = L.marker([lat, lon]).addTo(map);// vytvoří nový ukazatek podle získaných informací
  //text
  $("#vypisovani").load(window.location.href + " #vypisovani"); //update textu s id vypisovani
}
setInterval(updateDiv(), 1000); //funkce se spustí každých 1000 milisekund
