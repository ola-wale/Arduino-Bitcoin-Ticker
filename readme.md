<h1>Arduino Bitcoin Ticker</h1>
<h5>MicroController's Class Project </h5>
 Arduino bitcoin ticker - get's bitcoin's price from various exchanges and outputs it to the 16x2 display shield.
 Author/s:
 Wale Morenigbade 
 <br>
 <b>How it works.</b>
 <br>
 <b>Modes/Exchanges/Pairs:</b>
 <br>
 There are five 'modes' <br>
 Mode 0 - Pulls the price from Kraken.com on the BTC/EUR Pair : https://api.kraken.com/0/public/Ticker?pair=XXBTZEUR <br>
 Mode 1 - Pulls the price from Kraken.com on the BTC/USD Pair : https://api.kraken.com/0/public/Ticker?pair=XXBTZUSD  <br>
 Mode 2 - Pulls the price from Bitstamp.com on the BTC/EUR Pair : https://www.bitstamp.net/api/v2/ticker/btceur/  <br>
 Mode 3 - Pulls the price from Bitstamp.com on the BTC/USD Pair : https://www.bitstamp.net/api/v2/ticker/btcusd/  <br>
 Mode 4 - Pulls the price from Bitbay.net on the BTC/PLN Pair : https://bitbay.net/API/Public/BTCPLN/ticker.json  <br>
 <br>
 I'm using my server as a conduit to poll these apis : It simplifies the data into a simple array of [2] elements: Pros: Less RAM usage on the arduino.
  <br>
 I've attached the .php file running on my server along with this file.