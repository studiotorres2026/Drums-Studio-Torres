DRUMS STUDIO TORRES — PROTOTIPO DE DESARROLLO 0.1

Este paquete es el código fuente inicial de un plugin JUCE para Windows.
Todavía NO es un VST3 compilado ni una versión lista para producción.

Incluye:
- Proyecto CMake que descarga JUCE 8.0.6 durante la configuración.
- Instrumento VST3 y Standalone.
- Diseño inicial con 16 pads.
- Arquitectura de 16 buses estéreo independientes + bus Main.
- Lectura de archivos WAV en el motor.
- Disparo MIDI: notas 36 a 51 (C2 a D#3, según numeración del DAW).

Pendiente antes de usarlo en sesiones:
- Completar controles gráficos de carga por pad y mixer.
- Añadir guardado/carga de kits y estado del plugin.
- Revisar sincronización y seguridad de carga de samples en tiempo real.
- Compilar y probar todas las salidas en FL Studio.
- Añadir manejo de polifonía, choke groups, medidores y efectos.

Requisitos para compilar:
- Windows 10 de 64 bits
- Visual Studio 2022 con Desktop development with C++
- CMake
- Conexión a Internet en la primera configuración (para descargar JUCE)

La compilación debe realizarse en modo Release y generar VST3.
No copies el plugin a la carpeta de FL Studio hasta completar las pruebas.
