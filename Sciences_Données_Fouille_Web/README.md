# Sciences des données et fouille du web
## Enseignant
Pierre-François Marteau
## Setup
Utilisé le plugin *ElasticSearch Head* sur google chrome pour avoir une vue des index
## Proxy et Pip
A la fac il faut modifier le proxy pour que les pip marchent
### Temporaire
Dans un terminale taper : 
```
export https_proxy=https://squidva.univ-ubs.fr:3128/
export http_proxy=http://squidva.univ-ubs.fr:3128/
```
### Permanent
Mettre a la fin du *~/.bashrc* les lignes : 
```
export https_proxy=https://squidva.univ-ubs.fr:3128/
export http_proxy=http://squidva.univ-ubs.fr:3128/
```
## Controle
## Exercice Lyme
```
P(A | B) = P(B) * P(B | A) / P(B)
P(Lyme) = 31 / 100 000 = 4.3^10-4
P( non Lyme) = 1 - P(Lyme) = 0.99957
P(+ | Lyme) = 0.98
P(- | Lyme) = 0.02
P(+ | non Lyme) = 0.03
P(- | non Lyme) = 0.97
P(+) = P(+ | Lyme) * P(Lyme) + P(+ | non Lyme) * P( non Lyme)
P(-) = P(- | Lyme) * P(Lyme) + P(- | non Lyme) * P( non Lyme)
P(Lyme | +) = 0.98 * 4.3^10-4 / (0.98 * 4.3^10-4 + 0.03) = 0.98 * 4.3^10-4 / 0.03  ~= 0.014 
```
