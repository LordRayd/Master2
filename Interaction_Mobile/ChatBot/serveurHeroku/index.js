const express = require('express');
const {WebhookClient} = require("dialogflow-fulfillment");
const {welcome, defaultFallback} = require("./intents/welcome");
const {accompagner} = require('./intents/accompagner');
const {acheter} = require('./intents/acheter');

const app = express();
const port = process.env.PORT || 8080;

app.post("/dialogflow", express.json(), (req, res) => {
    const agent = new WebhookClient({request: req, response: res});
    let intentMap = new Map();
    intentMap.set("Default Welcome Intent", welcome);
    intentMap.set("Default Fallback Intent", defaultFallback);
    intentMap.set("Accompagner", accompagner);
    intentMap.set("Acheter", acheter);
    agent.handleRequest(intentMap);
});

app.get('*', (req, res, next) => {
	res.status(200).send('La page que vous rechercher n\'est pas réferencé');
	next();
});

app.listen(port, function () {
    console.log('Serveur lancer sur le port : ', port);
});