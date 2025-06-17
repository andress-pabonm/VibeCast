# 🎧 VibeCast - Reproductor de Música Estilo Spotify

**Universidad de Cuenca**  
**Materia:** Programación II  
**Proyecto:** Software Interactivo de Reproducción de Música  
**Lenguaje:** C  
**Instructora:** Ing. Elina Ávila  
**Fecha de Entrega:** 18 de julio de 2025  
**Repositorio GitHub:** [https://github.com/andress-pabonm/VibeCast](https://github.com/andress-pabonm/VibeCast)

---

## 👥 Equipo de Desarrollo - *VibeCast Team*

- Bryan Ávila  
- Jostin Aucancela  
- Pablo Condo  
- Mario Guamán  
- Juan Mayancela  
- Thomas Molina  
- Alex Muñoz  
- Andrés Pabón  
- Josué Peñaloza  
- Michael Revelo  
- Paula Sacoto  
- Kirman Sánchez  
- Paul Solórzano  
- Rocio Vicuña  

---

## 📌 Descripción

**VibeCast** es un software desarrollado en lenguaje C que simula funcionalidades avanzadas de plataformas de streaming musical como Spotify. Esta aplicación permite la gestión dinámica de usuarios, reproducción de música con anuncios, generación de estadísticas de uso y recomendaciones personalizadas, todo ello utilizando estructuras de datos dinámicas implementadas manualmente.

---

## 🧠 Funcionalidades Principales

- 🧍‍♂️ Gestión de Usuarios (CRUD)
- 🎶 Reproducción de Música con y sin Anuncios
- 📊 Generación de Estadísticas de Uso
- 🤖 Recomendaciones Personalizadas basadas en amigos
- 🔐 Manejo de Planes Free y Premium
- 🧾 Reportes Generados en Archivos `.txt`

---

## 🧱 Estructuras de Datos Utilizadas

| Estructura | Uso |
|-----------|-----|
| Árboles Binarios de Búsqueda (ABB) | Gestión de usuarios y artistas |
| Listas Ligadas | Playlists, discos, amigos, canciones |
| Colas | Gestión y reproducción de anuncios |
| Pilas | Historial de reproducción |

---

## 🛠️ Detalles de Implementación

### 👤 Usuarios
- Datos: correo, nombre, país, nickname, contraseña, plan (`free` / `premium`)
- Guardados en un **ABB**
- Tienen historial de reproducción (implementado como **pila**)
- Pueden tener múltiples **playlists** personales (**listas ligadas**)
- Cada usuario puede tener amigos (**lista ligada**)

### 🎵 Canciones
- Datos: nombre, duración, artista, archivo origen (mp3, URL, etc.)
- CRUD completo (*Solo se puede eliminar si no pertenece a un playlist*)
- Asignadas a discos y artistas

### 👩‍🎤 Artistas y Discos
- Un artista tiene nombre y una lista de discos (**lista ligada**)
- Un disco tiene nombre, fecha de lanzamiento y canciones (**lista ligada**)
- Un artista debe tener al menos 1 disco, y un disco al menos 1 canción

### 📃 Reproducción y Anuncios
- Cada 3 canciones, se reproduce un **anuncio** (solo para usuarios `free`)
- Los anuncios son gestionados con una **cola**
- Datos del anuncio: contenido (texto, mp3, etc.) y anunciante

### 💳 Plan Premium
- Los usuarios pueden **comprar o renovar** el plan premium
- Se registra validez y valor del plan

### 🤝 Red Social
- Los usuarios pueden agregar amigos (lista ligada)
- Se pueden recomendar canciones escuchadas por sus amigos

---

## 📈 Estadísticas Generadas (.txt)

- 🏆 Top 5 Canciones Más Escuchadas  
- 🎤 Artistas Preferidos por Usuario  
- ⏱️ Tiempo Total de Reproducción  
- 📢 Número de Anuncios Escuchados (usuarios `free`)

---

## 📦 Entregables

- ✅ Código Fuente del Proyecto
- ✅ Diagrama de Estructuras de Datos
- ✅ Archivos de Prueba (`.txt`) con usuarios, canciones y anuncios de ejemplo
- ✅ Reportes Generados Automáticamente

---

## 🧪 Evaluación

| Criterio | Peso |
|---------|------|
| Diseño de Estructuras de Datos | 25% |
| Implementación de Estructuras de Datos | 25% |
| Interfaz de Usuario | 15% |
| Documentación | 5% |
| Sustentación | 30% |

---

## 🎯 Reto de Equipos

- Presentación de la solución en **30 minutos**
- Exposición por **2 o 3 integrantes**
- Cualquier integrante puede ser consultado por la docente
- Evaluación interna entre compañeros
- **La mejor solución gana 10 puntos extra para el examen final**

---

## 🔗 Repositorio

Puedes acceder al código fuente completo en GitHub:  
👉 [https://github.com/andress-pabonm/VibeCast](https://github.com/andress-pabonm/VibeCast)

---

## 🚀 Recomendaciones

Antes de codificar:
- Diseñe estructuras sólidas y bien definidas
- Planifique la interacción entre módulos
- Asegure la correcta validación de entradas, especialmente en fechas

---

¡Gracias por revisar este proyecto!  
**Desarrollado con dedicación por el *VibeCast Team*, Universidad de Cuenca.**
