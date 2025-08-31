## Guía para IAs y LLMs

Bienvenida/o. Este repositorio pertenece a "money-home": una app para registrar gastos diarios y visualizar el gasto mensual.

Este documento indica cómo interactuar útil y seguramente con el proyecto.

### Objetivos del proyecto
- Registrar gastos de forma rápida y consistente.
- Agregar y visualizar totales mensuales y por categoría.
- Mantener el código simple, legible y fácil de probar.

### Preferencias y convenciones
- Cambios pequeños y enfocados al objetivo.
- Evitar dependencias innecesarias.
- Nombres descriptivos para funciones, variables y commits.
- Actualizar la documentación al cambiar comportamientos.

### Seguridad y privacidad
- No incluir secretos ni credenciales en el repositorio.
- Respetar datos de usuario: no generar ni subir datos sensibles.
- Antes de sugerir telemetría o analítica, consultar al humano.

### Cómo puedes ayudar
- Mejorar README y guías de uso.
- Proponer estructura de carpetas y modularización.
- Añadir validaciones y manejo de errores.
- Escribir scripts de desarrollo (linters, formateo, tests) si faltan.
- Identificar y documentar tareas abiertas como issues.

### Interacción con el repositorio
- Si desconoces el stack, inspeccionar archivos raíz (CMakeLists, src/, android/).
- Evitar cambios breaking sin acordarlo primero.
- Si creas nuevos archivos, explicar brevemente su propósito en el PR.

### Multiplataforma (Windows/Android/iOS)
- GUI: Qt Widgets + Qt Charts, empaquetado Android vía `android/` y `QT_ANDROID_PACKAGE_SOURCE_DIR`.
- Rutas de datos: usar `QStandardPaths::AppDataLocation` para almacenar CSV en escritorio y móvil.
- Android/iOS: evitar rutas relativas en disco; no requerir permisos de almacenamiento externos.

### Estilo de contribución sugerido
- Describir el problema, la solución y alternativas consideradas.
- Incluir pasos de verificación manual o pruebas automatizadas si existen.

### Roadmap sugerido (orientativo)
- Registro de gastos: CRUD básico y validaciones.
- Categorización de gastos y filtros.
- Resúmenes mensuales con gráficos simples.
- Exportación/Importación (CSV/JSON), opcional.

### Contacto humano
Si necesitas aclaraciones, pregunta con un breve resumen de lo analizado y tus supuestos.

Gracias por colaborar con money-home.

