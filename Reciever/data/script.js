let lon = document.getElementById('lon').innerText;
let lat = document.getElementById('lat').innerText;
var map = L.map('map').setView([lat, lon], 12);
L.tileLayer('https://api.mapbox.com/styles/v1/{id}/tiles/{z}/{x}/{y}?access_token=pk.eyJ1IjoibWNrdWt1cmljZWN6IiwiYSI6ImNreGJ3NDR1ejBwdjEyeHBnMmthamhsajgifQ.GAvd9JUztKQTiwEl5V2JIA', {
  attribution: 'Map data &copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors, Imagery © <a href="https://www.mapbox.com/">Mapbox</a>',
  maxZoom: 18,
  id: 'mapbox/streets-v11',
  tileSize: 512,
  zoomOffset: -1,
}).addTo(map);
var marker = L.marker([lat, lon]).addTo(map);
////////////////////////////////////////////////;///////////////
setInterval(function updateDiv() {
  $("#vypisovani").load(window.location.href + " #vypisovani");
}, 1000);
setInterval(function updateMap() {
  lon = document.getElementById('lon').innerText;
  lat = document.getElementById('lat').innerText;
  map.removeLayer(marker);
  marker = L.marker([lat, lon]).addTo(map);
}, 1000);