<?php

require("vendor/autoload.php");

use GuzzleHttp\Client;

//header("Content-type:application/json");

$output = array();

$client = new GuzzleHttp\Client();

$mode = $_GET['mode'];

if(!isset($_GET['mode'])){
	die("?mode query??");
}

try{
	switch ($mode) {
		case '0':
			$response = $client->get('https://api.kraken.com/0/public/Ticker?pair=XXBTZEUR');
			$body = $response->getBody();
			$body = json_decode($body);
			$output['price'] = "€".$body->result->XXBTZEUR->a[0];
			$output['pair'] = "Kraken BTC/EUR";
		break;
		case '1':
			$response = $client->get('https://api.kraken.com/0/public/Ticker?pair=XXBTZUSD');
			$body = $response->getBody();
			$body = json_decode($body);
			$output['price'] = "$".$body->result->XXBTZUSD->a[0];
			$output['pair'] = "Kraken BTC/USD";
		break;
		case '2':
			$response = $client->get('https://www.bitstamp.net/api/v2/ticker/btceur/');
			$body = $response->getBody();
			$body = json_decode($body);
			$output['price'] = "€".$body->last;
			$output['pair'] = "Bitstamp BTC/EUR";
		break;
		case '3':
			$response = $client->get('https://www.bitstamp.net/api/v2/ticker/btcusd/');
			$body = $response->getBody();
			$body = json_decode($body);
			$output['price'] = "$".$body->last;
			$output['pair'] = "Bitstamp BTC/USD";
		break;
		case '4':
			$response = $client->get('https://bitbay.net/API/Public/BTCPLN/ticker.json');
			$body = $response->getBody();
			$body = json_decode($body);
			$output['price'] = "zł".$body->last;
			$output['pair'] = "Bitbay BTC/PLN";
		break;
	}
} 
catch(Exception $e){
	die($e->getMessage());
}


echo "~";
echo json_encode($output,JSON_UNESCAPED_UNICODE );
die();
