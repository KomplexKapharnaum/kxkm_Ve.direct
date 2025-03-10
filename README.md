# kxkm_Ve.direct

## Description
Ce projet permet de lire les données des champs Victron Energy MPPT et de les envoyer vers InfluxDB sur le Cloud. Il est basé sur M5 Stack et utilise la bibliothèque VeDirectFrameHandler.

## Fonctionnalités
- Connexion WiFi et maintien de la connexion
- Connexion à AWS et maintien de la connexion
- Connexion à Home Assistant et maintien de la connexion
- Connexion à InfluxDB et maintien de la connexion
- Mise à jour de l'affichage
- Mesure de la consommation électrique
- Synchronisation de l'heure avec un serveur NTP
- Mise à jour de la force du signal WiFi

## Prérequis
- M5 Stack
- Bibliothèque VeDirectFrameHandler
- Compte InfluxDB
- Compte AWS (si utilisé)
- Home Assistant (si utilisé)

## Installation
1. Clonez le dépôt :
    ```sh
    git clone https://github.com/mk4001/VE.Direct2InfluxDB-2.0.git
    ```
2. Ouvrez le projet dans votre IDE Arduino ou PlatformIO.
3. Installez les bibliothèques nécessaires :
    - Arduino
    - M5Stack
    - EmonLib
    - VeDirectFrameHandler
    - WiFi

## Configuration
Modifiez le fichier `config/config.h` pour configurer les paramètres de votre réseau WiFi, AWS, Home Assistant et InfluxDB.

## Utilisation
1. Téléversez le code sur votre M5 Stack.
2. Ouvrez le moniteur série pour vérifier les connexions et les mesures.

## Auteur
Clément Saillant

## Licence
Ce projet est sous licence GNU General Public License v3.0. Voir le fichier [LICENSE](LICENSE) pour plus de détails.

## Références
- [VE.Direct2InfluxDB-2.0](https://github.com/mk4001/VE.Direct2InfluxDB-2.0)
