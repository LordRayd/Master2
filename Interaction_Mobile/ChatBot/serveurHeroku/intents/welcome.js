function welcome(agent) {
    agent.add('Message accueil');
}

function defaultFallback(agent) {
    agent.add('Je suis incapable de comprendre');
}

module.exports = {
    welcome: welcome,
    defaultFallback: defaultFallback
};