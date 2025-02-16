# kxkm_Ve.direct

## Description
Ce projet utilise M5 Stack pour écrire les champs Victron Energy MPPT dans InfluxDB sur le Cloud en utilisant la bibliothèque VeDirectFrameHandler. Il a été testé sur une installation solaire/plomb autonome et monitorée à distance.

## Auteur
Clément Saillant

## Licence
GNU General Public License

## Installation
1. Cloner le dépôt GitHub.
   ```bash
   git clone <URL_DU_DEPOT>
   ```
2. Ouvrir le projet avec PlatformIO.

## Configuration
Modifier le fichier `platformio.ini` pour configurer les dépendances et les paramètres de construction.

## Utilisation
1. Configurer les paramètres WiFi et InfluxDB dans le fichier `src/main.cpp`.
2. Compiler et téléverser le code sur l'appareil M5 Stack.
3. Surveiller les données envoyées à InfluxDB.

## Dépendances
- [EmonLib-esp32](https://github.com/Savjee/EmonLib-esp32.git)
- [VeDirectFrameHandler](https://github.com/MartinVerges/VeDirectFrameHandler.git)
- [InfluxDB-Client-for-Arduino](https://github.com/tobiasschuerg/InfluxDB-Client-for-Arduino.git)
- [M5Stack](https://github.com/m5stack/M5Stack)

## Remerciements
Komplex Kapharnaum pour le soutien et les ressources fournies pour ce projet.
